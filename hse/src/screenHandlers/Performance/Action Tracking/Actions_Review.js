/**
 * Screen handler: Actions Review (HSE_TgActnsRviw)
 * Menu path: Performance -> Action Tracking -> Actions Review (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_SOURCE_TXN, REJECT_CAR, ACCEPT_CAR, CORRECTIVE_ACTIONS, CLOSE_CAR.
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
