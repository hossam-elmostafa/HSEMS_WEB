/**
 * Screen handler: In-Site EQ (HSE_EQInspctnWrk)
 * Menu path: Performance -> EQ Inspection -> In-Site EQ (from HSE.json)
 * C++: EQ inspection flow – any custom buttons delegate to backend.
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
