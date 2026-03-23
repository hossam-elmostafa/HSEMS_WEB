/**
 * Screen handler: Chemical Register (HSE_ChmclRgstr)
 * Menu path: Environment -> Chemical Register -> Chemical Register (from HSE.json)
 * C++: ChemicalRegisterCategory – VIEW_ALL_ACTIVE_ITEMS, VIEW_ALL_IN-ACTIVE_ITEMS,
 *      VIEW_DEPARTMENT_ITEMS (change criteria), UPDT (UpdateQuantities).
 *
 * RQ_HSE_23_3_26_20_48: Implemented G1-G6 gaps:
 *   G1 – ShowScreen disables DELETE (C++ DisplayScreenReady).
 *   G2 – toolBarButtonClicked sets LSTMDFYDT / LSTMDFYBY on main SAVE.
 *   G3 – ButtonClicked UPDT calls UpdateQuantities + Chemical_Register_Update_Qty SP.
 *   G4 – SubFieldChanged auto-generates qty card serial (C++ DisplayFieldChange / getNewSerialNoForATab).
 *   G5 – toolBarButtonClicked NEW on Change History copies 9 header fields.
 *   G6 – toolBarButtonClicked SAVE on Change History refreshes screen.
 */

const SCREEN_TAG = 'HSE_ChmclRgstr';
const TABLE_MAIN = 'HSE_CHMCLRGSTR';
const SUBFORM_QTYCRD = 'HSE_CHMCLRGSTR_QTYCRD';
const SUBFORM_CHNGHSTRY = 'HSE_CHMCLRGSTR_CHNGHSTRY';

const BTN_VIEW_ACTIVE = 'VIEW_ALL_ACTIVE_ITEMS';
const BTN_VIEW_INACTIVE = 'VIEW_ALL_IN-ACTIVE_ITEMS';
const BTN_VIEW_DEPARTMENT = 'VIEW_DEPARTMENT_ITEMS';
const BTN_UPDT = 'UPDT';

// RQ_HSE_23_3_26_20_48: module-level devInterfaceObj for SubFieldChanged / MainSubReposition
let _devInterfaceObj = {};

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Change criteria and refresh (C++ ChangeCriteria).
 */
function applyCriteria(devInterfaceObj, criteria) {
  const ChangeCriteria = devInterfaceObj?.ChangeCriteria;
  const refreshData = devInterfaceObj?.refreshData;
  if (typeof ChangeCriteria === 'function' && criteria) {
    ChangeCriteria(SCREEN_TAG, '', criteria);
  }
  if (typeof refreshData === 'function') refreshData('');
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_20_48  G3 – UpdateQuantities (C++ ChemicalRegisterCategory::UpdateQuantities)
// ---------------------------------------------------------------------------

/**
 * C++ logic:
 *   1. If CHMCLRGSTR_QTYCRD_QHBFR is NOT empty → error "already used to update"
 *   2. DoToolBarAction(TOOLBAR_SAVE, main, subform) → save the qty card tab
 *   3. Read masterPK, txnType, detailSerial, txnQty
 *   4. exec Chemical_Register_Update_Qty '@pk', serial, qty, isTypeOne
 *   5. Refresh sub + main
 */
async function updateQuantities(devInterfaceObj) {
  const { FormGetField, executeSQLPromise, DoToolBarAction, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!FormGetField || !executeSQLPromise) return;

  const qtyBfr = String(FormGetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QTYCRD_QHBFR', 'tab') ?? '').trim();
  if (qtyBfr !== '') {
    if (typeof AskYesNoMessage === 'function') {
      AskYesNoMessage('This record was already used to update');
    }
    return;
  }

  if (typeof DoToolBarAction === 'function') {
    DoToolBarAction('SAVE', TABLE_MAIN, SUBFORM_QTYCRD);
  }

  const strMasterPK = String(FormGetField(TABLE_MAIN, 'CHMCLRGSTR_prmryKy', 'scr') ?? '').trim();
  const strTxnType  = String(FormGetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QTYCRD_TXNTYP', 'tab') ?? '').trim();
  const strDtlSrial = String(FormGetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QtyCrd_SrlN', 'tab') ?? '').trim();
  const strTxnQty   = String(FormGetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QtyCrd_TxnQty', 'tab') ?? '').trim();

  const isTypeOne = (strTxnType === '1') ? 1 : 0;
  const sql = `exec Chemical_Register_Update_Qty '${escapeSqlString(strMasterPK)}',${escapeSqlString(strDtlSrial)},${escapeSqlString(strTxnQty)},${isTypeOne}`;

  try {
    const result = await executeSQLPromise(sql);
    if (result < 0) {
      if (typeof refreshData === 'function') refreshData('');
    }
  } catch (e) {
    console.warn('[Web_HSE] Chemical Register UpdateQuantities:', e);
  }

  if (typeof refreshData === 'function') refreshData('');
}

// ---------------------------------------------------------------------------
// Exported handlers
// ---------------------------------------------------------------------------

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, strSubFormTag, strTabTag, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const subForm = (strSubFormTag || strTabTag || '').toString().toUpperCase();

  if (btn === BTN_VIEW_ACTIVE) {
    applyCriteria(devInterfaceObj, "WHERE (ChmclRgstr_Actv = 'Y')");
    return;
  }
  if (btn === BTN_VIEW_INACTIVE) {
    applyCriteria(devInterfaceObj, "WHERE (ChmclRgstr_Actv = 'N')");
    return;
  }
  if (btn === BTN_VIEW_DEPARTMENT) {
    const FormGetField = devInterfaceObj?.FormGetField;
    if (FormGetField) {
      const dprtmnt = FormGetField(TABLE_MAIN, 'ChmclRgstr_DprtmntNm', 'scr');
      const val = (dprtmnt != null && dprtmnt !== '') ? escapeSqlString(dprtmnt) : '';
      if (val) applyCriteria(devInterfaceObj, `WHERE (ChmclRgstr_DprtmntNm = '${val}')`);
    }
    return;
  }
  // RQ_HSE_23_3_26_20_48 G3: UPDT on qty card → UpdateQuantities
  if (subForm === SUBFORM_QTYCRD && btn === BTN_UPDT) {
    await updateQuantities(devInterfaceObj);
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_20_48  G1 – ShowScreen disables DELETE (C++ DisplayScreenReady)
// ---------------------------------------------------------------------------

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  // C++: EnableToolbar("", TOOLBAR_DELETE, FALSE)
  setScreenDisableBtn(false, false, true);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_20_48  G2 + G5 + G6 – toolBarButtonClicked
// ---------------------------------------------------------------------------

/**
 * RQ_HSE_23_3_26_20_48
 * G2: SAVE (main, complete) → set LSTMDFYDT + LSTMDFYBY
 * G5: NEW  (Change History tab, complete) → copy 9 header fields
 * G6: SAVE (Change History tab, complete) → refresh screen
 */

const CHANGE_HISTORY_FIELD_MAP = [
  ['CHMCLRGSTR_PURPS',        'CHMCLRGSTR_CHNGHSTRY_PURPS'],
  ['CHMCLRGSTR_SGNLWRD',      'CHMCLRGSTR_CHNGHSTRY_SGNLWRD'],
  ['CHMCLRGSTR_HZRDSTTMNT',   'CHMCLRGSTR_CHNGHSTRY_HZRDSTTMNT'],
  ['CHMCLRGSTR_PRCUTNSTTMNT', 'CHMCLRGSTR_CHNGHSTRY_PRCUTNSTTMNT'],
  ['CHMCLRGSTR_RLTDSUPPLR',   'CHMCLRGSTR_CHNGHSTRY_RLTDSUPPLR'],
  ['CHMCLRGSTR_WST',           'CHMCLRGSTR_CHNGHSTRY_WST'],
  ['CHMCLRGSTR_QTY',           'CHMCLRGSTR_CHNGHSTRY_QTY'],
  ['CHMCLRGSTR_UOM',           'CHMCLRGSTR_CHNGHSTRY_UOM'],
  ['CHMCLRGSTR_FRQUNCY',      'CHMCLRGSTR_CHNGHSTRY_FRQUNCY'],
];

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const { strBtnName, strTabTag, complete, devInterfaceObj: evtDev } = eventObj;
  const dev = evtDev || _devInterfaceObj;
  _devInterfaceObj = dev;

  const btn = String(strBtnName || '').toUpperCase();
  const tab = String(strTabTag || '').toUpperCase();

  try {
    if (complete === 1) {
      const { FormGetField, FormSetField, refreshData } = dev;

      // G2: main-form SAVE → stamp last modified date and user
      if (btn === 'SAVE' && tab === '') {
        if (FormSetField) {
          const now = new Date();
          const mdy = `${String(now.getMonth() + 1).padStart(2, '0')}/${String(now.getDate()).padStart(2, '0')}/${now.getFullYear()}`;
          FormSetField(TABLE_MAIN, 'CHMCLRGSTR_LSTMDFYDT', mdy, 'scr');
          if (typeof dev.GetUserLogin === 'function') {
            FormSetField(TABLE_MAIN, 'CHMCLRGSTR_LSTMDFYBY', dev.GetUserLogin(), 'scr');
          }
        }
      }

      // G5: NEW on Change History tab → copy 9 header fields to new row
      if (btn === 'NEW' && tab === SUBFORM_CHNGHSTRY) {
        if (FormGetField && FormSetField) {
          for (const [srcField, dstField] of CHANGE_HISTORY_FIELD_MAP) {
            const val = FormGetField(TABLE_MAIN, srcField, 'scr') ?? '';
            FormSetField(SUBFORM_CHNGHSTRY, dstField, String(val), 'tab');
          }
        }
      }

      // G6: SAVE on Change History tab → refresh (C++ FormMakeRefresh(2))
      if (btn === 'SAVE' && tab === SUBFORM_CHNGHSTRY) {
        if (typeof refreshData === 'function') refreshData('');
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Chemical Register toolBarButtonClicked:', error);
  }

  callBackFn(eventObj);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_20_48  G4 – SubFieldChanged: qty card serial auto-generation
// ---------------------------------------------------------------------------

/**
 * C++ DisplayFieldChange: when any field containing "CHMCLRGSTR_QTYCRD" changes,
 * if serial (CHMCLRGSTR_QTYCRD_SrlN) is empty → getNewSerialNoForATab.
 *
 * getNewSerialNoForATab("HSE_CHMCLRGSTR","ChmclRgstr_PrmryKy",
 *     "HSE_CHMCLRGSTR_QTYCRD","MainLink","CHMCLRGSTR_QTYCRD_SrlN")
 *   → SELECT ISNULL(MAX(CHMCLRGSTR_QTYCRD_SrlN)+1,1)
 *     FROM HSE_CHMCLRGSTR_QTYCRD WHERE MainLink = '<masterPK>'
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  if (fld.indexOf('CHMCLRGSTR_QTYCRD') >= 0) {
    const serialVal = String(FormGetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QTYCRD_SrlN', 'tab') ?? '').trim();
    if (serialVal === '') {
      const masterPK = String(FormGetField(TABLE_MAIN, 'ChmclRgstr_PrmryKy', 'scr') ?? '').trim();
      if (masterPK) {
        try {
          const sql = `SELECT ISNULL(MAX(CHMCLRGSTR_QTYCRD_SrlN)+1,1) AS NextSrl FROM HSE_CHMCLRGSTR_QTYCRD WHERE MainLink = '${escapeSqlString(masterPK)}'`;
          const data = await executeSQLPromise(sql);
          const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
          const nextVal = row?.NextSrl ?? row?.NEXTSRL ?? row?.nextsrl ?? Object.values(row || {})[0];
          if (nextVal != null) {
            FormSetField(SUBFORM_QTYCRD, 'CHMCLRGSTR_QTYCRD_SrlN', String(nextVal), 'tab');
          }
        } catch (e) {
          console.warn('[Web_HSE] Chemical Register serial auto-generation:', e);
        }
      }
    }
  }

  return { cancel: 0 };
}
