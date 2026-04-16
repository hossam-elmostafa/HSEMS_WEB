/**
 * RQ_HSE_12_4_26_00_40 — GAP-5: Corrective action popup button handlers.
 * C++: CCorrectiveActionEntry::DisplayCustomButtonClicked.
 * Buttons: REJECT_ACTION (status 4), ACCEPT_ACTION (status 5), KEEP_ACTION_PENDING (status 6),
 *          ACTION_COMPLETED (status 8), VIEW_SOURCE_CAR.
 *
 * GAP-6 / GAP-14: Field-change rules for CRRCTVACTNS_ACC and CRRCTVACTNS_ACTACC.
 */

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

function getAutoFieldNum(eventObj) {
  return getFieldFromRecord(eventObj, 'AutoFieldNum') || getFieldFromRecord(eventObj, 'AUTOFIELDNUM');
}

/** RQ_HSE_12_4_26_00_40 — GAP-12: must match defValObj key in carCustomButtons.openCorrectiveActionsPopup */
const CORRECTIVE_POPUP_SNAPSHOT_CAR_DATE_FIELD = 'CRENTRY_CRDT';

/**
 * RQ_HSE_12_4_26_00_40 — GAP-12: Prefer CAR date injected via openScr defValObj (popup context), then parent forms.
 */
function getEffectiveCarentryCrDateForRvstrgdt(devInterfaceObj, strScrTag) {
  const FormGetField = devInterfaceObj?.FormGetField;
  const tabTag = String(strScrTag || '').trim();
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
  return (
    tryGet(tabTag, 'TAB') ||
    tryGet(tabTag, 'SCR') ||
    tryGet('HSE_CRRCTVACTNS', 'TAB') ||
    tryGet('HSE_CRRCTVACTNS', 'SCR') ||
    tryGet('HSE_CRENTRY', 'SCR') ||
    tryGet('HSE_TgCrEntry', 'SCR') ||
    ''
  );
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-5: Handle corrective action popup buttons.
 * @returns {Promise<boolean>} true if button was consumed
 */
export async function handleCorrectiveActionPopupButton(buttonName, strScrTag, eventObj, devInterfaceObj) {
  const btn = (buttonName && String(buttonName).toUpperCase()) || '';
  const scr = (strScrTag && String(strScrTag).toUpperCase()) || '';

  const isPopup = scr.includes('CRRCTVACTNS');
  if (!isPopup) return false;

  const { executeSQLPromise, FormSetField, FormGetField, doToolbarAction, refreshData, AskYesNoMessage, openScr } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;

  const autoFieldNum = getAutoFieldNum(eventObj);
  const tabTag = String(eventObj?.strTabTag || scr || '').trim();

  if (btn === 'REJECT_ACTION') {
    const rejectRsn = getFieldFromRecord(eventObj, 'CRRCTVACTNS_RJCRSN');
    if (!rejectRsn) {
      if (AskYesNoMessage) AskYesNoMessage('Please enter reject reason (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    if (autoFieldNum && typeof FormSetField === 'function') {
      try {
        FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', '4', 'tab');
        if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', tabTag, '');
        if (typeof refreshData === 'function') refreshData('');
      } catch (e) {
        console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 REJECT_ACTION:', e);
      }
    }
    return true;
  }

  if (btn === 'ACCEPT_ACTION') {
    if (autoFieldNum && typeof FormSetField === 'function') {
      try {
        FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', '5', 'tab');
        // RQ_HSE_12_4_26_00_40 — auto-set accepted date
        const { getCarServerDate } = await import('./carServerDate.js');
        const dt = await getCarServerDate(executeSQLPromise, devInterfaceObj?.getCurrentFormatedDate);
        if (dt) FormSetField(tabTag, 'CRRCTVACTNS_ACPTDDT', dt, 'tab');
        if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', tabTag, '');
        if (typeof refreshData === 'function') refreshData('');
      } catch (e) {
        console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 ACCEPT_ACTION:', e);
      }
    }
    return true;
  }

  if (btn === 'KEEP_ACTION_PENDING') {
    const pendRsn = getFieldFromRecord(eventObj, 'CRRCTVACTNS_PNDRSN');
    if (!pendRsn) {
      if (AskYesNoMessage) AskYesNoMessage('Please enter pending reason (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    if (autoFieldNum && typeof FormSetField === 'function') {
      try {
        FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', '6', 'tab');
        if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', tabTag, '');
        if (typeof refreshData === 'function') refreshData('');
      } catch (e) {
        console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 KEEP_ACTION_PENDING:', e);
      }
    }
    return true;
  }

  if (btn === 'ACTION_COMPLETED') {
    const cmpDt = getFieldFromRecord(eventObj, 'CRRCTVACTNS_CMPDT');
    const excBy = getFieldFromRecord(eventObj, 'CRRCTVACTNS_EXCBY');
    if (!cmpDt) {
      if (AskYesNoMessage) AskYesNoMessage('Please enter Completion Date (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    if (!excBy) {
      if (AskYesNoMessage) AskYesNoMessage('Please enter Executed By (RQ_HSE_12_4_26_00_40)');
      return true;
    }
    if (autoFieldNum && typeof FormSetField === 'function') {
      try {
        FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', '8', 'tab');
        if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', tabTag, '');
        if (typeof refreshData === 'function') refreshData('');
      } catch (e) {
        console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 ACTION_COMPLETED:', e);
      }
    }
    return true;
  }

  if (btn === 'VIEW_SOURCE_CAR') {
    if (typeof openScr === 'function') {
      const lnk = getFieldFromRecord(eventObj, 'CRRCTVACTNS_LNKWTHMN');
      if (lnk) {
        const criteria = `SELECT * FROM HSE_CRENTRY WHERE PrmKy IN (SELECT PrmKy FROM HSE_ACTNSENTRY_RTCSS WHERE AutoFieldNum = '${escapeSqlString(lnk)}')`;
        openScr('HSE_TgCrEntry', {}, criteria, 'list', false, {}, false, false);
      }
    }
    return true;
  }

  return false;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-6 / GAP-14: Field-change rules for corrective actions popup.
 * Review variant: CRRCTVACTNS_ACC → ACTSTT 3 (accepted) or 2 (rejected).
 * Follow-up variant: CRRCTVACTNS_ACTACC → ACTSTT 9 (accepted) or 7 (not accepted).
 */
/** RQ_HSE_12_4_26_00_40 — GAP-14: carCustomButtons opens Approval/Editing with this tag (not _Rvw / _ActEnt / _FlwUp). */
const GENERIC_CORRECTIVE_ACTIONS_POPUP_TAG = 'HSE_TGCRRCTVACTNS';

/**
 * RQ_HSE_12_4_26_00_40 — GAP-14: Same ACTSTT mapping as Review when CRRCTVACTNS_ACC changes; exclude Follow-up (uses ACTACC).
 */
function shouldApplyReviewAccToActstt(scrUpper) {
  const u = String(scrUpper || '').toUpperCase();
  if (u.includes('FLWUP')) return false;
  if (u.includes('RVW')) return true;
  if (u === GENERIC_CORRECTIVE_ACTIONS_POPUP_TAG) return true;
  return false;
}

export function applyCorrectiveActionPopupFieldChange(devInterfaceObj, strScrTag, fieldName, fieldVal) {
  const fld = String(fieldName || '').toUpperCase();
  const scr = String(strScrTag || '').toUpperCase();
  const { FormSetField } = devInterfaceObj || {};
  if (typeof FormSetField !== 'function') return;

  const tabTag = strScrTag || '';

  // RQ_HSE_12_4_26_00_40 — GAP-6 / GAP-14: Review-style ACC → ACTSTT (includes generic HSE_TGCRRCTVACTNS)
  if (fld === 'CRRCTVACTNS_ACC' && shouldApplyReviewAccToActstt(scr)) {
    const v = String(fieldVal ?? '').trim().toUpperCase();
    try {
      FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', v === 'Y' ? '3' : '2', 'tab');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-14 CRRCTVACTNS_ACC:', e);
    }
  }

  // RQ_HSE_12_4_26_00_40 — GAP-6: Follow-up variant
  if (fld === 'CRRCTVACTNS_ACTACC' && scr.includes('FLWUP')) {
    const v = String(fieldVal ?? '').trim().toUpperCase();
    try {
      FormSetField(tabTag, 'CRRCTVACTNS_ACTSTT', v === 'Y' ? '9' : '7', 'tab');
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-6 CRRCTVACTNS_ACTACC:', e);
    }
  }

  // RQ_HSE_12_4_26_00_40 — GAP-12: revised target date >= CAR open date (desktop DisplayFieldChange; parent + defVal snapshot)
  if (fld === 'CRRCTVACTNS_RVSTRGDT') {
    const newDate = fieldVal;
    if (newDate) {
      try {
        const carDate = getEffectiveCarentryCrDateForRvstrgdt(devInterfaceObj, tabTag);
        if (carDate && new Date(newDate) < new Date(carDate)) {
          if (devInterfaceObj.AskYesNoMessage) {
            devInterfaceObj.AskYesNoMessage(`Entered date must be >= CAR Date: [${carDate}] (RQ_HSE_12_4_26_00_40)`);
          }
        }
      } catch (_) {
        /* best-effort */
      }
    }
  }
}
