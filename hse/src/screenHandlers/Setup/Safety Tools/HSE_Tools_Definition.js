/**
 * Screen handler: HSE Tools Definition (HSE_TGTOOLEQUIP)
 * Menu path: Setup -> Safety Tools -> HSE Tools Definition (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
