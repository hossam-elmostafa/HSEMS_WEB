/**
 * Screen handler: Audit Plan Approval (HSE_AudtPlnApprvl)
 * Menu path: Performance -> Auditing -> Audit Plan Approval (from HSE.json)
 * C++: AuditModuleCategory – APPROVE, REJECT, COVERED_STANDARDS, ADTPLNENT_RJC, etc.; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
