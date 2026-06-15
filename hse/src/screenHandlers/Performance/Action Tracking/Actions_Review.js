/**
 * Screen handler: Actions Review (HSE_TgActnsRviw)
 * Menu path: Performance -> Action Tracking -> Actions Review (from HSE.json)
 * C++: NewCarEntryCategory – VIEW_SOURCE_TXN, REJECT_CAR, ACCEPT_CAR, CORRECTIVE_ACTIONS, CLOSE_CAR.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
// RQ_HSE_12_4_26_00_40 — C++ NewCarEntryCategory DisplayFieldChange CRRCTVACTNS_LNKCSS_ACC
import { applyActionsReviewLnkCssAccChange } from '../../../utils/carActionsReviewDisplayFieldParity.js';
// RQ_HSE_12_4_26_00_40 — server date for CRENTRY_RVWDT (align with desktop GetServerDate())
import { getCarServerDate } from '../../../utils/carServerDate.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
  // RQ_HSE_12_4_26_00_40 — C++ NewCarEntryCategory DisplayScreenReady (HSE_TGACTNSRVIW):
  // default CRENTRY_RVWDT when empty using SQL Server GETDATE() (not client clock).
  // BEFORE: getCurrentFormatedDate() — client time-zone dependent.
  // AFTER:  getCarServerDate() queries SELECT GETDATE() first, falls back to client if unavailable.
  const { FormGetField, FormSetField, getCurrentFormatedDate, executeSQLPromise } = devInterfaceObj || {};
  const formTag = 'HSE_TgActnsRviw';
  try {
    if (typeof FormGetField === 'function' && typeof FormSetField === 'function') {
      let rv =
        FormGetField(formTag, 'CRENTRY_RVWDT', 'scr') ||
        FormGetField(formTag, 'CRENTRY_RVWDT', 'SCR') ||
        FormGetField('HSE_CRENTRY', 'CRENTRY_RVWDT', 'SCR');
      if (rv == null || String(rv).trim() === '') {
        const dt = await getCarServerDate(executeSQLPromise, getCurrentFormatedDate);
        if (dt) FormSetField(formTag, 'CRENTRY_RVWDT', dt, 'scr');
      }
    }
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 Actions Review CRENTRY_RVWDT default:', e);
  }
}

/**
 * RQ_HSE_12_4_26_00_40
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const scr = String(strScrTag || '').toUpperCase();
  if (scr.includes('ACTNSRVIW')) {
    try {
      applyActionsReviewLnkCssAccChange(devInterfaceObj, fieldName, fieldVal);
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 Actions Review SubFieldChanged:', e);
    }
  }
  return { cancel: 0 };
}
