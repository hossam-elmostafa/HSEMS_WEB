/**
 * Screen handler: PTW Type (HSE_TgPTWsTyp)
 * Menu path: Setup -> Permit to Work -> PTW Type (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
