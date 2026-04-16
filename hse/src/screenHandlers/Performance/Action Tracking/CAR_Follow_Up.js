/**
 * Screen handler: CAR Follow Up (HSE_TgCrFloUp)
 * Menu path: Performance -> Action Tracking -> CAR Follow Up (from HSE.json)
 * C++: NewCarEntryCategory – REJECT_CAR, CORRECTIVE_ACTIONS, CLOSE_CAR.
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
// RQ_HSE_12_4_26_00_40 — C++ NewCarEntryCategory DisplayScreenReady / tab toolbars on HSE_TGCRFLOUP
import {
  applyCarFollowUpChildToolbarParity,
  applyCarFollowUpCorrectionsActFllDt,
} from '../../../utils/carFollowUpToolbarParity.js';

/** @type {object} */
let _devInterfaceObj = {};

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
  applyCarFollowUpChildToolbarParity(_devInterfaceObj, strTabTag);
  // RQ_HSE_12_4_26_00_40 — C++ DisplayRecordRepostion actual follow-up date on Corrections tab
  // applyCarFollowUpCorrectionsActFllDt is now async (queries server date); await it here.
  await applyCarFollowUpCorrectionsActFllDt(_devInterfaceObj, strTabTag);
}

/**
 * RQ_HSE_12_4_26_00_40 — Re-apply per active tab (shell does not pass devInterface here).
 * applyCarFollowUpCorrectionsActFllDt is async; fire-and-forget is acceptable on reposition
 * because the field update is best-effort (server-date query may complete after reposition settles).
 */
export function MainSubReposition(strFormTag, _mainPos, _selTab, strSelectedTabTag) {
  applyCarFollowUpChildToolbarParity(_devInterfaceObj, strSelectedTabTag);
  // intentional fire-and-forget — sync caller context; result settles asynchronously
  applyCarFollowUpCorrectionsActFllDt(_devInterfaceObj, strSelectedTabTag);
}
