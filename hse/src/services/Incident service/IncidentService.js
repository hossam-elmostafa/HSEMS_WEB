import {
  handleEntryCompletedButton,
  handleRejectIncidentButton,
  handleRejectReasonOkButtonForIncident,
  handleConfirmIncidentButton,
  handleAcceptIncidentButton,
  handleCloseIncidentButton,
  handleViewRejectReasonButtonForIncident,
  handleFlashReportEntryCompletedButton,
  handleFlashReportRejectButton,
  handleFlashReportAcceptButton,
  handleCloseIncidentMedicalReportButton,
  hasPendingRejectIncident,
} from './IncidentButtonHandlers';

/**
 * IncidentService.js
 * 
 * Main service file for Incident (Near Miss & Accident) module
 * Handles button clicks and integrates with the HSE framework
 * 
 * Requirements:
 * - RQ_HSM_HSE_18_12_25_10_19: Implement Incident (Near Miss & Accident)
 * - RQ_HSM_30_12_25_07_50: Implement Entry Completed
 * - RQ_HSM_30_12_25_08_10: Reject
 * - RQ_HSM_30_12_25_08_10_1: Accept
 * 
 * Button Mappings (based on C++ code):
 * - ACDNTENT_ENTCMPLT / ACDNTENT_EntCmplt: Entry Completed button (AccidentEntryCategory.cpp)
 *   - RQ_HSM_30_12_25_07_50: When clicked, validates recommended actions and completes the incident entry
 * - ACDNTENT_RJC: Reject button (AccidentCategory.cpp)
 *   - RQ_HSM_30_12_25_08_10: Opens reject reason screen, then executes rejectIncident stored procedure
 * - ACDNTENT_CNFRM: Confirm button (AccidentConfirmationCategory.cpp)
 * - ACDNTENT_ACCEPT: Accept button (custom)
 *   - RQ_HSM_30_12_25_08_10_1: Executes acceptIncidentTXN stored procedure to accept the incident
 * - ACDNTENT_CLOSE: Close button (custom)
 * - AcdntEnt_Trc_VWRjctRsns: View Reject Reasons button in Tracing tab
 * - RJCTRSN_BTN_OK: OK button in Reject Reason screen
 */

/**
 * Handle button clicks for Incident module
 * @param {string} buttonName - The button name/ID
 * @param {string} screenTag - The screen tag
 * @param {Object} eventObj - The full event object from the framework
 * @param {Object} devInterface - Object containing devInterface functions
 * @returns {Promise<boolean>} - Returns true if button was handled, false otherwise
 */
export async function handleIncidentButtonClick(buttonName, screenTag, eventObj, devInterface) {
  console.log('[Web_HSE] ===== Incident Button Click Handler =====');
  console.log('[Web_HSE] Button Name:', buttonName);
  console.log('[Web_HSE] Screen Tag:', screenTag);
  console.log('[Web_HSE] Event Object:', eventObj);

  // Normalize button name to uppercase for comparison
  const normalizedButtonName = buttonName?.toUpperCase() || '';

  try {
    // Entry Completed button (Entry screen)
    // RQ_HSM_30_12_25_07_50: Implement Entry Completed
    // Button IDs: ACDNTENT_ENTCMPLT (uppercase) or ACDNTENT_EntCmplt (mixed case from JSON)
    // The normalization converts to uppercase, so both will match 'ACDNTENT_ENTCMPLT'
    if (normalizedButtonName === 'ACDNTENT_ENTCMPLT' || normalizedButtonName === 'ENTRY_COMPLETED') {
      await handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Entry Completed button (Investigation Entry screen)
    // C++ Reference: IncdntInvstgtnEntryCategory.cpp line 38: ACDNTENT_ENTRYCMPLTD
    if (normalizedButtonName === 'ACDNTENT_ENTRYCMPLTD' || normalizedButtonName === 'ENTRY_COMPLETED_INV') {
      // Uses same handler as regular Entry Completed - calls completeAccidentTXN
      await handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Reject button
    // RQ_HSM_30_12_25_08_10: Reject
    if (normalizedButtonName === 'ACDNTENT_RJC' || normalizedButtonName === 'REJECT_INCIDENT') {
      await handleRejectIncidentButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Confirm button
    if (normalizedButtonName === 'ACDNTENT_CNFRM' || normalizedButtonName === 'CONFIRM_INCIDENT') {
      await handleConfirmIncidentButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Accept button
    // RQ_HSM_30_12_25_08_10_1: Accept
    if (normalizedButtonName === 'ACDNTENT_ACCEPT' || normalizedButtonName === 'ACCEPT_INCIDENT') {
      await handleAcceptIncidentButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Close button
    if (normalizedButtonName === 'ACDNTENT_CLOSE' || normalizedButtonName === 'CLOSE_INCIDENT') {
      await handleCloseIncidentButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Close button (Investigation Approval screen)
    // C++ Reference: IncdntInvstgtnAprvCategory.cpp line 23: ACDNTENT_CLS
    // Uses closeAccidentTXN stored procedure with different parameter order
    if (normalizedButtonName === 'ACDNTENT_CLS') {
      await handleCloseIncidentButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // View Reject Reasons button (from Tracing tab)
    if (normalizedButtonName === 'ACDNTENT_TRC_VWRJCTRSNS' || 
        normalizedButtonName === 'ACDNTENT_VWRJCTRSNS' ||
        normalizedButtonName === 'VIEW_REJECT_REASONS') {
      await handleViewRejectReasonButtonForIncident(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Reject Reason OK button
    // This button is clicked in the reject reason screen after user enters reject reasons
    if (normalizedButtonName === 'RJCTRSN_BTN_OK') {
      // Check if we have a pending Incident rejection
      if (hasPendingRejectIncident()) {
        console.log('[Web_HSE] Detected RJCTRSN_BTN_OK click with pending Incident rejection');
        await handleRejectReasonOkButtonForIncident(devInterface);
        return true;
      }
    }

    // Flash Report Entry Completed button
    // C++ Reference: IncdntFlshRprtCategory.cpp line 151: NCDNTFLSHRPRT_ENTRYCMPLT
    if (normalizedButtonName === 'NCDNTFLSHRPRT_ENTRYCMPLT' || normalizedButtonName === 'FLSHRPRT_ENTRYCMPLT') {
      await handleFlashReportEntryCompletedButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Flash Report Reject button (from Review screen)
    // C++ Reference: IncdntFlshRprtCategory.cpp line 153: INCDNTFLSHRPRTRVEW_RJCT
    if (normalizedButtonName === 'INCDNTFLSHRPRTRVEW_RJCT' || normalizedButtonName === 'FLSHRPRTRVEW_RJCT') {
      await handleFlashReportRejectButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Flash Report Accept button (from Review screen)
    // C++ Reference: IncdntFlshRprtCategory.cpp line 155: INCDNTFLSHRPRTRVEW_ACCPT
    if (normalizedButtonName === 'INCDNTFLSHRPRTRVEW_ACCPT' || normalizedButtonName === 'FLSHRPRTRVEW_ACCPT') {
      await handleFlashReportAcceptButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Incident Medical Report Close button
    // C++ Reference: HSEMSCommonCategory.cpp line 650-652: INCDNTMDCLRPRT_CLS
    if (normalizedButtonName === 'INCDNTMDCLRPRT_CLS' || normalizedButtonName === 'CLOSE_INCDNTMDCLRPRT') {
      await handleCloseIncidentMedicalReportButton(buttonName, screenTag, eventObj, devInterface);
      return true;
    }

    // Button not handled by this service
    console.log('[Web_HSE] Button not handled by Incident service:', buttonName);
    return false;
  } catch (error) {
    console.error('[Web_HSE] Error in handleIncidentButtonClick:', error);
    return false;
  }
}

// Flag to track if service has been initialized
let isInitialized = false;

/**
 * Initialize Incident service
 * This function can be called when the application starts
 * It will only log initialization message once to avoid console spam
 */
export function initializeIncidentService() {
  // Only initialize once
  if (isInitialized) {
    return;
  }
  
  isInitialized = true;
  // Only log a simple message to reduce console spam
  // Full button list available in code/documentation
  console.log('[Web_HSE] Incident Service initialized - 14 buttons supported');
}

export default {
  handleIncidentButtonClick,
  initializeIncidentService,
};

