import {
  handleEntryCompletedButton,
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
 * 
 * Requirements:
 * - RQ_HSM_22_01_26_10_36.03: Aspects Register
 * - RQ_HSM_22_01_26_10_36.04: Aspects Entry
 * - RQ_HSM_22_01_26_10_37.01: Aspects Entry Custom Buttons
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
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
  console.log('[Web_HSE] Aspects Register Service initialized - 1 button supported (Entry Completed)');
}

export default {
  handleAspectsRegisterButtonClick,
  initializeAspectsRegisterService,
};
