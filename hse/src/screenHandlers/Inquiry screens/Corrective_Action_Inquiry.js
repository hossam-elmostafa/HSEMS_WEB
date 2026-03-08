/**
 * Screen handler: Corrective Action Inquiry (HSE_TgcrctveAccInq)
 * Menu path: Inquiry screens -> Corrective Action Inquiry (from HSE.json)
 * C++: CARInquiry – filter buttons; uses full SQL SELECT for criteria.
 */

const SCREEN_TAG = 'HSE_TgcrctveAccInq';
const TABLE = 'HSE_CRCTVEACCENT';
const FIELD = 'CRCTVEACCENT_RECSTS';

const BUTTONS = {
  CRCTVEACCENT_VWINCMP: `SELECT * FROM ${TABLE} WHERE (${FIELD}=1)`,
  CRCTVEACCENT_VWRJCT: `SELECT * FROM ${TABLE} WHERE (${FIELD}=3)`,
  CRCTVEACCENT_VWCMP: `SELECT * FROM ${TABLE} WHERE (${FIELD}=4)`,
  CRCTVEACCENT_VWCNFRM: `SELECT * FROM ${TABLE} WHERE (${FIELD}=5)`,
  CRCTVEACCENT_VWCLSD: `SELECT * FROM ${TABLE} WHERE (${FIELD}=6)`,
  CRCTVEACCENT_VWALL: '',
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
