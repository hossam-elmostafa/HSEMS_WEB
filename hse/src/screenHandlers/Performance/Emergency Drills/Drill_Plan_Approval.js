/**
 * Screen handler: Drill Plan Approval (HSE_DrllPlnApprvlAtApprvl)
 * Menu path: Performance -> Emergency Drills -> Drill Plan Approval (from HSE.json)
 * C++: DrillPlanAprvCategory – APPROVE_DRILL_PLAN, REJECT_DRILL_PLAN.
 */

import { getCurrentRecordPRMKY } from '../../../utils/rejectReasonUtils.js';

const TABLE_VIEW = 'HSE_DRLLPLN_VW';
const KEY_FIELD = 'DRLLPLN_PRMRYKY';

function getPlanKey(eventObj, devInterfaceObj) {
  if (devInterfaceObj?.FormGetField) {
    const v = devInterfaceObj.FormGetField(TABLE_VIEW, KEY_FIELD, 'scr');
    if (v != null && v !== '') return String(v).trim();
  }
  return getCurrentRecordPRMKY(eventObj) || '';
}

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return;
  const pk = getPlanKey(eventObj, devInterfaceObj);
  if (!pk) return;

  if (btn === 'APPROVE_DRILL_PLAN') {
    try {
      await executeSQLPromise(`EXECUTE ApproveDrillPlan '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Drill Plan Approval:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error approving drill plan');
    }
    return;
  }
  if (btn === 'REJECT_DRILL_PLAN') {
    const ok = typeof AskYesNoMessage === 'function' ? AskYesNoMessage('Are you sure to reject the drill plan?') : true;
    if (!ok) return;
    try {
      await executeSQLPromise(`EXECUTE RejectDrillPlan '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Drill Plan Reject:', e);
      if (AskYesNoMessage) AskYesNoMessage('Error rejecting drill plan');
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
