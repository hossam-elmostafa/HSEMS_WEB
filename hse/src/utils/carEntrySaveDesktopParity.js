import { getNextAllocatedCrentryCrn } from './carCrentrySerialAllocation.js';
import { buildCrentryTriggerContextBatchSql } from './carTriggerContextBatch.js';

/**
 * CAR Entry (HSE_TGCRENTRY) — desktop save parity with CNewCarEntryCategory::DisplayToolBarButtonClicked SAVE.
 * Application (hse) layer only; no WebInfra Client/Server changes.
 *
 * Pre-save (iComplete==0): seed ##TEMP_HSE_TABLE; if CRENTRY_CRYR empty, set year from CRENTRY_CRDT and next CRENTRY_CRN.
 * Post-save (iComplete==1): set CRENTRY_CRSTT = '01' for first save (RQ-SF-9-1-23_2_7).
 *
 * RQ_HSE_12_4_26_00_40 — GAP-5 / GAP-11: Pre-save still runs in a separate HTTP round-trip from WebInfra INSERT;
 * triggers on that INSERT may not see ##TEMP_HSE_TABLE. Mitigation: post-save UPDATE + ChangeEntityStatus run in one
 * batch after the same context seed; SESSION_CONTEXT is set in both phases. Triggers that only fire on INSERT
 * remain a known limitation without DB trigger changes or a single-connection save API.
 *
 * RQ_HSE_4_4_25_18_22 — CAR Entry web parity: save payload sanitize, fullRecord merge, PrmKy/year-serial post-save.
 */

const FORM_SCR_TAG = 'HSE_TgCrEntry';
const TABLE_MAIN = 'HSE_CRENTRY';

function escSql(v) {
  return String(v ?? '').replace(/'/g, "''");
}

/** Avoid sending JS undefined/null or the strings 'undefined'/'null' into SQL or form merge (int/varchar conversion errors). */
function cleanFormScalar(v) {
  if (v == null) return '';
  const s = String(v).trim();
  if (s === 'undefined' || s === 'null' || s === 'NaN') return '';
  return s;
}

function isStrictPositiveIntString(s) {
  const t = cleanFormScalar(s);
  return /^\d+$/.test(t) && parseInt(t, 10) > 0;
}

// RQ_HSE_4_4_25_18_22
function isBadSqlScalarString(s) {
  const t = String(s ?? '').trim();
  return t === '' || t === 'undefined' || t === 'null' || t === 'NaN';
}

/**
 * Mutates save payload so WebInfra `formatInsertStmnt` never turns JS undefined / string "undefined"
 * into SQL varchar 'undefined' (breaks INT columns). Also normalizes DBCombo-shaped `{ key, value }`.
 * RQ_HSE_4_4_25_18_22
 */
function sanitizeFullRecordForCarSave(fullRec) {
  if (!fullRec || typeof fullRec !== 'object') return;
  const keys = Object.keys(fullRec);
  for (const k of keys) {
    if (k === 'strWhereCondition') continue;
    if (k === 'undefined' || k === 'null') {
      delete fullRec[k];
      continue;
    }
    const v = fullRec[k];
    if (v === undefined) {
      fullRec[k] = null;
      continue;
    }
    if (v === null) continue;

    if (typeof v === 'number' && !Number.isFinite(v)) {
      fullRec[k] = null;
      continue;
    }

    if (typeof v === 'string') {
      if (isBadSqlScalarString(v)) fullRec[k] = null;
      continue;
    }

    if (Array.isArray(v)) {
      const next = v
        .map((item) => {
          if (item === undefined || item === null) return null;
          if (typeof item === 'string' && isBadSqlScalarString(item)) return null;
          return item;
        })
        .filter((item) => item !== null && item !== undefined && item !== '');
      fullRec[k] = next.length ? next : null;
      continue;
    }

    if (typeof v === 'object' && Object.getPrototypeOf(v) === Object.prototype) {
      if (Object.prototype.hasOwnProperty.call(v, 'key')) {
        const rawKey = v.key;
        if (rawKey === undefined || rawKey === null || isBadSqlScalarString(String(rawKey))) {
          fullRec[k] = null;
        }
      }
    }
  }
}

/** Match screen JSON key casing (e.g. CRENTRY_CRYR vs mixed) when writing into `fullRecord`. RQ_HSE_4_4_25_18_22 */
function setFullRecField(fullRec, logicalName, value) {
  if (!fullRec || value == null || cleanFormScalar(value) === '') return;
  const upper = logicalName.toUpperCase();
  const existingKey = Object.keys(fullRec).find((key) => key.toUpperCase() === upper);
  fullRec[existingKey || upper] = value;
}

/** After FormSetField / async state, copy header fields into the same object sent as `fullRecord` on save. RQ_HSE_4_4_25_18_22 */
function mergeCrentryFormIntoFullRecord(devInterfaceObj, fullRec) {
  if (!fullRec || typeof fullRec !== 'object') return;
  const { FormGetField } = devInterfaceObj || {};
  if (typeof FormGetField !== 'function') return;

  const fields = ['CRENTRY_CRYR', 'CRENTRY_CRN', 'CRENTRY_ST', 'CRENTRY_DPR', 'CRENTRY_CRDT'];
  for (const logical of fields) {
    let v = FormGetField(TABLE_MAIN, logical, 'SCR');
    if (v == null || cleanFormScalar(v) === '') v = FormGetField(FORM_SCR_TAG, logical, 'SCR');
    const s = cleanFormScalar(v);
    if (s === '') continue;
    setFullRecField(fullRec, logical, v);
  }
}

/** @param {unknown} dateStr */
function yearStringFromCrentryDate(dateStr) {
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

function formGetCrentry(dev, field) {
  const { FormGetField } = dev || {};
  if (typeof FormGetField !== 'function') return '';
  let v = FormGetField(TABLE_MAIN, field, 'SCR');
  if (v == null || cleanFormScalar(v) === '') v = FormGetField(FORM_SCR_TAG, field, 'SCR');
  return cleanFormScalar(v);
}

function formSetCrentry(dev, field, value, srcType = 'SCR') {
  const { FormSetField } = dev || {};
  if (typeof FormSetField !== 'function') return;
  const safe = cleanFormScalar(value);
  if (safe === '') return;
  FormSetField(FORM_SCR_TAG, field, safe, srcType);
  try {
    FormSetField(TABLE_MAIN, field, safe, srcType);
  } catch (_) {
    /* table tag may not be bound on all shells */
  }
}


async function getNextCarSerialForYear(executeSQLPromise, yearStr, getValFromRecordSet) {
  // RQ_HSE_12_4_26_00_40 — M6: shared allocator (MAX+1 + skip occupied)
  const s = await getNextAllocatedCrentryCrn(executeSQLPromise, yearStr, getValFromRecordSet);
  const n = parseInt(s, 10);
  return Number.isFinite(n) && n > 0 ? n : 1;
}

/**
 * Desktop SAVE before persist: global temp + UpdateYearAndSerial when year blank.
 * RQ_HSE_4_4_25_18_22
 * @param {Record<string, unknown>} devInterfaceObj
 */
async function runCarEntryPreSaveDesktopParity(devInterfaceObj, eventObj) {
  const { executeSQLPromise, getValFromRecordSet } = devInterfaceObj || {};
  if (typeof executeSQLPromise !== 'function') return;

  sanitizeFullRecordForCarSave(eventObj?.fullRecord);

  // RQ_HSE_12_4_26_00_40 — GAP-5: shared batch builder (same shape as client-side INSERT paths)
  const batch = buildCrentryTriggerContextBatchSql(
    (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '',
    'CAR Entry',
    'Creating new CAR'
  );

  try {
    await executeSQLPromise(batch);
  } catch (e) {
    console.warn('[Web_HSE] CAR Entry pre-save ##TEMP_HSE_TABLE / SESSION_CONTEXT:', e);
  }

  const cryr = formGetCrentry(devInterfaceObj, 'CRENTRY_CRYR');
  if (cryr.trim() === '') {
    const crdt = formGetCrentry(devInterfaceObj, 'CRENTRY_CRDT');
    const strYear = yearStringFromCrentryDate(crdt);
    if (strYear) {
      formSetCrentry(devInterfaceObj, 'CRENTRY_CRYR', strYear, 'SCR');
      const nextSerial = await getNextCarSerialForYear(
        executeSQLPromise,
        strYear,
        getValFromRecordSet
      );
      formSetCrentry(devInterfaceObj, 'CRENTRY_CRN', String(nextSerial), 'SCR');
      setFullRecField(eventObj?.fullRecord, 'CRENTRY_CRYR', strYear);
      setFullRecField(eventObj?.fullRecord, 'CRENTRY_CRN', String(nextSerial));
    }
  }

  mergeCrentryFormIntoFullRecord(devInterfaceObj, eventObj?.fullRecord);
  sanitizeFullRecordForCarSave(eventObj?.fullRecord);
}

function prmkyFromSavedRow(row) {
  if (!row || typeof row !== 'object') return '';
  const candidates = [
    row.PRMKY,
    row.PrmKy,
    row.prmky,
    row.PrmKY,
    row.pRMKY,
  ];
  for (const v of candidates) {
    const s = cleanFormScalar(v);
    if (isStrictPositiveIntString(s)) return s;
  }
  for (const [k, v] of Object.entries(row)) {
    if (String(k).toUpperCase() === 'PRMKY') {
      const s = cleanFormScalar(v);
      if (isStrictPositiveIntString(s)) return s;
    }
  }
  return '';
}

/** Client save callback sometimes passes wrong shape (`recordset` vs `recordsets`); normalize to one row object. RQ_HSE_4_4_25_18_22 */
function normalizeSavedRowFromEvent(eventObj) {
  const fr = eventObj?.fullRecord;
  if (fr == null) return null;
  if (Array.isArray(fr)) return fr[0] && typeof fr[0] === 'object' ? fr[0] : null;
  if (typeof fr === 'object') return fr;
  return null;
}

/**
 * Resolve PrmKy for post-save UPDATE. WebInfra `handleSaveBtnClk` may set `fullRecord` from `data.recordset[0]`
 * while the driver returns the inserted row on `recordsets[last][0]` — so PK is often missing unless we recover it.
 * RQ_HSE_4_4_25_18_22
 */
function resolvePrmkyForCarPostSave(devInterfaceObj, eventObj) {
  const row = normalizeSavedRowFromEvent(eventObj);
  let pk = prmkyFromSavedRow(row);
  if (pk) return pk;

  const { FormGetField } = devInterfaceObj || {};
  if (typeof FormGetField !== 'function') return '';

  const tags = [FORM_SCR_TAG, TABLE_MAIN];
  const srcTypes = ['SCR'];
  const flds = ['PRMKY', 'PrmKy', 'prmky'];
  for (const tag of tags) {
    for (const st of srcTypes) {
      for (const f of flds) {
        const v = FormGetField(tag, f, st);
        pk = prmkyFromSavedRow({ PRMKY: v });
        if (pk) return pk;
      }
    }
  }
  return '';
}

/**
 * When PrmKy is not available, same natural key as pre-save year/serial (C++ UpdateYearAndSerial). RQ_HSE_4_4_25_18_22
 * @param {string} [contextPrefixSql] RQ_HSE_12_4_26_00_40 — GAP-5: same-connection trigger context before UPDATE
 */
async function updateCarEntryStatusByYearSerial(executeSQLPromise, cryr, crn, contextPrefixSql = '') {
  const y = cleanFormScalar(cryr);
  const n = cleanFormScalar(crn);
  if (!/^\d{1,4}$/.test(y) || !/^\d+$/.test(n)) return;
  await executeSQLPromise(
    `${contextPrefixSql}UPDATE HSE_CRENTRY SET CRENTRY_CRSTT = '01' WHERE CRENTRY_CRYR = '${escSql(y)}' AND CRENTRY_CRN = '${escSql(n)}'`
  );
}

/**
 * Desktop SAVE after persist: CRENTRY_CRSTT = '01' for new record (comment RQ-SF-9-1-23_2_7).
 * CHSEMSCommonCategory::insertIntoTracing has no HSE_TGCRENTRY branch — no InsertRecIntoTracingTab CREATED on desktop.
 * RQ_HSE_4_4_25_18_22
 * @param {Record<string, unknown>} devInterfaceObj
 * @param {{ isNewMode?: boolean, fullRecord?: Record<string, unknown> }} eventObj
 */
async function runCarEntryPostSaveDesktopParity(devInterfaceObj, eventObj) {
  if (!eventObj?.isNewMode) return;
  const { executeSQLPromise, FormSetField } = devInterfaceObj || {};
  const pk = resolvePrmkyForCarPostSave(devInterfaceObj, eventObj);
  const unRaw = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const ctxSql = buildCrentryTriggerContextBatchSql(unRaw, 'CAR Entry', 'Creating new CAR');

  if (typeof executeSQLPromise === 'function') {
    let updatedDb = false;
    if (pk) {
      const pkNum = parseInt(pk, 10);
      if (Number.isFinite(pkNum) && pkNum > 0) {
        try {
          // RQ_HSE_12_4_26_00_40 — GAP-5: context + UPDATE in one batch (same connection as UPDATE trigger)
          await executeSQLPromise(
            `${ctxSql}UPDATE HSE_CRENTRY SET CRENTRY_CRSTT = '01' WHERE PrmKy = ${pkNum}`
          );
          updatedDb = true;
          // RQ_HSE_12_4_26_00_40 — GAP-20: tracing (separate batch so ChangeEntityStatus failure does not roll back UPDATE)
          try {
            const escUser = escSql(unRaw);
            await executeSQLPromise(
              `${ctxSql}EXECUTE ChangeEntityStatus '${pkNum}','CAR Entry','${escUser}','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','01','Creating new CAR'`
            );
          } catch (traceErr) {
            console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-20 first-save tracing:', traceErr);
          }
        } catch (e) {
          console.warn('[Web_HSE] CAR Entry post-save CRENTRY_CRSTT UPDATE:', e);
        }
      }
    }
    if (!updatedDb) {
      const cryr = formGetCrentry(devInterfaceObj, 'CRENTRY_CRYR');
      const crn = formGetCrentry(devInterfaceObj, 'CRENTRY_CRN');
      try {
        await updateCarEntryStatusByYearSerial(executeSQLPromise, cryr, crn, ctxSql);
      } catch (e) {
        console.warn('[Web_HSE] CAR Entry post-save CRENTRY_CRSTT by year/serial:', e);
      }
    }
  }

  if (typeof FormSetField === 'function') {
    FormSetField(FORM_SCR_TAG, 'CRENTRY_CRSTT', '01', 'SCR');
    try {
      FormSetField(TABLE_MAIN, 'CRENTRY_CRSTT', '01', 'SCR');
    } catch (_) {}
  }

  if (typeof devInterfaceObj?.refreshData === 'function') {
    try {
      devInterfaceObj.refreshData('');
    } catch (_) {
      /* ignore */
    }
  }
}

/** Used by `CAR_Entry.js` → `toolBarButtonClicked` (main SAVE). RQ_HSE_4_4_25_18_22 */
export const carEntryDesktopSave = {
  preSave: runCarEntryPreSaveDesktopParity,
  postSave: runCarEntryPostSaveDesktopParity,
};
