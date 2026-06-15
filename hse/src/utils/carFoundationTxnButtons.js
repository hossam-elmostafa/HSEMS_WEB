/**
 * RQ_HSE_30_3_26_14_57 — Desktop parity for CAR foundation / corrective-action transactions.
 * C++: CAREntryCategory::CompleteCorrectiveActionEntry (completeCARTXN),
 *      CARRcvdCategory::AcceptCARExe, CARUndrExctnCategory::CARJobCmpltd / CARJobPndng,
 *      CARRcvdCategory / CARUndrExctnCategory reject → OpenReasonsScr + rejectCARTXN (5-arg).
 * RQ_HSE_12_4_26_00_40 — reject via HSE_RJCTRSN + rejectCARTXN parity (module CRCTVEACCENT-FNDRJ).
 */

import { getCurrentRecordPRMKY, openRejectReasonScreen } from './rejectReasonUtils.js';
// RQ_HSE_12_4_26_00_40 — GAP-9: CAR Not Accepted / Accepted at account level
import { handleCarNotAccepted, handleCarAccepted } from './carAccountAcceptReject.js';
// RQ_HSE_12_4_26_00_40 — GAP-9: View Source TXN on Actions Received / Under Execution
import { handleViewSourceTxnLayer2 } from './carViewSourceTxnLayer2.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * @param {Function} executeSQLPromise
 * @param {string} procName — unquoted procedure name (trusted)
 * @param {string} prmky
 * @param {string} sourceScreenCaption — GetScrCptnByTag equivalent
 * @param {string} userName
 */
export async function executeCarTxnThreeArgs(executeSQLPromise, procName, prmky, sourceScreenCaption, userName) {
  const sql = `EXECUTE ${procName} '${escapeSqlString(prmky)}','${escapeSqlString(sourceScreenCaption)}','${escapeSqlString(userName)}'`;
  await executeSQLPromise(sql);
}

const SCREEN_CAPTION_BY_TAG = {
  HSE_TGCRCTVEACCENT: 'Corrective Action Entry',
  HSE_TGACTNSRCVD: 'Corrective Actions Received',
  HSE_TGACTNSUNDREXEC: 'Corrective Actions Under Execution',
};

/**
 * RQ_HSE_12_4_26_00_40 — GAP-13: Layer 2 "Actions Received" may arrive as HSE_TGACTNSRCVD (index HSE_TgActnsRcvd)
 * or an alternate shell/menu tag (e.g. HSE_TGCRCTVEACCRCVD per gap analysis). Normalize so button handlers match.
 */
const ACTIONS_RECEIVED_SCREEN_TAG_ALIASES = new Set(['HSE_TGACTNSRCVD', 'HSE_TGCRCTVEACCRCVD']);

/** Canonical tag used for SCREEN_CAPTION_BY_TAG / FSA_TAG_FOR_REJECT keys after normalization. */
const ACTIONS_RECEIVED_CANONICAL_TAG = 'HSE_TGACTNSRCVD';

/**
 * RQ_HSE_12_4_26_00_40 — GAP-13
 * @param {string} strScrTag
 * @returns {string} uppercased tag; Actions Received aliases → ACTIONS_RECEIVED_CANONICAL_TAG
 */
export function canonicalFoundationTxnScreenTag(strScrTag) {
  const u = String(strScrTag || '').toUpperCase();
  if (ACTIONS_RECEIVED_SCREEN_TAG_ALIASES.has(u)) return ACTIONS_RECEIVED_CANONICAL_TAG;
  return u;
}

/** C++ m_FSATableName passed as 4th arg to rejectCARTXN (CARCategory.cpp). RQ_HSE_12_4_26_00_40 */
const FSA_TAG_FOR_REJECT = {
  HSE_TGACTNSRCVD: 'HSE_TGCRCTVEACCRCVD',
  HSE_TGACTNSUNDREXEC: 'HSE_TGCRCTVEACCUNDREXCTN',
};

/** Distinct RJCTRSN_MODULETYPE for foundation reject (add to DB constraints if needed). RQ_HSE_12_4_26_00_40 */
export const RJCTRSN_MODULETYPE_FOUNDATION_CAR_REJECT = 'CRCTVEACCENT-FNDRJ';

/** @type {null | { prmky: string, fsaTableTag: string, caption: string }} */
let pendingFoundationCarReject = null;

export function getPendingFoundationCarReject() {
  return pendingFoundationCarReject;
}

export function clearPendingFoundationCarReject() {
  pendingFoundationCarReject = null;
}

/**
 * RQ_HSE_12_4_26_00_40 — 5th arg to rejectCARTXN: prefer reject-row key from HSE_RJCTRSN screen (desktop m_strFinal / CMS_RSNS); fallback CAR PrmKy.
 * @param {object} [eventObj]
 * @param {string|number} fallbackPrmky
 */
function extractRjctRsnCorrelationForRejectTxn(eventObj, fallbackPrmky) {
  const rec = eventObj?.fullRecord;
  const row = Array.isArray(rec) ? rec[0] : rec;
  if (!row || typeof row !== 'object') return String(fallbackPrmky ?? '').trim();
  const names = [
    'RJCTRSN_FINAL',
    'RJCTRSN_RSNKY',
    'RJCTRSN_PRMKY',
    'CMS_RSNS_PRMKY',
    'RJCTRSNKEY',
    'RJCTRSN_RSNS_RFREC', // RQ_HSE_12_4_26_00_40 — gap R3: some shells expose reason row key under alternate names
    'RJCTRSN_RSNSKEY',
    'PRMKY',
    'PrmKy',
  ];
  for (const name of names) {
    const key = Object.keys(row).find((k) => k.toUpperCase() === name.toUpperCase());
    if (!key) continue;
    const v = row[key];
    if (v != null && String(v).trim() !== '') return String(v).trim();
  }
  return String(fallbackPrmky ?? '').trim();
}

/**
 * After HSE_RJCTRSN RJCTRSN_BTN_OK — EXECUTE rejectCARTXN 5-arg (CARCategory::RejectCorrectiveAction).
 * RQ_HSE_12_4_26_00_40 — pass eventObj from RJCTRSN_OK so 5th arg can match desktop when fullRecord exposes a reason row key.
 */
export async function handleRejectReasonOkForFoundationCarTxn(devInterface, eventObj) {
  if (!pendingFoundationCarReject) return false;
  const { prmky, fsaTableTag, caption } = pendingFoundationCarReject;
  const executeSQLAsync = devInterface?.executeSQLPromise || devInterface?.executeSQL;
  const refreshData = devInterface?.refreshData;
  const getUserName = devInterface?.getUserName;
  if (!executeSQLAsync || !prmky) {
    clearPendingFoundationCarReject();
    return false;
  }
  const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const corrRaw = extractRjctRsnCorrelationForRejectTxn(eventObj, prmky);
  const corr = escapeSqlString(corrRaw || String(prmky));
  try {
    const sql = `EXECUTE rejectCARTXN '${escapeSqlString(prmky)}','${escapeSqlString(caption)}','${escapeSqlString(userName)}','${escapeSqlString(fsaTableTag)}','${corr}'`;
    await executeSQLAsync(sql);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.error('[Web_HSE] RQ_HSE_12_4_26_00_40 rejectCARTXN:', e);
  }
  clearPendingFoundationCarReject();
  return true;
}

function captionForTag(strScrTag) {
  // RQ_HSE_12_4_26_00_40 — GAP-13: caption map keys canonical Actions Received tag
  const u = canonicalFoundationTxnScreenTag(strScrTag);
  return SCREEN_CAPTION_BY_TAG[u] || u;
}

/**
 * C++: CarEntryCategory — CRCTVEACCENT_ENTCMPLT → EXECUTE completeCARTXN
 * @returns {Promise<boolean>} true if this handler consumed the action (success or handled error)
 */
export async function handleCompleteCorrectiveActionEntry(eventObj, devInterfaceObj) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;

  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) {
    if (AskYesNoMessage) AskYesNoMessage('You must save the Record first');
    return true;
  }

  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const screenTag = (eventObj?.strScrTag || '').toString();
  const caption = captionForTag(screenTag);

  try {
    await executeCarTxnThreeArgs(executeSQLPromise, 'completeCARTXN', prmky, caption, user);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_30_3_26_14_57 completeCARTXN:', e);
    if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error executing completeCARTXN'));
  }
  return true;
}

/**
 * Actions Received / Actions Under Execution — SPs matching CARRcvdCategory / CARUndrExctnCategory.
 * @returns {Promise<boolean>} true if button was handled
 */
export async function handleCarFoundationTxnButtons(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = (buttonName && String(buttonName).toUpperCase()) || '';
  // RQ_HSE_12_4_26_00_40 — GAP-13: normalize Actions Received aliases before strict equality checks
  const scr = canonicalFoundationTxnScreenTag(strScrTag);
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage, openScr } = devInterfaceObj || {};

  // RQ_HSE_12_4_26_00_40 — GAP-9: before prmky gate (opens inquiry from row basis + ACCDNTNUM)
  if (btn === 'VIEW_SOURCE_TXN' && (scr === 'HSE_TGACTNSRCVD' || scr === 'HSE_TGACTNSUNDREXEC')) {
    return handleViewSourceTxnLayer2(eventObj, devInterfaceObj);
  }

  if (!executeSQLPromise) return false;

  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;

  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';
  const caption = captionForTag(scr);

  const run = async (proc) => {
    await executeCarTxnThreeArgs(executeSQLPromise, proc, prmky, caption, user);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  };

  // RQ_HSE_12_4_26_00_40 — GAP-9: Account-level accept/reject (shared across Received + Under Execution)
  if (btn === 'CRCTVEACCENT_CARNOTACPTD') {
    return handleCarNotAccepted(eventObj, devInterfaceObj);
  }
  if (btn === 'CRCTVEACCENT_CARACPTD') {
    return handleCarAccepted(eventObj, devInterfaceObj);
  }

  try {
    if (scr === 'HSE_TGACTNSRCVD') {
      if (btn === 'CRCTVEACCENT_ACPTDEXECTN') {
        await run('AcceptCARExe');
        return true;
      }
      // RQ_HSE_12_4_26_00_40 — C++ CARRcvdCategory: OpenReasonsScr then RejectCorrectiveAction → rejectCARTXN
      if (btn === 'CRCTVEACCENT_RJCTEXECTN') {
        const fsa = FSA_TAG_FOR_REJECT[scr];
        if (typeof openScr === 'function' && fsa) {
          pendingFoundationCarReject = { prmky, fsaTableTag: fsa, caption };
          openRejectReasonScreen(openScr, RJCTRSN_MODULETYPE_FOUNDATION_CAR_REJECT, prmky);
          return true;
        }
        return false;
      }
    }
    if (scr === 'HSE_TGACTNSUNDREXEC') {
      if (btn === 'CRCTVEACCENT_JOBCMPLTD') {
        await run('CARJobCmpltd');
        return true;
      }
      if (btn === 'CRCTVEACCENT_JOBPNDNG') {
        // RQ_HSE_12_4_26_00_40 — GAP-18: desktop checks RECSTS != '26' before allowing re-pending
        try {
          const stsRs = await executeSQLPromise(`SELECT TOP 1 ISNULL(CRCTVEACCENT_RECSTS,'') AS S FROM HSE_CRCTVEACCENT WHERE PrmKy = ${parseInt(prmky, 10)}`);
          const stsRow = stsRs?.recordset?.[0] ?? stsRs?.[0];
          if (String(stsRow?.S ?? stsRow?.s ?? '').trim() === '26') {
            if (AskYesNoMessage) AskYesNoMessage('Record is already pending. (RQ_HSE_12_4_26_00_40)');
            return true;
          }
        } catch (_) { /* proceed on read error */ }
        await run('CARJobPndng');
        return true;
      }
      if (btn === 'CRCTVEACCENT_RJCT_UNDREXCTN') {
        const fsa = FSA_TAG_FOR_REJECT[scr];
        if (typeof openScr === 'function' && fsa) {
          pendingFoundationCarReject = { prmky, fsaTableTag: fsa, caption };
          openRejectReasonScreen(openScr, RJCTRSN_MODULETYPE_FOUNDATION_CAR_REJECT, prmky);
          return true;
        }
        return false;
      }
    }
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_30_3_26_14_57 carFoundationTxnButtons:', e);
    if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Operation failed'));
    return true;
  }

  return false;
}
