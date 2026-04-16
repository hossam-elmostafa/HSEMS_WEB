/**
 * RQ_HSE_12_4_26_00_40 — GAP-2/3/4/9: Shared button handlers for Layer 2 (HSE_CRCTVEACCENT) screens.
 * C++: CarConfirmationCategory, CARJobVrfctnCategory, CarFlwupVstCategory.
 * Covers: Confirmation, Job Verification, Follow-Up Visit screens + VIEW_SOURCE_TXN (GAP-9).
 */

import { handleViewSourceTxnLayer2 } from './carViewSourceTxnLayer2.js';
import { getCurrentRecordPRMKY, openRejectReasonScreen } from './rejectReasonUtils.js';
import { executeCarTxnThreeArgs } from './carFoundationTxnButtons.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

function getFieldFromRecord(eventObj, fieldName) {
  const fullRecordArr = eventObj?.fullRecord ?? eventObj?.fullRecordArr ?? [];
  const rec = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
  if (!rec || typeof rec !== 'object') return '';
  const u = String(fieldName).toUpperCase();
  for (const k of Object.keys(rec)) {
    if (k.toUpperCase() === u) return (rec[k] != null ? String(rec[k]) : '').trim();
  }
  return '';
}

const SCREEN_CAPTION = {
  HSE_TGCRCTVEACCCNFRMTN: 'Corrective Action Confirmation',
  HSE_TGCRCTVEACCJOBVRFCTN: 'Corrective Action Job Verification',
  HSE_TGCARFLWUPVSTS: 'Corrective Action Approval',
};

const FSA_TAG_FOR_REJECT = {
  HSE_TGCRCTVEACCCNFRMTN: 'HSE_TGCRCTVEACCCNFRMTN',
  HSE_TGCRCTVEACCJOBVRFCTN: 'HSE_TGCRCTVEACCJOBVRFCTN',
  HSE_TGCARFLWUPVSTS: 'HSE_TGCARFLWUPVSTS',
};

const RJCTRSN_MODULE_BY_SCREEN = {
  HSE_TGCRCTVEACCCNFRMTN: 'CRCTVEACCENT-L1',
  HSE_TGCRCTVEACCJOBVRFCTN: 'CRCTVEACCENT-L1',
  HSE_TGCARFLWUPVSTS: 'CRCTVEACCENT-L2',
};

/** @type {null | object} */
let pendingLayer2Reject = null;

export function getPendingLayer2Reject() { return pendingLayer2Reject; }
export function clearPendingLayer2Reject() { pendingLayer2Reject = null; }

/**
 * RQ_HSE_12_4_26_00_40 — After HSE_RJCTRSN OK for Layer 2 reject: EXECUTE rejectCARTXN 5-arg.
 */
export async function handleRejectReasonOkForLayer2(devInterface, eventObj) {
  if (!pendingLayer2Reject) return false;
  const { prmky, fsaTableTag, caption } = pendingLayer2Reject;
  const executeSQLAsync = devInterface?.executeSQLPromise || devInterface?.executeSQL;
  const refreshData = devInterface?.refreshData;
  const getUserName = devInterface?.getUserName;
  if (!executeSQLAsync || !prmky) {
    clearPendingLayer2Reject();
    return false;
  }
  const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  try {
    const sql = `EXECUTE rejectCARTXN '${escapeSqlString(prmky)}','${escapeSqlString(caption)}','${escapeSqlString(userName)}','${escapeSqlString(fsaTableTag)}','${escapeSqlString(prmky)}'`;
    await executeSQLAsync(sql);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.error('[Web_HSE] RQ_HSE_12_4_26_00_40 Layer2 rejectCARTXN:', e);
  }
  clearPendingLayer2Reject();
  return true;
}

function captionForTag(scr) {
  return SCREEN_CAPTION[scr] || scr;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-2: Confirmation screen buttons.
 * C++: CarConfirmationCategory — CNFRM, CNFRMTN_RJC, CNFRMTN_CNCL.
 */
async function handleConfirmationButtons(btn, scr, eventObj, dev) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage, openScr } = dev || {};
  if (!executeSQLPromise) return false;
  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;
  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const caption = captionForTag(scr);

  if (btn === 'CRCTVEACCENT_CNFRM') {
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'confirmCARTXN', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 confirmCARTXN:', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error confirming CAR'));
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_CNFRMTN_RJC') {
    if (typeof openScr === 'function') {
      pendingLayer2Reject = { prmky, fsaTableTag: FSA_TAG_FOR_REJECT[scr], caption };
      openRejectReasonScreen(openScr, RJCTRSN_MODULE_BY_SCREEN[scr], prmky);
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_CNFRMTN_CNCL') {
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'CancelCARTXN', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CancelCARTXN (confirmation):', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error cancelling CAR'));
    }
    return true;
  }

  return false;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-3: Job Verification screen buttons.
 * C++: CARJobVrfctnCategory — JOBACPTD, JOBRJCTD, CNCL.
 */
async function handleJobVerificationButtons(btn, scr, eventObj, dev) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage, openScr } = dev || {};
  if (!executeSQLPromise) return false;
  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;
  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const caption = captionForTag(scr);

  if (btn === 'CRCTVEACCENT_JOBACPTD_JOBVRFCTN') {
    const modelNum = getFieldFromRecord(eventObj, 'CRCTVEACCENT_CARMODELNUM');
    if (!modelNum) {
      if (AskYesNoMessage) AskYesNoMessage('CAR model number is required (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'CARJobAcptd', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CARJobAcptd:', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error accepting job'));
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_JOBRJCTD_JOBVRFCTN') {
    if (typeof openScr === 'function') {
      pendingLayer2Reject = { prmky, fsaTableTag: FSA_TAG_FOR_REJECT[scr], caption };
      openRejectReasonScreen(openScr, RJCTRSN_MODULE_BY_SCREEN[scr], prmky);
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_CNCL_JOBVRFCTN') {
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'CARJobCncld', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CARJobCncld:', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error cancelling job'));
    }
    return true;
  }

  return false;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-4: Follow-Up Visit screen buttons.
 * C++: CarFlwupVstCategory — CLS2NDCYCL (closeCARTXN + notification), RJC (auto-gen guard), CNCL.
 */
async function handleFollowUpVisitButtons(btn, scr, eventObj, dev) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage, openScr } = dev || {};
  if (!executeSQLPromise) return false;
  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;
  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const caption = captionForTag(scr);

  if (btn === 'CLS2NDCYCL') {
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'closeCARTXN', prmky, caption, user);
      // RQ_HSE_12_4_26_00_40 — GAP-15: email notification on close
      try {
        const rs = await executeSQLPromise(
          `SELECT CRCTVEACCENT_NO, CRCTVEACCENT_DPRT FROM HSE_CRCTVEACCENT WHERE PrmKy = ${parseInt(prmky, 10)}`
        );
        const row = rs?.recordset?.[0] ?? rs?.[0];
        if (row) {
          const carNo = String(row.CRCTVEACCENT_NO ?? row.crctveaccent_no ?? '').trim();
          const dept = String(row.CRCTVEACCENT_DPRT ?? row.crctveaccent_dprt ?? '').trim();
          if (dept && carNo) {
            const mailRs = await executeSQLPromise(
              `SELECT TOP 1 ISNULL(DEPARTMENT_EMAIL,'') AS EMAIL FROM CMN_DEPARTMENT WHERE DEPARTMENT_CODE = '${escapeSqlString(dept)}'`
            );
            const mailRow = mailRs?.recordset?.[0] ?? mailRs?.[0];
            const email = String(mailRow?.EMAIL ?? mailRow?.email ?? '').trim();
            if (email) {
              await executeSQLPromise(
                `EXECUTE closeCARNotification '${escapeSqlString(email)}','${escapeSqlString(carNo)}'`
              );
            }
          }
        }
      } catch (notifErr) {
        console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 closeCARNotification:', notifErr);
      }
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 closeCARTXN:', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error closing CAR'));
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_CARFLWUPVSTS_RJC') {
    const autoGen = getFieldFromRecord(eventObj, 'CRCTVEACCENT_AUTOGNRTD');
    if (autoGen.toUpperCase() === 'Y') {
      if (AskYesNoMessage) AskYesNoMessage('Automatic generated CARs can be cancelled only. (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    if (typeof openScr === 'function') {
      pendingLayer2Reject = { prmky, fsaTableTag: FSA_TAG_FOR_REJECT[scr], caption };
      openRejectReasonScreen(openScr, RJCTRSN_MODULE_BY_SCREEN[scr], prmky);
    }
    return true;
  }

  if (btn === 'CRCTVEACCENT_CARFLWUPVSTS_CNCL') {
    try {
      await executeCarTxnThreeArgs(executeSQLPromise, 'CancelCARTXN', prmky, caption, user);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CancelCARTXN (follow-up):', e);
      if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error cancelling CAR'));
    }
    return true;
  }

  return false;
}

/**
 * RQ_HSE_12_4_26_00_40 — Main dispatcher for Layer 2 buttons.
 * @returns {Promise<boolean>} true if button was consumed
 */
export async function handleCarLayer2Button(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = (buttonName && String(buttonName).toUpperCase()) || '';
  const scr = (strScrTag && String(strScrTag).toUpperCase()) || '';

  // RQ_HSE_12_4_26_00_40 — GAP-9: C++ CarCategory::OpenTXNInquiryScreen on Layer 2
  if (btn === 'VIEW_SOURCE_TXN') {
    return handleViewSourceTxnLayer2(eventObj, devInterfaceObj);
  }

  if (scr === 'HSE_TGCRCTVEACCCNFRMTN') {
    return handleConfirmationButtons(btn, scr, eventObj, devInterfaceObj);
  }
  if (scr === 'HSE_TGCRCTVEACCJOBVRFCTN') {
    return handleJobVerificationButtons(btn, scr, eventObj, devInterfaceObj);
  }
  if (scr === 'HSE_TGCARFLWUPVSTS') {
    return handleFollowUpVisitButtons(btn, scr, eventObj, devInterfaceObj);
  }

  return false;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-2: Check confirmation policy before opening screen.
 * C++: CarConfirmationCategory::CheckPolicy(Is_CAR_Confirmation_Required).
 */
export async function checkConfirmationPolicy(executeSQLPromise, AskYesNoMessage) {
  try {
    const rs = await executeSQLPromise(
      `SELECT TOP 1 ISNULL(HSEPLC_CRCTVEACCCNFRMTNRQRD,'') AS V FROM HSE_HSEPLC`
    );
    const row = rs?.recordset?.[0] ?? rs?.[0];
    const val = String(row?.V ?? row?.v ?? '').trim().toUpperCase();
    if (val !== 'Y') {
      if (typeof AskYesNoMessage === 'function') {
        AskYesNoMessage('CAR Confirmation checkbox must be checked in policy to open this screen. (RQ_HSE_12_4_26_00_40)');
      }
      return false;
    }
    return true;
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 checkConfirmationPolicy:', e);
    return true;
  }
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-4: Check follow-up visit policy before opening screen.
 * C++: CarFlwupVstCategory::CheckPolicy(Is_CAR_Follow_Up_Required).
 */
export async function checkFollowUpVisitPolicy(executeSQLPromise, AskYesNoMessage) {
  try {
    const rs = await executeSQLPromise(
      `SELECT TOP 1 ISNULL(HSEPLC_CRCTVEACCFLW_UPRQRD,'') AS V FROM HSE_HSEPLC`
    );
    const row = rs?.recordset?.[0] ?? rs?.[0];
    const val = String(row?.V ?? row?.v ?? '').trim().toUpperCase();
    if (val !== 'Y') {
      if (typeof AskYesNoMessage === 'function') {
        AskYesNoMessage('CAR Follow-Up Visit checkbox must be checked in policy to open this screen. (RQ_HSE_12_4_26_00_40)');
      }
      return false;
    }
    return true;
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 checkFollowUpVisitPolicy:', e);
    return true;
  }
}
