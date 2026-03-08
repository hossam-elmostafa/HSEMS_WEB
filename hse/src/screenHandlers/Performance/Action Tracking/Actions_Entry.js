/**
 * Screen handler: Actions Entry (HSE_TgActnsEntry)
 * Menu path: Performance -> Action Tracking -> Actions Entry (from HSE.json)
 * C++: NewCarEntryCategory – all custom buttons (VIEW_SOURCE_TXN, VIEW_CAR_REVIEW_INFO, VIEW_CAR_APPROVE_INFO, REJECT_CAR, ENTRY_COMPLETED, CORRECTIVE_ACTIONS).
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
