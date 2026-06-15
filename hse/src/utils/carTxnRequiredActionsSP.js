/**
 * Required-action CAR buttons — desktop CHSEMSCommonCategory::generateCAR / updateCAR / cancelCAR / openRelatedCARs / accptWrk / rjctWrk.
 * C++ calls sp_Generate_CARs, sp_Update_CARs, cancelCAR, acceptRqrdActnCAR, rjctRqrdActnCAR.
 * RQ_HSE_12_4_26_00_40 — gap R7 / A18 (site survey & other TXN required-action rows).
 */

import { runTxnAndRefresh } from '../services/ModuleButtonHandlers/moduleButtonHandlersUtils.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/** Physical table HSE_* → link column {TABLEPART}_LNK (desktop generateCAR). */
function linkColumnForRequiredActionTable(tableName) {
  const t = String(tableName || '').trim();
  if (!t) return '';
  const part = t.replace(/^HSE_/i, '');
  return `${part}_LNK`;
}

function readFormField(FormGetField, table, field, sources = ['scr', 'tab', 'SCR', 'TAB']) {
  if (typeof FormGetField !== 'function' || !table || !field) return '';
  for (const src of sources) {
    try {
      const v = FormGetField(table, field, src);
      if (v != null && String(v).trim() !== '') return String(v).trim();
    } catch (_) {
      /* ignore */
    }
  }
  return '';
}

function numericTxnKey(raw) {
  const s = String(raw ?? '').trim();
  if (!s) return '';
  const n = parseInt(s, 10);
  return Number.isFinite(n) && n > 0 ? String(n) : '';
}

/**
 * @param {string} buttonName
 * @param {string} strScrTag
 * @param {object} eventObj
 * @param {object} devInterfaceObj
 * @returns {Promise<boolean>} true if handled
 */
export async function handleTxnRequiredActionCarButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = String(buttonName || '').toUpperCase();
  const scr = String(strScrTag || '').toUpperCase();

  // C++: skip drill inquiry contexts (TODO in legacy — hide button instead)
  if (scr.includes('DRLLPLNINQ')) return false;

  const tableName = String(
    eventObj?.strTblNam ?? eventObj?.Table_Name ?? eventObj?.tableName ?? ''
  ).trim();
  if (!tableName) return false;

  const isGenerate = btn.includes('_GENERATECAR');
  const isUpdate = btn.includes('_UPDTCAR');
  const isCancel = btn.includes('_CNLCAR');
  const isViewRelated = btn.includes('_VIEWRELATEDCAR');
  const isWrkAcptd = btn.includes('_WRKACPTD');
  const isWrkNotAcptd = btn.includes('_WRKNOTACPTD');

  if (!isGenerate && !isUpdate && !isCancel && !isViewRelated && !isWrkAcptd && !isWrkNotAcptd) {
    return false;
  }

  const { executeSQLPromise, FormGetField, openScr, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;

  const subFormTag = String(eventObj?.strTabTag || '').trim() || String(strScrTag || '').trim();
  const linkCol = linkColumnForRequiredActionTable(tableName);
  if (!linkCol) return false;

  const txnRaw = readFormField(FormGetField, tableName, linkCol);
  const txnKey = numericTxnKey(txnRaw);
  if (!txnKey) {
    if (AskYesNoMessage) {
      AskYesNoMessage(
        'RQ_HSE_12_4_26_00_40: Missing required-action link key for CAR operation. Save the row and retry.'
      );
    }
    return true;
  }

  const escSub = escapeSqlString(subFormTag);

  try {
    if (isGenerate) {
      const sql = `EXECUTE sp_Generate_CARs ${txnKey},'${escSub}'`;
      await runTxnAndRefresh(devInterfaceObj, sql, 'RQ_HSE_12_4_26_00_40: Error generating CAR(s)');
      return true;
    }

    if (isUpdate) {
      const carKey = readFormField(FormGetField, tableName, 'CAR_KEY');
      const ck = numericTxnKey(carKey);
      if (!ck) {
        if (AskYesNoMessage) AskYesNoMessage('RQ_HSE_12_4_26_00_40: CAR_KEY is required to update CAR.');
        return true;
      }
      const sql = `EXECUTE sp_Update_CARs ${txnKey},'${escSub}',${ck}`;
      await executeSQLPromise(sql);
      if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
      return true; /* consumed RQ_HSE_12_4_26_00_40 */
    }

    if (isCancel) {
      const carKey = readFormField(FormGetField, tableName, 'CAR_KEY');
      const ck = numericTxnKey(carKey);
      if (!ck) {
        if (AskYesNoMessage) AskYesNoMessage('RQ_HSE_12_4_26_00_40: CAR_KEY is required to cancel CAR.');
        return true;
      }
      const sql = `EXECUTE cancelCAR ${txnKey},'${escSub}',${ck}`;
      await runTxnAndRefresh(devInterfaceObj, sql, 'RQ_HSE_12_4_26_00_40: Error cancelling CAR');
      return true;
    }

    if (isViewRelated) {
      const carKey = readFormField(FormGetField, tableName, 'CAR_KEY');
      const ck = numericTxnKey(carKey);
      if (!ck) {
        if (AskYesNoMessage) AskYesNoMessage('RQ_HSE_12_4_26_00_40: CAR_KEY is required to view related CAR.');
        return true;
      }
      if (typeof openScr !== 'function') return true;
      const criteria = `SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_CARMODELNUM = ${ck})`;
      openScr('HSE_TGCRCTVEACCINQ', {}, criteria, 'list', false, {}, false, false);
      return true;
    }

    if (isWrkAcptd) {
      const mainLink = readFormField(FormGetField, tableName, 'MAINLINK');
      const ml = numericTxnKey(mainLink);
      if (!ml) {
        if (AskYesNoMessage) AskYesNoMessage('RQ_HSE_12_4_26_00_40: MAINLINK is required to accept work.');
        return true;
      }
      const sql = `EXECUTE acceptRqrdActnCAR '${escSub}',${ml}`;
      await runTxnAndRefresh(devInterfaceObj, sql, 'RQ_HSE_12_4_26_00_40: Error accepting required-action CAR work');
      return true;
    }

    if (isWrkNotAcptd) {
      const mainLink = readFormField(FormGetField, tableName, 'MAINLINK');
      const ml = numericTxnKey(mainLink);
      const carKey = readFormField(FormGetField, tableName, 'CAR_KEY');
      const ck = numericTxnKey(carKey);
      if (!ml || !ck) {
        if (AskYesNoMessage) {
          AskYesNoMessage('RQ_HSE_12_4_26_00_40: MAINLINK and CAR_KEY are required to reject work.');
        }
        return true;
      }
      const sql = `EXECUTE rjctRqrdActnCAR '${escSub}',${ml},${ck}`;
      await runTxnAndRefresh(devInterfaceObj, sql, 'RQ_HSE_12_4_26_00_40: Error rejecting required-action CAR work');
      return true;
    }
  } catch (e) {
    console.warn('[Web_HSE] carTxnRequiredActionsSP RQ_HSE_12_4_26_00_40:', e);
    if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Required-action CAR operation failed'));
    return true;
  }

  return false;
}
