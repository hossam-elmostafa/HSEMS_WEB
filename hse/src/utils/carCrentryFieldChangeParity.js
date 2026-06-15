/**
 * C++ NewCarEntryCategory::DisplayFieldChange — CAR date / CAR source serial parity.
 * RQ_HSE_12_4_26_00_40
 */

import { getNextAllocatedCrentryCrn, getNextAllocatedCrentryCrsrln } from './carCrentrySerialAllocation.js';

const TABLE_MAIN = 'HSE_CRENTRY';
const FORM_SCR_TAG = 'HSE_TgCrEntry';

function cleanScalar(v) {
  if (v == null) return '';
  const s = String(v).trim();
  if (s === 'undefined' || s === 'null' || s === 'NaN') return '';
  return s;
}

/** @param {Record<string, unknown>} devInterfaceObj */
function formGetCrentry(dev, field) {
  const { FormGetField } = dev || {};
  if (typeof FormGetField !== 'function') return '';
  let v = FormGetField(TABLE_MAIN, field, 'SCR');
  if (v == null || cleanScalar(v) === '') v = FormGetField(FORM_SCR_TAG, field, 'SCR');
  return cleanScalar(v);
}

/** @param {Record<string, unknown>} devInterfaceObj */
function formSetCrentry(dev, field, value, srcType = 'SCR') {
  const { FormSetField } = dev || {};
  if (typeof FormSetField !== 'function') return;
  const safe = cleanScalar(value);
  if (safe === '') return;
  try {
    FormSetField(FORM_SCR_TAG, field, safe, srcType);
  } catch (_) {
    /* ignore */
  }
  try {
    FormSetField(TABLE_MAIN, field, safe, srcType);
  } catch (_) {
    /* ignore */
  }
}

/** Year from CRENTRY_CRDT (ISO or common locales). */
export function yearStringFromCrentryDate(dateStr) {
  if (dateStr == null || dateStr === '') return '';
  const s = String(dateStr).trim();
  const t = Date.parse(s);
  if (!Number.isNaN(t)) return String(new Date(t).getFullYear());
  const m = s.match(/^(\d{1,2})[/.-](\d{1,2})[/.-](\d{2,4})\b/);
  if (m) {
    let y = parseInt(m[3], 10);
    if (y < 100) y += 2000;
    return String(y);
  }
  const iso = s.match(/^(\d{4})[/.-](\d{1,2})[/.-](\d{1,2})/);
  if (iso) return iso[1];
  return '';
}

/**
 * Next CRENTRY_CRN for year (desktop getNxtSrl when serial not source-scoped).
 * RQ_HSE_12_4_26_00_40
 */
export async function getNextCarNumberForYear(executeSQLPromise, yearStr, getValFromRecordSet) {
  // RQ_HSE_12_4_26_00_40 — M6: shared allocator (MAX+1 + skip occupied)
  return getNextAllocatedCrentryCrn(executeSQLPromise, yearStr, getValFromRecordSet);
}

/**
 * Next CRENTRY_CRSRLN for year + source (C++ getNxtSrl with WHERE year+source).
 * RQ_HSE_12_4_26_00_40
 */
export async function getNextCarSerialBySource(executeSQLPromise, yearStr, sourceStr, getValFromRecordSet) {
  if (!cleanScalar(yearStr) || !cleanScalar(sourceStr)) return '1';
  return getNextAllocatedCrentryCrsrln(executeSQLPromise, yearStr, sourceStr, getValFromRecordSet);
}

/**
 * When CAR date year differs from CRENTRY_CRYR: set year + next CRN (C++ UpdateYearAndSerial).
 * RQ_HSE_12_4_26_00_40
 */
export async function applyCrentryDateYearSerialIfChanged(devInterfaceObj) {
  const { executeSQLPromise, getValFromRecordSet } = devInterfaceObj || {};
  if (typeof executeSQLPromise !== 'function') return;

  const crdt = formGetCrentry(devInterfaceObj, 'CRENTRY_CRDT');
  const strYear = yearStringFromCrentryDate(crdt);
  if (!strYear) return;

  const strCurrentYear = formGetCrentry(devInterfaceObj, 'CRENTRY_CRYR');
  if (strCurrentYear === strYear) return;

  formSetCrentry(devInterfaceObj, 'CRENTRY_CRYR', strYear, 'SCR');
  const nextCrn = await getNextCarNumberForYear(executeSQLPromise, strYear, getValFromRecordSet);
  formSetCrentry(devInterfaceObj, 'CRENTRY_CRN', nextCrn, 'SCR');
}

/**
 * CAR Review: when CRENTRY_CRSRC changes, refresh CRENTRY_CRSRLN (C++ HSE_TGCRRVW branch).
 * RQ_HSE_12_4_26_00_40
 */
export async function applyCrentrySourceSerialOnReview(devInterfaceObj) {
  const { executeSQLPromise, getValFromRecordSet } = devInterfaceObj || {};
  if (typeof executeSQLPromise !== 'function') return;

  const strCARSrc = formGetCrentry(devInterfaceObj, 'CRENTRY_CRSRC');
  if (!strCARSrc) {
    formSetCrentry(devInterfaceObj, 'CRENTRY_CRSRLN', '', 'SCR');
    return;
  }
  const strCARYear = formGetCrentry(devInterfaceObj, 'CRENTRY_CRYR');
  if (!strCARYear) return;
  const srl = await getNextCarSerialBySource(executeSQLPromise, strCARYear, strCARSrc, getValFromRecordSet);
  formSetCrentry(devInterfaceObj, 'CRENTRY_CRSRLN', srl, 'SCR');
}
