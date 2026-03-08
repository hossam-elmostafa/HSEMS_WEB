/**
 * Screen handler: Site Survey Inquiry (HSE_TgSitSrvyInq)
 * Menu path: Inquiry screens -> Site Survey Inquiry (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
