import { manageCommentsTabToolBar, isObservationTabsEnabled, manageObservationTabs } from '../services/Observation service/ObservationService';
import { OBSERVATION_SCREEN_TAGS } from '../config/constants';

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

    // Debug: Log when Tracing tab is accessed
    if (strSelectedTabTag && (strSelectedTabTag.includes('TRC') || strSelectedTabTag.includes('TRACING'))) {
      const { FormGetField } = devInterfaceObj;
      if (FormGetField) {
        // Try to get the observation number to verify link field
        const obsNum = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') ||
                      FormGetField('HSE_vwNRSTMISCENT', 'NrstMiscEnt_NrstMiscNum') ||
                      FormGetField(strFormTag, 'NRSTMISCENT_NRSTMISCNUM') ||
                      FormGetField(strFormTag, 'NrstMiscEnt_NrstMiscNum') ||
                      '';
        console.log('[Web_HSE] Tracing tab accessed:', {
          formTag: strFormTag,
          tabTag: strSelectedTabTag,
          observationNumber: obsNum,
          note: 'If no records show, verify:',
          checks: [
            '1. View HSE_NrstMiscEntTrc exists in database',
            '2. Records exist in HSE_NRSTMISCENTTRC table with NRSTMISCENTTRC_LNK matching observation number',
            '3. Check server logs for SQL query and results'
          ]
        });
      }
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
export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag) {
  setScreenDisableBtn(false, false, false);
  
  // Enable observation tabs when screen first loads (not just after save)
  // Only enable for main screen, not for tabs
  if (!strTabTag || strTabTag === '') {
    try {
      const normalizedScrTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
      const isObservationScreen = OBSERVATION_SCREEN_TAGS.some(tag => 
        normalizedScrTag.includes(tag.toUpperCase())
      ) || normalizedScrTag.includes('NRSTMISC') || normalizedScrTag.includes('NERMSENT');
      
      if (isObservationScreen) {
        const { executeSQLPromise, TabEnable } = devInterfaceObj;
        
        if (executeSQLPromise && TabEnable) {
          // Check if tabs should be enabled
          const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterfaceObj.getValFromRecordSet);
          
          // Enable/disable tabs based on configuration
          manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
        }
      }
    } catch (error) {
      console.warn('[Web_HSE] Error enabling observation tabs in ShowScreen:', error);
    }
  }
}

