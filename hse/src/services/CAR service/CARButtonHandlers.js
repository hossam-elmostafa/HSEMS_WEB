import { toast } from "react-toastify";

/**
 * CARButtonHandlers.js
 * 
 * Handles all button click events for CAR module screens
 * 
 * Requirements:
 * - RQ_AG_25_12_25_15_19.01.01: Reject CAR - https://app.clickup.com/t/86c785pwa
 * - RQ_AG_25_12_25_15_19.01.02: CAR Review info - https://app.clickup.com/t/86c785q04
 * - RQ_AG_25_12_25_15_19.01.03: Accept CAR - https://app.clickup.com/t/86c785q2w
 * - RQ_AG_25_12_25_15_19.01.04: View Source TXN - https://app.clickup.com/t/86c785q7c
 */

// Store pending reject CAR execution info (module-level)
let pendingRejectCAR = null;

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


export function setPendingRejectCAR(carInfo) {
  pendingRejectCAR = carInfo;
}

export function clearPendingRejectCAR() {
  pendingRejectCAR = null;
}

export function hasPendingRejectCAR() {
  return pendingRejectCAR !== null;
}

/**
 * Handle Reject CAR button click
 * RQ_AG_25_12_25_15_19.01.01: Reject CAR
 * Implements the logic from CarCategory::DisplayCustomButtonClicked for CRCTVEACCENT_RJC
 * and CarConfirmationCategory::DisplayCustomButtonClicked for CRCTVEACCENT_CNFRMTN_RJC
 * 
 * C++ Reference:
 * - CARCategory.cpp line 70-80: CRCTVEACCENT_RJC button handler
 * - CARConfirmationCategory.cpp line 66-69: CRCTVEACCENT_CNFRMTN_RJC button handler
 * 
 * @param {string} buttonName - The button name (e.g. CRCTVEACCENT_RJC, CRCTVEACCENT_CNFRMTN_RJC)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRejectCARButton(buttonName, screenTag, eventObj, devInterface) {
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
      console.error('[Web_HSE] Missing required devInterface functions for Reject CAR button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (CAR number)
    // C++: CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag, pCustomButtonClicked->pMultiSelectedRows);
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.CRCTVEACCENT_CARMODELNUM ||
        firstRecord?.CARMODELNUM ||
        '';
    }

    const formTag = screenTag || 'HSE_TGCRCTVEACCENT';

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        FormGetField('HSE_CRCTVEACCENT', 'CRCTVEACCENT_CARMODELNUM') ||
        FormGetField(formTag, 'CRCTVEACCENT_CARMODELNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find CAR number. Please select a record first.');
      return;
    }

    // Determine module type based on screen tag
    // C++: rejectRecord() uses module type "CRCTVEACCENT-L1" for confirmation screen
    // For CAR, the module type is typically "CRCTVEACCENT-L1" for confirmation/review screens
    let moduleType = 'CRCTVEACCENT-L1'; // Default for confirmation/review screens
    if (formTag === 'HSE_TGCRCTVEACCCNFRMTN') {
      moduleType = 'CRCTVEACCENT-L1';
    } else if (formTag === 'HSE_TGCRCTVEACCFLWUP') {
      moduleType = 'CRCTVEACCENT-L2';
    } else {
      // For entry screen, use L1 as default
      moduleType = 'CRCTVEACCENT-L1';
    }

    // C++: rejectRecord() opens the reject reason screen
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

    // Store info for executing rejectCARTXN after reject reason screen OK button is clicked
    // C++: After reject reason screen closes, checks getgbUpdateRejectedRecord() flag
    // In JS: We use ButtonClicked event to detect when RJCTRSN_BTN_OK is clicked
    // C++: setgbUpdateRejectedRecord(false) is set initially, then true when OK is clicked
    setPendingRejectCAR({
      keyFieldValue: keyFieldValue.toString(), // Ensure it's a string for the stored procedure
      formTag,
      getUserName,
      executeSQLAsync,
      refreshData,
      toast,
    });

    // Open the reject reason screen - it has its own built-in dialog
    // C++: rejectRecord(strFormTag) -> opens HSE_TGRjctRsn screen
    console.log('[Web_HSE] Opening reject reason screen for CAR:', {
      screenTag: rejectReasonScreenTag,
      carNumber: keyFieldValue,
      moduleType: moduleType,
    });
    
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, defValObj, false);

    // C++: After screen closes, if getgbUpdateRejectedRecord() == true, execute rejectCARTXN
    // In JS: We use ButtonClicked event to detect when RJCTRSN_BTN_OK is clicked
    // The stored procedure will be executed automatically when OK button is clicked
    console.log('[Web_HSE] Reject reason screen opened. Waiting for OK button click (RJCTRSN_BTN_OK)...');
    console.log('[Web_HSE] Pending rejection info stored:', {
      carNumber: keyFieldValue,
      screenTag: formTag,
    });
  } catch (error) {
    console.error('[Web_HSE] Error in handleRejectCARButton:', error);
    toast.error('An error occurred while processing the Reject CAR action');
  }
}

/**
 * Handle Reject Reason screen OK button click for CAR
 * C++: CRejectReason::RejectReasonOk() - sets gbUpdateRejectedRecord = true
 * This is called when RJCTRSN_BTN_OK is clicked in the reject reason screen
 * RQ_AG_25_12_25_15_19.01.01: Reject CAR
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleRejectReasonOkButtonForCAR(devInterface) {
  try {
    if (!pendingRejectCAR) {
      console.warn('[Web_HSE] ⚠ No pending rejection info found. Stored procedure will not execute.');
      return;
    }

    console.log('[Web_HSE] Executing rejectCARTXN stored procedure...');
    
    const { keyFieldValue, formTag, getUserName, executeSQLAsync, refreshData, toast } = pendingRejectCAR;
    const userName = getUserName() || '';
    const sourceScreenName = formTag;
    
    // C++: strSQL.Format("EXECUTE rejectCARTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
    // Note: keyFieldValue should be passed as string (stored procedure expects VARCHAR)
    const spSql = `EXECUTE rejectCARTXN '${keyFieldValue.toString().replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','${sourceScreenName.replace(/'/g, "''")}'`;
    
    console.log('[Web_HSE] Executing SQL:', spSql);
    await executeSQLAsync(spSql);
    console.log('[Web_HSE] ✓ rejectCARTXN executed successfully');
    
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`CAR ${keyFieldValue} rejected successfully`);
    
    // Clear pending execution
    clearPendingRejectCAR();
  } catch (error) {
    console.error('[Web_HSE] ✗ Error executing rejectCARTXN:', error);
    if (pendingRejectCAR && pendingRejectCAR.toast) {
      pendingRejectCAR.toast.error('Error rejecting CAR: ' + (error.message || 'Unknown error'));
    }
    clearPendingRejectCAR();
  }
}

/**
 * Handle View Reject Reasons button click in CAR Tracing tab
 * C++: CHSEMSCommonCategory::viewRejectReason() - opens reject reasons screen locked
 * Button: CrEntry_Trc_VWRjctRsns (in Tracing tab)
 * 
 * @param {string} buttonName - The button name (e.g. CrEntry_Trc_VWRjctRsns, CRENTRY_VWRJCTRSNS)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewRejectReasonButtonForCAR(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      openScr,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || !openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View Reject Reasons button');
      toast.error('System error: Required functions not available');
      return;
    }

    // C++: CString strKeyField=FormGetField("HSE_CRCTVEACCENTTRC","CRCTVEACCENTTRC_TRCNG_REFRJCTRSN");
    // For new CAR module, the table is HSE_CrEntry_Trc
    // Try both old and new table names
    let keyField = FormGetField('HSE_CRCTVEACCENTTRC', 'CRCTVEACCENTTRC_TRCNG_REFRJCTRSN') ||
                   FormGetField('HSE_CrEntry_Trc', 'CRENTRY_TRC_REFRJCTRSN') ||
                   FormGetField('HSE_CRENTRY_TRC', 'CRENTRY_TRC_REFRJCTRSN') ||
                   FormGetField(screenTag, 'CRENTRY_TRC_REFRJCTRSN') ||
                   '';

    if (!keyField || keyField === '') {
      console.warn('[Web_HSE] No reject reason key field found. Cannot open reject reasons screen.');
      toast.warning('No reject reason found for this record');
      return;
    }

    // C++: strSQL.Format("SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '%s')",strKeyField);
    // C++: strDefValues.Format("RSNS_RFREC|%s",strKeyField);
    // C++: ShowScreen(0,"CMS_RSNS","Reject Reasons",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
    const rejectReasonScreenTag = 'CMS_RSNS';
    const criteria = `SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '${keyField.replace(/'/g, "''")}')`;
    const defValObj = {
      RSNS_RFREC: keyField,
    };
    const bLocked = true;

    console.log('[Web_HSE] Opening reject reasons screen for CAR:', {
      screenTag: rejectReasonScreenTag,
      keyField: keyField,
      criteria: criteria,
    });

    // Open the reject reasons screen locked
    openScr(rejectReasonScreenTag, {}, criteria, 'edit', true, defValObj, bLocked);
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewRejectReasonButtonForCAR:', error);
    toast.error('An error occurred while opening reject reasons screen');
  }
}

/**
 * Handle CAR Review Info button click
 * RQ_AG_25_12_25_15_19.01.02: CAR Review info
 * Implements the logic from NewCarEntryCategory::DisplayCustomButtonClicked for CAR_REVIEW_INFO
 * 
 * C++ Reference:
 * - NewCarEntryCategory.cpp line 537-556: CAR_REVIEW_INFO button handler
 * - NewCarEntryCategory.cpp line 804-826: acceptRecord() function
 * - HSEMSCommonCategory.cpp line 1654-1670: Accept() helper function
 * 
 * @param {string} buttonName - The button name (e.g. CAR_REVIEW_INFO, CAR_Review_Info)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleCARReviewInfoButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      openScr,
      callServerApp,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || !openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for CAR Review Info button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGCRRVW';
    const normalizedFormTag = formTag.toUpperCase();

    // C++: Only works on HSE_TGCRRVW screen (but also allow other CAR screens for flexibility)
    // The button can appear on multiple screens, but the logic is primarily for HSE_TGCRRVW
    if (normalizedFormTag !== 'HSE_TGCRRVW') {
      console.log('[Web_HSE] CAR Review Info button on screen:', formTag, '- will try to proceed');
      // Don't return - allow it to work on other screens too
    }

    // C++: Check policy settings
    // RQ-2-2024.8, Bugs-2-2024.4
    // Get CAR source and policy basis
    // Try multiple form tags to get CAR source
    let strCARSource = safeFormGetField(FormGetField, formTag, 'CRENTRY_CRSRC') || 
                      safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_CRSRC') || 
                      safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_CRSRC') || '';
    
    // Get policy fields - need to call server to get policy tab field
    // C++: GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS")
    // C++: GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENBCRRVWINF")
    let strEnable_CAR_Review_Info = 'N';
    
    try {
      // Try to read from database directly
      // C++: GetPolicyTabField reads from HSE_HSEPLC_ADT table
      // C++: GetPolicyTabField does: "Select fieldName From tableName" (gets first row)
      const policySql = `SELECT TOP 1 HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRRVWINF FROM HSE_HSEPLC_ADT ORDER BY PRMKY DESC`;
      try {
        const policyResult = await executeSQLAsync(policySql);
        // Handle different result formats
        let policy = null;
        if (Array.isArray(policyResult)) {
          policy = policyResult[0];
        } else if (policyResult?.recordset && policyResult.recordset.length > 0) {
          policy = policyResult.recordset[0];
        } else if (policyResult && typeof policyResult === 'object') {
          policy = policyResult;
        }
        
        if (policy) {
          const strCARBasis = policy?.HSEPLC_ADT_CRBSS || '';
          console.log('[Web_HSE] Policy check - CAR Source:', strCARSource, 'CAR Basis:', strCARBasis);
          if (strCARSource && strCARSource === strCARBasis) {
            strEnable_CAR_Review_Info = (policy?.HSEPLC_ADT_ENBCRRVWINF || 'N').toString().toUpperCase();
            console.log('[Web_HSE] Policy check - Enable CAR Review Info:', strEnable_CAR_Review_Info);
          } else {
            // If CAR source doesn't match basis, default to enabled (policy might not be set up)
            console.log('[Web_HSE] CAR source does not match policy basis, defaulting to enabled');
            strEnable_CAR_Review_Info = 'Y';
          }
        } else {
          console.warn('[Web_HSE] No policy record found, defaulting to enabled');
          strEnable_CAR_Review_Info = 'Y';
        }
      } catch (policyError) {
        console.warn('[Web_HSE] Could not read policy settings, defaulting to enabled:', policyError);
        // Default to enabled if we can't read policy
        strEnable_CAR_Review_Info = 'Y';
      }
    } catch (error) {
      console.warn('[Web_HSE] Error checking policy settings:', error);
      // Default to enabled if policy check fails
      strEnable_CAR_Review_Info = 'Y';
    }

    if (strEnable_CAR_Review_Info !== 'Y') {
      toast.warning('According to current System Policy. This function is not allowed');
      return;
    }

    // Extract event data to get key field value
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};
    
    // Get key field value (CAR primary key)
    // C++: CString strPrimKey=FormGetField(strForm_Tag,"PRMKY");
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue = firstRecord?.PRMKY || firstRecord?.CRENTRY_PRMKY || '';
    }

    // Fallback: read from form - try multiple form tags safely
    if (!keyFieldValue) {
      keyFieldValue = safeFormGetField(FormGetField, formTag, 'PRMKY') || 
                     safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'PRMKY') || 
                     safeFormGetField(FormGetField, 'HSE_CRENTRY', 'PRMKY') ||
                     safeFormGetField(FormGetField, formTag, 'CRENTRY_PRMKY') || '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find CAR record. Please select a record first.');
      return;
    }

    // C++: Accept("CRCTVEACCENT-RV",strPrimKey);
    // Opens HSE_TgCrRvwInf screen with module type "CRCTVEACCENT-RV"
    // Note: Screen tag is HSE_TgCrRvwInf (with Tg prefix), table name is HSE_CrRvwInf
    const moduleType = 'CRCTVEACCENT-RV';
    const carReviewInfoScreenTag = 'HSE_TgCrRvwInf';
    
    // C++: strCriteria.Format("SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= %s) AND (CrRvwInf_MdlTyp= '%s')",strKeyField,strSourceModule);
    const criteria = `SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn = ${keyFieldValue}) AND (CrRvwInf_MdlTyp = '${moduleType.replace(/'/g, "''")}')`;
    
    // C++: strDefValues.Format("CrRvwInf_LnkWthMn|%s|CrRvwInf_MdlTyp|%s",strKeyField,strSourceModule);
    const defValObj = {
      CrRvwInf_LnkWthMn: keyFieldValue,
      CrRvwInf_MdlTyp: moduleType,
    };

    console.log('[Web_HSE] Opening CAR Review Info screen:', {
      screenTag: carReviewInfoScreenTag,
      carKey: keyFieldValue,
      moduleType: moduleType,
    });

    // C++: ShowScreen(0,"HSE_CrRvwInf","CAR Review Info",NORMAL_MODE,true,strCriteria,"",strDefValues,"",bLocked);
    // bLocked = false for CAR_REVIEW_INFO button (allows editing)
    openScr(carReviewInfoScreenTag, {}, criteria, 'edit', true, defValObj, false);
  } catch (error) {
    console.error('[Web_HSE] Error in handleCARReviewInfoButton:', error);
    toast.error('An error occurred while opening CAR Review Info screen');
  }
}

/**
 * Handle Accept CAR button click
 * RQ_AG_25_12_25_15_19.01.03: Accept CAR
 * Implements the logic from NewCarEntryCategory::AcceptCARProcess
 * 
 * C++ Reference:
 * - NewCarEntryCategory.cpp line 1086-1147: AcceptCARProcess() function
 * - NewCarEntryCategory.cpp line 1149-1159: InsertIntoTracingTabProcess() function
 * 
 * @param {string} buttonName - The button name (e.g. ACCEPT_CAR)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleAcceptCARButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      FormSetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      doToolbarAction,
      callServerApp,
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
      console.error('[Web_HSE] Missing required devInterface functions for Accept CAR button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_TGCRRVW';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (CAR primary key)
    // C++: CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag, pCustomButtonClicked->pMultiSelectedRows);
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue = firstRecord?.PRMKY || firstRecord?.CRENTRY_PRMKY || '';
    }

    // Fallback: read from form - try multiple form tags safely
    if (!keyFieldValue) {
      keyFieldValue = safeFormGetField(FormGetField, formTag, 'PRMKY') || 
                     safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'PRMKY') || 
                     safeFormGetField(FormGetField, 'HSE_CRENTRY', 'PRMKY') ||
                     safeFormGetField(FormGetField, formTag, 'CRENTRY_PRMKY') || '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find CAR record. Please select a record first.');
      return;
    }

    // C++: Check if CAR Review Info exists (if policy requires it)
    // RQ-2-2024.8, Bugs-2-2024.4
    let bUpdateAcceptedRecord = true;
    let strEnable_CAR_Review_Info = 'N';
    
    // Try multiple form tags to get CAR source
    const strCARSource = safeFormGetField(FormGetField, formTag, 'CRENTRY_CRSRC') || 
                        safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_CRSRC') || 
                        safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_CRSRC') || '';

    // Check policy settings
    try {
      // Read from database
      const policySql = `SELECT TOP 1 HSEPLC_ADT_CRBSS, HSEPLC_ADT_ENBCRRVWINF FROM HSE_HSEPLC_ADT ORDER BY PRMKY DESC`;
      try {
        const policyResult = await executeSQLAsync(policySql);
        // Handle different result formats
        let policy = null;
        if (Array.isArray(policyResult)) {
          policy = policyResult[0];
        } else if (policyResult?.recordset && policyResult.recordset.length > 0) {
          policy = policyResult.recordset[0];
        } else if (policyResult && typeof policyResult === 'object') {
          policy = policyResult;
        }
        
        if (policy) {
          const strCARBasis = policy?.HSEPLC_ADT_CRBSS || '';
          if (strCARSource && strCARSource === strCARBasis) {
            strEnable_CAR_Review_Info = (policy?.HSEPLC_ADT_ENBCRRVWINF || 'N').toString().toUpperCase();
          }
        }
      } catch (policyError) {
        console.warn('[Web_HSE] Could not read policy settings:', policyError);
      }
    } catch (error) {
      console.warn('[Web_HSE] Error checking policy settings:', error);
    }

    // C++: if (strEnable_CAR_Review_Info=="Y")
    if (strEnable_CAR_Review_Info === 'Y') {
      // C++: Check if CAR Review Info record exists
      // C++: strSql.Format("update HSE_CRRVWINF set CrRvwInf_LnkWthMn=CrRvwInf_LnkWthMn where CrRvwInf_LnkWthMn=%s",KeyFieldValue);
      // C++: long lRetFieldsCount = ExecuteSQL(strSql);
      // C++: bUpdateAcceptedRecord=(lRetFieldsCount ==1);
      const checkReviewInfoSql = `UPDATE HSE_CRRVWINF SET CrRvwInf_LnkWthMn=CrRvwInf_LnkWthMn WHERE CrRvwInf_LnkWthMn=${keyFieldValue}`;
      try {
        const result = await executeSQLAsync(checkReviewInfoSql);
        // Check rows affected (SQL Server returns rowsAffected)
        const rowsAffected = result?.rowsAffected?.[0] || result?.rowsAffected || 0;
        bUpdateAcceptedRecord = rowsAffected === 1;
      } catch (error) {
        console.warn('[Web_HSE] Error checking CAR Review Info:', error);
        bUpdateAcceptedRecord = false;
      }
    }

    if (bUpdateAcceptedRecord) {
      // C++: InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
      // C++: strSQL.Format("EXECUTE ChangeEntityStatus '%s','%s','%s','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','02','%s'",KeyFieldValue,strSourceScreenName,strUserName,strStatus);
      const strSourceScreenName = 'CAR Review';
      const strStatus = 'CAR Accepted';
      const userName = getUserName() || '';
      
      const insertTracingSql = `EXECUTE ChangeEntityStatus '${keyFieldValue.toString().replace(/'/g, "''")}','${strSourceScreenName.replace(/'/g, "''")}','${userName.replace(/'/g, "''")}','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','02','${strStatus.replace(/'/g, "''")}'`;
      
      console.log('[Web_HSE] Inserting tracing record for CAR Accepted:', insertTracingSql);
      await executeSQLAsync(insertTracingSql);

      // C++: FormSetField(strFormTag,"CRENTRY_CRSTT","10");
      FormSetField(formTag, 'CRENTRY_CRSTT', '10');

      // C++: DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
      doToolbarAction('SAVE', formTag, '');

      // C++: RefreshScreen("",REFRESH_SELECTED);
      refreshData('', 'REFRESH_SELECTED');
      
      toast.success('CAR accepted successfully');
    } else {
      // C++: AfxMessageBox("Please insert a CAR Review Information");
      toast.warning('Please insert a CAR Review Information');
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleAcceptCARButton:', error);
    toast.error('An error occurred while processing the Accept CAR action');
  }
}

/**
 * Handle View Source TXN button click
 * RQ_AG_25_12_25_15_19.01.04: View Source TXN
 * Implements the logic from NewCarEntryCategory::DisplayCustomButtonClicked for VIEW_SOURCE_TXN
 * and CarCategory::OpenTXNInquiryScreen
 * 
 * C++ Reference:
 * - NewCarEntryCategory.cpp line 267-302: VIEW_SOURCE_TXN button handler
 * - CARCategory.cpp line 166-250: OpenTXNInquiryScreen() function
 * 
 * @param {string} buttonName - The button name (e.g. VIEW_SOURCE_TXN)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewSourceTXNButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    console.log('[Web_HSE] ===== View Source TXN Button Handler START =====');
    console.log('[Web_HSE] eventObj received:', eventObj ? 'YES' : 'NO', eventObj);
    
    const {
      FormGetField,
      openScr,
      executeSQLPromise,
      getValFromRecordSet,
      getFldValFrmFullRec,
    } = devInterface;

    // Validate required functions
    if (!FormGetField || !openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View Source TXN button');
      toast.error('System error: Required functions not available');
      return;
    }

    const formTag = screenTag || 'HSE_TGCRENTRY';
    
    // Get primary key from eventObj (like other handlers do)
    // C++: CString strPrmKy=FormGetField(strFormTag,"PrmKy");
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};
    let keyFieldValue = '';
    
    console.log('[Web_HSE] eventObj structure:', {
      hasFullRecordArr: !!fullRecordArr,
      hasFullRecordArrKeys: !!fullRecordArrKeys,
      fullRecordArrKeysLength: fullRecordArrKeys?.length,
      fullRecordArrType: Array.isArray(fullRecordArr) ? 'array' : typeof fullRecordArr,
    });
    
    // Try to get primary key from eventObj first (most reliable)
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
      console.log('[Web_HSE] Got primary key from eventObj.fullRecordArrKeys:', keyFieldValue);
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      // Try to get from fullRecordArr
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue = firstRecord?.PRMKY || firstRecord?.PrmKy || firstRecord?.Prmky || firstRecord?.prmky || '';
      if (keyFieldValue) {
        console.log('[Web_HSE] Got primary key from eventObj.fullRecordArr:', keyFieldValue);
      } else {
        console.log('[Web_HSE] fullRecordArr exists but PRMKY not found. First record keys:', firstRecord ? Object.keys(firstRecord).slice(0, 10) : 'N/A');
      }
    }
    
    // Fallback: try FormGetField with multiple form tags and field names
    if (!keyFieldValue) {
      const prmkyVariations = ['PRMKY', 'PrmKy', 'Prmky', 'prmky'];
      const formTagVariations = [formTag, 'HSE_CRENTRY', 'HSE_TGCRENTRY', 'HSE_TGCRRVW', 'HSE_CRCTVEACCENT'];
      
      for (const fTag of formTagVariations) {
        for (const fName of prmkyVariations) {
          const value = safeFormGetField(FormGetField, fTag, fName);
          if (value) {
            keyFieldValue = value.toString();
            console.log('[Web_HSE] Got primary key from FormGetField:', { formTag: fTag, fieldName: fName, value: keyFieldValue });
            break;
          }
        }
        if (keyFieldValue) break;
      }
    }
    
    console.log('[Web_HSE] Final primary key value for View Source TXN:', keyFieldValue);

    // C++: Check if auto generated is Yes
    // C++: CString strAutoGenerated=FormGetField(strFormTag,"CRENTRY_ATGNR");
    // The field CRENTRY_ATGNR exists in the screen structure, but FormGetField might not find it
    // if the field is not visible or accessible in the current context
    // Try multiple field names and form tags to find the auto-generated field
    let strAutoGenerated = '';
    const fieldNames = ['CRENTRY_ATGNR', 'CRENTRY_AUTOGNRTD', 'ATGNR', 'AUTOGNRTD'];
    const formTags = [formTag, 'HSE_CRENTRY', 'HSE_TGCRENTRY', 'HSE_TGCRRVW', 'HSE_CRCTVEACCENT'];
    
    // Try all combinations and log what we're trying
    console.log('[Web_HSE] Attempting to find auto-generated field. FormTag:', formTag);
    for (const fTag of formTags) {
      for (const fName of fieldNames) {
        const value = safeFormGetField(FormGetField, fTag, fName);
        if (value && value.trim() !== '') {
          strAutoGenerated = value.toUpperCase().trim();
          console.log('[Web_HSE] Found auto-generated field via FormGetField:', { formTag: fTag, fieldName: fName, value: strAutoGenerated });
          break;
        }
      }
      if (strAutoGenerated) break;
    }

    // If FormGetField didn't find it, try querying the database directly using the primary key
    if (!strAutoGenerated && executeSQLPromise && getValFromRecordSet) {
      try {
        // Get the primary key value
        const keyFieldValue = safeFormGetField(FormGetField, formTag, 'PRMKY') || 
                             safeFormGetField(FormGetField, 'HSE_CRENTRY', 'PRMKY') ||
                             safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'PRMKY') ||
                             safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'PRMKY') || '';
        
        if (keyFieldValue) {
          const sql = `SELECT CRENTRY_ATGNR FROM HSE_CRENTRY WHERE PrmKy = ${keyFieldValue}`;
          console.log('[Web_HSE] Querying database for auto-generated field. SQL:', sql);
          const result = await executeSQLPromise(sql);
          if (result && result.recordset && result.recordset.length > 0) {
            const dbValue = getValFromRecordSet(result, 0, 'CRENTRY_ATGNR');
            if (dbValue) {
              strAutoGenerated = dbValue.toString().toUpperCase().trim();
              console.log('[Web_HSE] Found auto-generated field via database query:', strAutoGenerated);
            }
          }
        }
      } catch (dbError) {
        console.warn('[Web_HSE] Could not query database for auto-generated field:', dbError);
      }
    }

    // If we found the field and it's explicitly 'N' or 'NO', block the operation
    if (strAutoGenerated && (strAutoGenerated === 'N' || strAutoGenerated === 'NO')) {
      toast.warning("Auto generated is set to 'No'. Only auto-generated CARs have related transactions.");
      return;
    }
    
    // If field is not found or is empty, log a warning but allow to continue
    // This handles cases where:
    // 1. The field might not be accessible via FormGetField or database query
    // 2. The CAR was created manually (in which case there might not be a source transaction)
    // 3. The field exists but is not loaded in the current form context
    if (!strAutoGenerated) {
      console.warn('[Web_HSE] Auto generated field not found via FormGetField or database. This might be a manual CAR or the field is not accessible. Proceeding with View Source TXN...');
      // Don't return - allow the operation to continue
      // The user can still try to view the source transaction if it exists
      // If the transaction doesn't exist, the screen will show no records
    } else if (strAutoGenerated === 'Y' || strAutoGenerated === 'YES') {
      console.log('[Web_HSE] Auto generated is Yes - proceeding with View Source TXN');
    }

    // C++: Get CAR source and TXN number
    // C++: CString strCARBasis = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELBAS");
    // C++: CString strTXNNum   = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_ACCDNTNUM");
    // For new CAR module, fields are CRENTRY_CRSRC and CRENTRY_TXNNUM or similar
    // Try multiple form tags and field names safely
    let strCARBasis = safeFormGetField(FormGetField, formTag, 'CRENTRY_CRSRC') || 
                       safeFormGetField(FormGetField, formTag, 'CRENTRY_CARMODELBAS') ||
                       safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_CRSRC') ||
                       safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_CARMODELBAS') ||
                       safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'CRENTRY_CRSRC') ||
                       safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'CRENTRY_CARMODELBAS') ||
                       safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_CRSRC') ||
                       safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_CARMODELBAS') || '';
    // Note: The field name is CRENTRY_TXNN (not CRENTRY_TXNNUM) based on C++ code
    let strTXNNum = safeFormGetField(FormGetField, formTag, 'CRENTRY_TXNN') || 
                     safeFormGetField(FormGetField, formTag, 'CRENTRY_TXNNUM') ||
                     safeFormGetField(FormGetField, formTag, 'CRENTRY_ACCDNTNUM') ||
                     safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_TXNN') ||
                     safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_TXNNUM') ||
                     safeFormGetField(FormGetField, 'HSE_CRENTRY', 'CRENTRY_ACCDNTNUM') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'CRENTRY_TXNN') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'CRENTRY_TXNNUM') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRENTRY', 'CRENTRY_ACCDNTNUM') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_TXNN') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_TXNNUM') ||
                     safeFormGetField(FormGetField, 'HSE_TGCRRVW', 'CRENTRY_ACCDNTNUM') || '';

    // If FormGetField didn't find the values, try querying the database directly
    console.log('[Web_HSE] Checking if database query needed. CAR Source found:', !!strCARBasis, 'TXN Num found:', !!strTXNNum, 'Has executeSQLPromise:', !!executeSQLPromise, 'Has getValFromRecordSet:', !!getValFromRecordSet, 'Has keyFieldValue:', !!keyFieldValue);
    
    if ((!strCARBasis || !strTXNNum) && executeSQLPromise && getValFromRecordSet && keyFieldValue) {
      try {
        console.log('[Web_HSE] Attempting database query. Primary Key:', keyFieldValue, 'FormTag:', formTag);
        
        if (keyFieldValue) {
          const sql = `SELECT CRENTRY_CRSRC, CRENTRY_TXNN, CRENTRY_TXNYR FROM HSE_CRENTRY WHERE PrmKy = ${keyFieldValue}`;
          console.log('[Web_HSE] Querying database for CAR source and TXN number. SQL:', sql);
          const result = await executeSQLPromise(sql);
          console.log('[Web_HSE] Database query result:', result);
          if (result && result.recordset && result.recordset.length > 0) {
            const record = result.recordset[0];
            console.log('[Web_HSE] Database recordset:', record);
            if (!strCARBasis) {
              // Try getValFromRecordSet first, then fall back to direct access
              const dbCARSource = getValFromRecordSet ? getValFromRecordSet(result, 0, 'CRENTRY_CRSRC') : null;
              const directCARSource = record.CRENTRY_CRSRC || record.CrEntry_CrSrc || record.crEntry_crSrc;
              const finalCARSource = dbCARSource || directCARSource;
              if (finalCARSource) {
                strCARBasis = finalCARSource.toString();
                console.log('[Web_HSE] Found CAR Source via database query:', strCARBasis);
              } else {
                console.log('[Web_HSE] CRENTRY_CRSRC is null or empty in database. Record keys:', Object.keys(record));
              }
            }
            if (!strTXNNum) {
              // Try getValFromRecordSet first, then fall back to direct access
              const dbTXNNum = getValFromRecordSet ? getValFromRecordSet(result, 0, 'CRENTRY_TXNN') : null;
              const directTXNNum = record.CRENTRY_TXNN || record.CrEntry_Txnn || record.crEntry_txnn;
              const finalTXNNum = dbTXNNum || directTXNNum;
              if (finalTXNNum !== null && finalTXNNum !== undefined) {
                strTXNNum = finalTXNNum.toString();
                console.log('[Web_HSE] Found TXN Number via database query:', strTXNNum);
              } else {
                console.log('[Web_HSE] CRENTRY_TXNN is null or empty in database. Record keys:', Object.keys(record));
              }
            }
          } else {
            console.warn('[Web_HSE] Database query returned no records for PrmKy:', keyFieldValue);
          }
        } else {
          console.warn('[Web_HSE] Primary key not found, cannot query database');
        }
      } catch (dbError) {
        console.error('[Web_HSE] Error querying database for CAR source and TXN number:', dbError);
      }
    } else {
      console.log('[Web_HSE] Skipping database query - executeSQLPromise:', !!executeSQLPromise, 'getValFromRecordSet:', !!getValFromRecordSet);
    }

    console.log('[Web_HSE] View Source TXN - CAR Source:', strCARBasis, 'TXN Number:', strTXNNum);

    if (!strTXNNum) {
      toast.warning('Unable to find transaction number. This CAR may not have a source transaction, or the transaction number field is not populated. Please check the CAR record or enable automatic CAR generation in the system.');
      console.warn('[Web_HSE] Transaction number not found. CAR Source:', strCARBasis);
      return;
    }
    
    if (!strCARBasis) {
      console.warn('[Web_HSE] CAR Source not found, but proceeding with TXN Number:', strTXNNum);
    }

    // Map CAR source to screen tag and key field
    // C++: switch(nCARBasis) based on numeric value
    // We'll use string matching for CAR source
    const strCARSrcUpper = strCARBasis.toUpperCase();
    let strTXNTag = '';
    let strTXNKeyField = '';
    let strTableName = '';
    let strWhereCond = '';

    // Map CAR source to inquiry screen
    // Based on CARCategory.cpp OpenTXNInquiryScreen()
    // Note: Database uses 'OBSRVATION' (without 'E'), but we check for both variations
    if (strCARSrcUpper.includes('OBSERVATION') || strCARSrcUpper.includes('OBSRVATION') || strCARSrcUpper === '3') {
      // Observation
      strTXNTag = 'HSE_TgNrstMiscInq';
      strTXNKeyField = 'NRSTMISCENT_NRSTMISCNUM';
      strTableName = 'HSE_vwNrstMiscEnt';
    } else if (strCARSrcUpper.includes('AUDIT') || strCARSrcUpper === '2' || strCARSrcUpper.includes('INTERNAL AUDIT')) {
      // Audit
      strTXNTag = 'HSE_TgAdtPlnInq';
      strTXNKeyField = 'MAINLINK';
      strTableName = 'HSE_AdtPlnEnt';
    } else if (strCARSrcUpper.includes('PERFORMANCE') || strCARSrcUpper === '1') {
      // Performance Measurement
      strTXNTag = 'HSE_TgperfMsrmntInq';
      strTXNKeyField = 'PERFMSRMNTENT_VSTNO';
      strTableName = 'HSE_perfMsrmntEnt';
    } else if (strCARSrcUpper.includes('HAZARD') || strCARSrcUpper === '4') {
      // Potential Hazard
      strTXNTag = 'HSE_TgptnlhzrdInq';
      strTXNKeyField = 'PTNLHZRDENT_PTNLHZRDNO';
      strTableName = 'HSE_ptnlhzrdEnt';
    } else if (strCARSrcUpper.includes('VEHICLE') || strCARSrcUpper === '5') {
      // Vehicle Accident
      strTXNTag = 'HSE_TgVclAcdntInqtn';
      strTXNKeyField = 'VCLACDNTENT_VCLACDNTNO';
      strTableName = 'HSE_VclAcdntEnt';
    } else if (strCARSrcUpper.includes('INCIDENT') || strCARSrcUpper === '6') {
      // Incident
      strTXNTag = 'HSE_TgAcdntInq';
      strTXNKeyField = 'ACDNTENT_ACDNTNUM';
      strTableName = 'HSE_ACDNTENT';
    } else if (strCARSrcUpper.includes('LOSS') || strCARSrcUpper === '7') {
      // Loss Accident
      strTXNTag = 'HSE_TgLossAcdntInq';
      strTXNKeyField = 'LOSSACDNTENT_ACTHZRDNO';
      strTableName = 'HSE_LossAcdntEnt';
    } else if (strCARSrcUpper.includes('RISK') || strCARSrcUpper === '8') {
      // Risk Assessment
      strTXNTag = 'HSE_TgrskassmntInq';
      strTXNKeyField = 'RSKASSMNTENT_ASSMNTNO';
      strTableName = 'HSE_rskassmntEnt';
    } else if (strCARSrcUpper.includes('SITE') || strCARSrcUpper === '11') {
      // Site Survey
      strTXNTag = 'HSE_TGSITSRVYINQ';
      strTXNKeyField = 'SITSRVYENT_SITSRVYNO';
      strTableName = 'HSE_SITSRVYENT';
    } else if (strCARSrcUpper.includes('DRILL') || strCARSrcUpper === '12') {
      // Drill
      strTXNTag = 'HSE_DRLLPLNINQ';
      strTXNKeyField = 'DRLLPLN_PRMRYKY';
      strTableName = 'HSE_DRLLPLN_VW';
    } else if (strCARSrcUpper.includes('ENV') || strCARSrcUpper === '13') {
      // ENV-Measure
      strTXNTag = 'HSE_TGENVMNTREXCTN';
      strTXNKeyField = 'ENVMNTRPLAN_YR';
      strTableName = 'HSE_ENVMNTRPLAN';
    }

    if (!strTableName || !strTXNKeyField) {
      toast.warning('Unable to determine source transaction type for this CAR.');
      return;
    }

    // C++: strWhereCond="SELECT * FROM "+strTableName+" WHERE ("+strTXNKeyField+" = "+strTXNNum+")";
    strWhereCond = `SELECT * FROM ${strTableName} WHERE (${strTXNKeyField} = ${strTXNNum})`;

    console.log('[Web_HSE] Opening source transaction screen:', {
      screenTag: strTXNTag,
      tableName: strTableName,
      keyField: strTXNKeyField,
      txnNum: strTXNNum,
    });

    // C++: ShowScreen(0,strTXNTag,"TXN Inquiry",NORMAL_MODE,false,strWhereCond,"","","",true);
    // bLocked = true (read-only)
    openScr(strTXNTag, {}, strWhereCond, 'edit', true, {}, true);
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewSourceTXNButton:', error);
    toast.error('An error occurred while opening source transaction screen');
  }
}

