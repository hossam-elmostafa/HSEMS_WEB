/**
 * Screen handler: Site Survey Follow-up / Result Entry (HSE_TgSitSrvyFlwup)
 * Menu path: Performance -> Site Survey -> Site Survey Result Entry (from HSE.json)
 * C++: SitSrvyFlwupCategory – SITSRVYENT_CLS (CloseSiteSurveyTXN); SAVE then close.
 * RQ_HSE_23_3_26_22_44: SITSRVYENT_CLS handled in ModuleButtonHandlers (SAVE + closeSiteSurveyTXN); no ButtonClicked needed here.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
