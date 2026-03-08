/**
 * Screen handler: Incident Flash Inquiry (HSE_TgIncdntFlshRprtInq)
 * Menu path: Inquiry screens -> Incident Flash Inquiry (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
