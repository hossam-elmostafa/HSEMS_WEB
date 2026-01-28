import {
  handleEntryCompletedButton,
  handleCloseSiteSurveyButton,
  handleConfirmSiteSurveyButton,
  handleRejectSiteSurveyButton,
  handleRejectReasonOkButtonForSiteSurvey,
  hasPendingRejectSiteSurvey,
  handleRequiredActionsButton,
} from './SiteSurveyButtonHandlers';

/**
 * SiteSurveyService.js
 * 
 * Main service file for Site Survey module
 * Handles button clicks and integrates with the HSE framework
 * 
 * Button Mappings (based on C++ code):
 * - SITSRVYENT_EntCmplt / SITSRVYENT_ENTCMPLT: Entry Completed button (SitSrvyEntCategory.cpp)
 *   - When clicked, completes the site survey entry by executing completeSitSrvyTXN stored procedure
 * - SITSRVYENT_CLS: Close button (SitSrvyFlwupCategory.cpp)
 *   - When clicked, closes the site survey by executing closeSiteSurveyTXN stored procedure
 * - SITSRVYENT_CNFRM: Confirm button (SitSrvyCnfrmtnCategory.cpp)
 *   - When clicked, confirms the site survey by executing confirmSiteSrvy stored procedure
 * - SITSRVYENT_RJCT: Reject button (SitSrvyCategory.cpp)
 *   - When clicked, opens reject reason screen, then executes rejectSitSrvy stored procedure
 * - RJCTRSN_BTN_OK: OK button in Reject Reason screen
 *   - When clicked, processes the Site Survey rejection if there's a pending rejection
 */

/**
 * Handle button clicks for Site Survey module
 * @param {string} buttonName - The button name/ID
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object from the framework
 * @param {Object} devInterface - Object containing devInterface functions
 * @returns {Promise<boolean>} - Returns true if button was handled, false otherwise
 */
export async function handleSiteSurveyButtonClick(buttonName, screenTag, eventObj, devInterface) {
  console.log('[Web_HSE] ===== Site Survey Button Click Handler =====');
  console.log('[Web_HSE] Button Name:', buttonName);
  console.log('[Web_HSE] Screen Tag:', screenTag);
  console.log('[Web_HSE] Event Object:', eventObj);

  // Normalize button name to uppercase for comparison
  const normalizedButtonName = buttonName?.toUpperCase() || '';

  try {
    // Entry Completed button (Entry screen)
    // C++ Reference: SitSrvyEntCategory.cpp line 26: SITSRVYENT_ENTCMPLT
    // Button IDs: SITSRVYENT_ENTCMPLT (uppercase in C++) or SITSRVYENT_EntCmplt (mixed case from JSON)
    // The normalization converts to uppercase, so both will match 'SITSRVYENT_ENTCMPLT'
    if (normalizedButtonName === 'SITSRVYENT_ENTCMPLT' || normalizedButtonName === 'ENTRY_COMPLETED') {
      await handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Close button (Follow-up screen)
    // C++ Reference: SitSrvyFlwupCategory.cpp line 34: SITSRVYENT_CLS
    if (normalizedButtonName === 'SITSRVYENT_CLS' || normalizedButtonName === 'CLOSE_SITE_SURVEY') {
      await handleCloseSiteSurveyButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Confirm button (Confirmation screen)
    // C++ Reference: SitSrvyCnfrmtnCategory.cpp line 26: SITSRVYENT_CNFRM
    if (normalizedButtonName === 'SITSRVYENT_CNFRM' || normalizedButtonName === 'CONFIRM_SITE_SURVEY') {
      await handleConfirmSiteSurveyButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Reject button (Confirmation and Follow-up screens)
    // C++ Reference: SitSrvyCategory.cpp line 134: SITSRVYENT_RJCT
    if (normalizedButtonName === 'SITSRVYENT_RJCT' || normalizedButtonName === 'REJECT_SITE_SURVEY') {
      await handleRejectSiteSurveyButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Reject Reason OK button
    // This button is clicked in the reject reason screen after user enters reject reasons
    if (normalizedButtonName === 'RJCTRSN_BTN_OK') {
      // Check if we have a pending Site Survey rejection
      if (hasPendingRejectSiteSurvey()) {
        console.log('[Web_HSE] Detected RJCTRSN_BTN_OK click with pending Site Survey rejection');
        await handleRejectReasonOkButtonForSiteSurvey(devInterface);
        return true;
      }
    }

    // Required Actions button (Findings tab)
    // C++ Reference: SitSrvyCategory.cpp line 148: SITSRVYRQRDACTN_RQRDACTNBTN
    // RQ_HSM_HSE_05_01_26_10_20.3.1: Required Actions
    if (normalizedButtonName === 'SITSRVYRQRDACTN_RQRDACTNBTN' || normalizedButtonName === 'REQUIRED_ACTIONS') {
      await handleRequiredActionsButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Button not handled by this service
    console.log('[Web_HSE] Button not handled by Site Survey service:', buttonName);
    return false;
  } catch (error) {
    console.error('[Web_HSE] Error in handleSiteSurveyButtonClick:', error);
    return false;
  }
}

// Flag to track if service has been initialized
let isInitialized = false;

/**
 * Initialize Site Survey service
 * This function can be called when the application starts
 * It will only log initialization message once to avoid console spam
 */
export function initializeSiteSurveyService() {
  // Only initialize once
  if (isInitialized) {
    return;
  }
  
  isInitialized = true;
  // Only log a simple message to reduce console spam
  console.log('[Web_HSE] Site Survey Service initialized - 5 buttons supported (Entry Completed, Close, Confirm, Reject, Required Actions)');
}

export default {
  handleSiteSurveyButtonClick,
  initializeSiteSurveyService,
};

