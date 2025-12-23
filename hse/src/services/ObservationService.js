import { toast } from "react-toastify";
import { WEB_HSE_API_URL, OBSERVATION_SCREEN_TAGS } from '../config/constants';

/**
 * Check if a screen tag belongs to the Observation module
 * @param {string} screenTag - The screen tag to check
 * @returns {boolean} - True if the screen is an observation screen
 */
export function isObservationScreen(screenTag) {
  if (!screenTag) return false;
  
  const normalizedTag = screenTag.toString().toUpperCase();
  return OBSERVATION_SCREEN_TAGS.some(tag => normalizedTag === tag.toUpperCase());
}

/**
 * Send button click event to Web_HSE backend
 * Handles Reject, Confirm, Cancel, and Entry Complete buttons on Observation module screens
 * @param {string} buttonName - The button name/ID to log
 * @param {string} screenTag - The screen tag to check if it's an observation screen
 * @param {Object} eventObj - The full event object containing fullRecord, strScrTag, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 */
// Store pending reject observation execution info (module-level)
let pendingRejectObservation = null;

export function setPendingRejectObservation(observationInfo) {
  pendingRejectObservation = observationInfo;
}

export function clearPendingRejectObservation() {
  pendingRejectObservation = null;
}

export function sendButtonClickToBackend(buttonName, screenTag, eventObj = {}, devInterface = {}) {
  // Debug logging to see what's being passed
  console.log('[Web_HSE Debug] Button clicked:', {
    buttonName,
    screenTag,
    isObservationScreen: isObservationScreen(screenTag)
  });
  
  // Normalize button name for comparison (buttonName is already normalized to uppercase in buttonEvents.js)
  const normalizedButton = buttonName ? buttonName.toString().toUpperCase() : '';
  const normalizedScreenTag = screenTag ? screenTag.toString().toUpperCase() : '';
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_OK
  // Handle reject reason screen OK button (this is part of observation module flow)
  if (normalizedScreenTag === 'HSE_TGRJCTRSN' && normalizedButton === 'RJCTRSN_BTN_OK') {
    console.log('[Web_HSE] ✓ Reject reason screen OK button (RJCTRSN_BTN_OK) clicked!');
    handleRejectReasonOkButton(devInterface);
    return; // Don't process further
  }
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_CANCEL
  if (normalizedScreenTag === 'HSE_TGRJCTRSN' && normalizedButton === 'RJCTRSN_BTN_CANCEL') {
    console.log('[Web_HSE] Reject reason screen Cancel button clicked. Clearing pending rejection.');
    clearPendingRejectObservation();
    return; // Don't process further
  }
  
  // Only process if this is an observation screen
  if (!isObservationScreen(screenTag)) {
    console.log('[Web_HSE Debug] Not an observation screen, skipping');
    return;
  }
  
  // Handle buttons: Reject, Confirm, Cancel, Entry Complete
  // Button names follow pattern:
  // - NRSTMISCENT_RJC (Reject)
  // - NRSTMISCENT_ACP (Confirm/Accept)
  // - NRSTMISCENT_CNCL (Cancel)
  // - NRSTMISCENT_ENTCMPLT (Entry Complete)
  if (normalizedButton.includes('_RJC') || normalizedButton.endsWith('_RJC')) {
    // RQ_HSM_22_12_10_50: Handle Reject button (button name contains or ends with _RJC)
    // ClickUp Task: https://app.clickup.com/t/86c76v8yr
    handleRejectButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton === 'NRSTMISCENT_ENTCMPLT' || normalizedButton.endsWith('_ENTCMPLT')) {
    // Handle Entry Complete button
    handleEntryCompleteButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton === 'NRSTMISCENT_ACP' || normalizedButton.endsWith('_ACP')) {
    // RQ_HSM_22_12_10_48: Handle Confirm/Accept button
    // ClickUp Task: https://app.clickup.com/t/86c76v5ze
    handleConfirmButton(buttonName, screenTag, eventObj, devInterface);
  } else {
    // Other buttons are not handled
    console.log('[Web_HSE Debug] Button not handled. Button name:', normalizedButton);
  }
}




/**
 * Handle Reject button click
 * RQ_HSM_22_12_10_50: Handle Reject Custom Button
 * ClickUp Task: https://app.clickup.com/t/86c76v8yr
 * Implements the logic from NearMissCategory::DisplayCustomButtonClicked for NRSTMISCENT_RJC
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
async function handleRejectButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      openScr,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !openScr
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Reject button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Near Miss / Observation number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.NRSTMISCENT_NRSTMISCNUM ||
        firstRecord?.NRSTMISCNUM ||
        '';
    }

    const formTag = screenTag || 'HSE_TGNRSTMISCCNFRMTN';

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') ||
        FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Observation number. Please select a record first.');
      return;
    }

    // Determine module type based on screen tag
    // C++: rejectRecord() sets "NRSTMISCENT-L1" for confirmation screen, "NRSTMISCENT-L2" for follow-up
    let moduleType = 'NRSTMISCENT-L1'; // Default for confirmation screen
    if (formTag === 'HSE_TGNRSTMISCCNFRMTN') {
      moduleType = 'NRSTMISCENT-L1';
    } else if (formTag === 'HSE_TGNRSTMISCFLWUP') {
      moduleType = 'NRSTMISCENT-L2';
    }

    // C++: ShowRejectreason() opens the reject reason screen
    // Use openScr with the reject reason screen tag and default values
    // The screen will handle the dialog/prompt internally
    const rejectReasonScreenTag = 'HSE_TGRJCTRSN';
    const criteria = `SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN='${keyFieldValue.replace(/'/g, "''")}') AND (RJCTRSN_MODULETYPE='${moduleType}') AND (ISNULL(RJCTRSN_TRACINGLNK, 0) = 0)`;
    
    // Set default values for the reject reason screen
    // C++: FormSetField("HSE_RJCTRSN","RJCTRSN_LINKWITHMAIN",strKeyValForRjctScr)
    //      FormSetField("HSE_RJCTRSN","RJCTRSN_MODULETYPE",strRjctRsnSrcScr)
    const defValObj = {
      RJCTRSN_LINKWITHMAIN: keyFieldValue,
      RJCTRSN_MODULETYPE: moduleType,
      RJCTRSN_TRACINGLNK: 0,
    };

    // Check if reject reason already exists before opening screen
    // C++: After screen closes, checks getgbUpdateRejectedRecord() flag
    // In JS: We'll check if a reject reason was added after screen opens
    const checkRejectReasonBeforeSql = `SELECT COUNT(0) FROM HSE_RJCTRSN WHERE RJCTRSN_MODULETYPE = '${moduleType.replace(/'/g, "''")}' AND RJCTRSN_LINKWITHMAIN = '${keyFieldValue.replace(/'/g, "''")}' AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0`;
    
    let rejectReasonCountBefore = 0;
    try {
      const resultBefore = await executeSQLAsync(checkRejectReasonBeforeSql);
      if (resultBefore && resultBefore.length > 0 && resultBefore[0]) {
        const countObj = resultBefore[0];
        rejectReasonCountBefore = parseInt(Object.values(countObj)[0] || 0, 10);
      }
    } catch (error) {
      console.warn('[Web_HSE] Error checking reject reason count before:', error);
    }

    // Store info for executing rejectObservation after reject reason screen OK button is clicked
    // C++: After reject reason screen closes, checks getgbUpdateRejectedRecord() flag
    // In JS: We use ButtonClicked event to detect when RJCTRSN_BTN_OK is clicked
    // C++: setgbUpdateRejectedRecord(false) is set initially, then true when OK is clicked
    setPendingRejectObservation({
      keyFieldValue: keyFieldValue.toString(), // Ensure it's a string for the stored procedure
      formTag,
      getUserName,
      executeSQLAsync,
      refreshData,
      toast,
    });

    // Open the reject reason screen - it has its own built-in dialog
    // C++: ShowRejectreason(strCriteria) -> opens HSE_TGRJCTRSN screen
    console.log('[Web_HSE] Opening reject reason screen:', {
      screenTag: rejectReasonScreenTag,
      observationNumber: keyFieldValue,
      moduleType: moduleType,
    });
    
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, defValObj);

    // C++: After screen closes, if getgbUpdateRejectedRecord() == true, execute rejectObservation
    // In JS: We use ButtonClicked event to detect when RJCTRSN_BTN_OK is clicked
    // The stored procedure will be executed automatically when OK button is clicked
    console.log('[Web_HSE] Reject reason screen opened. Waiting for OK button click (RJCTRSN_BTN_OK)...');
    console.log('[Web_HSE] Pending rejection info stored:', {
      observationNumber: keyFieldValue,
      screenTag: formTag,
    });
  } catch (error) {
    console.error('[Web_HSE] Error in handleRejectButton:', error);
    toast.error('An error occurred while processing the Reject action');
  }
}

/**
 * Handle Confirm (Accept) button click
 * RQ_HSM_22_12_10_48: Handle Confirm Custom Button
 * ClickUp Task: https://app.clickup.com/t/86c76v5ze
 * Implements the logic from NearMissConfirmationCategory::AcceptNearMiss for NRSTMISCENT_ACP
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
async function handleConfirmButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      AskYesNoMessage,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !AskYesNoMessage
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Confirm button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Near Miss / Observation number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.NRSTMISCENT_NRSTMISCNUM ||
        firstRecord?.NRSTMISCNUM ||
        '';
    }

    const formTag = screenTag || 'HSE_TGNRSTMISCCNFRMTN';

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') ||
        FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Observation number. Please select a record first.');
      return;
    }

    // Check if there is a new reject reason for this record
    // Equivalent to: IsNewRjcRsnExist('NRSTMISCENT-L1', keyFieldValue)
    const checkRejectSql = `
      SELECT COUNT(0) AS CNT
      FROM HSE_RJCTRSN
      WHERE RJCTRSN_MODULETYPE = 'NRSTMISCENT-L1'
        AND RJCTRSN_LINKWITHMAIN = '${keyFieldValue.replace(/'/g, "''")}'
        AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0
    `;

    let rejectReasonCount = 0;
    try {
      const rs = await executeSQLAsync(checkRejectSql);
      // WebInfra returns an object with recordset array; fall back gracefully
      const recordset = rs?.recordset || rs?.[0]?.recordset || [];
      if (Array.isArray(recordset) && recordset.length > 0) {
        const row = recordset[0];
        const val = row.CNT ?? row.cnt ?? Object.values(row)[0];
        rejectReasonCount = parseInt(val, 10) || 0;
      }
    } catch (err) {
      console.warn('[Web_HSE] Failed to check reject reasons, proceeding without that check:', err);
    }

    // Build stored procedure call
    const sourceScreenName = formTag;
    const userName = getUserName() || '';
    const spSql = `EXECUTE confirmNearMissTXN '${keyFieldValue.replace(/'/g, "''")}','${sourceScreenName.replace(
      /'/g,
      "''"
    )}','${userName.replace(/'/g, "''")}'`;

    // If reject reasons exist, ask the user before proceeding
    if (rejectReasonCount > 0) {
      const msg =
        'Reject reason for this record exists.\n\rDo you want to delete it and approve this observation?';
      const confirmed = await AskYesNoMessage('Prompt', msg);
      if (!confirmed) {
        // User chose not to proceed
        return;
      }
    }

    try {
      await executeSQLAsync(spSql);
      refreshData('', 'REFRESH_SELECTED');
      toast.success('Observation confirmed successfully');
    } catch (err) {
      console.error('[Web_HSE] Error executing confirmNearMissTXN:', err);
      toast.error('Error confirming observation. Please try again.');
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleConfirmButton:', error);
    toast.error('An error occurred while processing the Confirm action');
  }
}


/**
 * Handle Entry Complete button click
 * Implements the logic from NearMissEntryCategory::DisplayCustomButtonClicked for NRSTMISCENT_ENTCMPLT
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */

//needs testing and validation  
async function handleEntryCompleteButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const { FormGetField, executeSQL, executeSQLPromise, getUserName, refreshData, getFldValFrmFullRec } = devInterface;
    
    // Check if required functions are available
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !getUserName || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Entry Complete button');
      toast.error('System error: Required functions not available');
      return;
    }
    
    // Prefer executeSQLPromise for async operations, fallback to executeSQL
    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, strTabTag, fullRecordArrKeys } = eventObj || {};
    
    // Check if record is saved (must have selected records)
    // Rule: If (Near Miss Confirmation) checkbox [11@600] = "un-checked", then
    // If no records selected, show error message
    let recordCount = 0;
    if (!strTabTag || strTabTag === '') {
      // Main form - check if we have selected records
      recordCount = fullRecordArr ? (Array.isArray(fullRecordArr) ? fullRecordArr.length : 1) : 0;
    } else {
      // Subform - check if we have selected records
      recordCount = fullRecordArr ? (Array.isArray(fullRecordArr) ? fullRecordArr.length : 1) : 0;
    }

    if (recordCount === 0) {
      toast.warning('You must save the Record first');
      return;
    }

    // Get screen tag - use the form tag from event or screenTag parameter
    const formTag = screenTag || 'HSE_TGNRSTMISCENT';
    
    // Get key field value - try from fullRecordArrKeys first, then from form field
    // C++: GetKeyField(pCustomButtonClicked->Form_Tag, pCustomButtonClicked->pMultiSelectedRows)
    let observationId = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      // Get key from selected records (most reliable)
      observationId = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      // Try to get from fullRecord array
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      observationId = firstRecord?.NRSTMISCENT_NRSTMISCNUM || firstRecord?.NRSTMISCNUM || '';
    }
    
    // If still not found, try to get from form field
    if (!observationId) {
      observationId = FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') || '';
    }
    
    if (!observationId) {
      toast.warning('Unable to find Observation ID. Please save the record first.');
      return;
    }
    
    // RQ-2-2024.14: Update NRSTMISCENT_RPRDSC with NRSTMISCENT_NRSTMISCDESC
    const description = FormGetField(formTag, 'NRSTMISCENT_NRSTMISCDESC') || '';

    // Update the RPRDSC field
    if (description) {
      const updateSql = `UPDATE HSE_VWNRSTMISCENT SET NRSTMISCENT_RPRDSC='${description.replace(/'/g, "''")}' WHERE NRSTMISCENT_NRSTMISCNUM=${observationId}`;
      try {
        await executeSQLAsync(updateSql);
      } catch (error) {
        console.error('[Web_HSE] Error updating RPRDSC:', error);
        toast.error('Error updating record description');
        return;
      }
    }

    // Get key field value for the stored procedure
    // The key field is NRSTMISCENT_NRSTMISCNUM
    const keyFieldValue = observationId;
    
    // Get source screen name - using screen tag to get caption
    // In C++: GetScrCptnByTag(66, pCustomButtonClicked->Form_Tag, "")
    // For now, we'll use the screen tag as the source screen name
    const sourceScreenName = formTag;
    
    // Get user name
    const userName = getUserName() || '';
    
    // Execute the completeNearMissTXN stored procedure
    // C++: EXECUTE completeNearMissTXN '%s','%s','%s'
    const storedProcSql = `EXECUTE completeNearMissTXN '${keyFieldValue.replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    try {
      const result = await executeSQLAsync(storedProcSql);
      
      // Refresh the screen
      refreshData('', 'REFRESH_SELECTED');
      
      // Show success message
      toast.success('Observation entry completed successfully');
      
      // Note: Email functionality is commented out in C++ code, so we skip it here
      // If needed in the future, it can be implemented similar to the C++ code
      
    } catch (error) {
      console.error('[Web_HSE] Error executing completeNearMissTXN:', error);
      toast.error('Error completing observation entry. Please try again.');
    }
    
  } catch (error) {
    console.error('[Web_HSE] Error in handleEntryCompleteButton:', error);
    toast.error('An error occurred while processing the Entry Complete action');
  }
}

/**
 * Handle Reject Reason screen OK button click
 * C++: CRejectReason::RejectReasonOk() - sets gbUpdateRejectedRecord = true
 * This is called when RJCTRSN_BTN_OK is clicked in the reject reason screen
 * RQ_HSM_22_12_10_50: Handle Reject Custom Button
 * ClickUp Task: https://app.clickup.com/t/86c76v8yr
 * @param {Object} devInterface - Object containing devInterface functions
 */
async function handleRejectReasonOkButton(devInterface) {
  try {
    if (!pendingRejectObservation) {
      console.warn('[Web_HSE] ⚠ No pending rejection info found. Stored procedure will not execute.');
      return;
    }

    console.log('[Web_HSE] Executing rejectObservation stored procedure...');
    
    const { keyFieldValue, formTag, getUserName, executeSQLAsync, refreshData, toast } = pendingRejectObservation;
    const userName = getUserName() || '';
    const sourceScreenName = formTag;
    
    // C++: strSQL.Format("EXECUTE rejectObservation '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    // Note: keyFieldValue should be passed as string (stored procedure expects VARCHAR)
    const spSql = `EXECUTE rejectObservation '${keyFieldValue.toString().replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Executing SQL:', spSql);
    await executeSQLAsync(spSql);
    console.log('[Web_HSE] ✓ rejectObservation executed successfully');
    
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Observation ${keyFieldValue} rejected successfully`);
    
    // Clear pending execution
    clearPendingRejectObservation();
  } catch (error) {
    console.error('[Web_HSE] ✗ Error executing rejectObservation:', error);
    if (pendingRejectObservation && pendingRejectObservation.toast) {
      pendingRejectObservation.toast.error('Error rejecting observation: ' + (error.message || 'Unknown error'));
    }
    clearPendingRejectObservation();
  }
}

