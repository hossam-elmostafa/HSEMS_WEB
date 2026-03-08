/**
 * Screen handler: Environmental Monitoring Plan (HSE_TgEnvMntrPlan)
 * Menu path: Environment -> Environment Measures -> Environmental Monitoring Plan (from HSE.json)
 * C++: EnvMntrPlanCategory – ENVMNTRPLAN_APRVPLAN, ENVMNTRPLAN_REOPNPLN, ENVMNTRPLAN_RELOADACTVTS, ENVMNTRPLANACTVTS_PLTNTS.
 */

const SCREEN_TAG = 'HSE_TgEnvMntrPlan';
const TABLE_PLAN = 'HSE_ENVMNTRPLAN';
const KEY_FIELD = 'EnvMntrPlan_Yr';
const POPUP_TAG = 'HSE_TgEnvMntrActvtPltnt'; // C++: HSE_TGENVMNTRACTVTSPLTNT
const TABLE_ACTVTS = 'HSE_ENVMNTRPLANACTVTS';
const LINK_FIELD = 'ENVMNTRPLANACTVTS_PRMKY';

const BTN_APRV_PLAN = 'ENVMNTRPLAN_APRVPLAN';
const BTN_REOPN_PLAN = 'ENVMNTRPLAN_REOPNPLN';
const BTN_RELOAD_ACTVTS = 'ENVMNTRPLAN_RELOADACTVTS';
const BTN_ACTVTS_PLTNTS = 'ENVMNTRPLANACTVTS_PLTNTS';

function getPlanKey(devInterfaceObj) {
  if (!devInterfaceObj?.FormGetField) return '';
  const v = devInterfaceObj.FormGetField(TABLE_PLAN, KEY_FIELD, 'scr');
  return (v != null && v !== '') ? String(v).trim() : '';
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

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
