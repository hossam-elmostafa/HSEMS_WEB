/**
 * Screen handler: Waste Receiving Entry (HSE_TgWstRcvngEntry)
 * Menu path: Environment -> Waste Management -> Waste Receiving Entry (from HSE.json)
 * C++: WstRcvngEntCategory – WSTRCVNGENTRY_VWCMPLTDENTRS, WSTRCVNGENTRY_RTRNTOENTRY, WSTRCVNGENTRY_ENTRYCMPLTD.
 *
 * RQ_HSE_23_3_26_18_00: Added SubFieldChanged (G1 – QOH auto-calc),
 *   MainSubReposition (G2 – record lock on completion).
 */

const SCREEN_TAG = 'HSE_TgWstRcvngEntry';
const KEY_FIELD = 'WstRcvngEntry_PrmKy';
const TABLE_NAME = 'HSE_WSTRCVNGENTRY';

const BTN_VIEW_COMPLETED = 'WSTRCVNGENTRY_VWCMPLTDENTRS';
const BTN_RETURN_TO_ENTRY = 'WSTRCVNGENTRY_RTRNTOENTRY';
const BTN_ENTRY_COMPLETED = 'WSTRCVNGENTRY_ENTRYCMPLTD';

// RQ_HSE_23_3_26_18_00: module-level devInterfaceObj for MainSubReposition / SubFieldChanged
let _devInterfaceObj = {};

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

function getPrimaryKey(eventObj) {
  return getFieldFromRecord(eventObj, KEY_FIELD) || getFieldFromRecord(eventObj, 'PrmKy') || getFieldFromRecord(eventObj, 'PRMKY');
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * View Completed Entries: change criteria to completed only (ENTRYSTTS=5) and lock screen.
 * C++: ChangeCriteria("HSE_TGWSTRCVNGENTRY","","WHERE (WSTRCVNGENTRY_ENTRYSTTS=5)"); LockRecord("","",true);
 */
function handleViewCompletedEntries(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return;
  const strCriteria = 'WHERE (WSTRCVNGENTRY_ENTRYSTTS=5)';
  openScr(SCREEN_TAG, {}, strCriteria, 'list', false, {}, false, true);
}

/**
 * Return completed waste to entry: execute Return_compltd_waste_to_entry, then refresh.
 * C++: returnWasteRcvEntry(strPrmryKy);
 */
async function handleReturnToEntry(eventObj, devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(eventObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE Return_compltd_waste_to_entry '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Waste Receiving Entry Return to entry:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error executing Return to entry');
  }
}

/**
 * Entry Completed: execute Complete_Waste_Entry, then refresh.
 * C++: completeWasteRcvEntry(strPrmryKy);
 */
async function handleEntryCompleted(eventObj, devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPrimaryKey(eventObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE Complete_Waste_Entry '${escapeSqlString(pk)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Waste Receiving Entry Complete:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error executing Entry Completed');
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_18_00  G1 – QOH auto-calculation (C++: WstRcvngEntCategory::DisplayFieldChange)
// ---------------------------------------------------------------------------

/**
 * C++: getQuantityBefore(strZoneCode, strItemCode) —
 *   SELECT WSTZNSALWDITMS_QOH FROM HSE_WSTZNSALWDITMS
 *   WHERE WSTZNSALWDITMS_ITMCOD = @item AND WSTZNSALWDITMS_LNK = @zone
 */
async function getQuantityBefore(executeSQLPromise, zoneCode, itemCode) {
  const sql = `SELECT WSTZNSALWDITMS_QOH FROM HSE_WSTZNSALWDITMS WHERE WSTZNSALWDITMS_ITMCOD = '${escapeSqlString(itemCode)}' AND WSTZNSALWDITMS_LNK = '${escapeSqlString(zoneCode)}'`;
  try {
    const data = await executeSQLPromise(sql);
    const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
    const val = row?.WSTZNSALWDITMS_QOH ?? row?.wstznsalwditms_qoh ?? Object.values(row || {})[0];
    return parseFloat(val) || 0;
  } catch (_e) {
    return 0;
  }
}

/**
 * SubFieldChanged – called by screenEvents.js when any field changes.
 * Mirrors C++ WstRcvngEntCategory::DisplayFieldChange.
 *
 * RQ_HSE_23_3_26_18_00
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  if (fld === 'WSTRCVNGENTRY_TRNSTZN' || fld === 'WSTRCVNGENTRY_ITMCOD') {
    const zoneCode = FormGetField(TABLE_NAME, 'WSTRCVNGENTRY_TRNSTZN', 'scr') || '';
    const itemCode = FormGetField(TABLE_NAME, 'WSTRCVNGENTRY_ITMCOD', 'scr') || '';
    if (zoneCode && itemCode) {
      const qohBefore = await getQuantityBefore(executeSQLPromise, zoneCode, itemCode);
      FormSetField(TABLE_NAME, 'WSTRCVNGENTRY_QOHBFR', String(qohBefore.toFixed(3)), 'scr');
    }
  } else if (fld === 'WSTRCVNGENTRY_RCVDQTY') {
    const qohBefore = parseFloat(FormGetField(TABLE_NAME, 'WSTRCVNGENTRY_QOHBFR', 'scr')) || 0;
    const rcvdQty = parseFloat(String(fieldVal ?? '').trim()) || 0;
    const qohAfter = qohBefore + rcvdQty;
    FormSetField(TABLE_NAME, 'WSTRCVNGENTRY_QOHAFTR', String(qohAfter.toFixed(3)), 'scr');
  }

  return { cancel: 0 };
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_18_00  G2 – Record lock when completed (C++: WstMngmntCommonCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

/**
 * MainSubReposition – called by screenEvents.js on row navigation.
 * C++: if(ENTRYSTTS=="5") { DisableToolbar DELETE/NEW/SAVE; LockRecord; }
 *
 * RQ_HSE_23_3_26_18_00
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  const status = FormGetField(TABLE_NAME, 'WSTRCVNGENTRY_ENTRYSTTS', 'scr');
  const n = parseInt(String(status ?? '').trim(), 10);
  if (Number.isFinite(n) && n >= 5) {
    setScreenDisableBtn(true, true, true);
  } else {
    setScreenDisableBtn(false, false, false);
  }
}

// ---------------------------------------------------------------------------
// Button handling (existing)
// ---------------------------------------------------------------------------

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
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
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
