/**
 * Screen handler: Action Hierarchy Control (HSE_TgActnClssfctn)
 * Menu path: Setup -> CAR -> Action Hierarchy Control (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
