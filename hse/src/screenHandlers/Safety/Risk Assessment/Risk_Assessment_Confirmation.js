/**
 * Screen handler: Risk Assessment Confirmation (HSE_TgrskassmntApr)
 * Menu path: Safety -> Risk Assessment -> Risk Assessment Confirmation (from HSE.json)
 * C++: RiskAssessmentApprovalCategory – RSKASSMNTENT_APRV; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'RSKASSMNTENT_APRV') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
