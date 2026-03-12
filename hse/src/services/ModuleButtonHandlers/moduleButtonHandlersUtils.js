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
    'HSE_TGINCDNTINVSTGTNENTRY': 'Incident Investigation Entry',
    'HSE_TGINCDNTINVSTGTNAPRVL': 'Incident Investigation Approval',
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

export { escapeSqlString };
