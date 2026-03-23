/**
 * Screen handler: Aspects Review (HSE_AspctsRvwAtRvw)
 * Menu path: Environment -> Aspects Register -> Aspects Review
 * C++: EnvrnmntAspctReviewCategory – DELETE/NEW off, policy HSEPLC_EDTASPCTRVWSCR,
 *      DisplayFieldChange risk on RSKRNK_1, ACCEPTED / REJECTED.
 *
 * <!-- RQ_HSE_23_3_26_22_02 -->
 */

import {
  applyAspectLineRiskValidation,
  fetchHseplcAspectFlags,
  isAspectsLineTab,
} from './aspectsRegisterPolicy.js';

const TABLE_VIEW = 'HSE_ASPCTS_VIEW';
const KEY_FIELD = 'PrmryKy';

const BTN_ACCEPTED = 'ACCEPTED';
const BTN_REJECTED = 'REJECTED';

/** <!-- RQ_HSE_23_3_26_22_02 --> */
let _devInterfaceObj = {};

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_VIEW, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

async function handleAccept(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE EnvAspctRvwAccpt '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Aspects Review Accept:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error accepting');
  }
}

async function handleReject(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE EnvAspctRjct '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Aspects Review Reject:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error rejecting');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_ACCEPTED) {
    await handleAccept(devInterfaceObj);
    return;
  }
  if (btn === BTN_REJECTED) {
    await handleReject(devInterfaceObj);
  }
}

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  if (!isAspectsLineTab(strTabTag)) return { cancel: 0 };
  return applyAspectLineRiskValidation(dev, fieldName, { showInvalidMessage: false, mode: 'review' });
}

/**
 * C++ DisplayScreenReady — always disable NEW + DELETE; if allowAspctReview N, disable SAVE.
 * RQ_HSE_23_3_26_22_02 — tab toolbar parity via FormEnableButton (bForCurrTabOnly).
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const dev = _devInterfaceObj;
  const fe = dev?.FormEnableButton;

  const resetTabToolbar = () => {
    if (typeof fe !== 'function') return;
    fe(2, 1, 'NEW', true, true);
    fe(2, 1, 'SAVE', true, true);
    fe(2, 1, 'DELETE', true, true);
  };

  resetTabToolbar();

  try {
    const pol = await fetchHseplcAspectFlags(dev);
    if (pol.allowReview === 'N') {
      setScreenDisableBtn(true, true, true);
      if (typeof fe === 'function') {
        fe(2, 1, 'NEW', false, true);
        fe(2, 1, 'SAVE', false, true);
        fe(2, 1, 'DELETE', false, true);
      }
      return;
    }
  } catch (e) {
    console.warn('[Web_HSE] Aspects Review ShowScreen policy:', e);
  }

  setScreenDisableBtn(true, false, true);
  if (typeof fe === 'function') {
    fe(2, 1, 'NEW', false, true);
    fe(2, 1, 'DELETE', false, true);
    fe(2, 1, 'SAVE', true, true);
  }
}
