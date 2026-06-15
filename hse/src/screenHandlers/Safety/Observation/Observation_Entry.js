/**
 * Screen handler: Observation Entry (HSE_TgNrstMiscEnt / HSE_TGNRSTMISCENT)
 * Menu path: Safety -> Observation -> Observation Entry (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
 * C++: When NEW is clicked on Attachments tab, set next Line No. (getNxtSrl) via HSEMSCommonCategory.
 * RQ_HSE_4_4_25_18_22 — Created From (NRSTMISCENT_CREATFRM) on NEW / SAVE pre; getDevInterface on toolbar.
 * RQ_HSE_5_4_26_14_19 — Comments tab Source Screen = getScreenCaption (SAVE pre / post-NEW).
 */

import {
  sendButtonClickToBackend,
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
  // RQ_HSE_5_4_26_14_19
  applyObservationCommentsSourceScreen,
} from '../../../services/Observation service/ObservationService.js';
import { mapUnknownFieldTypes } from '../../../utils/fieldTypeMapper.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';
// RQ_HSE_4_4_25_18_22
import {
  applyObservationEntryCreatedFrom,
  ensureObservationCreatFrmScalarInFullRecord,
} from '../../../utils/observationEntryCreatedFrom.js';
// RQ_HSE_4_4_25_18_22
import { ensureObservationYearAndNoOnNewSave } from '../../../utils/observationEntryObservationNo.js';
// RQ_HSE_4_4_25_18_22
import { getDevInterface } from '../../../events/buttonEvents.js';

const TABLE_MAIN_VIEW = 'HSE_vwNRSTMISCENT';
const MAIN_KEY_FIELD = 'NrstMiscEnt_NrstMiscNum';

// RQ_HSE_25_3_26_10_30 — Observation EN→AR: NRSTMISCENT_NRSTMISCDESC → API translate → NRSTMISCENT_RPRDSC
const OBS_TRANSLATE_FROM = 'English';
const OBS_TRANSLATE_TO = 'Arabic';

/** Session LRU: same English text → reuse last API result (no round-trip). */
const OBS_TRANSLATE_CLIENT_CACHE_MAX = 80;
const obsTranslateClientCache = new Map();

function obsTranslateClientCacheGet(key) {
  if (!obsTranslateClientCache.has(key)) return undefined;
  const v = obsTranslateClientCache.get(key);
  obsTranslateClientCache.delete(key);
  obsTranslateClientCache.set(key, v);
  return v;
}

function obsTranslateClientCacheSet(key, data) {
  if (obsTranslateClientCache.has(key)) obsTranslateClientCache.delete(key);
  obsTranslateClientCache.set(key, data);
  while (obsTranslateClientCache.size > OBS_TRANSLATE_CLIENT_CACHE_MAX) {
    obsTranslateClientCache.delete(obsTranslateClientCache.keys().next().value);
  }
}

/** Keep a reference to the real devInterface (client passes appGlobalVal to SubFieldChanged). */
let _devInterfaceObj = {};

/** Tab tags and config for NEW serial (C++ HSEMSCommonCategory getNxtSrl). */
const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGNRSTMISCENTATCH: {
    tabTable: 'HSE_NrstMiscEntAtch',
    serialField: 'NRSTMISCENTATCH_SRLNO',
    opts: {
      tableName: 'HSE_NrstMiscEntAtch',
      serialFieldName: 'NRSTMISCENTATCH_SRLNO',
      linkFieldName: 'NRSTMISCENTATCH_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE: {
    tabTable: 'HSE_NRSTMISCENTPSSBLNRSTMISCCUSE',
    serialField: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM',
    opts: {
      tableName: 'HSE_NRSTMISCENTPSSBLNRSTMISCCUSE',
      serialFieldName: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM',
      linkFieldName: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGNRSTMISCENTRQRDACC: {
    tabTable: 'HSE_NRSTMISCENTRQDACTN',
    serialField: 'NRSTMISCENTRQDACTN_ACTNNO',
    opts: {
      tableName: 'HSE_NRSTMISCENTRQDACTN',
      serialFieldName: 'NRSTMISCENTRQDACTN_ACTNNO',
      linkFieldName: 'NRSTMISCENTRQDACTN_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
};

/**
 * Handle custom button clicks on Observation Entry (Reject, Confirm, Cancel, Close, Entry Complete, etc.).
 * Delegates to ObservationService.sendButtonClickToBackend.
 */
export function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag } = eventObj || {};
  const devInterface = eventObj.devInterfaceObj || {};
  _devInterfaceObj = devInterface || _devInterfaceObj;
  const buttonName = Button_Name ? Button_Name.toString().toUpperCase() : '';
  const screenTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  sendButtonClickToBackend(buttonName, screenTag, eventObj, devInterface);
}

/**
 * Handle toolbar button clicks on Observation Entry (tab enabling after save, Comments tab management).
 * When NEW is clicked on Attachments tab, sets next Line No. (C++: getNxtSrl + FormSetField for HSE_TGNRSTMISCENTATCH).
 * RQ_HSE_4_4_25_18_22 — Created From phases + getDevInterface fallback
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || getDevInterface() || {};
  _devInterfaceObj = devInterface || _devInterfaceObj;
  let strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;
  const completeStr = complete == null ? '' : String(complete).trim();
  // RQ_HSE_4_4_25_18_22
  const isPostPhase = complete === 1 || completeStr === '1';
  const isPrePhase =
    !isPostPhase &&
    (complete == null ||
      complete === '' ||
      complete === 0 ||
      complete === false ||
      completeStr === '0');

  // RQ_HSE_4_4_25_18_22
  try {
    if (strBtnName === 'NEW' && isPostPhase && !strTabTag && strScrTag === 'HSE_TGNRSTMISCENT') {
      applyObservationEntryCreatedFrom(devInterface, eventObj);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Entry NEW Created From:', e);
  }

  // RQ_HSE_4_4_25_18_22 — flatten Combo object on fullRecord before save INSERT (smallint column)
  try {
    if (strBtnName === 'SAVE' && isPrePhase && !strTabTag && strScrTag === 'HSE_TGNRSTMISCENT' && eventObj.isNewMode) {
      ensureObservationCreatFrmScalarInFullRecord(eventObj?.fullRecord);
      applyObservationEntryCreatedFrom(devInterface, eventObj);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Entry SAVE pre Created From:', e);
  }

  // RQ_HSE_4_4_25_18_22 — NRSTMISCENT_YR / NRSTMISCENT_NO (desktop getNxtSrl)
  try {
    if (strBtnName === 'SAVE' && isPrePhase && !strTabTag && strScrTag === 'HSE_TGNRSTMISCENT' && eventObj.isNewMode) {
      await ensureObservationYearAndNoOnNewSave(devInterface, eventObj);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Entry SAVE pre observation year/no:', e);
  }

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'SAVE' && isPrePhase && strTabTag && strTabTag.includes('CMNTS') && eventObj.isNewMode) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, eventObj?.fullRecord);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Entry SAVE pre Comments SRCSCRN:', e);
  }

  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) {
        await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry toolBarButtonClicked tab NEW serial:', error);
  }

  try {
    if (complete === 1) {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
      if (strTabTag && strTabTag.includes('CMNTS')) {
        manageCommentsTabToolBar(strScrTag, strTabTag, devInterface);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'NEW' && isPostPhase && strTabTag && strTabTag.includes('CMNTS')) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, undefined);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Entry post-callback Comments SRCSCRN:', e);
  }
}

/**
 * Handle ShowScreen for Observation Entry: set button state, map unknown field types, enable observation tabs on main screen.
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  _devInterfaceObj = devInterface || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Entry ShowScreen: error enabling observation tabs:', error);
    }
  }
}

// RQ_HSE_25_3_26_10_00 — local English normalize fallback when AI rephrase/translate is unavailable or empty
function autoRephraseObservationDescription(value) {
  const raw = String(value ?? '');
  let s = raw.replace(/\s+/g, ' ').trim();
  if (!s) return '';
  s = s.charAt(0).toUpperCase() + s.slice(1);
  if (s.length >= 25 && !/[.!?]$/.test(s)) s += '.';
  return s;
}

// RQ_HSE_25_3_26_10_30 — API base for fetch() fallback when devInterface.aiTranslatePromise is not used
function resolveApiBaseForAi(dev) {
  if (typeof dev?.getApiBaseUrl === 'function') {
    const b = String(dev.getApiBaseUrl() || '').trim().replace(/\/$/, '');
    if (b) return b;
  }
  if (typeof process !== 'undefined' && process.env?.REACT_APP_SERVER_URL) {
    const b = String(process.env.REACT_APP_SERVER_URL).trim().replace(/\/$/, '');
    if (b) return b;
  }
  if (typeof window !== 'undefined' && window.location) {
    const { protocol, hostname, port } = window.location;
    const p = String(port || '');
    if (p === '3000' || p === '5173') {
      return `${protocol}//${hostname}:5000`;
    }
    return String(window.location.origin || '').replace(/\/$/, '');
  }
  return '';
}

// RQ_HSE_25_3_26_10_00 — POST /util/ai/rephrase (same patterns as requestAiTranslateEnToAr)
async function requestAiRephrase(text, dev) {
  if (typeof dev?.aiRephrasePromise === 'function') {
    return dev.aiRephrasePromise(text);
  }
  const apiBase = resolveApiBaseForAi(dev);
  if (!apiBase) {
    throw new Error('AI rephrase: could not resolve API base URL.');
  }
  const fullUrl = new URL('/util/ai/rephrase', apiBase).href;
  const headers = { 'Content-Type': 'application/json' };
  const token = typeof dev?.getAccessToken === 'function' ? dev.getAccessToken() : '';
  if (token) headers.Authorization = `Bearer ${token}`;
  const response = await fetch(fullUrl, {
    method: 'POST',
    headers,
    credentials: 'include',
    body: JSON.stringify({ params: { text } }),
  });
  if (!response.ok) {
    let msg = '';
    try {
      const errJson = await response.json();
      msg = errJson?.userError || errJson?.error || '';
    } catch {
      msg = '';
    }
    throw new Error(`AI rephrase request failed (${response.status})${msg ? `: ${msg}` : ''}`);
  }
  return response.json();
}

// RQ_HSE_25_3_26_10_30
async function requestAiTranslateEnToAr(text, dev) {
  const key = String(text ?? '').trim();
  const cached = obsTranslateClientCacheGet(key);
  if (cached) return cached;

  const opts = { from: OBS_TRANSLATE_FROM, to: OBS_TRANSLATE_TO };
  if (typeof dev?.aiTranslatePromise === 'function') {
    const data = await dev.aiTranslatePromise(text, opts);
    obsTranslateClientCacheSet(key, data);
    return data;
  }
  const apiBase = resolveApiBaseForAi(dev);
  if (!apiBase) {
    throw new Error('AI translate: could not resolve API base URL.');
  }
  const fullUrl = new URL('/util/ai/translate', apiBase).href;
  const headers = { 'Content-Type': 'application/json' };
  const token = typeof dev?.getAccessToken === 'function' ? dev.getAccessToken() : '';
  if (token) headers.Authorization = `Bearer ${token}`;
  const payload = { params: { text, ...opts } };
  const response = await fetch(fullUrl, {
    method: 'POST',
    headers,
    credentials: 'include',
    body: JSON.stringify(payload),
  });
  if (!response.ok) {
    let msg = '';
    try {
      const errJson = await response.json();
      msg = errJson?.userError || errJson?.error || '';
    } catch {
      msg = '';
    }
    throw new Error(`AI translate request failed (${response.status})${msg ? `: ${msg}` : ''}`);
  }
  const data = await response.json();
  obsTranslateClientCacheSet(key, data);
  return data;
}

/**
 * On change to main-form NRSTMISCENT_NRSTMISCDESC, set NRSTMISCENT_RPRDSC from AI rephrase (POST /util/ai/rephrase).
 * For EN→AR instead, call requestAiTranslateEnToAr and read data.translated.
 * On failure or empty AI text, falls back to local English normalize helper.
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const formDev =
    devInterfaceObj && typeof devInterfaceObj.FormGetField === 'function' ? devInterfaceObj : _devInterfaceObj;
  const { FormGetField, FormSetField } = formDev;
  if (!FormGetField || !FormSetField) return { cancel: 0 };

  const apiDev = {
    ...(devInterfaceObj && typeof devInterfaceObj === 'object' ? devInterfaceObj : {}),
    ..._devInterfaceObj,
  };

  const tab = String(strTabTag ?? '').toUpperCase().trim();
  if (tab) return { cancel: 0 };

  const fld = String(fieldName ?? '').toUpperCase().trim();
  if (fld !== 'NRSTMISCENT_NRSTMISCDESC') return { cancel: 0 };

  const src = String(fieldVal ?? FormGetField(TABLE_MAIN_VIEW, 'NRSTMISCENT_NRSTMISCDESC', 'scr') ?? '').trim();
  if (!src) return { cancel: 0 };

  const oldTrim = String(oldFieldVal ?? '').trim();
  if (oldTrim === src) return { cancel: 0 };

  try {
    const data = await requestAiRephrase(src, apiDev);
    // Server returns { rephrased } for /util/ai/rephrase; { translated } for /util/ai/translate
    const outText = String(data?.rephrased ?? '').trim();
    if (outText) FormSetField(TABLE_MAIN_VIEW, 'NRSTMISCENT_RPRDSC', outText, 'scr');
    else FormSetField(TABLE_MAIN_VIEW, 'NRSTMISCENT_RPRDSC', autoRephraseObservationDescription(src), 'scr');
  } catch (e) {
    try {
      FormSetField(TABLE_MAIN_VIEW, 'NRSTMISCENT_RPRDSC', autoRephraseObservationDescription(src), 'scr');
    } catch (e2) {
      console.warn('[Web_HSE] Observation Entry translate / fallback:', e2);
    }
  }

  return { cancel: 0 };
}
