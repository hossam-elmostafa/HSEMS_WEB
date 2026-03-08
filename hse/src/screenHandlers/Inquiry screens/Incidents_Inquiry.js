/**
 * Screen handler: Incidents Inquiry (HSE_TgAcdntInq)
 * Menu path: Inquiry screens -> Incidents Inquiry (from HSE.json)
 * C++: LossAccidentInq – filter buttons by LOSSACDNTENT_RECSTS.
 */

const SCREEN_TAG = 'HSE_TgAcdntInq';
const FIELD = 'LOSSACDNTENT_RECSTS';

const BUTTONS = {
  LOSSACDNTENT_VWINCMPLT: `WHERE (${FIELD}=1)`,
  LOSSACDNTENT_VWCNFRM: `WHERE (${FIELD}=5)`,
  LOSSACDNTENT_VWCLOSD: `WHERE (${FIELD}=6)`,
  LOSSACDNTENT_VWCMPLT: `WHERE (${FIELD}=4)`,
  LOSSACDNTENT_VWRJCT: `WHERE (${FIELD}=3)`,
  LOSSACDNTENT_VWALL: '',
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
