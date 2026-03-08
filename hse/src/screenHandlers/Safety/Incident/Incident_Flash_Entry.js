/**
 * Screen handler: Incident Flash Entry (HSE_TgIncdntFlshRprt)
 * Menu path: Safety -> Incident -> Incident Flash Entry (from HSE.json)
 * C++: IncdntFlshRprtCategory – NCDNTFLSHRPRT_ENTRYCMPLT (CompleteFlashEntry).
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
  if (btn !== 'NCDNTFLSHRPRT_ENTRYCMPLT') return;
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE CompleteFlashEntry '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Incident Flash Entry Complete:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error completing flash entry');
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
