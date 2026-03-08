/**
 * Screen handler: Audit Notification (HSE_AudtNtfctn)
 * Menu path: Performance -> Auditing -> Audit Notification (from HSE.json)
 * C++: AuditModuleCategory – VIEW_AUDIT_CHECKLIST, COVERED_STANDARDS, EDIT_*, etc.; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
