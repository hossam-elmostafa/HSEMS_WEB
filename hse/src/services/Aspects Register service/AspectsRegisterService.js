import {
  handleEntryCompletedButton,
  handleViewCurrentMonthButton,
  handleViewHistoryButton,
} from './AspectsRegisterButtonHandlers';

/**
 * AspectsRegisterService.js
 * 
 * Main service file for Aspects Register module
 * Handles button clicks and integrates with the HSE framework
 * 
 * Button Mappings (based on C++ code):
 * - ENTRY_COMPLETED: Entry Completed button (EnvrnmntAspctCategory.cpp)
 *   - When clicked, completes the aspects entry by executing EnvAspctEntryComplete stored procedure
 * - VIEW_CURRENT_MONTH: View Current Month button (EnvrnmntAspctCategory.cpp line 140)
 *   - When clicked, updates criteria to show only records for current month (ASPCTS_YR, ASPCTS_MNTH)
 * 
 * Requirements:
 * - RQ_HSM_22_01_26_10_36.03: Aspects Register
 * - RQ_HSM_22_01_26_10_36.04: Aspects Entry
 * - RQ_HSM_22_01_26_10_37.01: Aspects Entry Custom Buttons
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
 * - RQ_HSM_27_01_26_22_56.02: View Current Month
 *   Custom Button ID: VIEW_CURRENT_MONTH
 * - RQ_HSM_27_01_26_22_56.03: View History (all history; admin: all depts, non-admin: user's dept(s))
 *   Custom Button ID: VIEW_HISTORY
 */

/**
 * Handle button clicks for Aspects Register module
 * @param {string} buttonName - The button name/ID
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object from the framework
 * @param {Object} devInterface - Object containing devInterface functions
 * @returns {Promise<boolean>} - Returns true if button was handled, false otherwise
 */
export async function handleAspectsRegisterButtonClick(buttonName, screenTag, eventObj, devInterface) {
  console.log('[Web_HSE] ===== Aspects Register Button Click Handler =====');
  console.log('[Web_HSE] Button Name:', buttonName);
  console.log('[Web_HSE] Screen Tag:', screenTag);
  console.log('[Web_HSE] Event Object:', eventObj);

  // Normalize button name to uppercase for comparison
  const normalizedButtonName = buttonName?.toUpperCase() || '';
  
  // Normalize screen tag to check if this is an Aspects Register screen
  const normalizedScreenTag = screenTag?.toUpperCase() || '';
  const isAspectsScreen = normalizedScreenTag.includes('ASPCTS') || 
                          normalizedScreenTag === 'HSE_ASPCTSENTRYATENTRY' ||
                          normalizedScreenTag === 'HSE_ASPCTSRVWATRVW';

  // Only handle buttons for Aspects Register screens
  if (!isAspectsScreen) {
    return false;
  }

  try {
    // Entry Completed button (Entry screen)
    // C++ Reference: EnvrnmntAspctCategory.cpp line 152: ENTRY_COMPLETED
    // Button ID: ENTRY_COMPLETED
    if (normalizedButtonName === 'ENTRY_COMPLETED') {
      await handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // View Current Month button (Aspects Entry)
    // C++ Reference: EnvrnmntAspctCategory.cpp line 140: VIEW_CURRENT_MONTH
    if (normalizedButtonName === 'VIEW_CURRENT_MONTH') {
      await handleViewCurrentMonthButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // View History button (RQ_HSM_27_01_26_22_56.03) - all history, admin vs non-admin department filter
    // C++ Reference: EnvrnmntAspctCategory.cpp lines 131-139: VIEW_HISTORY
    if (normalizedButtonName === 'VIEW_HISTORY') {
      await handleViewHistoryButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Button not handled by this service
    console.log('[Web_HSE] Button not handled by Aspects Register service:', buttonName);
    return false;
  } catch (error) {
    console.error('[Web_HSE] Error in handleAspectsRegisterButtonClick:', error);
    return false;
  }
}

// Flag to track if service has been initialized
let isInitialized = false;

/**
 * Initialize Aspects Register service
 * This function can be called when the application starts
 * It will only log initialization message once to avoid console spam
 */
export function initializeAspectsRegisterService() {
  // Only initialize once
  if (isInitialized) {
    return;
  }
  
  isInitialized = true;
  // Only log a simple message to reduce console spam
  console.log('[Web_HSE] Aspects Register Service initialized - 3 buttons (Entry Completed, View Current Month, View History)');
}

export default {
  handleAspectsRegisterButtonClick,
  initializeAspectsRegisterService,
};
