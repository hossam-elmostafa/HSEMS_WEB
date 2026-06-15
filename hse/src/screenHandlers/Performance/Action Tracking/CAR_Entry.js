/**
 * Screen handler: CAR Entry (HSE_TgCrEntry)
 * Menu path: Performance -> Action Tracking -> CAR Entry (from HSE.json)
 * C++: NewCarEntryCategory – SAVE/NEW/field-change for year & serial, default site/dept on NEW.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { getNextSerialForTab } from '../../../utils/tabNewSerialUtils.js';

const SCREEN_TAG = 'HSE_TGCRENTRY';
const FORM_TABLE = 'HSE_TgCrEntry';
const TABLE_MAIN = 'HSE_CRENTRY';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

function isBlank(val) {
  if (val == null || val === undefined) return true;
  const s = String(val).trim();
  return s === '' || s.toLowerCase() === 'undefined';
}

/**
 * Extract calendar year from CAR Date (C++ COleDateTime on CRENTRY_CRDT).
 * @param {string} dateStr
 * @returns {string}
 */
function parseYearFromCarDate(dateStr) {
  if (isBlank(dateStr)) return '';
  const s = String(dateStr).trim();

  const dmy4 = s.match(/^(\d{1,2})\/(\d{1,2})\/(\d{4})$/);
  if (dmy4) return dmy4[3];

  // Client often sends DD/MM/YY e.g. 06/11/26 (DataMask DD/MM/YYYY but 2-digit year in UI)
  const dmy2 = s.match(/^(\d{1,2})\/(\d{1,2})\/(\d{2})$/);
  if (dmy2) {
    const yy = parseInt(dmy2[3], 10);
    return String(yy >= 70 ? 1900 + yy : 2000 + yy);
  }

  const iso = s.match(/^(\d{4})-(\d{2})-(\d{2})/);
  if (iso) return iso[1];

  const d = new Date(s);
  if (!Number.isNaN(d.getTime())) return String(d.getFullYear());
  return '';
}

function resolveCarYear(dateStr) {
  const fromDate = parseYearFromCarDate(dateStr);
  if (fromDate) return fromDate;
  return String(new Date().getFullYear());
}

/**
 * Get default site from policy (C++ GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST').
 * @param {Object} devInterface
 * @returns {Promise<string>}
 */
async function getDefaultSite(devInterface) {
  const { executeSQLPromise, getValFromRecordSet } = devInterface || {};
  if (!executeSQLPromise || !getValFromRecordSet) return '';
  try {
    const data = await executeSQLPromise("EXEC GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST'");
    const val = data?.recordset?.[0] ? getValFromRecordSet(data, 'HSEPLC_DFLST') : '';
    return val != null ? String(val).trim() : '';
  } catch (e) {
    console.warn('[Web_HSE] CAR Entry getDefaultSite:', e);
    return '';
  }
}

/**
 * Get first employee department for current user (C++ getEmpDep(..., true)).
 * @param {Object} devInterface
 * @returns {Promise<string>}
 */
async function getUserDepartment(devInterface) {
  const { executeSQLPromise, getValFromRecordSet, getUserName, getCurrentUserObj } = devInterface || {};
  if (!executeSQLPromise) return '';
  let userKey = '';
  if (typeof getUserName === 'function') {
    const n = getUserName();
    if (n != null && String(n).trim() !== '') userKey = String(n).trim();
  }
  if (!userKey) {
    const u = getCurrentUserObj?.();
    if (u?.LogIn != null && String(u.LogIn).trim() !== '') userKey = String(u.LogIn).trim();
  }
  if (!userKey) return '';
  try {
    const esc = escapeSqlString(userKey);
    const sql = `SELECT TOP 1 ISNULL(EMPLOYEE_WSHOP,'') AS W FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '${esc}' OR EMPLOYEE_LOGINNAME = '${esc}'`;
    const data = await executeSQLPromise(sql);
    const val = data?.recordset?.[0] && getValFromRecordSet ? getValFromRecordSet(data, 'W') : data?.recordset?.[0]?.W;
    const w = val != null ? String(val).trim() : '';
    if (!w) return '';
    const stx = String.fromCharCode(2);
    const parts = w.includes(stx) ? w.split(stx).map((p) => p.trim()).filter(Boolean) : [w];
    return parts[0] || '';
  } catch (e) {
    console.warn('[Web_HSE] CAR Entry getUserDepartment:', e);
    return '';
  }
}

function getRecordField(record, fieldName) {
  if (!record || typeof record !== 'object') return '';
  const u = String(fieldName).toUpperCase();
  for (const k of Object.keys(record)) {
    if (k.toUpperCase() === u) {
      const v = record[k];
      return v != null && String(v).toLowerCase() !== 'undefined' ? String(v).trim() : '';
    }
  }
  return '';
}

function setRecordField(record, fieldName, value) {
  if (!record || typeof record !== 'object') return;
  const u = String(fieldName).toUpperCase();
  const existingKey = Object.keys(record).find((k) => k.toUpperCase() === u);
  if (existingKey) record[existingKey] = value;
  else record[fieldName] = value;
}

/** Remove keys whose value is JS undefined or the string "undefined" (server INSERT would quote them). */
function removeUndefinedRecordFields(record) {
  if (!record || typeof record !== 'object') return;
  for (const key of Object.keys(record)) {
    const val = record[key];
    if (val === undefined || (typeof val === 'string' && val.toLowerCase() === 'undefined')) {
      delete record[key];
    }
  }
  // PrmKy omitted from JSON becomes PRMKY = 'undefined' in post-insert SELECT (UniqueFields = PrmKy)
  const pkKey = Object.keys(record).find((k) => k.toUpperCase() === 'PRMKY');
  if (pkKey && isBlank(record[pkKey])) delete record[pkKey];
}

/**
 * Resolve year + CAR No. from the save payload (and optional form fields).
 * @returns {Promise<{ year: string, carNo: string } | null>}
 */
async function resolveYearAndCarNo(record, devInterface) {
  const { FormGetField } = devInterface || {};
  if (!record || typeof record !== 'object') return null;

  let dateVal = getRecordField(record, 'CRENTRY_CRDT');
  if (isBlank(dateVal) && FormGetField) {
    dateVal = FormGetField(FORM_TABLE, 'CRENTRY_CRDT', 'scr');
  }

  let currentYear = getRecordField(record, 'CRENTRY_CRYR');
  if (isBlank(currentYear) && FormGetField) {
    const v = FormGetField(FORM_TABLE, 'CRENTRY_CRYR', 'scr');
    if (!isBlank(v)) currentYear = String(v).trim();
  }
  if (isBlank(currentYear)) currentYear = resolveCarYear(dateVal);

  let carNo = getRecordField(record, 'CRENTRY_CRN');
  if (isBlank(carNo) && FormGetField) {
    const v = FormGetField(FORM_TABLE, 'CRENTRY_CRN', 'scr');
    if (!isBlank(v)) carNo = String(v).trim();
  }
  if (isBlank(carNo)) {
    carNo = String(await getNextSerialForTab(devInterface, SERIAL_OPTS, currentYear));
  }
  return { year: currentYear, carNo };
}

const SERIAL_OPTS = {
  tableName: TABLE_MAIN,
  serialFieldName: 'CRENTRY_CRN',
  linkFieldName: 'CRENTRY_CRYR',
};

/**
 * Set CAR Year on the form only (no serial). Used on CAR Date field change so the
 * locked CAR No. stays EMPTY until save (web requirement TC-CARS-42); the next CAR No.
 * is assigned at save time in ensureYearAndSerialBeforeSave.
 * @param {Object} devInterface
 * @param {string} strYear
 */
function setCarYearOnly(devInterface, strYear) {
  const { FormSetField } = devInterface || {};
  if (!FormSetField || isBlank(strYear)) return;
  FormSetField(FORM_TABLE, 'CRENTRY_CRYR', String(strYear).trim(), 'scr');
}

/**
 * Set CAR Year and next CAR No. on the save payload and form (C++ UpdateYearAndSerial).
 * @param {Object} devInterface
 * @param {string} strYear
 * @param {Object} [record] - eventObj.fullRecord snapshot sent to saveRec
 */
async function applyYearAndSerial(devInterface, strYear, record) {
  const { FormSetField } = devInterface || {};
  if (isBlank(strYear)) return;
  const year = String(strYear).trim();
  const nextSerial = await getNextSerialForTab(devInterface, SERIAL_OPTS, year);
  const carNo = String(nextSerial);
  if (record) {
    setRecordField(record, 'CRENTRY_CRYR', year);
    setRecordField(record, 'CRENTRY_CRN', carNo);
  }
  if (FormSetField) {
    FormSetField(FORM_TABLE, 'CRENTRY_CRYR', year, 'scr');
    FormSetField(FORM_TABLE, 'CRENTRY_CRN', carNo, 'scr');
  }
  return { year, carNo };
}

/**
 * Seed tracing temp table before first save (C++ NewCarEntryCategory SAVE complete=0).
 * @param {Object} devInterface
 */
async function seedTracingTempTable(devInterface) {
  const { executeSQLPromise } = devInterface || {};
  if (!executeSQLPromise) return;
  try {
    await executeSQLPromise(
      "IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))"
    );
    await executeSQLPromise('DELETE FROM ##TEMP_HSE_TABLE');
    await executeSQLPromise("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('ActionDescription','Creating new CAR')");
    await executeSQLPromise("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('SrcScreen','CAR Entry')");
  } catch (e) {
    console.warn('[Web_HSE] CAR Entry seedTracingTempTable:', e);
  }
}

/**
 * Ensure year and CAR No. on the save payload before insert (desktop SAVE pre-hook).
 * Reassigns eventObj.fullRecord with a fresh object: the original may be a frozen
 * React state object (in-place mutation would throw / be ignored), and FormSetField
 * alone does not update the snapshot that saveRec sends to the server.
 * @param {Object} eventObj
 * @param {Object} devInterface
 */
async function ensureYearAndSerialBeforeSave(eventObj, devInterface) {
  const record = eventObj?.fullRecord;
  if (!record || typeof record !== 'object') return;

  const resolved = await resolveYearAndCarNo(record, devInterface);
  if (!resolved) return;

  // Clone so we never depend on mutating a (possibly frozen) state object.
  const nextRecord = { ...record };
  setRecordField(nextRecord, 'CRENTRY_CRYR', resolved.year);
  setRecordField(nextRecord, 'CRENTRY_CRN', resolved.carNo);
  removeUndefinedRecordFields(nextRecord);
  eventObj.fullRecord = nextRecord;

  const { FormSetField } = devInterface || {};
  if (FormSetField) {
    FormSetField(FORM_TABLE, 'CRENTRY_CRYR', resolved.year, 'scr');
    FormSetField(FORM_TABLE, 'CRENTRY_CRN', resolved.carNo, 'scr');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

/**
 * C++ NewCarEntryCategory::DisplayFieldChange – CRENTRY_CRDT sets CRENTRY_CRYR + CRENTRY_CRN.
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, _updatedAppGlobalVal, devInterfaceObj) {
  const scr = (strScrTag && strScrTag.toString().toUpperCase()) || '';
  if (scr !== SCREEN_TAG) return { cancel: 0 };
  if (strTabTag && String(strTabTag).trim() !== '') return { cancel: 0 };

  const fld = (fieldName && fieldName.toString().toUpperCase()) || '';
  if (fld !== 'CRENTRY_CRDT') return { cancel: 0 };

  const devInterface = devInterfaceObj || {};
  const { FormGetField } = devInterface;
  if (!FormGetField) return { cancel: 0 };

  try {
    const year = parseYearFromCarDate(fieldVal) || resolveCarYear(fieldVal);
    const currentYear = FormGetField(FORM_TABLE, 'CRENTRY_CRYR', 'scr');
    if (String(currentYear || '').trim() !== year) {
      // Year only — CAR No. stays empty/locked until save (assigned in the SAVE pre-hook).
      setCarYearOnly(devInterface, year);
    }
  } catch (error) {
    console.warn('[Web_HSE] CAR Entry SubFieldChanged:', error);
  }
  return { cancel: 0 };
}

/**
 * C++ NewCarEntryCategory::DisplayToolBarButtonClicked – NEW default site/dept; SAVE year/serial/status.
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  const strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  if (strScrTag === SCREEN_TAG) {
    try {
      const { FormSetField, FormGetField } = devInterface;

      if (complete === 1 && strBtnName === 'NEW' && strTabTag === '' && FormSetField) {
        const defaultSite = await getDefaultSite(devInterface);
        if (defaultSite) FormSetField(FORM_TABLE, 'CRENTRY_ST', defaultSite, 'scr');
        const userDep = await getUserDepartment(devInterface);
        if (userDep) FormSetField(FORM_TABLE, 'CRENTRY_DPR', userDep, 'scr');

        // CAR Year/No. are the screen UniqueFields, so the infra keeps the previously
        // selected row's values on NEW. Desktop starts a CAR with an empty CAR No.
        // (assigned at save) and the year derived from CAR Date — mirror that here.
        // Clear CAR No. LAST: setting a field to '' deletes it from selectedHeaderRow,
        // so any later field write must not re-merge a stale row that still holds it.
        const carDate = FormGetField ? FormGetField(FORM_TABLE, 'CRENTRY_CRDT', 'scr') : '';
        FormSetField(FORM_TABLE, 'CRENTRY_CRYR', resolveCarYear(carDate), 'scr');
        FormSetField(FORM_TABLE, 'CRENTRY_CRN', '', 'scr');
      }

      if (complete === 0 && strBtnName === 'SAVE' && strTabTag === '') {
        await seedTracingTempTable(devInterface);
        await ensureYearAndSerialBeforeSave(eventObj, devInterface);
      }

      if (complete === 1 && strBtnName === 'SAVE' && strTabTag === '' && FormSetField) {
        FormSetField(FORM_TABLE, 'CRENTRY_CRSTT', '01', 'scr');
      }
    } catch (error) {
      console.warn('[Web_HSE] CAR Entry toolBarButtonClicked:', error);
    }
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, _strScrTag, _strTabTag, _devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
