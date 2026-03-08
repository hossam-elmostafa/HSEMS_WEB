/**
 * Screen handler: Rescue Entities (HSE_TgRscuEntts)
 * Menu path: Setup -> Rescue Plans -> Rescue Entities (from HSE.json)
 * Minimal handler: enable toolbar on ShowScreen.
 */

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
