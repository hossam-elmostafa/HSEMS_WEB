/**
 * Screen handler: Risk Assessment Inquiry (HSE_TgrskassmntInq)
 * Menu path: Inquiry screens -> Risk Assessment Inquiry (from HSE.json)
 * C++: RiskAssesmentInquiry – filter buttons by RSKASSMNTENT_RECSTS.
 */

const SCREEN_TAG = 'HSE_TgrskassmntInq';
const FIELD = 'RSKASSMNTENT_RECSTS';

const BUTTONS = {
  RSKASSMNTENT_VWINCMPLT: `WHERE (${FIELD}=1)`,
  RSKASSMNTENT_VWAPPRV: `WHERE (${FIELD}=5)`,
  RSKASSMNTENT_VWCLSD: `WHERE (${FIELD}=6)`,
  RSKASSMNTENT_VWCMPLT: `WHERE (${FIELD}=4)`,
  RSKASSMNTENT_VWRJCT: `WHERE (${FIELD}=3)`,
  RSKASSMNTENT_VWALL: '',
};

function applyCriteria(devInterfaceObj, criteria) {
  const ChangeCriteria = devInterfaceObj?.ChangeCriteria;
  const refreshData = devInterfaceObj?.refreshData;
  if (typeof ChangeCriteria === 'function') ChangeCriteria(SCREEN_TAG, '', criteria);
  if (typeof refreshData === 'function') refreshData('');
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const criteria = BUTTONS[btn];
  if (criteria !== undefined) applyCriteria(devInterfaceObj, criteria);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
