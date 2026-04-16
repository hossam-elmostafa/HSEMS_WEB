/**
 * Corrective / Creative Access Entry — HSE_TgcrctveAccEnt (normalizes to HSE_TGCRCTVEACCENT)
 * Menu: Performance → Action Tracking → Corrective Action Entry (typical)
 * C++: CAREntryCategory::DisplayCustomButtonClicked — CRCTVEACCENT_ENTCMPLT → completeCARTXN
 * RQ_HSE_30_3_26_14_57
 * RQ_HSE_12_4_26_00_40 — GAP-8: default list criteria; GAP-10: CancelCARTXN; GAP-11: fill from CAR basis; GAP-21: delete prevention
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import {
  executeCarTxnThreeArgs,
  handleCompleteCorrectiveActionEntry,
} from '../../../utils/carFoundationTxnButtons.js';
import { getCurrentRecordPRMKY } from '../../../utils/rejectReasonUtils.js';
// RQ_HSE_12_4_26_00_40 — GAP-11
import { fillFromCarBasis } from '../../../utils/carEntryFillFromBasis.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name || '').toString().toUpperCase();
  if (btn === 'CRCTVEACCENT_ENTCMPLT') {
    const consumed = await handleCompleteCorrectiveActionEntry(eventObj, devInterfaceObj);
    if (consumed) return;
  }
  // RQ_HSE_12_4_26_00_40 — GAP-10: C++ CarEntryCategory CRCTVEACCENT_CNCL → EXECUTE CancelCARTXN
  if (btn === 'CRCTVEACCENT_CNCL') {
    const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage } = devInterfaceObj || {};
    if (!executeSQLPromise) {
      sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
      return;
    }
    const prmky = getCurrentRecordPRMKY(eventObj);
    if (!prmky) {
      if (AskYesNoMessage) AskYesNoMessage('You must save the Record first. (RQ_HSE_12_4_26_00_40 GAP-10)');
      return;
    }
    try {
      const pkNum = parseInt(prmky, 10);
      if (Number.isFinite(pkNum) && pkNum > 0) {
        const stsRs = await executeSQLPromise(
          `SELECT TOP 1 ISNULL(CRCTVEACCENT_RECSTS,'') AS S FROM HSE_CRCTVEACCENT WHERE PrmKy = ${pkNum}`
        );
        const row = stsRs?.recordset?.[0] ?? stsRs?.[0];
        if (String(row?.S ?? row?.s ?? '').trim() !== '1') {
          if (AskYesNoMessage) {
            AskYesNoMessage('Only incomplete corrective action accounts can be cancelled. (RQ_HSE_12_4_26_00_40 GAP-10)');
          }
          return;
        }
      }
    } catch (_) {
      /* proceed if status read fails */
    }
    const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
    const caption = 'Corrective Action Entry';
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'CancelCARTXN', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-10 CancelCARTXN (Corrective Action Entry):', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error cancelling corrective action'));
    }
    return;
  }
  sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

// RQ_HSE_12_4_26_00_40 — GAP-11: when CARMODELBAS changes, fill location/dept from source TXN
export async function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, devInterfaceObj) {
  const fld = String(fieldName || '').toUpperCase();
  if (fld === 'CRCTVEACCENT_CARMODELBAS') {
    try {
      const txnNum = devInterfaceObj?.FormGetField?.('HSE_CRCTVEACCENT', 'CRCTVEACCENT_ACCDNTNUM', 'scr') || '';
      if (fieldVal && txnNum) {
        await fillFromCarBasis(devInterfaceObj, strScrTag || 'HSE_CRCTVEACCENT', fieldVal, txnNum);
      }
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-11 SubFieldChanged CARMODELBAS:', e);
    }
  }
  return { cancel: 0 };
}

// RQ_HSE_12_4_26_00_40 — GAP-21: prevent DELETE for non-incomplete records
export async function toolBarButtonClicked(eventObj, callBackFn) {
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  if (strBtnName === 'DELETE') {
    const dev = eventObj.devInterfaceObj || {};
    const exec = dev.executeSQLPromise;
    if (typeof exec === 'function') {
      try {
        const pk = (eventObj.fullRecord && (Array.isArray(eventObj.fullRecord) ? eventObj.fullRecord[0] : eventObj.fullRecord))?.PRMKY
          || (eventObj.fullRecord && (Array.isArray(eventObj.fullRecord) ? eventObj.fullRecord[0] : eventObj.fullRecord))?.prmky;
        if (pk) {
          const rs = await exec(`SELECT TOP 1 ISNULL(CRCTVEACCENT_RECSTS,'') AS S FROM HSE_CRCTVEACCENT WHERE PrmKy = ${parseInt(pk, 10)}`);
          const row = rs?.recordset?.[0] ?? rs?.[0];
          if (String(row?.S ?? row?.s ?? '').trim() !== '1') {
            if (dev.AskYesNoMessage) dev.AskYesNoMessage('Only incomplete records can be deleted. (RQ_HSE_12_4_26_00_40)');
            return;
          }
        }
      } catch (_) { /* proceed on error */ }
    }
  }
  if (typeof callBackFn === 'function') callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-8: desktop CarEntryCategory::ChangeToDefaultCriteria — list only incomplete
 * corrective-action rows (CRCTVEACCENT_RECSTS = '1'). Assumption: NULL/blank status is not treated as incomplete
 * here; extend with OR if your DB defaults empty to open work.
 */
export async function onMenuItemClicked(userObj, strScrTag, callBackFn) {
  const tag = String(strScrTag || '').toUpperCase().trim();
  if (tag !== 'HSE_TGCRCTVEACCENT') {
    callBackFn({ isAllowed: true, strScrCriteria: '', strMsg: '' });
    return;
  }
  const strScrCriteria = `WHERE CRCTVEACCENT_RECSTS = '1'`;
  callBackFn({ isAllowed: true, strScrCriteria, strMsg: '' });
}
