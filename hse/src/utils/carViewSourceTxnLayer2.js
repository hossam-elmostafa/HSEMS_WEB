/**
 * RQ_HSE_12_4_26_00_40 — GAP-9: Layer 2 OpenTXNInquiryScreen — VIEW_SOURCE_TXN on HSE_CRCTVEACCENT flows.
 * C++: CarCategory::OpenTXNInquiryScreen from CRCTVEACCENT_CARMODELBAS + CRCTVEACCENT_ACCDNTNUM (+ year where needed).
 * Inquiry tags / filter shapes aligned with carCustomButtons.js handleViewSourceTxn (Layer 1 CRSRC path).
 */

import { BASIS_QUERY_MAP } from './carEntryFillFromBasis.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

function getFieldFromRecord(eventObj, fieldName) {
  const fullRecordArr = eventObj?.fullRecord ?? eventObj?.fullRecordArr ?? [];
  const rec = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
  if (!rec || typeof rec !== 'object') return '';
  const u = String(fieldName).toUpperCase();
  for (const k of Object.keys(rec)) {
    if (k.toUpperCase() === u) return (rec[k] != null ? String(rec[k]) : '').trim();
  }
  return '';
}

/** Basis code (string) → inquiry screen tag (must match host menu registration). */
const INQUIRY_TAG_BY_BASIS = {
  '1': 'HSE_TgperfMsrmntInq',
  '2': 'HSE_TgAdtPlnInq',
  '3': 'HSE_TgNrstMiscInq',
  '4': 'HSE_TgptnlhzrdInq',
  '5': 'HSE_TgVclAcdntInqtn',
  '6': 'HSE_TgAcdntInq',
  '8': 'HSE_TgrskassmntInq',
  '11': 'HSE_TgSitSrvyInq',
};

/**
 * Build list criteria from BASIS_QUERY_MAP key column; observation/audit use year when present on row.
 * @param {string} basisCode
 * @param {string} txnEsc — already SQL-escaped transaction key value
 * @param {string} txnYrEsc — escaped year for dual-key modules
 */
function buildOpenCriteria(basisCode, txnEsc, txnYrEsc) {
  const code = String(basisCode ?? '').trim();
  if (!BASIS_QUERY_MAP[code]) return null;
  if (code === '3') {
    if (txnYrEsc) return `(NrstMiscEnt_Yr = '${txnYrEsc}') AND (NrstMiscEnt_NrstMiscNum = '${txnEsc}')`;
    return `(NrstMiscEnt_NrstMiscNum = '${txnEsc}')`;
  }
  if (code === '2') {
    if (txnYrEsc) return `(ADTPLNENT_ADTYR = '${txnYrEsc}') AND (ADTPLNENT_ADTNUM = '${txnEsc}')`;
    return `(ADTPLNENT_ADTNUM = '${txnEsc}')`;
  }
  const m = BASIS_QUERY_MAP[code];
  return `(${m.key} = '${txnEsc}')`;
}

/**
 * @returns {boolean} true if consumed (opened inquiry or showed message)
 */
export function handleViewSourceTxnLayer2(eventObj, devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const AskYesNoMessage = devInterfaceObj?.AskYesNoMessage;
  if (typeof openScr !== 'function') return false;

  const basisRaw = getFieldFromRecord(eventObj, 'CRCTVEACCENT_CARMODELBAS');
  const txnNum = getFieldFromRecord(eventObj, 'CRCTVEACCENT_ACCDNTNUM');
  const txnYr =
    getFieldFromRecord(eventObj, 'CRCTVEACCENT_TXNYR') ||
    getFieldFromRecord(eventObj, 'CRCTVEACCENT_TXN_YR') ||
    '';

  if (!String(txnNum).trim()) {
    if (typeof AskYesNoMessage === 'function') {
      AskYesNoMessage('Source transaction number is missing on this corrective action row. (RQ_HSE_12_4_26_00_40 GAP-9)');
    }
    return true;
  }

  const code = String(basisRaw ?? '').trim();
  const tag = INQUIRY_TAG_BY_BASIS[code];
  const txnEsc = escapeSqlString(txnNum);
  const txnYrEsc = escapeSqlString(txnYr);
  const criteria = buildOpenCriteria(code, txnEsc, txnYrEsc);

  if (!tag || !criteria) {
    if (typeof AskYesNoMessage === 'function') {
      AskYesNoMessage(
        `View Source TXN is not available in web for CAR basis code "${code || '?'}". (RQ_HSE_12_4_26_00_40 GAP-9)`
      );
    }
    return true;
  }

  openScr(tag, {}, criteria, 'list', false, {}, false, false);
  return true;
}
