/**
 * Screen handler: Audit Result Confirmation (HSE_ADTRSLTCNFRMTN)
 * Menu path: Performance -> Auditing -> Audit Result Confirmation (from HSE.json)
 * C++: AuditModuleCategory – RESULT_CONFIRMED, CLAUSE-LEVEL_RESULT, VIEW_CLAUSES, etc.; delegate to backend.
 *
 * RQ_HSE_12_4_26_00_40 — GAP-1: Call handleAuditResultConfirmationCarButton FIRST so RESULT_CONFIRMED
 * always runs client-side CAR generation even when ModuleButtonHandlers/index.js is not in the pipeline.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { handleAuditResultConfirmationCarButton } from '../../../utils/auditResultConfirmationCar.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (!Button_Name) return;
  // RQ_HSE_12_4_26_00_40 — GAP-1: RESULT_CONFIRMED consumed here; other buttons unchanged
  const consumed = await handleAuditResultConfirmationCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (consumed) return;
  sendButtonClickToBackend(String(Button_Name).toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
