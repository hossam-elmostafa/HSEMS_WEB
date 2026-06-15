/**
 * RQ_HSE_11_4_26_15_44 — G_ABS_7
 * Screen handler: Observation Policy (HSE_TgHseobsrvtn / HSE_HSEOBSRVTN)
 * Menu path: Setup -> Observations -> Observation (policy screen)
 *
 * C++: Observation policy exit check — HSEOBSRVTN_GnrCrStt must be non-empty.
 * On SAVE pre-phase, validate that Generated CAR Status has a value; block save if empty.
 */

import { mapUnknownFieldTypes } from '../../../utils/fieldTypeMapper.js';
import { validateObservationPolicyGnrCrStt } from '../../../utils/observationBusinessRules.js';

// RQ_HSE_11_4_26_15_44 — G_ABS_7: SAVE gate for policy GnrCrStt
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;
  const completeStr = complete == null ? '' : String(complete).trim();
  const isPrePhase =
    !(complete === 1 || completeStr === '1') &&
    (complete == null || complete === '' || complete === 0 || complete === false || completeStr === '0');

  if (strBtnName === 'SAVE' && isPrePhase) {
    const check = validateObservationPolicyGnrCrStt(devInterface);
    if (!check.ok) {
      if (devInterface?.toast?.warning) {
        devInterface.toast.warning(check.message);
      }
      callBackFn({ ...eventObj, cancel: true });
      return;
    }
  }

  callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);
  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Policy ShowScreen:', error);
  }
}
