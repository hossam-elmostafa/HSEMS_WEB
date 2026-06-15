/**
 * Screen handler: Actions Entry (HSE_TgActnsEntry)
 * Menu path: Performance -> Action Tracking -> Actions Entry (from HSE.json)
 * C++: NewCarEntryCategory – all custom buttons (VIEW_SOURCE_TXN, VIEW_CAR_REVIEW_INFO, VIEW_CAR_APPROVE_INFO, REJECT_CAR, ENTRY_COMPLETED, CORRECTIVE_ACTIONS).
 * RQ_HSE_12_4_26_00_40 — GAP-7: NEW on Corrections/Root Causes tabs → auto-serial.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
// RQ_HSE_12_4_26_00_40 — GAP-7
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN = 'HSE_CRENTRY';
const MAIN_KEY_FIELD = 'PRMKY';

// RQ_HSE_12_4_26_00_40 — GAP-7: C++ getNxtSrl on NEW for corrections and root causes subforms
const TAB_NEW_SERIAL_CONFIG = {
  HSE_ACTNSENTRY_CRR: {
    tabTable: 'HSE_ACTNSENTRY_CRR',
    serialField: 'ACTNSENTRY_Crr_Srl',
    opts: {
      tableName: 'HSE_ACTNSENTRY_CRR',
      serialFieldName: 'ACTNSENTRY_Crr_Srl',
      linkFieldName: 'PrmKy',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_ACTNSENTRY_RTCSS: {
    tabTable: 'HSE_ACTNSENTRY_RTCSS',
    serialField: 'ACTNSENTRY_RtCss_Srl',
    opts: {
      tableName: 'HSE_ACTNSENTRY_RTCSS',
      serialFieldName: 'ACTNSENTRY_RtCss_Srl',
      linkFieldName: 'PrmKy',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
};

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

// RQ_HSE_12_4_26_00_40 — GAP-7: toolbar NEW → auto-serial for corrections/root causes
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
    }
  } catch (error) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-7 Actions Entry toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
