import { toast } from "react-toastify";

/**
 * AspectsRegisterButtonHandlers.js
 * 
 * Handles all button click events for Aspects Register module screens
 * 
 * C++ Reference Files:
 * - EnvrnmntAspctCategory.cpp
 * 
 * Requirements:
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
 */

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

/**
 * Handle Entry Completed button click for Aspects Entry
 * 
 * Requirement:
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
 * 
 * This function implements the "Entry Completed" functionality for Aspects Entry screen.
 * When the user clicks the "Entry Completed" button, it:
 * 1. Gets the primary key from the selected record
 * 2. Executes the EnvAspctEntryComplete stored procedure to:
 *    - Complete the aspects entry
 *    - Update the status accordingly
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: EnvrnmntAspctCategory.cpp line 152-164: ENTRY_COMPLETED button handler
 * C++ Reference: EnvrnmntAspctCategory.cpp line 204-210: completeEntry() helper function
 * 
 * @param {string} buttonName - The button name (e.g. ENTRY_COMPLETED)
 * @param {string} screenTag - The screen tag (e.g. HSE_AspctsEntryAtEntry)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 * 
 * @example
 * // Called when user clicks "Entry Completed" button on Aspects Entry screen
 * await handleEntryCompletedButton('ENTRY_COMPLETED', 'HSE_AspctsEntryAtEntry', eventObj, devInterface);
 */
export async function handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !refreshData
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Entry Completed button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_AspctsEntryAtEntry';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // C++: CString strPrmryKy = FormGetField("HSE_ASPCTS_VIEW","PrmryKy");
    // Get primary key from the form
    let primaryKey = '';
    
    // Try to get from event object first
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      primaryKey = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        primaryKey = String(
          firstRecord.PrmryKy ||
          firstRecord.PRMRYKY ||
          firstRecord.ASPCTS_PRMKY ||
          firstRecord.ASPCTS_PrmryKy ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!primaryKey) {
      primaryKey =
        safeFormGetField(FormGetField, 'HSE_ASPCTS_VIEW', 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_Aspcts_VIEW', 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_ASPCTSENTRYATENTRY', 'PrmryKy') ||
        safeFormGetField(FormGetField, formTag, 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_ASPCTS_VIEW', 'PRMRYKY') ||
        safeFormGetField(FormGetField, formTag, 'PRMRYKY') ||
        '';
      primaryKey = String(primaryKey).trim();
    }

    if (!primaryKey || primaryKey === 'undefined' || primaryKey === 'null') {
      toast.warning('Unable to find primary key. Please select a record first.');
      return;
    }

    // C++: void EnvrnmntAspctCategory::completeEntry(CString strPrmryKey)
    // C++: strSQL.Format("EXECUTE EnvAspctEntryComplete '%s'",strPrmryKey);
    const completeSql = `EXECUTE EnvAspctEntryComplete '${primaryKey.replace(/'/g, "''")}'`;
    
    // Log parameter values for debugging
    console.log('[Web_HSE] Aspects Entry Completed parameters:', {
      primaryKey,
    });

    console.log('[Web_HSE] Completing Aspects entry:', completeSql);
    try {
      await executeSQLAsync(completeSql);
      console.log('[Web_HSE] ✓ Aspects entry completed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      
      // Check if error is about invalid column name (database schema issue)
      if (errorMessage.includes('invalid column name') || errorMessage.includes('Invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i) || 
                           errorMessage.match(/Invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'unknown column';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure 'EnvAspctEntryComplete' definition.`);
        console.error('[Web_HSE] Stored procedure called: EnvAspctEntryComplete');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        console.error('[Web_HSE] Common issues:');
        console.error('[Web_HSE] - Column may have been renamed or removed');
        console.error('[Web_HSE] - Column name may have a typo in the stored procedure');
        console.error('[Web_HSE] - Database schema may need to be updated');
        return;
      }
      
      // Generic database error
      console.error('[Web_HSE] Database error completing Aspects entry:', errorMessage);
      toast.error(`Error completing Aspects entry: ${errorMessage}`);
      return;
    }

    // Refresh screen
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Aspects entry ${primaryKey} completed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleEntryCompletedButton:', error);
    toast.error('An error occurred while completing the Aspects entry');
  }
}
