import { toast } from "react-toastify";

/**
 * ChemicalRegisterButtonHandlers.js
 * 
 * Handles all button click events for Chemical Register module screens
 * 
 * Requirements:
 * - RQ_HSM_27_01_26_22_57.03: View All Active Items
 * - RQ_HSM_27_01_26_22_57.04: View All In-Active Items
 * - RQ_HSM_27_01_26_22_57.05: View Department Items
 * - RQ_HSM_27_01_26_22_57.12: Update (QTY Card)
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 74-75: VIEW_ALL_ACTIVE_ITEMS button handler
 * - ChemicalRegisterCategory.cpp line 76-77: VIEW_ALL_IN-ACTIVE_ITEMS button handler
 * - ChemicalRegisterCategory.cpp line 78-80: VIEW_DEPARTMENT_ITEMS button handler
 * - ChemicalRegisterCategory.cpp line 82-84: UPDT on HSE_CHMCLRGSTR_QTYCRD calls UpdateQuantities()
 */

/**
 * Safe wrapper for FormGetField that handles errors gracefully
 * @param {Function} FormGetField - The FormGetField function
 * @param {string} formTag - The form tag to read from
 * @param {string} fieldName - The field name to read
 * @param {string} scrType - The screen type ("scr" for screen, "tab" for tab)
 * @returns {string} - The field value or empty string if error
 */
function safeFormGetField(FormGetField, formTag, fieldName, scrType = 'scr') {
  try {
    if (!FormGetField || !formTag || !fieldName) {
      return '';
    }
    // FormGetField signature: FormGetField(tagNam, FldNam, scrType)
    const value = FormGetField(formTag, fieldName, scrType);
    return value || '';
  } catch (error) {
    // Silently return empty string if form tag or field doesn't exist
    return '';
  }
}

/**
 * Extract field value from fullRecord with case-insensitive matching
 * @param {Object|Array} fullRecord - The full record object or array
 * @param {string} fieldName - The field name to search for
 * @returns {string} The field value or empty string
 */
function getFieldFromFullRecord(fullRecord, fieldName) {
  if (!fullRecord) {
    return '';
  }
  
  // Handle array case
  const record = Array.isArray(fullRecord) ? fullRecord[0] : fullRecord;
  if (!record || typeof record !== 'object') {
    return '';
  }
  
  // Try exact match first
  if (record[fieldName] !== undefined && record[fieldName] !== null && record[fieldName] !== '') {
    return String(record[fieldName]);
  }
  
  // Try case-insensitive match
  const fieldNameUpper = fieldName.toUpperCase();
  const fieldNameLower = fieldName.toLowerCase();
  
  for (const key in record) {
    if (record.hasOwnProperty(key)) {
      const keyUpper = key.toUpperCase();
      const keyLower = key.toLowerCase();
      
      // Match exact uppercase or lowercase (include 0 as valid for numeric fields)
      if (keyUpper === fieldNameUpper || keyLower === fieldNameLower) {
        const value = record[key];
        if (value !== undefined && value !== null) {
          if (typeof value === 'object' && !Array.isArray(value)) return ''; // caller may use getValueFromRecordByKeyPattern for objects
          return String(value);
        }
      }
    }
  }
  
  return '';
}

/**
 * Get a value from a record by finding a key that contains any of the given substrings (case-insensitive).
 * Used when exact field names are unknown (e.g. server returns different casing).
 * @param {Object|Array} fullRecord - The full record object or array
 * @param {string[]} keySubstrings - Array of substrings to match in key (e.g. ['SRLN', 'SERIAL'])
 * @returns {string|number|undefined} The first non-null value found, or undefined
 */
function getValueFromRecordByKeyPattern(fullRecord, keySubstrings) {
  if (!fullRecord || !Array.isArray(keySubstrings) || keySubstrings.length === 0) return undefined;
  const record = Array.isArray(fullRecord) ? fullRecord[0] : fullRecord;
  if (!record || typeof record !== 'object') return undefined;
  const subs = keySubstrings.map(s => s.toUpperCase());
  for (const key in record) {
    if (!record.hasOwnProperty(key)) continue;
    const keyUpper = key.toUpperCase();
    if (!subs.some(s => keyUpper.includes(s))) continue;
    const v = record[key];
    if (v === undefined || v === null) continue;
    if (typeof v === 'object' && v !== null && !Array.isArray(v)) {
      if ('key' in v && (v.key !== undefined && v.key !== null)) return v.key;
      if ('value' in v && (v.value !== undefined && v.value !== null)) return v.value;
    }
    return v;
  }
  return undefined;
}

/**
 * Get field value from a row with uppercase-key normalization (matches framework getFldValFrmRow).
 * selectedTabRow/record may have mixed-case keys; FormGetField looks up with uppercase field name.
 * @param {Object} row - Single record object
 * @param {string} fieldName - Field name (will be uppercased for lookup)
 * @returns {*} Value or undefined
 */
function getFieldFromRowNormalized(row, fieldName) {
  if (!row || typeof row !== 'object' || !fieldName) return undefined;
  const keyUpper = String(fieldName).toUpperCase();
  const normalized = {};
  for (const key in row) {
    if (!Object.prototype.hasOwnProperty.call(row, key)) continue;
    normalized[key.toUpperCase()] = row[key];
  }
  const v = normalized[keyUpper];
  if (v === undefined || v === null) return undefined;
  if (typeof v === 'object' && v !== null && !Array.isArray(v)) {
    if (v.key !== undefined && v.key !== null) return v.key;
    if (v.value !== undefined && v.value !== null) return v.value;
  }
  return v;
}

/**
 * Get first non-empty or numeric value from record for keys matching any of the substrings.
 * Use when the first matching key might have an empty string and another matching key has the value.
 */
function getFirstNonEmptyFromRecordByKeyPattern(fullRecord, keySubstrings) {
  if (!fullRecord || !Array.isArray(keySubstrings) || keySubstrings.length === 0) return undefined;
  const record = Array.isArray(fullRecord) ? fullRecord[0] : fullRecord;
  if (!record || typeof record !== 'object') return undefined;
  const subs = keySubstrings.map(s => s.toUpperCase());
  let fallback;
  for (const key in record) {
    if (!record.hasOwnProperty(key)) continue;
    const keyUpper = key.toUpperCase();
    if (!subs.some(s => keyUpper.includes(s))) continue;
    const v = record[key];
    if (v === undefined || v === null) continue;
    let scalar = v;
    if (typeof v === 'object' && v !== null && !Array.isArray(v)) {
      if ('key' in v && (v.key !== undefined && v.key !== null)) scalar = v.key;
      else if ('value' in v && (v.value !== undefined && v.value !== null)) scalar = v.value;
      else continue;
    }
    const s = String(scalar).trim();
    if (s !== '' || (typeof scalar === 'number' && !Number.isNaN(scalar))) {
      return scalar;
    }
    if (fallback === undefined) fallback = scalar;
  }
  return fallback;
}

/**
 * Handle View All Active Items button click
 * RQ_HSM_27_01_26_22_57.03: View All Active Items
 * Implements the logic from ChemicalRegisterCategory::DisplayCustomButtonClicked for VIEW_ALL_ACTIVE_ITEMS
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 74-75: VIEW_ALL_ACTIVE_ITEMS button handler
 *   ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'Y')");
 * 
 * @param {string} buttonName - The button name (e.g. VIEW_ALL_ACTIVE_ITEMS)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewAllActiveItemsButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    console.log('[Web_HSE] ===== View All Active Items Button Handler START =====');
    console.log('[Web_HSE] Button clicked! buttonName:', buttonName);
    console.log('[Web_HSE] screenTag:', screenTag);
    console.log('[Web_HSE] eventObj:', eventObj);
    console.log('[Web_HSE] devInterface available:', !!devInterface);
    console.log('[Web_HSE] devInterface keys:', devInterface ? Object.keys(devInterface).slice(0, 10) : 'devInterface is null');
    
    // First, show a toast to confirm the button is being detected
    toast.info('View All Active Items button clicked - applying filter...');
    
    const {
      FormGetField,
      FormSetField,
      openScr,
      refreshData,
      executeSQL,
      executeSQLPromise,
    } = devInterface;

    // Validate required functions
    if (!openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View All Active Items button');
      toast.error('System error: Required functions not available');
      return;
    }

    // Determine the correct Chemical Register screen tag
    // The button can be clicked from any screen, but we need to filter the Chemical Register screen
    // The button should always filter the Chemical Register screen, regardless of where it's clicked
    
    // Get the screen tag from eventObj if available (more reliable)
    const eventScreenTag = eventObj?.strScrTag || eventObj?.strScrTag || '';
    
    // Check if the current screen is a Chemical Register screen
    const isCurrentScreenChemicalRegister = 
      (screenTag && (
        screenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        screenTag.toString().includes('ChmclRgstr')
      )) ||
      (eventScreenTag && (
        eventScreenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        eventScreenTag.toString().includes('ChmclRgstr')
      ));
    
    // Use the current screen tag if it's a Chemical Register screen, otherwise use default
    // This allows the button to work from any screen but always filter the Chemical Register
    const formTag = isCurrentScreenChemicalRegister 
      ? (screenTag || eventScreenTag || 'HSE_ChmclRgstr')
      : 'HSE_ChmclRgstr';
    const tableName = 'HSE_CHMCLRGSTR';
    
    // C++: ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'Y')");
    // This calls ChangeFormSQL(strCriteria) which changes the SQL for the current form
    // The form then automatically refreshes with the new criteria
    // 
    // In web framework, we use openScr with the Chemical Register screen tag and criteria
    // The button works from any screen but always filters the Chemical Register screen
    // The criteria format should be just the WHERE clause as the C++ code passes it
    const criteria = "WHERE (ChmclRgstr_Actv = 'Y')";
    
    console.log('[Web_HSE] View All Active Items button clicked:', {
      currentScreenTag: screenTag,
      eventScreenTag: eventScreenTag,
      formTag: formTag,
      tableName: tableName,
      criteria: criteria,
      isCurrentScreenChemicalRegister: isCurrentScreenChemicalRegister,
    });

    // C++ equivalent: ChangeCriteria changes the SQL for the Chemical Register screen and refreshes it
    // Web framework: We need to set the criteria and then refresh the screen
    // openScr(screenTag, tabTag, criteria, mode, bNew, defValues, bLocked)
    // For list screens, use 'list' mode, bNew=false to refresh if already open
    try {
      console.log('[Web_HSE] Applying filter criteria to Chemical Register screen:', {
        screenTag: formTag,
        criteria: criteria,
      });
      
      // Step 1: Open/refresh the Chemical Register screen with the filter criteria
      // This matches the C++ ChangeCriteria behavior - changes filter on Chemical Register screen
      // The button works from any screen but always filters the Chemical Register
      // If we're already on the Chemical Register screen, this should refresh it with the new criteria
      // If we're on a different screen, this will open/focus the Chemical Register screen with the filter
      openScr(formTag, {}, criteria, 'list', false, {});
      
      // Step 2: Explicitly refresh the screen to ensure the filter is applied and data is reloaded
      // C++ ChangeCriteria automatically refreshes, so we need to do the same in web framework
      // Use a small delay to ensure openScr completes before refreshing
      if (refreshData) {
        // Use setTimeout to ensure openScr has time to set the criteria before refresh
        setTimeout(() => {
          console.log('[Web_HSE] Refreshing screen to apply filter...');
          // Refresh all data to show the filtered results
          // Use REFRESH_ALL to refresh the entire screen with the new criteria
          refreshData('', 'REFRESH_ALL');
          console.log('[Web_HSE] ✓ Screen refreshed with filter applied');
        }, 200);
      } else {
        // If refreshData is not available, the openScr should handle the refresh
        console.log('[Web_HSE] refreshData not available, relying on openScr to refresh');
      }
      
      console.log('[Web_HSE] ✓ Filter applied successfully to Chemical Register screen');
      toast.success('Showing all active items in Chemical Register');
    } catch (error) {
      console.error('[Web_HSE] Error applying filter:', error);
      
      // Fallback: Try to refresh even if openScr failed
      if (refreshData) {
        console.log('[Web_HSE] Attempting fallback refresh...');
        try {
          refreshData('', 'REFRESH_ALL');
        } catch (refreshError) {
          console.error('[Web_HSE] Fallback refresh also failed:', refreshError);
        }
      }
      
      toast.error('Unable to apply filter: ' + (error.message || 'Unknown error'));
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewAllActiveItemsButton:', error);
    toast.error('An error occurred while filtering active items: ' + (error.message || 'Unknown error'));
  }
}

/**
 * Handle View All In-Active Items button click
 * RQ_HSM_27_01_26_22_57.04: View All In-Active Items
 * Implements the logic from ChemicalRegisterCategory::DisplayCustomButtonClicked for VIEW_ALL_IN-ACTIVE_ITEMS
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 76-77: VIEW_ALL_IN-ACTIVE_ITEMS button handler
 *   ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'N')");
 * 
 * @param {string} buttonName - The button name (e.g. VIEW_ALL_IN-ACTIVE_ITEMS)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewAllInActiveItemsButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    console.log('[Web_HSE] ===== View All In-Active Items Button Handler START =====');
    console.log('[Web_HSE] Button clicked! buttonName:', buttonName);
    console.log('[Web_HSE] screenTag:', screenTag);
    console.log('[Web_HSE] eventObj:', eventObj);
    console.log('[Web_HSE] devInterface available:', !!devInterface);
    
    // First, show a toast to confirm the button is being detected
    toast.info('View All In-Active Items button clicked - applying filter...');
    
    const {
      FormGetField,
      FormSetField,
      openScr,
      refreshData,
      executeSQL,
      executeSQLPromise,
    } = devInterface;

    // Validate required functions
    if (!openScr) {
      console.error('[Web_HSE] Missing required devInterface functions for View All In-Active Items button');
      toast.error('System error: Required functions not available');
      return;
    }

    // Determine the correct Chemical Register screen tag
    // The button can be clicked from any screen, but we need to filter the Chemical Register screen
    // The button should always filter the Chemical Register screen, regardless of where it's clicked
    
    // Get the screen tag from eventObj if available (more reliable)
    const eventScreenTag = eventObj?.strScrTag || eventObj?.strScrTag || '';
    
    // Check if the current screen is a Chemical Register screen
    const isCurrentScreenChemicalRegister = 
      (screenTag && (
        screenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        screenTag.toString().includes('ChmclRgstr')
      )) ||
      (eventScreenTag && (
        eventScreenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        eventScreenTag.toString().includes('ChmclRgstr')
      ));
    
    // Use the current screen tag if it's a Chemical Register screen, otherwise use default
    // This allows the button to work from any screen but always filter the Chemical Register
    const formTag = isCurrentScreenChemicalRegister 
      ? (screenTag || eventScreenTag || 'HSE_ChmclRgstr')
      : 'HSE_ChmclRgstr';
    const tableName = 'HSE_CHMCLRGSTR';
    
    // C++: ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'N')");
    // This calls ChangeFormSQL(strCriteria) which changes the SQL for the current form
    // The form then automatically refreshes with the new criteria
    // 
    // In web framework, we use openScr with the Chemical Register screen tag and criteria
    // The button works from any screen but always filters the Chemical Register screen
    // The criteria format should be just the WHERE clause as the C++ code passes it
    const criteria = "WHERE (ChmclRgstr_Actv = 'N')";
    
    console.log('[Web_HSE] View All In-Active Items button clicked:', {
      currentScreenTag: screenTag,
      eventScreenTag: eventScreenTag,
      formTag: formTag,
      tableName: tableName,
      criteria: criteria,
      isCurrentScreenChemicalRegister: isCurrentScreenChemicalRegister,
    });

    // C++ equivalent: ChangeCriteria changes the SQL for the Chemical Register screen and refreshes it
    // Web framework: We need to set the criteria and then refresh the screen
    // openScr(screenTag, tabTag, criteria, mode, bNew, defValues, bLocked)
    // For list screens, use 'list' mode, bNew=false to refresh if already open
    try {
      console.log('[Web_HSE] Applying filter criteria to Chemical Register screen:', {
        screenTag: formTag,
        criteria: criteria,
      });
      
      // Step 1: Open/refresh the Chemical Register screen with the filter criteria
      // This matches the C++ ChangeCriteria behavior - changes filter on Chemical Register screen
      // The button works from any screen but always filters the Chemical Register
      // If we're already on the Chemical Register screen, this should refresh it with the new criteria
      // If we're on a different screen, this will open/focus the Chemical Register screen with the filter
      openScr(formTag, {}, criteria, 'list', false, {});
      
      // Step 2: Explicitly refresh the screen to ensure the filter is applied and data is reloaded
      // C++ ChangeCriteria automatically refreshes, so we need to do the same in web framework
      // Use a small delay to ensure openScr completes before refreshing
      if (refreshData) {
        // Use setTimeout to ensure openScr has time to set the criteria before refresh
        setTimeout(() => {
          console.log('[Web_HSE] Refreshing screen to apply filter...');
          // Refresh all data to show the filtered results
          // Use REFRESH_ALL to refresh the entire screen with the new criteria
          refreshData('', 'REFRESH_ALL');
          console.log('[Web_HSE] ✓ Screen refreshed with filter applied');
        }, 200);
      } else {
        // If refreshData is not available, the openScr should handle the refresh
        console.log('[Web_HSE] refreshData not available, relying on openScr to refresh');
      }
      
      console.log('[Web_HSE] ✓ Filter applied successfully to Chemical Register screen');
      toast.success('Showing all in-active items in Chemical Register');
    } catch (error) {
      console.error('[Web_HSE] Error applying filter:', error);
      
      // Fallback: Try to refresh even if openScr failed
      if (refreshData) {
        console.log('[Web_HSE] Attempting fallback refresh...');
        try {
          refreshData('', 'REFRESH_ALL');
        } catch (refreshError) {
          console.error('[Web_HSE] Fallback refresh also failed:', refreshError);
        }
      }
      
      toast.error('Unable to apply filter: ' + (error.message || 'Unknown error'));
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewAllInActiveItemsButton:', error);
    toast.error('An error occurred while filtering in-active items: ' + (error.message || 'Unknown error'));
  }
}

/**
 * Handle View Department Items button click
 * RQ_HSM_27_01_26_22_57.05: View Department Items
 * Implements the logic from ChemicalRegisterCategory::DisplayCustomButtonClicked for VIEW_DEPARTMENT_ITEMS
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 78-80: VIEW_DEPARTMENT_ITEMS button handler
 *   CString strDprtmnt = FormGetField("HSE_CHMCLRGSTR","ChmclRgstr_DprtmntNm");
 *   ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_DprtmntNm = '"+strDprtmnt+"')");
 * 
 * @param {string} buttonName - The button name (e.g. VIEW_DEPARTMENT_ITEMS)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing devInterface functions
 */
export async function handleViewDepartmentItemsButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    console.log('[Web_HSE] ===== View Department Items Button Handler START =====');
    console.log('[Web_HSE] Button clicked! buttonName:', buttonName);
    console.log('[Web_HSE] screenTag:', screenTag);
    console.log('[Web_HSE] eventObj:', eventObj);
    console.log('[Web_HSE] devInterface available:', !!devInterface);
    
    // First, show a toast to confirm the button is being detected
    toast.info('View Department Items button clicked - applying filter...');
    
    const {
      FormGetField,
      FormSetField,
      openScr,
      refreshData,
      executeSQL,
      executeSQLPromise,
    } = devInterface;

    // Validate required functions
    if (!openScr || !FormGetField) {
      console.error('[Web_HSE] Missing required devInterface functions for View Department Items button');
      toast.error('System error: Required functions not available');
      return;
    }

    // Determine the correct Chemical Register screen tag
    // The button can be clicked from any screen, but we need to filter the Chemical Register screen
    // The button should always filter the Chemical Register screen, regardless of where it's clicked
    
    // Get the screen tag from eventObj if available (more reliable)
    const eventScreenTag = eventObj?.strScrTag || eventObj?.strScrTag || '';
    
    // Check if the current screen is a Chemical Register screen
    const isCurrentScreenChemicalRegister = 
      (screenTag && (
        screenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        screenTag.toString().includes('ChmclRgstr')
      )) ||
      (eventScreenTag && (
        eventScreenTag.toString().toUpperCase().includes('CHMCLRGSTR') ||
        eventScreenTag.toString().includes('ChmclRgstr')
      ));
    
    // Use the current screen tag if it's a Chemical Register screen, otherwise use default
    // This allows the button to work from any screen but always filter the Chemical Register
    const formTag = isCurrentScreenChemicalRegister 
      ? (screenTag || eventScreenTag || 'HSE_ChmclRgstr')
      : 'HSE_ChmclRgstr';
    const tableName = 'HSE_CHMCLRGSTR';
    
    // C++: CString strDprtmnt = FormGetField("HSE_CHMCLRGSTR","ChmclRgstr_DprtmntNm");
    // Get the department name from the current form
    // Try multiple form tags to find the department field
    let strDprtmnt = '';
    const possibleFormTags = [formTag, 'HSE_CHMCLRGSTR', 'HSE_ChmclRgstr', 'HSE_TGCHMCLRGSTR'];
    const departmentFieldName = 'ChmclRgstr_DprtmntNm';
    
    console.log('[Web_HSE] Attempting to get department name from form fields...');
    console.log('[Web_HSE] eventObj.fullRecord:', eventObj?.fullRecord);
    console.log('[Web_HSE] eventObj.allSelectedRecords:', eventObj?.allSelectedRecords);
    
    // First, try to get from eventObj fullRecord (most reliable when a record is selected)
    if (eventObj?.fullRecord) {
      strDprtmnt = getFieldFromFullRecord(eventObj.fullRecord, departmentFieldName);
      if (strDprtmnt && strDprtmnt.trim() !== '') {
        console.log('[Web_HSE] ✓ Found department name from eventObj.fullRecord:', strDprtmnt);
      }
    }
    
    // Also try allSelectedRecords if fullRecord didn't work
    if ((!strDprtmnt || strDprtmnt.trim() === '') && eventObj?.allSelectedRecords) {
      strDprtmnt = getFieldFromFullRecord(eventObj.allSelectedRecords, departmentFieldName);
      if (strDprtmnt && strDprtmnt.trim() !== '') {
        console.log('[Web_HSE] ✓ Found department name from eventObj.allSelectedRecords:', strDprtmnt);
      }
    }
    
    // If not found in eventObj, try to get from form using FormGetField
    // FormGetField(tagNam, FldNam, scrType) - scrType should be "scr" for screen
    if (!strDprtmnt || strDprtmnt.trim() === '') {
      console.log('[Web_HSE] Department not found in eventObj, trying FormGetField...');
      for (const tag of possibleFormTags) {
        // Try with "scr" type first (screen)
        strDprtmnt = safeFormGetField(FormGetField, tag, departmentFieldName, 'scr');
        if (strDprtmnt && strDprtmnt.trim() !== '') {
          console.log('[Web_HSE] ✓ Found department name from form tag (scr):', tag, 'value:', strDprtmnt);
          break;
        }
        // Also try with "tab" type in case we're on a tab
        strDprtmnt = safeFormGetField(FormGetField, tag, departmentFieldName, 'tab');
        if (strDprtmnt && strDprtmnt.trim() !== '') {
          console.log('[Web_HSE] ✓ Found department name from form tag (tab):', tag, 'value:', strDprtmnt);
          break;
        }
      }
    }
    
    // Log all available fields in fullRecord for debugging
    if (eventObj?.fullRecord) {
      const record = Array.isArray(eventObj.fullRecord) ? eventObj.fullRecord[0] : eventObj.fullRecord;
      if (record && typeof record === 'object') {
        const allFields = Object.keys(record);
        const deptFields = allFields.filter(key => 
          key.toUpperCase().includes('DPRTMNT') || 
          key.toUpperCase().includes('DEPARTMENT')
        );
        console.log('[Web_HSE] Available department-related fields in fullRecord:', deptFields);
        if (deptFields.length > 0) {
          deptFields.forEach(field => {
            console.log(`[Web_HSE]   ${field}:`, record[field]);
          });
        }
        // Also log all fields if department field not found
        if (deptFields.length === 0) {
          console.log('[Web_HSE] All available fields in fullRecord:', allFields.slice(0, 20)); // Limit to first 20
        }
      }
    }
    
    if (!strDprtmnt || strDprtmnt.trim() === '') {
      toast.warning('Please select a record with a department name first, or enter a department name in the current record.');
      console.warn('[Web_HSE] Department name not found. Cannot apply filter.');
      console.warn('[Web_HSE] Searched in:', {
        fullRecord: !!eventObj?.fullRecord,
        allSelectedRecords: !!eventObj?.allSelectedRecords,
        formTags: possibleFormTags,
        eventObjKeys: eventObj ? Object.keys(eventObj) : [],
      });
      return;
    }
    
    // C++: ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_DprtmntNm = '"+strDprtmnt+"')");
    // Escape single quotes in department name to prevent SQL injection
    const escapedDprtmnt = strDprtmnt.toString().replace(/'/g, "''");
    const criteria = `WHERE (ChmclRgstr_DprtmntNm = '${escapedDprtmnt}')`;
    
    console.log('[Web_HSE] View Department Items button clicked:', {
      currentScreenTag: screenTag,
      eventScreenTag: eventScreenTag,
      formTag: formTag,
      tableName: tableName,
      departmentName: strDprtmnt,
      criteria: criteria,
      isCurrentScreenChemicalRegister: isCurrentScreenChemicalRegister,
    });

    // C++ equivalent: ChangeCriteria changes the SQL for the Chemical Register screen and refreshes it
    // Web framework: We need to set the criteria and then refresh the screen
    // openScr(screenTag, tabTag, criteria, mode, bNew, defValues, bLocked)
    // For list screens, use 'list' mode, bNew=false to refresh if already open
    try {
      console.log('[Web_HSE] Applying filter criteria to Chemical Register screen:', {
        screenTag: formTag,
        criteria: criteria,
        departmentName: strDprtmnt,
      });
      
      // Step 1: Open/refresh the Chemical Register screen with the filter criteria
      // This matches the C++ ChangeCriteria behavior - changes filter on Chemical Register screen
      // The button works from any screen but always filters the Chemical Register
      // If we're already on the Chemical Register screen, this should refresh it with the new criteria
      // If we're on a different screen, this will open/focus the Chemical Register screen with the filter
      openScr(formTag, {}, criteria, 'list', false, {});
      
      // Step 2: Explicitly refresh the screen to ensure the filter is applied and data is reloaded
      // C++ ChangeCriteria automatically refreshes, so we need to do the same in web framework
      // Use a small delay to ensure openScr completes before refreshing
      if (refreshData) {
        // Use setTimeout to ensure openScr has time to set the criteria before refresh
        setTimeout(() => {
          console.log('[Web_HSE] Refreshing screen to apply filter...');
          // Refresh all data to show the filtered results
          // Use REFRESH_ALL to refresh the entire screen with the new criteria
          refreshData('', 'REFRESH_ALL');
          console.log('[Web_HSE] ✓ Screen refreshed with filter applied');
        }, 200);
      } else {
        // If refreshData is not available, the openScr should handle the refresh
        console.log('[Web_HSE] refreshData not available, relying on openScr to refresh');
      }
      
      console.log('[Web_HSE] ✓ Filter applied successfully to Chemical Register screen');
      toast.success(`Showing items for department: ${strDprtmnt}`);
    } catch (error) {
      console.error('[Web_HSE] Error applying filter:', error);
      
      // Fallback: Try to refresh even if openScr failed
      if (refreshData) {
        console.log('[Web_HSE] Attempting fallback refresh...');
        try {
          refreshData('', 'REFRESH_ALL');
        } catch (refreshError) {
          console.error('[Web_HSE] Fallback refresh also failed:', refreshError);
        }
      }
      
      toast.error('Unable to apply filter: ' + (error.message || 'Unknown error'));
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewDepartmentItemsButton:', error);
    toast.error('An error occurred while filtering department items: ' + (error.message || 'Unknown error'));
  }
}

/** QTY Card tab tag - Update button only applies on this tab */
const QTY_CARD_TAB_TAG = 'HSE_CHMCLRGSTR_QTYCRD';
const CHEMICAL_REGISTER_FORM_TAG = 'HSE_CHMCLRGSTR';

/**
 * Handle Update (UPDT) button click on QTY Card tab
 * RQ_HSM_27_01_26_22_57.12: Update - Chemical Register QTY Card Custom Buttons
 * Implements ChemicalRegisterCategory::UpdateQuantities() from DisplayCustomButtonClicked when strSubFormTag==HSE_CHMCLRGSTR_QTYCRD && strButtonName==UPDT
 *
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 82-84: else if(strSubFormTag=="HSE_CHMCLRGSTR_QTYCRD" && strButtonName=="UPDT") { UpdateQuantities(); }
 * - ChemicalRegisterCategory.cpp line 109-137: UpdateQuantities() - if QOH Before empty: save tab, then exec Chemical_Register_Update_Qty; else message "This record was already used to update"
 *
 * @param {string} buttonName - The button name (UPDT)
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object (strTabTag must be HSE_CHMCLRGSTR_QTYCRD)
 * @param {Object} devInterface - Object containing FormGetField, doToolbarAction, executeSQLPromise, refreshData
 */
export async function handleQtyCardUpdateButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const strTabTag = (eventObj?.strTabTag || '').toString();
    const tabTagUpper = strTabTag.toUpperCase();
    if (tabTagUpper !== QTY_CARD_TAB_TAG && !tabTagUpper.includes('QTYCRD')) {
      console.log('[Web_HSE] QTY Card Update button ignored - not on QTY Card tab. strTabTag:', strTabTag);
      return;
    }

    console.log('[Web_HSE] ===== QTY Card Update (UPDT) Button Handler =====');
    toast.info('Updating quantities...');

    const {
      FormGetField,
      doToolbarAction,
      executeSQLPromise,
      executeSQL,
      refreshData,
    } = devInterface;

    const executeSQLAsync = executeSQLPromise || executeSQL;
    if (!FormGetField || !executeSQLAsync) {
      console.error('[Web_HSE] Missing FormGetField or executeSQL for QTY Card Update');
      toast.error('System error: Required functions not available');
      return;
    }

    const tabTag = tabTagUpper === QTY_CARD_TAB_TAG ? strTabTag : QTY_CARD_TAB_TAG;
    const formTag = CHEMICAL_REGISTER_FORM_TAG;

    // C++: strQtyBfr = FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QTYCRD_QHBFR");
    const strQtyBfr = safeFormGetField(FormGetField, tabTag, 'CHMCLRGSTR_QTYCRD_QHBFR', 'tab')
      || safeFormGetField(FormGetField, tabTag, 'CHMCLRGSTR_QTYCRD_QHBFR', 'scr');

    if (strQtyBfr !== undefined && strQtyBfr !== null && String(strQtyBfr).trim() !== '') {
      toast.warning('This record was already used to update');
      return;
    }

    // Helper: get a value from form or from fullRecord (tab row)
    const fromForm = (tag, fld, scrType) => safeFormGetField(FormGetField, tag, fld, scrType);
    const fromRecord = (rec, fld) => getFieldFromFullRecord(rec, fld);
    const fromRecordByPattern = (rec, subs) => getValueFromRecordByKeyPattern(rec, subs);
    const fromRecordByPatternNonEmpty = (rec, subs) => getFirstNonEmptyFromRecordByKeyPattern(rec, subs);
    // Tab row may have mixed-case keys; normalize to uppercase like framework getFldValFrmRow
    const fromRowNormalized = (row, fld) => (row ? getFieldFromRowNormalized(row, fld) : undefined);

    // Prefer reading from fullRecord first (selected tab row when button clicked) before any save
    const tabRow = Array.isArray(eventObj?.fullRecord) ? eventObj.fullRecord[0] : eventObj?.fullRecord;
    const altRow = Array.isArray(eventObj?.allSelectedRecords) ? eventObj.allSelectedRecords[0] : eventObj?.allSelectedRecords;
    const anyRow = tabRow || altRow;
    // Detail table/tab can be mixed case (e.g. HSE_CHMCLRGSTR_QtyCrd) - try both
    const tblNam = (eventObj?.strTblNam || tabTag || '').toString();

    let strMasterPK = fromRecord(eventObj?.fullRecord, 'CHMCLRGSTR_prmryKy')
      || fromRecord(eventObj?.fullRecord, 'ChmclRgstr_PrmryKy')
      || fromRecord(eventObj?.allSelectedRecords, 'CHMCLRGSTR_prmryKy')
      || fromRecordByPattern(eventObj?.fullRecord, ['PRMRYKY', 'PRMRY_KY', 'CHMCLRGSTR_PRM'])
      || fromForm(formTag, 'CHMCLRGSTR_prmryKy', 'scr')
      || fromForm(formTag, 'ChmclRgstr_PrmryKy', 'scr');

    let strTxnType = fromRecord(eventObj?.fullRecord, 'CHMCLRGSTR_QTYCRD_TXNTYP')
      || fromRecord(eventObj?.allSelectedRecords, 'CHMCLRGSTR_QTYCRD_TXNTYP')
      || fromRecordByPattern(eventObj?.fullRecord, ['TXNTYP', 'TXN_TYP'])
      || fromForm(tabTag, 'CHMCLRGSTR_QTYCRD_TXNTYP', 'tab')
      || fromForm(tabTag, 'CHMCLRGSTR_QTYCRD_TXNTYP', 'scr');

    // C++ uses CHMCLRGSTR_QtyCrd_SrlN; screen JSON HSE_CHMCLRGSTR_QtyCrd.json uses DataFieldName CHMCLRGSTR_QTYCRD_SRLN
    const serialFieldNames = [
      'CHMCLRGSTR_QTYCRD_SRLN',
      'CHMCLRGSTR_QtyCrd_SrlN',
      'CHMCLRGSTR_QTYCRD_SrlN',
      'HSE_CHMCLRGSTR_QtyCrd_SrlN',
      'HSE_CHMCLRGSTR_QTYCRD_SrlN',
      (tblNam ? tblNam + '_SrlN' : ''),
      (tblNam ? tblNam + '_SRLN' : ''),
      'SrlN',
      'SRLN',
      'SerialNo',
      'SERIAL_NO',
      'LineNo',
      'LINE_NO',
    ].filter(Boolean);
    const txnQtyFieldNames = ['CHMCLRGSTR_QtyCrd_TxnQty', 'CHMCLRGSTR_QTYCRD_TxnQty', 'CHMCLRGSTR_QTYCRD_TXNQTY'];
    let strDtlSrial = '';
    let strTxnQty = '';
    for (const f of serialFieldNames) {
      const v = fromRecord(eventObj?.fullRecord, f) || fromRecord(eventObj?.allSelectedRecords, f)
        || fromRowNormalized(anyRow, f)
        || fromForm(tabTag, f, 'tab') || fromForm(tabTag, f, 'scr');
      if (v !== undefined && v !== null && (String(v).trim() !== '' || (typeof v === 'number' && !Number.isNaN(v)))) {
        strDtlSrial = v;
        break;
      }
    }
    if (!strDtlSrial && anyRow) {
      const v = fromRecordByPatternNonEmpty(eventObj?.fullRecord, ['SRLN', 'SRIN', 'SERIAL', 'LINENO', 'SRL']);
      if (v !== undefined && v !== null) strDtlSrial = v;
    }
    if (!strDtlSrial && anyRow) {
      const v = fromRecordByPattern(eventObj?.fullRecord, ['SRLN', 'SRIN', 'SERIAL', 'LINENO', 'SRL']);
      if (v !== undefined && v !== null) strDtlSrial = v;
    }
    // Fallback: scan all keys in tab row for anything that looks like serial/line (server may use different names)
    if (!strDtlSrial && anyRow && typeof anyRow === 'object') {
      const serialLike = ['SRL', 'SERIAL', 'LINE', 'SRIN', 'LINENO'];
      for (const key of Object.keys(anyRow)) {
        const keyUpper = key.toUpperCase();
        if (keyUpper.includes('TXNQTY') || keyUpper.includes('TXN_TYP')) continue;
        if (!serialLike.some(s => keyUpper.includes(s))) continue;
        const v = anyRow[key];
        if (v === undefined || v === null) continue;
        const scalar = typeof v === 'object' && v !== null && !Array.isArray(v)
          ? (v.key !== undefined && v.key !== null ? v.key : (v.value !== undefined && v.value !== null ? v.value : null))
          : v;
        if (scalar === null) continue;
        const s = String(scalar).trim();
        if (s !== '' || (typeof scalar === 'number' && !Number.isNaN(scalar))) {
          strDtlSrial = scalar;
          break;
        }
      }
    }
    for (const f of txnQtyFieldNames) {
      const v = fromRecord(eventObj?.fullRecord, f) || fromRecord(eventObj?.allSelectedRecords, f)
        || fromForm(tabTag, f, 'tab') || fromForm(tabTag, f, 'scr');
      if (v !== undefined && v !== null && String(v).trim() !== '') {
        strTxnQty = v;
        break;
      }
    }
    if (!strTxnQty && anyRow) {
      const v = fromRecordByPattern(eventObj?.fullRecord, ['TXNQTY', 'TXN_QTY', 'TxnQty']);
      if (v !== undefined && v !== null) strTxnQty = v;
    }

    // If we're missing Serial, TxnQty, or MasterPK: save then read from form (C++ flow).
    // Serial can be empty in fullRecord (e.g. auto-generated) but present in form after save.
    if (!strDtlSrial || !strTxnQty || !strMasterPK) {
      if (doToolbarAction) {
        doToolbarAction('SAVE', formTag, tabTag);
      }
      const delays = [400, 600, 900];
      for (const delayMs of delays) {
        await new Promise(resolve => setTimeout(resolve, delayMs));
        if (!strMasterPK) {
          strMasterPK = fromForm(formTag, 'CHMCLRGSTR_prmryKy', 'scr')
            || fromForm(formTag, 'ChmclRgstr_PrmryKy', 'scr');
        }
        if (!strTxnType) {
          strTxnType = fromForm(tabTag, 'CHMCLRGSTR_QTYCRD_TXNTYP', 'tab')
            || fromForm(tabTag, 'CHMCLRGSTR_QTYCRD_TXNTYP', 'scr');
        }
        for (const f of serialFieldNames) {
          const v = fromForm(tabTag, f, 'tab') || fromForm(tabTag, f, 'scr');
          if (v !== undefined && v !== null && (String(v).trim() !== '' || (typeof v === 'number' && !Number.isNaN(v)))) {
            strDtlSrial = strDtlSrial || v;
            break;
          }
        }
        for (const f of txnQtyFieldNames) {
          const v = fromForm(tabTag, f, 'tab') || fromForm(tabTag, f, 'scr');
          if (v !== undefined && v !== null && String(v).trim() !== '') {
            strTxnQty = strTxnQty || v;
            break;
          }
        }
        if (strDtlSrial && strTxnQty && strMasterPK) break;
      }
    }

    // Last resort: use first numeric value in tab row from a key that looks like serial/line (not QOH/TxnQty)
    if (!strDtlSrial && anyRow && typeof anyRow === 'object') {
      const serialKeyHints = ['SRL', 'SERIAL', 'LINE', 'SRIN', 'LINENO', 'SRN'];
      for (const key of Object.keys(anyRow)) {
        const keyUpper = key.toUpperCase();
        if (keyUpper.includes('TXNQTY') || keyUpper.includes('QHBFR') || keyUpper.includes('QHAFT')) continue;
        if (!serialKeyHints.some(h => keyUpper.includes(h))) continue;
        const v = anyRow[key];
        if (v === undefined || v === null) continue;
        const n = typeof v === 'number' ? v : (typeof v === 'object' && v && (v.key !== undefined || v.value !== undefined) ? (v.key !== undefined && v.key !== null ? Number(v.key) : Number(v.value)) : NaN);
        if (Number.isFinite(n) && n >= 0) {
          strDtlSrial = n;
          break;
        }
        const s = String(v).trim();
        if (s !== '' && !Number.isNaN(Number(s)) && Number(s) >= 0) {
          strDtlSrial = s;
          break;
        }
      }
    }

    // Reject missing or literal "undefined" / "null" (never send these to SQL)
    const safeStr = (v) => (v === undefined || v === null ? '' : String(v).trim());
    const masterPKStr = safeStr(strMasterPK);
    if (!masterPKStr || masterPKStr.toLowerCase() === 'undefined' || masterPKStr.toLowerCase() === 'null') {
      toast.warning('Missing master key. Please save the record and try again.');
      return;
    }

    // Normalize value from form/fullRecord (may be object e.g. { key, value } for combo)
    const toScalar = (v) => {
      if (v === undefined || v === null) return '';
      if (typeof v === 'object' && v !== null && !Array.isArray(v)) {
        if ('key' in v && v.key !== undefined && v.key !== null) return String(v.key);
        if ('value' in v && v.value !== undefined && v.value !== null) return String(v.value);
      }
      return String(v);
    };
    // Parse numeric values: accept number, string number, or string with commas/spaces
    const parseSerial = (v) => {
      const s = toScalar(v).replace(/,/g, '').trim();
      if (s === '' || s.toLowerCase() === 'undefined' || s.toLowerCase() === 'null') return NaN;
      const n = parseInt(s, 10);
      return Number.isNaN(n) ? parseFloat(s) : n;
    };
    const parseTxnQty = (v) => {
      const s = toScalar(v).replace(/,/g, '').trim();
      if (s === '' || s.toLowerCase() === 'undefined' || s.toLowerCase() === 'null') return NaN;
      return parseFloat(s);
    };
    const serialNum = parseSerial(strDtlSrial);
    const txnQtyNum = parseTxnQty(strTxnQty);

    if (Number.isNaN(serialNum) || Number.isNaN(txnQtyNum)) {
      const rec = anyRow || (Array.isArray(eventObj?.fullRecord) ? eventObj.fullRecord[0] : eventObj?.fullRecord);
      const keys = rec && typeof rec === 'object' ? Object.keys(rec) : [];
      const keyValues = rec && typeof rec === 'object' ? keys.reduce((acc, k) => {
        const v = rec[k];
        acc[k] = typeof v === 'object' && v && !Array.isArray(v) ? (v.key ?? v.value ?? v) : v;
        return acc;
      }, {}) : {};
      console.warn('[Web_HSE] QTY Card Update: Serial or TxnQty missing/invalid.', {
        strDtlSrial,
        strTxnQty,
        masterPK: !!strMasterPK,
        tabRowKeys: keys,
        tabRowKeyValues: keyValues,
        hasFullRecord: !!eventObj?.fullRecord,
      });
      toast.warning('Serial and Transaction Qty are required. Please select a QTY Card row and ensure both fields have values, then try again.');
      return;
    }

    const txnTypeFlag = (strTxnType === '1' || String(strTxnType).toUpperCase() === 'TRUE') ? 1 : 0;
    const masterPKEscaped = masterPKStr.replace(/'/g, "''");
    // Ensure no parameter is undefined/NaN when building SQL (prevents "Invalid column name 'undefined'" from DB)
    const serialParam = Number.isFinite(serialNum) ? serialNum : 0;
    const txnQtyParam = Number.isFinite(txnQtyNum) ? txnQtyNum : 0;
    const sql = `exec Chemical_Register_Update_Qty '${masterPKEscaped}',${serialParam},${txnQtyParam},${txnTypeFlag}`;

    try {
      await executeSQLAsync(sql);
      toast.success('Quantities updated successfully');
      if (refreshData) {
        setTimeout(() => refreshData('', 'REFRESH_ALL'), 200);
      }
    } catch (err) {
      console.error('[Web_HSE] Chemical_Register_Update_Qty failed:', err);
      toast.error('Failed to update quantities: ' + (err?.message || 'Unknown error'));
      if (refreshData) {
        try {
          refreshData('', 'REFRESH_ALL');
        } catch (e) {
          console.warn('[Web_HSE] Refresh after error failed:', e);
        }
      }
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleQtyCardUpdateButton:', error);
    toast.error('An error occurred while updating quantities: ' + (error?.message || 'Unknown error'));
  }
}
