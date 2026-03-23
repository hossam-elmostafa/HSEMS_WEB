/**
 * Screen handler: Site Survey Confirmation (HSE_TgSitSrvyCnfrmtn)
 * Menu path: Performance -> Site Survey -> Site Survey Confirmation (from HSE.json)
 * C++: SitSrvyCnfrmtnCategory – SITSRVYENT_CNFRM (confirmSiteSrvy); Required Actions from SitSrvyCategory.
 * RQ_HSE_23_3_26_22_44: SITSRVYRQRDACTN_RQRDACTNBTN opens popup with TXNSTS=4 (confirm handled in ModuleButtonHandlers).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { openSiteSurveyRequiredActionsPopup } from './siteSurveyRequiredActions.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'SITSRVYRQRDACTN_RQRDACTNBTN') {
    openSiteSurveyRequiredActionsPopup(devInterfaceObj, strScrTag);
    return;
  }
  if (btn === 'SITSRVYENT_CNFRM') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
