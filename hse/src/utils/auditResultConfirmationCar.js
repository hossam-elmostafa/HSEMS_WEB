/**
 * RQ_HSE_12_4_26_00_40 — GAP-5: Audit Result Confirmation — RESULT_CONFIRMED on Audits tab; trigger context batched with INSERT (carTriggerContextBatch.js).
 * RQ_HSE_12_4_26_00_40 — GAP-2: Optional FK column on HSE_CRENTRY verified via INFORMATION_SCHEMA.
 * C++: CAuditModuleCategory::AuditConfirmation_AddRecordToCAREntry.
 *
 * REWRITTEN to use client-side INSERT (same pattern as generateCARFromObservation in
 * ObservationButtonHandlers.js) because the database is managed by the desktop system
 * and we cannot create new stored procedures.
 */

import { getNextAllocatedCrentryCrn, getNextAllocatedCrentryCrsrln } from './carCrentrySerialAllocation.js';
import { buildCrentryTriggerContextBatchSql } from './carTriggerContextBatch.js';

function esc(v) { return String(v ?? '').replace(/'/g, "''"); }

function getScalarStr(row, keys) {
  if (!row || typeof row !== 'object') return '';
  for (const k of keys) {
    const u = k.toUpperCase();
    const found = Object.keys(row).find((x) => x.toUpperCase() === u);
    if (found != null) {
      const v = String(row[found] ?? '').trim();
      if (v) return v;
    }
  }
  return '';
}

function getScalarInt(row, keys) {
  if (!row || typeof row !== 'object') return 0;
  for (const k of keys) {
    const u = k.toUpperCase();
    const found = Object.keys(row).find((x) => x.toUpperCase() === u);
    if (found != null) {
      const v = parseInt(row[found], 10);
      if (Number.isFinite(v)) return v;
    }
  }
  return 0;
}

const isDupKeyErr = (e) => {
  const m = String(e?.message ?? e ?? '').toLowerCase();
  return m.includes('2627') || m.includes('2601') || m.includes('duplicate') || m.includes('unique constraint');
};

/**
 * @returns {Promise<boolean>} true if this handler consumed the click
 */
export async function handleAuditResultConfirmationCarButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = String(buttonName || '').toUpperCase();
  if (btn !== 'RESULT_CONFIRMED') return false;

  const scr = String(strScrTag || '').toUpperCase();
  if (scr !== 'HSE_ADTRSLTCNFRMTN') return false;

  const tab = String(eventObj?.strTabTag || '').toUpperCase();
  if (tab !== 'HSE_ADTRSLTCNFRMTN_AUDTS') return false;

  const {
    executeSQLPromise,
    FormGetField,
    FormSetField,
    doToolbarAction,
    refreshData,
    getUserName,
    AskYesNoMessage,
  } = devInterfaceObj || {};

  if (!executeSQLPromise) return false;

  let prmky = '';
  const rec = eventObj?.fullRecord;
  const row0 = Array.isArray(rec) ? rec[0] : rec;
  if (row0 && typeof row0 === 'object') {
    const pk = Object.keys(row0).find((x) => x.toUpperCase() === 'PRMKY');
    if (pk) prmky = String(row0[pk] ?? '').trim();
  }
  if (!prmky && typeof FormGetField === 'function') {
    prmky = String(
      FormGetField('HSE_ADTRSLTCNFRMTN_AUDTS', 'PrmKy', 'tab') ||
        FormGetField('HSE_ADTRSLTCNFRMTN_AUDTS', 'PrmKy', 'scr') ||
        FormGetField('HSE_ADTRSLTCNFRMTN_AUDTS', 'PrmKy', 'TAB') ||
        ''
    ).trim();
  }

  if (!prmky || !/^\d+$/.test(prmky)) {
    if (AskYesNoMessage) {
      AskYesNoMessage('RQ_HSE_12_4_26_00_40: Cannot resolve Audits tab primary key (PrmKy) for Result Confirmed.');
    }
    return true;
  }

  const n = parseInt(prmky, 10);

  try {
    // RQ_HSE_12_4_26_00_40 — GAP-5: fetch qualifying findings that require CAR generation
    const findingsRs = await executeSQLPromise(
      `SELECT PrmKy, ISNULL(CLUS_LVLRSULTPPUP_FNDNGS,'') AS FNDNGS, ISNULL(CLUS_LVLRSULTPPUP_DESC,'') AS FDESC FROM dbo.HSE_CLUS_LVLRSULTPPUP WHERE MainLink = ${n}`
    );
    const findingsRows = findingsRs?.recordset ?? findingsRs ?? [];
    if (!Array.isArray(findingsRows) || findingsRows.length === 0) {
      return true;
    }

    // RQ_HSE_12_4_26_00_40 — GAP-5: read audit policy for CAR generation
    let CARStatus = '';
    let strCARSource = '';
    let strOwnerDept = '';
    try {
      const polRs = await executeSQLPromise(`SELECT HSEPLC_ADT_GNRCRSTT, HSEPLC_ADT_CRBSS FROM HSE_HSEPLC_ADT`);
      const polRow = polRs?.recordset?.[0] ?? polRs?.[0];
      CARStatus = getScalarStr(polRow, ['HSEPLC_ADT_GNRCRSTT']);
      strCARSource = getScalarStr(polRow, ['HSEPLC_ADT_CRBSS']);
    } catch (_) { /* use defaults */ }
    if (CARStatus.length < 2) CARStatus = CARStatus.padStart(2, '0');
    if (!CARStatus) CARStatus = '01';

    try {
      const deptRs = await executeSQLPromise(`SELECT HSEPLC_OWNRDPRT FROM HSE_HSEPLC`);
      const deptRow = deptRs?.recordset?.[0] ?? deptRs?.[0];
      strOwnerDept = getScalarStr(deptRow, ['HSEPLC_OWNRDPRT']);
    } catch (_) {}

    const userName = (typeof getUserName === 'function' ? getUserName() : '') || '';

    // RQ_HSE_12_4_26_00_40 — GAP-5: resolve employee number from login
    let strEmployee = '';
    const escLogin = esc(userName);
    for (const col of ['EMPLOYEE_USRID', 'EMPLOYEE_LOGINNAME', 'EMPLOYEE_USERID']) {
      try {
        const empRs = await executeSQLPromise(`SELECT TOP 1 EMPLOYEE_NO FROM CMN_EMPLOYEE WHERE ${col} = '${escLogin}'`);
        const empRow = empRs?.recordset?.[0] ?? empRs?.[0];
        const no = getScalarStr(empRow, ['EMPLOYEE_NO']);
        if (no) { strEmployee = no; break; }
      } catch (_) {}
    }

    // RQ_HSE_12_4_26_00_40 — GAP-5: read audit plan details for context fields
    let strAuditYear = '';
    let strAuditNum = '';
    let strDetailNum = '';
    try {
      const auditRs = await executeSQLPromise(
        `SELECT TOP 1 ISNULL(ADTPLNENT_ADTYR,'') AS AYR, ISNULL(ADTPLNENT_ADTNUM,'') AS ANUM, ISNULL(ADTPLNENT_DTLNUM,'') AS DNUM FROM HSE_ADTPLNENT WHERE PrmKy = ${n}`
      );
      const aRow = auditRs?.recordset?.[0] ?? auditRs?.[0];
      strAuditYear = getScalarStr(aRow, ['AYR']);
      strAuditNum = getScalarStr(aRow, ['ANUM']);
      strDetailNum = getScalarStr(aRow, ['DNUM']);
    } catch (_) {}

    const now = new Date();
    const strCARDate = `${now.getFullYear()}-${String(now.getMonth() + 1).padStart(2, '0')}-${String(now.getDate()).padStart(2, '0')}`;
    const strCARYear = String(now.getFullYear());

    // RQ_HSE_12_4_26_00_40 — GAP-5: context is prepended per INSERT (same batch as INSERT — avoids pool split vs separate seed call)

    // RQ_HSE_12_4_26_00_40 — GAP-2: omit CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK if not on HSE_CRENTRY (avoids silent total failure)
    let includeFindingFkColumn = true;
    try {
      const colRs = await executeSQLPromise(
        `SELECT COUNT(*) AS Cnt FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = 'dbo' AND TABLE_NAME = 'HSE_CRENTRY' AND COLUMN_NAME = 'CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK'`
      );
      const colRow = colRs?.recordset?.[0] ?? colRs?.[0];
      includeFindingFkColumn = getScalarInt(colRow, ['Cnt', 'CNT']) > 0;
      if (!includeFindingFkColumn) {
        console.warn(
          '[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-2: HSE_CRENTRY has no CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK; CAR INSERT omits finding FK (desktop parity link lost for this DB).'
        );
      }
    } catch (schemaErr) {
      console.warn(
        '[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-2: could not read INFORMATION_SCHEMA for CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK; keeping legacy INSERT shape.',
        schemaErr
      );
    }

    let generatedCount = 0;

    // RQ_HSE_12_4_26_00_40 — GAP-5: for each finding, generate a CAR (mirrors C++ AuditConfirmation_AddRecordToCAREntry)
    for (const finding of findingsRows) {
      const findingPK = getScalarInt(finding, ['PrmKy', 'PRMKY', 'prmky']);
      const findingDesc = getScalarStr(finding, ['FDESC', 'CLUS_LVLRSULTPPUP_DESC']);

      let strCRN = '1';
      let insertErr = null;
      for (let attempt = 0; attempt < 5; attempt++) {
        strCRN = await getNextAllocatedCrentryCrn(executeSQLPromise, strCARYear, undefined);
        const strSRLN = await getNextAllocatedCrentryCrsrln(executeSQLPromise, strCARYear, strCARSource, undefined);

        // RQ_HSE_12_4_26_00_40 — GAP-2: dynamic column list when FK column absent
        const baseCols =
          'CRENTRY_CRDT,CRENTRY_CRYR,CRENTRY_CRN,CRENTRY_CRSTT,CRENTRY_DPR,CRENTRY_NM,CRENTRY_NCDSC,CRENTRY_CRPRR,CRENTRY_ATGNR,CRENTRY_CRSRC,CRENTRY_CRSRLN,CRENTRY_TXNYR,CRENTRY_TXNN,CRENTRY_DTLN';
        const baseVals = `'${esc(strCARDate)}','${esc(strCARYear)}','${esc(strCRN)}','${esc(CARStatus)}','${esc(strOwnerDept)}','${esc(strEmployee)}','${esc(findingDesc)}','m','Y','${esc(strCARSource)}','${esc(strSRLN)}','${esc(strAuditYear)}','${esc(strAuditNum)}','${esc(strDetailNum)}'`;
        const cols = includeFindingFkColumn
          ? `${baseCols},CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK`
          : baseCols;
        const vals = includeFindingFkColumn ? `${baseVals},${findingPK || 'NULL'}` : baseVals;

        try {
          const ctxBatch = buildCrentryTriggerContextBatchSql(
            userName,
            'Audit Result Confirmation',
            'Creating new CAR'
          );
          await executeSQLPromise(`${ctxBatch}INSERT INTO HSE_CRENTRY (${cols}) VALUES (${vals})`);
          insertErr = null;
          generatedCount++;
          break;
        } catch (e) {
          insertErr = e;
          if (!isDupKeyErr(e)) {
            console.error('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-5 audit CAR INSERT:', e);
            break;
          }
        }
      }
      if (insertErr && !isDupKeyErr(insertErr)) break;

      // RQ_HSE_12_4_26_00_40 — GAP-5: CopyImages per finding (best-effort)
      if (!insertErr) {
        try {
          const prmRs = await executeSQLPromise(
            `SELECT TOP 1 PrmKy FROM HSE_CRENTRY WHERE CRENTRY_CRYR='${esc(strCARYear)}' AND CRENTRY_CRN='${esc(strCRN)}'`
          );
          const prmRow = prmRs?.recordset?.[0] ?? prmRs?.[0];
          const newPK = getScalarStr(prmRow, ['PrmKy', 'PRMKY']);
          if (newPK && findingPK) {
            await executeSQLPromise(
              `EXEC CopyImages 'HSEMS','HSE_CLUS_LVLRSULTPPUP','PrmKy','${esc(String(findingPK))}','CLUS_LVLRSULTPPUP_IMG','HSE_CRENTRY','PrmKy','${esc(newPK)}','CRENTRY_NCPHT'`
            );
          }
        } catch (imgErr) {
          console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-5 CopyImages:', imgErr);
        }
      }
    }

    // RQ_HSE_12_4_26_00_40 — GAP-5: update audit status to confirmed
    if (typeof FormSetField === 'function') {
      try {
        FormSetField('HSE_ADTRSLTCNFRMTN_AUDTS', 'AUDTPLNENTRY_AUDTS_AUDTSTTUS', '10', 'tab');
      } catch (_) {
        try {
          FormSetField('HSE_AUDTPLNENTRY_AUDTS', 'AUDTPLNENTRY_AUDTS_AUDTSTTUS', '10', 'tab');
        } catch (_) {}
      }
    }
    if (typeof doToolbarAction === 'function') {
      try { doToolbarAction('SAVE', 'HSE_ADTRSLTCNFRMTN', 'HSE_ADTRSLTCNFRMTN_AUDTS'); } catch (_) {}
      try { doToolbarAction('SAVE', 'HSE_ADTRSLTCNFRMTN', ''); } catch (_) {}
    }
    if (typeof refreshData === 'function') refreshData('');

    if (generatedCount > 0 && AskYesNoMessage) {
      AskYesNoMessage(`${generatedCount} CAR(s) generated from audit findings. (RQ_HSE_12_4_26_00_40)`);
    }
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-5 auditResultConfirmationCar:', e);
    if (AskYesNoMessage) {
      AskYesNoMessage(`${e?.message || String(e)} — RQ_HSE_12_4_26_00_40`);
    }
  }

  return true;
}
