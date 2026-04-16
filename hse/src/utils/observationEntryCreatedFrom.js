/**
 * Observation Entry — "Created From" (NRSTMISCENT_CREATFRM) for web vs external entry.
 * HSE application layer only; no WebInfra changes.
 * RQ_HSE_4_4_25_18_22
 *
 * Codes (per product convention):
 * - 2 = Web Page (external observation entry page)
 * - 3 = System (main HSE web application)
 *
 * External entry signals (first match wins):
 * 1) URL query `obsCreatedFrom=webpage` (or `web`, `external`, `2`)
 * 2) sessionStorage key `HSE_OBSERVATION_ENTRY_CREATED_FROM` = `webpage` | `web` | `external` | `2`
 * 3) `window.__HSE_OBSERVATION_EXTERNAL_ENTRY__ === true`
 *
 * After applying code 2, sessionStorage flag is cleared so the next NEW from the internal app defaults to System (3).
 */

const STORAGE_KEY = 'HSE_OBSERVATION_ENTRY_CREATED_FROM';
const QUERY_KEYS = ['obsCreatedFrom', 'obsEntrySource'];

const FROM_WEB_PAGE = 2;
const FROM_SYSTEM = 3;

/**
 * Stored CODE for NRSTMISCENT_CREATFRM (smallint in DB). Must be a scalar for INSERT — server only unwraps
 * `{ key }` for DBCombo; Type "Combo" is sent as-is and becomes '[object Object]' in SQL. RQ_HSE_4_4_25_18_22
 */
function creatFrmScalarForCode(code) {
  const c = code === FROM_WEB_PAGE ? FROM_WEB_PAGE : FROM_SYSTEM;
  return String(c);
}

// RQ_HSE_4_4_25_18_22
function parseExternalSignal(raw) {
  const s = String(raw ?? '')
    .trim()
    .toLowerCase();
  if (!s) return false;
  return s === 'webpage' || s === 'web' || s === 'external' || s === '2' || s === 'true';
}

/**
 * @returns {typeof FROM_WEB_PAGE | typeof FROM_SYSTEM}
 * RQ_HSE_4_4_25_18_22
 */
export function getObservationEntryCreatedFromCode() {
  if (typeof window === 'undefined') return FROM_SYSTEM;
  try {
    const params = new URLSearchParams(window.location.search || '');
    for (const k of QUERY_KEYS) {
      if (parseExternalSignal(params.get(k))) return FROM_WEB_PAGE;
    }
    if (parseExternalSignal(window.sessionStorage?.getItem(STORAGE_KEY))) return FROM_WEB_PAGE;
    if (window.__HSE_OBSERVATION_EXTERNAL_ENTRY__ === true) return FROM_WEB_PAGE;
  } catch (_) {
    /* ignore */
  }
  return FROM_SYSTEM;
}

// RQ_HSE_4_4_25_18_22
function mergeIntoFullRecord(eventObj, fieldLogical, value) {
  const row = eventObj?.fullRecord;
  if (!row || typeof row !== 'object' || value == null) return;
  const u = fieldLogical.toUpperCase();
  const k = Object.keys(row).find((key) => key.toUpperCase() === u) || u;
  row[k] = value;
}

/**
 * If Created From was set as a Combo object, collapse to `.key` before save (avoids varchar '[object Object]' → smallint). RQ_HSE_4_4_25_18_22
 * @param {Record<string, unknown> | null | undefined} fullRec
 */
export function ensureObservationCreatFrmScalarInFullRecord(fullRec) {
  if (!fullRec || typeof fullRec !== 'object') return;
  const u = FIELD.toUpperCase();
  const k = Object.keys(fullRec).find((key) => key.toUpperCase() === u);
  if (!k) return;
  const v = fullRec[k];
  if (v && typeof v === 'object' && !Array.isArray(v) && Object.prototype.hasOwnProperty.call(v, 'key')) {
    const raw = v.key;
    fullRec[k] = raw == null ? '' : String(raw);
  }
}

const FORM_TAG = 'HSE_TgNrstMiscEnt';
const TABLE_VIEW = 'HSE_vwNRSTMISCENT';
const FIELD = 'NRSTMISCENT_CREATFRM';

/**
 * Sets locked combo Created From on Observation Entry header and syncs `eventObj.fullRecord` for save.
 * RQ_HSE_4_4_25_18_22
 * @param {Record<string, unknown>} devInterfaceObj
 * @param {{ fullRecord?: Record<string, unknown> } | undefined} eventObj
 */
export function applyObservationEntryCreatedFrom(devInterfaceObj, eventObj) {
  const { FormSetField } = devInterfaceObj || {};
  if (typeof FormSetField !== 'function') return;

  const code = getObservationEntryCreatedFromCode();
  // RQ_HSE_4_4_25_18_22 — scalar CODE only (Combo ≠ DBCombo on server insert path)
  const scalar = creatFrmScalarForCode(code);

  try {
    FormSetField(TABLE_VIEW, FIELD, scalar, 'scr');
  } catch (_) {
    /* table tag may differ */
  }
  try {
    FormSetField(FORM_TAG, FIELD, scalar, 'scr');
  } catch (_) {
    /* ignore */
  }

  mergeIntoFullRecord(eventObj, FIELD, scalar);

  if (code === FROM_WEB_PAGE) {
    try {
      window.sessionStorage?.removeItem(STORAGE_KEY);
    } catch (_) {
      /* ignore */
    }
  }
}
