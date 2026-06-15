/**
 * RQ_HSE_12_4_26_00_40 — GAP-11: Auto-fill location/dept from CAR basis TXN.
 * C++: CarEntryCategory::FillCarDescAndLoc — on CARMODELBAS field change,
 * queries source transaction table to auto-fill department, site, location, area, case description.
 */

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/** Exported for RQ_HSE_12_4_26_00_40 — GAP-9 Layer 2 View Source TXN (same basis → source table as desktop fill). */
export const BASIS_QUERY_MAP = {
  '1': { table: 'HSE_PERFMSRMNTENT', key: 'PERFMSRMNTENT_VSTNO', fields: { desc: 'PERFMSRMNTENT_DESC', dept: 'PERFMSRMNTENT_DPRT', site: 'PERFMSRMNTENT_ST', loc: 'PERFMSRMNTENT_LOCTYP', area: 'PERFMSRMNTENT_AR' } },
  '2': { table: 'HSE_ADTPLNENT', key: 'ADTPLNENT_ADTNUM', fields: { desc: 'ADTPLNENT_DESC', dept: 'ADTPLNENT_DPRT', site: 'ADTPLNENT_ST', loc: null, area: null } },
  '3': { table: 'HSE_NRSTMISCENT', key: 'NRSTMISCENT_NRSTMISCNUM', fields: { desc: 'NRSTMISCENT_DESC', dept: 'NRSTMISCENT_WSHP', site: 'NRSTMISCENT_ST', loc: 'NRSTMISCENT_LOCTYP', area: 'NRSTMISCENT_AR' } },
  '4': { table: 'HSE_PTNLHZRDENT', key: 'PTNLHZRDENT_PTNLHZRDNO', fields: { desc: 'PTNLHZRDENT_DESC', dept: 'PTNLHZRDENT_DPRT', site: 'PTNLHZRDENT_ST', loc: 'PTNLHZRDENT_LOCTYP', area: 'PTNLHZRDENT_AR' } },
  '5': { table: 'HSE_VCLACDNTENT', key: 'VCLACDNTENT_VCLACDNTNO', fields: { desc: 'VCLACDNTENT_DESC', dept: 'VCLACDNTENT_DPRT', site: 'VCLACDNTENT_ST', loc: null, area: null } },
  '6': { table: 'HSE_ACDNTENT', key: 'ACDNTENT_ACDNTNUM', fields: { desc: 'ACDNTENT_DESC', dept: 'ACDNTENT_DPRT', site: 'ACDNTENT_ST', loc: 'ACDNTENT_LOCTYP', area: 'ACDNTENT_AR' } },
  '8': { table: 'HSE_RSKASSMNTENT', key: 'RSKASSMNTENT_ASSMNTNO', fields: { desc: 'RSKASSMNTENT_DESC', dept: 'RSKASSMNTENT_DPRT', site: 'RSKASSMNTENT_ST', loc: null, area: null } },
  '11': { table: 'HSE_SITSRVYENT', key: 'SITSRVYENT_SITSRVYNO', fields: { desc: 'SITSRVYENT_DESC', dept: 'SITSRVYENT_DPRT', site: 'SITSRVYENT_ST', loc: 'SITSRVYENT_LOCTYP', area: 'SITSRVYENT_AR' } },
};

const TARGET_FIELDS = {
  desc: 'CRCTVEACCENT_CASESDESC',
  dept: 'CRCTVEACCENT_DPRT',
  site: 'CRCTVEACCENT_SIT',
  loc: 'CRCTVEACCENT_LOCTYP',
  area: 'CRCTVEACCENT_AREA',
};

/**
 * @param {object} devInterfaceObj
 * @param {string} formTag — screen/form tag to set fields on
 * @param {string} basisCode — CRCTVEACCENT_CARMODELBAS value
 * @param {string} txnNum — source transaction number (CRCTVEACCENT_ACCDNTNUM or similar)
 */
export async function fillFromCarBasis(devInterfaceObj, formTag, basisCode, txnNum) {
  const { executeSQLPromise, FormSetField } = devInterfaceObj || {};
  if (!executeSQLPromise || typeof FormSetField !== 'function') return;

  const mapping = BASIS_QUERY_MAP[String(basisCode ?? '').trim()];
  if (!mapping || !txnNum) return;

  const escTxn = escapeSqlString(String(txnNum).trim());
  const selectCols = Object.entries(mapping.fields)
    .filter(([, col]) => col != null)
    .map(([alias, col]) => `ISNULL(${col},'') AS [${alias}]`)
    .join(', ');

  try {
    const sql = `SELECT TOP 1 ${selectCols} FROM ${mapping.table} WHERE ${mapping.key} = '${escTxn}'`;
    const rs = await executeSQLPromise(sql);
    const row = rs?.recordset?.[0] ?? rs?.[0];
    if (!row) return;

    for (const [alias, targetField] of Object.entries(TARGET_FIELDS)) {
      const val = String(row[alias] ?? '').trim();
      if (val) {
        try {
          FormSetField(formTag, targetField, val, 'scr');
        } catch (_) { /* field may not exist on form */ }
      }
    }
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-11 fillFromCarBasis:', e);
  }
}
