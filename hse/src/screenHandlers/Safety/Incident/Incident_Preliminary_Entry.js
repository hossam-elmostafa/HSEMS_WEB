/**
 * Screen handler: Incident Preliminary Entry (HSE_TgAcdntEnt)
 * Menu path: Safety -> Incident -> Incident Preliminary Entry (from HSE.json)
 * C++: AccidentEntryCategory + AccidentCategory – ACDNTENT_ENTCMPLT, ACDNTENTINJRDPRSN_INJRDBODYPART, ACDNTENT_RJC, INJRYTYP, PRSNL_PRTCTVE_EQUIP, ACDNTENTACDNTRSN_GETROTCUS, INJURY_ANALYSIS; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const INCIDENT_ENTRY_BUTTONS = [
  'ACDNTENT_ENTCMPLT',
  'ACDNTENTINJRDPRSN_INJRDBODYPART',
  'ACDNTENT_RJC',
  'INJRYTYP',
  'PRSNL_PRTCTVE_EQUIP',
  'ACDNTENTACDNTRSN_GETROTCUS',
  'INJURY_ANALYSIS',
];

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (INCIDENT_ENTRY_BUTTONS.includes(btn)) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
