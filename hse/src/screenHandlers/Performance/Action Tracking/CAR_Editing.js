/**
 * Screen handler: CAR Editing (HSE_TgCrEdtng)
 * Menu path: Performance -> Action Tracking -> CAR Editing (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_CAR_REVIEW_INFO, VIEW_CAR_APPROVE_INFO, CORRECTIVE_ACTIONS.
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
