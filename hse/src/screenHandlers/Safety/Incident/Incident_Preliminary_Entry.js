/**
 * Screen handler: Incident Preliminary Entry (HSE_TgAcdntEnt)
 * Menu path: Safety -> Incident -> Incident Preliminary Entry (from HSE.json)
 * C++: AccidentEntryCategory + AccidentCategory – ACDNTENT_ENTCMPLT, ...; NEW on tabs sets next serial (HSEMSCommonCategory getNxtSrl).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN = 'HSE_ACDNTENT';
const MAIN_KEY_FIELD = 'ACDNTENT_ACDNTNUM';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGACDNTENTACDNTRSN: {
    tabTable: 'HSE_ACDNTENTACDNTRSN',
    serialField: 'ACDNTENTACDNTRSN_LNRNUM',
    opts: {
      tableName: 'HSE_ACDNTENTACDNTRSN',
      serialFieldName: 'ACDNTENTACDNTRSN_LNRNUM',
      linkFieldName: 'AcdntEntAcdntRsn_Lnk',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGACDNTENTTKNRMDLACTNS: {
    tabTable: 'HSE_ACDNTENTTKNRMDLACTNS',
    serialField: 'ACDNTENTTKNRMDLACTNS_LNRNUM',
    opts: {
      tableName: 'HSE_ACDNTENTTKNRMDLACTNS',
      serialFieldName: 'ACDNTENTTKNRMDLACTNS_LNRNUM',
      linkFieldName: 'ACDNTENTTKNRMDLACTNS_LNK',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGACDNTENTRECMNDACC: {
    tabTable: 'HSE_ACDNTENTRECMNDACC',
    serialField: 'ACDNTENTRECMNDACC_ACTNNO',
    opts: {
      tableName: 'HSE_ACDNTENTRECMNDACC',
      serialFieldName: 'ACDNTENTRECMNDACC_ACTNNO',
      linkFieldName: 'ACDNTENTRECMNDACC_LNK',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGACDNTENTATCH: {
    tabTable: 'HSE_ACDNTENTATCH',
    serialField: 'ACDNTENTATCH_SRLNO',
    opts: {
      tableName: 'HSE_ACDNTENTATCH',
      serialFieldName: 'ACDNTENTATCH_SRLNO',
      linkFieldName: 'ACDNTENTATCH_LNK',
      parentTableName: TABLE_MAIN,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
};

const INCIDENT_ENTRY_BUTTONS = [
  'ACDNTENT_ENTCMPLT',
  'ACDNTENTINJRDPRSN_INJRDBODYPART',
  'ACDNTENT_RJC',
  'INJRYTYP',
  'PRSNL_PRTCTVE_EQUIP',
  'ACDNTENTACDNTRSN_GETROTCUS',
  'INJURY_ANALYSIS',
];

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (INCIDENT_ENTRY_BUTTONS.includes(btn)) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

/**
 * toolBarButtonClicked: when NEW on tab, set next serial (C++ HSEMSCommonCategory getNxtSrl for ACDNTENT tabs).
 */
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
    console.warn('[Web_HSE] Incident Preliminary Entry toolBarButtonClicked tab NEW serial:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
