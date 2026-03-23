/**
 * Site Survey — Required Actions popup (desktop SitSrvyCategory::DisplayCustomButtonClicked parity).
 * RQ_HSE_23_3_26_22_44
 */

const TABLE_FNDNG = 'HSE_SITSRVYENTFNDNG';
const FNDNG_LINK = 'MAINLINK';
export const SITE_SURVEY_RQRDACTN_POPUP_TAG = 'HSE_TgSitSrvyRqrdActn';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Open HSE_TgSitSrvyRqrdActn filtered by current finding MAINLINK.
 * @param {Object} devInterfaceObj - openScr, FormGetField
 * @param {string} strScrTag - owning header screen tag (any case)
 */
export function openSiteSurveyRequiredActionsPopup(devInterfaceObj, strScrTag) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof openScr !== 'function' || !FormGetField) return;

  const linkVal = FormGetField(TABLE_FNDNG, FNDNG_LINK, 'scr');
  if (linkVal == null || linkVal === '') return;

  const esc = escapeSqlString(String(linkVal).trim());
  const strCriteria = `SELECT * FROM HSE_SITSRVYRQRDACTN WHERE (SITSRVYRQRDACTN_LNK = '${esc}')`;

  const scr = (strScrTag || '').toUpperCase();
  const defValObj = { SITSRVYRQRDACTN_LNK: linkVal };

  if (scr === 'HSE_TGSITSRVYENT') {
    defValObj.TXNSTS = '1';
  } else if (scr === 'HSE_TGSITSRVYCNFRMTN') {
    defValObj.TXNSTS = '4';
  }

  const bLockScr = scr === 'HSE_TGSITSRVYINQ';

  openScr(SITE_SURVEY_RQRDACTN_POPUP_TAG, {}, strCriteria, 'edit', false, defValObj, false, bLockScr);
}
