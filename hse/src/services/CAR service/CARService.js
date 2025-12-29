import { CAR_SCREEN_TAGS } from '../../config/constants';

// Import button handlers from CARButtonHandlers
import {
  handleRejectCARButton,
  handleRejectReasonOkButtonForCAR,
  handleViewRejectReasonButtonForCAR,
  handleViewRejectReasonButton,
  handleCARReviewInfoButton,
  handleAcceptCARButton,
  handleViewSourceTXNButton,
  setPendingRejectCAR,
  clearPendingRejectCAR,
  hasPendingRejectCAR,
} from './CARButtonHandlers';

// Re-export for backward compatibility
export {
  setPendingRejectCAR,
  clearPendingRejectCAR,
};

/**
 * CARService.js
 * 
 * Main entry point for CAR module functionality.
 * Routes button clicks to appropriate handlers and provides utility functions.
 * 
 * This file has been refactored to delegate to:
 * - CARButtonHandlers.js: All button click handlers
 */

/**
 * Check if a screen tag belongs to the CAR module
 * @param {string} screenTag - The screen tag to check
 * @returns {boolean} - True if the screen is a CAR screen
 */
export function isCARScreen(screenTag) {
  if (!screenTag) return false;
  
  const normalizedTag = screenTag.toString().toUpperCase();
  
  // First check exact matches in the CAR_SCREEN_TAGS array
  if (CAR_SCREEN_TAGS.some(tag => normalizedTag === tag.toUpperCase())) {
    return true;
  }
  
  // Also check if the tag contains CAR-related keywords (more flexible matching)
  // This helps catch variations and new screens that might not be in the list
  // Be specific to avoid false positives:
  // - CRCTVE (Corrective Action)
  // - CAR (but not as part of other words)
  // - ACTN (Actions, but be careful - only if it's clearly CAR-related)
  if (normalizedTag.includes('CRCTVE')) {
    return true;
  }
  
  // Check for CAR as a standalone word or as part of CAR-specific patterns
  if (normalizedTag.includes('CAR') && (
    normalizedTag.includes('CARMODEL') ||
    normalizedTag.includes('CARFLWUP') ||
    normalizedTag.startsWith('HSE_TGCAR') ||
    normalizedTag.includes('CRRVW') ||
    normalizedTag.includes('CRAPRVL') ||
    normalizedTag.includes('CRFLOUP') ||
    normalizedTag.includes('CREDTNG') ||
    normalizedTag.includes('CRENTRY')
  )) {
    return true;
  }
  
  // Check for ACTN (Actions) screens that are CAR-related
  // Only match if it's clearly a CAR Actions screen
  if (normalizedTag.includes('ACTN') && (
    normalizedTag.includes('ACTNSENTRY') ||
    normalizedTag.includes('ACTNSRVIW') ||
    normalizedTag.includes('ACTNSRCVD') ||
    normalizedTag.includes('ACTNSUNDREXEC')
  )) {
    return true;
  }
  
  return false;
}

/**
 * Send button click event to Web_HSE backend
 * Handles Reject CAR button on CAR module screens
 * 
 * This is the main routing function that delegates to specific button handlers.
 * 
 * @param {string} buttonName - The button name/ID to log
 * @param {string} screenTag - The screen tag to check if it's a CAR screen
 * @param {Object} eventObj - The full event object containing fullRecord, strScrTag, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 */
export function sendButtonClickToBackend(buttonName, screenTag, eventObj = {}, devInterface = {}) {
  // Debug logging to see what's being passed
  console.log('[Web_HSE Debug] CAR Button clicked:', {
    buttonName,
    screenTag,
    isCARScreen: isCARScreen(screenTag)
  });
  
  // Normalize button name for comparison (buttonName is already normalized to uppercase in buttonEvents.js)
  const normalizedButton = buttonName ? buttonName.toString().toUpperCase() : '';
  const normalizedScreenTag = screenTag ? screenTag.toString().toUpperCase() : '';
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_OK
  // Handle reject reason screen OK button (this is part of CAR module flow)
  // Check both possible screen tag casings: HSE_TGRJCTRSN (uppercase) or HSE_TGRjctRsn (mixed case from config)
  if ((normalizedScreenTag === 'HSE_TGRJCTRSN' || normalizedScreenTag === 'HSE_TGRJCTRSN') && normalizedButton === 'RJCTRSN_BTN_OK') {
    // Check if this is for CAR (pendingRejectCAR exists) vs Observation (pendingRejectObservation exists)
    // We need to check which one is pending - this is a simple check
    if (hasPendingRejectCAR()) {
      console.log('[Web_HSE] ✓ Reject reason screen OK button (RJCTRSN_BTN_OK) clicked for CAR!');
      handleRejectReasonOkButtonForCAR(devInterface);
      return; // Don't process further
    }
    // If pendingRejectCAR doesn't exist, let Observation service handle it (if pendingRejectObservation exists)
    // This will be handled by ObservationService in buttonEvents.js
  }
  
  // C++: CRejectReason::DisplayCustomButtonClicked - handles RJCTRSN_BTN_CANCEL
  // Check both possible screen tag casings: HSE_TGRJCTRSN (uppercase) or HSE_TGRjctRsn (mixed case)
  if ((normalizedScreenTag === 'HSE_TGRJCTRSN' || normalizedScreenTag === 'HSE_TGRJCTRSN') && normalizedButton === 'RJCTRSN_BTN_CANCEL') {
    // Check if this is for CAR
    if (hasPendingRejectCAR()) {
      console.log('[Web_HSE] Reject reason screen Cancel button clicked for CAR. Clearing pending rejection.');
      clearPendingRejectCAR();
      return; // Don't process further
    }
    // If pendingRejectCAR doesn't exist, let Observation service handle it
  }
  
  // Only process if this is a CAR screen
  if (!isCARScreen(screenTag)) {
    console.log('[Web_HSE Debug] Not a CAR screen, skipping');
    return;
  }
  
  // Handle buttons: Reject CAR
  // Button names follow pattern:
  // - CRCTVEACCENT_RJC (Reject CAR - Entry screen)
  // - CRCTVEACCENT_CNFRMTN_RJC (Reject CAR - Confirmation/Review screen)
  // - REJECT_CAR (Reject CAR - new CAR module)
  if (normalizedButton === 'CRCTVEACCENT_RJC' || 
      normalizedButton === 'CRCTVEACCENT_CNFRMTN_RJC' || 
      normalizedButton === 'REJECT_CAR' ||
      normalizedButton.includes('_RJC')) {
    // RQ_AG_25_12_25_15_19.01.01: Reject CAR
    handleRejectCARButton(buttonName, screenTag, eventObj, devInterface);
  }
  // Handle View Reject Reasons button in Tracing tab
  // Button: CrEntry_Trc_VWRjctRsns or CRENTRY_VWRJCTRSNS
  else if (normalizedButton === 'CRENTRY_VWRJCTRSNS' || 
           normalizedButton === 'CRENTRY_TRC_VWRJCTRSNS' ||
           normalizedButton.includes('VWRJCTRSNS')) {
    handleViewRejectReasonButtonForCAR(buttonName, screenTag, eventObj, devInterface);
  }
  // Handle CAR Review Info button
  // RQ_AG_25_12_25_15_19.01.02: CAR Review info
  // Button IDs: CAR_Review_Info, CAR_REVIEW_INFO, View_CAR_Review_Info, VIEW_CAR_REVIEW_INFO
  else if (normalizedButton === 'CAR_REVIEW_INFO' || 
           normalizedButton === 'CAR_REVIEWINFO' ||
           normalizedButton === 'VIEW_CAR_REVIEW_INFO' ||
           normalizedButton.includes('CAR_REVIEW_INFO') ||
           normalizedButton.includes('CAR_REVIEWINFO')) {
    console.log('[Web_HSE] ✓ CAR Review Info button matched:', normalizedButton);
    handleCARReviewInfoButton(buttonName, screenTag, eventObj, devInterface);
  }
  // Handle Accept CAR button
  // RQ_AG_25_12_25_15_19.01.03: Accept CAR
  else if (normalizedButton === 'ACCEPT_CAR' || 
           normalizedButton === 'ACCEPTCAR') {
    console.log('[Web_HSE] ✓ Accept CAR button matched:', normalizedButton);
    handleAcceptCARButton(buttonName, screenTag, eventObj, devInterface);
  }
  // Handle View Source TXN button
  // RQ_AG_25_12_25_15_19.01.04: View Source TXN
  else if (normalizedButton === 'VIEW_SOURCE_TXN' || 
           normalizedButton === 'VIEWSOURCETXN' ||
           normalizedButton.includes('VIEW_SOURCE_TXN')) {
    console.log('[Web_HSE] ✓ View Source TXN button matched:', normalizedButton);
    handleViewSourceTXNButton(buttonName, screenTag, eventObj, devInterface);
  }
  // Handle View Reject Reason button
  // RQ_AG_28_12_25_15_23.01: View Reject Reason
  else if (normalizedButton === 'VIEW_REJECT_REASON' || 
           normalizedButton === 'VIEWREJECTREASON' ||
           normalizedButton === 'VIEW_REJECTREASON' ||
           normalizedButton.includes('VIEW_REJECT_REASON')) {
    console.log('[Web_HSE] ✓ View Reject Reason button matched:', normalizedButton);
    handleViewRejectReasonButton(buttonName, screenTag, eventObj, devInterface);
  } else {
    // Other buttons are not handled
    console.log('[Web_HSE Debug] CAR Button not handled. Button name:', normalizedButton);
  }
}

