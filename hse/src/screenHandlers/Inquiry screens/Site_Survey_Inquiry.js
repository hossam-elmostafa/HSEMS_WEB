/**
 * Screen handler: Site Survey Inquiry (HSE_TgSitSrvyInq)
 * Menu path: Inquiry screens -> Site Survey Inquiry (from HSE.json)
 * RQ_HSE_23_3_26_22_44: Required Actions popup in read-only/locked mode (desktop SitSrvyCategory parity).
 */

import { openSiteSurveyRequiredActionsPopup } from '../Performance/Site Survey/siteSurveyRequiredActions.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'SITSRVYRQRDACTN_RQRDACTNBTN') {
    openSiteSurveyRequiredActionsPopup(devInterfaceObj, strScrTag);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
