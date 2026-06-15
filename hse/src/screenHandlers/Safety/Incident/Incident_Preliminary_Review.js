/**
 * Screen handler: Incident Preliminary Review (HSE_TgAcdntcnfrmtn)
 * Menu path: Safety -> Incident -> Incident Preliminary Review (from HSE.json)
 * C++: AccidentConfirmationCategory – ACDNTENT_CNFRM (ConfirmAccident), ACDNTENTINJRDPRSN_INJRDBODYPART (OpenBodyParts); delegate to backend.
 * RQ_HSE_13_3_26_4_18
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { handleAccidentInjuredPersonCustomButton } from '../../../utils/incidentInjuredPersonPopups.js';
import { getDevInterface } from '../../../events/buttonEvents.js';
import {
  getAccidentConfirmationPolicyAllowsReviewScreen,
  INCIDENT_PRELIMINARY_REVIEW_SCREEN_TAG,
} from '../../../services/ModuleButtonHandlers/moduleButtonHandlersUtils.js';

/** Same text as ShowScreen / desktop Policy change required. */
const ACCIDENT_CONFIRMATION_POLICY_MSG =
  'Accident Confirmation must be enabled in HSE policy to use this screen. Policy change required.';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'ACDNTENT_CNFRM' || btn === 'ACDNTENTINJRDPRSN_INJRDBODYPART') {
    // RQ_HSE_13_3_26_4_18
    if (await handleAccidentInjuredPersonCustomButton(devInterfaceObj, btn, strScrTag)) return;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (btn === 'ACDNTENT_CNFRM' && typeof doToolbarAction === 'function') {
      doToolbarAction('SAVE', eventObj.strFormTag || strScrTag, '');
    }
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

/**
 * C++ AccidentConfirmationCategory::OpenScreen — CheckPolicy HSEPLC_ACDNTCNFRMTNRQRD.
 * If accident confirmation is not enabled in HSE policy, block use of this screen (desktop: Policy change required).
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  try {
    const allowed = await getAccidentConfirmationPolicyAllowsReviewScreen(devInterfaceObj || {});
    if (!allowed) {
      const msg = ACCIDENT_CONFIRMATION_POLICY_MSG;
      if (typeof devInterfaceObj?.showMessage === 'function') {
        devInterfaceObj.showMessage(msg);
      } else if (devInterfaceObj?.AskYesNoMessage) {
        devInterfaceObj.AskYesNoMessage(msg);
      } else {
        console.warn('[Web_HSE]', msg);
      }
      setScreenDisableBtn(true, true, true);
      return;
    }
    setScreenDisableBtn(false, false, false);
  } catch (e) {
    console.warn('[Web_HSE] Incident Preliminary Review ShowScreen:', e);
    setScreenDisableBtn(false, false, false);
  }
}

/**
 * Block menu navigation when Accident Confirmation policy is off (parity with C++ OpenScreen; complements ShowScreen).
 */
export async function onMenuItemClicked(userObj, strScrTag, callBackFn) {
  const tag = String(strScrTag || '').toUpperCase().trim();
  if (tag !== INCIDENT_PRELIMINARY_REVIEW_SCREEN_TAG) {
    callBackFn({ isAllowed: true, strScrCriteria: '', strMsg: '' });
    return;
  }
  try {
    const allowed = await getAccidentConfirmationPolicyAllowsReviewScreen(getDevInterface() || {});
    if (!allowed) {
      callBackFn({
        isAllowed: false,
        strScrCriteria: '',
        strMsg: ACCIDENT_CONFIRMATION_POLICY_MSG,
      });
      return;
    }
  } catch (e) {
    console.warn('[Web_HSE] Incident Preliminary Review onMenuItemClicked:', e);
  }
  callBackFn({ isAllowed: true, strScrCriteria: '', strMsg: '' });
}
