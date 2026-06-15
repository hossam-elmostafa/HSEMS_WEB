/*
  HSE — ##TEMP_HSE_TABLE trigger hardening (plan: car_save_temp_hse_table_72a50b1e)

  Policy
  ------
  Do not change WebInfra Client or Webinfra.Server for this issue. Fix belongs in SQL Server
  (this script + DBA ALTER TRIGGER) and/or the HSE application package under HSEMS_WEB_Original/hse.

  Problem
  -------
  Web saveRec runs INSERT/UPDATE in a new SQL session. Client-side executeSQL that seeds
  ##TEMP_HSE_TABLE uses another session; the global temp is gone before save. Triggers that
  SELECT from ##TEMP_HSE_TABLE without checking raise: Invalid object name '##TEMP_HSE_TABLE'.

  Goal
  ----
  When ##TEMP_HSE_TABLE exists, keep current behaviour (desktop / Observation paths).
  When it does not exist, use safe defaults so INSERT/UPDATE still succeeds.

  This file does NOT alter your database. Run the discovery section in SSMS, then edit the
  real trigger/procedure bodies in a controlled migration (dev → UAT → prod).
*/

SET NOCOUNT ON;

/* --- 1) Discover objects referencing the temp table name ---------------- */
SELECT
    o.type_desc,
    SCHEMA_NAME(o.schema_id) AS schema_name,
    o.name AS object_name,
    o.object_id
FROM sys.sql_modules AS m
INNER JOIN sys.objects AS o ON m.object_id = o.object_id
WHERE m.definition LIKE N'%TEMP_HSE_TABLE%'
ORDER BY o.type_desc, o.name;

/* Optional: search only triggers on HSE_CRENTRY */
SELECT
    tr.name AS trigger_name,
    OBJECT_NAME(tr.parent_id) AS parent_table
FROM sys.triggers AS tr
INNER JOIN sys.sql_modules AS m ON m.object_id = tr.object_id
WHERE m.definition LIKE N'%TEMP_HSE_TABLE%'
  AND tr.parent_id = OBJECT_ID(N'dbo.HSE_CRENTRY', N'U');

/*
  --- 2) Pattern to apply inside trigger / procedure (template) -----------

  Declare working variables with web-safe defaults (align with desktop CAR Entry when unknown):

    DECLARE @ActionDescription NVARCHAR(200) = N'Creating new CAR';
    DECLARE @SrcScreen         NVARCHAR(200) = N'CAR Entry';
    DECLARE @UserID            NVARCHAR(200) = NULL;

  If the global temp exists, overwrite from key/value rows (match keys your trigger already uses):

    IF OBJECT_ID(N'tempdb..##TEMP_HSE_TABLE') IS NOT NULL
    BEGIN
        SELECT @ActionDescription = VALUErec FROM ##TEMP_HSE_TABLE WHERE KEYrec = N'ActionDescription';
        SELECT @SrcScreen         = VALUErec FROM ##TEMP_HSE_TABLE WHERE KEYrec = N'SrcScreen';
        SELECT @UserID            = VALUErec FROM ##TEMP_HSE_TABLE WHERE KEYrec = N'UserID';
    END

  Use @ActionDescription / @SrcScreen / @UserID in INSERTs to tracing tables instead of
  subqueries that read ##TEMP_HSE_TABLE directly without a guard.

  Adjust default @SrcScreen if the trigger serves multiple screens (e.g. N'Observation Approval'
  is only correct when temp was seeded from that flow; N'CAR Entry' is a reasonable fallback
  for direct web CAR Entry saves).

  --- 3) Regression tests (manual) ------------------------------------------
  - New CAR Entry save from web (temp absent) — should succeed.
  - Observation → CAR path (temp present in-session) — tracing should still match seeded SrcScreen.
  - Desktop client — unchanged if it still seeds ##TEMP_HSE_TABLE before save.
*/
