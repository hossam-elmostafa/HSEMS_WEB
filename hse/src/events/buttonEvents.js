import { sendButtonClickToBackend } from '../services/ObservationService';

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
export function toolBarButtonClicked(eventObj, callBackFn) {
  let { fullRecord, isNewMode, strBtnName, strScrTag, strTabTag, complete } = eventObj;
  strScrTag = strScrTag.toUpperCase();
  strBtnName = strBtnName.toUpperCase();
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
      
      // Only log to backend for Observation module screens
      // Pass screen tag to check if it's an observation screen
      sendButtonClickToBackend(normalizedButtonName, strScrTag);
    }

    // Any additional button-specific logic can go here
    // (Currently just logging for observation screens, but this is where you'd add business logic)
    
  } catch (error) {
    // Silently handle any errors - logging already happened above
    console.warn('[Web_HSE] Error in ButtonClicked:', error.message);
  }
}

