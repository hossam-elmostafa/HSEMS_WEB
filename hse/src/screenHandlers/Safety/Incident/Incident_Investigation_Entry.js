/**
 * Screen handler: Incident Investigation Entry (HSE_TgIncdntInvstgtnEntry)
 * Menu path: Safety -> Incident -> Incident Investigation Entry (from HSE.json)
 * C++: IncdntInvstgtnEntryCategory – ACDNTENT_ENTRYCMPLTD; delegate to backend.
 * RQ_HSE_13_3_26_4_18 — same injured-person pop-ups as preliminary entry when those buttons are present
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { handleAccidentInjuredPersonCustomButton } from '../../../utils/incidentInjuredPersonPopups.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (await handleAccidentInjuredPersonCustomButton(devInterfaceObj, btn, strScrTag)) return;
  if (btn === 'ACDNTENT_ENTRYCMPLTD') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
