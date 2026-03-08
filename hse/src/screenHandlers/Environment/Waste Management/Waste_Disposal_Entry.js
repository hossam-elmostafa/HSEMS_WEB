/**
 * Screen handler: Waste Disposal Entry (HSE_TgWstDspslEntry)
 * Menu path: Environment -> Waste Management -> Waste Disposal Entry (from HSE.json)
 * C++: WstDspslEntCategory – WSTDSPSLENTRY_VWCMPLTDENTRS, WSTDSPSLENTRY_RTRNTOENTRY, WSTDSPSLENTRY_ENTRYCMPLTD.
 */

const SCREEN_TAG = 'HSE_TgWstDspslEntry';
const KEY_FIELD = 'WstDspslEntry_PrmKy';

const BTN_VIEW_COMPLETED = 'WSTDSPSLENTRY_VWCMPLTDENTRS';
const BTN_RETURN_TO_ENTRY = 'WSTDSPSLENTRY_RTRNTOENTRY';
const BTN_ENTRY_COMPLETED = 'WSTDSPSLENTRY_ENTRYCMPLTD';

function getFieldFromRecord(eventObj, fieldName) {
  const fullRecordArr = eventObj?.fullRecord ?? eventObj?.fullRecordArr ?? [];
  const rec = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
  if (!rec || typeof rec !== 'object') return '';
  const u = String(fieldName).toUpperCase();
  for (const k of Object.keys(rec)) {
    if (k.toUpperCase() === u) return (rec[k] != null ? String(rec[k]) : '').trim();
  }
  return '';
}

function getPrimaryKey(eventObj, devInterfaceObj) {
  if (devInterfaceObj?.FormGetField) {
    const pk = devInterfaceObj.FormGetField('HSE_WSTDSPSLENTRY', 'WSTDSPSLENTRY_PRMKY', 'scr');
    if (pk != null && pk !== '') return String(pk).trim();
  }
  return getFieldFromRecord(eventObj, KEY_FIELD) || getFieldFromRecord(eventObj, 'PrmKy') || getFieldFromRecord(eventObj, 'PRMKY');
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * View Completed Entries: change criteria to completed only (ENTRYSTTS=5) and lock screen.
 * C++: ChangeCriteria("HSE_TGWSTDSPSLENTRY","","WHERE (WSTDSPSLENTRY_ENTRYSTTS=5)"); LockRecord("","",true);
 */
function handleViewCompletedEntries(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return;
  const strCriteria = 'WHERE (WSTDSPSLENTRY_ENTRYSTTS=5)';
  openScr(SCREEN_TAG, {}, strCriteria, 'list', false, {}, false, true);
}

/**
 * Return completed waste disposal to entry: execute Return_compltd_waste_Dspsl_to_entry, then refresh.
 * C++: returnWasteDspslEntry(strPrmryKy);
 */
async function handleReturnToEntry(eventObj, devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(eventObj, devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE Return_compltd_waste_Dspsl_to_entry '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Waste Disposal Entry Return to entry:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error executing Return to entry');
  }
}

/**
 * Entry Completed: execute Complete_Waste_Dspsl_Entry, then refresh.
 * C++: completeDspslEntry(strPrmryKy);
 */
async function handleEntryCompleted(eventObj, devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(eventObj, devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE Complete_Waste_Dspsl_Entry '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Waste Disposal Entry Complete:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error executing Entry Completed');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_VIEW_COMPLETED) {
    handleViewCompletedEntries(devInterfaceObj);
    return;
  }
  if (btn === BTN_RETURN_TO_ENTRY) {
    await handleReturnToEntry(eventObj, devInterfaceObj);
    return;
  }
  if (btn === BTN_ENTRY_COMPLETED) {
    await handleEntryCompleted(eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
