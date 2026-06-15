/**
 * Screen handler: EQ Inspection Request (HSE_EQInspctnRqust)
 * Menu path: Performance -> EQ Inspection -> EQ Inspection Request (from HSE.json)
 * RQ_HSE_23_3_26_23_15: Custom buttons run via ModuleButtonHandlers; SAVE duplicate warning; SubFieldChanged parity.
 */

import { escapeSqlString } from '../../../services/ModuleButtonHandlers/moduleButtonHandlersUtils.js';
import { showEqInformationDialog } from '../../../utils/eqInspectionDialogs.js';
import { eqInspectionSubFieldChanged } from './eqInspectionSubFieldChanged.js';

const SCREEN_TAG_UC = 'HSE_EQINSPCTNRQUST';

async function assignNextEqInspectionNumberOnNew(devInterfaceObj) {
  const { FormGetField, FormSetField, executeSQLPromise } = devInterfaceObj || {};
  if (!FormGetField || !FormSetField || !executeSQLPromise) return;
  const yr = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_INSPCTNYR', 'scr') ?? '').trim();
  if (!yr) return;
  try {
    const data = await executeSQLPromise(
      `SELECT ISNULL(MAX(EQINSPCTN_INSPCTNN)+1,1) AS NXT FROM HSE_EQINSPCTN WHERE EQINSPCTN_INSPCTNYR = '${escapeSqlString(yr)}'`
    );
    const row = data?.recordset?.[0] ?? data?.[0];
    const nxt = row?.NXT ?? row?.nxt ?? '';
    if (nxt !== '' && nxt != null) {
      FormSetField('HSE_EQINSPCTN', 'EQINSPCTN_INSPCTNN', String(nxt), 'scr');
    }
  } catch (e) {
    console.warn('[Web_HSE] EQ inspection number on NEW:', e);
  }
}

async function warnDuplicateOpenEqInspection(devInterfaceObj) {
  const { FormGetField, executeSQLPromise } = devInterfaceObj || {};
  if (!FormGetField || !executeSQLPromise) return;
  const strEqTyp = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQTYP', 'scr') ?? '').trim();
  const strEqSrlNum = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQSRLN', 'scr') ?? '').trim();
  const strEqPltNum = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQPLTN', 'scr') ?? '').trim();
  if (!strEqTyp || !strEqSrlNum) return;

  let strSQL;
  if (strEqPltNum !== '') {
    strSQL = `SELECT COUNT(0) AS CNT FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '${escapeSqlString(strEqTyp)}') AND (EQINSPCTN_EQSRLN = '${escapeSqlString(strEqSrlNum)}') AND (EQINSPCTN_EQPLTN = '${escapeSqlString(strEqPltNum)}') AND (EQInspctn_InspctnSttus < 5)`;
  } else {
    strSQL = `SELECT COUNT(0) AS CNT FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '${escapeSqlString(strEqTyp)}') AND (EQINSPCTN_EQSRLN = '${escapeSqlString(strEqSrlNum)}') AND (EQINSPCTN_EQPLTN IS NULL) AND (EQInspctn_InspctnSttus < 5)`;
  }
  try {
    const data = await executeSQLPromise(strSQL);
    const row = data?.recordset?.[0] ?? data?.[0];
    const cnt = parseInt(row?.CNT ?? row?.cnt ?? 0, 10) || 0;
    if (cnt > 0) {
      void showEqInformationDialog(devInterfaceObj, 'This Equipment entered before!', 'Information');
    }
  } catch (e) {
    console.warn('[Web_HSE] EQ duplicate open inspection check:', e);
  }
}

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj;
  const strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const { complete } = eventObj;

  if (strScrTag === SCREEN_TAG_UC && strBtnName === 'SAVE' && complete === 0 && strTabTag === '') {
    await warnDuplicateOpenEqInspection(devInterface);
  }

  // C++ EQInspctnCategory::DisplayToolBarButtonClicked — NEW after complete: next EQINSPCTN_INSPCTNN for year
  if (strScrTag === SCREEN_TAG_UC && strBtnName === 'NEW' && complete === 1 && strTabTag === '') {
    await assignNextEqInspectionNumberOnNew(devInterface);
  }

  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  return eqInspectionSubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
