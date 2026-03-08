/**
 * Screen handler: Schedule Change Reason (HSE_SchdulChngRsn)
 * Menu path: Setup -> Audits -> Schedule Change Reason (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
