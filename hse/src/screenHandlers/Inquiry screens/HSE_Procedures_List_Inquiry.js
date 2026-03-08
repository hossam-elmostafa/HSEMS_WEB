/**
 * Screen handler: HSE Procedures List Inquiry (HSE_TGHSEPRCDRLISTINQ)
 * Menu path: Inquiry screens -> HSE Procedures List Inquiry (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
