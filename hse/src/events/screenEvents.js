import { manageCommentsTabToolBar } from '../services/TabManagementService';

// Module-level variable to store devInterface functions
let devInterfaceObj = {};

/**
 * Set devInterface functions for screen event handlers
 * @param {Object} devInterface - Object containing devInterface functions
 */
export function setDevInterface(devInterface) {
  devInterfaceObj = devInterface || {};
}

/**
 * @name SubFieldChanged
 * @description this event called when you change field in screen or tab
 * @param {string} strScrTag header screen tag
 * @param {string} strTabTag tab tag if changed field in tab
 * @param {string} fieldName changed field name   
 * @param {string} oldFieldVal old field value
 * @param {string} fieldVal new field value
 * @param {object} updatedAppGlobalVal object Please ignore it for now
 * 
 * This event doesn't require callback
 */
export function SubFieldChanged(strScrTag, strTabTag, fieldName, oldFieldVal, fieldVal, updatedAppGlobalVal) {
  // Empty implementation - can be extended later
}

/**
 * @name MainSubReposition
 * @description this event is called when navigating between rows from screen or tab
 * C++: DisplayRecordRepostion() - handles Comments tab management when navigating
 * 
 * RQ_HSM_22_12_11_13: Implement Comments Tab
 * ClickUp Task: https://app.clickup.com/t/86c76vt58 (or create new task)
 * 
 * @param {string} strFormTag header screen tag
 * @param {*} Main_Position   Not used
 * @param {*} Seleted_Tab     Not used
 * @param {string} strSelectedTabTag tab tag
 */
export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  try {
    // Normalize tags
    if (strFormTag) {
      strFormTag = strFormTag.toString().toUpperCase();
    }
    if (strSelectedTabTag) {
      strSelectedTabTag = strSelectedTabTag.toString().toUpperCase();
    }

    // C++: if(strSubFormTag.Find("CMNT") != -1 && pRecordRepostion->iComplete == 1)
    // For Comments tab, manage toolbar and fields when navigating
    // Note: In C++, this checks iComplete==1, but in JS MainSubReposition doesn't have that flag
    // We'll manage it whenever the Comments tab is accessed
    if (strSelectedTabTag && strSelectedTabTag.includes('CMNTS')) {
      // C++: manageCommentsTabToolBar(strFormTag, strSubFormTag);
      manageCommentsTabToolBar(strFormTag, strSelectedTabTag, devInterfaceObj);
    }
  } catch (error) {
    console.warn('[Web_HSE] Error in MainSubReposition:', error);
  }
}

/**
 * @name ShowScreen
 * @description this event is called when screen\tab is open,Note : this event called after fetching data 'This mean you can't change open
 *              criteria here.
 *              You Must set attributes for screen buttons here using setScreenDisableBtn(bDisableNew, bDisableSave, bDisableDelete)
 * @param {Function} setScreenDisableBtn - Function to set screen button states
 * @param {*} strScrTag screen tag
 * @param {*} strTabTag tab tag
 */
export function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag) {
  setScreenDisableBtn(false, false, false);
}

