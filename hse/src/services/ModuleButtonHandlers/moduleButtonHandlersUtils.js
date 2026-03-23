/**
 * Shared utilities for module custom button handlers.
 * Used by Risk Assessment, Site Survey, PTW, Incident, etc. (desktop Category DisplayCustomButtonClicked parity).
 */

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Get key field value from ButtonClicked eventObj.
 * Tries: fullRecordArrKeys[0], first record[keyFieldName], FormGetField(tableName, keyFieldName).
 * @param {Object} eventObj - ButtonClicked event
 * @param {Object} devInterfaceObj - FormGetField, etc.
 * @param {{ tableName: string, keyFieldName: string, formTag?: string }} options
 * @returns {string}
 */
export function getKeyFromEvent(eventObj, devInterfaceObj, options = {}) {
  const { tableName, keyFieldName, formTag } = options;
  const fullRecordArr = eventObj?.fullRecord ?? eventObj?.fullRecordArr ?? [];
  const fullRecordArrKeys = eventObj?.fullRecordArrKeys;

  if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
    return fullRecordArrKeys[0].toString().trim();
  }

  const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
  if (firstRecord && typeof firstRecord === 'object') {
    const k = keyFieldName?.toUpperCase?.() ?? keyFieldName;
    for (const key of Object.keys(firstRecord)) {
      if (key.toUpperCase() === k) return (firstRecord[key] != null ? String(firstRecord[key]) : '').trim();
    }
    const prmky = firstRecord.PRMKY ?? firstRecord.prmky;
    if (prmky != null) return String(prmky).trim();
  }

  const FormGetField = devInterfaceObj?.FormGetField;
  if (FormGetField && (tableName || formTag)) {
    const t = tableName || formTag;
    const v = FormGetField(t, keyFieldName, 'scr');
    if (v != null && v !== '') return String(v).trim();
  }

  return '';
}

/**
 * RQ_HSE_23_3_26_23_15 — HSE_EQINSPCTN primary key: metadata uses EQINSPCTN_PRMKY; some bindings may use EQINSPCTN_PRMRYKY.
 * @param {Object} eventObj
 * @param {Object} devInterfaceObj
 * @param {string} [formTag]
 * @returns {string}
 */
export function getEQInspectionPrimaryKey(eventObj, devInterfaceObj, formTag) {
  const ft = formTag ?? eventObj?.strScrTag ?? eventObj?.strFormTag;
  let k = getKeyFromEvent(eventObj, devInterfaceObj, {
    tableName: 'HSE_EQINSPCTN',
    keyFieldName: 'EQINSPCTN_PRMKY',
    formTag: ft,
  });
  if (!k) {
    k = getKeyFromEvent(eventObj, devInterfaceObj, {
      tableName: 'HSE_EQINSPCTN',
      keyFieldName: 'EQINSPCTN_PRMRYKY',
      formTag: ft,
    });
  }
  return (k && String(k).trim()) || '';
}

/**
 * Execute SQL and refresh. C++: ExecuteSQL(strSQL); RefreshScreen("", REFRESH_SELECTED);
 */
export async function runTxnAndRefresh(devInterfaceObj, sql, messageOnError = 'Operation failed') {
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;
  try {
    await executeSQLPromise(sql);
    if (typeof refreshData === 'function') refreshData('');
    return true;
  } catch (e) {
    console.warn('[Web_HSE] ModuleButtonHandlers:', e);
    if (typeof AskYesNoMessage === 'function') AskYesNoMessage(messageOnError);
    return false;
  }
}

/**
 * Run multiple SQL statements in sequence then refresh once. Used for UpdateStatus + InsertRecIntoTracingTab (e.g. Vehicle Accident).
 */
export async function runMultipleSqlAndRefresh(devInterfaceObj, sqlStatements, messageOnError = 'Operation failed') {
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise || !Array.isArray(sqlStatements) || sqlStatements.length === 0) return false;
  try {
    for (const sql of sqlStatements) {
      await executeSQLPromise(sql);
    }
    if (typeof refreshData === 'function') refreshData('');
    return true;
  } catch (e) {
    console.warn('[Web_HSE] ModuleButtonHandlers:', e);
    if (typeof AskYesNoMessage === 'function') AskYesNoMessage(messageOnError);
    return false;
  }
}

/**
 * Simple screen caption by form tag (C++ GetScrCptnByTag(66, formTag, "") replacement).
 * BUG_HSE_HSM_14_3_26_17_33: Observation screens added so reject tracing tab Source Screen uses caption (desktop behaviour).
 * RQ_HSE_23_3_26_3_36: Incident Flash / Medical / Investigation tags mapped for reject tracing captions.
 */
export function getScreenCaption(strScrTag) {
  if (!strScrTag) return '';
  const t = String(strScrTag).toUpperCase();
  const map = {
    'HSE_TGRSKASSMNTENT': 'Risk Assessment Entry',
    'HSE_TGRSKASSMNTAPR': 'Risk Assessment Approval',
    'HSE_TGRSKASSMNTFOLLWUPDT': 'Risk Assessment Follow-up',
    'HSE_TGSITSRVYENT': 'Site Survey Entry',
    'HSE_TGSITSRVYCNFRMTN': 'Site Survey Confirmation',
    'HSE_TGSITSRVYFLWUP': 'Site Survey Result',
    'HSE_TGPTWSRGSTRENT': 'PTW Register Entry',
    'HSE_TGPTWSRGSTRCNFRMTN': 'PTW Register Confirmation',
    'HSE_TGPTWSRGSTRCLS': 'PTW Register Close',
    'HSE_TGACDNTENT': 'Incident Preliminary Entry',
    'HSE_TGACDNTCNFRMTN': 'Incident Preliminary Review',
    'HSE_TGACDNTFOLLWUPDT': 'Incident Preliminary Follow-Up',
    // RQ_HSE_23_3_26_3_36
    'HSE_TGINCDNTINVSTGTNENTRY': 'Incident Investigation Entry',
    'HSE_TGINCDNTINVSTGTNAPRVL': 'Incident Investigation Approval',
    'HSE_TGINCDNTFLSHRPRT': 'Incident Flash Entry',
    'HSE_TGINCDNTFLSHRPRTRVEW': 'Incident Flash Review',
    'HSE_TGINCDNTMDCLRPRT': 'Incident Medical Report Entry',
    'HSE_TGINCDNTMDCLRPRTFLWUP': 'Incident Medical Report Follow-Up',
    // BUG_HSE_HSM_14_3_26_17_33: Observation screens – tracing tab Source Screen must be caption not tag (desktop uses GetScrCptnByTag).
    'HSE_TGNRSTMISCENT': 'Observation Entry',
    'HSE_TGNRSTMISCCNFRMTN': 'Observation Review',
    'HSE_TGNRSTMISCFLWUP': 'Observation Approval',
    // RQ_HSE_23_3_26_15_42: Awareness / Training Plan — reject tracing & pending reject source caption
    'HSE_TGAWRNSPLNENTRY': 'Awareness Plan Entry',
    'HSE_TGAWRNSPLNAPRVL': 'Awareness Plan Approval',
    'HSE_TGAWRNSPLNEXCTN': 'Awareness Plan Execution',
    // RQ_HSE_23_3_26_17_05: Rescue Plan — cancel / tracing source caption
    'HSE_TGRSCUPLNENTRY': 'Rescue Plan Entry',
    'HSE_TGRSCUPLNINQURY': 'Rescue Plan Inquiry',
    // RQ_HSE_23_3_26_23_15 — EQ Inspection (uppercase screen tags)
    'HSE_EQINSPCTNRQUST': 'EQ Inspection Request',
    'HSE_EQINSPCTNRSULT': 'EQ Inspection Result',
    'HSE_EQINSPCTNWRK': 'In-Site EQ',
    'HSE_EQINSPCTNINQRY': 'EQ Inspection Inquiry',
  };
  return map[t] || strScrTag;
}

/**
 * C++ UpdateStatus + InsertActionRecordIntoTracingTab equivalent using SPs updateTXNSts and InsertRecIntoTracingTab.
 * Used by Vehicle Accident (and optionally Potential Hazard, Loss Accident) where desktop has no single completion SP.
 * @param {Object} devInterfaceObj
 * @param {string} key - record key
 * @param {string} tablePart - e.g. VCLACDNTENT
 * @param {string} keyFldNam - e.g. VCLACDNTENT_VCLACDNTNO
 * @param {string} status - e.g. '4', '5', '6'
 * @param {string} actionLabel - e.g. 'Confirmed', 'Completed', 'Closed'
 */
export function buildUpdateStatusAndTracingSql(key, tablePart, keyFldNam, status, actionLabel, sourceScreenName, userName) {
  const ek = escapeSqlString(key);
  const es = escapeSqlString(sourceScreenName || '');
  const eu = escapeSqlString(userName || '');
  const linkPart = keyFldNam.replace(tablePart + '_', '').replace(tablePart, '') || keyFldNam;
  const sql1 = `EXECUTE updateTXNSts '${ek}','${status}','${tablePart}','${keyFldNam}'`;
  const sql2 = `EXECUTE InsertRecIntoTracingTab '${tablePart}','${linkPart}','${actionLabel}','${es}','${eu}','${ek}'`;
  return [sql1, sql2];
}

/**
 * Normalized uppercase screen tag(s) for Incident Preliminary Entry workflow (`ACDNTENT_ENTCMPLT`, preliminary `ACDNTENT_RJC`, etc.).
 * Confirm the tag in your APP JSON / screen metadata: e.g. `HSE_TgAcdntEnt` maps to **`HSE_TGACDNTENT`** (uppercase).
 * Add more entries to the Set only if production registers an alias screen tag for the same behaviour.
 */
export const INCIDENT_PRELIMINARY_ENTRY_SCREEN_TAGS = new Set(['HSE_TGACDNTENT']);

/** Menu `TagName` for Incident Preliminary Review — must match APP JSON; same logical screen as `screenHandlers` key `HSE_TgAcdntcnfrmtn`. */
export const INCIDENT_PRELIMINARY_REVIEW_MENU_TAG = 'HSE_TgAcdntcnfrmtn';

/** Uppercase screen tag for Incident Preliminary Review (`HSE_TGACDNTCNFRMTN`). */
export const INCIDENT_PRELIMINARY_REVIEW_SCREEN_TAG = 'HSE_TGACDNTCNFRMTN';

export function isIncidentPreliminaryEntryScreenTag(strScrTag) {
  if (!strScrTag) return false;
  return INCIDENT_PRELIMINARY_ENTRY_SCREEN_TAGS.has(String(strScrTag).toUpperCase());
}

function policyMeansAccidentConfirmationEnabled(v) {
  const s = String(v ?? '').trim().toUpperCase();
  return s === '1' || s === 'Y' || s === 'TRUE' || s === 'T';
}

/**
 * C++ AccidentConfirmationCategory::OpenScreen — CheckPolicy("HSEPLC_ACDNTCNFRMTNRQRD").
 * @returns {Promise<boolean>} true if preliminary review screen is permitted
 */
export async function getAccidentConfirmationPolicyAllowsReviewScreen(devInterfaceObj) {
  const exec = devInterfaceObj?.executeSQLPromise || devInterfaceObj?.executeSQL;
  const getValFromRecordSet = devInterfaceObj?.getValFromRecordSet;
  if (!exec) return true;
  try {
    const data = await exec(`EXEC GetPolicyValue 'HSE_HSEPLC','HSEPLC_ACDNTCNFRMTNRQRD'`);
    let v = '';
    if (getValFromRecordSet && data?.recordset?.[0]) {
      v = getValFromRecordSet(data, 'HSEPLC_ACDNTCNFRMTNRQRD');
    } else {
      const row = data?.recordset?.[0] ?? data?.[0];
      if (row && typeof row === 'object') {
        v = row.HSEPLC_ACDNTCNFRMTNRQRD ?? row.hseplc_acdntcnfrmtnrqrd ?? Object.values(row)[0];
      }
    }
    if (v !== '' && v != null) return policyMeansAccidentConfirmationEnabled(v);
  } catch (e) {
    console.warn('[Web_HSE] GetPolicyValue HSEPLC_ACDNTCNFRMTNRQRD:', e);
  }
  try {
    const data = await exec(`SELECT TOP 1 HSEPLC_ACDNTCNFRMTNRQRD AS V FROM HSE_HSEPLC`);
    const row = data?.recordset?.[0] ?? data?.[0];
    const v = row?.V ?? row?.v ?? row?.HSEPLC_ACDNTCNFRMTNRQRD;
    return policyMeansAccidentConfirmationEnabled(v);
  } catch (e2) {
    console.warn('[Web_HSE] HSE_HSEPLC HSEPLC_ACDNTCNFRMTNRQRD fallback:', e2);
    return true;
  }
}

/**
 * Desktop AccidentEntryCategory comment: header "No. of injured persons" vs injured-person tab rows.
 * Field from HSE_TgAcdntEnt header.json: ACDNTENT_NUMINJRDPRSN.
 * @returns {{ ok: boolean, message?: string }}
 */
export async function validateInjuredPersonCountMatchesHeader(devInterfaceObj, accdntKey) {
  const fg = devInterfaceObj?.FormGetField;
  const exec = devInterfaceObj?.executeSQLPromise || devInterfaceObj?.executeSQL;
  if (!fg || !exec || !accdntKey) return { ok: true };
  let declared = fg('HSE_ACDNTENT', 'ACDNTENT_NUMINJRDPRSN', 'scr');
  if (declared == null || declared === '') return { ok: true };
  const d = parseInt(String(declared).trim(), 10);
  if (Number.isNaN(d)) return { ok: true };
  const ek = escapeSqlString(String(accdntKey).trim());
  try {
    const rs = await exec(
      `SELECT COUNT(0) AS CNT FROM HSE_ACDNTENTINJRDPRSN WHERE ACDNTENTINJRDPRSN_LNK = '${ek}'`
    );
    const row = rs?.recordset?.[0] ?? rs?.[0]?.recordset?.[0] ?? rs?.[0];
    const cnt = parseInt(row?.CNT ?? row?.cnt ?? Object.values(row || {})[0], 10) || 0;
    if (cnt !== d) {
      return {
        ok: false,
        message:
          'Number of injured persons in the main field must match the injured persons tab. Update the header or tab records.',
      };
    }
  } catch (e) {
    console.warn('[Web_HSE] validateInjuredPersonCountMatchesHeader:', e);
    return { ok: true };
  }
  return { ok: true };
}

/**
 * C++ AccidentCategory::getAccidentRootCauses — ACDNTENTACDNTRSN_GETROTCUS button.
 */
export async function executeGetAccidentRootCauses(devInterfaceObj) {
  const fg = devInterfaceObj?.FormGetField;
  const exec = devInterfaceObj?.executeSQLPromise || devInterfaceObj?.executeSQL;
  const refreshData = devInterfaceObj?.refreshData;
  const getUserName = devInterfaceObj?.getUserName;
  if (!fg || !exec) return false;
  const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const key = fg('HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM', 'scr');
  const typ = fg('HSE_ACDNTENT', 'ACDNTENT_ACDNTTYP', 'scr');
  if (!key || String(key).trim() === '') {
    if (devInterfaceObj?.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Save the record first.');
    return false;
  }
  if (!typ || String(typ).trim() === '') {
    if (devInterfaceObj?.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Select accident type first.');
    return false;
  }
  const ek = escapeSqlString(String(key).trim());
  const et = escapeSqlString(String(typ).trim());
  const eu = escapeSqlString(userName);
  try {
    await exec(`EXECUTE getAccidentRootCauses '${eu}',${ek},'${et}'`);
    if (typeof refreshData === 'function') refreshData('');
    return true;
  } catch (e) {
    console.warn('[Web_HSE] executeGetAccidentRootCauses:', e);
    if (devInterfaceObj?.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error loading root causes.');
    return false;
  }
}

export { escapeSqlString };
