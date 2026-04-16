/**
 * Observation Entry — Observation Year + Observation No. for new saves (desktop getNxtSrl parity).
 * C++ CHSEMSCommonCategory::getNxtSrl(serialField, table, linkField, yearValue):
 *   SELECT ISNULL(MAX(NRSTMISCENT_NO)+1,1) FROM <physTable> WHERE NRSTMISCENT_YR = '<year>'
 * Tries HSE_NRSTMISCENT then HSE_NrstMiscEnt (screen ReportTableName).
 * RQ_HSE_4_4_25_18_22
 */

const TABLE_VIEW = 'HSE_vwNRSTMISCENT';
const FORM_SCR_TAG = 'HSE_TgNrstMiscEnt';
/** Primary: dashboards / T-SQL use HSE_NRSTMISCENT. Fallback: screen JSON ReportTableName. */
const PHYS_TABLE_CANDIDATES = ['HSE_NRSTMISCENT', 'HSE_NrstMiscEnt'];

function escSql(v) {
  return String(v ?? '').replace(/'/g, "''");
}

function cleanScalar(v) {
  if (v == null) return '';
  const s = String(v).trim();
  if (s === 'undefined' || s === 'null' || s === 'NaN') return '';
  return s;
}

/** @param {unknown} dateStr */
function yearStringFromIssueDate(dateStr) {
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

function setFullRecField(fullRec, logicalName, value) {
  if (!fullRec || value == null || cleanScalar(value) === '') return;
  const upper = logicalName.toUpperCase();
  const existingKey = Object.keys(fullRec).find((key) => key.toUpperCase() === upper);
  fullRec[existingKey || upper] = value;
}

function formGetObs(dev, field) {
  const { FormGetField } = dev || {};
  if (typeof FormGetField !== 'function') return '';
  let v = FormGetField(TABLE_VIEW, field, 'SCR');
  if (v == null || cleanScalar(v) === '') v = FormGetField(FORM_SCR_TAG, field, 'SCR');
  return cleanScalar(v);
}

function formSetObs(dev, field, value) {
  const { FormSetField } = dev || {};
  if (typeof FormSetField !== 'function') return;
  const safe = cleanScalar(value);
  if (safe === '') return;
  FormSetField(FORM_SCR_TAG, field, safe, 'SCR');
  try {
    FormSetField(TABLE_VIEW, field, safe, 'SCR');
  } catch (_) {
    /* view tag may differ on some shells */
  }
}

function isValidObservationYear(y) {
  const s = cleanScalar(y);
  if (!s || s.toUpperCase() === 'YYYY') return false;
  return /^\d{4}$/.test(s);
}

function observationNoNeedsAssignment(noStr) {
  const s = cleanScalar(noStr);
  if (s === '') return true;
  if (!/^\d+$/.test(s)) return true;
  return parseInt(s, 10) <= 0;
}

/**
 * @returns {Promise<number|null>} Next serial, or null if SQL failed (do not guess 1 — avoids duplicate key).
 */
async function getNextObservationNoForYear(executeSQLPromise, yearStr, getValFromRecordSet) {
  const yRaw = cleanScalar(yearStr);
  if (!/^\d{4}$/.test(yRaw)) return null;
  const y = escSql(yRaw);
  for (const tbl of PHYS_TABLE_CANDIDATES) {
    const sql = `SELECT ISNULL(MAX(NRSTMISCENT_NO)+1,1) AS New_Serial FROM ${tbl} WHERE NRSTMISCENT_YR = '${y}'`;
    try {
      const data = await executeSQLPromise(sql);
      const val =
        data?.recordset?.[0] && typeof getValFromRecordSet === 'function'
          ? getValFromRecordSet(data, 'New_Serial')
          : data?.recordset?.[0]?.New_Serial;
      const n = parseInt(val, 10);
      if (Number.isFinite(n) && n > 0) return n;
    } catch (e) {
      console.warn('[Web_HSE] Observation Entry getNextObservationNoForYear:', tbl, e);
    }
  }
  return null;
}

function mergeObservationYrNoIntoFullRecord(dev, fullRec) {
  if (!fullRec || typeof fullRec !== 'object') return;
  const yr = formGetObs(dev, 'NRSTMISCENT_YR');
  const no = formGetObs(dev, 'NRSTMISCENT_NO');
  if (yr) setFullRecField(fullRec, 'NRSTMISCENT_YR', yr);
  if (no) setFullRecField(fullRec, 'NRSTMISCENT_NO', no);
}

/**
 * Pre-SAVE (new main record): ensure NRSTMISCENT_YR and NRSTMISCENT_NO match desktop serial rules.
 * @param {Record<string, unknown>} devInterfaceObj
 * @param {{ isNewMode?: boolean, fullRecord?: Record<string, unknown> }} eventObj
 */
export async function ensureObservationYearAndNoOnNewSave(devInterfaceObj, eventObj) {
  if (!eventObj?.isNewMode) return;
  const { executeSQLPromise, getValFromRecordSet } = devInterfaceObj || {};
  if (typeof executeSQLPromise !== 'function') return;

  let yearStr = formGetObs(devInterfaceObj, 'NRSTMISCENT_YR');
  if (!isValidObservationYear(yearStr)) {
    const fromDate = yearStringFromIssueDate(formGetObs(devInterfaceObj, 'NRSTMISCENT_ISUDAT'));
    yearStr = fromDate && isValidObservationYear(fromDate) ? fromDate : String(new Date().getFullYear());
    formSetObs(devInterfaceObj, 'NRSTMISCENT_YR', yearStr);
  }

  const currentNo = formGetObs(devInterfaceObj, 'NRSTMISCENT_NO');
  if (observationNoNeedsAssignment(currentNo)) {
    const next = await getNextObservationNoForYear(executeSQLPromise, yearStr, getValFromRecordSet);
    if (next != null) formSetObs(devInterfaceObj, 'NRSTMISCENT_NO', String(next));
  }

  mergeObservationYrNoIntoFullRecord(devInterfaceObj, eventObj.fullRecord);
}
