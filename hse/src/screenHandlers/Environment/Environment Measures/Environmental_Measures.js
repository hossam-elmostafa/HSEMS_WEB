/**
 * Screen handler: Environmental Measures / Execution (HSE_TgEnvMntrExctn)
 * Menu path: Environment -> Environment Measures -> Environmental Measures (from HSE.json)
 * C++: EnvMntrExctnCategory – ENVMNTRPLAN_CLSPLAN, ENVMNTREXCTNACTVTS_PLTNTS, ENVMNTREXCTNACTVTS_CMPLTACTVTY, CNCLACTVTY, REOPNACTVTY.
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

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
