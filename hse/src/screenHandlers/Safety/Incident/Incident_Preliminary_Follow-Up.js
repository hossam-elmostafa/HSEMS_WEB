/**
 * Screen handler: Incident Preliminary Follow-Up (HSE_TgAcdntfollwUpdt)
 * Menu path: Safety -> Incident -> Incident Preliminary Follow-Up (from HSE.json)
 * C++: AccidentFollowUpCategory – ACDNTENT_CLS (CloseAccident), ACDNTENTINJRDPRSN_INJRDBODYPART (OpenBodyParts); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'ACDNTENT_CLS' || btn === 'ACDNTENTINJRDPRSN_INJRDBODYPART') {
    if (btn === 'ACDNTENT_CLS') {
      const doToolbarAction = devInterfaceObj?.doToolbarAction;
      if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj.strFormTag || strScrTag, '');
    }
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
