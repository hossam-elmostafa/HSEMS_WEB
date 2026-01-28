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
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 74-75: VIEW_ALL_ACTIVE_ITEMS button handler
 * - ChemicalRegisterCategory.cpp line 76-77: VIEW_ALL_IN-ACTIVE_ITEMS button handler
 * - ChemicalRegisterCategory.cpp line 78-80: VIEW_DEPARTMENT_ITEMS button handler
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
      
      // Match exact uppercase or lowercase
      if (keyUpper === fieldNameUpper || keyLower === fieldNameLower) {
        const value = record[key];
        if (value !== undefined && value !== null && value !== '') {
          return String(value);
        }
      }
    }
  }
  
  return '';
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
