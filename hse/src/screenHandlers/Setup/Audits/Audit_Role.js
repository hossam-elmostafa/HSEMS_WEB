/**
 * Screen handler: Audit Role (HSE_AudtRl)
 * Menu path: Setup -> Audits -> Audit Role (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
