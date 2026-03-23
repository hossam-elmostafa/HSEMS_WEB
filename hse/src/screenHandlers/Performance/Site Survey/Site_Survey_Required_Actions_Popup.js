/**
 * Screen handler: Site Survey Required Actions popup (HSE_TgSitSrvyRqrdActn)
 * C++: SitSrvyCategory::DisplayFieldChange — SITSRVYRQRDACTN_ACTNNO auto-serial.
 * RQ_HSE_23_3_26_22_44
 */

import { applyRequiredActionSerialIfEmpty } from './siteSurveySerialUtils.js';

let _devInterfaceObj = {};

/**
 * C++: if field name matches required-actions form and SITSRVYRQRDACTN_ACTNNO empty → getNewSerialNoForATab.
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  if (!dev?.FormGetField) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();
  if (fld.indexOf('SITSRVYRQRDACTN') < 0) return { cancel: 0 };

  await applyRequiredActionSerialIfEmpty(dev);
  return { cancel: 0 };
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
