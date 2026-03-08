/**
 * Screen handler: EQ Inspection Result (HSE_EQInspctnRsult)
 * Menu path: Performance -> EQ Inspection -> EQ Inspection Result (from HSE.json)
 * C++: EQInspctnCategory – all custom buttons (ENTRYCMPLTD, RELOADCHCKLST, reject/hold/accept/stop/resume/exit/cancel/complete, VW* filters, etc.); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn && (btn.startsWith('EQINSPCTN_') || btn.startsWith('INSPCTNCHCKLST_'))) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
