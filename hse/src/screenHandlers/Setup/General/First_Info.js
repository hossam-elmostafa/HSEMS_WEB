/**
 * Screen handler: First Info (HSE_TgFirstInfo)
 * Menu path: Setup -> General -> First Info (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
