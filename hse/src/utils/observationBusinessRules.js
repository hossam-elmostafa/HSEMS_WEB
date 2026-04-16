/**
 * RQ_HSE_11_4_26_15_44
 * Observation business rules — desktop parity logic for the hse web layer.
 *
 * G_ABS_1: getRootCauses / getExpectedHazards orchestration
 * G_ABS_2: New-record defaults (concerned dept + default Require CAR from HSE_HSEPLCCAR)
 * G_ABS_3: OBSACC ↔ RQRCR field-change coupling (NearMissCategory::DisplayFieldChange)
 * G_ABS_4: Duplicate reject-reason prevention on toolbar SAVE
 * G_ABS_5: getTargetMonthSTD / observation targets integration
 * G_ABS_6: SetRecordStatus / child TXNSTS sync with header RECSTS
 * G_ABS_7: Policy screen HSEOBSRVTN_GnrCrStt non-empty validation
 */

import { toast as toastify } from 'react-toastify';
import { escapeSqlStringLiteral } from './sqlLiteralEscape.js';

// RQ_HSE_11_4_26_15_44
function observationWarn(devInterface, message) {
  const t = devInterface?.toast;
  if (t && typeof t.warning === 'function') {
    t.warning(message);
    return;
  }
  toastify.warning(message);
}

// ---------------------------------------------------------------------------
// SQL result normalizer (same pattern as ObservationTabManagement.firstRowFromSqlResult)
// ---------------------------------------------------------------------------
function firstRow(result) {
  if (result == null) return null;
  if (Array.isArray(result) && result.length > 0) return result[0];
  const rs = result.recordset;
  if (Array.isArray(rs) && rs.length > 0) return rs[0];
  if (Array.isArray(result.recordsets) && result.recordsets.length > 0) {
    const rs0 = result.recordsets[0];
    if (Array.isArray(rs0) && rs0.length > 0) return rs0[0];
  }
  const nested = result[0]?.recordset;
  if (Array.isArray(nested) && nested.length > 0) return nested[0];
  return null;
}

function colVal(row, ...names) {
  if (!row || typeof row !== 'object') return '';
  for (const n of names) {
    if (row[n] != null && row[n] !== '') return String(row[n]).trim();
    const lower = n.toLowerCase();
    if (row[lower] != null && row[lower] !== '') return String(row[lower]).trim();
  }
  return '';
}

// ---------------------------------------------------------------------------
// G_ABS_1 — getRootCauses / getExpectedHazards orchestration
// C++: NearMissEntryCategory — optional "Get root causes" / "Get expected hazards"
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// Calls getRootCauses stored procedure for the given observation type, then refreshes.
export async function executeGetRootCauses(devInterface, observationType) {
  const { executeSQLPromise, refreshData } = devInterface || {};
  if (!executeSQLPromise) return;
  // RQ_HSE_11_4_26_15_44 — user feedback when type missing (legacy requires type for template proc)
  if (!observationType || !String(observationType).trim()) {
    observationWarn(devInterface, 'Observation type is required before loading root causes.');
    return;
  }
  const esc = escapeSqlStringLiteral(observationType);
  try {
    await executeSQLPromise(`EXECUTE getRootCauses '${esc}'`);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (err) {
    console.warn('[Web_HSE] getRootCauses failed:', err);
    observationWarn(devInterface, 'Could not load root causes. Check observation type and database availability.');
  }
}

// RQ_HSE_11_4_26_15_44
// Calls getExpectedHazards stored procedure for the given observation type, then refreshes.
export async function executeGetExpectedHazards(devInterface, observationType) {
  const { executeSQLPromise, refreshData } = devInterface || {};
  if (!executeSQLPromise) return;
  // RQ_HSE_11_4_26_15_44
  if (!observationType || !String(observationType).trim()) {
    observationWarn(devInterface, 'Observation type is required before loading expected hazards.');
    return;
  }
  const esc = escapeSqlStringLiteral(observationType);
  try {
    await executeSQLPromise(`EXECUTE getExpectedHazards '${esc}'`);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (err) {
    console.warn('[Web_HSE] getExpectedHazards failed:', err);
    observationWarn(devInterface, 'Could not load expected hazards. Check observation type and database availability.');
  }
}

// ---------------------------------------------------------------------------
// G_ABS_2 — New-record defaults
// C++: NearMissCategory — default Concerned Department from user's department
// C++: checkRequireCAR — HSE_HSEPLCCAR.HSEPLCCAR_DFLRQRCR on new recommended-action lines
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// On NEW main record, default NRSTMISCENT_CNCRNDDEP from user's department (CMN_EMPLOYEE).
export async function applyNewObservationDefaults(devInterface) {
  const { executeSQLPromise, FormSetField, getUserName } = devInterface || {};
  if (!executeSQLPromise || !FormSetField || !getUserName) return;
  const login = typeof getUserName === 'function' ? getUserName() : '';
  if (!login) return;
  const esc = escapeSqlStringLiteral(String(login).trim());
  try {
    const result = await executeSQLPromise(
      `SELECT TOP 1 EMPLOYEE_DPRT FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID='${esc}' OR EMPLOYEE_LOGINNAME='${esc}'`
    );
    const row = firstRow(result);
    const dept = colVal(row, 'EMPLOYEE_DPRT', 'employee_dprt');
    if (dept) {
      FormSetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_CNCRNDDEP', dept, 'scr');
    }
  } catch (err) {
    console.warn('[Web_HSE] applyNewObservationDefaults (concerned dept):', err);
  }
}

// RQ_HSE_11_4_26_15_44
// On NEW recommended-action line, default Require CAR from policy HSE_HSEPLCCAR.HSEPLCCAR_DFLRQRCR.
export async function applyDefaultRequireCAROnNewAction(devInterface, tabTable) {
  const { executeSQLPromise, FormSetField } = devInterface || {};
  if (!executeSQLPromise || !FormSetField) return;
  try {
    const result = await executeSQLPromise(
      `SELECT TOP 1 ISNULL(HSEPLCCAR_DFLRQRCR,'') AS DFLRQRCR FROM HSE_HSEPLCCAR`
    );
    const row = firstRow(result);
    const val = colVal(row, 'DFLRQRCR', 'HSEPLCCAR_DFLRQRCR', 'hseplccar_dflrqrcr');
    if (val) {
      const tbl = tabTable || 'HSE_NRSTMISCENTRQDACTN';
      FormSetField(tbl, 'NRSTMISCENTRQDACTN_RQRCAR', val, 'tab');
    }
  } catch (err) {
    console.warn('[Web_HSE] applyDefaultRequireCAROnNewAction:', err);
  }
}

// ---------------------------------------------------------------------------
// G_ABS_3 — OBSACC ↔ RQRCR coupling
// C++: NearMissCategory::DisplayFieldChange — when OBSACC changes:
//   Y → enable RQRCR
//   N → lock RQRCR and align to N
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// Called from SubFieldChanged when NRSTMISCENT_OBSACC changes.
export function handleObsAccFieldChange(devInterface, newValue) {
  const { FormSetField, setFieldReadOnly } = devInterface || {};
  if (!FormSetField) return;
  const val = String(newValue ?? '').toUpperCase().trim();
  const TABLE = 'HSE_vwNRSTMISCENT';
  const RQRCR = 'NRSTMISCENT_RQRCR';
  // RQ_HSE_11_4_26_15_44 — do not force RQRCR on blank OBSACC (avoid clobbering draft header)
  if (val === '') {
    return;
  }
  if (val === 'Y') {
    if (typeof setFieldReadOnly === 'function') {
      setFieldReadOnly(TABLE, RQRCR, false);
    }
  } else {
    FormSetField(TABLE, RQRCR, 'N', 'scr');
    if (typeof setFieldReadOnly === 'function') {
      setFieldReadOnly(TABLE, RQRCR, true);
    }
  }
}

// ---------------------------------------------------------------------------
// G_ABS_4 — Duplicate reject-reason prevention on toolbar SAVE
// C++: NearMissCategory save path — blocks duplicate HSE_RJCTRSN rows
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// Before SAVE on HSE_RJCTRSN (new mode), check for existing row with same link+module+tracing=0.
// Returns { ok: true } if save may proceed, { ok: false, message } if duplicate.
export async function checkDuplicateRejectReason(devInterface, fullRecord) {
  const { executeSQLPromise } = devInterface || {};
  if (!executeSQLPromise || !fullRecord) return { ok: true };
  const link = String(fullRecord.RJCTRSN_LINKWITHMAIN ?? '').trim();
  const mod = String(fullRecord.RJCTRSN_MODULETYPE ?? '').trim();
  if (!link || !mod) return { ok: true };
  const escLink = escapeSqlStringLiteral(link);
  const escMod = escapeSqlStringLiteral(mod);
  try {
    const sql = `SELECT COUNT(0) AS CNT FROM HSE_RJCTRSN WHERE RJCTRSN_LINKWITHMAIN='${escLink}' AND RJCTRSN_MODULETYPE='${escMod}' AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0`;
    const result = await executeSQLPromise(sql);
    const row = firstRow(result);
    const cnt = parseInt(colVal(row, 'CNT', 'cnt') || '0', 10);
    if (cnt > 0) {
      return { ok: false, message: 'A reject reason for this record already exists. Duplicate entry is not allowed.' };
    }
  } catch (err) {
    console.warn('[Web_HSE] checkDuplicateRejectReason:', err);
  }
  return { ok: true };
}

// ---------------------------------------------------------------------------
// G_ABS_5 — getTargetMonthSTD / observation targets
// C++: getTargetMonthSTD reads HSE_OBSRVTNTRGTSTRGTS for expected observations by dept/month/year.
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// On department change in entry, derive NRSTMISCENT_TRGTMNTHSTD from observation targets table.
export async function applyTargetMonthSTD(devInterface, issueDateStr, reportingDept) {
  const { executeSQLPromise, FormSetField } = devInterface || {};
  if (!executeSQLPromise || !FormSetField) return;
  if (!issueDateStr || !reportingDept) return;
  const escDept = escapeSqlStringLiteral(reportingDept);
  try {
    const d = new Date(issueDateStr);
    if (isNaN(d.getTime())) return;
    const month = d.getMonth() + 1;
    const year = d.getFullYear();
    const sql = `SELECT TOP 1 OBSRVTNTRGTSTRGTS_TRGT AS TGT FROM HSE_OBSRVTNTRGTSTRGTS WHERE OBSRVTNTRGTSTRGTS_DPRT='${escDept}' AND OBSRVTNTRGTSTRGTS_MNTH=${month} AND OBSRVTNTRGTSTRGTS_YR=${year}`;
    const result = await executeSQLPromise(sql);
    const row = firstRow(result);
    const tgt = colVal(row, 'TGT', 'OBSRVTNTRGTSTRGTS_TRGT', 'obsrvtntrgtstrgts_trgt');
    if (tgt) {
      FormSetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_TRGTMNTHSTD', tgt, 'scr');
    }
  } catch (err) {
    console.warn('[Web_HSE] applyTargetMonthSTD:', err);
  }
}

// ---------------------------------------------------------------------------
// G_ABS_6 — SetRecordStatus / child TXNSTS sync
// C++: NearMissCategory SAVE path — when header RECSTS is 2 or 3, sync child TXNSTS → 1.
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// After main-form SAVE (post), sync child TXNSTS with header RECSTS where applicable.
const CHILD_TXNSTS_TABLES = [
  { table: 'HSE_NRSTMISCENTPSSBLNRSTMISCCUSE', txnField: 'NRSTMISCENTPSSBLNRSTMISCCUSE_TXNSTS', linkField: 'NRSTMISCENTPSSBLNRSTMISCCUSE_LNK' },
  { table: 'HSE_NRSTEXPCTDHRZ',                txnField: 'NRSTEXPCTDHRZ_TXNSTS',                linkField: 'NRSTEXPCTDHRZ_LNK' },
  { table: 'HSE_NRSTMISCENTRQDACTN',           txnField: 'NRSTMISCENTRQDACTN_TXNSTS',           linkField: 'NRSTMISCENTRQDACTN_LNK' },
];

export async function syncChildTxnStatus(devInterface, headerKey, headerRecsts) {
  const { executeSQLPromise } = devInterface || {};
  if (!executeSQLPromise || !headerKey) return;
  const sts = parseInt(headerRecsts, 10);
  if (sts !== 2 && sts !== 3) return;
  const escKey = escapeSqlStringLiteral(String(headerKey));
  for (const child of CHILD_TXNSTS_TABLES) {
    try {
      await executeSQLPromise(
        `UPDATE ${child.table} SET ${child.txnField}=1 WHERE ${child.linkField}='${escKey}' AND ISNULL(${child.txnField},0)<>1`
      );
    } catch (err) {
      console.warn(`[Web_HSE] syncChildTxnStatus ${child.table}:`, err);
    }
  }
}

// ---------------------------------------------------------------------------
// G_ABS_7 — Policy screen HSEOBSRVTN_GnrCrStt non-empty validation
// C++: Observation policy exit check — HSEOBSRVTN_GnrCrStt must be non-empty.
// ---------------------------------------------------------------------------

// RQ_HSE_11_4_26_15_44
// Validates observation policy before save — GnrCrStt must have a value.
export function validateObservationPolicyGnrCrStt(devInterface) {
  const { FormGetField } = devInterface || {};
  if (typeof FormGetField !== 'function') return { ok: true };
  let val = '';
  try {
    val = FormGetField('HSE_HSEOBSRVTN', 'HSEOBSRVTN_GnrCrStt', 'SCR');
  } catch (_) {
    return { ok: true };
  }
  const trimmed = val != null ? String(val).trim() : '';
  if (!trimmed) {
    return { ok: false, message: 'Generated CAR Status (HSEOBSRVTN_GnrCrStt) must not be empty. Please set a value before saving.' };
  }
  return { ok: true };
}
