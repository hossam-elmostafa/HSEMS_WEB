/**
 * ObservationTabManagement.js
 * 
 * Handles tab enabling/disabling and Comments tab management for Observation module
 * 
 * Tasks:
 * - RQ_HSM_22_12_10_59: Implement Investigation Team Tab
 * - RQ_HSM_22_12_11_13: Implement Comments Tab
 * 
 * Implements C++ logic from:
 * - NearMissEntryCategory::DisplayRecordRepostion (tab enabling)
 * - NearMissEntryCategory::isObservationTabsEnabled (check if tabs should be enabled)
 * - CHSEMSCommonCategory::manageCommentsTabToolBar (Comments tab management)
 */

/**
 * Check if observation tabs should be enabled
 * C++: NearMissEntryCategory::isObservationTabsEnabled()
 * @param {Function} executeSQLAsync - Async SQL execution function
 * @param {Function} getValFromRecordSet - Function to extract value from recordset
 * @returns {Promise<boolean>} True if tabs should be enabled
 */
export async function isObservationTabsEnabled(executeSQLAsync, getValFromRecordSet) {
  try {
    // C++: SELECT TOP 1 ISNULL(HSEOBSRVTN_LCKENTRYTABS,'') FROM HSE_HSEOBSRVTN
    const sql = "SELECT TOP 1 ISNULL(HSEOBSRVTN_LCKENTRYTABS,'') AS LCKENTRYTABS FROM HSE_HSEOBSRVTN";
    const result = await executeSQLAsync(sql);
    
    // Extract value from result
    let observationTabsLocked = '';
    if (result && result.length > 0) {
      const row = result[0];
      observationTabsLocked = (row.LCKENTRYTABS || row.lckentrytabs || Object.values(row)[0] || '').toString().toUpperCase();
    }
    
    // C++: if(strObservationTabsLocked != "YES") { bObservationTabsEnabled = true; }
    return observationTabsLocked !== 'YES';
  } catch (error) {
    console.warn('[Web_HSE] Error checking observation tabs enabled status:', error);
    // Default to enabled if error occurs
    return true;
  }
}

/**
 * Enable or disable observation tabs based on record completion status
 * C++: NearMissEntryCategory::DisplayRecordRepostion() when iComplete==1
 * @param {string} formTag - Main form tag (e.g., "HSE_TGNRSTMISCENT", "HSE_TgNrstMisccnfrmtn")
 * @param {boolean} tabsEnabled - Whether tabs should be enabled
 * @param {Function} TabEnable - Function to enable/disable tabs
 */
export function manageObservationTabs(formTag, tabsEnabled, TabEnable) {
  if (!TabEnable) {
    console.warn('[Web_HSE] TabEnable function not available');
    return;
  }

  const normalizedFormTag = formTag ? formTag.toString().toUpperCase() : '';
  
  // Determine tab tags based on screen type
  // Entry screen: HSE_TgNrstMiscEnt
  // Review screen: HSE_TgNrstMisccnfrmtn
  // Approval/Follow-up screen: HSE_TgNrstMiscflwup
  // Reward screen: HSE_TgNrMisRward
  let investigationTeamTab, commentsTab, tracingTab;
  
  if (normalizedFormTag.includes('CNFRMTN') || normalizedFormTag === 'HSE_TGNRSTMISCCNFRMTN') {
    // Observation Review screen
    investigationTeamTab = 'HSE_TgNrstMiscCnfrminvstgtntems';
    commentsTab = 'HSE_TGNRMSCNFRMCMNTS';
    tracingTab = 'HSE_TgNrstMisccnfrmtnTrc';
  } else if (normalizedFormTag.includes('FLWUP') || normalizedFormTag === 'HSE_TGNRSTMISCFLWUP') {
    // Observation Approval/Follow-up screen
    investigationTeamTab = 'HSE_TgNrstMiscFlwupinvstgtntems';
    commentsTab = 'HSE_TGNRMSFLWUPCMNTS';
    tracingTab = 'HSE_TgNrstMiscflwupTrc';
  } else if (normalizedFormTag.includes('RWARD') || normalizedFormTag === 'HSE_TGNRMISRWARD') {
    // Observation Reward screen
    investigationTeamTab = 'HSE_TgNrMisRwardInvstgtntems';
    commentsTab = 'HSE_TgNrMisRwardCMNTS';
    tracingTab = 'HSE_TgNrMisRwardTrc';
  } else {
    // Default: Observation Entry screen
    investigationTeamTab = 'HSE_TgNrstMiscEntinvstgtntems';
    commentsTab = 'HSE_TGNERMSENTCMNTS';
    tracingTab = 'HSE_TgNrstMiscEntTrc';
  }

  try {
    // C++: EnableTab("HSE_TgNrstMiscEntinvstgtntems",true/false);
    // C++: EnableTab("HSE_TGNERMSENTCMNTS",true/false);
    // C++: EnableTab("HSE_TgNrstMiscEntTrc",true/false); // Tracing tab
    TabEnable(formTag, investigationTeamTab, tabsEnabled);
    TabEnable(formTag, commentsTab, tabsEnabled);
    TabEnable(formTag, tracingTab, tabsEnabled);
  } catch (error) {
    console.error('[Web_HSE] Error managing observation tabs:', error);
  }
}

/**
 * Get the true table name for a form/tab
 * This is a simplified version - in C++ it calls GetTrueTableName(66, formTag, subFormTag)
 * For Comments tab, the table name is typically "HSE_NERMSCMNTS"
 * @param {string} formTag - Main form tag
 * @param {string} subFormTag - Sub form/tab tag
 * @returns {string} Table name
 */
function getTrueTableName(formTag, subFormTag) {
  // For Comments tab, return the table name directly
  if (subFormTag && subFormTag.toUpperCase().includes('CMNTS')) {
    return 'HSE_NERMSCMNTS';
  }
  // For other tabs, you might need to map them
  // For now, return a default based on the subform tag
  return subFormTag || formTag;
}

/**
 * Manage Comments tab toolbar and fields based on FLAG and user permissions
 * C++: CHSEMSCommonCategory::manageCommentsTabToolBar(strFormTag, strSubFormTag)
 * @param {string} formTag - Main form tag
 * @param {string} subFormTag - Sub form/tab tag (e.g., "HSE_TGNERMSENTCMNTS")
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function manageCommentsTabToolBar(formTag, subFormTag, devInterface) {
  const {
    FormGetField,
    getUserName,
    setScreenDisableBtn,
    changeFldAttrb,
  } = devInterface;

  if (!FormGetField || !getUserName || !setScreenDisableBtn) {
    console.warn('[Web_HSE] Missing required functions for Comments tab management');
    return;
  }

  try {
    // C++: CString strTableName = GetTrueTableName(66, strFormTag, strSubFormTag);
    const tableName = getTrueTableName(formTag, subFormTag);

    // C++: CString strFlag = FormGetField(strTableName, "FLAG");
    const strFlag = FormGetField(tableName, 'FLAG') || '';

    if (strFlag === '1') {
      // C++: disableCmntTabFields(strSubFormTag);
      // C++: disableCmntTabToolBar(strFormTag, strSubFormTag);
      disableCmntTabFields(subFormTag, changeFldAttrb);
      disableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn);
    } else if (strFlag === '0') {
      // C++: CString strCommentedBy = FormGetField(strTableName, "CMNTDNEBY");
      // C++: CString userName = GetUserLogin();
      const strCommentedBy = FormGetField(tableName, 'CMNTDNEBY') || '';
      const userName = getUserName() || '';

      // C++: if(strCommentedBy.Compare(userName) == 0)
      if (strCommentedBy === userName) {
        // C++: enableCmntTabFields(strSubFormTag);
        // C++: enableCmntTabToolBar(strFormTag, strSubFormTag);
        enableCmntTabFields(subFormTag, changeFldAttrb);
        enableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn);
      } else {
        // C++: disableCmntTabToolBar(strFormTag, strSubFormTag);
        // C++: disableCmntTabFields(strSubFormTag);
        disableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn);
        disableCmntTabFields(subFormTag, changeFldAttrb);
      }
    } else if (strFlag === '') {
      // C++: enableCmntTabFields(strSubFormTag);
      // C++: enableCmntTabToolBar(strFormTag, strSubFormTag);
      // C++: setCmntScreenSource(strFormTag, strSubFormTag, strTableName);
      enableCmntTabFields(subFormTag, changeFldAttrb);
      enableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn);
      // Note: setCmntScreenSource is commented out in C++ code, so we skip it
    }
  } catch (error) {
    console.error('[Web_HSE] Error managing Comments tab toolbar:', error);
  }
}

/**
 * Disable Comments tab toolbar buttons (delete, save)
 * C++: CHSEMSCommonCategory::disableCmntTabToolBar(strFormTag, strSubFormTag)
 * @param {string} formTag - Main form tag
 * @param {string} subFormTag - Sub form/tab tag
 * @param {Function} setScreenDisableBtn - Function to disable screen buttons
 */
function disableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn) {
  // C++: EnableToolbar(strFormTag, strSubFormTag, TOOLBAR_DELETE, FALSE);
  // C++: EnableToolbar(strFormTag, strSubFormTag, TOOLBAR_SAVE, FALSE);
  // In JS: setScreenDisableBtn(bDisableNew, bDisableSave, bDisableDelete)
  // For tab, we need to disable Save and Delete
  if (setScreenDisableBtn) {
    setScreenDisableBtn(false, true, true); // Disable Save and Delete
  }
}

/**
 * Enable Comments tab toolbar buttons (delete, save)
 * C++: CHSEMSCommonCategory::enableCmntTabToolBar(strFormTag, strSubFormTag)
 * @param {string} formTag - Main form tag
 * @param {string} subFormTag - Sub form/tab tag
 * @param {Function} setScreenDisableBtn - Function to enable screen buttons
 */
function enableCmntTabToolBar(formTag, subFormTag, setScreenDisableBtn) {
  // C++: EnableToolbar(strFormTag, strSubFormTag, TOOLBAR_DELETE, TRUE);
  // C++: EnableToolbar(strFormTag, strSubFormTag, TOOLBAR_SAVE, TRUE);
  if (setScreenDisableBtn) {
    setScreenDisableBtn(false, false, false); // Enable all buttons
  }
}

/**
 * Disable Comments tab fields
 * C++: CHSEMSCommonCategory::disableCmntTabFields(strSubFormTag)
 * @param {string} subFormTag - Sub form/tab tag
 * @param {Function} changeFldAttrb - Function to change field attributes
 */
function disableCmntTabFields(subFormTag, changeFldAttrb) {
  // C++: SetFieldVisibleAttrib(strSubFormTag, "CMNT", "ALWAYSLOCKED");
  if (changeFldAttrb) {
    try {
      changeFldAttrb(subFormTag, 'CMNT', 'ALWAYSLOCKED');
    } catch (error) {
      console.warn('[Web_HSE] Error disabling Comments tab field:', error);
    }
  }
}

/**
 * Enable Comments tab fields
 * C++: CHSEMSCommonCategory::enableCmntTabFields(strSubFormTag)
 * @param {string} subFormTag - Sub form/tab tag
 * @param {Function} changeFldAttrb - Function to change field attributes
 */
function enableCmntTabFields(subFormTag, changeFldAttrb) {
  // C++: SetFieldVisibleAttrib(strSubFormTag, "CMNT", "MUST");
  if (changeFldAttrb) {
    try {
      changeFldAttrb(subFormTag, 'CMNT', 'MUST');
    } catch (error) {
      console.warn('[Web_HSE] Error enabling Comments tab field:', error);
    }
  }
}

