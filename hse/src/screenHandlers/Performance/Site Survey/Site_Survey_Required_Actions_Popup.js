/**
 * Screen handler: Site Survey Required Actions popup (HSE_TgSitSrvyRqrdActn)
 * C++: SitSrvyCategory::DisplayFieldChange — SITSRVYRQRDACTN_ACTNNO auto-serial.
 * C++: SitSrvyRqrdActn — GENERATECAR, ViewRelatedCAR, UpdtCAR, CnlCAR, WrkAcptd, WrkNotAcptd (carTxnRequiredActionsSP).
 * RQ_HSE_23_3_26_22_44
 * RQ_HSE_12_4_26_00_40 — GAP-7: route custom CAR buttons through handleTxnRequiredActionCarButton before backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { handleTxnRequiredActionCarButton } from '../../../utils/carTxnRequiredActionsSP.js';
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

/**
 * RQ_HSE_12_4_26_00_40 — GAP-7: Site Survey required-actions CAR / work buttons (sp_Generate_CARs, etc.)
 */
export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  if (!Button_Name) return;
  const dev = devInterfaceObj || _devInterfaceObj;
  const consumed = await handleTxnRequiredActionCarButton(Button_Name, strScrTag, eventObj, dev);
  if (consumed) return;
  sendButtonClickToBackend(String(Button_Name).toUpperCase(), strScrTag, eventObj, dev);
}
