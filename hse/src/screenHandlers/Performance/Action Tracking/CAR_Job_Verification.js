/**
 * RQ_HSE_12_4_26_00_40 — GAP-3: Screen handler for Corrective Action Job Verification (HSE_TgCrctveAccJobVrfctn).
 * C++: CARJobVrfctnCategory — JOBACPTD (CARJobAcptd), JOBRJCTD (rejectCARTXN), CNCL (CARJobCncld).
 */

import { handleCarLayer2Button } from '../../../utils/carLayer2Buttons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-3
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarLayer2Button(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-3
  setScreenDisableBtn(false, false, false);
}
