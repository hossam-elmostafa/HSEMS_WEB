/**
 * Screen handler: Control Measures (HSE_TGHRCTRL)
 * Menu path: Setup -> Risks -> Control Measures (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
