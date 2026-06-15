/**
 * Screen handler: Environmental Measure Entry (HSE_TgEnvMsrEntry)
 * Menu path: Environment -> Environment Measures -> Environmental Measure Entry
 * C++: EnvMsrEntryCategory – ENVMSRENTRY_ENTRYCMPLTD, ENVMSRENTRYPOLTANT_PRPRCARS,
 *      ENVMSRENTRYPOLTANT_RELDPOLTNTS.
 *
 * RQ_HSE_23_3_26_21_14: New handler implementing gaps GB-1 through GB-6:
 *   GB-1 – toolBarButtonClicked NEW: auto-generate ENVMSRENTRY_MSRNO.
 *   GB-2 – SubFieldChanged: deviation auto-calculation on MSRDVAL change.
 *   GB-3 – toolBarButtonClicked SAVETAB on pollutant tab: set LSTMDFYDT/LSTMDFYBY + save main.
 *   GB-4 – ButtonClicked PRPRCARS: open Recommended CARs popup.
 *   GB-5 – ButtonClicked RELDPOLTNTS: reload pollutants via SP.
 *   GB-6 – MainSubReposition: record lock when ENTRYSTTS >= 5.
 */

const SCREEN_TAG = 'HSE_TgEnvMsrEntry';
const TABLE_NAME = 'HSE_ENVMSRENTRY';
const TABLE_POLLUTANT = 'HSE_ENVMSRENTRYPOLTANT';
const SUBTAB_POLLUTANT = 'HSE_TGENVMSRENTRYPOLTANT';

const BTN_PRPRCARS = 'ENVMSRENTRYPOLTANT_PRPRCARS';
const BTN_RELDPOLTNTS = 'ENVMSRENTRYPOLTANT_RELDPOLTNTS';

// RQ_HSE_23_3_26_21_14: module-level devInterfaceObj for MainSubReposition / SubFieldChanged
let _devInterfaceObj = {};

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-4 – Prepare CARs popup (C++: EnvMsrEntryCategory::DisplayCustomButtonClicked)
// ---------------------------------------------------------------------------

function handlePrepareCARs(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof openScr !== 'function' || !FormGetField) return;
  const linkVal = FormGetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_PRMKY', 'tab');
  if (linkVal == null || linkVal === '') return;
  const strCriteria = `SELECT * FROM HSE_EnvMsrEntryPoltantPrpCAR WHERE EnvMsrEntryPoltantPrpCAR_Lnk = ${linkVal}`;
  const defValObj = { EnvMsrEntryPoltantPrpCAR_Lnk: linkVal };
  openScr('HSE_TgEnvMsrEntryPoltantPrpCAR', {}, strCriteria, 'edit', false, defValObj, false, false);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-5 – ReLoad Pollutants (C++: EnvMsrEntryCategory::DisplayCustomButtonClicked)
// ---------------------------------------------------------------------------

async function handleReloadPollutants(devInterfaceObj) {
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (!FormGetField) return;
  const prmKy = String(FormGetField(TABLE_NAME, 'ENVMSRENTRY_PRMKY', 'scr') ?? '').trim();
  if (!prmKy) return;
  const ok = typeof AskYesNoMessage === 'function'
    ? AskYesNoMessage('Are you sure to re-load pollutant records?')
    : true;
  if (!ok) return;
  try {
    await executeSQLPromise(`EXECUTE reldPoltnts '${escapeSqlString(prmKy)}'`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Measure Entry ReLoad Pollutants:', e);
    if (AskYesNoMessage) AskYesNoMessage('Error re-loading pollutants');
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-4, GB-5 – ButtonClicked
// ---------------------------------------------------------------------------

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_PRPRCARS) {
    handlePrepareCARs(devInterfaceObj);
    return;
  }
  if (btn === BTN_RELDPOLTNTS) {
    await handleReloadPollutants(devInterfaceObj);
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-1 – Measure number on NEW, GB-3 – LSTMDFYDT/LSTMDFYBY on SAVETAB
// (C++: EnvMsrEntryCategory::DisplayToolBarButtonClicked)
// ---------------------------------------------------------------------------

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const { strBtnName, strTabTag, complete, devInterfaceObj: evtDev } = eventObj;
  const dev = evtDev || _devInterfaceObj;
  _devInterfaceObj = dev;

  const btn = String(strBtnName || '').toUpperCase();
  const tab = String(strTabTag || '').toUpperCase();

  try {
    // GB-1: auto-generate measure number on NEW (main form, after complete)
    if (btn === 'NEW' && complete === 1 && tab === '') {
      const { executeSQLPromise, FormGetField, FormSetField } = dev;
      if (executeSQLPromise && FormGetField && FormSetField) {
        const yr = String(FormGetField(TABLE_NAME, 'ENVMSRENTRY_YR', 'scr') ?? '').trim();
        if (yr) {
          const sql = `SELECT ISNULL(MAX(ENVMSRENTRY_MSRNO)+1, 1) AS NextMsrNo FROM HSE_ENVMSRENTRY WHERE ENVMSRENTRY_YR = '${escapeSqlString(yr)}'`;
          const data = await executeSQLPromise(sql);
          const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
          const nextNo = row?.NextMsrNo ?? row?.NEXTMSRNO ?? '';
          if (nextNo !== '' && nextNo != null) {
            FormSetField(TABLE_NAME, 'ENVMSRENTRY_MSRNO', String(nextNo), 'scr');
          }
        }
      }
    }

    // GB-3: set LSTMDFYDT / LSTMDFYBY on SAVETAB for pollutant tab, then save main
    if (btn === 'SAVETAB' && complete === 1 && tab === SUBTAB_POLLUTANT.toUpperCase()) {
      const { FormSetField, executeSQLPromise } = dev;
      if (FormSetField) {
        const now = new Date();
        const dd = String(now.getDate()).padStart(2, '0');
        const mm = String(now.getMonth() + 1).padStart(2, '0');
        const yyyy = now.getFullYear();
        const dateStr = `${dd}/${mm}/${yyyy}`;
        FormSetField(TABLE_NAME, 'ENVMSRENTRY_LSTMDFYDT', dateStr, 'scr');

        const getUserLogin = dev.getUserLogin || dev.getUserName || dev.getUser;
        if (typeof getUserLogin === 'function') {
          FormSetField(TABLE_NAME, 'ENVMSRENTRY_LSTMDFYBY', getUserLogin(), 'scr');
        }

        // C++: DoToolBarAction(TOOLBAR_SAVE, "HSE_TGENVMSRENTRY", "")
        if (typeof dev.DoToolBarAction === 'function') {
          dev.DoToolBarAction('SAVE', SCREEN_TAG, '');
        } else if (typeof dev.refreshData === 'function') {
          dev.refreshData('');
        }
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Environmental Measure Entry toolBarButtonClicked:', error);
  }

  callBackFn(eventObj);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-2 – Deviation auto-calculation
// (C++: EnvMsrEntryCategory::DisplayFieldChange)
// ---------------------------------------------------------------------------

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField } = dev;
  if (!FormGetField || !FormSetField) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  if (fld === 'ENVMSRENTRYPOLTANT_MSRDVAL') {
    const nMinVal = parseInt(String(FormGetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_MINVAL', 'tab') ?? '0'), 10);
    const nMaxVal = parseInt(String(FormGetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_MAXVAL', 'tab') ?? '0'), 10);
    const nMsrdVal = parseInt(String(FormGetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_MSRDVAL', 'tab') ?? '0'), 10);

    let deviation = 0;
    let resultAccepted = 'Y';

    if (nMsrdVal < nMinVal) {
      deviation = nMsrdVal - nMinVal;
      resultAccepted = 'N';
    } else if (nMsrdVal > nMaxVal) {
      deviation = nMsrdVal - nMaxVal;
      resultAccepted = 'N';
    }

    FormSetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_DVTN', String(deviation), 'tab');
    FormSetField(TABLE_POLLUTANT, 'ENVMSRENTRYPOLTANT_RSLTACPTD', resultAccepted, 'tab');
  }

  return { cancel: 0 };
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-6 – Record lock when completed
// (C++: EnvMsrEntryCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  const status = FormGetField(TABLE_NAME, 'ENVMSRENTRY_ENTRYSTTS', 'scr');
  const n = parseInt(String(status ?? '').trim(), 10);
  if (Number.isFinite(n) && n >= 5) {
    setScreenDisableBtn(true, true, true);
  } else {
    setScreenDisableBtn(false, false, false);
  }
}

// ---------------------------------------------------------------------------
// ShowScreen
// ---------------------------------------------------------------------------

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
