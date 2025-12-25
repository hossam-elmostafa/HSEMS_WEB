import { OBSERVATION_SCREEN_TAGS } from '../config/constants';

// Import button handlers from ObservationButtonHandlers
import {
  handleViewRejectReasonButton,
  handleRejectButton,
  handleConfirmButton,
  handleCancelButton,
  handleEntryCompleteButton,
  handleRejectReasonOkButton,
  setPendingRejectObservation,
  clearPendingRejectObservation,
} from './ObservationButtonHandlers';

// Import tab management functions from ObservationTabManagement
import {
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
} from './ObservationTabManagement';

// Re-export for backward compatibility
export {
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
  setPendingRejectObservation,
  clearPendingRejectObservation,
};

/**
 * ObservationService.js
 * 
 * Main entry point for Observation module functionality.
 * Routes button clicks to appropriate handlers and provides utility functions.
 * 
 * This file has been refactored to delegate to:
 * - ObservationButtonHandlers.js: All button click handlers
 * - ObservationTabManagement.js: Tab enabling/disabling and Comments tab management
 */

/**
 * Check if a screen tag belongs to the Observation module
 * @param {string} screenTag - The screen tag to check
 * @returns {boolean} - True if the screen is an observation screen
 */
export function isObservationScreen(screenTag) {
  if (!screenTag) return false;
  
  const normalizedTag = screenTag.toString().toUpperCase();
  return OBSERVATION_SCREEN_TAGS.some(tag => normalizedTag === tag.toUpperCase());
}

/**
 * Send button click event to Web_HSE backend
 * Handles Reject, View Reject Reason, Confirm, Cancel, and Entry Complete buttons on Observation module screens
 * 
 * This is the main routing function that delegates to specific button handlers.
 * 
 * @param {string} buttonName - The button name/ID to log
 * @param {string} screenTag - The screen tag to check if it's an observation screen
 * @param {Object} eventObj - The full event object containing fullRecord, strScrTag, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 */
export function sendButtonClickToBackend(buttonName, screenTag, eventObj = {}, devInterface = {}) {
  // Debug logging to see what's being passed
  console.log('[Web_HSE Debug] Button clicked:', {
    buttonName,
    screenTag,
    isObservationScreen: isObservationScreen(screenTag)
  });
  
  // Normalize button name for comparison (buttonName is already normalized to uppercase in buttonEvents.js)
  const normalizedButton = buttonName ? buttonName.toString().toUpperCase() : '';
  const normalizedScreenTag = screenTag ? screenTag.toString().toUpperCase() : '';
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_OK
  // Handle reject reason screen OK button (this is part of observation module flow)
  // Check both possible screen tag casings: HSE_TGRJCTRSN (uppercase) or HSE_TGRJCTRSN (mixed case from config)
  if ((normalizedScreenTag === 'HSE_TGRJCTRSN' || normalizedScreenTag === 'HSE_TGRJCTRSN') && normalizedButton === 'RJCTRSN_BTN_OK') {
    console.log('[Web_HSE] ✓ Reject reason screen OK button (RJCTRSN_BTN_OK) clicked!');
    handleRejectReasonOkButton(devInterface);
    return; // Don't process further
  }
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_CANCEL
  // Check both possible screen tag casings: HSE_TGRJCTRSN (uppercase) or HSE_TGRJCTRSN (mixed case)
  if ((normalizedScreenTag === 'HSE_TGRJCTRSN' || normalizedScreenTag === 'HSE_TGRJCTRSN') && normalizedButton === 'RJCTRSN_BTN_CANCEL') {
    console.log('[Web_HSE] Reject reason screen Cancel button clicked. Clearing pending rejection.');
    clearPendingRejectObservation();
    return; // Don't process further
  }
  
  // Only process if this is an observation screen
  if (!isObservationScreen(screenTag)) {
    console.log('[Web_HSE Debug] Not an observation screen, skipping');
    return;
  }
  
  // Handle buttons: View Reject Reason, Reject, Confirm, Cancel, Entry Complete
  // Button names follow pattern:
  // - *_VWRJCTRSNS (View Reject Reasons on tracing tab)
  // - NRSTMISCENT_RJC (Reject)
  // - NRSTMISCENT_ACP (Confirm/Accept)
  // - NRSTMISCENT_CNCL (Cancel)
  // - NRSTMISCENT_ENTCMPLT (Entry Complete)
  if (normalizedButton.endsWith('_VWRJCTRSNS')) {
    // RQ_HSM_22_12_09_57: View Reject Reason (Observation Entry)
    // RQ_HSM_22_12_11_10: Handle View Reject Reason Custom Button
    // ClickUp Task: https://app.clickup.com/t/86c76vt58
    handleViewRejectReasonButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton.includes('_RJC') || normalizedButton.endsWith('_RJC')) {
    // RQ_HSM_22_12_10_50: Handle Reject button (button name contains or ends with _RJC)
    // ClickUp Task: https://app.clickup.com/t/86c76v8yr
    handleRejectButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton === 'NRSTMISCENT_ENTCMPLT' || normalizedButton.endsWith('_ENTCMPLT')) {
    // Handle Entry Complete button
    handleEntryCompleteButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton === 'NRSTMISCENT_ACP' || normalizedButton.endsWith('_ACP')) {
    // RQ_HSM_22_12_10_48: Handle Confirm/Accept button
    // ClickUp Task: https://app.clickup.com/t/86c76v5ze
    handleConfirmButton(buttonName, screenTag, eventObj, devInterface);
  } else if (normalizedButton === 'NRSTMISCENT_CNCL' || normalizedButton.endsWith('_CNCL')) {
    // RQ_HSM_22_12_10_55: Handle Cancel Custom Button
    // ClickUp Task: https://app.clickup.com/t/86c76v9yr
    handleCancelButton(buttonName, screenTag, eventObj, devInterface);
  } else {
    // Other buttons are not handled
    console.log('[Web_HSE Debug] Button not handled. Button name:', normalizedButton);
  }
}
