import { toast } from "react-toastify";

/**
 * SiteSurveyButtonHandlers.js
 * 
 * Handles all button click events for Site Survey module screens
 * 
 * C++ Reference Files:
 * - SitSrvyEntCategory.cpp
 * - SitSrvyFlwupCategory.cpp
 * - SitSrvyCnfrmtnCategory.cpp
 * - SitSrvyCategory.cpp
 */

// Store pending reject Site Survey execution info (module-level)
let pendingRejectSiteSurvey = null;

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

export function setPendingRejectSiteSurvey(siteSurveyInfo) {
  pendingRejectSiteSurvey = siteSurveyInfo;
}

export function clearPendingRejectSiteSurvey() {
  pendingRejectSiteSurvey = null;
}

export function hasPendingRejectSiteSurvey() {
  return pendingRejectSiteSurvey !== null;
}

/**
 * Handle Entry Completed button click for Site Survey Entry
 * 
 * Requirement:
 * - RQ_HSM_HSE_05_01_26_10_20.7: Entry Completed
 * 
 * This function implements the "Entry Completed" functionality for Site Survey Entry screen.
 * When the user clicks the "Entry Completed" button, it:
 * 1. Validates that a record has been saved (selected)
 * 2. Executes the completeSitSrvyTXN stored procedure to:
 *    - Insert a tracing record with "Entry Completed" action description
 *    - Update the site survey status to "Completed"
 *    - Set the source screen name and entry user
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: SitSrvyEntCategory.cpp line 23-36: SITSRVYENT_ENTCMPLT button handler
 * C++ Reference: SitSrvyEntCategory.cpp line 41-52: CompleteSitSrvy() helper function
 * 
 * @param {string} buttonName - The button name (e.g. SITSRVYENT_EntCmplt, SITSRVYENT_ENTCMPLT)
 * @param {string} screenTag - The screen tag (e.g. HSE_TgSitSrvyEnt)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 * 
 * @example
 * // Called when user clicks "Entry Completed" button on Site Survey Entry screen
 * await handleEntryCompletedButton('SITSRVYENT_EntCmplt', 'HSE_TgSitSrvyEnt', eventObj, devInterface);
 */
export async function handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
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
    const formTag = screenTag || 'HSE_TgSitSrvyEnt';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // C++: Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
    // C++: if( Count == 0) { AfxMessageBox("You must save the Record first"); return false; }
    const recordCount = (fullRecordArrKeys && fullRecordArrKeys.length > 0) 
      ? fullRecordArrKeys.length 
      : (fullRecordArr && Array.isArray(fullRecordArr) ? fullRecordArr.length : (fullRecordArr ? 1 : 0));

    if (recordCount === 0) {
      toast.warning('You must save the Record first');
      return;
    }

    // Get key field value (Site Survey number)
    // C++: CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
    // C++ Reference: HSEMSCommonCategory.cpp line 5653-5654: For SITSRVY, key field is "SITSRVYENT_SITSRVYNO"
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      keyFieldValue = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        keyFieldValue = String(
          firstRecord.SITSRVYENT_SITSRVYNO ||
          firstRecord.SitSrvyEnt_SitSrvyNo ||
          firstRecord.SITSRVYNO ||
          firstRecord.SitSrvyNo ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SITSRVYENT', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, formTag, 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SitSrvyEnt_SitSrvyNo') ||
        safeFormGetField(FormGetField, formTag, 'SitSrvyEnt_SitSrvyNo') ||
        '';
    }

    if (!keyFieldValue || keyFieldValue === 'undefined' || keyFieldValue === 'null') {
      toast.warning('Unable to find Site Survey number. Please select a record first.');
      return;
    }

    // Ensure keyFieldValue is a string
    keyFieldValue = String(keyFieldValue).trim();
    if (!keyFieldValue) {
      toast.warning('Site Survey number is empty. Please select a valid record first.');
      return;
    }

    // C++: CompleteSitSrvy(pCustomButtonClicked);
    // C++: strSQL.Format("EXECUTE completeSitSrvyTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const userName = String(getUserName() || '').trim();
    const strSourceScreenName = 'Site Survey Entry'; // Screen caption for HSE_TgSitSrvyEnt
    
    // Validate all parameters before building SQL
    if (!userName) {
      console.error('[Web_HSE] User name is empty or undefined');
      toast.error('System error: Unable to get user name');
      return;
    }

    const completeSql = `EXECUTE completeSitSrvyTXN '${keyFieldValue.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    // Log parameter values for debugging
    console.log('[Web_HSE] Site Survey Entry Completed parameters:', {
      keyFieldValue,
      strSourceScreenName,
      userName,
    });

    console.log('[Web_HSE] Completing Site Survey entry:', completeSql);
    try {
      await executeSQLAsync(completeSql);
      console.log('[Web_HSE] ✓ Site Survey entry completed successfully');
    } catch (dbError) {
      // Check if error is about invalid column name
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: completeSitSrvyTXN');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError; // Re-throw if it's a different error
    }

    // Refresh screen
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Site Survey ${keyFieldValue} entry completed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleEntryCompletedButton:', error);
    toast.error('An error occurred while completing the Site Survey entry');
  }
}

/**
 * Handle Close button click for Site Survey Follow-up
 * 
 * This function implements the "Close" functionality for Site Survey Follow-up screen.
 * When the user clicks the "Close" button, it:
 * 1. Saves the current record (DoToolBarAction TOOLBAR_SAVE)
 * 2. Executes the closeSiteSurveyTXN stored procedure to:
 *    - Update the site survey status to "Closed"
 *    - Insert a tracing record with closure information
 *    - Set the source screen name and entry user
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: SitSrvyFlwupCategory.cpp line 29-38: SITSRVYENT_CLS button handler
 * C++ Reference: SitSrvyFlwupCategory.cpp line 40-49: CloseSiteSurveyTXN() helper function
 * 
 * @param {string} buttonName - The button name (e.g. SITSRVYENT_CLS)
 * @param {string} screenTag - The screen tag (e.g. HSE_TgSitSrvyFlwup)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, doToolbarAction, etc.)
 * 
 * @example
 * // Called when user clicks "Close" button on Site Survey Follow-up screen
 * await handleCloseSiteSurveyButton('SITSRVYENT_CLS', 'HSE_TgSitSrvyFlwup', eventObj, devInterface);
 */
export async function handleCloseSiteSurveyButton(buttonName, screenTag, eventObj, devInterface) {
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
      console.error('[Web_HSE] Missing required devInterface functions for Close Site Survey button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TgSitSrvyFlwup';

    // C++: DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
    // Save before closing
    if (doToolbarAction) {
      doToolbarAction('SAVE', formTag, '');
    }

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Site Survey number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      keyFieldValue = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        keyFieldValue = String(
          firstRecord.SITSRVYENT_SITSRVYNO ||
          firstRecord.SitSrvyEnt_SitSrvyNo ||
          firstRecord.SITSRVYNO ||
          firstRecord.SitSrvyNo ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SITSRVYENT', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, formTag, 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SitSrvyEnt_SitSrvyNo') ||
        safeFormGetField(FormGetField, formTag, 'SitSrvyEnt_SitSrvyNo') ||
        '';
      keyFieldValue = String(keyFieldValue).trim();
    }

    if (!keyFieldValue || keyFieldValue === 'undefined' || keyFieldValue === 'null') {
      toast.warning('Unable to find Site Survey number. Please select a record first.');
      return;
    }

    // C++: CloseSiteSurveyTXN(pCustomButtonClicked);
    // C++: strSQL.Format("EXECUTE closeSiteSurveyTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
    // Note: Parameter order is KeyFieldValue, strUserName, strSourceScreenName (different from complete!)
    const userName = String(getUserName() || '').trim();
    const strSourceScreenName = 'Site Survey Follow-up'; // Screen caption for HSE_TgSitSrvyFlwup
    
    if (!userName) {
      console.error('[Web_HSE] User name is empty or undefined');
      toast.error('System error: Unable to get user name');
      return;
    }

    const closeSql = `EXECUTE closeSiteSurveyTXN '${keyFieldValue.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Closing Site Survey:', closeSql);
    console.log('[Web_HSE] Site Survey Close parameters:', {
      keyFieldValue,
      userName,
      strSourceScreenName,
    });

    try {
      await executeSQLAsync(closeSql);
      console.log('[Web_HSE] ✓ Site Survey closed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: closeSiteSurveyTXN');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Site Survey ${keyFieldValue} closed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleCloseSiteSurveyButton:', error);
    toast.error('An error occurred while closing the Site Survey');
  }
}

/**
 * Handle Confirm button click for Site Survey Confirmation
 * 
 * This function implements the "Confirm" functionality for Site Survey Confirmation screen.
 * When the user clicks the "Confirm" button, it:
 * 1. Validates that a record is selected
 * 2. Executes the confirmSiteSrvy stored procedure to:
 *    - Update the site survey status to "Confirmed"
 *    - Insert a tracing record with confirmation information
 *    - Set the source screen name and entry user
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: SitSrvyCnfrmtnCategory.cpp line 23-33: SITSRVYENT_CNFRM button handler
 * C++ Reference: SitSrvyCnfrmtnCategory.cpp line 44-53: confirmSiteSrvy() helper function
 * 
 * @param {string} buttonName - The button name (e.g. SITSRVYENT_CNFRM)
 * @param {string} screenTag - The screen tag (e.g. HSE_TgSitSrvyCnfrmtn)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 * 
 * @example
 * // Called when user clicks "Confirm" button on Site Survey Confirmation screen
 * await handleConfirmSiteSurveyButton('SITSRVYENT_CNFRM', 'HSE_TgSitSrvyCnfrmtn', eventObj, devInterface);
 */
export async function handleConfirmSiteSurveyButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Confirm Site Survey button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TgSitSrvyCnfrmtn';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Site Survey number)
    // C++: CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
    // C++: if(KeyFieldValue != "") confirmSiteSrvy(pCustomButtonClicked);
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      keyFieldValue = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        keyFieldValue = String(
          firstRecord.SITSRVYENT_SITSRVYNO ||
          firstRecord.SitSrvyEnt_SitSrvyNo ||
          firstRecord.SITSRVYNO ||
          firstRecord.SitSrvyNo ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SITSRVYENT', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, formTag, 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SitSrvyEnt_SitSrvyNo') ||
        safeFormGetField(FormGetField, formTag, 'SitSrvyEnt_SitSrvyNo') ||
        '';
      keyFieldValue = String(keyFieldValue).trim();
    }

    if (!keyFieldValue || keyFieldValue === 'undefined' || keyFieldValue === 'null') {
      toast.warning('Unable to find Site Survey number. Please select a record first.');
      return;
    }

    // C++: confirmSiteSrvy(pCustomButtonClicked);
    // C++: strSQL.Format("EXECUTE confirmSiteSrvy '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const userName = String(getUserName() || '').trim();
    const strSourceScreenName = 'Site Survey Confirmation'; // Screen caption for HSE_TgSitSrvyCnfrmtn
    
    if (!userName) {
      console.error('[Web_HSE] User name is empty or undefined');
      toast.error('System error: Unable to get user name');
      return;
    }

    const confirmSql = `EXECUTE confirmSiteSrvy '${keyFieldValue.replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Confirming Site Survey:', confirmSql);
    console.log('[Web_HSE] Site Survey Confirm parameters:', {
      keyFieldValue,
      strSourceScreenName,
      userName,
    });

    try {
      await executeSQLAsync(confirmSql);
      console.log('[Web_HSE] ✓ Site Survey confirmed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: confirmSiteSrvy');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }

    // Refresh screen
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Site Survey ${keyFieldValue} confirmed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleConfirmSiteSurveyButton:', error);
    toast.error('An error occurred while confirming the Site Survey');
  }
}

/**
 * Handle Reject button click for Site Survey
 * 
 * Requirement:
 * - RQ_HSM_HSE_05_01_26_13_58.7.2: Reject
 * 
 * This function implements the "Reject" functionality for Site Survey screens (Confirmation and Follow-up).
 * When the user clicks the "Reject" button, it:
 * 1. Opens the Reject Reason screen (HSE_TGRjctRsn) for the user to enter rejection reasons
 * 2. Stores pending rejection information to be processed when the user clicks OK in the reject reason screen
 * 3. The actual rejection is processed in handleRejectReasonOkButtonForSiteSurvey() when RJCTRSN_BTN_OK is clicked
 * 4. The rejection process executes the rejectSitSrvy stored procedure which:
 *    - Updates the site survey status to "Rejected"
 *    - Inserts a tracing record with rejection information
 *    - Sets the source screen name and entry user
 * 
 * C++ Reference: SitSrvyCategory.cpp line 130-147: SITSRVYENT_RJCT button handler
 * C++ Reference: SitSrvyCategory.cpp line 181-194: rejectRecord() helper function
 * 
 * @param {string} buttonName - The button name (e.g. SITSRVYENT_RJCT)
 * @param {string} screenTag - The screen tag (e.g. HSE_TgSitSrvyCnfrmtn, HSE_TgSitSrvyFlwup)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, openScr, etc.)
 * 
 * @example
 * // Called when user clicks "Reject" button on Site Survey Confirmation or Follow-up screen
 * await handleRejectSiteSurveyButton('SITSRVYENT_RJCT', 'HSE_TgSitSrvyCnfrmtn', eventObj, devInterface);
 */
export async function handleRejectSiteSurveyButton(buttonName, screenTag, eventObj, devInterface) {
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
      console.error('[Web_HSE] Missing required devInterface functions for Reject Site Survey button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TgSitSrvyCnfrmtn';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Site Survey number)
    // C++: CString strSrvyNo=FormGetField("HSE_SITSRVYENT","SITSRVYENT_SITSRVYNO");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      keyFieldValue = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        keyFieldValue = String(
          firstRecord.SITSRVYENT_SITSRVYNO ||
          firstRecord.SitSrvyEnt_SitSrvyNo ||
          firstRecord.SITSRVYNO ||
          firstRecord.SitSrvyNo ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SITSRVYENT', 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, formTag, 'SITSRVYENT_SITSRVYNO') ||
        safeFormGetField(FormGetField, 'HSE_SitSrvyEnt', 'SitSrvyEnt_SitSrvyNo') ||
        safeFormGetField(FormGetField, formTag, 'SitSrvyEnt_SitSrvyNo') ||
        '';
      keyFieldValue = String(keyFieldValue).trim();
    }

    if (!keyFieldValue || keyFieldValue === 'undefined' || keyFieldValue === 'null') {
      toast.warning('Unable to find Site Survey number. Please select a record first.');
      return;
    }

    // Determine module type based on screen tag
    // C++: rejectRecord() uses module type "SITSRVYENT-L1" for confirmation screen, "SITSRVYENT-L2" for follow-up
    const normalizedFormTag = formTag.toUpperCase();
    let moduleType = 'SITSRVYENT-L1'; // Default for confirmation screen
    if (normalizedFormTag.includes('CNFRMTN') || normalizedFormTag.includes('CONFIRMATION')) {
      moduleType = 'SITSRVYENT-L1';
    } else if (normalizedFormTag.includes('FLWUP') || normalizedFormTag.includes('FOLLOWUP') || normalizedFormTag.includes('FOLLOW')) {
      moduleType = 'SITSRVYENT-L2';
    }

    // C++: rejectRecord() opens the reject reason screen
    // C++: Reject("SITSRVYENT-L1",strSrvyNo); or Reject("SITSRVYENT-L2",strSrvyNo);
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
    
    setPendingRejectSiteSurvey({
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
    console.log('[Web_HSE] Opening reject reason screen for Site Survey:', {
      screenTag: rejectReasonScreenTag,
      siteSurveyNumber: keyFieldValue,
      moduleType: moduleType,
    });
    
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, defValObj);

    console.log('[Web_HSE] Reject reason screen opened. Waiting for OK button click (RJCTRSN_BTN_OK)...');
    console.log('[Web_HSE] Pending rejection info stored:', {
      siteSurveyNumber: keyFieldValue,
      screenTag: formTag,
    });
  } catch (error) {
    console.error('[Web_HSE] Error in handleRejectSiteSurveyButton:', error);
    toast.error('An error occurred while processing the Reject Site Survey action');
  }
}

/**
 * Handle Reject Reason screen OK button click for Site Survey
 * C++: CRejectReason::RejectReasonOk() - sets gbUpdateRejectedRecord = true
 * This is called when RJCTRSN_BTN_OK is clicked in the reject reason screen
 * 
 * C++ Reference: SitSrvyCategory.cpp line 136-146: Processing after reject reason OK
 * 
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRejectReasonOkButtonForSiteSurvey(devInterface) {
  try {
    if (!pendingRejectSiteSurvey) {
      console.warn('[Web_HSE] ⚠ No pending rejection info found. Stored procedure will not execute.');
      return;
    }

    console.log('[Web_HSE] Processing Site Survey rejection and inserting tracing record...');
    
    const { keyFieldValue, formTag, getUserName, executeSQLAsync, refreshData, toast, FormSetField, doToolbarAction } = pendingRejectSiteSurvey;
    const userName = String(getUserName() || '').trim();
    
    // Determine source screen name based on screen tag
    const normalizedFormTag = formTag.toUpperCase();
    let strSourceScreenName = 'Site Survey Confirmation'; // Default
    if (normalizedFormTag.includes('CNFRMTN') || normalizedFormTag.includes('CONFIRMATION')) {
      strSourceScreenName = 'Site Survey Confirmation';
    } else if (normalizedFormTag.includes('FLWUP') || normalizedFormTag.includes('FOLLOWUP') || normalizedFormTag.includes('FOLLOW')) {
      strSourceScreenName = 'Site Survey Follow-up';
    }
    
    // C++: strSQL.Format("EXECUTE rejectSitSrvy '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    const rejectSql = `EXECUTE rejectSitSrvy '${keyFieldValue.toString().replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Rejecting Site Survey:', rejectSql);
    try {
      await executeSQLAsync(rejectSql);
      console.log('[Web_HSE] ✓ Site Survey rejected successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      if (errorMessage.includes('WRKLOK') || errorMessage.includes('WRKLOC') || errorMessage.includes('invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'WRKLOC/WRKLOK';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure definitions.`);
        console.error('[Web_HSE] Stored procedure called: rejectSitSrvy');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        return;
      }
      throw dbError;
    }
    
    // Refresh screen
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Site Survey ${keyFieldValue} rejected successfully`);
    
    // Clear pending execution
    clearPendingRejectSiteSurvey();
  } catch (error) {
    console.error('[Web_HSE] ✗ Error executing rejectSitSrvy:', error);
    if (pendingRejectSiteSurvey && pendingRejectSiteSurvey.toast) {
      pendingRejectSiteSurvey.toast.error('Error rejecting Site Survey: ' + (error.message || 'Unknown error'));
    }
    clearPendingRejectSiteSurvey();
  }
}

