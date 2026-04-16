/*
  HSE_CRENTRY — optional server-side guard for CRENTRY_CRSTT changes (CAR workflow)
  RQ_HSE_12_4_26_00_40

  Purpose (gap M1 — HIGH)
  -----------------------
  Mitigate invalid lifecycle jumps when CRENTRY_CRSTT is updated outside CAR custom buttons
  (integrations, ad-hoc SQL, or form saves that bypass client assertCrentryStatusAllows).

  Behaviour
  ---------
  1) If CRENTRY_CRSTT does not change (after trim), allow.
  2) If SESSION_CONTEXT(N'HSE_CAR_CRSTT_GUARD_OFF') = N'1', skip all checks (admin / migration).
  3) If old status is 25 (Closed) and new status is not 25, block (reopen) unless bypass above.
  4) Otherwise require (old_stt, new_stt) in whitelist table dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40.
  5) AFTER INSERT: if new row has non-empty CRENTRY_CRSTT, require (N'', new_stt) in the same whitelist (closes M1 INSERT bypass).

  Apply in dev → UAT → prod. Extend the whitelist table for ERP-specific repair paths (e.g. 02→05).
  If this conflicts with existing triggers or bulk jobs, use SESSION_CONTEXT bypass in that session:

    EXEC sys.sp_set_session_context @key = N'HSE_CAR_CRSTT_GUARD_OFF', @value = N'1';
*/

SET NOCOUNT ON;
GO

IF OBJECT_ID(N'dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40', N'U') IS NULL
BEGIN
  CREATE TABLE dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 (
    FROM_STT VARCHAR(10) NOT NULL,
    TO_STT   VARCHAR(10) NOT NULL,
    CONSTRAINT PK_HSE_CRENTRY_CRSTT_TRN_RQ124 PRIMARY KEY CLUSTERED (FROM_STT, TO_STT)
  );
END;
GO

/* Idempotent seed: web + desktop-aligned primary paths (trimmed two-char codes) */
MERGE dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 AS t
USING (VALUES
  (N'',       N'01'),
  (N'',       N'05'),
  (N'01',     N'05'),
  (N'05',     N'10'),
  (N'05',     N'02'),
  (N'10',     N'16'),
  (N'10',     N'11'),
  (N'16',     N'19'),
  (N'16',     N'12'),
  (N'19',     N'15'),
  (N'19',     N'06'),
  (N'19',     N'20'),
  (N'15',     N'25'),
  (N'15',     N'15'),
  /* RQ_HSE_12_4_26_00_40 — close CAR from Actions Review / Actions Entry without prior approve (desktop does not client-gate status on CLOSE_CAR) */
  (N'16',     N'25'),
  (N'19',     N'25')
) AS s (FROM_STT, TO_STT)
ON t.FROM_STT = s.FROM_STT AND t.TO_STT = s.TO_STT
WHEN NOT MATCHED THEN INSERT (FROM_STT, TO_STT) VALUES (s.FROM_STT, s.TO_STT);
GO

IF OBJECT_ID(N'dbo.trg_HSE_CRENTRY_CRSTT_transition_guard', N'TR') IS NOT NULL
  DROP TRIGGER dbo.trg_HSE_CRENTRY_CRSTT_transition_guard;
GO

CREATE TRIGGER dbo.trg_HSE_CRENTRY_CRSTT_transition_guard
ON dbo.HSE_CRENTRY
AFTER UPDATE
AS
BEGIN
  SET NOCOUNT ON;

  IF NOT UPDATE(CRENTRY_CRSTT)
    RETURN;

  IF TRY_CAST(SESSION_CONTEXT(N'HSE_CAR_CRSTT_GUARD_OFF') AS nvarchar(10)) = N'1'
    RETURN;

  /* No-op: trimmed status unchanged */
  IF NOT EXISTS (
    SELECT 1
    FROM inserted AS i
    INNER JOIN deleted AS d ON d.PrmKy = i.PrmKy
    WHERE LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N''))) <> LTRIM(RTRIM(ISNULL(d.CRENTRY_CRSTT, N'')))
  )
    RETURN;

  IF EXISTS (
    SELECT 1
    FROM inserted AS i
    INNER JOIN deleted AS d ON d.PrmKy = i.PrmKy
    WHERE LTRIM(RTRIM(ISNULL(d.CRENTRY_CRSTT, N''))) = N'25'
      AND LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N''))) <> N'25'
  )
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: Cannot change CAR status after close (25). Use SESSION_CONTEXT HSE_CAR_CRSTT_GUARD_OFF=1 if intentional.', 16, 1);
    ROLLBACK TRANSACTION;
    RETURN;
  END;

  IF EXISTS (
    SELECT 1
    FROM inserted AS i
    INNER JOIN deleted AS d ON d.PrmKy = i.PrmKy
    WHERE LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N''))) <> LTRIM(RTRIM(ISNULL(d.CRENTRY_CRSTT, N'')))
      AND NOT EXISTS (
        SELECT 1
        FROM dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 AS w
        WHERE w.FROM_STT = LTRIM(RTRIM(ISNULL(d.CRENTRY_CRSTT, N'')))
          AND w.TO_STT = LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N'')))
      )
  )
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: CRENTRY_CRSTT transition not allowed. Extend HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 or use bypass context.', 16, 1);
    ROLLBACK TRANSACTION;
    RETURN;
  END;
END;
GO

/* RQ_HSE_12_4_26_00_40 — INSERT guard: block ad-hoc INSERT with lifecycle status not allowed as “first” status from empty */
IF OBJECT_ID(N'dbo.trg_HSE_CRENTRY_CRSTT_insert_guard', N'TR') IS NOT NULL
  DROP TRIGGER dbo.trg_HSE_CRENTRY_CRSTT_insert_guard;
GO

CREATE TRIGGER dbo.trg_HSE_CRENTRY_CRSTT_insert_guard
ON dbo.HSE_CRENTRY
AFTER INSERT
AS
BEGIN
  SET NOCOUNT ON;

  IF TRY_CAST(SESSION_CONTEXT(N'HSE_CAR_CRSTT_GUARD_OFF') AS nvarchar(10)) = N'1'
    RETURN;

  IF EXISTS (
    SELECT 1
    FROM inserted AS i
    WHERE LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N''))) <> N''
      AND NOT EXISTS (
        SELECT 1
        FROM dbo.HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 AS w
        WHERE w.FROM_STT = N''
          AND w.TO_STT = LTRIM(RTRIM(ISNULL(i.CRENTRY_CRSTT, N'')))
      )
  )
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: CRENTRY_CRSTT initial value not allowed on INSERT. Extend HSE_CRENTRY_CRSTT_TRN_RQ_HSE_12_4_26_00_40 (FROM_STT='''' -> TO_STT) or use SESSION_CONTEXT bypass.', 16, 1);
    ROLLBACK TRANSACTION;
    RETURN;
  END;
END;
GO
