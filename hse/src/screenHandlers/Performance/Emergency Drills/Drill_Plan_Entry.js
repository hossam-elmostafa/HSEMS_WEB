/**
 * Screen handler: Drill Plan Entry (HSE_DrllPlnEntryAtEntry)
 * Menu path: Performance -> Emergency Drills -> Drill Plan Entry (from HSE.json)
 * C++: DrillPlanEntryCategory – ShowDrillPlanEntry, DisplayToolBarButtonClicked, DisplayCustomButtonClicked.
 */


const DRILL_PLAN_ENTRY_TAG = 'HSE_DRLLPLNENTRYATENTRY';
const TABLE_DRLLPLN_VW = 'HSE_DRLLPLN_VW';

/**
 * Get allow-drill-entry policy from HSE_HSEPLC_EMRDRL (C++ allowDrillEntry).
 * @param {Object} devInterface - executeSQLPromise, getValFromRecordSet
 * @returns {Promise<string>} 'Y' or 'N'
 */
async function allowDrillEntry(devInterface) {
  const { executeSQLPromise, getValFromRecordSet } = devInterface || {};
  if (!executeSQLPromise || !getValFromRecordSet) return 'Y';
  try {
    const sql = "SELECT TOP 1 ISNULL(HSEPLC_EMRDRL_ALLDRLPLNENT,'') AS V FROM HSE_HSEPLC_EMRDRL";
    const data = await executeSQLPromise(sql);
    const val = data?.recordset?.[0] ? getValFromRecordSet(data, 'V') : '';
    return (val != null ? String(val).toUpperCase().trim() : '') || 'Y';
  } catch (e) {
    console.warn('[Web_HSE] Drill Plan Entry allowDrillEntry:', e);
    return 'Y';
  }
}

/**
 * Get drill year from policy (C++ getPolicyDrillYr).
 * @param {Object} devInterface - executeSQLPromise, getValFromRecordSet
 * @returns {Promise<string>}
 */
async function getPolicyDrillYr(devInterface) {
  const { executeSQLPromise, getValFromRecordSet } = devInterface || {};
  if (!executeSQLPromise || !getValFromRecordSet) return '';
  try {
    const sql = "SELECT TOP 1 ISNULL(HSEPLC_EMRDRL_DRLYR,'') AS V FROM HSE_HSEPLC_EMRDRL";
    const data = await executeSQLPromise(sql);
    const val = data?.recordset?.[0] ? getValFromRecordSet(data, 'V') : '';
    return val != null ? String(val).trim() : '';
  } catch (e) {
    console.warn('[Web_HSE] Drill Plan Entry getPolicyDrillYr:', e);
    return '';
  }
}

/**
 * Get default site from policy (C++ GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST').
 * @param {Object} devInterface - executeSQLPromise, getValFromRecordSet
 * @returns {Promise<string>}
 */
async function getDefaultSite(devInterface) {
  const { executeSQLPromise, getValFromRecordSet } = devInterface || {};
  if (!executeSQLPromise || !getValFromRecordSet) return '';
  try {
    const sql = "EXEC GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST'";
    const data = await executeSQLPromise(sql);
    const val = data?.recordset?.[0] ? getValFromRecordSet(data, 'HSEPLC_DFLST') : '';
    return val != null ? String(val).trim() : '';
  } catch (e) {
    console.warn('[Web_HSE] Drill Plan Entry getDefaultSite:', e);
    return '';
  }
}

/**
 * ShowScreen: mirror C++ ShowDrillPlanEntry – toolbar state from allowDrillEntry; criteria from getPolicyDrillYr.
 * setScreenDisableBtn(disableNew, disableSave, disableDelete).
 */
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  try {
    const allowed = await allowDrillEntry(devInterface);
    if (allowed === 'N') {
      setScreenDisableBtn(true, true, true);
      return;
    }
    const drillYr = await getPolicyDrillYr(devInterface);
    if (drillYr === '') {
      const msg = 'Please insert HSE policy - Emergency Drills - all fields first.';
      if (typeof devInterface.showMessage === 'function') {
        devInterface.showMessage(msg);
      } else {
        console.warn('[Web_HSE]', msg);
      }
      setScreenDisableBtn(true, true, true);
      return;
    }
    setScreenDisableBtn(false, false, false);
    if (typeof devInterface.ChangeCriteria === 'function' && strScrTag) {
      const criteria = `WHERE (DRLLPLN_DRLLYR=${drillYr} AND DRLLPLN_DRLLSTTUS < 5)`;
      devInterface.ChangeCriteria(DRILL_PLAN_ENTRY_TAG, '', criteria);
    }
  } catch (error) {
    console.warn('[Web_HSE] Drill Plan Entry ShowScreen:', error);
    setScreenDisableBtn(false, false, false);
  }
}

/**
 * toolBarButtonClicked: NEW (set DRLLPLN_DRLLYR, default site); SAVE (set last modified); NEW complete=0 cancel if !allowDrillEntry.
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  if (strScrTag !== DRILL_PLAN_ENTRY_TAG) {
    callBackFn(eventObj);
    return;
  }

  try {
    if (complete === 0 && strBtnName === 'NEW') {
      const allowed = await allowDrillEntry(devInterface);
      if (allowed === 'N' && !eventObj.fromCustomButton) {
        const msg = 'HSE Policy prevents adding new Planned Drills. Kindly use "Create Un-planned Drill" instead';
        if (typeof devInterface.showMessage === 'function') devInterface.showMessage(msg);
        else console.warn('[Web_HSE]', msg);
        if (typeof eventObj.iCancel !== 'undefined') eventObj.iCancel = 1;
        callBackFn(eventObj);
        return;
      }
    }

    if (complete === 1) {
      const { FormSetField } = devInterface;
      if (FormSetField) {
        if (strBtnName === 'NEW') {
          const drillYr = await getPolicyDrillYr(devInterface);
          if (drillYr) FormSetField(TABLE_DRLLPLN_VW, 'DRLLPLN_DRLLYR', drillYr);
          const defaultSite = await getDefaultSite(devInterface);
          if (defaultSite) FormSetField(DRILL_PLAN_ENTRY_TAG, 'DRLLPLN_ST', defaultSite);
        } else if (strBtnName === 'SAVE' && strTabTag === '') {
          const now = new Date();
          const mdy = `${String(now.getMonth() + 1).padStart(2, '0')}/${String(now.getDate()).padStart(2, '0')}/${now.getFullYear()}`;
          FormSetField(TABLE_DRLLPLN_VW, 'DRLLPLN_LSTMDFYDT', mdy);
          if (typeof devInterface.GetUserLogin === 'function') {
            FormSetField(TABLE_DRLLPLN_VW, 'DRLLPLN_LSTMDFYBY', devInterface.GetUserLogin());
          }
        }
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Drill Plan Entry toolBarButtonClicked:', error);
  }

  callBackFn(eventObj);
}

/**
 * ButtonClicked: PLAN_COMPLETED (exec COMPLETE_DRILL_PLAN), CREATE_UN-PLANNED_DRILL (confirm + New + set fields), RE-LOAD_SCENARIO (exec RELOAD_SCENARIO).
 */
export function ButtonClicked(eventObj) {
  const devInterface = eventObj.devInterfaceObj || {};
  const buttonName = (eventObj.Button_Name && eventObj.Button_Name.toString().toUpperCase().replace(/-/g, '_')) || '';
  const strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const fullRecordArr = eventObj.fullRecord || eventObj.fullRecordArr || [];
  const count = Array.isArray(fullRecordArr) ? fullRecordArr.length : 0;

  if (strScrTag !== DRILL_PLAN_ENTRY_TAG && strScrTag !== 'HSE_DRLLPLNENTRYATENTRY') {
    return;
  }

  const { FormGetField, executeSQLPromise, refreshData } = devInterface;

  if (buttonName === 'PLAN_COMPLETED') {
    if (count === 0) {
      const msg = 'You must save the Record first';
      if (typeof devInterface.showMessage === 'function') devInterface.showMessage(msg);
      else console.warn('[Web_HSE]', msg);
      return;
    }
    (async () => {
      try {
        const keyField = FormGetField ? FormGetField(TABLE_DRLLPLN_VW, 'DRLLPLN_PRMRYKY', 'scr') : '';
        if (keyField && executeSQLPromise) {
          await executeSQLPromise(`EXECUTE COMPLETE_DRILL_PLAN '${String(keyField).replace(/'/g, "''")}'`);
          if (typeof refreshData === 'function') refreshData('');
        }
      } catch (e) {
        console.warn('[Web_HSE] Drill Plan Entry COMPLETE_DRILL_PLAN:', e);
      }
    })();
    return;
  }

  if (buttonName === 'CREATE_UN_PLANNED_DRILL' || buttonName === 'CREATE_UN-PLANNED_DRILL') {
    const msg = 'Are you sure to create new un-planned drill?';
    if (typeof devInterface.showMessage === 'function' && typeof devInterface.confirm === 'function') {
      devInterface.confirm(msg, () => doCreateUnplannedDrill(eventObj));
    } else {
      if (typeof window !== 'undefined' && window.confirm) {
        if (window.confirm(msg)) doCreateUnplannedDrill(eventObj);
      } else {
        doCreateUnplannedDrill(eventObj);
      }
    }
    return;
  }

  if (buttonName === 'RE_LOAD_SCENARIO' || buttonName === 'RE-LOAD_SCENARIO') {
    (async () => {
      try {
        const keyField = FormGetField ? FormGetField(TABLE_DRLLPLN_VW, 'DRLLPLN_PRMRYKY', 'scr') : '';
        const dtlDrillTyp = FormGetField ? FormGetField(TABLE_DRLLPLN_VW, 'DRLLPLN_DTLDRLTYP', 'scr') : '';
        if (keyField && executeSQLPromise) {
          const k = String(keyField).replace(/'/g, "''");
          const d = String(dtlDrillTyp || '').replace(/'/g, "''");
          await executeSQLPromise(`EXECUTE RELOAD_SCENARIO '${k}','${d}'`);
          if (typeof refreshData === 'function') refreshData('');
        }
      } catch (e) {
        console.warn('[Web_HSE] Drill Plan Entry RELOAD_SCENARIO:', e);
      }
    })();
    return;
  }
}

/**
 * Helper: perform Create Un-planned Drill (programmatic New + set DRLLPLN_DRLLSTTUS=10, DRLLPLN_DRLLPLNND=N).
 */
function doCreateUnplannedDrill(eventObj) {
  const devInterface = eventObj.devInterfaceObj || {};
  const { DoToolBarAction, FormSetField } = devInterface;
  eventObj.fromCustomButton = true;
  if (typeof DoToolBarAction === 'function') {
    DoToolBarAction('NEW', DRILL_PLAN_ENTRY_TAG, '');
  }
  if (FormSetField) {
    FormSetField(TABLE_DRLLPLN_VW, 'DRLLPLN_DRLLSTTUS', '10');
    FormSetField(TABLE_DRLLPLN_VW, 'DRLLPLN_DRLLPLNND', 'N');
  }
  delete eventObj.fromCustomButton;
}
