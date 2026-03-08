/**
 * Screen handler: Incident Preliminary Review (HSE_TgAcdntcnfrmtn)
 * Menu path: Safety -> Incident -> Incident Preliminary Review (from HSE.json)
 * C++: AccidentConfirmationCategory – ACDNTENT_CNFRM (ConfirmAccident), ACDNTENTINJRDPRSN_INJRDBODYPART (OpenBodyParts); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'ACDNTENT_CNFRM' || btn === 'ACDNTENTINJRDPRSN_INJRDBODYPART') {
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (btn === 'ACDNTENT_CNFRM' && typeof doToolbarAction === 'function') {
      doToolbarAction('SAVE', eventObj.strFormTag || strScrTag, '');
    }
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
