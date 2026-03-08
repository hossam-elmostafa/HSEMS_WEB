/**
 * Screen handler: Incident Period (HSE_TgIncdntPrd)
 * Menu path: Setup -> Incidents -> Incident Period (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
