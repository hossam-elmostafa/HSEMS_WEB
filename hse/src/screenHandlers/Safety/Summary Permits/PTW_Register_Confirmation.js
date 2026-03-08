/**
 * Screen handler: PTW Register Confirmation (HSE_TgPTWsrgstrcnfrmtn)
 * Menu path: Safety -> Summary Permits -> PTW Register Confirmation (from HSE.json)
 * C++: PtwClsCategory – PTWSRGSTRENT_CLSPTW; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PTWSRGSTRENT_CLSPTW') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
