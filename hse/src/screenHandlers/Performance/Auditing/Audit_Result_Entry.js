/**
 * Screen handler: Audit Result Entry (HSE_AudtExcutnRsult)
 * Menu path: Performance -> Auditing -> Audit Result Entry (from HSE.json)
 * C++: AuditModuleCategory – PLAN_CLOSED, AUDITS_STRENGTH_POINTS, AUDIT_COMPLETED, etc.; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
