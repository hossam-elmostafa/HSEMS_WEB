import { sendButtonClickToBackend } from '../services/ObservationService';

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
 */
// Note: Pending reject observation is now managed in ObservationService.js
// This keeps buttonEvents.js as a general dispatcher without business logic

export function toolBarButtonClicked(eventObj, callBackFn) {
  let { fullRecord, isNewMode, strBtnName, strScrTag, strTabTag, complete } = eventObj;
  strScrTag = strScrTag.toUpperCase();
  strBtnName = strBtnName.toUpperCase();
  
  // This is a generalized event handler - no business logic here
  // All business logic is handled in ObservationService.js via ButtonClicked event
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
      
      // Handle Observation module buttons with devInterface access
      // This is a generalized handler - business logic is in ObservationService.js
      sendButtonClickToBackend(normalizedButtonName, strScrTag, eventObj, devInterfaceObj);
    }

    // Any additional button-specific logic can go here
    // (Currently just logging for observation screens, but this is where you'd add business logic)
    
  } catch (error) {
    // Silently handle any errors - logging already happened above
    console.warn('[Web_HSE] Error in ButtonClicked:', error.message);
  }
}

