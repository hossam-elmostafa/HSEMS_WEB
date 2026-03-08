/**
 * Shared helpers for opening the Reject Reason popup (HSE_TGRJCTRSN).
 * C++: NewCarEntryCategory::Reject(strSourceModule, strKeyField) / RejectCARProcess.
 */

export const REJECT_REASON_SCREEN_TAG = 'HSE_TGRJCTRSN';
const RJCTRSN_LINKWITHMAIN = 'RJCTRSN_LINKWITHMAIN';
const RJCTRSN_MODULETYPE = 'RJCTRSN_MODULETYPE';
const RJCTRSN_TRACINGLNK = 'RJCTRSN_TRACINGLNK';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Open Reject Reason popup with criteria and default values.
 * @param {Function} openScr - devInterfaceObj.openScr
 * @param {string} strSourceModule - e.g. CRCTVEACCENT-AE, CRCTVEACCENT-RV, ACTION-RV, CARFLOWUP-RV
 * @param {string} strPrimKey - current record PRMKY
 */
export function openRejectReasonScreen(openScr, strSourceModule, strPrimKey) {
  if (typeof openScr !== 'function') return;
  const escapedKey = escapeSqlString(strPrimKey);
  const escapedMod = escapeSqlString(strSourceModule);
  const strCriteria = `SELECT * FROM HSE_RJCTRSN WHERE (${RJCTRSN_LINKWITHMAIN}= '${escapedKey}') AND (${RJCTRSN_MODULETYPE}= '${escapedMod}') AND (${RJCTRSN_TRACINGLNK} = 0)`;
  const defValObj = {
    [RJCTRSN_LINKWITHMAIN]: strPrimKey,
    [RJCTRSN_MODULETYPE]: strSourceModule
  };
  openScr(REJECT_REASON_SCREEN_TAG, {}, strCriteria, 'edit', false, defValObj, false, false);
}

/**
 * Get PRMKY from ButtonClicked eventObj (fullRecord / fullRecordArr).
 */
export function getCurrentRecordPRMKY(eventObj) {
  const fullRecordArr = eventObj?.fullRecord ?? eventObj?.fullRecordArr ?? [];
  const rec = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
  if (!rec || typeof rec !== 'object') return '';
  return (rec.PRMKY ?? rec.prmky ?? rec['PRMKY'] ?? '') + '';
}
