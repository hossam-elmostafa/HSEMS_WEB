/**
 * Screen handler: PTW Registry Inquiry (HSE_TgPTWsrgstrInq)
 * Menu path: Inquiry screens -> PTW Registry Inquiry (from HSE.json)
 * C++: PTWInquiry – filter buttons by PTWSRGSTRENT_RECSTS.
 */

const SCREEN_TAG = 'HSE_TgPTWsrgstrInq';
const FIELD = 'PTWSRGSTRENT_RECSTS';

const BUTTONS = {
  PTWSRGSTRENT_VWINCMPLT: `WHERE (${FIELD}=1)`,
  PTWSRGSTRENT_VWCNFRM: `WHERE (${FIELD}=5)`,
  PTWSRGSTRENT_VWCLSD: `WHERE (${FIELD}=6)`,
  PTWSRGSTRENT_VWCMPLT: `WHERE (${FIELD}=4)`,
  PTWSRGSTRENT_VWRJCT: `WHERE (${FIELD}=3)`,
  PTWSRGSTRENT_VWALL: '',
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
