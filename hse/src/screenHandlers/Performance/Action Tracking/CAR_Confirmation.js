/**
 * RQ_HSE_12_4_26_00_40 — GAP-2: Screen handler for Corrective Action Confirmation (HSE_TgCrctveAccCnfrmtn).
 * C++: CarConfirmationCategory — CNFRM (confirmCARTXN), RJC (rejectCARTXN), CNCL (CancelCARTXN).
 * Policy gate: HSEPLC_CRCTVEACCCNFRMTNRQRD must be 'Y'.
 */

import { handleCarLayer2Button, checkConfirmationPolicy } from '../../../utils/carLayer2Buttons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-2
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarLayer2Button(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-2: policy gate
  setScreenDisableBtn(false, false, false);
  const exec = devInterfaceObj?.executeSQLPromise;
  const msg = devInterfaceObj?.AskYesNoMessage;
  if (typeof exec === 'function') {
    await checkConfirmationPolicy(exec, msg);
  }
}
