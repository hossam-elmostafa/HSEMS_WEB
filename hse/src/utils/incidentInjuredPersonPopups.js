/**
 * Desktop parity: OpenBodyParts / openInjuryType / openInjueryRsltScr / PPE popup for incident screens.
 * C++: HSEMSCommonCategory::OpenBodyParts, openInjuryType, openInjueryRsltScr; AccidentCategory::openPrsnlPrtctvEqpmntScr
 * RQ_HSE_13_3_26_4_18
 * Accident path: bLocked=true for all host screens (C++; HSE_TGACDNTENT unlock block commented out).
 */

import { isIncidentPreliminaryEntryScreenTag } from '../services/ModuleButtonHandlers/moduleButtonHandlersUtils.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/** Screen tags (match C++ ShowScreen / HSEObj menu map). */
export const SCREEN_TAGS = {
  BODY_PARTS: 'HSE_TGHMNBODYPART',
  INJURY_TYPE: 'HSE_TGINJRYTP',
  INJURY_ANALYSIS: 'HSE_TgInjryAnlysis',
  PPE: 'HSE_TGPRSNLPRTCTVEQPMNT',
};

/**
 * Read current injured-person line key from tab context (C++ FormGetField on subform row).
 * @param {Object} devInterfaceObj
 * @param {'accident'|'medical'} mode
 * @returns {string}
 */
export function getInjuredPersonLineKey(devInterfaceObj, mode) {
  const fg = devInterfaceObj?.FormGetField;
  if (typeof fg !== 'function') return '';
  if (mode === 'medical') {
    const v = fg('HSE_INCDNTMDCLRPRTINJRDPRSNS', 'INCDNTMDCLRPRTINJRDPRSNS_LNRNUM', 'tab');
    return v != null && v !== '' ? String(v).trim() : '';
  }
  const v = fg('HSE_ACDNTENTINJRDPRSN', 'ACDNTENTINJRDPRSN_LNRNUM', 'tab');
  return v != null && v !== '' ? String(v).trim() : '';
}

/**
 * @param {Object} devInterfaceObj
 * @param {'accident'|'medical'} mode
 * @returns {string} Main accident number or empty
 */
function getAccidentMainKey(devInterfaceObj) {
  const fg = devInterfaceObj?.FormGetField;
  if (typeof fg !== 'function') return '';
  const v = fg('HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM', 'scr');
  return v != null && v !== '' ? String(v).trim() : '';
}

/**
 * C++ AccidentEntryCategory: open body parts only if injured-person tab has rows for this accident.
 * @param {Object} devInterfaceObj
 * @param {string} mainAcdntNum
 * @returns {Promise<boolean>}
 */
async function accidentInjuredTabHasRows(devInterfaceObj, mainAcdntNum) {
  if (!mainAcdntNum || !devInterfaceObj?.executeSQLPromise) return false;
  const ek = escapeSqlString(mainAcdntNum);
  try {
    const rs = await devInterfaceObj.executeSQLPromise(
      `SELECT COUNT(0) AS CNT FROM HSE_ACDNTENTINJRDPRSN WHERE ACDNTENTINJRDPRSN_LNK = '${ek}'`
    );
    const row = rs?.recordset?.[0] ?? rs?.[0]?.recordset?.[0] ?? rs?.[0];
    const n = parseInt(row?.CNT ?? row?.cnt ?? Object.values(row || {})[0], 10) || 0;
    return n > 0;
  } catch (e) {
    console.warn('[Web_HSE] incidentInjuredPersonPopups accidentInjuredTabHasRows:', e);
    return false;
  }
}

/**
 * Open human body parts popup (HSE_HMNBODYPART).
 * @param {boolean} lockScreen - C++ bLocked (medical entry uses false per BGC; accident often locked in current DLL)
 */
export function openBodyPartsForIncident(devInterfaceObj, strScrTag, mode, lockScreen = true) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return false;
  const lineKey = getInjuredPersonLineKey(devInterfaceObj, mode);
  if (!lineKey) {
    if (devInterfaceObj?.AskYesNoMessage) {
      devInterfaceObj.AskYesNoMessage('Select an injured person line or add a row on the Injured Persons tab.');
    }
    return false;
  }
  const esc = escapeSqlString(lineKey);
  const criteria = `SELECT * FROM HSE_HMNBODYPART WHERE (HMNBODYPART_LNK = '${esc}')`;
  const defValObj = { HMNBODYPART_LNK: lineKey };
  openScr(SCREEN_TAGS.BODY_PARTS, {}, criteria, 'edit', false, defValObj, false, lockScreen);
  return true;
}

export function openInjuryTypeForIncident(devInterfaceObj, strScrTag, mode, lockScreen = true) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return false;
  const lineKey = getInjuredPersonLineKey(devInterfaceObj, mode);
  if (!lineKey) {
    if (devInterfaceObj?.AskYesNoMessage) {
      devInterfaceObj.AskYesNoMessage('Select an injured person line or add a row on the Injured Persons tab.');
    }
    return false;
  }
  const esc = escapeSqlString(lineKey);
  const criteria = `SELECT * FROM HSE_INJRYTP WHERE (INJRYTP_LNK = '${esc}')`;
  const defValObj = { INJRYTP_LNK: lineKey };
  openScr(SCREEN_TAGS.INJURY_TYPE, {}, criteria, 'edit', false, defValObj, false, lockScreen);
  return true;
}

/**
 * C++ openInjueryRsltScr: medical entry/follow-up unlocked; accident path locked (default bLocked=true).
 */
export function openInjuryAnalysisForIncident(devInterfaceObj, strScrTag, mode) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return false;
  const lineKey = getInjuredPersonLineKey(devInterfaceObj, mode);
  if (!lineKey) {
    if (devInterfaceObj?.AskYesNoMessage) {
      devInterfaceObj.AskYesNoMessage('Select an injured person line or add a row on the Injured Persons tab.');
    }
    return false;
  }
  const scr = (strScrTag || '').toUpperCase();
  let bLockScr = true;
  if (mode === 'medical' && (scr === 'HSE_TGINCDNTMDCLRPRT' || scr === 'HSE_TGINCDNTMDCLRPRTFLWUP')) {
    bLockScr = false;
  }
  const esc = escapeSqlString(lineKey);
  const criteria = `SELECT * FROM HSE_InjryAnlysis WHERE (InjryAnlysis_LINK = '${esc}')`;
  const defValObj = { InjryAnlysis_LINK: lineKey };
  openScr(SCREEN_TAGS.INJURY_ANALYSIS, {}, criteria, 'edit', false, defValObj, false, bLockScr);
  return true;
}

/** C++ AccidentCategory::openPrsnlPrtctvEqpmntScr — uses injured-person line key. */
export function openPpeForAccident(devInterfaceObj, lockScreen = true) {
  const openScr = devInterfaceObj?.openScr;
  if (typeof openScr !== 'function') return false;
  const lineKey = getInjuredPersonLineKey(devInterfaceObj, 'accident');
  if (!lineKey) {
    if (devInterfaceObj?.AskYesNoMessage) {
      devInterfaceObj.AskYesNoMessage('Select an injured person line or add a row on the Injured Persons tab.');
    }
    return false;
  }
  const esc = escapeSqlString(lineKey);
  const criteria = `SELECT * FROM HSE_PRSNLPRTCTVEQPMNT WHERE (PRSNLPRTCTVEQPMNT_LINK = '${esc}')`;
  const defValObj = { PRSNLPRTCTVEQPMNT_LINK: lineKey };
  openScr(SCREEN_TAGS.PPE, {}, criteria, 'edit', false, defValObj, false, lockScreen);
  return true;
}

/**
 * Route custom buttons for preliminary incident (entry / review / follow-up) and investigation entry if needed.
 * @returns {boolean} true if handled
 */
export async function handleAccidentInjuredPersonCustomButton(devInterfaceObj, buttonName, strScrTag) {
  const btn = (buttonName || '').toUpperCase();
  const scr = (strScrTag || '').toUpperCase();
  const isEntryLike =
    isIncidentPreliminaryEntryScreenTag(strScrTag) || scr === 'HSE_TGINCDNTINVSTGTNENTRY';
  const accidentPopupLocked = true;

  if (btn === 'ACDNTENTINJRDPRSN_INJRDBODYPART') {
    if (isEntryLike) {
      const mainKey = getAccidentMainKey(devInterfaceObj);
      const hasRows = await accidentInjuredTabHasRows(devInterfaceObj, mainKey);
      if (!hasRows) {
        if (devInterfaceObj?.toast?.warning) {
          devInterfaceObj.toast.warning('Add at least one injured person before opening body parts.');
        } else if (devInterfaceObj?.AskYesNoMessage) {
          devInterfaceObj.AskYesNoMessage('Add at least one injured person before opening body parts.');
        }
        return true;
      }
    }
    openBodyPartsForIncident(devInterfaceObj, strScrTag, 'accident', accidentPopupLocked);
    return true;
  }
  if (btn === 'INJRYTYP') {
    openInjuryTypeForIncident(devInterfaceObj, strScrTag, 'accident', accidentPopupLocked);
    return true;
  }
  if (btn === 'INJURY_ANALYSIS') {
    openInjuryAnalysisForIncident(devInterfaceObj, strScrTag, 'accident');
    return true;
  }
  if (btn === 'PRSNL_PRTCTVE_EQUIP') {
    openPpeForAccident(devInterfaceObj, accidentPopupLocked);
    return true;
  }
  return false;
}

/**
 * Medical report entry / follow-up injured-person buttons.
 * @returns {boolean} true if handled
 */
export function handleMedicalInjuredPersonCustomButton(devInterfaceObj, buttonName, strScrTag) {
  const btn = (buttonName || '').toUpperCase();
  if (
    btn !== 'INCDNTMDCLRPRTINJRDPRSNS_INJRDBODYPART' &&
    btn !== 'INCDNTMDCLRPRTINJRDPRSNS_INJRYTYP' &&
    btn !== 'INCDNTMDCLRPRTINJRDPRSNS_INJRYCTGRY'
  ) {
    return false;
  }
  const scr = (strScrTag || '').toUpperCase();
  const medicalUnlocked = scr === 'HSE_TGINCDNTMDCLRPRT' || scr === 'HSE_TGINCDNTMDCLRPRTFLWUP';
  if (btn === 'INCDNTMDCLRPRTINJRDPRSNS_INJRDBODYPART') {
    openBodyPartsForIncident(devInterfaceObj, strScrTag, 'medical', !medicalUnlocked);
    return true;
  }
  if (btn === 'INCDNTMDCLRPRTINJRDPRSNS_INJRYTYP') {
    openInjuryTypeForIncident(devInterfaceObj, strScrTag, 'medical', !medicalUnlocked);
    return true;
  }
  if (btn === 'INCDNTMDCLRPRTINJRDPRSNS_INJRYCTGRY') {
    openInjuryAnalysisForIncident(devInterfaceObj, strScrTag, 'medical');
    return true;
  }
  return false;
}
