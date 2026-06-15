/**
 * CAR number allocation — desktop getNxtSrl is effectively MAX+1 with table scope.
 * RQ_HSE_12_4_26_00_40 — M6 MEDIUM: extend MAX+1 with in-batch collision skip (holes / partial races);
 * callers should still retry INSERT on unique-key errors when allocation and INSERT are separate round-trips.
 */

function escSql(v) {
  return String(v ?? '').replace(/'/g, "''");
}

function readNewSerialFromRs(data, getValFromRecordSet) {
  const row = data?.recordset?.[0] ?? data?.[0];
  const val =
    row && typeof getValFromRecordSet === 'function'
      ? getValFromRecordSet(data, 'New_Serial')
      : row?.New_Serial ?? row?.new_serial;
  return val;
}

/**
 * Next free CRENTRY_CRN for a calendar year (locked read + skip if that number already exists).
 * @param {Function} executeSQLPromise
 * @param {string} yearStr
 * @param {Function} [getValFromRecordSet]
 * @returns {Promise<string>}
 */
export async function getNextAllocatedCrentryCrn(executeSQLPromise, yearStr, getValFromRecordSet) {
  const yRaw = String(yearStr ?? '').trim();
  if (!/^\d{1,4}$/.test(yRaw)) return '1';
  const y = escSql(yRaw);
  // RQ_HSE_12_4_26_00_40 — single batch: UPDLOCK range + WHILE EXISTS skips occupied numbers (legacy getNxtSrl-style safety net)
  const sql = `
DECLARE @n int;
SELECT @n = ISNULL(MAX(CRENTRY_CRN), 0) + 1
FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
WHERE CRENTRY_CRYR = N'${y}';
WHILE EXISTS (
  SELECT 1
  FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
  WHERE CRENTRY_CRYR = N'${y}'
    AND CRENTRY_CRN = @n
)
  SET @n = @n + 1;
SELECT @n AS New_Serial;
`;
  try {
    const data = await executeSQLPromise(sql);
    const val = readNewSerialFromRs(data, getValFromRecordSet);
    const n = parseInt(val, 10);
    return Number.isFinite(n) && n > 0 ? String(n) : '1';
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 getNextAllocatedCrentryCrn:', e);
    return '1';
  }
}

/**
 * Next free CRENTRY_CRSRLN for year + source text (same pattern as CRN).
 * @param {Function} executeSQLPromise
 * @param {string} yearStr
 * @param {string} sourceStr
 * @param {Function} [getValFromRecordSet]
 * @returns {Promise<string>}
 */
export async function getNextAllocatedCrentryCrsrln(executeSQLPromise, yearStr, sourceStr, getValFromRecordSet) {
  const y = escSql(String(yearStr ?? '').trim());
  const src = escSql(String(sourceStr ?? '').trim());
  if (!y || !src) return '1';
  const sql = `
DECLARE @n int;
SELECT @n = ISNULL(MAX(CRENTRY_CRSRLN), 0) + 1
FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
WHERE CRENTRY_CRYR = N'${y}' AND CRENTRY_CRSRC = N'${src}';
WHILE EXISTS (
  SELECT 1
  FROM dbo.HSE_CRENTRY WITH (UPDLOCK, HOLDLOCK)
  WHERE CRENTRY_CRYR = N'${y}'
    AND CRENTRY_CRSRC = N'${src}'
    AND CRENTRY_CRSRLN = @n
)
  SET @n = @n + 1;
SELECT @n AS New_Serial;
`;
  try {
    const data = await executeSQLPromise(sql);
    const val = readNewSerialFromRs(data, getValFromRecordSet);
    const n = parseInt(val, 10);
    return Number.isFinite(n) && n > 0 ? String(n) : '1';
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 getNextAllocatedCrentryCrsrln:', e);
    return '1';
  }
}
