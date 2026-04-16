/**
 * RQ_HSE_12_4_26_00_40 — GAP-3: Screen handler for Corrective Action Inquiry (Action Tracking path).
 * C++: CARInquiry — 6 filter buttons mapping to CRCTVEACCENT_RECSTS values.
 * Read-only inquiry screen; buttons change list criteria.
 *
 * Handles BOTH button name conventions (desktop-style and alternate):
 *   CRCTVEACCENT_VW* (Inquiry screens buttons)  AND  CRCTVEACCINQ_VIEW* (Action Tracking buttons).
 */

const SCREEN_TAG = 'HSE_TgCrctveAccInq_AT';
const TABLE = 'HSE_CRCTVEACCENT';
const FIELD = 'CRCTVEACCENT_RECSTS';

const BUTTONS = {
  // Desktop / Inquiry convention
  CRCTVEACCENT_VWINCMP: `SELECT * FROM ${TABLE} WHERE (${FIELD}=1)`,
  CRCTVEACCENT_VWRJCT: `SELECT * FROM ${TABLE} WHERE (${FIELD}=3)`,
  CRCTVEACCENT_VWCMP: `SELECT * FROM ${TABLE} WHERE (${FIELD}=4)`,
  CRCTVEACCENT_VWCNFRM: `SELECT * FROM ${TABLE} WHERE (${FIELD}=5)`,
  CRCTVEACCENT_VWCLSD: `SELECT * FROM ${TABLE} WHERE (${FIELD}=6)`,
  CRCTVEACCENT_VWALL: '',
  // Alternate naming convention
  CRCTVEACCINQ_VIEWINCOMPLETE: `SELECT * FROM ${TABLE} WHERE (${FIELD}=1)`,
  CRCTVEACCINQ_VIEWREJECTED: `SELECT * FROM ${TABLE} WHERE (${FIELD}=3)`,
  CRCTVEACCINQ_VIEWCOMPLETED: `SELECT * FROM ${TABLE} WHERE (${FIELD}=4)`,
  CRCTVEACCINQ_VIEWCONFIRMED: `SELECT * FROM ${TABLE} WHERE (${FIELD}=5)`,
  CRCTVEACCINQ_VIEWCLOSED: `SELECT * FROM ${TABLE} WHERE (${FIELD}=6)`,
  CRCTVEACCINQ_VIEWALL: '',
};

export async function ButtonClicked(eventObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-3
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const criteria = BUTTONS[btn];
  if (criteria !== undefined && typeof devInterfaceObj?.ChangeCriteria === 'function') {
    devInterfaceObj.ChangeCriteria(SCREEN_TAG, '', criteria);
    if (typeof devInterfaceObj.refreshData === 'function') devInterfaceObj.refreshData('');
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  // RQ_HSE_12_4_26_00_40 — GAP-3: read-only inquiry — disable NEW/DELETE
  setScreenDisableBtn(true, false, true);
}
