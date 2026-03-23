/**
 * Screen handler: Rescue Plan Entry (HSE_TgRscuPlnEntry)
 * Menu path: Safety -> Rescue Plan -> Rescue Plan Entry (from HSE.json)
 * C++: RscuPlnCategory — DisplayToolBarButtonClicked (NEW serials); custom buttons handled in handleRescuePlanButton.
 * RQ_HSE_23_3_26_17_05: toolBarButtonClicked + main/tab NEW serial parity with RscuPlnCategory.cpp; cancel/close/return/view in ModuleButtonHandlers.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab, getNextSerialForTab } from '../../../utils/tabNewSerialUtils.js';

const RESCUE_PLAN_ENTRY_TAG = 'HSE_TGRSCUPLNENTRY';
const TABLE_HSE_RSCUPLN = 'HSE_RSCUPLN';

/** Desktop subform tags from RscuPlnCategory::DisplayToolBarButtonClicked — RQ_HSE_23_3_26_17_05 */
const TAB_NEW_SERIAL_CONFIG = {
  HSE_TGRSCUPLNENTRY_PTNTLHZRDS: {
    tabTable: 'HSE_RSCUPLN_PTNTLHZRDS',
    serialField: 'RSCUPLN_PTNTLHZRDS_SRLN',
    opts: {
      tableName: 'HSE_RSCUPLN_PTNTLHZRDS',
      serialFieldName: 'RSCUPLN_PTNTLHZRDS_SRLN',
      linkFieldName: 'RSCUPLN_PTNTLHZRDS_LNK',
      parentTableName: TABLE_HSE_RSCUPLN,
      parentKeyFieldName: 'RSCUPLN_PRMRYKY',
    },
  },
  HSE_TGRSCUPLNENTRY_ATTCHMNTS: {
    tabTable: 'HSE_RSCUPLN_ATTCHMNTS',
    serialField: 'RSCUPLN_ATTCHMNTS_LNN',
    opts: {
      tableName: 'HSE_RSCUPLN_ATTCHMNTS',
      serialFieldName: 'RSCUPLN_ATTCHMNTS_LNN',
      linkFieldName: 'RSCUPLN_ATTCHMNTS_LNK',
      parentTableName: TABLE_HSE_RSCUPLN,
      parentKeyFieldName: 'RSCUPLN_PRMRYKY',
    },
  },
  HSE_TGRSCUPLNENTRY_RQURDTLS: {
    tabTable: 'HSE_RSCUPLN_RQURDTLS',
    serialField: 'RSCUPLN_RQURDTLS_SRLN',
    opts: {
      tableName: 'HSE_RSCUPLN_RQURDTLS',
      serialFieldName: 'RSCUPLN_RQURDTLS_SRLN',
      linkFieldName: 'RSCUPLN_RQURDTLS_LNK',
      parentTableName: TABLE_HSE_RSCUPLN,
      parentKeyFieldName: 'RSCUPLN_PRMRYKY',
    },
  },
  HSE_TGRSCUPLNENTRY_CMMUNCTNMTHDS: {
    tabTable: 'HSE_RSCUPLN_CMMUNCTNMTHDS',
    serialField: 'RSCUPLN_CMMUNCTNMTHDS_SRLN',
    opts: {
      tableName: 'HSE_RSCUPLN_CMMUNCTNMTHDS',
      serialFieldName: 'RSCUPLN_CMMUNCTNMTHDS_SRLN',
      linkFieldName: 'RSCUPLN_CMMUNCTNMTHDS_LNK',
      parentTableName: TABLE_HSE_RSCUPLN,
      parentKeyFieldName: 'RSCUPLN_PRMRYKY',
    },
  },
};

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  try {
    if (strScrTag === RESCUE_PLAN_ENTRY_TAG && strBtnName === 'NEW' && complete === 1) {
      const { FormSetField, FormGetField } = devInterface;
      if (FormSetField && FormGetField) {
        if (strTabTag === '') {
          const yrVal = FormGetField(TABLE_HSE_RSCUPLN, 'RSCUPLN_PLNYR', 'scr');
          const nextPlnn = await getNextSerialForTab(
            devInterface,
            {
              tableName: TABLE_HSE_RSCUPLN,
              serialFieldName: 'RSCUPLN_PLNN',
              linkFieldName: 'RSCUPLN_PLNYR',
              parentTableName: TABLE_HSE_RSCUPLN,
              parentKeyFieldName: 'RSCUPLN_PLNYR',
            },
            yrVal
          );
          FormSetField(TABLE_HSE_RSCUPLN, 'RSCUPLN_PLNN', String(nextPlnn), 'scr');
        } else {
          const config = TAB_NEW_SERIAL_CONFIG[strTabTag];
          if (config) await setNextSerialOnNewTab(devInterface, config.tabTable, config.serialField, config.opts);
        }
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Rescue Plan Entry toolBarButtonClicked:', error);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  // RQ_HSE_23_3_26_17_05: handled in handleRescuePlanButton (buttonEvents) before this runs
  if (
    [
      'RSCUPLN_PLNCNCLD',
      'RSCUPLN_PLNCLSD',
      'RSCUPLNENT_CLS',
      'RSCUPLNINQURY_RTRNPLNTOENTRY',
      'RSCUPLN_TRCNG_VWCNCLRSN',
    ].includes(btn)
  ) {
    return;
  }
  if (btn) sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
