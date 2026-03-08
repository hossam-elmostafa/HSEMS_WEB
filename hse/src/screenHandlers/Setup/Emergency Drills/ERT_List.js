/**
 * Screen handler: ERT List (HSE_TgERTLst)
 * Menu path: Setup -> Emergency Drills -> ERT List (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
