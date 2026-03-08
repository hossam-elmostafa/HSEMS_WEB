/**
 * Screen handler: Aspects Entry (HSE_AspctsEntryAtEntry)
 * Menu path: Environment -> Aspects Register -> Aspects Entry (from HSE.json)
 * C++: EnvrnmntAspctCategory – VIEW_HISTORY, VIEW_CURRENT_MONTH (change criteria), ENTRY_COMPLETED (EnvAspctEntryComplete).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const TABLE_VIEW = 'HSE_ASPCTS_VIEW';
const KEY_FIELD = 'PrmryKy';

const BTN_VIEW_HISTORY = 'VIEW_HISTORY';
const BTN_VIEW_CURRENT_MONTH = 'VIEW_CURRENT_MONTH';
const BTN_ENTRY_COMPLETED = 'ENTRY_COMPLETED';

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_VIEW, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * ENTRY_COMPLETED: EXECUTE EnvAspctEntryComplete '<PrmryKy>', then refresh.
 * C++: completeEntry(strPrmryKy);
 */
async function handleEntryCompleted(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE EnvAspctEntryComplete '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Aspects Entry Complete:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error completing entry');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_ENTRY_COMPLETED) {
    await handleEntryCompleted(devInterfaceObj);
    return;
  }
  if (btn === BTN_VIEW_HISTORY || btn === BTN_VIEW_CURRENT_MONTH) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
