/**
 * Screen handler: CAR Entry (HSE_TgCrEntry)
 * Menu path: Performance -> Action Tracking -> CAR Entry (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_SOURCE_TXN, ...; NEW on tabs sets next serial (HSEMSCommonCategory getNxtSrl).
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN = 'HSE_CRCTVEACCENT';
const MAIN_KEY_FIELD = 'CRCTVEACCENT_CARMODELNUM';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGCRCTVEACCENTTAKRMDALACC: {
    tabTable: 'HSE_CRCTVEACCENTTAKRMDALACC',
    serialField: 'CRCTVEACCENTTAKRMDALACC_LNRNUM',
    opts: {
      tableName: 'HSE_CRCTVEACCENTTAKRMDALACC',
      serialFieldName: 'CRCTVEACCENTTAKRMDALACC_LNRNUM',
      linkFieldName: 'crctveAccEntTakrmdalAcc_LNK',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGCRCTVEACCENTROOTCUSE: {
    tabTable: 'HSE_CRCTVEACCENTROOTCUSE',
    serialField: 'CRCTVEACCENTROOTCUSE_LNRNUM',
    opts: {
      tableName: 'HSE_CRCTVEACCENTROOTCUSE',
      serialFieldName: 'CRCTVEACCENTROOTCUSE_LNRNUM',
      linkFieldName: 'CRCTVEACCENTROOTCUSE_LNK',
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
    console.warn('[Web_HSE] CAR Entry toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
