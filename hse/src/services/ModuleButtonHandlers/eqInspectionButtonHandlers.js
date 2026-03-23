/**
 * RQ_HSE_23_3_26_23_15 — Equipment inspection custom buttons (EQInspctnCategory::DisplayCustomButtonClicked parity).
 */

import {
  getEQInspectionPrimaryKey,
  escapeSqlString,
  runTxnAndRefresh,
} from './moduleButtonHandlersUtils.js';
import {
  showEqInformationDialog,
  confirmEqYesNo,
} from '../../utils/eqInspectionDialogs.js';

/** Menu TagName for EQ Inspection Inquiry (matches `screenHandlers` registry key `HSE_EQInspctnInqry`). */
const EQ_INQ_SCREEN_TAG = 'HSE_EQInspctnInqry';

function isEqInspectionButtonName(btn) {
  const b = String(btn || '').toUpperCase();
  return b.startsWith('EQINSPCTN_') || b.startsWith('INSPCTNCHCKLST_');
}

function applyEqInquiryChangeCriteria(devInterfaceObj, whereSql) {
  const ChangeCriteria = devInterfaceObj?.ChangeCriteria;
  if (typeof ChangeCriteria !== 'function') return;
  ChangeCriteria(EQ_INQ_SCREEN_TAG, '', whereSql);
}

function promptSaveFirst(devInterfaceObj) {
  void showEqInformationDialog(devInterfaceObj, 'You must save the Record first', 'Prompt');
}

/**
 * @returns {Promise<boolean>} true if the event was consumed (handled or intentional no-op)
 */
export async function handleEQInspectionModuleButtons(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = String(buttonName || '').toUpperCase();
  if (!isEqInspectionButtonName(btn)) return false;

  const { FormGetField, ChangeCriteria, openScr, AskYesNoMessage } = devInterfaceObj || {};

  // ----- Inquiry filters (desktop ChangeCriteria; second call on VWRJCTDEQ wins — use status = 8 only)
  if (btn === 'EQINSPCTN_VWRJCTDEQ') {
    applyEqInquiryChangeCriteria(devInterfaceObj, 'WHERE (EQINSPCTN_INSPCTNSTTUS=8)');
    return true;
  }
  if (btn === 'EQINSPCTN_VWONHOLDEQ') {
    applyEqInquiryChangeCriteria(devInterfaceObj, 'WHERE (EQINSPCTN_INSPCTNSTTUS=9)');
    return true;
  }
  if (btn === 'EQINSPCTN_VWWRKEQ') {
    applyEqInquiryChangeCriteria(devInterfaceObj, 'WHERE (EQINSPCTN_INSPCTNSTTUS=10)');
    return true;
  }
  // Monthly filters: use EQINSPCTN_EQENTRYDT (APP JSON DataFieldName); desktop C++ used EQInspctn_EQEntryDT — same column, stable uppercase name for SQL
  if (btn === 'EQINSPCTN_VWMNTHSTPDEQDFCT') {
    applyEqInquiryChangeCriteria(
      devInterfaceObj,
      'WHERE (EQINSPCTN_INSPCTNSTTUS=11) AND MONTH(EQINSPCTN_EQENTRYDT) = MONTH(GETDATE())'
    );
    return true;
  }
  if (btn === 'EQINSPCTN_VWMNTHEXTEQDFCT') {
    applyEqInquiryChangeCriteria(
      devInterfaceObj,
      'WHERE (EQINSPCTN_INSPCTNSTTUS=18) AND MONTH(EQINSPCTN_EQENTRYDT) = MONTH(GETDATE())'
    );
    return true;
  }
  if (btn === 'EQINSPCTN_VWMNTHEXTEQJOBCNCLD') {
    applyEqInquiryChangeCriteria(
      devInterfaceObj,
      'WHERE (EQINSPCTN_INSPCTNSTTUS=19) AND MONTH(EQINSPCTN_EQENTRYDT) = MONTH(GETDATE())'
    );
    return true;
  }
  if (btn === 'EQINSPCTN_VWMNTHEXTEQJOBCMPLTD') {
    applyEqInquiryChangeCriteria(
      devInterfaceObj,
      'WHERE (EQINSPCTN_INSPCTNSTTUS=20) AND MONTH(EQINSPCTN_EQENTRYDT) = MONTH(GETDATE())'
    );
    return true;
  }
  if (btn === 'EQINSPCTN_VWALL') {
    applyEqInquiryChangeCriteria(devInterfaceObj, '');
    return true;
  }
  if (btn === 'EQINSPCTN_VWEQUNDRENTRY') {
    return true;
  }

  if (btn === 'EQINSPCTN_VWINSPCTNRQSTHSTRY') {
    if (!FormGetField || typeof openScr !== 'function') return true;
    const typ = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQTYP', 'scr') ?? '').trim();
    const srl = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQSRLN', 'scr') ?? '').trim();
    const plt = String(FormGetField('HSE_EQINSPCTN', 'EQINSPCTN_EQPLTN', 'scr') ?? '').trim();
    if (!typ || !srl) {
      void showEqInformationDialog(
        devInterfaceObj,
        'Select equipment type and serial before viewing inspection history.',
        'Prompt'
      );
      return true;
    }
    const et = escapeSqlString(typ);
    const es = escapeSqlString(srl);
    const ep = escapeSqlString(plt);
    let strSQL;
    if (plt !== '') {
      strSQL = `SELECT * FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '${et}') AND (EQINSPCTN_EQSRLN = '${es}') AND (EQINSPCTN_EQPLTN = '${ep}') AND (EQINSPCTN_INSPCTNSTTUS >= 5)`;
    } else {
      strSQL = `SELECT * FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '${et}') AND (EQINSPCTN_EQSRLN = '${es}') AND (EQINSPCTN_EQPLTN IS NULL) AND (EQINSPCTN_INSPCTNSTTUS >= 5)`;
    }
    const defValObj = {
      EQINSPCTN_EQTYP: typ,
      EQINSPCTN_EQSRLN: srl,
      EQINSPCTN_EQPLTN: plt,
    };
    openScr(EQ_INQ_SCREEN_TAG, {}, strSQL, 'list', false, defValObj, false, false);
    return true;
  }

  const key = getEQInspectionPrimaryKey(eventObj, devInterfaceObj, strScrTag);

  if (btn === 'EQINSPCTN_ENTRYCMPLTD') {
    if (!key) {
      promptSaveFirst(devInterfaceObj);
      return true;
    }
    const ek = escapeSqlString(key);
    return runTxnAndRefresh(devInterfaceObj, `EXECUTE completeEQInspctn '${ek}'`, 'Error completing EQ inspection');
  }

  if (btn === 'INSPCTNCHCKLST_RELOADCHCKLST') {
    if (!key) {
      promptSaveFirst(devInterfaceObj);
      return true;
    }
    const msg = 'Are you sure to re-load the checklist?';
    const confirmed = await confirmEqYesNo(AskYesNoMessage, msg);
    if (!confirmed) return true;
    const ek = escapeSqlString(key);
    const reason = escapeSqlString('Checklist is re-loaded');
    return runTxnAndRefresh(
      devInterfaceObj,
      `EXECUTE reldEQInspctnChckLst '${ek}','${reason}'`,
      'Error reloading inspection checklist'
    );
  }

  const transactional = {
    EQINSPCTN_DATAENTRYRJCTD: { sql: (ek) => `EXECUTE rjctEQInspctnDataEntry '${ek}'`, err: 'Error rejecting EQ data entry' },
    EQINSPCTN_EQINSPCTNRJCTD: { sql: (ek) => `EXECUTE rjctEQInspctn '${ek}'`, err: 'Error rejecting EQ inspection' },
    EQINSPCTN_EQHOLD: { sql: (ek) => `EXECUTE holdEQInspctn '${ek}'`, err: 'Error holding EQ inspection' },
    EQINSPCTN_EQACPTD: { sql: (ek) => `EXECUTE AcceptEQInspctn '${ek}'`, err: 'Error accepting EQ inspection' },
    EQINSPCTN_EQSTPDDFCTD: { sql: (ek) => `EXECUTE stopDefectedEQ '${ek}'`, err: 'Error stopping defective equipment' },
    EQINSPCTN_EQRSMWRK: { sql: (ek) => `EXECUTE resumeEQWrk '${ek}'`, err: 'Error resuming equipment work' },
    EQINSPCTN_EQEXTDFCTS: { sql: (ek) => `EXECUTE exitDefectedEQ '${ek}'`, err: 'Error exiting defective equipment' },
    EQINSPCTN_EQEXTWRKCNCLD: { sql: (ek) => `EXECUTE canclEQWrk '${ek}'`, err: 'Error cancelling equipment work' },
    EQINSPCTN_EQEXTWRKCMPLTD: { sql: (ek) => `EXECUTE cmpltEQWrk '${ek}'`, err: 'Error completing equipment work' },
  };

  const txn = transactional[btn];
  if (txn) {
    if (!key) {
      promptSaveFirst(devInterfaceObj);
      return true;
    }
    const ek = escapeSqlString(key);
    return runTxnAndRefresh(devInterfaceObj, txn.sql(ek), txn.err);
  }

  return false;
}
