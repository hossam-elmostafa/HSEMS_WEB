/**
 * Screen handler: Incident Investigation Approval (HSE_TgIncdntInvstgtnAprvl)
 * Menu path: Safety -> Incident -> Incident Investigation Approval (from HSE.json)
 * C++: IncdntInvstgtnAprvCategory – ACDNTENT_CLS, ACDNTENT_RJC; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'ACDNTENT_CLS' || btn === 'ACDNTENT_RJC') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
