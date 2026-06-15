/**
 * Screen handler: Actions Under Execution (HSE_TgActnsUndrExec)
 * Menu path: Performance -> Action Tracking -> Actions Under Execution (from HSE.json)
 * C++: CAR/Action Tracking – custom buttons via handleCarButton; NEW on tab sets next serial (HSE_TGCRCTVEACCUNDREXCTNCRPRCSS).
 * RQ_HSE_12_4_26_00_40 — GAP-12: department-filtered work queue via onMenuItemClicked.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { handleCarFoundationTxnButtons } from '../../../utils/carFoundationTxnButtons.js'; // RQ_HSE_30_3_26_14_57
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { setNextSerialOnNewTab } from '../../../utils/tabNewSerialUtils.js';
import { getDevInterface } from '../../../events/buttonEvents.js'; // RQ_HSE_12_4_26_00_40

function escSql(v) { return String(v ?? '').replace(/'/g, "''"); }

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
  const handledFoundation = await handleCarFoundationTxnButtons(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (handledFoundation) return;
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

/**
 * RQ_HSE_12_4_26_00_40 — GAP-12: C++ CARUndrExctnCategory::ChangeToDefaultCriteria:
 * RECSTS=26 OR (CRCTVEACCENT_DPRT IN user's departments AND RECSTS=25).
 */
export async function onMenuItemClicked(userObj, strScrTag, callBackFn) {
  const dev = getDevInterface() || {};
  const exec = dev.executeSQLPromise || dev.executeSQL;
  let strScrCriteria = '';

  if (typeof exec === 'function') {
    try {
      const login = (typeof dev.getUserName === 'function' ? dev.getUserName() : '') || userObj?.LogIn || '';
      if (login) {
        const deptRs = await exec(`SELECT EMPLOYEE_WSHOP FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '${escSql(login)}' OR EMPLOYEE_LOGINNAME = '${escSql(login)}'`);
        const rows = deptRs?.recordset ?? [];
        const depts = rows.map(r => String(r.EMPLOYEE_WSHOP ?? '').trim()).filter(Boolean);
        if (depts.length > 0) {
          const deptList = depts.map(d => `'${escSql(d)}'`).join(',');
          strScrCriteria = `WHERE CRCTVEACCENT_RECSTS = '26' OR (CRCTVEACCENT_DPRT IN (${deptList}) AND CRCTVEACCENT_RECSTS = '25')`;
        }
      }
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-12 Under Execution onMenuItemClicked:', e);
    }
  }

  callBackFn({ isAllowed: true, strScrCriteria, strMsg: '' });
}
