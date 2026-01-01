import { sendButtonClickToBackend as sendObservationButtonClick, isObservationTabsEnabled, manageObservationTabs, manageCommentsTabToolBar } from '../services/Observation service/ObservationService';
import { sendButtonClickToBackend as sendCARButtonClick } from '../services/CAR service/CARService';
import { handleIncidentButtonClick } from '../services/Incident service/IncidentService';
import { handleSiteSurveyButtonClick } from '../services/Site Survey service/SiteSurveyService';
import { OBSERVATION_SCREEN_TAGS } from '../config/constants';

// Module-level variable to store devInterface functions
let devInterfaceObj = {};

/**
 * Set devInterface functions for ButtonClicked handler
 * @param {Object} devInterface - Object containing devInterface functions
 */
export function setDevInterface(devInterface) {
  devInterfaceObj = devInterface || {};
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
  
  try {
    // C++: if(pRecordRepostion->iComplete==1) - handle tab enabling after save
    // complete: 0 -> before event, 1 -> after event
    if (complete === 1) {
      // Check if this is an Observation screen
      // Use constants and also check for common observation screen patterns
      const isObservationScreen = OBSERVATION_SCREEN_TAGS.some(tag => 
        strScrTag.includes(tag.toUpperCase())
      ) || strScrTag.includes('NRSTMISC') || strScrTag.includes('NERMSENT');
      
      if (isObservationScreen) {
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
      if (strTabTag && strTabTag.includes('CMNTS')) {
        manageCommentsTabToolBar(strScrTag, strTabTag, devInterfaceObj);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Error in toolBarButtonClicked tab management:', error);
  }
  
  // Always call callback to continue normal flow
  callBackFn(eventObj);
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
export function ButtonClicked(eventObj) {
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
    
    // Normalize screen tag
    if (strScrTag) {
      strScrTag = strScrTag.toString().toUpperCase();
    }
    
    // Extract and normalize button name
    let normalizedButtonName = '';
    if (Button_Name) {
      normalizedButtonName = Button_Name.toString().toUpperCase();
      
      // Handle CAR module buttons first (if applicable)
      // This is a generalized handler - business logic is in CARService.js
      sendCARButtonClick(normalizedButtonName, strScrTag, eventObj, devInterfaceObj);
      
      // Handle Observation module buttons with devInterface access
      // This is a generalized handler - business logic is in ObservationService.js
      sendObservationButtonClick(normalizedButtonName, strScrTag, eventObj, devInterfaceObj);
      
      // Handle Incident module buttons with devInterface access
      // This is a generalized handler - business logic is in IncidentService.js
      handleIncidentButtonClick(normalizedButtonName, strScrTag, eventObj, devInterfaceObj);
      
      // Handle Site Survey module buttons with devInterface access
      // This is a generalized handler - business logic is in SiteSurveyService.js
      handleSiteSurveyButtonClick(normalizedButtonName, strScrTag, eventObj, devInterfaceObj);
    }

    // Any additional button-specific logic can go here
    // (Currently just logging for observation screens, but this is where you'd add business logic)
    
  } catch (error) {
    // Silently handle any errors - logging already happened above
    console.warn('[Web_HSE] Error in ButtonClicked:', error.message);
  }
}

