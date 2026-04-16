/*
  HSE — CAR trigger hardening: concrete ALTER TRIGGER template  (gap M3 — MEDIUM)
  RQ_HSE_12_4_26_00_40

  Problem
  -------
  Web saveRec (WebInfra) executes INSERT/UPDATE in a NEW SQL connection drawn from
  the connection pool.  The application-layer executeSQLPromise that seeds
  ##TEMP_HSE_TABLE runs in a DIFFERENT connection; the global temp is gone before
  the trigger fires.

  Triggers that reference ##TEMP_HSE_TABLE without an OBJECT_ID guard will raise:
      Msg 208 — Invalid object name '##TEMP_HSE_TABLE'

  This script provides a concrete ALTER TRIGGER template that DBA applies to each
  affected trigger on HSE_CRENTRY (and related tables) identified by
  HSE_TEMP_HSE_TABLE_trigger_hardening.sql (discovery query).

  Steps for DBA
  -------------
  1. Run discovery query in HSE_TEMP_HSE_TABLE_trigger_hardening.sql to list
     triggers that reference ##TEMP_HSE_TABLE.
  2. For each trigger, open its definition in SSMS (right-click → Script Trigger As → ALTER TO).
  3. Find the section that reads from ##TEMP_HSE_TABLE directly (e.g. "SELECT … FROM ##TEMP_HSE_TABLE WHERE …")
     and replace it with the GUARDED pattern below.
  4. Apply SESSION_CONTEXT read as SECONDARY fallback (set by web pre-save).
  5. Hardcode web-safe defaults as TERTIARY fallback.
  6. Test: see §Regression tests.

  Pattern to insert at top of trigger body (replace existing ## reads)
  ----------------------------------------------------------------------
*/

SET NOCOUNT ON;
GO

/* -----------------------------------------------------------------------
   STEP 1 — Declare variables with safe defaults (desktop-aligned fallbacks)
   ----------------------------------------------------------------------- */

/*
    DECLARE @ActionDescription NVARCHAR(200) = N'Creating new CAR';   -- safe web default
    DECLARE @SrcScreen         NVARCHAR(200) = N'CAR Entry';           -- safe web default
    DECLARE @UserID            NVARCHAR(200) = NULL;

    ---
    PRIMARY path: read from ##TEMP_HSE_TABLE when it exists in THIS session
    (desktop client and Observation close path seed the temp table in the same connection).
    ---

    IF OBJECT_ID(N'tempdb..##TEMP_HSE_TABLE') IS NOT NULL
    BEGIN
        SELECT @ActionDescription = ISNULL(VALUErec, @ActionDescription)
        FROM   ##TEMP_HSE_TABLE
        WHERE  KEYrec = N'ActionDescription';

        SELECT @SrcScreen = ISNULL(VALUErec, @SrcScreen)
        FROM   ##TEMP_HSE_TABLE
        WHERE  KEYrec = N'SrcScreen';

        SELECT @UserID = ISNULL(VALUErec, @UserID)
        FROM   ##TEMP_HSE_TABLE
        WHERE  KEYrec = N'UserID';
    END

    ---
    SECONDARY path: SESSION_CONTEXT (set by web pre-save via sp_set_session_context).
    Only overwrites when ##TEMP_HSE_TABLE was absent (pure web INSERT path).
    ---

    ELSE
    BEGIN
        DECLARE @sc_desc NVARCHAR(200) =
            TRY_CAST(SESSION_CONTEXT(N'HSE_CAR_ActionDescription') AS NVARCHAR(200));
        DECLARE @sc_scr NVARCHAR(200) =
            TRY_CAST(SESSION_CONTEXT(N'HSE_CAR_SrcScreen') AS NVARCHAR(200));
        DECLARE @sc_uid NVARCHAR(200) =
            TRY_CAST(SESSION_CONTEXT(N'HSE_CAR_UserID') AS NVARCHAR(200));

        IF @sc_desc IS NOT NULL SET @ActionDescription = @sc_desc;
        IF @sc_scr  IS NOT NULL SET @SrcScreen         = @sc_scr;
        IF @sc_uid  IS NOT NULL SET @UserID             = @sc_uid;
    END

    -- Replace all downstream uses of ##TEMP_HSE_TABLE in the trigger with @ActionDescription,
    -- @SrcScreen, @UserID variables.
*/

/* -----------------------------------------------------------------------
   STEP 2 — SESSION_CONTEXT seeding from web pre-save (application layer)
   The web pre-save (carEntrySaveDesktopParity.js) already seeds ##TEMP_HSE_TABLE.
   This script additionally shows how to set SESSION_CONTEXT as a belt-and-suspenders
   fallback so the trigger gets context even when the temp table is absent.

   DBA: no change needed here; the JS batch below is already emitted by the app.
   Shown for documentation only:

     EXEC sys.sp_set_session_context
          @key   = N'HSE_CAR_ActionDescription',
          @value = N'Creating new CAR';

     EXEC sys.sp_set_session_context
          @key   = N'HSE_CAR_SrcScreen',
          @value = N'CAR Entry';

     EXEC sys.sp_set_session_context
          @key   = N'HSE_CAR_UserID',
          @value = N'<loginUserName>';

   NOTE: sp_set_session_context persists for the life of the connection in the pool.
   Under connection pooling the same pooled connection MAY serve the saveRec call if
   the pool has only one connection (dev / single-user scenarios).
   In high-concurrency environments, the ##TEMP_HSE_TABLE guard remains the reliable path.
   ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
   STEP 3 — Regression tests (run after ALTER TRIGGER in dev then UAT)
   ----------------------------------------------------------------------- */

/*
  Test A — Web CAR Entry save (temp absent, context not set):
    INSERT INTO HSE_CRENTRY (...) VALUES (...);
    → trigger fires, OBJECT_ID guard evaluates FALSE, SESSION_CONTEXT variables NULL,
      defaults 'Creating new CAR' / 'CAR Entry' are used → succeeds with safe defaults.

  Test B — Web CAR Entry save with SESSION_CONTEXT pre-set:
    EXEC sys.sp_set_session_context @key=N'HSE_CAR_SrcScreen', @value=N'CAR Entry';
    EXEC sys.sp_set_session_context @key=N'HSE_CAR_ActionDescription', @value=N'Creating new CAR';
    INSERT INTO HSE_CRENTRY (...) VALUES (...);
    → trigger picks up SESSION_CONTEXT values → correct tracing description.

  Test C — Desktop / Observation close (temp present):
    IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL
        CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) PRIMARY KEY, VALUErec VARCHAR(50));
    INSERT INTO ##TEMP_HSE_TABLE VALUES('ActionDescription','Generating CAR from Observation');
    INSERT INTO ##TEMP_HSE_TABLE VALUES('SrcScreen','Observation Approval');
    INSERT INTO HSE_CRENTRY (...) VALUES (...);
    → trigger reads ##TEMP_HSE_TABLE → correct 'Observation Approval' in tracing.

  Test D — Pre-existing desktop behaviour unchanged:
    Same as C with desktop session — should produce identical tracing records as before.
*/

/* -----------------------------------------------------------------------
   END OF TEMPLATE
   ----------------------------------------------------------------------- */
GO
