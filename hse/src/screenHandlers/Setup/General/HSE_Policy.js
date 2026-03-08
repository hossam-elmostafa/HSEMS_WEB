/**
 * Screen handler: HSE Policy (HSE_TgHSEPlc)
 * Menu path: Setup -> General -> HSE Policy (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
