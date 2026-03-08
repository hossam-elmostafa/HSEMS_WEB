/**
 * Screen handler: Potential Severity (HSE_TGPTNTLSVRTY)
 * Menu path: Setup -> Incidents -> Potential Severity (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
