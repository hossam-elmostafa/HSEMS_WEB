/**
 * RQ_HSE_12_4_26_00_40 — GAP-5: ##TEMP_HSE_TABLE + SESSION_CONTEXT in one SQL batch immediately before
 * HSE_CRENTRY DML so triggers run on the same pooled connection as the seed (desktop = single connection).
 * Pre-save alone cannot fix framework INSERT on another connection; post-save UPDATE / client INSERT benefit here.
 */

function escSql(v) {
  return String(v ?? '').replace(/'/g, "''");
}

/**
 * @param {string} userName - login / display user for UserID row
 * @param {string} srcScreen - SrcScreen temp row + session key
 * @param {string} actionDescription - ActionDescription temp row + session key
 * @returns {string} SQL batch (no leading/trailing semicolon required before concatenation)
 */
export function buildCrentryTriggerContextBatchSql(userName, srcScreen, actionDescription) {
  const u = escSql(userName);
  const src = escSql(srcScreen);
  const act = escSql(actionDescription);
  return `IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL
BEGIN
  CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,
    VALUErec VARCHAR(50) COLLATE DATABASE_DEFAULT,
    CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec));
END;
DELETE FROM ##TEMP_HSE_TABLE;
INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES('ActionDescription','${act}');
INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES('SrcScreen','${src}');
INSERT INTO ##TEMP_HSE_TABLE(KEYrec, VALUErec) VALUES('UserID','${u}');
EXEC sys.sp_set_session_context @key=N'HSE_CAR_ActionDescription', @value=N'${act}';
EXEC sys.sp_set_session_context @key=N'HSE_CAR_SrcScreen',         @value=N'${src}';
EXEC sys.sp_set_session_context @key=N'HSE_CAR_UserID',            @value=N'${u}';`;
}
