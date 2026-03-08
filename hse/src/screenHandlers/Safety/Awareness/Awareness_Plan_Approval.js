/**
 * Screen handler: Awareness Plan Approval (HSE_TgAwrnsPlnAprvl)
 * Menu path: Safety -> Awareness -> Awareness Plan Approval (from HSE.json)
 * C++: AwrnsPlnAprvlCategory – AWRNSPLNAPRVL_PLNENTRYAPRVD (approve), AWRNSPLNAPRVL_PLNENTRYRJCTD (reject via reasons popup); reject delegates to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const TABLE = 'HSE_AWRNSPLN';
const KEY_FIELD = 'SerialPK';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'AWRNSPLNAPRVL_PLNENTRYRJCTD') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
    return;
  }
  if (btn === 'AWRNSPLNAPRVL_PLNENTRYAPRVD') {
    const { FormGetField, executeSQLPromise, refreshData } = devInterfaceObj || {};
    if (!FormGetField || !executeSQLPromise) return;
    const pk = FormGetField(TABLE, KEY_FIELD, 'scr');
    if (pk == null || pk === '') return;
    try {
      await executeSQLPromise(`EXECUTE AwrnsPlnApproved ${pk}`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Awareness Plan Approval:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error approving');
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
