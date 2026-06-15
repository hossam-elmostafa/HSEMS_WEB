/*
  HSE_CRENTRY — block DELETE when auto-generated CAR (CRENTRY_ATGNR = Y)
  RQ_HSE_12_4_26_00_40

  Desktop: NewCarEntryCategory — DELETE blocked for auto-generated CARs.
  Web: client guard in CAR_Entry.js + this DB enforcement for API/raw SQL deletes.

  Apply in dev → UAT → prod after review. Adjust trigger name if it conflicts.
*/

SET NOCOUNT ON;
GO

IF OBJECT_ID(N'dbo.trg_HSE_CRENTRY_block_autogen_delete', N'TR') IS NOT NULL
  DROP TRIGGER dbo.trg_HSE_CRENTRY_block_autogen_delete;
GO

CREATE TRIGGER dbo.trg_HSE_CRENTRY_block_autogen_delete
ON dbo.HSE_CRENTRY
INSTEAD OF DELETE
AS
BEGIN
  SET NOCOUNT ON;
  IF EXISTS (
    SELECT 1
    FROM deleted
    WHERE UPPER(LTRIM(RTRIM(ISNULL(CRENTRY_ATGNR, N'')))) = N'Y'
  )
  BEGIN
    RAISERROR(N'RQ_HSE_12_4_26_00_40: Cannot delete auto-generated CAR (CRENTRY_ATGNR=Y).', 16, 1);
    RETURN;
  END;

  DELETE t
  FROM dbo.HSE_CRENTRY AS t
  INNER JOIN deleted AS d ON t.PrmKy = d.PrmKy;
END;
GO
