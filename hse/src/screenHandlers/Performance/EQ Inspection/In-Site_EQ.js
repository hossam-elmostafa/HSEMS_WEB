/**
 * Screen handler: In-Site EQ (HSE_EQInspctnWrk)
 * Menu path: Performance -> EQ Inspection -> In-Site EQ (from HSE.json)
 * RQ_HSE_23_3_26_23_15: On-site actions via ModuleButtonHandlers; SubFieldChanged parity.
 */

import { eqInspectionSubFieldChanged } from './eqInspectionSubFieldChanged.js';

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  return eqInspectionSubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
