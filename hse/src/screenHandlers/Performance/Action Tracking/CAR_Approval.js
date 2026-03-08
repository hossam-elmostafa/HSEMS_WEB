/**
 * Screen handler: CAR Approval (HSE_TgCrAprvl)
 * Menu path: Performance -> Action Tracking -> CAR Approval (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_SOURCE_TXN, REJECT_CAR, CANCEL_CAR, APPROVE_CAR, CAR_APPROVAL_INFO, VIEW_CAR_REVIEW_INFO, CORRECTIVE_ACTIONS.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
