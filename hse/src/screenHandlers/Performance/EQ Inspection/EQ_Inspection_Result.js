/**
 * Screen handler: EQ Inspection Result (HSE_EQInspctnRsult)
 * Menu path: Performance -> EQ Inspection -> EQ Inspection Result (from HSE.json)
 * RQ_HSE_23_3_26_23_15: Lifecycle buttons via ModuleButtonHandlers (`eqInspectionButtonHandlers.js`); SubFieldChanged parity.
 */

import { eqInspectionSubFieldChanged } from './eqInspectionSubFieldChanged.js';

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  return eqInspectionSubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
