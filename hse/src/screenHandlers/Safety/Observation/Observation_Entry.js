/**
 * Screen handler: Observation Entry (HSE_TgNrstMiscEnt / HSE_TGNRSTMISCENT)
 * Menu path: Safety -> Observation -> Observation Entry (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
 * C++: When NEW is clicked on Attachments tab, set next Line No. (getNxtSrl) via HSEMSCommonCategory.
 */

import {
  sendButtonClickToBackend,
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
} from '../../../services/Observation service/ObservationService.js';
import { mapUnknownFieldTypes } from '../../../utils/fieldTypeMapper.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN_VIEW = 'HSE_vwNRSTMISCENT';
const MAIN_KEY_FIELD = 'NrstMiscEnt_NrstMiscNum';

/** Tab tags and config for NEW serial (C++ HSEMSCommonCategory getNxtSrl). */
const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGNRSTMISCENTATCH: {
    tabTable: 'HSE_NrstMiscEntAtch',
    serialField: 'NRSTMISCENTATCH_SRLNO',
    opts: {
      tableName: 'HSE_NrstMiscEntAtch',
      serialFieldName: 'NRSTMISCENTATCH_SRLNO',
      linkFieldName: 'NRSTMISCENTATCH_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE: {
    tabTable: 'HSE_NRSTMISCENTPSSBLNRSTMISCCUSE',
    serialField: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM',
    opts: {
      tableName: 'HSE_NRSTMISCENTPSSBLNRSTMISCCUSE',
      serialFieldName: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM',
      linkFieldName: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
  HSE_TGNRSTMISCENTRQRDACC: {
    tabTable: 'HSE_NRSTMISCENTRQDACTN',
    serialField: 'NRSTMISCENTRQDACTN_ACTNNO',
    opts: {
      tableName: 'HSE_NRSTMISCENTRQDACTN',
      serialFieldName: 'NRSTMISCENTRQDACTN_ACTNNO',
      linkFieldName: 'NRSTMISCENTRQDACTN_LNK',
      parentTableName: TABLE_MAIN_VIEW,
      parentKeyFieldName: MAIN_KEY_FIELD,
    },
  },
};

/**
 * Handle custom button clicks on Observation Entry (Reject, Confirm, Cancel, Close, Entry Complete, etc.).
 * Delegates to ObservationService.sendButtonClickToBackend.
 */
export function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag } = eventObj || {};
  const devInterface = eventObj.devInterfaceObj || {};
  const buttonName = Button_Name ? Button_Name.toString().toUpperCase() : '';
  const screenTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  sendButtonClickToBackend(buttonName, screenTag, eventObj, devInterface);
}

/**
 * Handle toolbar button clicks on Observation Entry (tab enabling after save, Comments tab management).
 * When NEW is clicked on Attachments tab, sets next Line No. (C++: getNxtSrl + FormSetField for HSE_TGNRSTMISCENTATCH).
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  let strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) {
        await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry toolBarButtonClicked tab NEW serial:', error);
  }

  try {
    if (complete === 1) {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
      if (strTabTag && strTabTag.includes('CMNTS')) {
        manageCommentsTabToolBar(strScrTag, strTabTag, devInterface);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);
}

/**
 * Handle ShowScreen for Observation Entry: set button state, map unknown field types, enable observation tabs on main screen.
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Entry ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Entry ShowScreen: error enabling observation tabs:', error);
    }
  }
}
