/**
 * Screen handler: Certificate Provider (HSE_CrtfctPrvdr)
 * Menu path: Setup -> Audits -> Certificate Provider (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
