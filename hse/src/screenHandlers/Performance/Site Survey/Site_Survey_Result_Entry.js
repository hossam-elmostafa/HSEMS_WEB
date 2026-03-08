/**
 * Screen handler: Site Survey Follow-up / Result Entry (HSE_TgSitSrvyFlwup)
 * Menu path: Performance -> Site Survey -> Site Survey Result Entry (from HSE.json)
 * C++: SitSrvyFlwupCategory – SITSRVYENT_CLS (CloseSiteSurveyTXN); C++ calls DoToolBarAction(SAVE) first.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'SITSRVYENT_CLS') {
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', devInterfaceObj?.strFormTag || strScrTag, '');
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
