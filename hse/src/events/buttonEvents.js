import { sendButtonClickToBackend, isObservationTabsEnabled, manageObservationTabs, manageCommentsTabToolBar } from '../services/Observation service/ObservationService';
import { OBSERVATION_SCREEN_TAGS } from '../config/constants';
import { getScreenHandler } from '../screenHandlers/index';
import { handleModuleButton } from '../services/ModuleButtonHandlers/index.js';

// Module-level variable to store devInterface functions
let devInterfaceObj = {};

/**
 * Map of screen tags that are currently in "new" mode (C++ m_ScreensInNewMode).
 * Keys: screen tag (e.g. strScrTag), values: "true".
 * Use setbNewMode() to update; read via ScreensInNewMode[strScrTag] or hasScreenInNewMode(strScrTag).
 */
export const ScreensInNewMode = Object.create(null);

/**
 * Set devInterface functions for ButtonClicked handler
 * @param {Object} devInterface - Object containing devInterface functions
 */
export function setDevInterface(devInterface) {
  devInterfaceObj = devInterface || {};
}

/**
 * Read-only access to the dev interface (SQL, messages, etc.) for menu events and screen handlers.
 * Set by `hse.js` via `setDevInterface` after app init.
 * @returns {Object}
 */
export function getDevInterface() {
  return devInterfaceObj;
}

/**
 * Mark a screen as in new mode or not (C++ setbNewMode / m_ScreensInNewMode).
 * @param {string} strFormTag - Screen tag (header form tag)
 * @param {boolean} isNewMode - true to set, false to clear
 */
function setbNewMode(strFormTag, isNewMode) {
  if (isNewMode) {
    ScreensInNewMode[strFormTag] = 'true';
  } else {
    delete ScreensInNewMode[strFormTag];
  }
}

/**
 * Check if a screen is in new mode.
 * @param {string} strScrTag - Screen tag
 * @returns {boolean}
 */
export function hasScreenInNewMode(strScrTag) {
  return strScrTag != null && ScreensInNewMode[strScrTag] === 'true';
}

/** Screen tags that require GenerateTXNNo on SAVE when record was in new mode (C++ list). */
const SCREEN_TAGS_REQUIRING_TXN_NO = [
  'HSE_TGRSKASSMNTENT',
  'HSE_TGPERFMSRMNTENT',
  'HSE_TGACDNTENT',
  'HSE_TGNRSTMISCENT',
  'HSE_TGPTWSRGSTRENT',
  'HSE_TGCRCTVEACCENT',
  'HSE_TGSITSRVYENT',
  'HSE_TGINCDNTFLSHRPRT'
];

/**
 * Get key field value for TXN number generation (C++ getTXNKeyFldVal).
 * @param {string} strFormTag - Screen/form tag
 * @returns {string} Key field value or ''
 */
function getTXNKeyFldVal(strFormTag) {
  const { FormGetField } = devInterfaceObj;
  if (!FormGetField || !strFormTag) return '';
  const tag = strFormTag.toUpperCase();
  const map = {
    'HSE_TGRSKASSMNTENT': { table: 'HSE_RSKASSMNTENT', keyFld: 'RSKASSMNTENT_ASSMNTNO' },
    'HSE_TGPTNLHZRDENT': { table: 'HSE_PTNLHZRDENT', keyFld: 'PTNLHZRDENT_PTNLHZRDNO' },
    'HSE_TGPERFMSRMNTENT': { table: 'HSE_PERFMSRMNTENT', keyFld: 'PERFMSRMNTENT_VSTNO' },
    'HSE_TGACDNTENT': { table: 'HSE_ACDNTENT', keyFld: 'ACDNTENT_ACDNTNUM' },
    'HSE_TGPTWSRGSTRENT': { table: 'HSE_PTWSRGSTRENT', keyFld: 'PTWSRGSTRENT_PTWSNUM' },
    'HSE_TGCRCTVEACCENT': { table: 'HSE_CRCTVEACCENT', keyFld: 'CRCTVEACCENT_CARMODELNUM' },
    'HSE_TGNRSTMISCENT': { table: 'HSE_vwNRSTMISCENT', keyFld: 'NRSTMISCENT_NRSTMISCNUM' },
    'HSE_TGSITSRVYENT': { table: 'HSE_SITSRVYENT', keyFld: 'SITSRVYENT_SITSRVYNO' },
    'HSE_TGINCDNTFLSHRPRT': { table: 'HSE_INCDNTFLSHRPRT', keyFld: 'MAINLINK' }
  };
  const entry = map[tag];
  if (!entry) return '';
  const val = FormGetField(entry.table, entry.keyFld,'scr');
  return val != null ? String(val) : '';
}

/**
 * Generate TXN number for a screen after save (C++ GenerateTXNNo).
 * Executes generateNewTXNNum stored procedure then refreshes screen.
 * @param {string} strFormTag - Screen tag
 */
async function generateTXNNo(strFormTag) {
  const strKeyFldVal = getTXNKeyFldVal(strFormTag);
  if (!strFormTag || !strKeyFldVal) return;
  const { executeSQLPromise, refreshData } = devInterfaceObj;
  if (!executeSQLPromise) return;
  try {
    const strSQL = `EXECUTE generateNewTXNNum '${strFormTag}',${strKeyFldVal}`;
    await executeSQLPromise(strSQL);
    if (typeof refreshData === 'function') refreshData('');
  } catch (err) {
    console.warn('[Web_HSE] GenerateTXNNo failed:', err);
  }
}
/**
 * toolBarButtonClicked
 * @param {Object} eventObj 
 * @param {Function} callBackFn 
 * 
 * event Object Structure 
 * { fullRecord,          // full record object {field_Name:field_value,field_Name:field_value,...}
 *   isNewMode,           // boolean true/false
 *   strBtnName,          // strBtnName -> new,delete,save...
 *   strScrTag,           // header screen tag
 *   strTabTag,           // tab tag,if screen is detail else strTabTag = ''
 *   complete }           // complete 1,0 0 -> before event , 1-> after event
 * 
 * This event MUST call callBackFn after finish it's work
 * 
 * RQ_HSM_22_12_10_59: Implement Investigation Team Tab
 * RQ_HSM_22_12_11_13: Implement Comments Tab
 * 
 * C++: NearMissEntryCategory::DisplayRecordRepostion() when iComplete==1
 */
export async function toolBarButtonClicked(eventObj, callBackFn) {
  let { fullRecord, isNewMode, strBtnName, strScrTag, strTabTag, complete } = eventObj;
  strScrTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  strBtnName = strBtnName ? strBtnName.toString().toUpperCase() : '';
  strTabTag = strTabTag ? strTabTag.toString().toUpperCase() : '';

  const screenHandler = getScreenHandler(strScrTag);
  let observationTabsHandledByHandler = false;
  if (screenHandler && typeof screenHandler.toolBarButtonClicked === 'function') {
    eventObj.devInterfaceObj = devInterfaceObj;
    await screenHandler.toolBarButtonClicked(eventObj, callBackFn);
    observationTabsHandledByHandler = true;
  }

  try {
    // C++: if(strBtnName=="SAVE" && iComplete==1 && strSubFormTag=="") { getbNewMode(strFormTag); if(bNewRec) GenerateTXNNo(strFormTag); }
    if (strBtnName === 'SAVE' && complete === 1 && strTabTag === '') {
      const bNewRec = hasScreenInNewMode(strScrTag);
      if (bNewRec && SCREEN_TAGS_REQUIRING_TXN_NO.includes(strScrTag)) {
        await generateTXNNo(strScrTag);
      }
      setbNewMode(strScrTag, false);
    }

    // C++: if(pRecordRepostion->iComplete==1) - handle tab enabling after save
    // complete: 0 -> before event, 1 -> after event
    if (complete === 1) {
      if (strBtnName === 'NEW' && complete === 1 && strTabTag === '') {
        setbNewMode(strScrTag, true);
      }
    
      // Check if this is an Observation screen (skip if handler already ran observation tab logic)
      const isObservationScreen = OBSERVATION_SCREEN_TAGS.some(tag => 
        strScrTag.includes(tag.toUpperCase())
      ) || strScrTag.includes('NRSTMISC') || strScrTag.includes('NERMSENT');
      
      if (isObservationScreen && !observationTabsHandledByHandler) {
        const { executeSQLPromise, TabEnable } = devInterfaceObj;
        
        if (executeSQLPromise && TabEnable) {
          // C++: bool bObservationTabsEnabled = isObservationTabsEnabled();
          const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterfaceObj.getValFromRecordSet);
          
          // C++: EnableTab("HSE_TgNrstMiscEntinvstgtntems", true/false);
          // C++: EnableTab("HSE_TGNERMSENTCMNTS", true/false);
          manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
        }
      }
      
      // C++: if(strSubFormTag.Find("CMNT") != -1 && pRecordRepostion->iComplete == 1)
      // Manage Comments tab toolbar when navigating after save
      if (strTabTag && strTabTag.includes('CMNTS') && !observationTabsHandledByHandler) {
        manageCommentsTabToolBar(strScrTag, strTabTag, devInterfaceObj);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Error in toolBarButtonClicked tab management:', error);
  }
  
  // Call callback to continue normal flow (handler already called it when observationTabsHandledByHandler)
  if (!observationTabsHandledByHandler) {
    callBackFn(eventObj);
  }
}

/**
 * ButtonClicked
 * @param {*} eventObj 
 * 
 * event Object Structure 
 * {
 *    Button_Name,                // custom button name
 *    fullRecord: fullRecordArr,  // array of full records which selected when user click on custom button [{field_Name:field_value,field_Name:field_value,...},{field_Name:field_value,field_Name:field_value,...},...]
 *    lOwnerApp,                  // Not used Please Ignore it for now
 *    lOwnerTransAction,          // Not used Please Ignore it for now
 *    nRecordStatus,              // Not used Please Ignore it for now
 *    strScrTag,                  // header screen tag
 *    strTabTag,                  // tab tag,if screen is detail else strTabTag = ''
 *    strTblNam                   // table name
 *    fullRecordArrKeys           // array of keys of selected records [key_value_1,key_value_2,...]
 * }
 * 
 * This event doesn't require callback
 */
export async function ButtonClicked(eventObj) {
  try {
    // Extract event data
    let {
      Button_Name,
      fullRecord: fullRecordArr,
      lOwnerApp,
      lOwnerTransAction,
      nRecordStatus,
      strScrTag,
      strTabTag,
      strTblNam,
      fullRecordArrKeys,
    } = eventObj || {};

    eventObj.devInterfaceObj = devInterfaceObj;

    const normalizedStrScrTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
    const normalizedButtonName = Button_Name ? Button_Name.toString().toUpperCase() : '';

    // 1) Try module button handlers (Risk Assessment, Site Survey, PTW, Incident, Audit, Performance Measurement, Rescue Plan — RQ_HSE_23_3_26_17_05: cancel / return / CMS_RSNS)
    if (normalizedButtonName && normalizedStrScrTag) {
      const moduleHandled = await handleModuleButton(normalizedButtonName, normalizedStrScrTag, eventObj, devInterfaceObj);
      if (moduleHandled) return;
    }

    // 2) Try screen-specific handler (CAR, Observation, Drill Plan, etc.)
    const screenHandler = getScreenHandler(strScrTag);
    if (screenHandler && typeof screenHandler.ButtonClicked === 'function') {
      await screenHandler.ButtonClicked(eventObj);
      return;
    }

    // 3) Fallback: Observation module and logging
    if (normalizedButtonName) {
      sendButtonClickToBackend(normalizedButtonName, normalizedStrScrTag || strScrTag, eventObj, devInterfaceObj);
    }
  } catch (error) {
    console.warn('[Web_HSE] Error in ButtonClicked:', error.message);
  }
}

