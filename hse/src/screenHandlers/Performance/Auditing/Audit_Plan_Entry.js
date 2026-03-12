/**
 * Screen handler: Audit Plan Entry (HSE_AudtPlnEntry)
 * Menu path: Performance -> Auditing -> Audit Plan Entry (from HSE.json)
 * C++: AuditModuleCategory – ...; NEW on Activities tab sets next serial (HSEMSCommonCategory getNxtSrl).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGADTPLNENTACTVT: {
    tabTable: 'HSE_ADTPLNENTACTVT',
    serialField: 'ADTPLNENTACTVT_ACTVNUM',
    opts: {
      tableName: 'HSE_ADTPLNENTACTVT',
      serialFieldName: 'ADTPLNENTACTVT_ACTVNUM',
      linkFieldName: 'ADTPLNENTACTVT_LNK',
      parentTableName: 'HSE_ADTPLNENT',
      parentKeyFieldName: 'MAINLINK',
    },
  },
};

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
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
    console.warn('[Web_HSE] Audit Plan Entry toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
