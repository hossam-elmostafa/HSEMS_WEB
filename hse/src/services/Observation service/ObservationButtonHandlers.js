import { toast } from "react-toastify";

/**
 * ObservationButtonHandlers.js
 * 
 * Handles all button click events for Observation module screens
 * 
 * Requirements:
 * - RQ_HSM_22_12_09_57: View Reject Reason (Observation Entry)
 * - RQ_HSM_22_12_10_48: Handle Confirm Custom Button
 * - RQ_HSM_22_12_10_50: Handle Reject Custom Button
 * - RQ_HSM_22_12_10_55: Handle Cancel Custom Button
 * - RQ_HSM_22_12_11_10: Handle View Reject Reason Custom Button
 * - RQ_HSM_22_12_11_28: Handle Close Custom Button
 * - RQ_HSM_22_12_25_09_35: Implement Entry Completed
 * - RQ_HSM_22_12_25_11_43: Handle Reward Entry Completed Custom Button
 */

// Store pending reject observation execution info (module-level)
let pendingRejectObservation = null;

export function setPendingRejectObservation(observationInfo) {
  pendingRejectObservation = observationInfo;
}

export function clearPendingRejectObservation() {
  pendingRejectObservation = null;
}

/**
 * Handle View Reject Reason custom button click on Observation tracing tab
 * RQ_HSM_22_12_09_57: View Reject Reason (Observation Entry)
 * RQ_HSM_22_12_11_10: Handle View Reject Reason Custom Button
 * ClickUp Task: https://app.clickup.com/t/86c76vt58
 * Mirrors C++: CHSEMSCommonCategory::viewRejectReason for buttons ending with _VWRJCTRSNS
 * - Opens HSE_TGRJCTRSN in (effectively) locked mode to show existing reject reasons
 * @param {string} buttonName - The button name (e.g. NRSTMISCENTTRC_VWRJCTRSNS)
 * @param {string} screenTag - The screen tag (owner screen)
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewRejectReasonButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const { FormGetField, openScr } = devInterface;

    if (!openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View Reject Reason button');
      toast.error('System error: Required functions not available');
      return;
    }

    const rawBtnName = buttonName ? buttonName.toString() : '';
    if (!rawBtnName.toUpperCase().endsWith('_VWRJCTRSNS')) {
      console.warn('[Web_HSE] View Reject Reason handler called for non _VWRJCTRSNS button:', rawBtnName);
      return;
    }

    // Derive table and field names from button name
    // C++:
    //   strTableName = "HSE_" + (BtnName without "_VWRJCTRSNS")
    //   strActionType = <base> + "_ACCDESC"
    //   strFieldName  = <base> + "_MAINLNK"
    //   strLinkField  = <base> + "_LNK"
    const baseName = rawBtnName.replace(/_VWRJCTRSNS$/i, '');

    // Prefer values from the selected tracing row (fullRecord) instead of FormGetField,
    // because tracing tab fields are part of a sub-grid.
    const { fullRecord: fullRecordArr } = eventObj || {};
    const firstRecord = Array.isArray(fullRecordArr)
      ? fullRecordArr[0]
      : fullRecordArr || {};

    let linkValue =
      firstRecord?.NRSTMISCENTTRC_LNK ??
      firstRecord?.NRSTMISCENTTRC_LNK?.toString?.() ??
      '';

    // Normalize link value to string to safely use replace()
    if (linkValue !== null && linkValue !== undefined) {
      linkValue = String(linkValue);
    }

    if (!linkValue) {
      toast.warning('Unable to find linked Observation number for reject reasons.');
      return;
    }

    // For Observation module the base module name is NRSTMISCENT
    // C++: strModulefrmSource is used in "RJCTRSN_MODULETYPE like '<src>%'" (to match L1/L2)
    const moduleSource = 'NRSTMISCENT';

    // Use plain WHERE clause for list screen criteria (server builds full SELECT)
    // Remove any extra whitespace/newlines to ensure clean SQL
    const criteria = `WHERE RJCTRSN_LINKWITHMAIN = '${linkValue.replace(/'/g, "''")}' AND RJCTRSN_MODULETYPE LIKE '${moduleSource.replace(/'/g, "''")}%'`.trim();

    console.log('[Web_HSE] Opening View Reject Reason screen with criteria:', criteria);
    console.log('[Web_HSE] Screen tag:', 'HSE_TGRjctRsn', 'Link value:', linkValue);

    // Open reject reason screen in read-only/locked mode
    // C++: setgbOpenRejectScreenLocked(true) then ShowScreen(..., true) for locked
    // Use correct screen tag from config: HSE_TGRjctRsn (not HSE_TGRJCTRSN)
    // Use 'list' mode with bLockScr=true (8th parameter) to make it read-only
    // Pass empty string for criteria to let server handle it, or pass as WHERE clause
    const rejectReasonScreenTag = 'HSE_TGRjctRsn'; // Match the exact tag from header.json
    openScr(rejectReasonScreenTag, {}, criteria, 'list', false, {}, false, true);
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewRejectReasonButton:', error);
    toast.error('An error occurred while opening reject reasons.');
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
export async function handleRejectButton(buttonName, screenTag, eventObj, devInterface) {
  // (RQ_HSM_22_12_11_21) Handle Reject Custom Button
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
    const rejectReasonScreenTag = 'HSE_TGRjctRsn';
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
    // C++: ShowRejectreason(strCriteria) -> opens HSE_TGRjctRsn screen
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
export async function handleConfirmButton(buttonName, screenTag, eventObj, devInterface) {
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
 * Handle Cancel button click
 * RQ_HSM_22_12_10_55: Handle Cancel Custom Button
 * ClickUp Task: https://app.clickup.com/t/86c76v9yr
 * Implements the logic from NearMissCategory::DisplayCustomButtonClicked for NRSTMISCENT_CNCL
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleCancelButton(buttonName, screenTag, eventObj, devInterface) {
  // (RQ_HSM_22_12_11_30) Handle Cancel Custom Button
  try {
    const {
      FormGetField,
      FormSetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      AskYesNoMessage,
      doToolbarAction,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      !FormSetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !AskYesNoMessage ||
      !doToolbarAction
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Cancel button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGNRSTMISCCNFRMTN';

    // Get key field value and status from event object first (more reliable)
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};
    let keyFieldValue = '';
    let currentStatus = '';

    // Try to get values from event object first
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.NRSTMISCENT_NRSTMISCNUM ||
        firstRecord?.NRSTMISCNUM ||
        firstRecord?.NrstMiscEnt_NrstMiscNum ||
        '';
      currentStatus =
        firstRecord?.NRSTMISCENT_RECSTS ||
        firstRecord?.RECSTS ||
        '';
    }

    // Fallback: read from form if still empty
    if (!currentStatus) {
      // Try view name first (most reliable), then form tag
      // Wrap in try-catch to handle cases where form/view doesn't exist
      try {
        currentStatus = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_RECSTS') || '';
      } catch (e) {
        // Ignore error, try next option
      }
      if (!currentStatus) {
        try {
          currentStatus = FormGetField(formTag, 'NRSTMISCENT_RECSTS') || '';
        } catch (e) {
          // Ignore error, use empty string
        }
      }
    }

    // C++: Check if status is already 99 (cancelled)
    // Only check if we successfully retrieved the status
    if (currentStatus && (currentStatus === '99' || currentStatus === 99)) {
      toast.info('Record is already cancelled');
      return;
    }

    // C++: MessageBox(NULL,"Are you sure to cancel the observation - (Yes/No)?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
    const confirmed = await AskYesNoMessage(
      'Prompt',
      'Are you sure to cancel the observation - (Yes/No)?'
    );

    if (!confirmed) {
      // User clicked No, ignore the action
      return;
    }

    // C++: FormSetField(strForm_Tag,"NRSTMISCENT_RECSTS","99");
    FormSetField(formTag, 'NRSTMISCENT_RECSTS', '99');

    // Get observation number (use value from event if we already have it)
    // C++: FormGetField(strForm_Tag,"NrstMiscEnt_NrstMiscNum")
    if (!keyFieldValue) {
      // Wrap in try-catch to handle cases where form/view doesn't exist
      try {
        keyFieldValue = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') || '';
      } catch (e) {
        // Ignore error, try next option
      }
      if (!keyFieldValue) {
        try {
          keyFieldValue = FormGetField(formTag, 'NrstMiscEnt_NrstMiscNum') || '';
        } catch (e) {
          // Ignore error, try next option
        }
      }
      if (!keyFieldValue) {
        try {
          keyFieldValue = FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') || '';
        } catch (e) {
          // Ignore error, use empty string
        }
      }
    }

    const linkFieldVal = keyFieldValue;

    if (!linkFieldVal) {
      toast.warning('Unable to find Observation number. Please save the record first.');
      return;
    }

    // Get screen name based on form tag
    // C++:
    //   if (strForm_Tag=="HSE_TGNRSTMISCCNFRMTN") strScreenName="Observation Review";
    //   else if (strForm_Tag=="HSE_TGNRSTMISCFLWUP") strScreenName="Observation Approval";
    let screenName = '';
    const normalizedFormTag = formTag.toUpperCase();
    if (normalizedFormTag === 'HSE_TGNRSTMISCCNFRMTN' || normalizedFormTag.includes('CNFRMTN')) {
      screenName = 'Observation Review';
    } else if (normalizedFormTag === 'HSE_TGNRSTMISCFLWUP' || normalizedFormTag.includes('FLWUP')) {
      screenName = 'Observation Approval';
    } else {
      screenName = 'Observation Entry'; // Default fallback
    }

    // Get user name
    const userName = getUserName() || '';

    // C++: Insert tracing record
    // C++: strSQL.Format("insert into HSE_NRSTMISCENTTRC (NRSTMISCENTTRC_DATTIM,NRSTMISCENTTRC_ACCDESC,NRSTMISCENTTRC_LNK,NRSTMISCENTTRC_ENTUSR,SRCSCRN) values (getdate(),'Canceled',%s,'%s','%s')",linkFieldVal,strUserName,strScreenName);
    // Note: linkFieldVal is inserted as a number (no quotes), userName and screenName are strings (with quotes)
    // Ensure linkFieldVal is a number (remove any quotes or convert to number)
    const linkFieldValNum = typeof linkFieldVal === 'string' ? linkFieldVal.replace(/['"]/g, '') : linkFieldVal;
    const insertTracingSql = `INSERT INTO HSE_NRSTMISCENTTRC (NRSTMISCENTTRC_DATTIM,NRSTMISCENTTRC_ACCDESC,NRSTMISCENTTRC_LNK,NRSTMISCENTTRC_ENTUSR,SRCSCRN) VALUES (getdate(),'Canceled',${linkFieldValNum},'${userName.replace(/'/g, "''")}','${screenName.replace(/'/g, "''")}')`;

    console.log('[Web_HSE] Inserting Cancel tracing record with SQL:', insertTracingSql);
    console.log('[Web_HSE] Link field value:', linkFieldValNum, 'Type:', typeof linkFieldValNum);

    try {
      // Get timestamp just before inserting "Canceled" record
      // This helps us identify and delete any "Completed" records inserted after
      const getMaxTimestampSql = `SELECT MAX(NRSTMISCENTTRC_DATTIM) as MaxTime FROM HSE_NRSTMISCENTTRC WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum}`;
      let maxTimestampBefore = null;
      try {
        const maxTimeResult = await executeSQLAsync(getMaxTimestampSql);
        if (maxTimeResult?.recordsets?.[0]?.[0]?.MaxTime) {
          const timestampValue = maxTimeResult.recordsets[0][0].MaxTime;
          // SQL Server returns datetime as a string or Date object - handle both
          if (timestampValue instanceof Date) {
            maxTimestampBefore = timestampValue.toISOString().replace('T', ' ').substring(0, 23);
          } else if (typeof timestampValue === 'string') {
            maxTimestampBefore = timestampValue;
          } else {
            // Convert to string if it's some other format
            maxTimestampBefore = String(timestampValue);
          }
          console.log('[Web_HSE] Max timestamp before Cancel:', maxTimestampBefore, typeof maxTimestampBefore);
        }
      } catch (e) {
        console.warn('[Web_HSE] Could not get max timestamp:', e);
      }

      const insertResult = await executeSQLAsync(insertTracingSql);
      console.log('[Web_HSE] ✓ Tracing record inserted for cancelled observation. Result:', insertResult);

      // C++: DoToolBarAction(TOOLBAR_SAVE,strForm_Tag,"");
      doToolbarAction('SAVE', formTag, '');

      // After save, delete any "Completed" record that was inserted after our "Canceled" record
      // The save action might insert a "Completed" record, but we want "Canceled" to be the final action
      // Use a delay to ensure save completes, then delete "Completed" records and refresh
      setTimeout(async () => {
        try {
          let deletedRows = 0;
          if (maxTimestampBefore) {
            // maxTimestampBefore is already a SQL datetime string, use it directly
            const deleteCompletedSql = `DELETE FROM HSE_NRSTMISCENTTRC 
                                        WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum} 
                                        AND NRSTMISCENTTRC_ACCDESC = 'Completed' 
                                        AND NRSTMISCENTTRC_DATTIM > '${maxTimestampBefore}'`;
            const deleteResult = await executeSQLAsync(deleteCompletedSql);
            deletedRows = deleteResult?.rowsAffected?.[0] || 0;
            console.log('[Web_HSE] Deleted "Completed" record(s) inserted after "Canceled". Rows affected:', deletedRows);
          } else {
            // Fallback: delete any "Completed" record inserted in the last 5 seconds
            const deleteCompletedSql = `DELETE FROM HSE_NRSTMISCENTTRC 
                                        WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum} 
                                        AND NRSTMISCENTTRC_ACCDESC = 'Completed' 
                                        AND NRSTMISCENTTRC_DATTIM >= DATEADD(SECOND, -5, GETDATE())`;
            const deleteResult = await executeSQLAsync(deleteCompletedSql);
            deletedRows = deleteResult?.rowsAffected?.[0] || 0;
            console.log('[Web_HSE] Deleted recent "Completed" record(s) (fallback method). Rows affected:', deletedRows);
          }

          // Verify the "Canceled" record exists and check all tracing records
          const verifyCanceledSql = `SELECT TOP 1 NRSTMISCENTTRC_ACCDESC, NRSTMISCENTTRC_DATTIM 
                                     FROM HSE_NRSTMISCENTTRC 
                                     WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum} 
                                     AND NRSTMISCENTTRC_ACCDESC = 'Canceled' 
                                     ORDER BY NRSTMISCENTTRC_DATTIM DESC`;
          const verifyResult = await executeSQLAsync(verifyCanceledSql);
          const canceledExists = verifyResult?.recordsets?.[0]?.length > 0;
          console.log('[Web_HSE] Verification - "Canceled" record exists:', canceledExists);

          // Check all tracing records for this observation (from table)
          const allTracingSql = `SELECT NRSTMISCENTTRC_ACCDESC, NRSTMISCENTTRC_DATTIM, NRSTMISCENTTRC_LNK 
                                FROM HSE_NRSTMISCENTTRC 
                                WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum} 
                                ORDER BY NRSTMISCENTTRC_DATTIM DESC`;
          const allTracingResult = await executeSQLAsync(allTracingSql);
          const allRecords = allTracingResult?.recordsets?.[0] || [];
          console.log('[Web_HSE] All tracing records for observation', linkFieldValNum, ':', allRecords);

          // Check if records are visible through the view (what the tab uses)
          const viewTracingSql = `SELECT NRSTMISCENTTRC_ACCDESC, NRSTMISCENTTRC_DATTIM, NrstMiscEntTrc_Lnk 
                                 FROM HSE_NrstMiscEntTrc 
                                 WHERE NrstMiscEntTrc_Lnk = ${linkFieldValNum} 
                                 ORDER BY NRSTMISCENTTRC_DATTIM DESC`;
          try {
            const viewTracingResult = await executeSQLAsync(viewTracingSql);
            const viewRecords = viewTracingResult?.recordsets?.[0] || [];
            console.log('[Web_HSE] Tracing records visible through VIEW HSE_NrstMiscEntTrc:', viewRecords.length, 'records');
            if (viewRecords.length === 0 && allRecords.length > 0) {
              console.error('[Web_HSE] ⚠️ Records exist in table but NOT in view! This indicates a view/field name mismatch.');
              console.error('[Web_HSE] Table has', allRecords.length, 'records, but view returns', viewRecords.length);
            }
          } catch (viewError) {
            console.warn('[Web_HSE] Could not query view HSE_NrstMiscEntTrc:', viewError);
            console.warn('[Web_HSE] This might indicate the view name or field name is different');
          }

          // C++: RefreshScreen("",REFRESH_SELECTED);
          // Refresh after deletion to show the correct tracing records
          // Note: refreshData should refresh both main form and tabs
          refreshData('', 'REFRESH_SELECTED');
          
          // Force multiple refreshes to ensure tab data loads
          // The Tracing tab might need the user to click on it, but we'll try to refresh it
          setTimeout(() => {
            refreshData('', 'REFRESH_SELECTED');
            console.log('[Web_HSE] Second refresh triggered for Tracing tab');
          }, 500);
          
          setTimeout(() => {
            refreshData('', 'REFRESH_SELECTED');
            console.log('[Web_HSE] Third refresh triggered for Tracing tab');
          }, 1500);
          
          if (deletedRows > 0) {
            console.log('[Web_HSE] Successfully removed "Completed" record. Tracing tab should now show "Canceled"');
            console.log('[Web_HSE] If Tracing tab is still empty, please click on the Tracing tab to refresh it');
          }
          
          // Log summary for debugging
          console.log('[Web_HSE] Cancel operation complete. Records in DB:', allRecords.length, 'Canceled records:', allRecords.filter(r => r.NRSTMISCENTTRC_ACCDESC === 'Canceled').length);
        } catch (deleteError) {
          console.warn('[Web_HSE] Could not delete "Completed" record:', deleteError);
          // Still refresh even if deletion failed
          refreshData('', 'REFRESH_SELECTED');
        }
      }, 1000); // Wait 1 second for save to complete

      toast.success('Observation cancelled successfully');
    } catch (error) {
      console.error('[Web_HSE] Error cancelling observation:', error);
      toast.error('Error cancelling observation. Please try again.');
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleCancelButton:', error);
    toast.error('An error occurred while processing the Cancel action');
  }
}

/**
 * Handle Reward Entry Complete button click
 * RQ_HSM_22_12_25_11_43: Handle Reward Entry Completed Custom Button
 * Implements the logic from NearMissReward::DisplayCustomButtonClicked for NRSTMISCENT_ENTCMPLT (Reward screen)
 * 
 * C++ Reference: NearMissReward::completeNearMissReward
 * This is separate from handleEntryCompleteButton - it's specifically for the Reward screen
 * 
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRewardEntryCompleteButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const { FormSetField, refreshData } = devInterface;
    
    // Check if required functions are available
    if (!FormSetField || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Reward Entry Complete button');
      toast.error('System error: Required functions not available');
      return;
    }

    // Extract event data
    // C++: CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
    const { strTabTag, fullRecordArrKeys } = eventObj || {};
    
    // C++: int Count = 0;
    //      if(strSubFormTag == "")
    //      Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
    //      if( Count == 0) {
    //          AfxMessageBox("You must save the Record first");
    //          return false;
    //      }
    // Check if record is saved (must have selected records)
    let recordCount = 0;
    if (!strTabTag || strTabTag === '') {
      // Main form - check if we have selected records (equivalent to pMultiSelectedRows->lCount)
      recordCount = fullRecordArrKeys ? fullRecordArrKeys.length : 0;
    }

    if (recordCount === 0) {
      toast.warning('You must save the Record first');
      return;
    }

    // C++: void NearMissReward::completeNearMissReward(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
    // {
    //     FormSetField("HSE_vwNRSTMISCENT","NRSTMISCENT_RWRDFLG","Y");
    //     RefreshScreen("",REFRESH_SELECTED);
    // }
    
    // Set the reward flag to "Y"
    // C++: FormSetField("HSE_vwNRSTMISCENT","NRSTMISCENT_RWRDFLG","Y");
    FormSetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_RWRDFLG', 'Y');
    
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    
    // Show success message (separate from entry completed message)
    toast.success('Observation reward entry completed successfully');
    
  } catch (error) {
    console.error('[Web_HSE] Error in handleRewardEntryCompleteButton:', error);
    toast.error('An error occurred while processing the Reward Entry Complete action');
  }
}

/**
 * Handle Entry Complete button click
 * RQ_HSM_22_12_25_09_35: Implement Entry Completed
 * Implements the logic from NearMissEntryCategory::DisplayCustomButtonClicked for NRSTMISCENT_ENTCMPLT
 * 
 * C++ Reference: NearMissEntryCategory::DisplayCustomButtonClicked (lines 29-68)
 * 
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleEntryCompleteButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const { FormGetField, executeSQL, executeSQLPromise, getUserName, refreshData } = devInterface;
    
    // Check if required functions are available
    if (!FormGetField || (!executeSQL && !executeSQLPromise) || !getUserName || !refreshData) {
      console.error('[Web_HSE] Missing required devInterface functions for Entry Complete button');
      toast.error('System error: Required functions not available');
      return;
    }
    
    // Prefer executeSQLPromise for async operations, fallback to executeSQL
    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    // C++: CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
    const { strTabTag, fullRecordArrKeys } = eventObj || {};
    
    // C++: int Count = 0;
    //      if(strSubFormTag == "")
    //      Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
    //      if( Count == 0) {
    //          AfxMessageBox("You must save the Record first");
    //          return false;
    //      }
    // Check if record is saved (must have selected records)
    // Rule: If (Near Miss Confirmation) checkbox [11@600] = "un-checked", then
    let recordCount = 0;
    if (!strTabTag || strTabTag === '') {
      // Main form - check if we have selected records (equivalent to pMultiSelectedRows->lCount)
      recordCount = fullRecordArrKeys ? fullRecordArrKeys.length : 0;
    }

    if (recordCount === 0) {
      toast.warning('You must save the Record first');
      return;
    }

    // Get key field value (Observation number)
    // C++: CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag, pCustomButtonClicked->pMultiSelectedRows);
    let observationId = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      // Get key from selected records (most reliable, equivalent to GetKeyField)
      observationId = fullRecordArrKeys[0].toString();
    } else {
      // Fallback: try to get from form field
      // Try multiple form tags as fallback (view name is more reliable)
      try {
        observationId = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') || '';
      } catch (e) {
        // Ignore error, try next option
      }
      if (!observationId) {
        try {
          observationId = FormGetField('HSE_TgNrstMiscEnt', 'NRSTMISCENT_NRSTMISCNUM') || '';
        } catch (e) {
          // Ignore error, try next option
        }
      }
      if (!observationId && screenTag) {
        try {
          observationId = FormGetField(screenTag, 'NRSTMISCENT_NRSTMISCNUM') || '';
        } catch (e) {
          // Ignore error
        }
      }
    }
    
    if (!observationId) {
      toast.warning('Unable to find Observation ID. Please save the record first.');
      return;
    }
    
    // RQ-2-2024.14: Update NRSTMISCENT_RPRDSC with NRSTMISCENT_NRSTMISCDESC
    // C++: CString strDescription = FormGetField("HSE_TgNrstMiscEnt","NRSTMISCENT_NRSTMISCDESC");
    //      CString strID=FormGetField("HSE_TgNrstMiscEnt","NRSTMISCENT_NRSTMISCNUM");
    //      CString strSql;
    //      strSql.Format("update HSE_VWNRSTMISCENT set NRSTMISCENT_RPRDSC='%s' where NRSTMISCENT_NRSTMISCNUM=%s",strDescription,strID);
    //      long lRet = ExecuteSQL(strSql);
    
    // Try multiple form tags to get description (view name is more reliable)
    let description = '';
    try {
      description = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCDESC') || '';
    } catch (e) {
      // Ignore error, try next option
    }
    if (!description) {
      try {
        description = FormGetField('HSE_TgNrstMiscEnt', 'NRSTMISCENT_NRSTMISCDESC') || '';
      } catch (e) {
        // Ignore error, try next option
      }
    }
    if (!description && screenTag) {
      try {
        description = FormGetField(screenTag, 'NRSTMISCENT_NRSTMISCDESC') || '';
      } catch (e) {
        // Ignore error - description update is optional
      }
    }

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

    // C++: bool bNearMissComplete = CompleteNearMiss(pCustomButtonClicked);
    // CompleteNearMiss function:
    //   CString strUserName = GetUserLogin();
    //   CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");
    //   CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
    //   strSQL.Format("EXECUTE completeNearMissTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
    //   long lRet = ExecuteSQL(strSQL);
    //   RefreshScreen("",REFRESH_SELECTED);
    //   if(lRet != 0)
    //       bNearMissComplete  = true;
    
    // Get key field value for the stored procedure
    const keyFieldValue = observationId;
    
    // Get source screen name
    // C++: CString strSourceScreenName = GetScrCptnByTag(66, pCustomButtonClicked->Form_Tag, "");
    // Note: GetScrCptnByTag gets the screen caption from the screen tag
    // For now, we'll use the screen tag as the source screen name (this matches other implementations)
    const sourceScreenName = screenTag || 'HSE_TgNrstMiscEnt';
    
    // Get user name
    // C++: CString strUserName = GetUserLogin();
    const userName = getUserName() || '';
    
    // Execute the completeNearMissTXN stored procedure
    // C++: EXECUTE completeNearMissTXN '%s','%s','%s'
    const storedProcSql = `EXECUTE completeNearMissTXN '${keyFieldValue.replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}'`;
    
    try {
      const result = await executeSQLAsync(storedProcSql);
      
      // C++: RefreshScreen("",REFRESH_SELECTED);
      refreshData('', 'REFRESH_SELECTED');
      
      // Show success message
      toast.success('Observation entry completed successfully');
      
      // C++: Email functionality is commented out, so we skip it here
      // if(bNearMissComplete) {
      //     EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
      //     CString strMailBody = eMailMsgHandler->FormatMailBody(ID_MAIL_Observation_Complete,"");
      //     CString strMsg = m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strHSEDepMail,strMailBody,"");
      //     AfxMessageBox(strMsg);
      //     RefreshScreen("",REFRESH_SELECTED);
      // }
      
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
 * Handle Close button click
 * (RQ_HSM_22_12_11_28) Handle Close Custom Button
 * Implements the logic from NearMissFollowUpCategory::DisplayCustomButtonClicked for NRSTMISCENT_CLS
 * C++ Reference: NearMissFollowUpCategory::DisplayCustomButtonClicked (lines 43-270)
 * @param {string} buttonName - The button name
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleCloseButton(buttonName, screenTag, eventObj, devInterface) {
  // (RQ_HSM_22_12_11_28) Handle Close Custom Button
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
      console.error('[Web_HSE] Missing required devInterface functions for Close button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGNRSTMISCFLWUP';

    // C++: DoToolBarAction(TOOLBAR_SAVE, pCustomButtonClicked->Form_Tag, "");
    // Save the record first before closing
    doToolbarAction('SAVE', formTag, '');

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Observation number)
    // C++: CString KeyFieldValue = GetKeyField(strFormTag, pCustomButtonClicked->pMultiSelectedRows);
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

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      try {
        keyFieldValue = FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') || '';
      } catch (e) {
        // Ignore error, try next option
      }
      if (!keyFieldValue) {
        try {
          keyFieldValue = FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') || '';
        } catch (e) {
          // Ignore error
        }
      }
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Observation number. Please select a record first.');
      return;
    }

    // C++: CString strEmployeeName = GetEmployeeCodeForLoginUser();
    // C++: if(strEmployeeName != "")
    // Note: In C++, it checks for employee name, but we'll proceed with the close operation
    // The stored procedure may handle employee validation internally

    // Get screen name based on form tag
    // C++:
    //   if (strForm_Tag=="HSE_TGNRSTMISCCNFRMTN") strScreenName="Observation Review";
    //   else if (strForm_Tag=="HSE_TGNRSTMISCFLWUP") strScreenName="Observation Approval";
    let screenName = '';
    const normalizedFormTag = formTag.toUpperCase();
    if (normalizedFormTag === 'HSE_TGNRSTMISCCNFRMTN' || normalizedFormTag.includes('CNFRMTN')) {
      screenName = 'Observation Review';
    } else if (normalizedFormTag === 'HSE_TGNRSTMISCFLWUP' || normalizedFormTag.includes('FLWUP')) {
      screenName = 'Observation Approval';
    } else if (normalizedFormTag === 'HSE_TGNRMISRWARD' || normalizedFormTag.includes('RWARD')) {
      screenName = 'Observation Reward';
    } else {
      screenName = 'Observation Entry'; // Default fallback
    }

    // C++: CString strSourceScreenName = GetScrCptnByTag(66, strFormTag, "");
    // C++: CString strUserName = GetUserLogin();
    // C++: strSQL.Format("EXECUTE closeNearMissTXN '%s','%s','%s'", KeyFieldValue, strUserName, strSourceScreenName);
    const sourceScreenName = screenName; // Use screen name (caption) as source screen name
    const userName = getUserName() || '';
    const spSql = `EXECUTE closeNearMissTXN '${keyFieldValue.toString().replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}'`;

    console.log('[Web_HSE] Executing closeNearMissTXN stored procedure:', {
      observationNumber: keyFieldValue,
      userName: userName,
      sourceScreenName: sourceScreenName,
    });

    try {
      await executeSQLAsync(spSql);
      
      // C++: InsertActionRecordIntoTracingTab("NRSTMISCENT","NRSTMISCNUM",CLOSED);
      // C++: CLOSED maps to "Closed" action description
      // Similar to Cancel button, we need to insert a tracing record manually
      // C++: strSQL.Format("insert into HSE_NRSTMISCENTTRC (NRSTMISCENTTRC_DATTIM,NRSTMISCENTTRC_ACCDESC,NRSTMISCENTTRC_LNK,NRSTMISCENTTRC_ENTUSR,SRCSCRN) values (getdate(),'Closed',%s,'%s','%s')",linkFieldVal,strUserName,strScreenName);
      const linkFieldVal = keyFieldValue;
      const linkFieldValNum = typeof linkFieldVal === 'string' ? linkFieldVal.replace(/['"]/g, '') : linkFieldVal;
      const insertTracingSql = `INSERT INTO HSE_NRSTMISCENTTRC (NRSTMISCENTTRC_DATTIM,NRSTMISCENTTRC_ACCDESC,NRSTMISCENTTRC_LNK,NRSTMISCENTTRC_ENTUSR,SRCSCRN) VALUES (getdate(),'Closed',${linkFieldValNum},'${userName.replace(/'/g, "''")}','${screenName.replace(/'/g, "''")}')`;

      console.log('[Web_HSE] Inserting Close tracing record with SQL:', insertTracingSql);
      
      try {
        await executeSQLAsync(insertTracingSql);
        console.log('[Web_HSE] ✓ Tracing record inserted for closed observation');
        
        // Verify the tracing record was inserted
        const verifyClosedSql = `SELECT TOP 1 NRSTMISCENTTRC_ACCDESC, NRSTMISCENTTRC_DATTIM 
                                FROM HSE_NRSTMISCENTTRC 
                                WHERE NRSTMISCENTTRC_LNK = ${linkFieldValNum} 
                                AND NRSTMISCENTTRC_ACCDESC = 'Closed' 
                                ORDER BY NRSTMISCENTTRC_DATTIM DESC`;
        const verifyResult = await executeSQLAsync(verifyClosedSql);
        const closedExists = verifyResult?.recordsets?.[0]?.length > 0 || verifyResult?.length > 0;
        console.log('[Web_HSE] Verification - "Closed" tracing record exists:', closedExists);
        
      } catch (tracingError) {
        console.warn('[Web_HSE] Error inserting tracing record (continuing anyway):', tracingError);
        // Continue even if tracing record insertion fails
      }
      
      // C++: RefreshScreen("", REFRESH_SELECTED);
      // Refresh immediately
      refreshData('', 'REFRESH_SELECTED');
      
      // Additional refreshes with delays to ensure tracing tab loads in all screens
      // This helps ensure the tracing record is visible in both Approval and Reward screens
      setTimeout(() => {
        refreshData('', 'REFRESH_SELECTED');
        console.log('[Web_HSE] First delayed refresh for tracing tab');
      }, 500);
      
      setTimeout(() => {
        refreshData('', 'REFRESH_SELECTED');
        console.log('[Web_HSE] Second delayed refresh for tracing tab');
      }, 1500);
      
      setTimeout(() => {
        refreshData('', 'REFRESH_SELECTED');
        console.log('[Web_HSE] Third delayed refresh for tracing tab (ensures visibility in Reward screen)');
      }, 2500);
      
      toast.success('Observation closed successfully');
      
      // Note: CAR creation logic (if NRSTMISCENT_RQRCR == "Y") is handled by the stored procedure
      // The C++ code has extensive CAR creation logic, but it's likely moved to the stored procedure
      // as indicated by the comment "Commented_Code Moved to DB to check original code"
      
    } catch (error) {
      console.error('[Web_HSE] Error executing closeNearMissTXN:', error);
      toast.error('Error closing observation. Please try again.');
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleCloseButton:', error);
    toast.error('An error occurred while processing the Close action');
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
export async function handleRejectReasonOkButton(devInterface) {
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

