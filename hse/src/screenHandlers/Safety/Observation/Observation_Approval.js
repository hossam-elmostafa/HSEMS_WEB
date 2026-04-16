/**
 * Screen handler: Observation Approval (HSE_TgNrstMiscflwup)
 * Menu path: Safety -> Observation -> Observation Approval (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
 * BUG_HSE_HSM_14_3_26: Custom Close (NRSTMISCENT_CLS) handled in ObservationService.handleCloseButton with desktop parity.
 * RQ_HSE_5_4_26_14_19 — Comments tab Source Screen = getScreenCaption.
 */

import {
  sendButtonClickToBackend,
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
  // RQ_HSE_5_4_26_14_19
  applyObservationCommentsSourceScreen,
} from '../../../services/Observation service/ObservationService.js';
import { mapUnknownFieldTypes } from '../../../utils/fieldTypeMapper.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';

const TABLE_MAIN_VIEW = 'HSE_vwNRSTMISCENT';
const MAIN_KEY_FIELD = 'NrstMiscEnt_NrstMiscNum';

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE: {
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
  HSE_TGNRSTMISCFLWUPRQRDACC: {
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

export function ButtonClicked(eventObj) {
  // BUG_HSE_HSM_14_3_26: Close (NRSTMISCENT_CLS) routed to handleCloseButton in ObservationService
  const { Button_Name, strScrTag } = eventObj || {};
  const devInterface = eventObj.devInterfaceObj || {};
  const buttonName = Button_Name ? Button_Name.toString().toUpperCase() : '';
  const screenTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  sendButtonClickToBackend(buttonName, screenTag, eventObj, devInterface);
}

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  let strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;
  const completeStr = complete == null ? '' : String(complete).trim();
  const isPostPhase = complete === 1 || completeStr === '1';
  const isPrePhase =
    !isPostPhase &&
    (complete == null ||
      complete === '' ||
      complete === 0 ||
      complete === false ||
      completeStr === '0');

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'SAVE' && isPrePhase && strTabTag && strTabTag.includes('CMNTS') && eventObj.isNewMode) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, eventObj?.fullRecord);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Approval SAVE pre Comments SRCSCRN:', e);
  }

  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Approval toolBarButtonClicked tab NEW serial:', error);
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
    console.warn('[Web_HSE] Observation Approval toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'NEW' && isPostPhase && strTabTag && strTabTag.includes('CMNTS')) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, undefined);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Approval post-callback Comments SRCSCRN:', e);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Approval ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Approval ShowScreen: error enabling observation tabs:', error);
    }
  }
}
