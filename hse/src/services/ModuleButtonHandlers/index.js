/**
 * Module custom button handlers — desktop DisplayCustomButtonClicked parity.
 * Routes by screen tag and button name; executes stored procedures and refreshes.
 * Observation and CAR are handled by ObservationService and carCustomButtons.js.
 */

import {
  getKeyFromEvent,
  runTxnAndRefresh,
  runMultipleSqlAndRefresh,
  getScreenCaption,
  escapeSqlString,
  buildUpdateStatusAndTracingSql,
  isIncidentPreliminaryEntryScreenTag,
  validateInjuredPersonCountMatchesHeader,
} from './moduleButtonHandlersUtils.js';
import { openRejectReasonScreen } from '../../utils/rejectReasonUtils.js';
import { setPendingRejectForModule } from '../Observation service/ObservationButtonHandlers.js';
import { handleEQInspectionModuleButtons } from './eqInspectionButtonHandlers.js';

// ----- Risk Assessment (RiskAssessmentEntryCategory, RiskAssessmentApprovalCategory, RiskAssessmentFollowUpCategory) -----
const RISK_TABLE = 'HSE_RSKASSMNTENT';
const RISK_KEY_FIELD = 'RSKASSMNTENT_ASSMNTNO';
const RISK_ENTRY_TAGS = ['HSE_TGRSKASSMNTENT'];
const RISK_APR_TAGS = ['HSE_TGRSKASSMNTAPR'];
const RISK_FOLLOWUP_TAGS = ['HSE_TGRSKASSMNTFOLLWUPDT'];

async function handleRiskAssessmentButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: RISK_TABLE, keyFieldName: RISK_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Risk Assessment';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);

  if (buttonName === 'RSKASSMNTENT_ENTCMPLT' && (RISK_ENTRY_TAGS.includes(scr) || scr.includes('RSKASSMNTENT'))) {
    if (!key) {
      if (typeof devInterfaceObj?.AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE completeRskAssmntTXN '${ek}','${es}','${eu}'`, 'Error completing risk assessment');
  }
  // RQ_HSE_23_3_26_6_00: SAVE before confirm (desktop RiskAssessmentApprovalCategory parity)
  if (buttonName === 'RSKASSMNTENT_APRV' && RISK_APR_TAGS.some(t => scr === t)) {
    if (!key) return true;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj?.strFormTag || strScrTag, '');
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE confirmRskAssmntTXN '${ek}','${es}','${eu}'`, 'Error approving risk assessment');
  }
  // RQ_HSE_23_3_26_6_00: SAVE before close (desktop RiskAssessmentFollowUpCategory parity)
  if (buttonName === 'RSKASSMNTENT_CLS' && RISK_FOLLOWUP_TAGS.some(t => scr === t)) {
    if (!key) return true;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj?.strFormTag || strScrTag, '');
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closeRskAssmntTXN '${ek}','${eu}','${es}'`, 'Error closing risk assessment');
  }
  if (buttonName === 'RSKASSMNTENT_SHOWMATRIX') {
    return true;
  }
  // RQ_HSE_23_3_26_6_00: open potential hazard popup (desktop RiskAssessmentEntryCategory openScr HSE_TGPTNLHZRD)
  if (buttonName === 'POTENTIAL_HAZARDS') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') openScr('HSE_TGPTNLHZRD', '', true, key);
    return true;
  }
  // RQ_HSE_23_3_26_6_00: open task responsibility popup (desktop RiskAssessmentEntryCategory openScr HSE_TGTskRsp)
  if (buttonName === 'TASK_RESPONSIBILITY') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') openScr('HSE_TGTskRsp', '', true, key);
    return true;
  }
  if (buttonName === 'RSKASSMNTENT_RJC') {
    if (!key) return true;
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'RSKASSMNTENT-L1', key);
      setPendingRejectForModule('RISK', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  return false;
}

// ----- Site Survey (SitSrvyCnfrmtnCategory, SitSrvyFlwupCategory, SitSrvyCategory reject) -----
// RQ_HSE_23_3_26_22_44: SAVE before close; reject reason (SITSRVYENT-L1/L2) before rejectSitSrvy on Confirmation/Follow-up
const SITSRVY_TABLE = 'HSE_SITSRVYENT';
const SITSRVY_KEY_FIELD = 'SITSRVYENT_SITSRVYNO';

async function handleSiteSurveyButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: SITSRVY_TABLE, keyFieldName: SITSRVY_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Site Survey';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);

  if (buttonName === 'SITSRVYENT_CNFRM') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE confirmSiteSrvy '${ek}','${es}','${eu}'`, 'Error confirming site survey');
  }
  if (buttonName === 'SITSRVYENT_CLS') {
    if (!key) return true;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj?.strFormTag || strScrTag, '');
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closeSiteSurveyTXN '${ek}','${eu}','${es}'`, 'Error closing site survey');
  }
  if (buttonName === 'SITSRVYENT_RJCT') {
    if (!key) return true;
    const openScr = devInterfaceObj?.openScr;
    if (scr === 'HSE_TGSITSRVYCNFRMTN' && typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'SITSRVYENT-L1', key);
      setPendingRejectForModule('SITE_SURVEY', key, strScrTag, devInterfaceObj);
      return true;
    }
    if (scr === 'HSE_TGSITSRVYFLWUP' && typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'SITSRVYENT-L2', key);
      setPendingRejectForModule('SITE_SURVEY', key, strScrTag, devInterfaceObj);
      return true;
    }
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE rejectSitSrvy '${ek}','${es}','${eu}'`, 'Error rejecting site survey');
  }
  return false;
}

// ----- PTW (PtwEntryCategory, PtwConfirmationCategory, PtwClsCategory, PtwCategory reject) -----
const PTW_TABLE = 'HSE_PTWSRGSTRENT';
const PTW_KEY_FIELD = 'PTWSRGSTRENT_PTWSNUM';

async function handlePTWButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: PTW_TABLE, keyFieldName: PTW_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'PTW';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);

  if (buttonName === 'PTWSRGSTRENT_ENTCMPLT') {
    if (!key) {
      if (typeof devInterfaceObj?.AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE completePTWTXN '${ek}','${es}','${eu}'`, 'Error completing PTW');
  }
  // RQ_HSE_23_3_26_6_00: SAVE before confirm (desktop PtwConfirmationCategory parity)
  if (buttonName === 'PTWSRGSTRENT_CNFRM') {
    if (!key) return true;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj?.strFormTag || strScrTag, '');
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE confirmPTWTXN '${ek}','${eu}','${es}'`, 'Error confirming PTW');
  }
  // RQ_HSE_23_3_26_6_00: SAVE before close (desktop PtwClsCategory parity)
  if (buttonName === 'PTWSRGSTRENT_CLSPTW') {
    if (!key) return true;
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', eventObj?.strFormTag || strScrTag, '');
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closePTW '${ek}','${es}','${eu}'`, 'Error closing PTW');
  }
  if (buttonName === 'PTWSRGSTRENT_RJC') {
    if (!key) return true;
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'PTWSRGSTRENT-L1', key);
      setPendingRejectForModule('PTW', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  return false;
}

// ----- Incident (AccidentEntryCategory, AccidentConfirmationCategory, AccidentFollowUpCategory, IncdntInvstgtnAprvCategory) -----
// RQ_HSE_23_3_26_3_36
const ACDNT_TABLE = 'HSE_ACDNTENT';
const ACDNT_KEY_FIELD = 'ACDNTENT_ACDNTNUM';

/** C++ RefreshScreen("", REFRESH_SELECTED) after incident SPs — RQ_HSE_23_3_26_3_36 */
async function runIncidentTxn(devInterfaceObj, sql, messageOnError = 'Operation failed') {
  const { executeSQLPromise, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;
  try {
    await executeSQLPromise(sql);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
    return true;
  } catch (e) {
    console.warn('[Web_HSE] Incident txn:', e);
    if (typeof AskYesNoMessage === 'function') AskYesNoMessage(messageOnError);
    return false;
  }
}

// RQ_HSE_23_3_26_3_36
function incidentHasSelectedRecord(eventObj, key) {
  if (key) return true;
  const keys = eventObj?.fullRecordArrKeys;
  if (keys && keys.length > 0) return true;
  const fr = eventObj?.fullRecord ?? eventObj?.fullRecordArr;
  return Array.isArray(fr) && fr.length > 0;
}

// RQ_HSE_23_3_26_3_36
async function handleIncidentButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: ACDNT_TABLE, keyFieldName: ACDNT_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Incident';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);
  const formTag = eventObj?.strFormTag || strScrTag || 'HSE_TGACDNTENT';
  const executeSQLAsync = devInterfaceObj?.executeSQLPromise || devInterfaceObj?.executeSQL;
  const doToolbarAction = devInterfaceObj?.doToolbarAction;
  const AskYesNoMessage = devInterfaceObj?.AskYesNoMessage;

  // Investigation Approval: direct rejectIncident (IncdntInvstgtnAprvCategory — no reject-reason dialog) — RQ_HSE_23_3_26_3_36
  if (buttonName === 'ACDNTENT_RJC' && scr === 'HSE_TGINCDNTINVSTGTNAPRVL') {
    if (!key) return true;
    await runIncidentTxn(
      devInterfaceObj,
      `EXECUTE rejectIncident '${ek}','${es}','${eu}'`,
      'Error rejecting incident'
    );
    return true;
  }

  // Preliminary Entry: desktop rejectRecord does not open dialog — no-op — RQ_HSE_23_3_26_3_36
  if (buttonName === 'ACDNTENT_RJC' && isIncidentPreliminaryEntryScreenTag(strScrTag)) {
    return true;
  }

  // RQ_HSE_23_3_26_3_36 — Review/Follow-up reject: L1 vs L2 reject-reason module
  if (buttonName === 'ACDNTENT_RJC') {
    if (!key) return true;
    const openScr = devInterfaceObj?.openScr;
    const rjctMod = scr === 'HSE_TGACDNTFOLLWUPDT' ? 'ACDNTENT-L2' : 'ACDNTENT-L1';
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, rjctMod, key);
      setPendingRejectForModule('INCIDENT', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }

  // RQ_HSE_23_3_26_3_36 — SAVE before closeAccidentTXN (desktop parity)
  if (buttonName === 'ACDNTENT_CLS') {
    if (!key) return true;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', formTag, '');
    return runIncidentTxn(
      devInterfaceObj,
      `EXECUTE closeAccidentTXN '${ek}','${eu}','${es}'`,
      'Error closing incident'
    );
  }

  // RQ_HSE_23_3_26_3_36 — SAVE + L1 reject-reason check/delete before confirmAccidentTXN
  if (buttonName === 'ACDNTENT_CNFRM' || buttonName === 'ACDNTENT_APRV') {
    if (!key) return true;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', formTag, '');
    let rejectReasonCount = 0;
    if (executeSQLAsync) {
      try {
        const checkSql = `
          SELECT COUNT(0) AS CNT FROM HSE_RJCTRSN
          WHERE RJCTRSN_MODULETYPE = 'ACDNTENT-L1'
            AND RJCTRSN_LINKWITHMAIN = '${ek}'
            AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0
        `;
        const rs = await executeSQLAsync(checkSql);
        const recordset = rs?.recordset || rs?.[0]?.recordset || [];
        if (Array.isArray(recordset) && recordset.length > 0) {
          const row = recordset[0];
          const val = row.CNT ?? row.cnt ?? Object.values(row)[0];
          rejectReasonCount = parseInt(val, 10) || 0;
        }
      } catch (err) {
        console.warn('[Web_HSE] Incident confirm: reject reason check failed:', err);
      }
    }
    if (rejectReasonCount > 0) {
      const msg =
        'Reject reason for this record exists.\n\rDo you want to delete it and confirm this incident?';
      const confirmed =
        typeof AskYesNoMessage === 'function' ? await AskYesNoMessage('Prompt', msg) : false;
      if (!confirmed) return true;
      try {
        await executeSQLAsync(`
          DELETE FROM HSE_RJCTRSN
          WHERE RJCTRSN_MODULETYPE = 'ACDNTENT-L1'
            AND RJCTRSN_LINKWITHMAIN = '${ek}'
            AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0
        `);
      } catch (delErr) {
        console.error('[Web_HSE] Failed to delete incident reject reasons:', delErr);
        if (typeof AskYesNoMessage === 'function') AskYesNoMessage('Error deleting reject reasons.');
        return true;
      }
    }
    return runIncidentTxn(
      devInterfaceObj,
      `EXECUTE confirmAccidentTXN '${ek}','${es}','${eu}'`,
      'Error confirming incident'
    );
  }

  // Investigation Entry: ACDNTENT_ENTRYCMPLTD → completeAccidentTXN (IncdntInvstgtnEntryCategory) — RQ_HSE_23_3_26_3_36
  if (buttonName === 'ACDNTENT_ENTRYCMPLTD') {
    if (!key) {
      if (typeof AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    return runIncidentTxn(
      devInterfaceObj,
      `EXECUTE completeAccidentTXN '${ek}','${es}','${eu}'`,
      'Error completing incident'
    );
  }

  // Preliminary Entry: ACDNTENT_ENTCMPLT + ValidateBeforeIncdntEntryComplt prompt — RQ_HSE_23_3_26_3_36
  if (buttonName === 'ACDNTENT_ENTCMPLT' && isIncidentPreliminaryEntryScreenTag(strScrTag)) {
    if (!incidentHasSelectedRecord(eventObj, key)) {
      if (typeof AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    if (!key) {
      if (typeof AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    const injMatch = await validateInjuredPersonCountMatchesHeader(devInterfaceObj, key);
    if (!injMatch.ok && injMatch.message) {
      if (typeof AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage(injMatch.message);
      return true;
    }
    const rid = parseInt(String(key).trim(), 10);
    if (!Number.isNaN(rid) && String(rid) === String(key).trim() && executeSQLAsync) {
      try {
        const vrs = await executeSQLAsync(`SELECT dbo.ValidateBeforeIncdntEntryComplt(${rid}) AS V`);
        const row = vrs?.recordset?.[0] ?? vrs?.[0];
        const v = (row?.V ?? row?.v ?? '').toString().trim().toUpperCase();
        if (v === 'N') {
          const msg =
            'Some Recommended Actions don\'t have Linked Cause , proceed ?';
          const ok =
            typeof AskYesNoMessage === 'function' ? await AskYesNoMessage('Prompt', msg) : false;
          if (!ok) return true;
        }
      } catch (valErr) {
        console.warn('[Web_HSE] ValidateBeforeIncdntEntryComplt:', valErr);
      }
    }
    return runIncidentTxn(
      devInterfaceObj,
      `EXECUTE completeAccidentTXN '${ek}','${es}','${eu}'`,
      'Error completing incident'
    );
  }

  return false;
}

// ----- Performance Measurement -----
const PERF_TABLE = 'HSE_PERFMSRMNTENT';
const PERF_KEY_FIELD = 'PERFMSRMNTENT_VSTNO';

async function handlePerformanceMeasurementButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: PERF_TABLE, keyFieldName: PERF_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Performance Measurement';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);

  // Entry screen: ENTCMPLT -> complete; Evaluation screen: ENTCMPLT -> confirm (desktop same button name, different screen)
  if (buttonName === 'PERFMSRMNTENT_ENTCMPLT' || buttonName === 'PERFMSRMNTENT_ENTRYCMPLT') {
    if (!key) return true;
    const isEvaluation = scr.includes('EVAL') || scr.includes('PERFMSRMNTEVAL');
    const sp = isEvaluation ? `EXECUTE confirmPrfrmncMsrmntTXN '${ek}','${es}','${eu}'` : `EXECUTE completePrfrmncMsrmntTXN '${ek}','${es}','${eu}'`;
    return runTxnAndRefresh(devInterfaceObj, sp, isEvaluation ? 'Error confirming performance measurement' : 'Error completing performance measurement');
  }
  if (buttonName === 'PERFMSRMNTENT_CNFRM' || buttonName === 'PERFMSRMNTENT_APRV') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE confirmPrfrmncMsrmntTXN '${ek}','${es}','${eu}'`, 'Error confirming performance measurement');
  }
  if ((buttonName === 'PERFMSRMNTENT_CLS' || buttonName === 'PERFMSRMNTENT_CLSVST')) {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closePrfrmncMsrmntTXN '${ek}','${eu}','${es}'`, 'Error closing performance measurement');
  }
  if (buttonName === 'PERFMSRMNTENT_RJCT' || buttonName === 'PERFMSRMNTENT_RJC') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE rejectPerformaceTXN '${ek}','${es}','${eu}'`, 'Error rejecting performance measurement');
  }
  return false;
}

// ----- Audit -----
const AUDIT_TABLE = 'HSE_ADTPLNENT';
const AUDIT_KEY_FIELD = 'ADTPLNENT_ADTNUM';

async function handleAuditButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: AUDIT_TABLE, keyFieldName: AUDIT_KEY_FIELD, formTag: strScrTag });
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Audit';
  const ek = escapeSqlString(key);
  const eu = escapeSqlString(userName);
  const es = escapeSqlString(sourceScreen);

  if (buttonName === 'ADTPLNENT_ENTCMPLT' || buttonName === 'ADTPLNENT_ENTRYCMPLT' || buttonName === 'PLAN_ENTRY_COMPLETED') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE completeAuditTXN '${ek}','${es}','${eu}'`, 'Error completing audit');
  }
  if (buttonName === 'ADTPLNENT_APRV' || buttonName === 'ADTPLNENT_CNFRM') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE confirmAuditTXN '${ek}','${es}','${eu}'`, 'Error confirming audit');
  }
  if (buttonName === 'ADTPLNENT_RJC') {
    if (!key) return true;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE rejectAudit '${ek}','${es}','${eu}'`, 'Error rejecting audit');
  }
  return false;
}

// ----- Rescue Plan (RscuPlnCategory.cpp parity) -----
// RQ_HSE_23_3_26_17_05: cancel via reject-reason + cancelRscuPlan; return-to-entry (status ≥5 + Yes/No) + rtrnRscuPlnToEntry; tracing View cancel reason → CMS_RSNS; close unchanged.
const RESCUE_PLN_MAIN_TABLE = 'HSE_RSCUPLN';
const RESCUE_PLN_KEY_FIELD = 'RSCUPLN_PRMRYKY';
const RESCUE_ENT_TABLE = 'HSE_RSCUPLNENT';
const RESCUE_ENT_KEY_FIELD = 'RSCUPLNENT_RSCUPLNNO';

function getRescuePlanPrimaryKey(eventObj, devInterfaceObj, strScrTag) {
  let k = getKeyFromEvent(eventObj, devInterfaceObj, {
    tableName: RESCUE_PLN_MAIN_TABLE,
    keyFieldName: RESCUE_PLN_KEY_FIELD,
    formTag: strScrTag,
  });
  if (!k) {
    k = getKeyFromEvent(eventObj, devInterfaceObj, {
      tableName: RESCUE_ENT_TABLE,
      keyFieldName: RESCUE_ENT_KEY_FIELD,
      formTag: strScrTag,
    });
  }
  return k;
}

async function handleRescuePlanButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = String(buttonName || '').toUpperCase();
  const sourceScreen = getScreenCaption(strScrTag) || 'Rescue Plan';
  const FormGetField = devInterfaceObj?.FormGetField;
  const openScr = devInterfaceObj?.openScr;
  const AskYesNoMessage = devInterfaceObj?.AskYesNoMessage;

  if (btn === 'RSCUPLN_PLNCLSD' || btn === 'RSCUPLNENT_CLS') {
    const key = getKeyFromEvent(eventObj, devInterfaceObj, {
      tableName: RESCUE_ENT_TABLE,
      keyFieldName: RESCUE_ENT_KEY_FIELD,
      formTag: strScrTag,
    });
    if (!key) return true;
    // RQ_HSE_23_3_26_17_05: desktop comment — count rescue team tab rows must match header "No. rescue team members".
    // C++ delegates to closeRscuPlan SP (no client-side check); add best-effort client guard matching comment intent.
    const exec = devInterfaceObj?.executeSQLPromise;
    if (exec && FormGetField) {
      try {
        const planKey = getRescuePlanPrimaryKey(eventObj, devInterfaceObj, strScrTag) || key;
        const ePK = escapeSqlString(planKey);
        const teamCntSql = `SELECT COUNT(0) AS CNT FROM HSE_RSCUPLN_RSCUTM WHERE RSCUPLN_RSCUTM_LNK = '${ePK}'`;
        const teamData = await exec(teamCntSql);
        const teamRow = teamData?.recordset?.[0] ?? teamData?.[0]?.recordset?.[0] ?? teamData?.[0];
        const teamCnt = parseInt(teamRow?.CNT ?? teamRow?.cnt ?? Object.values(teamRow || {})[0], 10);
        const hdrVal = FormGetField(RESCUE_PLN_MAIN_TABLE, 'RSCUPLN_NORSCUTMMMBRS', 'scr');
        const hdrNum = parseInt(String(hdrVal ?? '').trim(), 10);
        if (Number.isFinite(teamCnt) && Number.isFinite(hdrNum) && teamCnt !== hdrNum) {
          if (typeof AskYesNoMessage === 'function') AskYesNoMessage("Sorry (rescue team members) doesn't match");
          return true;
        }
      } catch (_teamErr) {
        // Table/field may not exist in this deployment; fall through to SP which enforces its own rules.
      }
    }
    const ek = escapeSqlString(key);
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closeRscuPlan '${ek}'`, 'Error closing rescue plan');
  }

  if (btn === 'RSCUPLN_PLNCNCLD') {
    const key = getRescuePlanPrimaryKey(eventObj, devInterfaceObj, strScrTag);
    if (!key) {
      if (typeof AskYesNoMessage === 'function') AskYesNoMessage('You must save the Record first');
      return true;
    }
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'RSCUPLN-L1', key);
      setPendingRejectForModule('RESCUE_CANCEL', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }

  if (btn === 'RSCUPLNINQURY_RTRNPLNTOENTRY') {
    const key = getRescuePlanPrimaryKey(eventObj, devInterfaceObj, strScrTag);
    if (!key) {
      if (typeof AskYesNoMessage === 'function') AskYesNoMessage('You must save the Record first');
      return true;
    }
    if (!FormGetField) return true;
    const st = FormGetField(RESCUE_PLN_MAIN_TABLE, 'RSCUPLN_PLNSTTUS', 'scr');
    const n = parseInt(String(st ?? '').trim(), 10);
    if (!Number.isFinite(n) || n < 5) return true;
    const msg = 'Are you sure to return (rescue plan) to entry ?';
    const confirmed =
      typeof AskYesNoMessage === 'function' ? await AskYesNoMessage('Prompt', msg) : false;
    if (!confirmed) return true;
    const ek = escapeSqlString(key);
    await runTxnAndRefresh(
      devInterfaceObj,
      `EXECUTE rtrnRscuPlnToEntry '${ek}'`,
      'Error returning rescue plan to entry'
    );
    return true;
  }

  if (btn === 'RSCUPLN_TRCNG_VWCNCLRSN') {
    if (!FormGetField || typeof openScr !== 'function') return true;
    let ref = FormGetField('HSE_RSCUPLN_TRCNG', 'RSCUPLN_TRCNG_REFCLOSRSN', 'tab');
    if (ref == null || String(ref).trim() === '') {
      ref = FormGetField('HSE_RSCUPLN_TRCNG', 'RSCUPLN_TRCNG_REFCLOSRSN', 'scr');
    }
    const rs = ref != null ? String(ref).trim() : '';
    if (!rs) return true;
    const er = escapeSqlString(rs);
    const strCriteria = `SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '${er}')`;
    const defValObj = { RSNS_RFREC: rs };
    openScr('CMS_RSNS', {}, strCriteria, 'list', false, defValObj, false, true);
    return true;
  }

  return false;
}

// ----- EQ Inspection (EQInspctnCategory) — RQ_HSE_23_3_26_23_15 -----
async function handleEQInspectionButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  return handleEQInspectionModuleButtons(buttonName, strScrTag, eventObj, devInterfaceObj);
}

// ----- Vehicle Accident (UpdateStatus + InsertRecIntoTracingTab; reject uses reject reason then updateTXNSts 3) -----
const VCLACDNT_TABLE = 'HSE_VCLACDNTENT';
const VCLACDNT_KEY_FIELD = 'VCLACDNTENT_VCLACDNTNO';
const VCLACDNT_TABLE_PART = 'VCLACDNTENT';
const VCLACDNT_KEY_PART = 'VCLACDNTENT_VCLACDNTNO';

async function handleVehicleAccidentButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: VCLACDNT_TABLE, keyFieldName: VCLACDNT_KEY_FIELD, formTag: strScrTag });
  if (!key) return false;
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Vehicle Accident';

  if (buttonName === 'VCLACDNTENT_ENTCMPLT') {
    // Desktop: CompleteVehicleAccident — policy check then status 5+CONFIRM or 4+COMPLETE. Web: use 4+Completed (no policy check here).
    const sqls = buildUpdateStatusAndTracingSql(key, VCLACDNT_TABLE_PART, VCLACDNT_KEY_PART, '4', 'Completed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error completing vehicle accident');
  }
  if (buttonName === 'VCLACDNTENT_CNFRM' || buttonName === 'VCLACDNTENT_APRV') {
    const sqls = buildUpdateStatusAndTracingSql(key, VCLACDNT_TABLE_PART, VCLACDNT_KEY_PART, '5', 'Confirmed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error confirming vehicle accident');
  }
  if (buttonName === 'VCLACDNTENT_CLS') {
    const sqls = buildUpdateStatusAndTracingSql(key, VCLACDNT_TABLE_PART, VCLACDNT_KEY_PART, '6', 'Closed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error closing vehicle accident');
  }
  if (buttonName === 'VCLACDNTENT_RJC') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'VCLACDNTENT-L1', key);
      setPendingRejectForModule('VEHICLE_ACCIDENT', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  return false;
}

// ----- Waste (Receiving: Complete_Waste_Entry, Return_compltd_waste_to_entry) -----
async function handleWasteButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const scr = (strScrTag || '').toUpperCase();
  // Receiving: WSTRCVNGENTRY; key WstRcvngEntry_PrmKy / form HSE_WSTRCVNGENTRY
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: 'HSE_WSTRCVNGENTRY', keyFieldName: 'WstRcvngEntry_PrmKy', formTag: strScrTag });
  if (!key) return false;
  const ek = escapeSqlString(key);

  if (buttonName === 'WSTRCVNGENTRY_ENTRYCMPLTD') {
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE Complete_Waste_Entry '${ek}'`, 'Error completing waste receiving entry');
  }
  if (buttonName === 'WSTRCVNGENTRY_RTRNTOENTRY') {
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE Return_compltd_waste_to_entry '${ek}'`, 'Error returning waste to entry');
  }
  // Disposal: Complete_Waste_Dspsl_Entry, Return_compltd_waste_Dspsl_to_entry
  const keyDsp = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: 'HSE_WSTDSPSLENTRY', keyFieldName: 'WSTDSPSLENTRY_PRMKY', formTag: strScrTag });
  if (keyDsp && (buttonName === 'WSTDSPSLENTRY_ENTRYCMPLTD' || buttonName === 'WSTDSPSLENTRY_ENTRYCMPLT')) {
    const ed = escapeSqlString(keyDsp);
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE Complete_Waste_Dspsl_Entry '${ed}'`, 'Error completing waste disposal entry');
  }
  if (keyDsp && buttonName === 'WSTDSPSLENTRY_RTRNTOENTRY') {
    const ed = escapeSqlString(keyDsp);
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE Return_compltd_waste_Dspsl_to_entry '${ed}'`, 'Error returning waste disposal to entry');
  }
  return false;
}

// ----- Awareness (AwrnsPlnEntryCompleted; AwrnsPlnApproved; AwrnsPlnRejected; AwrnsPlnClosed) -----
// RQ_HSE_23_3_26_15_42
async function handleAwarenessButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: 'HSE_AWRNSPLN', keyFieldName: 'SerialPK', formTag: strScrTag });
  const sourceScreen = getScreenCaption(strScrTag) || 'Awareness Plan';

  if (!key) {
    if (buttonName === 'AWRNSPLNENTRY_PLNENTRYCMPLTD') {
      if (typeof devInterfaceObj?.AskYesNoMessage === 'function') devInterfaceObj.AskYesNoMessage('You must save the Record first');
      return true;
    }
    return true;
  }
  const ek = escapeSqlString(key);

  if (buttonName === 'AWRNSPLNENTRY_PLNENTRYCMPLTD') {
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE AwrnsPlnEntryCompleted ${ek}`, 'Error completing awareness plan entry');
  }
  if (buttonName === 'AWRNSPLNAPRVL_PLNENTRYAPRVD') {
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE AwrnsPlnApproved ${ek}`, 'Error approving awareness plan');
  }
  // RQ_HSE_23_3_26_15_42: reject-reason popup parity with desktop OpenReasonsScr → AwrnsPlnRejected
  if (buttonName === 'AWRNSPLNAPRVL_PLNENTRYRJCTD') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'AWRNSPLN-L1', key);
      setPendingRejectForModule('AWARENESS', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  // RQ_HSE_23_3_26_15_42: centralize close with other lifecycle buttons (desktop AwrnsPlnExctnCategory)
  if (buttonName === 'AWRNSPLNEXCTN_PLNCLSD') {
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE AwrnsPlnClosed ${ek}`, 'Error closing awareness plan');
  }
  return false;
}

// ----- Environment (Env Measures: completeEnvMsrEntry, rejectEnvMsr, approveEnvMsr; Env Monitoring: closeEnvMntrPlan) -----
async function handleEnvironmentButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: 'HSE_ENVMSRENTRY', keyFieldName: 'ENVMSRENTRY_PRMKY', formTag: strScrTag });
  const ek = escapeSqlString(key || '');

  if (buttonName === 'ENVMSRENTRY_ENTRYCMPLTD') {
    if (!key) return false;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE completeEnvMsrEntry '${ek}'`, 'Error completing environment measure entry');
  }
  if (buttonName === 'ENVMSRENTRY_REJECT') {
    if (!key) return false;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE rejectEnvMsr '${ek}'`, 'Error rejecting environment measure');
  }
  if (buttonName === 'ENVMSRENTRY_APPROVE') {
    if (!key) return false;
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE approveEnvMsr '${ek}'`, 'Error approving environment measure');
  }
  const keyPlan = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: 'HSE_ENVMNTRPLAN', keyFieldName: 'ENVMNTRPLAN_PRMKY', formTag: strScrTag });
  if (keyPlan && (buttonName === 'ENVMNTRPLAN_CLSPLAN' || buttonName === 'ENVMNTREXCTN_CLSPLAN')) {
    const ep = escapeSqlString(keyPlan);
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE closeEnvMntrPlan ${ep}`, 'Error closing environment monitoring plan');
  }
  return false;
}

// ----- Potential Hazard (UpdateStatus + InsertRecIntoTracingTab; reject via reject reason then updateTXNSts 3) -----
const PTNLHZRD_TABLE = 'HSE_PTNLHZRDENT';
const PTNLHZRD_KEY_FIELD = 'PTNLHZRDENT_PTNLHZRDNO';
const PTNLHZRD_TABLE_PART = 'PTNLHZRDENT';
const PTNLHZRD_KEY_PART = 'PTNLHZRDENT_PTNLHZRDNO';

async function handlePotentialHazardButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: PTNLHZRD_TABLE, keyFieldName: PTNLHZRD_KEY_FIELD, formTag: strScrTag });
  if (!key) return false;
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Potential Hazard';

  if (buttonName === 'PTNLHZRDENT_ENTCMPLT') {
    const sqls = buildUpdateStatusAndTracingSql(key, PTNLHZRD_TABLE_PART, PTNLHZRD_KEY_PART, '4', 'Completed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error completing potential hazard');
  }
  if (buttonName === 'PTNLHZRDENT_CNF') {
    const sqls = buildUpdateStatusAndTracingSql(key, PTNLHZRD_TABLE_PART, PTNLHZRD_KEY_PART, '5', 'Confirmed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error confirming potential hazard');
  }
  if (buttonName === 'PTNLHZRDENT_CLS') {
    const sqls = buildUpdateStatusAndTracingSql(key, PTNLHZRD_TABLE_PART, PTNLHZRD_KEY_PART, '6', 'Closed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error closing potential hazard');
  }
  if (buttonName === 'PTNLHZRDENT_RJC') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'PTNLHZRDENT-L1', key);
      setPendingRejectForModule('POTENTIAL_HAZARD', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  return false;
}

// ----- Loss Accident (UpdateStatus + InsertRecIntoTracingTab; reject via reject reason then updateTXNSts 3) -----
const LOSSACDNT_TABLE = 'HSE_LOSSACDNTENT';
const LOSSACDNT_KEY_FIELD = 'LOSSACDNTENT_ACTHZRDNO';
const LOSSACDNT_TABLE_PART = 'LOSSACDNTENT';
const LOSSACDNT_KEY_PART = 'LOSSACDNTENT_ACTHZRDNO';

async function handleLossAccidentButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const key = getKeyFromEvent(eventObj, devInterfaceObj, { tableName: LOSSACDNT_TABLE, keyFieldName: LOSSACDNT_KEY_FIELD, formTag: strScrTag });
  if (!key) return false;
  const userName = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  const sourceScreen = getScreenCaption(strScrTag) || 'Loss Accident';

  if (buttonName === 'LOSSACDNTENT_ENTCMPLT') {
    const sqls = buildUpdateStatusAndTracingSql(key, LOSSACDNT_TABLE_PART, LOSSACDNT_KEY_PART, '4', 'Completed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error completing loss accident');
  }
  if (buttonName === 'LOSSACDNTENT_CNFRMD') {
    const sqls = buildUpdateStatusAndTracingSql(key, LOSSACDNT_TABLE_PART, LOSSACDNT_KEY_PART, '5', 'Confirmed', sourceScreen, userName);
    return runMultipleSqlAndRefresh(devInterfaceObj, sqls, 'Error confirming loss accident');
  }
  if (buttonName === 'LOSSACDNTENT_RJCTD') {
    const openScr = devInterfaceObj?.openScr;
    if (typeof openScr === 'function') {
      openRejectReasonScreen(openScr, 'LOSSACDNTENT-L1', key);
      setPendingRejectForModule('LOSS_ACCIDENT', key, sourceScreen || strScrTag, devInterfaceObj);
    }
    return true;
  }
  return false;
}

/**
 * Main entry: try all module handlers. Returns true if any handler handled the button.
 * @param {string} buttonName - e.g. RSKASSMNTENT_ENTCMPLT
 * @param {string} strScrTag - e.g. HSE_TGRSKASSMNTENT
 * @param {Object} eventObj - ButtonClicked event
 * @param {Object} devInterfaceObj - executeSQLPromise, refreshData, getUserName, FormGetField, openScr, AskYesNoMessage
 * @returns {Promise<boolean>}
 */
export async function handleModuleButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  if (!buttonName || !strScrTag) return false;
  const btn = String(buttonName).toUpperCase();

  const handlers = [
    () => handleRiskAssessmentButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleSiteSurveyButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handlePTWButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleIncidentButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handlePerformanceMeasurementButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleAuditButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleRescuePlanButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleEQInspectionButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleVehicleAccidentButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleWasteButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleAwarenessButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleEnvironmentButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handlePotentialHazardButton(btn, strScrTag, eventObj, devInterfaceObj),
    () => handleLossAccidentButton(btn, strScrTag, eventObj, devInterfaceObj),
  ];

  for (const fn of handlers) {
    try {
      const handled = await fn();
      if (handled) return true;
    } catch (e) {
      console.warn('[Web_HSE] ModuleButtonHandlers handler error:', e);
    }
  }
  return false;
}
