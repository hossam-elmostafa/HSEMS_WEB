/**
 * Screen handler: Incident Flash Review (HSE_TgIncdntFlshRprtRvew)
 * Menu path: Safety -> Incident -> Incident Flash Review (from HSE.json)
 * C++: IncdntFlshRprtCategory – INCDNTFLSHRPRTRVEW_RJCT (RejectFlashFromReview), INCDNTFLSHRPRTRVEW_ACCPT (AccptFlashFromReview).
 */

const TABLE = 'HSE_INCDNTFLSHRPRT';
const KEY_FIELD = 'MAINLINK';

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;

  if (btn === 'INCDNTFLSHRPRTRVEW_RJCT') {
    try {
      await executeSQLPromise(`EXECUTE RejectFlashFromReview '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Incident Flash Review Reject:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error rejecting');
    }
    return;
  }
  if (btn === 'INCDNTFLSHRPRTRVEW_ACCPT') {
    try {
      await executeSQLPromise(`EXECUTE AccptFlashFromReview '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Incident Flash Review Accept:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error accepting');
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
