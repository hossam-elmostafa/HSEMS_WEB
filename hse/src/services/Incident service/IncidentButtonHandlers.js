import { toast } from "react-toastify";

/**
 * IncidentButtonHandlers.js
 * 
 * Handles all button click events for Incident (Near Miss & Accident) module screens
 * 
 * Requirements:
 * - RQ_HSM_HSE_18_12_25_10_19: Implement Incident (Near Miss & Accident)
 * 
 * C++ Reference Files:
 * - AccidentCategory.cpp
 * - AccidentEntryCategory.cpp
 * - AccidentConfirmationCategory.cpp
 */

// Store pending reject Incident execution info (module-level)
let pendingRejectIncident = null;

/**
 * Safe wrapper for FormGetField that handles errors gracefully
 * @param {Function} FormGetField - The FormGetField function
 * @param {string} formTag - The form tag to read from
 * @param {string} fieldName - The field name to read
 * @returns {string} - The field value or empty string if error
 */
function safeFormGetField(FormGetField, formTag, fieldName) {
  try {
    if (!FormGetField || !formTag || !fieldName) {
      return '';
    }
    const value = FormGetField(formTag, fieldName);
    return value || '';
  } catch (error) {
    // Silently return empty string if form tag or field doesn't exist
    return '';
  }
}

export function setPendingRejectIncident(incidentInfo) {
  pendingRejectIncident = incidentInfo;
}

export function clearPendingRejectIncident() {
  pendingRejectIncident = null;
}

export function hasPendingRejectIncident() {
  return pendingRejectIncident !== null;
}

/**
 * Handle Entry Completed button click for Incident Entry
 * 
 * Requirement: RQ_HSM_30_12_25_07_50 - Implement Entry Completed
 * 
 * This function implements the "Entry Completed" functionality for Incident Entry screen.
 * When the user clicks the "Entry Completed" button, it:
 * 1. Validates that recommended actions have linked causes (if applicable)
 * 2. Executes the completeAccidentTXN stored procedure to:
 *    - Insert a tracing record with "Entry Completed" action description
 *    - Update the incident status to "Completed" (status code 4)
 *    - Set the source screen name and entry user
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: AccidentEntryCategory.cpp line 34-56: ACDNTENT_ENTCMPLT button handler
 * 
 * @param {string} buttonName - The button name (e.g. ACDNTENT_ENTCMPLT, ACDNTENT_EntCmplt)
 * @param {string} screenTag - The screen tag (e.g. HSE_TGACDNTENT)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 * 
 * @example
 * // Called when user clicks "Entry Completed" button on Incident Entry screen
 * await handleEntryCompletedButton('ACDNTENT_ENTCMPLT', 'HSE_TGACDNTENT', eventObj, devInterface);
 */
export async function handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Entry Completed button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGACDNTENT';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Incident number)
    // C++: CString strKey = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.ACDNTENT_ACDNTNUM ||
        firstRecord?.ACDNTNUM ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
        safeFormGetField(FormGetField, formTag, 'ACDNTENT_ACDNTNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident number. Please select a record first.');
      return;
    }

    // C++: Check if valid for entry complete
    // C++: if(isValidForEntryComplete(strKey) == 'N')
    const validationSql = `SELECT dbo.ValidateBeforeIncdntEntryComplt(${keyFieldValue})`;
    let isValid = 'Y';
    try {
      const result = await executeSQLAsync(validationSql);
      if (result && result.length > 0 && result[0]) {
        const validationObj = result[0];
        isValid = Object.values(validationObj)[0] || 'Y';
      }
    } catch (error) {
      console.warn('[Web_HSE] Error checking validation for entry complete:', error);
    }

    // If validation fails, ask user if they want to proceed
    if (isValid === 'N') {
      const proceed = window.confirm(
        'Some Recommended Actions don\'t have Linked Cause, proceed?'
      );
      if (!proceed) {
        return;
      }
    }

    // C++: CompleteAccident(pCustomButtonClicked);
    // C++: strSQL.Format("EXECUTE completeAccidentTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const userName = getUserName() || '';
    const strSourceScreenName = 'Incident Entry'; // Screen caption for HSE_TGACDNTENT
    const completeSql = `EXECUTE completeAccidentTXN '${keyFieldValue.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Completing Incident entry:', completeSql);
    try {
      await executeSQLAsync(completeSql);
      console.log('[Web_HSE] ✓ Incident entry completed successfully');
    } catch (dbError) {
      // Check if error is about invalid column name
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: completeAccidentTXN');
        console.error('[Web_HSE] Expected column name format: ACDNTENT_WRKLOC or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError; // Re-throw if it's a different error
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident ${keyFieldValue} entry completed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleEntryCompletedButton:', error);
    toast.error('An error occurred while completing the Incident entry');
  }
}

/**
 * Handle Reject button click for Incident
 * C++ Reference: AccidentCategory.cpp line 230-246: ACDNTENT_RJC button handler
 * 
 * @param {string} buttonName - The button name (e.g. ACDNTENT_RJC)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRejectIncidentButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      openScr,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !openScr
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Reject Incident button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Incident number)
    // C++: CString strAcdntNum=FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.ACDNTENT_ACDNTNUM ||
        firstRecord?.ACDNTNUM ||
        '';
    }

    const formTag = screenTag || 'HSE_TGACDNTCNFRMTN';

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
        safeFormGetField(FormGetField, formTag, 'ACDNTENT_ACDNTNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident number. Please select a record first.');
      return;
    }

    // Determine module type based on screen tag
    // C++: rejectRecord() uses module type "ACDNTENT-L1" for confirmation screen, "ACDNTENT-L2" for follow-up
    let moduleType = 'ACDNTENT-L1'; // Default for confirmation screen
    if (formTag === 'HSE_TGACDNTCNFRMTN') {
      moduleType = 'ACDNTENT-L1';
    } else if (formTag === 'HSE_TGACDNTFOLLWUPDT') {
      moduleType = 'ACDNTENT-L2';
    }

    // C++: rejectRecord() opens the reject reason screen
    const rejectReasonScreenTag = 'HSE_TGRjctRsn';
    const criteria = `(RJCTRSN_LINKWITHMAIN='${keyFieldValue.replace(/'/g, "''")}') AND (RJCTRSN_MODULETYPE='${moduleType.replace(/'/g, "''")}') AND (ISNULL(RJCTRSN_TRACINGLNK, 0) = 0)`;
    
    // Set default values for the reject reason screen
    const defValObj = {
      RJCTRSN_LINKWITHMAIN: keyFieldValue,
      RJCTRSN_MODULETYPE: moduleType,
      RJCTRSN_TRACINGLNK: 0,
    };

    const {
      FormSetField,
      doToolbarAction,
    } = devInterface;
    
    setPendingRejectIncident({
      keyFieldValue: keyFieldValue.toString(),
      formTag,
      getUserName,
      executeSQLAsync,
      refreshData,
      toast,
      FormSetField,
      doToolbarAction,
    });

    // Open the reject reason screen
    console.log('[Web_HSE] Opening reject reason screen for Incident:', {
      screenTag: rejectReasonScreenTag,
      incidentNumber: keyFieldValue,
      moduleType: moduleType,
    });
    
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, defValObj);

    console.log('[Web_HSE] Reject reason screen opened. Waiting for OK button click (RJCTRSN_BTN_OK)...');
    console.log('[Web_HSE] Pending rejection info stored:', {
      incidentNumber: keyFieldValue,
      screenTag: formTag,
    });
  } catch (error) {
    console.error('[Web_HSE] Error in handleRejectIncidentButton:', error);
    toast.error('An error occurred while processing the Reject Incident action');
  }
}

/**
 * Handle Reject Reason screen OK button click for Incident
 * C++: CRejectReason::RejectReasonOk() - sets gbUpdateRejectedRecord = true
 * This is called when RJCTRSN_BTN_OK is clicked in the reject reason screen
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRejectReasonOkButtonForIncident(devInterface) {
  try {
    if (!pendingRejectIncident) {
      console.warn('[Web_HSE] ⚠ No pending rejection info found. Stored procedure will not execute.');
      return;
    }

    console.log('[Web_HSE] Processing Incident rejection and inserting tracing record...');
    
    const { keyFieldValue, formTag, getUserName, executeSQLAsync, refreshData, toast, FormSetField, doToolbarAction } = pendingRejectIncident;
    const userName = getUserName() || '';
    
    // C++: strSQL.Format("EXECUTE rejectIncident '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const sourceScreenName = 'Incident Confirmation'; // Screen caption for HSE_TGACDNTCNFRMTN
    const rejectSql = `EXECUTE rejectIncident '${keyFieldValue.toString().replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Rejecting Incident:', rejectSql);
    try {
      await executeSQLAsync(rejectSql);
      console.log('[Web_HSE] ✓ Incident rejected successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: rejectIncident');
        console.error('[Web_HSE] Expected column name format: ACDNTENT_WRKLOC or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }
    
    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident ${keyFieldValue} rejected successfully`);
    
    // Clear pending execution
    clearPendingRejectIncident();
  } catch (error) {
    console.error('[Web_HSE] ✗ Error executing rejectIncident:', error);
    if (pendingRejectIncident && pendingRejectIncident.toast) {
      pendingRejectIncident.toast.error('Error rejecting Incident: ' + (error.message || 'Unknown error'));
    }
    clearPendingRejectIncident();
  }
}

/**
 * Handle Confirm button click for Incident Confirmation
 * C++ Reference: AccidentConfirmationCategory.cpp line 49-54: ACDNTENT_CNFRM button handler
 * 
 * @param {string} buttonName - The button name (e.g. ACDNTENT_CNFRM)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleConfirmIncidentButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !doToolbarAction
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Confirm Incident button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGACDNTCNFRMTN';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Incident number)
    // C++: CString strAcdntNum=FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.ACDNTENT_ACDNTNUM ||
        firstRecord?.ACDNTNUM ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
        safeFormGetField(FormGetField, formTag, 'ACDNTENT_ACDNTNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident number. Please select a record first.');
      return;
    }

    // C++: Save before confirming
    // C++: DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
    if (doToolbarAction) {
      doToolbarAction('SAVE', formTag, '');
    }

    // C++: Check if new reject reasons exist
    // C++: bool RjctRsnExist=IsNewRjcRsnExist("ACDNTENT-L1",strAcdntNum);
    const moduleType = 'ACDNTENT-L1';
    const checkRejectReasonSql = `SELECT COUNT(0) FROM HSE_RJCTRSN WHERE RJCTRSN_MODULETYPE = '${moduleType.replace(/'/g, "''")}' AND RJCTRSN_LINKWITHMAIN = '${keyFieldValue.replace(/'/g, "''")}' AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0`;
    
    let rejectReasonCount = 0;
    try {
      const result = await executeSQLAsync(checkRejectReasonSql);
      if (result && result.length > 0 && result[0]) {
        const countObj = result[0];
        rejectReasonCount = parseInt(Object.values(countObj)[0] || 0, 10);
      }
    } catch (error) {
      console.warn('[Web_HSE] Error checking reject reason count:', error);
    }

    // If reject reasons exist, ask user if they want to delete them
    if (rejectReasonCount > 0) {
      const deleteRejectReasons = window.confirm(
        'Reject reasons exist for this incident. Do you want to delete them and confirm the incident?'
      );
      if (!deleteRejectReasons) {
        return;
      }
    }

    // C++: ConfirmAccident(pCustomButtonClicked);
    // C++: strSQL.Format("EXECUTE confirmAccidentTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const userName = getUserName() || '';
    const strSourceScreenName = 'Incident Confirmation'; // Screen caption for HSE_TGACDNTCNFRMTN
    const confirmSql = `EXECUTE confirmAccidentTXN '${keyFieldValue.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Confirming Incident:', confirmSql);
    try {
      await executeSQLAsync(confirmSql);
      console.log('[Web_HSE] ✓ Incident confirmed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: confirmAccidentTXN');
        console.error('[Web_HSE] Expected column name format: ACDNTENT_WRKLOC or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident ${keyFieldValue} confirmed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleConfirmIncidentButton:', error);
    toast.error('An error occurred while confirming the Incident');
  }
}

/**
 * Handle Accept button click for Incident
 * Similar to CAR Accept button logic
 * 
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleAcceptIncidentButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      FormSetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      !FormSetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !doToolbarAction
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Accept Incident button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGACDNTCNFRMTN';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Incident number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.ACDNTENT_ACDNTNUM ||
        firstRecord?.ACDNTNUM ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
        safeFormGetField(FormGetField, formTag, 'ACDNTENT_ACDNTNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident number. Please select a record first.');
      return;
    }

    // Execute accept stored procedure
    const userName = getUserName() || '';
    const strSourceScreenName = 'Incident Confirmation';
    const strStatus = 'Incident Accepted';
    
    const acceptSql = `EXECUTE acceptIncidentTXN '${keyFieldValue.toString().replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','${strStatus.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Accepting Incident:', acceptSql);
    try {
      await executeSQLAsync(acceptSql);
      console.log('[Web_HSE] ✓ Incident accepted successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: acceptIncidentTXN');
        console.error('[Web_HSE] Expected column name format: ACDNTENT_WRKLOC or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident ${keyFieldValue} accepted successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleAcceptIncidentButton:', error);
    toast.error('An error occurred while accepting the Incident');
  }
}

/**
 * Handle Close button click for Incident
 * 
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleCloseIncidentButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      FormSetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      !FormSetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !doToolbarAction
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Close Incident button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGACDNTFOLLWUPDT';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Incident number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.ACDNTENT_ACDNTNUM ||
        firstRecord?.ACDNTNUM ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
        safeFormGetField(FormGetField, formTag, 'ACDNTENT_ACDNTNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident number. Please select a record first.');
      return;
    }

    // Execute close stored procedure
    // Both Investigation Approval and Follow-up use closeAccidentTXN with 3 parameters
    // C++ Reference: 
    //   - IncdntInvstgtnAprvCategory.cpp line 62: KeyFieldValue, strUserName, strSourceScreenName
    //   - AccidentFollowUpCategory.cpp line 88: KeyFieldValue, strUserName, strSourceScreenName
    const userName = getUserName() || '';
    const normalizedScreenTag = (screenTag || formTag || '').toUpperCase();
    
    // Determine screen name based on screen tag
    let strSourceScreenName = 'Incident Follow-up'; // Default
    if (normalizedScreenTag.includes('INCDNTINVSTGTNAPRVL') || normalizedScreenTag.includes('INVSTGTNAPRVL')) {
      strSourceScreenName = 'Incident Investigation Approval';
    } else if (normalizedScreenTag.includes('FOLLWUP') || normalizedScreenTag.includes('FOLLOWUP')) {
      strSourceScreenName = 'Incident Follow-up';
    }
    
    // C++: strSQL.Format("EXECUTE closeAccidentTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
    const closeSql = `EXECUTE closeAccidentTXN '${keyFieldValue.toString().replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Closing Incident:', closeSql);
    try {
      await executeSQLAsync(closeSql);
      console.log('[Web_HSE] ✓ Incident closed successfully');
    } catch (dbError) {
      // Check if error is about invalid column name
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: closeAccidentTXN');
        console.error('[Web_HSE] Expected column name format: ACDNTENT_WRKLOC or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError; // Re-throw if it's a different error
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident ${keyFieldValue} closed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleCloseIncidentButton:', error);
    toast.error('An error occurred while closing the Incident');
  }
}

/**
 * Handle View Reject Reasons button click in Incident Tracing tab
 * C++: CHSEMSCommonCategory::viewRejectReason() - opens reject reasons screen locked
 * 
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewRejectReasonButtonForIncident(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      openScr,
      executeSQLPromise,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || !openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View Reject Reasons button');
      toast.error('System error: Required functions not available');
      return;
    }

    console.log('[Web_HSE] ===== View Reject Reason from Tracing Tab START =====');
    console.log('[Web_HSE] eventObj:', eventObj);
    console.log('[Web_HSE] screenTag:', screenTag);

    // Get Incident primary key from the tracing record
    const tracingTableName = screenTag || 'HSE_AcdntEnt_Trc';
    
    // Try to get Incident primary key from eventObj first
    let incidentPrimaryKey = '';
    if (eventObj && eventObj.fullRecord) {
      const fullRecord = Array.isArray(eventObj.fullRecord) ? eventObj.fullRecord[0] : eventObj.fullRecord;
      if (fullRecord) {
        incidentPrimaryKey = fullRecord.ACDNTENT_TRC_LNK || 
                           fullRecord.AcdntEnt_Trc_Lnk || 
                           fullRecord.ACDNTENT_TRC_LINK ||
                           fullRecord.LNK ||
                           '';
        console.log('[Web_HSE] Got Incident primary key from eventObj.fullRecord:', incidentPrimaryKey);
      }
    }

    // Fallback: Get from form fields
    if (!incidentPrimaryKey) {
      incidentPrimaryKey = safeFormGetField(FormGetField, tracingTableName, 'ACDNTENT_TRC_LNK') ||
                         safeFormGetField(FormGetField, 'HSE_AcdntEnt_Trc', 'ACDNTENT_TRC_LNK') ||
                         safeFormGetField(FormGetField, 'HSE_ACDNTENT_TRC', 'ACDNTENT_TRC_LNK') ||
                         safeFormGetField(FormGetField, 'HSE_TGACDNTENT', 'ACDNTENT_ACDNTNUM') ||
                         safeFormGetField(FormGetField, 'HSE_ACDNTENT', 'ACDNTENT_ACDNTNUM') ||
                         '';
      if (incidentPrimaryKey) {
        console.log('[Web_HSE] Got Incident primary key from FormGetField:', incidentPrimaryKey);
      }
    }

    // Final fallback: Query database if we have executeSQLPromise
    if (!incidentPrimaryKey && executeSQLPromise) {
      try {
        const sql = `SELECT TOP 1 ACDNTENT_TRC_LNK FROM HSE_ACDNTENT_TRC ORDER BY ACDNTENT_TRC_DATTIM DESC`;
        const result = await executeSQLPromise(sql);
        if (result && result.recordset && result.recordset.length > 0) {
          const record = result.recordset[0];
          incidentPrimaryKey = (record.ACDNTENT_TRC_LNK || record.AcdntEnt_Trc_Lnk || '').toString();
          if (incidentPrimaryKey) {
            console.log('[Web_HSE] Got Incident primary key from database query:', incidentPrimaryKey);
          }
        }
      } catch (dbError) {
        console.warn('[Web_HSE] Could not query database for Incident primary key:', dbError);
      }
    }

    if (!incidentPrimaryKey) {
      console.error('[Web_HSE] No Incident primary key found. Cannot open reject reasons screen.');
      toast.warning('No reject reason found for this record');
      return;
    }

    // Module type for Incident
    const moduleType = 'ACDNTENT';

    const criteria = `(RJCTRSN_LINKWITHMAIN = '${incidentPrimaryKey.toString().replace(/'/g, "''")}') AND (RJCTRSN_MODULETYPE LIKE '${moduleType}%')`;

    const rejectReasonScreenTag = 'HSE_TGRjctRsn';
    const bLocked = true; // Open in read-only/locked mode

    console.log('[Web_HSE] Opening reject reasons screen for Incident Tracing:', {
      screenTag: rejectReasonScreenTag,
      incidentPrimaryKey: incidentPrimaryKey,
      moduleType: moduleType,
      criteria: criteria,
    });

    // Open the reject reasons screen locked
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, {}, bLocked);
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewRejectReasonButtonForIncident:', error);
    toast.error('An error occurred while opening reject reasons screen');
  }
}

/**
 * Handle Flash Report Entry Completed button click
 * C++ Reference: IncdntFlshRprtCategory.cpp line 151-152: NCDNTFLSHRPRT_ENTRYCMPLT button handler
 * 
 * @param {string} buttonName - The button name (e.g. NCDNTFLSHRPRT_ENTRYCMPLT)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleFlashReportEntryCompletedButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Flash Report Entry Completed button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGINCDNTFLSHRPRT';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (MAINLINK for Flash Report)
    // C++: CString strPrmryKey = FormGetField("HSE_INCDNTFLSHRPRT","MAINLINK");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.MAINLINK ||
        firstRecord?.INCDNTFLSHRPRT_MAINLINK ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_INCDNTFLSHRPRT', 'MAINLINK') ||
        safeFormGetField(FormGetField, formTag, 'MAINLINK') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Flash Report record. Please select a record first.');
      return;
    }

    // C++: CompleteFlashEntry(strPrmryKey);
    // C++: strSQL.Format("EXECUTE CompleteFlashEntry '%s'",strPrmryKey);
    const completeSql = `EXECUTE CompleteFlashEntry '${keyFieldValue.toString().replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Completing Flash Report entry:', completeSql);
    try {
      await executeSQLAsync(completeSql);
      console.log('[Web_HSE] ✓ Flash Report entry completed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: CompleteFlashEntry');
        console.error('[Web_HSE] Expected column name format: INCDNTFLSHRPRT_AREANAM or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Flash Report ${keyFieldValue} entry completed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleFlashReportEntryCompletedButton:', error);
    toast.error('An error occurred while completing the Flash Report entry');
  }
}

/**
 * Handle Flash Report Reject button click from Review screen
 * C++ Reference: IncdntFlshRprtCategory.cpp line 153-154: INCDNTFLSHRPRTRVEW_RJCT button handler
 * 
 * @param {string} buttonName - The button name (e.g. INCDNTFLSHRPRTRVEW_RJCT)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleFlashReportRejectButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Flash Report Reject button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGINCDNTFLSHRPRTRVEW';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (MAINLINK for Flash Report)
    // C++: CString strPrmryKey = FormGetField("HSE_INCDNTFLSHRPRT","MAINLINK");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.MAINLINK ||
        firstRecord?.INCDNTFLSHRPRT_MAINLINK ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_INCDNTFLSHRPRT', 'MAINLINK') ||
        safeFormGetField(FormGetField, formTag, 'MAINLINK') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Flash Report record. Please select a record first.');
      return;
    }

    // C++: rejectFlashFromReview(strPrmryKey);
    // C++: strSQL.Format("EXECUTE RejectFlashFromReview '%s'",strPrmryKey);
    const rejectSql = `EXECUTE RejectFlashFromReview '${keyFieldValue.toString().replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Rejecting Flash Report from Review:', rejectSql);
    try {
      await executeSQLAsync(rejectSql);
      console.log('[Web_HSE] ✓ Flash Report rejected successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: RejectFlashFromReview');
        console.error('[Web_HSE] Expected column name format: INCDNTFLSHRPRT_AREANAM or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Flash Report ${keyFieldValue} rejected successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleFlashReportRejectButton:', error);
    toast.error('An error occurred while rejecting the Flash Report');
  }
}

/**
 * Handle Flash Report Accept button click from Review screen
 * C++ Reference: IncdntFlshRprtCategory.cpp line 155-156: INCDNTFLSHRPRTRVEW_ACCPT button handler
 * 
 * @param {string} buttonName - The button name (e.g. INCDNTFLSHRPRTRVEW_ACCPT)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleFlashReportAcceptButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Flash Report Accept button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGINCDNTFLSHRPRTRVEW';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (MAINLINK for Flash Report)
    // C++: CString strPrmryKey = FormGetField("HSE_INCDNTFLSHRPRT","MAINLINK");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.MAINLINK ||
        firstRecord?.INCDNTFLSHRPRT_MAINLINK ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_INCDNTFLSHRPRT', 'MAINLINK') ||
        safeFormGetField(FormGetField, formTag, 'MAINLINK') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Flash Report record. Please select a record first.');
      return;
    }

    // C++: accptFlashFromReview(strPrmryKey);
    // C++: strSQL.Format("EXECUTE AccptFlashFromReview '%s'",strPrmryKey);
    const acceptSql = `EXECUTE AccptFlashFromReview '${keyFieldValue.toString().replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Accepting Flash Report from Review:', acceptSql);
    try {
      await executeSQLAsync(acceptSql);
      console.log('[Web_HSE] ✓ Flash Report accepted successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: AccptFlashFromReview');
        console.error('[Web_HSE] Expected column name format: INCDNTFLSHRPRT_AREANAM or WorkLoc');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Flash Report ${keyFieldValue} accepted successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleFlashReportAcceptButton:', error);
    toast.error('An error occurred while accepting the Flash Report');
  }
}

/**
 * Handle Incident Medical Report Close button click
 * C++ Reference: HSEMSCommonCategory.cpp line 650-652: INCDNTMDCLRPRT_CLS button handler
 * 
 * @param {string} buttonName - The button name (e.g. INCDNTMDCLRPRT_CLS)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleCloseIncidentMedicalReportButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Close Incident Medical Report button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = 'HSE_INCDNTMDCLRPRT'; // Form table name (not screen tag)

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (IncdntMdclRprt_FlshNo)
    // C++: CString strKeyVal = FormGetField("HSE_INCDNTMDCLRPRT","IncdntMdclRprt_FlshNo");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.IncdntMdclRprt_FlshNo ||
        firstRecord?.INCDNTMDCLRPRT_FLSHNO ||
        firstRecord?.FlshNo ||
        '';
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, formTag, 'IncdntMdclRprt_FlshNo') ||
        safeFormGetField(FormGetField, formTag, 'INCDNTMDCLRPRT_FLSHNO') ||
        safeFormGetField(FormGetField, screenTag || 'HSE_TGINCDNTMDCLRPRTFLWUP', 'IncdntMdclRprt_FlshNo') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Incident Medical Report record. Please select a record first.');
      return;
    }

    // C++: closeIncdntMdclRprt(strKeyVal);
    // C++: strSQL.Format("EXECUTE closeIncdntMdclRprt '%s'",strKeyVal);
    const closeSql = `EXECUTE closeIncdntMdclRprt '${keyFieldValue.toString().replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Closing Incident Medical Report:', closeSql);
    try {
      await executeSQLAsync(closeSql);
      console.log('[Web_HSE] ✓ Incident Medical Report closed successfully');
    } catch (dbError) {
      // Extract user-friendly error message from response
      // Server returns: { userError: errMsg, error: errMsg, errObj: err }
      const userErrorMessage = dbError?.response?.data?.userError || 
                               dbError?.response?.data?.error || 
                               dbError?.message || 
                               dbError?.toString() || 
                               '';
      
      // Check for database schema errors
      if (userErrorMessage.includes('WRKLOK') || userErrorMessage.includes('WRKLOC') || userErrorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', userErrorMessage);
        const columnMatch = userErrorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: closeIncdntMdclRprt');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      
      // Display user-friendly validation error message from stored procedure
      if (userErrorMessage) {
        console.error('[Web_HSE] Stored procedure validation error:', userErrorMessage);
        toast.warning(userErrorMessage);
        return;
      }
      
      // If no user-friendly message, show generic error
      console.error('[Web_HSE] Error closing Incident Medical Report:', dbError);
      toast.error('An error occurred while closing the Incident Medical Report');
      return;
    }

    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Incident Medical Report ${keyFieldValue} closed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleCloseIncidentMedicalReportButton:', error);
    toast.error('An error occurred while closing the Incident Medical Report');
  }
}

