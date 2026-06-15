/**
 * Screen handler: Incident Medical Report Entry (HSE_TgIncdntMdclRprt)
 * Menu path: Safety -> Incident -> Incident Medical Report Entry (from HSE.json)
 * C++: HSEMSCommonCategory – INCDNTMDCLRPRT_ENTRYCMPLT (completeIncdntMdclRprt); INCDNTMDCLRPRTINJRDPRSNS_* delegate to backend.
 * RQ_HSE_23_3_26_3_36
 * RQ_HSE_13_3_26_4_18 — injured-person pop-ups via incidentInjuredPersonPopups.js
 */

import { handleMedicalInjuredPersonCustomButton } from '../../../utils/incidentInjuredPersonPopups.js';

const TABLE = 'HSE_INCDNTMDCLRPRT';
const KEY_FIELD = 'IncdntMdclRprt_FlshNo';

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === 'INCDNTMDCLRPRT_ENTRYCMPLT') {
    const { executeSQLPromise, refreshData } = devInterfaceObj || {};
    if (!executeSQLPromise) return;
    const pk = getPrimaryKey(devInterfaceObj);
    if (!pk) return;
    try {
      await executeSQLPromise(`EXECUTE completeIncdntMdclRprt '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Incident Medical Report Entry Complete:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error completing entry');
    }
    return;
  }
  if (handleMedicalInjuredPersonCustomButton(devInterfaceObj, btn, strScrTag)) return;
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
