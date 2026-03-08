/**
 * Screen handler: PTW Responsibility (HSE_TgPTWsRspnsblty)
 * Menu path: Setup -> Permit to Work -> PTW Responsibility (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
