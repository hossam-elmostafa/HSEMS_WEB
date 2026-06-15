/**
 * Site Survey serial helpers — C++ CHSEMSCommonCategory::getNewSerialNoForATab parity.
 * RQ_HSE_23_3_26_22_44
 */

export function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Read first non-empty FormGetField across candidate field names and scopes.
 */
export function formGetFirst(FormGetField, table, fieldNames, scopes = ['tab', 'scr']) {
  if (typeof FormGetField !== 'function') return '';
  for (const scope of scopes) {
    for (const f of fieldNames) {
      const v = FormGetField(table, f, scope);
      if (v != null && String(v).trim() !== '') return String(v).trim();
    }
  }
  return '';
}

/**
 * C++ getNewSerialNoForATab when strMasterKey != "":
 * SELECT ISNULL(MAX(serialField)+1,1) FROM detailTable WHERE detailLinkField = masterKeyVal
 * then FormSetField(detailTable, serialField, next).
 */
export async function applyNextDetailSerial(devInterface, {
  masterTable,
  masterKeyFields,
  detailTable,
  detailLinkField,
  serialField,
  formSetScope = 'tab',
}) {
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return;

  const masterKeyVal = formGetFirst(FormGetField, masterTable, masterKeyFields, ['scr']);
  if (!masterKeyVal) return;

  const serialVal = formGetFirst(FormGetField, detailTable, [serialField], ['tab', 'scr']);
  if (serialVal !== '') return;

  const escKey = escapeSqlString(masterKeyVal);
  const linkCols = Array.isArray(detailLinkField) ? detailLinkField : [detailLinkField];
  for (const linkCol of linkCols) {
    const sql = `SELECT ISNULL(MAX(${serialField})+1,1) AS NextSrl FROM ${detailTable} WHERE ${linkCol} = '${escKey}'`;
    try {
      const data = await executeSQLPromise(sql);
      const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
      const nextVal = row?.NextSrl ?? row?.NEXTSRL ?? row?.nextsrl ?? Object.values(row || {})[0];
      if (nextVal != null) {
        FormSetField(detailTable, serialField, String(nextVal), formSetScope);
        return;
      }
    } catch (e) {
      console.warn('[Web_HSE] Site Survey serial auto-generation (link column try):', linkCol, e);
    }
  }
}

/** Findings tab — C++ SitSrvyCategory: SITSRVYENTFNDNG_SRIL / SitSrvyEntFndng_LNK / HSE_SITSRVYENT + SitSrvyEnt_SitSrvyNo */
export async function applyFindingLineSerialIfEmpty(devInterface) {
  return applyNextDetailSerial(devInterface, {
    masterTable: 'HSE_SITSRVYENT',
    masterKeyFields: ['SITSRVYENT_SITSRVYNO', 'SitSrvyEnt_SitSrvyNo'],
    detailTable: 'HSE_SITSRVYENTFNDNG',
    detailLinkField: ['SitSrvyEntFndng_LNK', 'SITSRVYENTFNDNG_LNK'],
    serialField: 'SITSRVYENTFNDNG_SRIL',
    formSetScope: 'tab',
  });
}

/** Required Actions popup — C++ getNewSerialNoForATab(HSE_SITSRVYRQRDACTN, SitSrvyRqrdActn_LNK, …) */
export async function applyRequiredActionSerialIfEmpty(devInterface) {
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return;

  const TABLE = 'HSE_SITSRVYRQRDACTN';
  const linkVal = formGetFirst(FormGetField, TABLE, ['SitSrvyRqrdActn_LNK', 'SITSRVYRQRDACTN_LNK', 'SitSrvyRqrdActn_Lnk'], ['scr', 'tab']);
  if (!linkVal) return;

  const serialVal = formGetFirst(FormGetField, TABLE, ['SITSRVYRQRDACTN_ACTNNO'], ['tab', 'scr']);
  if (serialVal !== '') return;

  const esc = escapeSqlString(linkVal);
  const linkCols = ['SitSrvyRqrdActn_LNK', 'SITSRVYRQRDACTN_LNK'];
  for (const linkCol of linkCols) {
    const sql = `SELECT ISNULL(MAX(SITSRVYRQRDACTN_ACTNNO)+1,1) AS NextSrl FROM ${TABLE} WHERE ${linkCol} = '${esc}'`;
    try {
      const data = await executeSQLPromise(sql);
      const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
      const nextVal = row?.NextSrl ?? row?.NEXTSRL ?? row?.nextsrl ?? Object.values(row || {})[0];
      if (nextVal != null) {
        try {
          FormSetField(TABLE, 'SITSRVYRQRDACTN_ACTNNO', String(nextVal), 'scr');
        } catch (_) {
          FormSetField(TABLE, 'SITSRVYRQRDACTN_ACTNNO', String(nextVal), 'tab');
        }
        return;
      }
    } catch (e) {
      console.warn('[Web_HSE] Site Survey required-action serial (link try):', linkCol, e);
    }
  }
}
