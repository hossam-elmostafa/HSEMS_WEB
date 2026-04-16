/**
 * Shared custom button handlers for all CAR / Action Tracking screens.
 * C++: NewCarEntryCategory::DisplayCustomButtonClicked and related.
 * Screens: HSE_TGCRENTRY, HSE_TGCRRVW, HSE_TGCRAPRVL, HSE_TGACTNSENTRY, HSE_TGACTNSRVIW, HSE_TGCRFLOUP, HSE_TGCREDTNG
 */

import { getCurrentRecordPRMKY, openRejectReasonScreen } from './rejectReasonUtils.js';
import { getFieldSCR } from './formAccess.js';
// RQ_HSE_12_4_26_00_40 — workflow status guards (mitigates out-of-order CAR actions)
import { assertCrentryStatusAllows, fetchCrentryCrstt } from './carTransitionGuard.js';

const CAR_REVIEW_INFO_TAG = 'HSE_CrRvwInf';
const CAR_APPROVE_INFO_TAG = 'HSE_CrApprvlInf';
const REJECT_REASON_TAG = 'HSE_TGRJCTRSN';
const OBSERVATION_INQUIRY_TAG = 'HSE_TgNrstMiscInq';
const AUDIT_PLAN_INQUIRY_TAG = 'HSE_TgAdtPlnInq';

const CORRECTIVE_ACTIONS_BY_SCREEN = {
  'HSE_TGCRAPRVL': 'HSE_TGCRRCTVACTNS',
  'HSE_TGCREDTNG': 'HSE_TGCRRCTVACTNS',
  'HSE_TGACTNSRVIW': 'HSE_TgCrrctvActns_Rvw',
  'HSE_TGCRFLOUP': 'HSE_TgCrrctvActnsFlwUp',
  'HSE_TGACTNSENTRY': 'HSE_TgCrrctvActns_ActEnt'
};

const REJECT_SOURCE_BY_SCREEN = {
  'HSE_TGCRRVW': 'CRCTVEACCENT-RV',
  'HSE_TGCRAPRVL': 'CRCTVEACCENT-AP',
  'HSE_TGACTNSENTRY': 'CRCTVEACCENT-AE',
  'HSE_TGACTNSRVIW': 'ACTION-RV',
  'HSE_TGCRFLOUP': 'CARFLOWUP-RV'
};

const CRRCTVACTNS_LNKWTHMN = 'CRRCTVACTNS_LNKWTHMN';
/** RQ_HSE_12_4_26_00_40 — GAP-12: defValObj key; shell applies to popup so FormGetField reads CAR date in-tab. */
const CORRECTIVE_POPUP_SNAPSHOT_CAR_DATE_FIELD = 'CRENTRY_CRDT';
const CrRvwInf_LnkWthMn = 'CrRvwInf_LnkWthMn';
const CrRvwInf_MdlTyp = 'CrRvwInf_MdlTyp';
const CrApprvlInf_LnkWthMn = 'CrApprvlInf_LnkWthMn';
const CrApprvlInf_MdlTyp = 'CrApprvlInf_MdlTyp';
const SOURCE_APPROVE = 'CRCTVEACCENT-AP';

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

function openCarReviewInfo(openScr, strPrimKey, strSourceModule = '') {
  const esc = escapeSqlString(strPrimKey);
  const criteria = strSourceModule
    ? `SELECT * FROM ${CAR_REVIEW_INFO_TAG} WHERE (${CrRvwInf_LnkWthMn}= '${esc}') AND (${CrRvwInf_MdlTyp}= '${escapeSqlString(strSourceModule)}')`
    : `SELECT * FROM ${CAR_REVIEW_INFO_TAG} WHERE (${CrRvwInf_LnkWthMn}= '${esc}')`;
  const defValObj = { [CrRvwInf_LnkWthMn]: strPrimKey, [CrRvwInf_MdlTyp]: strSourceModule };
  openScr(CAR_REVIEW_INFO_TAG, {}, criteria, 'edit', false, defValObj, false, true);
}

/** Main form tag for reading CRENTRY_CRSRC (web shell tags differ from C++ HSE_TGCRENTRY literal). */
const FORM_TAG_CRENTRY_CRSRC_BY_SCR = {
  HSE_TGCRRVW: 'HSE_TGCRRVW',
  HSE_TGCRENTRY: 'HSE_TgCrEntry',
};

/**
 * C++ NewCarEntryCategory CAR_REVIEW_INFO: policy then acceptRecord -> HSE_CrRvwInf with CRCTVEACCENT-RV.
 * HSE_TGCRENTRY: CAR Entry APP JSON button CAR_Review_Info.
 */
async function tryOpenCarReviewInfoWithPolicy(eventObj, devInterfaceObj, scr) {
  const formTag = FORM_TAG_CRENTRY_CRSRC_BY_SCR[scr];
  const openScr = devInterfaceObj?.openScr;
  const { executeSQLPromise, AskYesNoMessage } = devInterfaceObj || {};
  const strPrimKey = getCurrentRecordPRMKY(eventObj);
  if (!formTag || typeof openScr !== 'function' || !strPrimKey || !executeSQLPromise) {
    return false;
  }

  const policyMsg =
    devInterfaceObj.getCurrentLang?.() === 'ar'
      ? 'وفقًا لسياسة النظام الحالية، هذه الوظيفة غير مسموح بها'
      : 'According to current System Policy. This function is not allowed';

  try {
    const strCARSource = getFieldSCR(devInterfaceObj, formTag, 'CRENTRY_CRSRC') || '';
    const policyRs = await executeSQLPromise(`SELECT HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRRVWINF FROM HSE_HSEPLC_ADT`);
    const policyRow = policyRs?.recordset?.[0] ?? policyRs?.[0];
    const strCARBasis = (policyRow?.HSEPLC_ADT_CRBSS ?? policyRow?.hseplc_adt_crbss ?? '').toString().trim();
    const strEnableRvwInf = (policyRow?.HSEPLC_ADT_ENBCRRVWINF ?? policyRow?.hseplc_adt_enbcrrvwinf ?? 'N').toString().trim();
    const allowed = strCARSource === strCARBasis && strEnableRvwInf === 'Y';
    if (!allowed) {
      if (AskYesNoMessage) AskYesNoMessage(policyMsg);
      return true;
    }
    openCarReviewInfo(openScr, strPrimKey, 'CRCTVEACCENT-RV');
    return true;
  } catch (e) {
    console.warn('[Web_HSE] CAR_REVIEW_INFO policy:', e);
    if (AskYesNoMessage) AskYesNoMessage(policyMsg);
    return true;
  }
}

function openCarApproveInfo(openScr, strPrimKey) {
  const esc = escapeSqlString(strPrimKey);
  const criteria = `SELECT * FROM ${CAR_APPROVE_INFO_TAG} WHERE (${CrApprvlInf_LnkWthMn}= '${esc}') AND (${CrApprvlInf_MdlTyp}= '${SOURCE_APPROVE}')`;
  const defValObj = { [CrApprvlInf_LnkWthMn]: strPrimKey, [CrApprvlInf_MdlTyp]: SOURCE_APPROVE };
  openScr(CAR_APPROVE_INFO_TAG, {}, criteria, 'edit', false, defValObj, false, true);
}

/**
 * C++ NewCarEntryCategory CAR_APPROVAL_INFO: policy gate then open HSE_CrApprvlInf (Bugs-2-2024.5).
 * RQ_HSE_12_4_26_00_40 — gap R8 / A12: mirror CAR_REVIEW_INFO policy behavior.
 */
async function tryOpenCarApproveInfoWithPolicy(eventObj, devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const { executeSQLPromise, AskYesNoMessage } = devInterfaceObj || {};
  const strPrimKey = getCurrentRecordPRMKY(eventObj);
  if (typeof openScr !== 'function' || !strPrimKey || !executeSQLPromise) {
    return false;
  }

  const policyMsg =
    devInterfaceObj.getCurrentLang?.() === 'ar'
      ? 'وفقًا لسياسة النظام الحالية، هذه الوظيفة غير مسموح بها'
      : 'According to current System Policy. This function is not allowed';

  try {
    const strCARSource = getFieldSCR(devInterfaceObj, 'HSE_TGCRAPRVL', 'CRENTRY_CRSRC') || '';
    const policyRs = await executeSQLPromise(
      `SELECT HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRAPPINF FROM HSE_HSEPLC_ADT`
    );
    const policyRow = policyRs?.recordset?.[0] ?? policyRs?.[0];
    const strCARBasis = (policyRow?.HSEPLC_ADT_CRBSS ?? policyRow?.hseplc_adt_crbss ?? '').toString().trim();
    const strEnableAppInf = (policyRow?.HSEPLC_ADT_ENBCRAPPINF ?? policyRow?.hseplc_adt_enbcrappinf ?? 'N')
      .toString()
      .trim();
    const allowed = strCARSource === strCARBasis && strEnableAppInf === 'Y';
    if (!allowed) {
      if (AskYesNoMessage) AskYesNoMessage(policyMsg);
      return true;
    }
    openCarApproveInfo(openScr, strPrimKey);
    return true;
  } catch (e) {
    console.warn('[Web_HSE] CAR_APPROVAL_INFO policy RQ_HSE_12_4_26_00_40:', e);
    if (AskYesNoMessage) AskYesNoMessage(policyMsg);
    return true;
  }
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-12: Read CAR open date from parent context while still on the opener screen
 * (desktop reads parent CCorrectiveActionEntry host; web shell may not expose HSE_CRENTRY from popup).
 */
function resolveParentCarentryCrDateForPopup(devInterfaceObj, eventObj, parentScrTag) {
  const FormGetField = devInterfaceObj?.FormGetField;
  const field = CORRECTIVE_POPUP_SNAPSHOT_CAR_DATE_FIELD;
  const tryGet = (tag, src) => {
    if (typeof FormGetField !== 'function' || !tag) return '';
    try {
      const v = FormGetField(tag, field, src);
      return v != null && String(v).trim() !== '' ? String(v).trim() : '';
    } catch (_) {
      return '';
    }
  };
  let d =
    tryGet(parentScrTag, 'SCR') ||
    tryGet('HSE_CRENTRY', 'SCR') ||
    tryGet('HSE_TgCrEntry', 'SCR');
  if (!d && eventObj) {
    d = (getFieldFromRecord(eventObj, field) || '').trim();
  }
  return d;
}

function openCorrectiveActionsPopup(openScr, strScrTag, strKeyField, devInterfaceObj, eventObj) {
  const popupTag = CORRECTIVE_ACTIONS_BY_SCREEN[strScrTag];
  if (!popupTag || !strKeyField) return;
  const esc = escapeSqlString(strKeyField);
  const criteria = `SELECT * FROM HSE_CRRCTVACTNS WHERE (${CRRCTVACTNS_LNKWTHMN}= '${esc}')`;
  const defValObj = { [CRRCTVACTNS_LNKWTHMN]: strKeyField };
  // RQ_HSE_12_4_26_00_40 — GAP-12: pass snapshot so RVSTRGDT validation reads from popup defVal context
  const carDt = resolveParentCarentryCrDateForPopup(devInterfaceObj, eventObj, strScrTag);
  if (carDt) defValObj[CORRECTIVE_POPUP_SNAPSHOT_CAR_DATE_FIELD] = carDt;
  openScr(popupTag, {}, criteria, 'edit', false, defValObj, false, false);
}

/**
 * VIEW_SOURCE_TXN — C++ NewCarEntryCategory; extended RQ_HSE_30_3_26_14_57 to mirror CarCategory::OpenTXNInquiryScreen
 * (CAR basis codes → inquiry tags / key fields) using CRENTRY_CRSRC text + CRENTRY_TXNN / TXNYR.
 */
function handleViewSourceTxn(eventObj, devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return;
  const strAutoGenerated = (getFieldFromRecord(eventObj, 'CRENTRY_ATGNR') || '').toUpperCase();
  if (strAutoGenerated !== 'Y') {
    const msg = devInterfaceObj.getCurrentLang?.() === 'ar' ? 'القيمة التلقائية ليست نعم' : "Auto generated is set to 'No'";
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage(msg);
    return;
  }
  const txnYr = getFieldFromRecord(eventObj, 'CRENTRY_TXNYR');
  const txnNo = getFieldFromRecord(eventObj, 'CRENTRY_TXNN');
  if (!String(txnNo || '').trim()) {
    const msg = devInterfaceObj.getCurrentLang?.() === 'ar' ? 'رقم المعاملة المصدر مفقود' : 'Source transaction number is missing';
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage(msg);
    return;
  }
  const escNo = escapeSqlString(txnNo);
  const escYr = escapeSqlString(txnYr);
  const strCARSrc = (getFieldFromRecord(eventObj, 'CRENTRY_CRSRC') || '').toUpperCase().replace(/\s+/g, ' ').trim();

  const PERF_INQUIRY_TAG = 'HSE_TgperfMsrmntInq';
  const SITE_SURVEY_INQUIRY_TAG = 'HSE_TgSitSrvyInq';
  const RISK_INQUIRY_TAG = 'HSE_TgrskassmntInq';
  const DRILL_INQUIRY_TAG = 'HSE_DRLLPLNINQ';
  const PTNL_INQUIRY_TAG = 'HSE_TgptnlhzrdInq';
  const VEH_INQUIRY_TAG = 'HSE_TgVclAcdntInqtn';
  const LOSS_INQUIRY_TAG = 'HSE_TgLossAcdntInq';

  const openList = (tag, criteria) => {
    openScr(tag, {}, criteria, 'list', false, {}, false, false);
  };

  if (strCARSrc === 'OBSERVATION' || strCARSrc === 'NEAR MISS') {
    openList(OBSERVATION_INQUIRY_TAG, `(NrstMiscEnt_Yr = '${escYr}') AND (NrstMiscEnt_NrstMiscNum = '${escNo}')`);
    return;
  }
  if (strCARSrc === 'INTERNAL AUDIT' || strCARSrc === 'AUDIT') {
    openList(AUDIT_PLAN_INQUIRY_TAG, `(ADTPLNENT_ADTYR = '${escYr}') AND (ADTPLNENT_ADTNUM = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('PERFORMANCE')) {
    openList(PERF_INQUIRY_TAG, `(PERFMSRMNTENT_VSTNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('SITE') && strCARSrc.includes('SURVEY')) {
    openList(SITE_SURVEY_INQUIRY_TAG, `(SITSRVYENT_SITSRVYNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('RISK')) {
    openList(RISK_INQUIRY_TAG, `(RSKASSMNTENT_ASSMNTNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('DRILL')) {
    openList(DRILL_INQUIRY_TAG, `(DRLLPLN_PRMRYKY = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('POTENTIAL') || strCARSrc.includes('HAZARD')) {
    openList(PTNL_INQUIRY_TAG, `(PTNLHZRDENT_PTNLHZRDNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('VEHICLE')) {
    openList(VEH_INQUIRY_TAG, `(VCLACDNTENT_VCLACDNTNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('LOSS')) {
    openList(LOSS_INQUIRY_TAG, `(LOSSACDNTENT_ACTHZRDNO = '${escNo}')`);
    return;
  }
  if (strCARSrc.includes('INCIDENT') || strCARSrc === 'ACCIDENT') {
    openList('HSE_TgAcdntInq', `(ACDNTENT_ACDNTNUM = '${escNo}')`);
    return;
  }

  const msg = devInterfaceObj.getCurrentLang?.() === 'ar' ? 'مصدر CAR غير مدعوم' : 'CAR source not supported for View Source TXN';
  if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage(msg);
}

/** View Reject Reason (locked) for CAR Entry - open with criteria CRCTVEACCENT% */
function handleViewRejectReasons(eventObj, devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return;
  const strPrimKey = getCurrentRecordPRMKY(eventObj);
  if (!strPrimKey) return;
  const esc = escapeSqlString(strPrimKey);
  const strCriteria = `SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN= '${esc}') AND (RJCTRSN_MODULETYPE like 'CRCTVEACCENT%')`;
  openScr(REJECT_REASON_TAG, {}, strCriteria, 'edit', false, {}, false, true);
}

/** ENTRY_COMPLETED for CAR Entry (HSE_TGCRENTRY) - tracing via ChangeEntityStatus, set status 05, save */
async function handleEntryCompletedCarEntry(eventObj, devInterfaceObj) {
  const { executeSQLPromise, FormSetField, doToolbarAction, refreshData, getUserName, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise || !FormSetField) return;
  const strPrmKy = getCurrentRecordPRMKY(eventObj);
  if (!strPrmKy) return;
  // RQ_HSE_12_4_26_00_40 — GAP-8: submit from draft 01 OR re-submit after review rejection 02
  const ok = await assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['01', '02'], AskYesNoMessage);
  if (!ok) return;
  const strUserName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  try {
    // RQ_HSE_12_4_26_00_40 — align with desktop InsertIntoTracingTabProcess → ChangeEntityStatus;
    // BEFORE: raw INSERT INTO HSE_CRENTRY_TRC (raw insert missed SP-side ERP audit logic)
    // AFTER:  ChangeEntityStatus (same SP called by C++ InsertIntoTracingTabProcess)
    await executeChangeEntityStatus(executeSQLPromise, strPrmKy, 'CAR Entry', strUserName, 'Entry Completed');
    FormSetField('HSE_TgCrEntry', 'CRENTRY_CRSTT', '05', 'scr');
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', 'HSE_TgCrEntry', '');
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] CAR Entry ENTRY_COMPLETED:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error applying Entry Completed');
  }
}

/** ENTRY_COMPLETED for Actions Entry - validations + tracing + status 16 + updates + save */
async function handleEntryCompletedActionsEntry(eventObj, devInterfaceObj) {
  const { executeSQLPromise, FormSetField, doToolbarAction, refreshData, getUserName, getValFromRecordSet, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise || !FormSetField || !doToolbarAction || !refreshData) return;
  const strPrmKy = getCurrentRecordPRMKY(eventObj);
  if (!strPrmKy) return;
  // RQ_HSE_12_4_26_00_40 — GAP-6/9/10: accept after review (10), re-submit after actions-entry reject (11), actions-review reject (12), or approval reject (06)
  const okSt = await assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['10', '11', '12', '06'], AskYesNoMessage);
  if (!okSt) return;
  const getCount = async (sql) => {
    const r = await executeSQLPromise(sql);
    const row = r?.recordset?.[0] ?? r?.[0];
    const c = row && (getValFromRecordSet ? getValFromRecordSet(r, 'C') : (row.C ?? row.c));
    return parseInt(c, 10) || 0;
  };
  try {
    if ((await getCount(`SELECT COUNT(*) AS C FROM HSE_ACTNSENTRY_RtCss WHERE prmky=${strPrmKy}`)) === 0) {
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Must enter root causes');
      return;
    }
    if ((await getCount(`SELECT COUNT(*) AS C FROM HSE_ACTNSENTRY_RtCss WHERE AutoFieldNum NOT IN (SELECT isnull(CrrctvActns_Lnkwthmn,0) FROM HSE_CrrctvActns) AND prmky=${strPrmKy}`)) !== 0) {
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Each root cause must at least have 1 action');
      return;
    }
    if ((await getCount(`SELECT COUNT(*) AS C FROM HSE_ACTNSENTRY_CRR WHERE prmky=${strPrmKy}`)) === 0) {
      if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('User MUST enter at least (1) record in Corrections tab');
      return;
    }
  } catch (e) {
    console.warn('[Web_HSE] ENTRY_COMPLETED validation:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Validation error');
    return;
  }
  const strUserName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  try {
    // RQ_HSE_12_4_26_00_40 — align with desktop InsertIntoTracingTabProcess → ChangeEntityStatus;
    // BEFORE: raw INSERT INTO HSE_CRENTRY_TRC (skipped SP-side ERP audit logic)
    // AFTER:  ChangeEntityStatus (same SP called by C++ InsertIntoTracingTabProcess)
    await executeChangeEntityStatus(executeSQLPromise, strPrmKy, 'Actions Entry', strUserName, 'Entry Completed');
    FormSetField('HSE_TgActnsEntry', 'CRENTRY_CRSTT', '16', 'scr');
    await executeSQLPromise(`update HSE_ACTNSENTRY_CRR set CRRCTVACTNS_LnkCss_Acc='Y',ACTNSENTRY_Crr_ActStt='A' where PrmKy=${strPrmKy}`);
    await executeSQLPromise(`update HSE_CRRCTVACTNS set CRRCTVACTNS_Acc='Y', CRRCTVACTNS_ACTSTT=3 where CrrctvActns_LnkWthMn in (select AutoFieldNum from HSE_ACTNSENTRY_RTCSS where PrmKy=${strPrmKy})`);
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', 'HSE_TgActnsEntry', '');
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] ENTRY_COMPLETED apply:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error applying Entry Completed');
  }
}

/** RQ_HSE_12_4_26_00_40 — shell may pass uppercase HSE_TG*; save APIs expect mixed-case tags (e.g. HSE_TgCrFloUp). */
function normalizeCarCloseFormTag(formTag) {
  const u = String(formTag || '').toUpperCase();
  if (u === 'HSE_TGACTNSRVIW') return 'HSE_TgActnsRviw';
  if (u === 'HSE_TGCRFLOUP') return 'HSE_TgCrFloUp';
  if (u === 'HSE_TGCREDTNG') return 'HSE_TgCrEdtng';
  return formTag || 'HSE_TgActnsRviw';
}

/** CLOSE_CAR: validate then set CRENTRY_CRSTT=25, save, refresh. formTag = current screen tag. */
async function handleCloseCar(eventObj, devInterfaceObj, formTag) {
  const { executeSQLPromise, FormSetField, doToolbarAction, refreshData } = devInterfaceObj || {};
  if (!executeSQLPromise || !FormSetField) return;
  const strPrmKy = getCurrentRecordPRMKY(eventObj);
  if (!strPrmKy) return;
  // RQ_HSE_12_4_26_00_40 — C++ CLOSE_CAR does not gate on CRENTRY_CRSTT; only child validations + close date
  const getCount = async (sql) => {
    try {
      const r = await executeSQLPromise(sql);
      return r?.recordsAffected ?? r?.rowsAffected ?? 0;
    } catch (_) { return -1; }
  };
  // RQ_HSE_12_4_26_00_40 — case-insensitive accepted flags (collation-safe)
  const unacceptedCorr = await getCount(
    `update HSE_ACTNSENTRY_CRR set prmky=prmky where prmky=${strPrmKy} AND UPPER(LTRIM(RTRIM(ISNULL(ACTNSENTRY_CRR_ACTACC,N'')))) <> N'Y'`
  );
  if (unacceptedCorr > 0) {
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Some "actions" are not accepted, please check the Corrections TAB');
    return;
  }
  const unacceptedAct = await getCount(
    `update HSE_CRRCTVACTNS set CrrctvActns_LnkWthMn=CrrctvActns_LnkWthMn where CrrctvActns_LnkWthMn in (select AutoFieldNum from HSE_ACTNSENTRY_RTCSS where PrmKy=${strPrmKy}) and UPPER(LTRIM(RTRIM(ISNULL(CRRCTVACTNS_ACTACC,N'')))) <> N'Y'`
  );
  if (unacceptedAct > 0) {
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Some "actions" are not accepted, please check the Corrective Actions');
    return;
  }
  const closeDate = getFieldFromRecord(eventObj, 'CRENTRY_CRCLSDT');
  if (!closeDate) {
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Please enter CAR closed date');
    return;
  }
  const tag = normalizeCarCloseFormTag(formTag);
  // RQ_HSE_12_4_26_00_40 — gap R12 / A3: record lifecycle trace via ChangeEntityStatus before persisting closed status (desktop may rely on same SP elsewhere).
  const closeSourceScreenByTag = {
    HSE_TGCRFLOUP: 'CAR Follow Up',
    HSE_TGACTNSRVIW: 'Actions Review',
    HSE_TGCREDTNG: 'CAR Editing',
  };
  const scrU = String(formTag || '').toUpperCase();
  const closeSourceScreen =
    closeSourceScreenByTag[scrU] ||
    (tag === 'HSE_TgCrFloUp' ? 'CAR Follow Up' : tag === 'HSE_TgActnsRviw' ? 'Actions Review' : 'CAR Editing');
  const strUserClose = (typeof devInterfaceObj?.getUserName === 'function' ? devInterfaceObj.getUserName() : '') || '';
  try {
    await executeChangeEntityStatus(executeSQLPromise, strPrmKy, closeSourceScreen, strUserClose, 'CAR Closed');
  } catch (traceErr) {
    console.warn('[Web_HSE] CLOSE_CAR ChangeEntityStatus RQ_HSE_12_4_26_00_40:', traceErr);
    if (devInterfaceObj.AskYesNoMessage) {
      devInterfaceObj.AskYesNoMessage(
        traceErr?.message ||
          'RQ_HSE_12_4_26_00_40: Could not write CAR close trace. Refresh and retry, or contact support.'
      );
    }
    return;
  }
  try {
    FormSetField(tag, 'CRENTRY_CRSTT', '25', 'scr');
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', tag, '');
    if (typeof refreshData === 'function') refreshData('');
  } catch (e) {
    console.warn('[Web_HSE] CLOSE_CAR:', e);
  }
}

// RQ_HSE_13_3_26_1_43: Pending CAR reject/cancel context (mirrors observation pendingRejectObservation mechanism)
let pendingRejectCAR = null;

export function setPendingRejectCAR(info) {
  pendingRejectCAR = info ? { ...info } : null;
}

export function clearPendingRejectCAR() {
  pendingRejectCAR = null;
}

export function getPendingRejectCAR() {
  return pendingRejectCAR;
}

// RQ_HSE_13_3_26_1_43: C++ InsertIntoTracingTabProcess → EXECUTE ChangeEntityStatus
// RQ_HSE_12_4_26_00_40 — M2b ASSUMPTION (verify on live DB): proc args are
//   1 PrmKey, 2 SourceScreen, 3 UserName, 4 table prefix 'HSE_CRENTRY_', 5 status column 'CRENTRY_CRSTT',
//   6 key column 'PRMKEY', 7 status token (prior / current CRENTRY_CRSTT from DB for tracing), 8 action description text.
// RQ_HSE_12_4_26_00_40 — gap R2: do NOT default token to '02' when DB read fails (wrong audit trail).
async function executeChangeEntityStatus(executeSQLAsync, prmky, sourceScreenName, userName, actionDescription) {
  let fromStt = await fetchCrentryCrstt(executeSQLAsync, prmky);
  if (!fromStt || !String(fromStt).trim()) {
    const err = new Error(
      'RQ_HSE_12_4_26_00_40: Unable to read current CRENTRY_CRSTT for tracing. Refresh the screen and retry.'
    );
    err.code = 'CAR_CRSTT_READ_FAIL';
    throw err;
  }
  const token = escapeSqlString(String(fromStt).trim());
  const sql = `EXECUTE ChangeEntityStatus '${escapeSqlString(prmky)}','${escapeSqlString(sourceScreenName)}','${escapeSqlString(userName)}','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','${token}','${escapeSqlString(actionDescription)}'`;
  await executeSQLAsync(sql);
}

// RQ_HSE_13_3_26_1_43: C++ AcceptCARProcess for HSE_TGCRRVW – policy check, tracing, status 10, save
async function handleAcceptCarReview(eventObj, devInterfaceObj) {
  const { executeSQLPromise, FormSetField, doToolbarAction, refreshData, getUserName, getValFromRecordSet, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise || !FormSetField) return;
  const strPrmKy = getCurrentRecordPRMKY(eventObj);
  if (!strPrmKy) return;
  // RQ_HSE_12_4_26_00_40 — accept review from submitted (05)
  const okRv = await assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['05'], AskYesNoMessage);
  if (!okRv) return;

  let bUpdateAcceptedRecord = true;

  // C++: Bugs-2-2024.4 – check if CAR Review Info is enabled in policy for this CAR source
  try {
    const strCARSource = getFieldSCR(devInterfaceObj, 'HSE_TGCRRVW', 'CRENTRY_CRSRC') || '';
    const policyRs = await executeSQLPromise(`SELECT HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRRVWINF FROM HSE_HSEPLC_ADT`);
    const policyRow = policyRs?.recordset?.[0] ?? policyRs?.[0];
    if (policyRow) {
      const strCARBasis = (policyRow.HSEPLC_ADT_CRBSS ?? policyRow.hseplc_adt_crbss ?? '').toString().trim();
      const strEnableRvwInf = (policyRow.HSEPLC_ADT_ENBCRRVWINF ?? policyRow.hseplc_adt_enbcrrvwinf ?? 'N').toString().trim();
      if (strCARSource === strCARBasis && strEnableRvwInf === 'Y') {
        const chkRs = await executeSQLPromise(`UPDATE HSE_CRRVWINF SET CrRvwInf_LnkWthMn=CrRvwInf_LnkWthMn WHERE CrRvwInf_LnkWthMn=${strPrmKy}`);
        const rowsAffected = chkRs?.recordsAffected ?? chkRs?.rowsAffected?.[0] ?? chkRs?.rowsAffected ?? 0;
        bUpdateAcceptedRecord = (rowsAffected === 1);
      }
    }
  } catch (e) {
    console.warn('[Web_HSE] AcceptCARProcess policy check:', e);
  }

  if (!bUpdateAcceptedRecord) {
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Please insert a CAR Review Information');
    return;
  }

  const strUserName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  try {
    await executeChangeEntityStatus(executeSQLPromise, strPrmKy, 'CAR Review', strUserName, 'CAR Accepted');
    FormSetField('HSE_TGCRRVW', 'CRENTRY_CRSTT', '10', 'scr');
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', 'HSE_TGCRRVW', '');
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.warn('[Web_HSE] AcceptCARProcess execute:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error accepting CAR');
  }
}

// RQ_HSE_13_3_26_1_43: C++ ApproveCARProcess2 for HSE_TGCRAPRVL – policy check, tracing, status 15, copy dept/project, save
async function handleApproveCarApproval(eventObj, devInterfaceObj) {
  const { executeSQLPromise, FormSetField, doToolbarAction, refreshData, getUserName, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise || !FormSetField) return;
  const strPrmKy = getCurrentRecordPRMKY(eventObj);
  if (!strPrmKy) return;
  // RQ_HSE_12_4_26_00_40 — approve from execution readiness (19)
  const okAp = await assertCrentryStatusAllows(executeSQLPromise, strPrmKy, ['19'], AskYesNoMessage);
  if (!okAp) return;

  let bUpdateApprovedRecord = true;

  // C++: Bugs-2-2024.5 – check if CAR Approval Info is enabled in policy
  try {
    const strCARSource = getFieldSCR(devInterfaceObj, 'HSE_TGCRAPRVL', 'CRENTRY_CRSRC') || '';
    const policyRs = await executeSQLPromise(`SELECT HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRAPPINF FROM HSE_HSEPLC_ADT`);
    const policyRow = policyRs?.recordset?.[0] ?? policyRs?.[0];
    if (policyRow) {
      const strCARBasis = (policyRow.HSEPLC_ADT_CRBSS ?? policyRow.hseplc_adt_crbss ?? '').toString().trim();
      const strEnableAppInf = (policyRow.HSEPLC_ADT_ENBCRAPPINF ?? policyRow.hseplc_adt_enbcrappinf ?? 'N').toString().trim();
      if (strCARSource === strCARBasis && strEnableAppInf === 'Y') {
        const chkRs = await executeSQLPromise(`UPDATE HSE_CRAPPRVLINF SET CRAPPRVLINF_LnkWthMn=CRAPPRVLINF_LnkWthMn WHERE CRAPPRVLINF_LnkWthMn=${strPrmKy}`);
        const rowsAffected = chkRs?.recordsAffected ?? chkRs?.rowsAffected?.[0] ?? chkRs?.rowsAffected ?? 0;
        bUpdateApprovedRecord = (rowsAffected === 1);
      }
    }
  } catch (e) {
    console.warn('[Web_HSE] ApproveCARProcess2 policy check:', e);
  }

  if (!bUpdateApprovedRecord) {
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Please insert a CAR Approval Information');
    return;
  }

  const strUserName = (typeof getUserName === 'function' ? getUserName() : '') || '';
  try {
    await executeChangeEntityStatus(executeSQLPromise, strPrmKy, 'CAR Approval', strUserName, 'CAR Approved');
    FormSetField('HSE_TGCRAPRVL', 'CRENTRY_CRSTT', '15', 'scr');

    // C++: copy Concerned Department and Project from Approval Info to main screen
    try {
      // RQ_HSE_12_4_26_00_40 — GAP-22: use ISNULL with both possible column name casings for robustness
      const infoRs = await executeSQLPromise(`SELECT ISNULL(CrApprvlInf_CncrnDprt, CRAPPRVLINF_CNCDPR) AS DEPT, ISNULL(CrApprvlInf_Prjct, CRAPPRVLINF_RLTPRJ) AS PRJ FROM HSE_CRAPPRVLINF WHERE CRAPPRVLINF_LnkWthMn=${strPrmKy}`);
      const infoRow = infoRs?.recordset?.[0] ?? infoRs?.[0];
      if (infoRow) {
        const dept = (infoRow.DEPT ?? infoRow.dept ?? infoRow.CrApprvlInf_CncrnDprt ?? infoRow.CRAPPRVLINF_CNCDPR ?? '').toString().trim();
        const prj = (infoRow.PRJ ?? infoRow.prj ?? infoRow.CrApprvlInf_Prjct ?? infoRow.CRAPPRVLINF_RLTPRJ ?? '').toString().trim();
        if (dept) FormSetField('HSE_TGCRAPRVL', 'CRENTRY_CNCDPR', dept, 'scr');
        if (prj) FormSetField('HSE_TGCRAPRVL', 'CRENTRY_PRJ', prj, 'scr');
      }
    } catch (cpyErr) {
      console.warn('[Web_HSE] ApproveCARProcess2 copy dept/project:', cpyErr);
    }

    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', 'HSE_TGCRAPRVL', '');
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.warn('[Web_HSE] ApproveCARProcess2 execute:', e);
    if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Error approving CAR');
  }
}

// RQ_HSE_13_3_26_1_43: C++ RejectCARProcess – after reject reason entered, ChangeEntityStatus + status change + save
// RQ_HSE_12_4_26_00_40: HSE_TGCRFLOUP → 15 matches C++ RejectCARProcess / gap report (return to “Approved” for follow-up rework — confirm with business if ambiguous).
const CAR_REJECT_STATUS_BY_SCREEN = {
  'HSE_TGCRRVW': '02',
  'HSE_TGCRAPRVL': '06',
  'HSE_TGACTNSENTRY': '11',
  'HSE_TGACTNSRVIW': '12',
  'HSE_TGCRFLOUP': '15',
};

const CAR_REJECT_ACTION_BY_SCREEN = {
  'HSE_TGCRRVW': 'Entry Rejected',
  'HSE_TGCRAPRVL': 'Entry Rejected',
  'HSE_TGACTNSENTRY': 'Entry Rejected',
  'HSE_TGACTNSRVIW': 'CAR Rejected',
  'HSE_TGCRFLOUP': 'CAR Rejected',
};

const CAR_SCREEN_CAPTION = {
  'HSE_TGCRRVW': 'CAR Review',
  'HSE_TGCRAPRVL': 'CAR Approval',
  'HSE_TGACTNSENTRY': 'Actions Entry',
  'HSE_TGACTNSRVIW': 'Actions Review',
  'HSE_TGCRFLOUP': 'CAR Follow Up',
  'HSE_TGCRENTRY': 'CAR Entry',
};

/**
 * RQ_HSE_13_3_26_1_43: Execute pending CAR reject/cancel action after user enters reject reason and clicks OK.
 * Called from ObservationService.js when RJCTRSN_BTN_OK fires for a CAR module type.
 */
export async function handleRejectReasonOkForCAR(devInterface, _eventObj) {
  if (!pendingRejectCAR) {
    console.warn('[Web_HSE] No pending CAR rejection info found.');
    return false;
  }

  const { screenTag, prmky, action, newStatus, actionDescription, sourceScreenName } = pendingRejectCAR;
  const executeSQLAsync = devInterface?.executeSQLPromise || devInterface?.executeSQL;
  const getUserName = devInterface?.getUserName || pendingRejectCAR.getUserName;
  const FormSetField = devInterface?.FormSetField || pendingRejectCAR.FormSetField;
  const doToolbarAction = devInterface?.doToolbarAction || pendingRejectCAR.doToolbarAction;
  const refreshData = devInterface?.refreshData || pendingRejectCAR.refreshData;

  if (!executeSQLAsync || !prmky) {
    clearPendingRejectCAR();
    return false;
  }

  const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';

  try {
    await executeChangeEntityStatus(executeSQLAsync, prmky, sourceScreenName, userName, actionDescription);

    if (typeof FormSetField === 'function') {
      FormSetField(screenTag, 'CRENTRY_CRSTT', newStatus, 'scr');
    }
    if (typeof doToolbarAction === 'function') {
      doToolbarAction('SAVE', screenTag, '');
    }
    if (typeof refreshData === 'function') {
      refreshData('', 'REFRESH_SELECTED');
    }
  } catch (e) {
    console.error('[Web_HSE] CAR reject/cancel execute error:', e);
  }

  clearPendingRejectCAR();
  return true;
}

/**
 * Main entry: handle CAR custom button. Returns true if the button was handled for this screen.
 * @param {string} buttonName - e.g. VIEW_CAR_REVIEW_INFO
 * @param {string} strScrTag - e.g. HSE_TGACTNSENTRY (uppercase)
 * @param {Object} eventObj - ButtonClicked event
 * @param {Object} devInterfaceObj - dev interface
 * @returns {Promise<boolean>}
 */
export async function handleCarButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = (buttonName && String(buttonName).toUpperCase()) || '';
  const scr = (strScrTag && String(strScrTag).toUpperCase()) || '';

  const openScr = devInterfaceObj?.openScr;
  const strPrimKey = getCurrentRecordPRMKY(eventObj);
  const strKeyField = getFieldFromRecord(eventObj, 'AutoFieldNum') || getFieldFromRecord(eventObj, 'AUTOFIELDNUM');

  if (btn === 'VIEW_SOURCE_TXN') {
    // RQ_HSE_12_4_26_00_40 — C++ NewCarEntryCategory: CAR Editing (HSE_TGCREDTNG) in same category as other CAR screens
    if (
      [
        'HSE_TGCRENTRY',
        'HSE_TGCRRVW',
        'HSE_TGACTNSENTRY',
        'HSE_TGACTNSRVIW',
        'HSE_TGCRAPRVL',
        'HSE_TGCREDTNG',
      ].includes(scr)
    ) {
      handleViewSourceTxn(eventObj, devInterfaceObj);
      return true;
    }
  }

  if (btn === 'CRENTRY_VWRJCTRSNS' && scr === 'HSE_TGCRENTRY') {
    handleViewRejectReasons(eventObj, devInterfaceObj);
    return true;
  }

  if (btn === 'ENTRY_COMPLETED') {
    if (scr === 'HSE_TGCRENTRY') {
      await handleEntryCompletedCarEntry(eventObj, devInterfaceObj);
      return true;
    }
    if (scr === 'HSE_TGACTNSENTRY') {
      await handleEntryCompletedActionsEntry(eventObj, devInterfaceObj);
      return true;
    }
  }

  if (btn === 'REJECT_CAR') {
    const src = REJECT_SOURCE_BY_SCREEN[scr];
    if (src && typeof openScr === 'function' && strPrimKey) {
      // RQ_HSE_12_4_26_00_40 — M4: blocking confirm when possible (AskYesNoMessage may be non-blocking in some shells)
      if (scr === 'HSE_TGCRFLOUP') {
        const ar = devInterfaceObj.getCurrentLang?.() === 'ar';
        const msgBlocking = ar
          ? 'تنبيه: رفض المتابعة (سلوك النظام القديم) يضبط حالة CAR على 15 (معتمد / إعادة عمل). لا يعادل رفض CAR في شاشة مراجعة CAR. هل تريد المتابعة؟'
          : 'Note (legacy): Follow-up reject sets CAR status to 15 (Approved / send back for rework). This is not the same as rejecting at CAR Review. Continue?';
        const msgNotifyOnly = ar
          ? 'تنبيه: رفض المتابعة يضبط حالة CAR على 15 (معتمد / إعادة عمل).'
          : 'Note (legacy): Follow-up reject sets CAR status to 15 (Approved / send back for rework).';
        // RQ_HSE_12_4_26_00_40 — GAP-19: prefer confirm() for blocking; fallback to AskYesNoQuestion if available; then AskYesNoMessage
        if (typeof globalThis !== 'undefined' && typeof globalThis.confirm === 'function') {
          if (!globalThis.confirm(msgBlocking)) return true;
        } else if (typeof devInterfaceObj?.AskYesNoQuestion === 'function') {
          const confirmed = await devInterfaceObj.AskYesNoQuestion(msgBlocking);
          if (!confirmed) return true;
        } else if (typeof devInterfaceObj?.AskYesNoMessage === 'function') {
          devInterfaceObj.AskYesNoMessage(msgNotifyOnly);
        }
      }
      // RQ_HSE_13_3_26_1_43: C++ RejectCARProcess – set pending context so OK button triggers ChangeEntityStatus + status change
      const rejectStatus = CAR_REJECT_STATUS_BY_SCREEN[scr];
      const rejectAction = CAR_REJECT_ACTION_BY_SCREEN[scr];
      const caption = CAR_SCREEN_CAPTION[scr] || scr;
      if (rejectStatus && rejectAction) {
        setPendingRejectCAR({
          screenTag: scr,
          prmky: strPrimKey,
          action: 'REJECT_CAR',
          newStatus: rejectStatus,
          actionDescription: rejectAction,
          sourceScreenName: caption,
          getUserName: devInterfaceObj?.getUserName,
          FormSetField: devInterfaceObj?.FormSetField,
          doToolbarAction: devInterfaceObj?.doToolbarAction,
          refreshData: devInterfaceObj?.refreshData,
        });
      }
      openRejectReasonScreen(openScr, src, strPrimKey);
      return true;
    }
  }

  if (btn === 'VIEW_CAR_REVIEW_INFO') {
    if (['HSE_TGCRAPRVL', 'HSE_TGACTNSENTRY', 'HSE_TGCREDTNG'].includes(scr) && openScr && strPrimKey) {
      openCarReviewInfo(openScr, strPrimKey, '');
      return true;
    }
  }

  if (btn === 'VIEW_CAR_APPROVE_INFO') {
    if (['HSE_TGACTNSENTRY', 'HSE_TGCREDTNG'].includes(scr) && openScr && strPrimKey) {
      openCarApproveInfo(openScr, strPrimKey);
      return true;
    }
  }

  if (btn === 'CORRECTIVE_ACTIONS') {
    if (CORRECTIVE_ACTIONS_BY_SCREEN[scr] && openScr && strKeyField) {
      // RQ_HSE_12_4_26_00_40 — GAP-12: pass dev + event for parent CAR date snapshot into popup defValObj
      openCorrectiveActionsPopup(openScr, scr, strKeyField, devInterfaceObj, eventObj);
      return true;
    }
  }

  if (btn === 'CAR_REVIEW_INFO' && (scr === 'HSE_TGCRRVW' || scr === 'HSE_TGCRENTRY')) {
    const handled = await tryOpenCarReviewInfoWithPolicy(eventObj, devInterfaceObj, scr);
    if (handled) return true;
  }

  if (btn === 'CAR_APPROVAL_INFO' && scr === 'HSE_TGCRAPRVL') {
    const handledPol = await tryOpenCarApproveInfoWithPolicy(eventObj, devInterfaceObj);
    if (handledPol) return true;
  }

  // RQ_HSE_13_3_26_1_43: C++ CANCEL_CAR on Approval – set pending with status 20, "CAR Cancelled"
  if (btn === 'CANCEL_CAR' && scr === 'HSE_TGCRAPRVL') {
    if (openScr && strPrimKey) {
      setPendingRejectCAR({
        screenTag: scr,
        prmky: strPrimKey,
        action: 'CANCEL_CAR',
        newStatus: '20',
        actionDescription: 'CAR Cancelled',
        sourceScreenName: 'CAR Approval',
        getUserName: devInterfaceObj?.getUserName,
        FormSetField: devInterfaceObj?.FormSetField,
        doToolbarAction: devInterfaceObj?.doToolbarAction,
        refreshData: devInterfaceObj?.refreshData,
      });
      openRejectReasonScreen(openScr, 'CRCTVEACCENT-AP', strPrimKey);
      return true;
    }
  }

  if (btn === 'CLOSE_CAR') {
    // RQ_HSE_12_4_26_00_40 — legacy CLOSE on follow-up / editing-style screens; align CAR Editing with desktop wording
    if (['HSE_TGACTNSRVIW', 'HSE_TGCRFLOUP', 'HSE_TGCREDTNG'].includes(scr)) {
      await handleCloseCar(eventObj, devInterfaceObj, eventObj?.strScrTag || strScrTag);
      return true;
    }
  }

  if (btn === 'ACCEPT_CAR') {
    // RQ_HSE_13_3_26_1_43: C++ AcceptCARProcess – check policy, tracing via ChangeEntityStatus, status 10, save
    if (scr === 'HSE_TGCRRVW') {
      await handleAcceptCarReview(eventObj, devInterfaceObj);
      return true;
    }
    if (scr === 'HSE_TGACTNSRVIW') {
      const { executeSQLPromise, FormSetField, doToolbarAction, refreshData, getUserName, AskYesNoMessage } = devInterfaceObj || {};
      if (strPrimKey && executeSQLPromise && FormSetField) {
        try {
          // RQ_HSE_12_4_26_00_40 — GAP-4: allow ACCEPT from 16 (post–Actions Entry) or 06 (desktop S06→S19 direct re-review after approval rejection)
          const okAc = await assertCrentryStatusAllows(executeSQLPromise, strPrimKey, ['16', '06'], AskYesNoMessage);
          if (!okAc) return true;
          const r1 = await executeSQLPromise(
            `update HSE_ACTNSENTRY_CRR set prmky=prmky where prmky=${strPrimKey} AND UPPER(LTRIM(RTRIM(ISNULL(CRRCTVACTNS_LNKCSS_ACC,N'')))) <> N'Y'`
          );
          if ((r1?.recordsAffected ?? r1?.rowsAffected ?? 0) > 0) {
            if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Please check the corrections TAB');
            return true;
          }
          const r2 = await executeSQLPromise(
            `update HSE_CRRCTVACTNS set CRRCTVACTNS_lnkwthmn=CRRCTVACTNS_lnkwthmn where UPPER(LTRIM(RTRIM(ISNULL(CRRCTVACTNS_acc,N'')))) = N'N' and CRRCTVACTNS_lnkwthmn in (select autoFieldnum from HSE_ACTNSENTRY_RTCSS where prmky=${strPrimKey})`
          );
          if ((r2?.recordsAffected ?? r2?.rowsAffected ?? 0) > 0) {
            if (devInterfaceObj.AskYesNoMessage) devInterfaceObj.AskYesNoMessage('Please check the Root Causes TAB & related corrective actions');
            return true;
          }
          const un = (typeof getUserName === 'function' ? getUserName() : '') || '';
          // RQ_HSE_12_4_26_00_40 — C++ InsertIntoTracingTabProcess → ChangeEntityStatus (not raw INSERT)
          await executeChangeEntityStatus(executeSQLPromise, strPrimKey, 'Actions Review', un, 'CAR Accepted');
          FormSetField('HSE_TgActnsRviw', 'CRENTRY_CRSTT', '19', 'scr');
          const upd = `with ROOT_CAUSES_TRGTDT as (select max(CRRCTVACTNS_trgDt) RootCauseTrgtdt from HSE_CRRCTVACTNS where CRRCTVACTNS_lnkWthMn in (select Autofieldnum from HSE_ACTNSENTRY_RTCSS where prmky=${strPrimKey})), MAIN_TRGTDT as (select CRENTRY_CRTRGDT MainTrgtdt from hse_CRENTRY where prmky=${strPrimKey}), JOIN_THE_TWO as(select * from ROOT_CAUSES_TRGTDT,MAIN_TRGTDT) update JOIN_THE_TWO set MainTrgtdt=RootCauseTrgtdt where isnull(MainTrgtdt,0)<RootCauseTrgtdt`;
          await executeSQLPromise(upd);
          if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', 'HSE_TgActnsRviw', '');
          if (typeof refreshData === 'function') refreshData('');
        } catch (e) {
          console.warn('[Web_HSE] ACCEPT_CAR Actions Review:', e);
        }
      }
      return true;
    }
  }

  // RQ_HSE_13_3_26_1_43: C++ ApproveCARProcess2 – check policy, tracing via ChangeEntityStatus, status 15, copy dept/project, save
  if (btn === 'APPROVE_CAR' && scr === 'HSE_TGCRAPRVL') {
    await handleApproveCarApproval(eventObj, devInterfaceObj);
    return true;
  }

  return false;
}
