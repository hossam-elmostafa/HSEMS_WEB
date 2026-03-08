/**
 * Screen handler: Site Survey Entry (HSE_TgSitSrvyEnt)
 * Menu path: Performance -> Site Survey -> Site Survey Entry (from HSE.json)
 * C++: SitSrvyEntCategory + SitSrvyCategory – SITSRVYENT_ENTCMPLT (CompleteSitSrvy), SITSRVYENT_RJCT (rejectRecord + rejectSitSrvy), SITSRVYRQRDACTN_RQRDACTNBTN (Open Required Actions).
 */

import { getCurrentRecordPRMKY } from '../../../utils/rejectReasonUtils.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const TABLE_MAIN = 'HSE_SITSRVYENT';
const KEY_FIELD = 'SITSRVYENT_SITSRVYNO';
const TABLE_FNDNG = 'HSE_SITSRVYENTFNDNG';
const FNDNG_LINK = 'MAINLINK';
const POPUP_TAG = 'HSE_TgSitSrvyRqrdActn';
const SCREEN_CAPTION = 'Site Survey Entry';

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

/**
 * SITSRVYRQRDACTN_RQRDACTNBTN: Open Required Actions popup for current finding row.
 * C++: strKeyField = FormGetField("HSE_SITSRVYENTFNDNG","MAINLINK"); ShowScreen("HSE_TGSITSRVYRQRDACTN", ..., strSQL, defValues with SITSRVYRQRDACTN_LNK and TXNSTS|1).
 */
function handleRequiredActions(eventObj, devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof openScr !== 'function' || !FormGetField) return;
  const linkVal = FormGetField(TABLE_FNDNG, FNDNG_LINK, 'scr');
  if (linkVal == null || linkVal === '') return;
  const esc = escapeSqlString(String(linkVal).trim());
  const strCriteria = `SELECT * FROM HSE_SITSRVYRQRDACTN WHERE (SITSRVYRQRDACTN_LNK = '${esc}')`;
  const defValObj = { SITSRVYRQRDACTN_LNK: linkVal, TXNSTS: '1' };
  openScr(POPUP_TAG, {}, strCriteria, 'edit', false, defValObj, false, false);
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === 'SITSRVYENT_ENTCMPLT') {
    await handleEntryComplete(eventObj, devInterfaceObj);
    return;
  }
  if (btn === 'SITSRVYRQRDACTN_RQRDACTNBTN') {
    handleRequiredActions(eventObj, devInterfaceObj);
    return;
  }
  if (btn === 'SITSRVYENT_RJCT') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
