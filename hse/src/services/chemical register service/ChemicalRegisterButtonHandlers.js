import { toast } from "react-toastify";

/**
 * ChemicalRegisterButtonHandlers.js
 * 
 * Handles all button click events for Chemical Register module screens
 * 
 * Requirements:
 * - RQ_HSM_27_01_26_22_57.03: View All Active Items - https://app.clickup.com/t/86c785pwa
 * 
 * C++ Reference:
 * - ChemicalRegisterCategory.cpp line 74-75: VIEW_ALL_ACTIVE_ITEMS button handler
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
