/**
 * Screen handler: Awareness Plan Execution (HSE_TgAwrnsPlnExctn)
 * Menu path: Safety -> Awareness -> Awareness Plan Execution (from HSE.json)
 * C++: AwrnsPlnExctnCategory – AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS (createUnPlndCrs), AWRNSPLNEXCTN_PLNCLSD (closeAwrnsPln); create delegates to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const TABLE = 'HSE_AWRNSPLN';
const KEY_FIELD = 'SerialPK';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
    return;
  }
  if (btn === 'AWRNSPLNEXCTN_PLNCLSD') {
    const { FormGetField, executeSQLPromise, refreshData } = devInterfaceObj || {};
    if (!FormGetField || !executeSQLPromise) return;
    const pk = FormGetField(TABLE, KEY_FIELD, 'scr');
    if (pk == null || pk === '') return;
    try {
      await executeSQLPromise(`EXECUTE AwrnsPlnClosed ${pk}`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Awareness Plan Execution Close:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error closing plan');
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
