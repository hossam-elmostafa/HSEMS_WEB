/**
 * CAR Follow-up (HSE_TGCRFLOUP): C++ DisplayScreenReady disables NEW/DELETE on
 * Corrections (HSE_ACTNSENTRY_Crr) and Root Causes (HSE_ACTNSENTRY_RtCss) toolbars.
 * RQ_HSE_12_4_26_00_40 — Web parity via FormEnableButton when the active tab matches those areas.
 * Assumption: tab tags contain ACTNSENTRY_CRR / ACTNSENTRY_RTCSS or equivalent (not shipped in repo).
 */

// RQ_HSE_12_4_26_00_40 — server date for ACTFLLDT (align with desktop GetServerDate())
import { getCarServerDate } from './carServerDate.js';

/**
 * @param {string} tabTag
 * @returns {'CRR'|'RTCSS'|'OTHER'}
 */
export function classifyCarFollowUpTab(tabTag) {
  const u = String(tabTag || '').toUpperCase();
  if (u.includes('ACTNSENTRY_CRR')) return 'CRR';
  if (u.includes('ACTNSENTRY_RTCSS')) return 'RTCSS';
  if (/(^|_)CRR($|_)/.test(u) && !u.includes('CRRCTV')) return 'CRR';
  if (u.includes('RTCSS')) return 'RTCSS';
  return 'OTHER';
}

/**
 * @param {object} dev - devInterfaceObj
 * @param {string} strSelectedTabTag - active sub-tab tag (empty => no-op)
 */
export function applyCarFollowUpChildToolbarParity(dev, strSelectedTabTag) {
  const fe = dev?.FormEnableButton;
  if (typeof fe !== 'function') return;
  const tab = String(strSelectedTabTag || '').trim();
  if (!tab) return;
  const kind = classifyCarFollowUpTab(tab);
  const enableStd = !(kind === 'CRR' || kind === 'RTCSS');
  try {
    fe(2, 1, 'NEW', enableStd, true);
    fe(2, 1, 'DELETE', enableStd, true);
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CAR Follow-up FormEnableButton:', e);
  }
}

/**
 * C++ NewCarEntryCategory::DisplayRecordRepostion — HSE_TGCRFLOUP + HSE_ACTNSENTRY_CRR:
 * default actual follow-up date when empty using SQL Server GETDATE().
 * RQ_HSE_12_4_26_00_40
 *
 * BEFORE: getCurrentFormatedDate() — client time-zone dependent.
 * AFTER:  getCarServerDate() queries SELECT GETDATE() first, falls back to client if unavailable.
 *
 * NOTE: This function is now async. CAR_Follow_Up.js ShowScreen awaits it;
 * MainSubReposition fires-and-forgets (acceptable: the field update is best-effort on reposition).
 */
const TAB_ACTNS_CRR = 'HSE_ACTNSENTRY_CRR';
const FLD_ACT_FLL_DT = 'ACTNSENTRY_CRR_ACTFLLDT';

export async function applyCarFollowUpCorrectionsActFllDt(dev, strSelectedTabTag) {
  if (classifyCarFollowUpTab(strSelectedTabTag || '') !== 'CRR') return;
  const { FormGetField, FormSetField, getCurrentFormatedDate, executeSQLPromise } = dev || {};
  if (typeof FormGetField !== 'function' || typeof FormSetField !== 'function') {
    return;
  }
  try {
    let cur =
      FormGetField(TAB_ACTNS_CRR, FLD_ACT_FLL_DT, 'tab') ||
      FormGetField(TAB_ACTNS_CRR, FLD_ACT_FLL_DT, 'TAB');
    if (cur != null && String(cur).trim() !== '') return;
    const dt = await getCarServerDate(executeSQLPromise, getCurrentFormatedDate);
    if (dt) FormSetField(TAB_ACTNS_CRR, FLD_ACT_FLL_DT, dt, 'tab');
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 CAR Follow-up ACTNSENTRY_CRR_ACTFLLDT:', e);
  }
}
