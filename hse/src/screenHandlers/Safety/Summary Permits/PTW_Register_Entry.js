/**
 * Screen handler: PTW Register Entry (HSE_TgPTWsrgstrEnt)
 * Menu path: Safety -> Summary Permits -> PTW Register Entry (from HSE.json)
 * C++: PtwCategory – PTWSRGSTRENT_RJC; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PTWSRGSTRENT_RJC') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
