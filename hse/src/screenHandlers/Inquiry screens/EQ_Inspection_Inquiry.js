/**
 * Screen handler: EQ Inspection Inquiry (HSE_EQInspctnInqry)
 * Menu path: Inquiry screens -> EQ Inspection Inquiry (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
