/**
 * Screen handler: Internal Resource (HSE_IntrnlRsurc)
 * Menu path: Setup -> Emergency Drills -> Internal Resource (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
