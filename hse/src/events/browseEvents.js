/**
 * OnBeforeBrowseButtonClick
 * @param {string} strScrTag header screen tag 
 * @param {string} strTabTag tab tag if screen is tab else this will be ''
 * @param {object} brwsObj 
 * brwsObj structure :
 * {
 *    isOpen,                 // true/false open browse dialog or not,don't change it unless you want tp prevent browse dialog from open!
 *    fieldObj                // Browse field object,will be described in details below  
 *    BrowseTag               // Browse tag which will be opened,you can change it if you want to open another screen based on some conditions in your business
 *    BrowseData              //
 *    BrowsingTable           // Table of Browse screen
 *    scrType                 // 
 *    BrowsingSQL             // Criteria for opened browsing screen,you can change it based on your new criteria
 *    fieldDBType             // Database type of field
 *    isfilterFld             // true/false if field browse from advanced filter then this will be true,else it will be false,Please don't change this.
 *    bAllowMultiSelection    // true/false enable or disable multi-selection in browse
 * }
 * 
 * @param {object} fullRecord full record object "full record of caller screen" {field_Name:field_value,field_Name:field_value,...}
 * @returns {Promise} this function always return promise with browsObj,You can keep browsObj as it or change any attribute and return it.
 */
export function OnBeforeBrowseButtonClick(strScrTag, strTabTag, brwsObj, fullRecord) {
  return new Promise((resolve, reject) => {
    strScrTag = strScrTag.toString().toUpperCase();   
    resolve(brwsObj);
  });
}

/**
 * UpdateAfterBrowse
 * @param {string} strScrTag              // header screen tag
 * @param {object} selectedBrowseRow      // return row fro browse screen {field_Name:field_value,field_Name:field_value,...}
 * @param {string} strBrowsingTable       // browse table name
 * @param {string} strFldVal              // value for return field from browse 
 * @param {string} strBrowsingField       // Then name of browsing field
 * @param {object} updatedAppGlobalVal    // Object 'Ignore this for now' 
 * 
 * This event doesn't require callback
 */
export function UpdateAfterBrowse(
  strScrTag,
  strTabTag,
  selectedBrowseRow,
  strBrowsingTable,
  strOldFldVal,
  strFldVal,
  strBrowsingField,
  updatedAppGlobalVal
) {
  // Empty implementation - can be extended later
}





