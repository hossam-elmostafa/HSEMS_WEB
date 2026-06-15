/**
 * Screen handler: Waste Disposal Entry (HSE_TgWstDspslEntry)
 * Menu path: Environment -> Waste Management -> Waste Disposal Entry (from HSE.json)
 * C++: WstDspslEntCategory – WSTDSPSLENTRY_VWCMPLTDENTRS, WSTDSPSLENTRY_RTRNTOENTRY, WSTDSPSLENTRY_ENTRYCMPLTD.
 *
 * RQ_HSE_23_3_26_18_00: Added SubFieldChanged (G4 – QOH validation with isItemRqrChkQOH),
 *   toolBarButtonClicked (G3 – manifest form number auto-generation on NEW),
 *   MainSubReposition (G2 – record lock on completion).
 */

const SCREEN_TAG = 'HSE_TgWstDspslEntry';
const KEY_FIELD = 'WstDspslEntry_PrmKy';
const TABLE_NAME = 'HSE_WSTDSPSLENTRY';
const DSPSLTAB_TABLE = 'HSE_WSTDSPSLENTRYDSPSLITMS';

const BTN_VIEW_COMPLETED = 'WSTDSPSLENTRY_VWCMPLTDENTRS';
const BTN_RETURN_TO_ENTRY = 'WSTDSPSLENTRY_RTRNTOENTRY';
const BTN_ENTRY_COMPLETED = 'WSTDSPSLENTRY_ENTRYCMPLTD';

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

function getPrimaryKey(eventObj, devInterfaceObj) {
  if (devInterfaceObj?.FormGetField) {
    const pk = devInterfaceObj.FormGetField(TABLE_NAME, 'WSTDSPSLENTRY_PRMKY', 'scr');
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

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_18_00  G4 – QOH validation (C++: WstDspslEntCategory::DisplayFieldChange)
// ---------------------------------------------------------------------------

/**
 * C++: isItemRqrChkQOH(strItemKey) —
 *   SELECT ISNULL(WSTITMS_CHKQOH,'YES') FROM HSE_WSTITMS WHERE WSTITMS_ITMCD = @item
 *   Returns false when result is 'NO'.
 */
async function isItemRqrChkQOH(executeSQLPromise, itemCode) {
  const sql = `SELECT ISNULL(WSTITMS_CHKQOH,'YES') AS CHKQOH FROM HSE_WSTITMS WHERE WSTITMS_ITMCD = '${escapeSqlString(itemCode)}'`;
  try {
    const data = await executeSQLPromise(sql);
    const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
    const val = String(row?.CHKQOH ?? row?.chkqoh ?? Object.values(row || {})[0] ?? 'YES').toUpperCase().trim();
    return val !== 'NO';
  } catch (_e) {
    return true;
  }
}

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
 * Mirrors C++ WstDspslEntCategory::DisplayFieldChange.
 *
 * RQ_HSE_23_3_26_18_00
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField, executeSQLPromise, AskYesNoMessage } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  // C++: zone or item changed (disposal items tab)
  if (fld === 'WSTDSPSLENTRY_TRNSTZN' || fld === 'WSTDSPSLENTRYDSPSLITMS_ITMCD') {
    const directDisposal = String(FormGetField(TABLE_NAME, 'WSTDSPSLENTRY_DRCTDSPSL', 'scr') || '').toUpperCase();
    if (directDisposal === 'N') {
      const zoneCode = FormGetField(TABLE_NAME, 'WSTDSPSLENTRY_TRNSTZN', 'scr') || '';
      const itemCode = FormGetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_ITMCD', 'tab') || '';
      if (zoneCode && itemCode) {
        const reqQOH = await isItemRqrChkQOH(executeSQLPromise, itemCode);
        if (reqQOH) {
          const qohBefore = await getQuantityBefore(executeSQLPromise, zoneCode, itemCode);
          FormSetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_QOHBFR', String(qohBefore.toFixed(3)), 'tab');
        } else {
          FormSetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_QOHBFR', '0', 'tab');
          FormSetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_QOHAFTR', '0', 'tab');
        }
      }
    }
    return { cancel: 0 };
  }

  // C++: disposal qty changed
  if (fld === 'WSTDSPSLENTRYDSPSLITMS_DSPSLQTY') {
    const itemCode = FormGetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_ITMCD', 'tab') || '';
    const reqQOH = await isItemRqrChkQOH(executeSQLPromise, itemCode);
    if (reqQOH) {
      const qohBefore = parseFloat(FormGetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_QOHBFR', 'tab')) || 0;
      const dspslQty = parseFloat(String(fieldVal ?? '').trim()) || 0;
      if (dspslQty > qohBefore) {
        // C++: ShowMessageEx(IDS_WST_QTY_NOT_VALID,...); FormSetField("","DSPSLQTY","");
        if (typeof AskYesNoMessage === 'function') {
          AskYesNoMessage('Disposal quantity exceeds available stock (QOH). Please enter a valid quantity.');
        }
        FormSetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_DSPSLQTY', '', 'tab');
        return { cancel: 1 };
      }
      const qohAfter = qohBefore - dspslQty;
      FormSetField(DSPSLTAB_TABLE, 'WSTDSPSLENTRYDSPSLITMS_QOHAFTR', String(qohAfter.toFixed(3)), 'tab');
    }
    return { cancel: 0 };
  }

  return { cancel: 0 };
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_18_00  G3 – Manifest form number on NEW (C++: WstDspslEntCategory::DisplayToolBarButtonClicked)
// ---------------------------------------------------------------------------

/**
 * toolBarButtonClicked – called by buttonEvents.js on toolbar actions.
 * On NEW (main form, after complete): auto-generate next manifest form number.
 * C++: getManifestFrmNo() → SELECT ISNULL(MAX(WSTDSPSLENTRY_MNFSTFRMNO)+1, 1)
 *
 * RQ_HSE_23_3_26_18_00
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const { strBtnName, strTabTag, complete, devInterfaceObj: evtDev } = eventObj;
  const dev = evtDev || _devInterfaceObj;
  _devInterfaceObj = dev;

  const btn = String(strBtnName || '').toUpperCase();
  const tab = String(strTabTag || '').toUpperCase();

  if (btn === 'NEW' && complete === 1 && tab === '') {
    const { executeSQLPromise, FormSetField } = dev;
    if (executeSQLPromise && FormSetField) {
      try {
        const sql = "SELECT ISNULL(MAX(WSTDSPSLENTRY_MNFSTFRMNO)+1, 1) AS MnfstFrmNo FROM HSE_WSTDSPSLENTRY";
        const data = await executeSQLPromise(sql);
        const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
        const val = row?.MnfstFrmNo ?? row?.MNFSTFRMNO ?? row?.mnfstfrmno ?? Object.values(row || {})[0];
        if (val != null) {
          FormSetField(TABLE_NAME, 'WSTDSPSLENTRY_MNFSTFRMNO', String(val), 'scr');
        }
      } catch (e) {
        console.warn('[Web_HSE] Waste Disposal manifest number generation:', e);
      }
    }
  }

  callBackFn(eventObj);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_18_00  G2 – Record lock when completed (C++: WstDspslEntCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

/**
 * MainSubReposition – called by screenEvents.js on row navigation.
 * C++: if(ENTRYSTTS >= 5) { DisableToolbar DELETE; }
 * Base class also disables NEW/SAVE and locks record when status=="5".
 *
 * RQ_HSE_23_3_26_18_00
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  const status = FormGetField(TABLE_NAME, 'WSTDSPSLENTRY_ENTRYSTTS', 'scr');
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
