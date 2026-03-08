/**
 * Screen handler: Incident Investigation Entry (HSE_TgIncdntInvstgtnEntry)
 * Menu path: Safety -> Incident -> Incident Investigation Entry (from HSE.json)
 * C++: IncdntInvstgtnEntryCategory – ACDNTENT_ENTRYCMPLTD; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'ACDNTENT_ENTRYCMPLTD') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
