/**
 * Shared policy / SQL helpers for Aspects Register (Entry + Review).
 * <!-- RQ_HSE_23_3_26_22_02 -->
 *
 * C++ refs: EnvrnmntAspctCategory, EnvrnmntAspctReviewCategory, CHSEMSCommonCategory::getEmpDep
 */

const STX = String.fromCharCode(2);

export const ASPECTS_ENTRY_SCREEN_TAG = 'HSE_AspctsEntryAtEntry';
export const ASPECTS_REVIEW_SCREEN_TAG = 'HSE_AspctsRvwAtRvw';
export const TABLE_ASPECTS_VIEW = 'HSE_ASPCTS_VIEW';
export const TABLE_ASPECTS_LINES = 'HSE_ASPCTS_ASPCTS';

export function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * @param {object} dev - devInterfaceObj
 * @returns {boolean}
 */
export function isAdminUserDev(dev) {
  if (typeof dev?.isAdminUser === 'function') {
    try {
      return !!dev.isAdminUser();
    } catch {
      /* fall through */
    }
  }
  const u = dev?.getCurrentUserObj?.();
  const t = (u?.Set_AS_Administrator ?? u?.set_as_administrator ?? '').toString();
  return t.toUpperCase().includes('ADMINISTRATOR');
}

/**
 * Current login key for CMN_EMPLOYEE (C++ passes GetUserLogin to getEmpDep which queries EMPLOYEE_USRID).
 */
export function getLoginKeyForEmployee(dev) {
  if (typeof dev?.getUserName === 'function') {
    const n = dev.getUserName();
    if (n != null && String(n).trim() !== '') return String(n).trim();
  }
  const u = dev?.getCurrentUserObj?.();
  if (u?.LogIn != null && String(u.LogIn).trim() !== '') return String(u.LogIn).trim();
  return '';
}

/**
 * @param {object} dev
 * @param {boolean} firstOnly - C++ getEmpDep(..., true)
 * @returns {Promise<string>} single code or comma-quoted list for SQL IN (...)
 */
export async function getEmployeeDepartmentClause(dev, firstOnly) {
  const { executeSQLPromise, getValFromRecordSet } = dev || {};
  const userKey = getLoginKeyForEmployee(dev);
  if (!executeSQLPromise || !userKey) return firstOnly ? '' : '';

  const sql = `SELECT TOP 1 ISNULL(EMPLOYEE_WSHOP,'') AS W FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '${escapeSqlString(userKey)}' OR EMPLOYEE_LOGINNAME = '${escapeSqlString(userKey)}'`;

  try {
    const data = await executeSQLPromise(sql);
    let w = '';
    if (getValFromRecordSet && data) {
      w = String(getValFromRecordSet(data, 'W') ?? '').trim();
    } else {
      const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
      w = String(row?.W ?? row?.w ?? '').trim();
    }
    if (!w) return firstOnly ? '' : '';

    const parts = w.includes(STX)
      ? w.split(STX).map((s) => s.trim()).filter(Boolean)
      : [w.trim()].filter(Boolean);
    const unique = [...new Set(parts)];
    if (firstOnly) return unique[0] || '';

    const quoted = unique.map((code) => `'${escapeSqlString(code)}'`);
    return quoted.join(',');
  } catch (e) {
    console.warn('[Web_HSE] aspectsRegisterPolicy getEmployeeDepartmentClause:', e);
    return firstOnly ? '' : '';
  }
}

/**
 * @returns {Promise<{ allowEntry: string, allowReview: string, aspectYr: string, aspectMth: string }>}
 */
export async function fetchHseplcAspectFlags(dev) {
  const { executeSQLPromise, getValFromRecordSet } = dev || {};
  const defaults = {
    allowEntry: 'Y',
    allowReview: 'Y',
    aspectYr: '',
    aspectMth: '',
  };
  if (!executeSQLPromise) return defaults;

  const sql = `SELECT TOP 1 
    ISNULL(HSEPLC_ALWASPCTENTRY,'') AS ALWENT,
    ISNULL(HSEPLC_EDTASPCTRVWSCR,'') AS ALWRVW,
    ISNULL(HSEPLC_ASPCTYR,'') AS YR,
    ISNULL(HSEPLC_ASPCTMNTH,'') AS MNTH
    FROM HSE_HSEPLC`;

  try {
    const data = await executeSQLPromise(sql);
    let row = null;
    if (getValFromRecordSet && data) {
      return {
        allowEntry: String(getValFromRecordSet(data, 'ALWENT') ?? 'Y').trim() || 'Y',
        allowReview: String(getValFromRecordSet(data, 'ALWRVW') ?? 'Y').trim() || 'Y',
        aspectYr: String(getValFromRecordSet(data, 'YR') ?? '').trim(),
        aspectMth: String(getValFromRecordSet(data, 'MNTH') ?? '').trim(),
      };
    }
    row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
    if (!row) return defaults;
    return {
      allowEntry: String(row.ALWENT ?? row.alwent ?? 'Y').trim() || 'Y',
      allowReview: String(row.ALWRVW ?? row.alwrvw ?? 'Y').trim() || 'Y',
      aspectYr: String(row.YR ?? row.yr ?? '').trim(),
      aspectMth: String(row.MNTH ?? row.mnth ?? '').trim(),
    };
  } catch (e) {
    console.warn('[Web_HSE] aspectsRegisterPolicy fetchHseplcAspectFlags:', e);
    return defaults;
  }
}

/**
 * C++ getfRskLvl
 * @param {string} riskRank
 * @returns {Promise<string>}
 */
export async function lookupRiskLevelDescription(dev, riskRank) {
  const r = String(riskRank ?? '').trim();
  if (!r || r === '0') return '';
  const { executeSQLPromise, getValFromRecordSet } = dev || {};
  if (!executeSQLPromise) return '';

  const rNum = parseInt(r, 10);
  if (!Number.isFinite(rNum) || rNum <= 0) return '';
  const sql = `SELECT TOP 1 RSKLVL_DSC AS DSC FROM HSE_RSKLVL WHERE RSKLVL_COD = (
    SELECT TOP 1 RSKRNKDESC_RSKLVL FROM HSE_RSKRNKDESC WHERE RSKRNKDESC_RNK = ${rNum}
  )`;

  try {
    const data = await executeSQLPromise(sql);
    if (getValFromRecordSet && data) {
      return String(getValFromRecordSet(data, 'DSC') ?? '').trim();
    }
    const row = data?.recordset?.[0] ?? data?.[0]?.recordset?.[0] ?? data?.[0];
    return String(row?.DSC ?? row?.dsc ?? '').trim();
  } catch (e) {
    console.warn('[Web_HSE] aspectsRegisterPolicy lookupRiskLevelDescription:', e);
    return '';
  }
}

/**
 * ChangeCriteria + refresh (WebInfra implements ChangeCriteria — RQ_HSE_23_3_26_22_02).
 */
export function applyAspectsListCriteria(dev, screenTag, whereSql) {
  const ChangeCriteria = dev?.ChangeCriteria;
  const refreshData = dev?.refreshData;
  const where = whereSql && String(whereSql).trim() ? String(whereSql).trim() : '';
  if (!where) return false;
  if (typeof ChangeCriteria !== 'function') {
    console.warn('[Web_HSE] aspectsRegisterPolicy: ChangeCriteria not available on devInterface');
    if (typeof dev?.AskYesNoMessage === 'function') {
      dev.AskYesNoMessage('View filter', 'ChangeCriteria is not available; cannot apply list criteria.');
    }
    return false;
  }
  const tag = screenTag || ASPECTS_ENTRY_SCREEN_TAG;
  try {
    ChangeCriteria(tag, '', where);
  } catch (e) {
    console.warn('[Web_HSE] aspectsRegisterPolicy ChangeCriteria:', e);
    return false;
  }
  if (typeof refreshData === 'function') refreshData('');
  return true;
}

/**
 * RQ_HSE_23_3_26_22_02 — desktop C×L → risk rank (product of numeric consequence and likelihood).
 */
export function computeAspectRiskRank(cVal, lVal) {
  const c = parseInt(String(cVal ?? '').trim(), 10);
  const l = parseInt(String(lVal ?? '').trim(), 10);
  if (!Number.isFinite(c) || !Number.isFinite(l) || c <= 0 || l <= 0) return null;
  return c * l;
}

function syncRiskRanksFromConsequenceLikelihood(dev) {
  const { FormGetField, FormSetField } = dev || {};
  if (!FormGetField || !FormSetField) return;
  const pre = computeAspectRiskRank(
    FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_CNSQUNC', 'tab'),
    FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_LKLHD', 'tab')
  );
  if (pre != null) {
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKRNK', String(pre), 'tab');
  }
  const post = computeAspectRiskRank(
    FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_CNSQUNC_1', 'tab'),
    FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_LKLHD_1', 'tab')
  );
  if (post != null) {
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKRNK_1', String(post), 'tab');
  }
}

const RISK_FIELDS_ENTRY = new Set([
  'ASPCTS_ASPCTS_CNSQUNC',
  'ASPCTS_ASPCTS_LKLHD',
  'ASPCTS_ASPCTS_CNSQUNC_1',
  'ASPCTS_ASPCTS_LKLHD_1',
]);

/** Review C++ focuses on RSKRNK_1; include pre/post C×L so edits on Review screen revalidate. */
const RISK_FIELDS_REVIEW = new Set([
  'ASPCTS_ASPCTS_RSKRNK_1',
  'ASPCTS_ASPCTS_CNSQUNC_1',
  'ASPCTS_ASPCTS_LKLHD_1',
  'ASPCTS_ASPCTS_CNSQUNC',
  'ASPCTS_ASPCTS_LKLHD',
]);

/**
 * Desktop Entry: if preRank < postRank → invalid. Review: same without message.
 */
export async function applyAspectLineRiskValidation(dev, fieldName, options = {}) {
  const { FormGetField, FormSetField, AskYesNoMessage } = dev || {};
  const { showInvalidMessage = false, mode = 'entry' } = options;

  if (!FormGetField || !FormSetField) return { cancel: 0 };

  const fld = String(fieldName || '').toUpperCase();
  const fields = mode === 'review' ? RISK_FIELDS_REVIEW : RISK_FIELDS_ENTRY;
  if (!fields.has(fld)) return { cancel: 0 };

  // RQ_HSE_23_3_26_22_02 — keep RSKRNK / RSKRNK_1 aligned with C×L before validation
  syncRiskRanksFromConsequenceLikelihood(dev);

  const n1 = parseInt(String(FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKRNK', 'tab') ?? '').trim(), 10);
  const n2 = parseInt(String(FormGetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKRNK_1', 'tab') ?? '').trim(), 10);

  if (!Number.isFinite(n1) || !Number.isFinite(n2)) {
    if (Number.isFinite(n1) && !Number.isFinite(n2)) {
      FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKACCPTD', 'N', 'tab');
    }
    return { cancel: 0 };
  }

  if (n1 < n2) {
    if (showInvalidMessage && typeof AskYesNoMessage === 'function') {
      AskYesNoMessage('Aspect risk', 'Invalid risk: post-mitigation rank cannot exceed pre-mitigation rank.');
    }
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKRNK_1', '', 'tab');
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_CNSQUNC_1', '', 'tab');
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_LKLHD_1', '', 'tab');
    FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKACCPTD', 'N', 'tab');
    return { cancel: 0 };
  }

  FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKACCPTD', 'Y', 'tab');

  if (
    (fld === 'ASPCTS_ASPCTS_CNSQUNC_1' || fld === 'ASPCTS_ASPCTS_LKLHD_1' || fld === 'ASPCTS_ASPCTS_RSKRNK_1') &&
    Number.isFinite(n2) &&
    n2 > 0
  ) {
    const lvl = await lookupRiskLevelDescription(dev, String(n2));
    if (lvl) FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKLVL_1', lvl, 'tab');
  }
  if ((fld === 'ASPCTS_ASPCTS_CNSQUNC' || fld === 'ASPCTS_ASPCTS_LKLHD') && Number.isFinite(n1) && n1 > 0) {
    const lvl = await lookupRiskLevelDescription(dev, String(n1));
    if (lvl) FormSetField(TABLE_ASPECTS_LINES, 'ASPCTS_ASPCTS_RSKLVL', lvl, 'tab');
  }

  return { cancel: 0 };
}

export function isAspectsLineTab(strTabTag) {
  const t = (strTabTag && String(strTabTag).toUpperCase()) || '';
  return t.includes('ASPCTS_ASPCTS') && !t.includes('TRCNG');
}
