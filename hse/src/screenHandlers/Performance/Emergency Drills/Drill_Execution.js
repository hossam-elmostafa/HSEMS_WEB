/**
 * Screen handler: Drill Execution (HSE_DrllExcutnAtExcutn)
 * Menu path: Performance -> Emergency Drills -> Drill Execution (from HSE.json)
 * C++: DrillPlanExecutionCategory – CLOSE_DRILL, CANCELL_DRILL, RE-LOAD_DRILL_SCENARIO.
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

  if (btn === 'CLOSE_DRILL') {
    try {
      await executeSQLPromise(`EXECUTE CloseDrill '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Drill Execution Close:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error closing drill');
    }
    return;
  }
  if (btn === 'CANCELL_DRILL') {
    const ok = typeof AskYesNoMessage === 'function' ? AskYesNoMessage('Are you sure to cancel the drill?') : true;
    if (!ok) return;
    try {
      await executeSQLPromise(`EXECUTE CancelDrill '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Drill Execution Cancel:', e);
      if (AskYesNoMessage) AskYesNoMessage('Error cancelling drill');
    }
    return;
  }
  if (btn === 'RE-LOAD_DRILL_SCENARIO') {
    try {
      await executeSQLPromise(`EXECUTE INSERT_ACTUAL_SCNR '${escapeSqlString(pk)}'`);
      if (typeof refreshData === 'function') refreshData('');
    } catch (e) {
      console.warn('[Web_HSE] Drill Execution Re-load scenario:', e);
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error re-loading scenario');
    }
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
