/**
 * Screen handler: Risk Assessment Entry (HSE_TgrskassmntEnt)
 * Menu path: Safety -> Risk Assessment -> Risk Assessment Entry (from HSE.json)
 * C++: RiskAssessmentEntryCategory – RSKASSMNTENT_ENTCMPLT, RSKASSMNTENT_SHOWMATRIX; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (['RSKASSMNTENT_ENTCMPLT', 'RSKASSMNTENT_SHOWMATRIX'].includes(btn)) sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
