/**
 * Screen handler: Aspects Entry (HSE_AspctsEntryAtEntry)
 * Menu path: Environment -> Aspects Register -> Aspects Entry
 * C++: EnvrnmntAspctCategory – policy gate, NEW year/month/dept, VIEW_HISTORY / VIEW_CURRENT_MONTH,
 *      DisplayFieldChange risk validation, DisplayRecordRepostion, ENTRY_COMPLETED.
 *
 * <!-- RQ_HSE_23_3_26_22_02 -->
 */

import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';
import {
  ASPECTS_ENTRY_SCREEN_TAG,
  TABLE_ASPECTS_VIEW,
  applyAspectLineRiskValidation,
  escapeSqlString,
  fetchHseplcAspectFlags,
  getEmployeeDepartmentClause,
  isAdminUserDev,
  isAspectsLineTab,
  applyAspectsListCriteria,
} from './aspectsRegisterPolicy.js';

const SCREEN_TAG = ASPECTS_ENTRY_SCREEN_TAG;
const TABLE_VIEW = TABLE_ASPECTS_VIEW;

const TAB_ASPECTS_UI = 'HSE_Aspcts_Aspcts';
const TAB_TRCNG_UI = 'HSE_Aspcts_Trcng';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_ASPCTS_ASPCTS: {
    tabTable: 'HSE_ASPCTS_ASPCTS',
    serialField: 'ASPCTS_ASPCTS_SRLNO',
    opts: {
      tableName: 'HSE_ASPCTS_ASPCTS',
      serialFieldName: 'ASPCTS_ASPCTS_SRLNO',
      linkFieldName: 'MAINLINK',
      parentTableName: TABLE_VIEW,
      parentKeyFieldName: 'PrmryKy',
    },
  },
};
const KEY_FIELD = 'PrmryKy';

const BTN_VIEW_HISTORY = 'VIEW_HISTORY';
const BTN_VIEW_CURRENT_MONTH = 'VIEW_CURRENT_MONTH';
const BTN_ENTRY_COMPLETED = 'ENTRY_COMPLETED';

/** <!-- RQ_HSE_23_3_26_22_02 --> module-level for MainSubReposition */
let _devInterfaceObj = {};

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_VIEW, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

/**
 * ENTRY_COMPLETED: EXECUTE EnvAspctEntryComplete '<PrmryKy>', then refresh.
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

async function handleViewHistory(dev) {
  const admin = isAdminUserDev(dev);
  let where;
  if (admin) {
    where = "WHERE (ASPCTS_DPRTMNT LIKE '%')";
  } else {
    const clause = await getEmployeeDepartmentClause(dev, false);
    if (!clause) {
      if (dev.AskYesNoMessage) dev.AskYesNoMessage('View History', 'No department scope found for current user.');
      return;
    }
    where = `WHERE (ASPCTS_DPRTMNT IN (${clause}))`;
  }
  applyAspectsListCriteria(dev, SCREEN_TAG, where);
}

async function handleViewCurrentMonth(dev) {
  const { FormGetField } = dev || {};
  if (!FormGetField) return;
  const yr = String(FormGetField(TABLE_VIEW, 'ASPCTS_YR', 'scr') ?? '').trim();
  const mnth = String(FormGetField(TABLE_VIEW, 'ASPCTS_MNTH', 'scr') ?? '').trim();
  if (!yr || !mnth) {
    if (dev.AskYesNoMessage) dev.AskYesNoMessage('View Current Month', 'Year and month must be set on the current record.');
    return;
  }

  const admin = isAdminUserDev(dev);
  let where;
  if (admin) {
    where = `WHERE (ASPCTS_YR = '${escapeSqlString(yr)}' AND ASPCTS_MNTH = '${escapeSqlString(mnth)}')`;
  } else {
    const clause = await getEmployeeDepartmentClause(dev, false);
    if (!clause) {
      if (dev.AskYesNoMessage) dev.AskYesNoMessage('View Current Month', 'No department scope found for current user.');
      return;
    }
    where = `WHERE (ASPCTS_DPRTMNT IN (${clause}) AND ASPCTS_YR = '${escapeSqlString(yr)}' AND ASPCTS_MNTH = '${escapeSqlString(mnth)}')`;
  }
  applyAspectsListCriteria(dev, SCREEN_TAG, where);
}

function setAspectTabsEnabled(dev, enabled) {
  const TabEnable = dev?.TabEnable;
  if (typeof TabEnable !== 'function') return;
  try {
    TabEnable(TAB_ASPECTS_UI, enabled);
    TabEnable(TAB_TRCNG_UI, enabled);
  } catch (e) {
    console.warn('[Web_HSE] Aspects Entry TabEnable:', e);
  }
}

/**
 * C++ DisplayToolBarButtonClicked: NEW main form after complete — set YR/MNTH, department.
 */
async function applyNewHeaderDefaults(dev) {
  const { FormSetField, changeFldAttrb } = dev || {};
  if (!FormSetField) return;

  const pol = await fetchHseplcAspectFlags(dev);
  const yr = pol.aspectYr || '';
  const mnth = pol.aspectMth || '';
  if (yr) FormSetField(TABLE_VIEW, 'ASPCTS_YR', yr, 'scr');
  if (mnth) FormSetField(TABLE_VIEW, 'ASPCTS_MNTH', mnth, 'scr');

  const admin = isAdminUserDev(dev);
  if (admin) {
    if (typeof changeFldAttrb === 'function') {
      try {
        changeFldAttrb('scr', 'ASPCTS_DPRTMNT', 'MUST');
      } catch (e) {
        console.warn('[Web_HSE] Aspects Entry changeFldAttrb MUST:', e);
      }
    }
  } else {
    const dep = await getEmployeeDepartmentClause(dev, true);
    if (dep) FormSetField(TABLE_VIEW, 'ASPCTS_DPRTMNT', dep, 'scr');
    if (typeof changeFldAttrb === 'function') {
      try {
        changeFldAttrb('scr', 'ASPCTS_DPRTMNT', 'ALWAYSLOCKED');
      } catch (e) {
        console.warn('[Web_HSE] Aspects Entry changeFldAttrb LOCKED:', e);
      }
    }
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_ENTRY_COMPLETED) {
    await handleEntryCompleted(devInterfaceObj);
    return;
  }
  if (btn === BTN_VIEW_HISTORY) {
    await handleViewHistory(devInterfaceObj || _devInterfaceObj);
    return;
  }
  if (btn === BTN_VIEW_CURRENT_MONTH) {
    await handleViewCurrentMonth(devInterfaceObj || _devInterfaceObj);
  }
}

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || _devInterfaceObj;
  _devInterfaceObj = devInterface;
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  try {
    // RQ_HSE_23_3_26_22_02 — C++ NEW cancel when policy year and month both empty (before row creation)
    if (strBtnName === 'NEW' && complete === 0 && strTabTag === '') {
      const pol = await fetchHseplcAspectFlags(devInterface);
      const hasYrOrMnth = !!(pol.aspectYr || pol.aspectMth);
      if (!hasYrOrMnth) {
        eventObj.cancel = 1;
        if (typeof devInterface.AskYesNoMessage === 'function') {
          devInterface.AskYesNoMessage(
            'Aspects Entry',
            'Reporting year/month are not configured in HSE Policy (HSE_HSEPLC). Cannot create a new record.'
          );
        }
        if (typeof callBackFn === 'function') callBackFn(eventObj);
        return;
      }
    }

    if (strBtnName === 'NEW' && complete === 1 && strTabTag === '') {
      await applyNewHeaderDefaults(devInterface);
    }

    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
    }
  } catch (error) {
    console.warn('[Web_HSE] Aspects Entry toolBarButtonClicked:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  if (!isAspectsLineTab(strTabTag)) return { cancel: 0 };
  return applyAspectLineRiskValidation(dev, fieldName, { showInvalidMessage: true, mode: 'entry' });
}

/**
 * C++ DisplayRecordRepostion — status >= 5: disable DELETE; status 10: lock (full toolbar off).
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn, setScrLockedAttrb } = dev || {};
  if (!FormGetField || !setScreenDisableBtn) return;

  const st = FormGetField(TABLE_VIEW, 'ASPCTS_ASPCTSTTUS', 'scr');
  const n = parseInt(String(st ?? '').trim(), 10);

  if (Number.isFinite(n) && n >= 10) {
    setScreenDisableBtn(true, true, true);
    // RQ_HSE_23_3_26_22_02 — desktop LockRecord / screen lock parity when status = Accepted (10)
    if (typeof setScrLockedAttrb === 'function') {
      try {
        setScrLockedAttrb('scr', true);
      } catch (e) {
        console.warn('[Web_HSE] Aspects Entry setScrLockedAttrb:', e);
      }
    }
  } else if (Number.isFinite(n) && n >= 5) {
    setScreenDisableBtn(false, false, true);
    if (typeof setScrLockedAttrb === 'function') {
      try {
        setScrLockedAttrb('scr', false);
      } catch (e) {
        console.warn('[Web_HSE] Aspects Entry setScrLockedAttrb:', e);
      }
    }
  } else {
    setScreenDisableBtn(false, false, false);
    if (typeof setScrLockedAttrb === 'function') {
      try {
        setScrLockedAttrb('scr', false);
      } catch (e) {
        console.warn('[Web_HSE] Aspects Entry setScrLockedAttrb:', e);
      }
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const dev = _devInterfaceObj;

  try {
    const pol = await fetchHseplcAspectFlags(dev);
    if (pol.allowEntry === 'N') {
      setScreenDisableBtn(true, true, true);
      setAspectTabsEnabled(dev, false);
      return;
    }
  } catch (e) {
    console.warn('[Web_HSE] Aspects Entry ShowScreen policy:', e);
  }

  setScreenDisableBtn(false, false, false);
  setAspectTabsEnabled(dev, true);
}
