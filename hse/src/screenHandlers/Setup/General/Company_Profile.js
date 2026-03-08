/**
 * Screen handler: Company Profile (HSE_TgCompPrf)
 * Menu path: Setup -> General -> Company Profile (from HSE.json)
 * C++: ShowCompanyProfile sets m_bEditMode then ParentManage; this handler mirrors edit mode by enabling toolbar.
 */

/**
 * Handle ShowScreen for Company Profile: enable toolbar (mirrors C++ m_bEditMode = true).
 * No observation tabs or field-type mapping for this screen.
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
