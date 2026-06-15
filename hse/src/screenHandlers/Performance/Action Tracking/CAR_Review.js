/**
 * Screen handler: CAR Review (HSE_TgCrRvw)
 * Menu path: Performance -> Action Tracking -> CAR Review (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_SOURCE_TXN, REJECT_CAR, ACCEPT_CAR, CAR_REVIEW_INFO, CORRECTIVE_ACTIONS.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
// RQ_HSE_12_4_26_00_40 — C++ HSE_TGCRRVW CRENTRY_CRSRC → CRENTRY_CRSRLN
import { applyCrentrySourceSerialOnReview } from '../../../utils/carCrentryFieldChangeParity.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}

/**
 * RQ_HSE_12_4_26_00_40
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const scr = String(strScrTag || '').toUpperCase();
  const fld = String(fieldName || '').toUpperCase();
  const tab = String(strTabTag || '').trim();
  if (scr === 'HSE_TGCRRVW' && !tab && fld === 'CRENTRY_CRSRC') {
    try {
      await applyCrentrySourceSerialOnReview(devInterfaceObj);
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CAR Review SubFieldChanged CRENTRY_CRSRC:', e);
    }
  }
  return { cancel: 0 };
}
