/**
 * Screen handler: External Support (HSE_ExtrnlSupprt)
 * Menu path: Setup -> Emergency Drills -> External Support (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
