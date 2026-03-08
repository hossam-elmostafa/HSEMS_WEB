/**
 * Screen handler: Clause Group (HSE_TgClausGrp)
 * Menu path: Setup -> Audits -> Clause Group (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
