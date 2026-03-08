/**
 * Screen handler: Medical Decision (HSE_TgMdclDcsn)
 * Menu path: Setup -> Incidents -> Medical Decision (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
