/**
 * Screen handler: Aspects Review (HSE_AspctsRvwAtRvw)
 * Menu path: Environment -> Aspects Register -> Aspects Review (from HSE.json)
 * C++: EnvrnmntAspctReviewCategory – ACCEPTED (EnvAspctRvwAccpt), REJECTED (EnvAspctRjct).
 */

const TABLE_VIEW = 'HSE_ASPCTS_VIEW';
const KEY_FIELD = 'PrmryKy';

const BTN_ACCEPTED = 'ACCEPTED';
const BTN_REJECTED = 'REJECTED';

function getPrimaryKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_VIEW, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Accept: EXECUTE EnvAspctRvwAccpt '<PrmryKy>', then refresh.
 */
async function handleAccept(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE EnvAspctRvwAccpt '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Aspects Review Accept:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error accepting');
  }
}

/**
 * Reject: EXECUTE EnvAspctRjct '<PrmryKy>', then refresh.
 */
async function handleReject(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE EnvAspctRjct '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Aspects Review Reject:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error rejecting');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_ACCEPTED) {
    await handleAccept(devInterfaceObj);
    return;
  }
  if (btn === BTN_REJECTED) {
    await handleReject(devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
