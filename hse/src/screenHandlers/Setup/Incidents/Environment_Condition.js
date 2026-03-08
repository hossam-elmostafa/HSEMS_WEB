/**
 * Screen handler: Environment Condition (HSE_TGENVRNMNTCND)
 * Menu path: Setup -> Incidents -> Environment Condition (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
