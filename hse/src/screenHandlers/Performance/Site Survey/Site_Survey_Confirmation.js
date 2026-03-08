/**
 * Screen handler: Site Survey Confirmation (HSE_TgSitSrvyCnfrmtn)
 * Menu path: Performance -> Site Survey -> Site Survey Confirmation (from HSE.json)
 * C++: SitSrvyCnfrmtnCategory – SITSRVYENT_CNFRM (confirmSiteSrvy).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'SITSRVYENT_CNFRM') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
