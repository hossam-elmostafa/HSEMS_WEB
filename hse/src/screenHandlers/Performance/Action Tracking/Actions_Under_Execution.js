/**
 * Screen handler: Actions Under Execution (HSE_TgActnsUndrExec)
 * Menu path: Performance -> Action Tracking -> Actions Under Execution (from HSE.json)
 * C++: CAR/Action Tracking – custom buttons via handleCarButton; NEW on tab sets next serial (HSE_TGCRCTVEACCUNDREXCTNCRPRCSS).
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN = 'HSE_CRCTVEACCENT';
const MAIN_KEY_FIELD = 'CRCTVEACCENT_CARMODELNUM';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGCRCTVEACCUNDREXCTNCRPRCSS: {
    tabTable: 'HSE_CRCTVEACCENTRQDACTN',
    serialField: 'CRCTVEACCENTRQDACTN_LNRNO',
    opts: {
      tableName: 'HSE_CRCTVEACCENTRQDACTN',
      serialFieldName: 'CRCTVEACCENTRQDACTN_LNRNO',
      linkFieldName: 'crctveAccEntRqdActn_Lnk',
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
    console.warn('[Web_HSE] Actions Under Execution toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
