/**
 * Screen handler: Observation Review (HSE_TgNrstMisccnfrmtn)
 * Menu path: Safety -> Observation -> Observation Review (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
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

const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGNRSTMISCCNFRMTNRQRDACC: {
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
  // Attachments tab – same table/serial as Observation Entry (C++ getNxtSrl for HSE_TGNRSTMISCCNFRMTNATCH).
  HSE_TGNRSTMISCCNFRMTNATCH: {
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
  HSE_NRSTMISCENTATCH: {
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
};

export function ButtonClicked(eventObj) {
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

  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config) {
        //BUG_HSE_HSM_14_3_17_35: Observation Review tab NEW serial is not working
        await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Review toolBarButtonClicked tab NEW serial:', error);
  }

  // Run observation tabs management and comments BEFORE calling back
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
    console.warn('[Web_HSE] Observation Review toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);

  // Re-apply serial after callback (the callback may trigger state resets that overwrite the field).
  // This mirrors the desktop C++ behaviour where getNxtSrl + FormSetField occurs after the framework has finished setting up the new row.
  try {
    if (strBtnName === 'NEW' && complete === 1 && strTabTag) {
      const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
      if (config && devInterface.FormSetField && devInterface.executeSQLPromise) {
        const { FormGetField, executeSQLPromise, getValFromRecordSet } = devInterface;
        const parentKey = FormGetField
          ? FormGetField(config.opts.parentTableName, config.opts.parentKeyFieldName, 'scr')
          : '';
        const key = parentKey != null && parentKey !== '' ? String(parentKey).trim() : '';
        const esc = key.replace(/'/g, "''");
        const where = config.opts.linkFieldName ? `WHERE ${config.opts.linkFieldName} = '${esc}'` : '';
        const sql = `SELECT ISNULL(MAX(${config.opts.serialFieldName})+1,1) AS New_Serial FROM ${config.opts.tableName} ${where}`.trim();
        const data = await executeSQLPromise(sql);
        let val = data?.recordset?.[0] && getValFromRecordSet
          ? getValFromRecordSet(data, 'New_Serial')
          : data?.recordset?.[0]?.New_Serial;
        const n = parseInt(val, 10);
        const newSerial = Number.isFinite(n) && n > 0 ? n : 1;
        devInterface.FormSetField(config.tabTable, config.serialField, String(newSerial), 'tab');
      }
    }
  } catch (e) {
    // Fallback serial attempt failed; the first attempt may have succeeded
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Review ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Review ShowScreen: error enabling observation tabs:', error);
    }
  }
}
