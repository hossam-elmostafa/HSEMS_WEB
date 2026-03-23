/**
 * Screen handler: Environmental Measures / Execution (HSE_TgEnvMntrExctn)
 * Menu path: Environment -> Environment Measures -> Environmental Measures (from HSE.json)
 * C++: EnvMntrExctnCategory – ENVMNTRPLAN_CLSPLAN, ENVMNTREXCTNACTVTS_PLTNTS, ENVMNTREXCTNACTVTS_CMPLTACTVTY, CNCLACTVTY, REOPNACTVTY.
 *
 * RQ_HSE_23_3_26_21_14: Added SubFieldChanged (GA-6 – activity serial auto-generation),
 *   MainSubReposition (GA-5 – activity record/button lock on completion),
 *   toolBarButtonClicked (GA-4 – SAVETAB refresh).
 */

const SCREEN_TAG = 'HSE_TgEnvMntrExctn';
const TABLE_PLAN = 'HSE_ENVMNTRPLAN';
const TABLE_ACTVTS = 'HSE_ENVMNTRPLANACTVTS';
const KEY_PLAN = 'EnvMntrPlan_Yr';
const KEY_ACTVTS = 'EnvMntrPlanActvts_PrmKy';
const POPUP_TAG = 'HSE_TgEnvMntrActvtPltnt'; // C++: HSE_TGENVMNTRACTVTSPLTNT

const BTN_CLSPLAN = 'ENVMNTRPLAN_CLSPLAN';
const BTN_PLTNTS = 'ENVMNTREXCTNACTVTS_PLTNTS';
const BTN_CMPLT_ACTVTY = 'ENVMNTREXCTNACTVTS_CMPLTACTVTY';
const BTN_CNCL_ACTVTY = 'ENVMNTREXCTNACTVTS_CNCLACTVTY';
const BTN_REOPN_ACTVTY = 'ENVMNTREXCTNACTVTS_REOPNACTVTY';

const ActivityStatus = { Pending: 1, Cancelled: 2, Completed: 5 };

// RQ_HSE_23_3_26_21_14: module-level devInterfaceObj for MainSubReposition / SubFieldChanged
let _devInterfaceObj = {};

function getPlanKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_PLAN, KEY_PLAN, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function getActvtsKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_ACTVTS, KEY_ACTVTS, 'scr');
  if (v != null && v !== '') return String(v).trim();
  const v2 = devInterfaceObj.FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_PRMKY', 'scr');
  return (v2 != null && v2 !== '') ? String(v2).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Close plan: EXECUTE closeEnvMntrPlan <yr>
 */
async function handleClosePlan(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPlanKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE closeEnvMntrPlan ${pk}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Measures Close plan:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error closing plan');
  }
}

/**
 * Open Measures popup for current activity row.
 */
function handlePltnts(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof openScr !== 'function' || !FormGetField) return;
  const linkVal = FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_PRMKY', 'scr');
  if (linkVal == null || linkVal === '') return;
  const strCriteria = `SELECT * FROM HSE_ENVMNTRACTVTSPLTNT WHERE ENVMNTRACTVTSPLTNT_LNK = ${linkVal}`;
  const actvStts = FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_ACTVSTTS', 'scr') || '';
  const defValObj = { ENVMNTRACTVTSPLTNT_LNK: linkVal, ActivityStatusHolder: actvStts };
  openScr(POPUP_TAG, {}, strCriteria, 'edit', false, defValObj, false, false);
}

/**
 * Change activity status via EXECUTE changeEnvMsrActvtStat <status>, <activityPrmKy>
 */
async function changeEnvMsrActvtStat(devInterfaceObj, activityNewStatus, activityPrmKy) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise || !activityPrmKy) return;
  try {
    await executeSQLPromise(`EXECUTE changeEnvMsrActvtStat ${activityNewStatus},${activityPrmKy}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Measures change activity status:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error updating activity status');
  }
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const actvtsKey = getActvtsKey(devInterfaceObj);

  if (btn === BTN_CLSPLAN) {
    await handleClosePlan(devInterfaceObj);
    return;
  }
  if (btn === BTN_PLTNTS) {
    handlePltnts(devInterfaceObj);
    return;
  }
  if (btn === BTN_CMPLT_ACTVTY && actvtsKey) {
    await changeEnvMsrActvtStat(devInterfaceObj, ActivityStatus.Completed, actvtsKey);
    return;
  }
  if (btn === BTN_CNCL_ACTVTY && actvtsKey) {
    await changeEnvMsrActvtStat(devInterfaceObj, ActivityStatus.Cancelled, actvtsKey);
    return;
  }
  if (btn === BTN_REOPN_ACTVTY && actvtsKey) {
    await changeEnvMsrActvtStat(devInterfaceObj, ActivityStatus.Pending, actvtsKey);
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-4 – toolBarButtonClicked: SAVETAB refresh
// (C++: EnvMntrExctnCategory::DisplayToolBarButtonClicked)
// ---------------------------------------------------------------------------

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const { strBtnName, complete, devInterfaceObj: evtDev } = eventObj;
  const dev = evtDev || _devInterfaceObj;
  _devInterfaceObj = dev;

  const btn = String(strBtnName || '').toUpperCase();

  try {
    // C++: if(iComplete==1 && strBtnName=="SAVETAB") { RefreshScreen("",REFRESH_SELECTED); }
    if (btn === 'SAVETAB' && complete === 1) {
      const { refreshData } = dev;
      if (typeof refreshData === 'function') refreshData('');
    }
  } catch (error) {
    console.warn('[Web_HSE] Environmental Measures toolBarButtonClicked:', error);
  }

  callBackFn(eventObj);
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-6 – SubFieldChanged: activity serial auto-generation
// (C++: EnvMntrExctnCategory::DisplayFieldChange)
// ---------------------------------------------------------------------------

/**
 * C++ DisplayFieldChange: when any field containing "ENVMNTRPLANACTVTS" changes,
 * if serial (ENVMNTRPLANACTVTS_SRIL) is empty -> getNewSerialNoForATab.
 *
 * getNewSerialNoForATab("HSE_TGENVMNTREXCTN","EnvMntrPlan_Yr",
 *     "HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_LNK","ENVMNTRPLANACTVTS_SRIL")
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  if (fld.indexOf('ENVMNTRPLANACTVTS') >= 0) {
    const serialVal = String(FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_SRIL', 'tab') ?? '').trim();
    if (serialVal === '') {
      const masterPK = String(FormGetField(TABLE_PLAN, KEY_PLAN, 'scr') ?? '').trim();
      if (masterPK) {
        try {
          const sql = `SELECT ISNULL(MAX(ENVMNTRPLANACTVTS_SRIL)+1, 1) AS NextSrl FROM HSE_ENVMNTRPLANACTVTS WHERE EnvMntrPlanActvts_LNK = '${escapeSqlString(masterPK)}'`;
          const data = await executeSQLPromise(sql);
          const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
          const nextSrl = row?.NextSrl ?? row?.NEXTSRL ?? '';
          if (nextSrl !== '' && nextSrl != null) {
            FormSetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_SRIL', String(nextSrl), 'tab');
          }
        } catch (e) {
          console.warn('[Web_HSE] Environmental Measures serial auto-generation:', e);
        }
      }
    }
  }

  return { cancel: 0 };
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-5 – MainSubReposition: activity record/button lock
// (C++: EnvMntrExctnCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

/**
 * C++: if(nActvtyStts >= 5) {
 *   DisableToolbar(strSubFormTag, DELETE/NEW/SAVE);
 *   FormEnableButton(false) for PLTNTS, CMPLTACTVTY, CNCLACTVTY;
 *   LockRecord("", strSubFormTag, true);
 * }
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  const actvStatus = FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_ACTVSTTS', 'scr');
  const n = parseInt(String(actvStatus ?? '').trim(), 10);

  if (Number.isFinite(n) && n >= 5) {
    setScreenDisableBtn(true, true, true);

    // Attempt custom button disable if API is available (C++: FormEnableButton)
    if (typeof dev.FormEnableButton === 'function') {
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_PLTNTS', false, true);
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_CMPLTACTVTY', false, true);
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_CNCLACTVTY', false, true);
    }
  } else {
    setScreenDisableBtn(false, false, false);

    if (typeof dev.FormEnableButton === 'function') {
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_PLTNTS', true, true);
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_CMPLTACTVTY', true, true);
      dev.FormEnableButton(2, 1, 'CUSTOM_ENVMNTREXCTNACTVTS_CNCLACTVTY', true, true);
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
