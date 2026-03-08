/**
 * Screen handler: Audit Plan Entry (HSE_AudtPlnEntry)
 * Menu path: Performance -> Auditing -> Audit Plan Entry (from HSE.json)
 * C++: AuditModuleCategory – PLAN_ENTRY_COMPLETED, COVERED_STANDARDS, IDENTIFY_*, APPROVE, REJECT, etc.; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
