/**
 * Screen handler: Site Survey Entry (HSE_TgSitSrvyEnt)
 * Menu path: Performance -> Site Survey -> Site Survey Entry (from HSE.json)
 * C++: SitSrvyEntCategory + SitSrvyCategory – SITSRVYENT_ENTCMPLT (CompleteSitSrvy), SITSRVYRQRDACTN_RQRDACTNBTN (Open Required Actions).
 * RQ_HSE_23_3_26_22_44: SITSRVYENT_RJCT handled in ModuleButtonHandlers (reject reason on Confirmation/Follow-up only on desktop; Entry uses direct SP if RJCT appears).
 */

import { getCurrentRecordPRMKY } from '../../../utils/rejectReasonUtils.js';
import { openSiteSurveyRequiredActionsPopup } from './siteSurveyRequiredActions.js';
import { applyFindingLineSerialIfEmpty } from './siteSurveySerialUtils.js';

const TABLE_MAIN = 'HSE_SITSRVYENT';
const KEY_FIELD = 'SITSRVYENT_SITSRVYNO';

// RQ_HSE_23_3_26_22_44: SubFieldChanged (findings line serial — SitSrvyCategory::DisplayFieldChange)
let _devInterfaceObj = {};

function getKeyFromForm(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_MAIN, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function getKeyFromEvent(eventObj, devInterfaceObj) {
  const fromForm = getKeyFromForm(devInterfaceObj);
  if (fromForm) return fromForm;
  return getCurrentRecordPRMKY(eventObj) || '';
}

function hasRecord(eventObj, devInterfaceObj) {
  const key = getKeyFromForm(devInterfaceObj);
  if (key) return true;
  const pk = getCurrentRecordPRMKY(eventObj);
  if (pk) return true;
  const arr = eventObj?.fullRecord ?? eventObj?.fullRecordArr;
  if (Array.isArray(arr) && arr.length > 0) return true;
  if (arr && typeof arr === 'object' && !Array.isArray(arr)) return true;
  return false;
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

const SCREEN_CAPTION = 'Site Survey Entry';

/**
 * SITSRVYENT_ENTCMPLT: Validate record saved, then EXECUTE completeSitSrvyTXN key, screenCaption, userName.
 * C++: Count check, then CompleteSitSrvy(KeyFieldValue, GetScrCptnByTag(66, Form_Tag, ""), GetUserLogin()).
 */
async function handleEntryComplete(eventObj, devInterfaceObj) {
  const { executeSQLPromise, refreshData, getUserName, AskYesNoMessage } = devInterfaceObj || {};
  if (!hasRecord(eventObj, devInterfaceObj)) {
    if (typeof AskYesNoMessage === 'function') AskYesNoMessage('You must save the Record first');
    return;
  }
  const key = getKeyFromEvent(eventObj, devInterfaceObj);
  if (!key) {
    if (typeof AskYesNoMessage === 'function') AskYesNoMessage('You must save the Record first');
    return;
  }
  if (!executeSQLPromise) return;
  const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const escKey = escapeSqlString(key);
  const escCaption = escapeSqlString(SCREEN_CAPTION);
  const escUser = escapeSqlString(userName);
  try {
    await executeSQLPromise(`EXECUTE completeSitSrvyTXN '${escKey}','${escCaption}','${escUser}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Site Survey Entry Complete:', e);
    if (AskYesNoMessage) AskYesNoMessage('Error completing site survey');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === 'SITSRVYENT_ENTCMPLT') {
    await handleEntryComplete(eventObj, devInterfaceObj);
    return;
  }
  if (btn === 'SITSRVYRQRDACTN_RQRDACTNBTN') {
    openSiteSurveyRequiredActionsPopup(devInterfaceObj, strScrTag);
  }
}

/**
 * C++ SitSrvyCategory::DisplayFieldChange: any field name containing SITSRVYENTFNDNG
 * and empty SITSRVYENTFNDNG_SRIL → getNewSerialNoForATab on HSE_SITSRVYENTFNDNG.
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  if (!dev?.FormGetField) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();
  if (fld.indexOf('SITSRVYENTFNDNG') < 0) return { cancel: 0 };

  const serialVal = String(dev.FormGetField('HSE_SITSRVYENTFNDNG', 'SITSRVYENTFNDNG_SRIL', 'tab') ?? '').trim();
  if (serialVal !== '') return { cancel: 0 };

  await applyFindingLineSerialIfEmpty(dev);
  return { cancel: 0 };
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
