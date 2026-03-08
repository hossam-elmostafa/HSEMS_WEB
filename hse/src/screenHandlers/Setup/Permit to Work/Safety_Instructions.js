/**
 * Screen handler: Safety Instructions (HSE_TgSftyInstrctn)
 * Menu path: Setup -> Permit to Work -> Safety Instructions (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
