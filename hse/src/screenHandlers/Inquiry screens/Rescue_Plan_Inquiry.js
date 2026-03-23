/**
 * Screen handler: Rescue Plan Inquiry (HSE_TgRscuPlnInqury)
 * Menu path: Inquiry screens -> Rescue Plan Inquiry (from HSE.json)
 * C++: RscuPlnCategory — close, return to entry, view cancel reason (handled in handleRescuePlanButton).
 * RQ_HSE_23_3_26_17_05
 */

import { sendButtonClickToBackend } from '../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  // RQ_HSE_23_3_26_17_05: RSCUPLN_PLNCLSD, RSCUPLNINQURY_RTRNPLNTOENTRY, RSCUPLN_TRCNG_VWCNCLRSN — handleRescuePlanButton runs first
  if (['RSCUPLN_PLNCLSD', 'RSCUPLNINQURY_RTRNPLNTOENTRY', 'RSCUPLN_TRCNG_VWCNCLRSN'].includes(btn)) {
    return;
  }
  if (btn) sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
