/**
 * Screen handler: Rescue Plan Entry (HSE_TgRscuPlnEntry)
 * Menu path: Safety -> Rescue Plan -> Rescue Plan Entry (from HSE.json)
 * C++: RscuPlnCategory – RSCUPLN_PLNCNCLD, RSCUPLN_PLNCLSD, RSCUPLNINQURY_RTRNPLNTOENTRY, RSCUPLN_TRCNG_VWCNCLRSN; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (['RSCUPLN_PLNCNCLD', 'RSCUPLN_PLNCLSD', 'RSCUPLNINQURY_RTRNPLNTOENTRY', 'RSCUPLN_TRCNG_VWCNCLRSN'].includes(btn)) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
