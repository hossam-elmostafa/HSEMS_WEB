/**
 * Screen handler: Audit Result Confirmation (HSE_ADTRSLTCNFRMTN)
 * Menu path: Performance -> Auditing -> Audit Result Confirmation (from HSE.json)
 * C++: AuditModuleCategory – RESULT_CONFIRMED, CLAUSE-LEVEL_RESULT, VIEW_CLAUSES, etc.; delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (Button_Name) sendButtonClickToBackend((Button_Name + '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
