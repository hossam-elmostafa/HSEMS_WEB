/**
 * Screen handler: Environmental Monitoring Plan (HSE_TgEnvMntrPlan)
 * Menu path: Environment -> Environment Measures -> Environmental Monitoring Plan (from HSE.json)
 * C++: EnvMntrPlanCategory – ENVMNTRPLAN_APRVPLAN, ENVMNTRPLAN_REOPNPLN, ENVMNTRPLAN_RELOADACTVTS, ENVMNTRPLANACTVTS_PLTNTS.
 *
 * RQ_HSE_23_3_26_21_14: Added SubFieldChanged (GA-1 – activity serial auto-generation),
 *   MainSubReposition (GA-2 – plan record lock on approval).
 */

const SCREEN_TAG = 'HSE_TgEnvMntrPlan';
const TABLE_PLAN = 'HSE_ENVMNTRPLAN';
const KEY_FIELD = 'EnvMntrPlan_Yr';
const POPUP_TAG = 'HSE_TgEnvMntrActvtPltnt'; // C++: HSE_TGENVMNTRACTVTSPLTNT
const TABLE_ACTVTS = 'HSE_ENVMNTRPLANACTVTS';
const LINK_FIELD = 'ENVMNTRPLANACTVTS_PRMKY';
const SUBTAB_ACTVTS = 'HSE_TGENVMNTRPLANACTVTS';

const BTN_APRV_PLAN = 'ENVMNTRPLAN_APRVPLAN';
const BTN_REOPN_PLAN = 'ENVMNTRPLAN_REOPNPLN';
const BTN_RELOAD_ACTVTS = 'ENVMNTRPLAN_RELOADACTVTS';
const BTN_ACTVTS_PLTNTS = 'ENVMNTRPLANACTVTS_PLTNTS';

// RQ_HSE_23_3_26_21_14: module-level devInterfaceObj for MainSubReposition / SubFieldChanged
let _devInterfaceObj = {};

function getPlanKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_PLAN, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Approve plan: EXECUTE approveEnvMntrPlan <yr>
 */
async function handleApprovePlan(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPlanKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE approveEnvMntrPlan ${pk}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Monitoring Plan Approve:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error approving plan');
  }
}

/**
 * Reopen plan: EXECUTE reOpnEnvMntrPlan <yr>
 */
async function handleReopenPlan(devInterfaceObj) {
  const { executeSQLPromise, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPlanKey(devInterfaceObj);
  if (!pk) return;
  try {
    await executeSQLPromise(`EXECUTE reOpnEnvMntrPlan ${pk}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Monitoring Plan Reopen:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error reopening plan');
  }
}

/**
 * Reload activities: confirm then EXECUTE reloadEnvMntrPlnActvty <yr>
 */
async function handleReloadActvts(devInterfaceObj) {
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPlanKey(devInterfaceObj);
  if (!pk) return;
  const msg = 'Are you sure to re-load the activity records?';
  const ok = typeof AskYesNoMessage === 'function' ? AskYesNoMessage(msg) : true;
  if (!ok) return;
  try {
    await executeSQLPromise(`EXECUTE reloadEnvMntrPlnActvty ${pk}`);
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] Environmental Monitoring Plan Reload activities:', e);
    if (AskYesNoMessage) AskYesNoMessage('Error re-loading activities');
  }
}

/**
 * Open Measures popup for selected activity row.
 * C++: strMainLinkFeild = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_PRMKY"); ShowScreen(..., "HSE_TGENVMNTRACTVTSPLTNT", ...);
 */
function handleActvtsPltnts(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  if (typeof openScr !== 'function' || !FormGetField) return;
  const linkVal = FormGetField(TABLE_ACTVTS, LINK_FIELD, 'scr');
  if (linkVal == null || linkVal === '') return;
  const strCriteria = `SELECT * FROM HSE_ENVMNTRACTVTSPLTNT WHERE ENVMNTRACTVTSPLTNT_LNK = ${linkVal}`;
  const defValObj = { ENVMNTRACTVTSPLTNT_LNK: linkVal };
  openScr(POPUP_TAG, {}, strCriteria, 'edit', false, defValObj, false, true);
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_APRV_PLAN) {
    await handleApprovePlan(devInterfaceObj);
    return;
  }
  if (btn === BTN_REOPN_PLAN) {
    await handleReopenPlan(devInterfaceObj);
    return;
  }
  if (btn === BTN_RELOAD_ACTVTS) {
    const planStts = devInterfaceObj?.FormGetField?.(TABLE_PLAN, 'ENVMNTRPLAN_PLNSTTS', 'scr');
    const n = parseInt(planStts, 10);
    if (n < 5) await handleReloadActvts(devInterfaceObj);
    return;
  }
  if (btn === BTN_ACTVTS_PLTNTS) {
    handleActvtsPltnts(devInterfaceObj);
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-1 – SubFieldChanged: activity serial auto-generation
// (C++: EnvMntrPlanCategory::DisplayFieldChange)
// ---------------------------------------------------------------------------

/**
 * C++ DisplayFieldChange: when any field containing "ENVMNTRPLANACTVTS" changes,
 * if serial (ENVMNTRPLANACTVTS_SRIL) is empty -> getNewSerialNoForATab.
 *
 * getNewSerialNoForATab("HSE_TGENVMNTRPLAN","EnvMntrPlan_Yr",
 *     "HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_LNK","ENVMNTRPLANACTVTS_SRIL")
 *   -> SELECT ISNULL(MAX(ENVMNTRPLANACTVTS_SRIL)+1,1)
 *     FROM HSE_ENVMNTRPLANACTVTS WHERE EnvMntrPlanActvts_LNK = '<masterPK>'
 */
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const dev = devInterfaceObj || _devInterfaceObj;
  const { FormGetField, FormSetField, executeSQLPromise } = dev;
  if (!FormGetField || !FormSetField || !executeSQLPromise) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();

  if (fld.indexOf('ENVMNTRPLANACTVTS') >= 0) {
    const serialVal = String(FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_SRIL', 'tab') ?? '').trim();
    if (serialVal === '') {
      const masterPK = String(FormGetField(TABLE_PLAN, KEY_FIELD, 'scr') ?? '').trim();
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
          console.warn('[Web_HSE] Environmental Monitoring Plan serial auto-generation:', e);
        }
      }
    }
  }

  return { cancel: 0 };
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-2 – MainSubReposition: plan record lock on approval
// (C++: EnvMntrPlanCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

/**
 * C++: if(nPlnStts >= 5) { DisableToolbar NEW/SAVE/DELETE; LockRecord; }
 * Additionally, when on Activities sub-tab and activity status >= 5, disable DELETE.
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  const planStatus = FormGetField(TABLE_PLAN, 'ENVMNTRPLAN_PLNSTTS', 'scr');
  const nPlanStts = parseInt(String(planStatus ?? '').trim(), 10);

  if (Number.isFinite(nPlanStts) && nPlanStts >= 5) {
    setScreenDisableBtn(true, true, true);
  } else {
    setScreenDisableBtn(false, false, false);

    // C++: when on Activities sub-tab, disable DELETE if activity status >= 5
    const tabTag = String(strSelectedTabTag || '').toUpperCase();
    if (tabTag === SUBTAB_ACTVTS.toUpperCase()) {
      const actvStatus = FormGetField(TABLE_ACTVTS, 'ENVMNTRPLANACTVTS_ACTVSTTS', 'tab');
      const nActvStts = parseInt(String(actvStatus ?? '').trim(), 10);
      if (Number.isFinite(nActvStts) && nActvStts >= 5) {
        setScreenDisableBtn(false, false, true);
      }
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
