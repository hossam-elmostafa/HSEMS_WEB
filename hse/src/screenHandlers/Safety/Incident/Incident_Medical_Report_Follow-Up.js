/**
 * Screen handler: Incident Medical Report Follow-Up (HSE_TgIncdntMdclRprtFlwUp)
 * Menu path: Safety -> Incident -> Incident Medical Report Follow-Up (from HSE.json)
 * C++: HSEMSCommonCategory – INCDNTMDCLRPRT_CLS (closeIncdntMdclRprt); INCDNTMDCLRPRTINJRDPRSNS_* delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

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

  if (btn === 'INCDNTMDCLRPRT_CLS') {
    const { executeSQLPromise, refreshData } = devInterfaceObj || {};
    if (!executeSQLPromise) return;
    const pk = getPrimaryKey(devInterfaceObj);
    if (!pk) return;
    try {
      await executeSQLPromise(`EXECUTE closeIncdntMdclRprt '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Incident Medical Report Follow-Up Close:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error closing');
    }
    return;
  }
  if (['INCDNTMDCLRPRTINJRDPRSNS_INJRDBODYPART', 'INCDNTMDCLRPRTINJRDPRSNS_INJRYTYP', 'INCDNTMDCLRPRTINJRDPRSNS_INJRYCTGRY'].includes(btn)) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
