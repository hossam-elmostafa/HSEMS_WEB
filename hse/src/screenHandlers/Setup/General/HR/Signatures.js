/**
 * Screen handler: Signatures (HSE_TGSGNTUR)
 * Menu path: Setup -> General -> HR -> Signatures (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
