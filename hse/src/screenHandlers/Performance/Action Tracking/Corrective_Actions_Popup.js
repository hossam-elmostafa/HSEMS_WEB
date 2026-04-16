/**
 * RQ_HSE_12_4_26_00_40 — GAP-1/11/12: Screen handler for Corrective Actions Popup.
 * C++: CCorrectiveActionEntry — REJECT_ACTION, ACCEPT_ACTION, KEEP_ACTION_PENDING,
 *      ACTION_COMPLETED, VIEW_SOURCE_CAR; field-change rules for CRRCTVACTNS_ACC,
 *      CRRCTVACTNS_ACTACC, CRRCTVACTNS_RVSTRGDT.
 * GAP-11: DisplayToolBarButtonClicked NEW → AddNewSerial (CrrctvActns_Srl) like HSEMSCommonCategory InitializeParameters.
 *
 * Serves all popup tag variants:
 *   HSE_TGCRRCTVACTNS, HSE_TgCrrctvActns_Rvw, HSE_TgCrrctvActns_ActEnt, HSE_TgCrrctvActnsFlwUp
 */

import {
  handleCorrectiveActionPopupButton,
  applyCorrectiveActionPopupFieldChange,
} from '../../../utils/carCorrectiveActionPopupButtons.js';
import { getNextSerialForTab } from '../../../utils/tabNewSerialUtils.js';

export async function ButtonClicked(eventObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-1/12
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  await handleCorrectiveActionPopupButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
}

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-1/12
  applyCorrectiveActionPopupFieldChange(devInterfaceObj, strScrTag, fieldName, fieldVal);
  return { cancel: 0 };
}

/** C++: only HSE_TGCRRCTVACTNS + ACTENT allow toolbar NEW (Review/Follow-up popups disable NEW). */
function allowsCorrectiveActionsPopupNewSerial(strScrTag) {
  const u = String(strScrTag || '').toUpperCase();
  if (u.includes('RVW') || u.includes('FLWUP')) return false;
  if (u === 'HSE_TGCRRCTVACTNS') return true;
  if (u.includes('ACTENT')) return true;
  return false;
}

function tryFormGet(FormGetField, table, field, src) {
  if (typeof FormGetField !== 'function') return '';
  try {
    const v = FormGetField(table, field, src);
    return v != null && String(v).trim() !== '' ? String(v).trim() : '';
  } catch (_) {
    return '';
  }
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-11: C++ AddNewSerial — getNxtSrl on HSE_CRRCTVACTNS.CrrctvActns_Srl
 * scoped by CrrctvActns_LnkWthMn when known; else PrmKy + HSE_CRENTRY (same pattern as corrections tab).
 */
async function applyCorrectiveActionsPopupNewSerial(devInterfaceObj, strScrTag) {
  const { FormGetField, FormSetField, executeSQLPromise } = devInterfaceObj || {};
  if (!FormGetField || !FormSetField || !executeSQLPromise) return;

  const serialField = 'CrrctvActns_Srl';
  const linkField = 'CrrctvActns_LnkWthMn';
  let linkMn =
    tryFormGet(FormGetField, strScrTag, linkField, 'tab') ||
    tryFormGet(FormGetField, strScrTag, 'CRRCTVACTNS_LNKWTHMN', 'tab') ||
    tryFormGet(FormGetField, 'HSE_CRRCTVACTNS', linkField, 'tab') ||
    tryFormGet(FormGetField, 'HSE_CRRCTVACTNS', 'CRRCTVACTNS_LNKWTHMN', 'tab');

  if (linkMn) {
    const n = await getNextSerialForTab(devInterfaceObj, {
      tableName: 'HSE_CRRCTVACTNS',
      serialFieldName: serialField,
      linkFieldName: linkField,
    }, linkMn);
    try {
      FormSetField(strScrTag, serialField, String(n), 'tab');
      FormSetField(strScrTag, linkField, linkMn, 'tab');
    } catch (_) {
      FormSetField('HSE_CRRCTVACTNS', serialField, String(n), 'tab');
      FormSetField('HSE_CRRCTVACTNS', linkField, linkMn, 'tab');
    }
    return;
  }

  // RQ_HSE_12_4_26_00_40 — Fallback: C++ passes PrmKy in InitializeParameters; scope MAX+1 by CAR PrmKy.
  // Do not use setNextSerialOnNewTab(tabTableName) here — popup FormSetField targets strScrTag, not HSE_CRRCTVACTNS.
  const parentPrmKy = tryFormGet(FormGetField, 'HSE_CRENTRY', 'PRMKY', 'scr');
  const n = await getNextSerialForTab(devInterfaceObj, {
    tableName: 'HSE_CRRCTVACTNS',
    serialFieldName: serialField,
    linkFieldName: 'PrmKy',
  }, parentPrmKy);
  try {
    FormSetField(strScrTag, serialField, String(n), 'tab');
  } catch (_) {
    FormSetField('HSE_CRRCTVACTNS', serialField, String(n), 'tab');
  }
}

// RQ_HSE_12_4_26_00_40 — GAP-11: C++ CorrectiveActionEntry::DisplayToolBarButtonClicked (NEW + iComplete==1)
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const dev = eventObj.devInterfaceObj || {};
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;
  const completeStr = complete == null ? '' : String(complete).trim();
  const isNewPostPhase = complete === 1 || completeStr === '1';
  const strScrTag = eventObj.strScrTag || '';

  try {
    if (strBtnName === 'NEW' && isNewPostPhase && allowsCorrectiveActionsPopupNewSerial(strScrTag)) {
      await applyCorrectiveActionsPopupNewSerial(dev, strScrTag);
    }
  } catch (error) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-11 Corrective_Actions_Popup toolBar NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
