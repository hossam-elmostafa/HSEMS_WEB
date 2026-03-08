/**
 * Screen handler: Drill Inquiry (HSE_DRLLPLNINQ)
 * Menu path: Inquiry screens -> Drill Inquiry (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
