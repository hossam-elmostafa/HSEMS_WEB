/**
 * Screen handler: PTW Register Entry (HSE_TgPTWsrgstrEnt)
 * Menu path: Safety -> Summary Permits -> PTW Register Entry (from HSE.json)
 * C++: PtwCategory – PTWSRGSTRENT_RJC; NEW on Safety Instructions tab sets next serial (HSEMSCommonCategory getNxtSrl).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGPTWSRGSTRENTSFTYINSTRCTNS: {
    tabTable: 'HSE_PTWSRGSTRSFTYINSTRCTNS',
    serialField: 'PTWSRGSTRSFTYINSTRCTNS_SRLNO',
    opts: {
      tableName: 'HSE_PTWSRGSTRSFTYINSTRCTNS',
      serialFieldName: 'PTWSRGSTRSFTYINSTRCTNS_SRLNO',
      linkFieldName: 'PTWSRGSTRSFTYINSTRCTNS_Lnk',
      parentTableName: 'HSE_PTWSRGSTRENT',
      parentKeyFieldName: 'PTWSRGSTRENT_PTWSNUM',
    },
  },
};

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PTWSRGSTRENT_RJC') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
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
    console.warn('[Web_HSE] PTW Register Entry toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
