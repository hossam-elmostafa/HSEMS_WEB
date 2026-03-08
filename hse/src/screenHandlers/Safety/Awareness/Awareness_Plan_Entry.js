/**
 * Screen handler: Awareness Plan Entry (HSE_TgAwrnsPlnEntry)
 * Menu path: Safety -> Awareness -> Awareness Plan Entry (from HSE.json)
 * C++: AwrnsPlnEntryCategory – AWRNSPLNENTRY_PLNENTRYCMPLTD (AwrnsPlnEntryCompleted).
 */

const TABLE = 'HSE_AWRNSPLN';
const KEY_FIELD = 'SerialPK';

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn !== 'AWRNSPLNENTRY_PLNENTRYCMPLTD') return;
  const { FormGetField, executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!FormGetField || !executeSQLPromise) return;
  const pk = FormGetField(TABLE, KEY_FIELD, 'scr');
  if (pk == null || pk === '') return;
  try {
    await executeSQLPromise(`EXECUTE AwrnsPlnEntryCompleted ${pk}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Awareness Plan Entry Complete:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error completing entry');
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
