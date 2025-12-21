import { toast } from "react-toastify";
import { WEB_HSE_API_URL, OBSERVATION_SCREEN_TAGS } from '../config/constants';

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
 * Handles Reject, Confirm, and Cancel buttons on Observation module screens
 * Currently only Reject is actively handled; Confirm and Cancel handling is commented out
 * Fire-and-forget - does not block UI
 * @param {string} buttonName - The button name/ID to log
 * @param {string} screenTag - The screen tag to check if it's an observation screen
 */
export function sendButtonClickToBackend(buttonName, screenTag) {
  // Debug logging to see what's being passed
  console.log('[Web_HSE Debug] Button clicked:', {
    buttonName,
    screenTag,
    isObservationScreen: isObservationScreen(screenTag)
  });
  
  // Only process if this is an observation screen
  if (!isObservationScreen(screenTag)) {
    console.log('[Web_HSE Debug] Not an observation screen, skipping');
    return;
  }
  
  // Normalize button name for comparison (buttonName is already normalized to uppercase in buttonEvents.js)
  const normalizedButton = buttonName ? buttonName.toString().toUpperCase() : '';
  
  // Handle all three buttons: Reject, Confirm, Cancel
  // Button names follow pattern: NRSTMISCENT_RJC (Reject), NRSTMISCENT_ACP (Confirm/Accept), NRSTMISCENT_CNCL (Cancel)
  // Based on header.json: NrstMiscEnt_Rjc, NrstMiscEnt_Acp, NrstMiscEnt_Cncl
  // Currently only Reject is actively handled
  if (normalizedButton.includes('_RJC') || normalizedButton.endsWith('_RJC')) {
    // Handle Reject button (button name contains or ends with _RJC)
    handleRejectButton(buttonName);
  }
  // else if (normalizedButton.includes('_ACP') || normalizedButton.endsWith('_ACP')) {
  //   // Handle Confirm/Accept button - commented out (button name contains or ends with _ACP)
  //   handleConfirmButton(buttonName);
  // }
  // else if (normalizedButton.includes('_CNCL') || normalizedButton.endsWith('_CNCL')) {
  //   // Handle Cancel button - commented out (button name contains or ends with _CNCL)
  //   handleCancelButton(buttonName);
  // }
  else {
    // Other buttons are not handled
    console.log('[Web_HSE Debug] Button not handled. Button name:', normalizedButton);
  }
}

/**
 * Handle Reject button click
 * @param {string} buttonName - The button name
 */
function handleRejectButton(buttonName) {
  // Log to browser console immediately (backup in case backend fails)
  console.log(`ButtonClicked: "${buttonName}"`);
  
  // Show toast notification to user
  try {
    toast.info(`Button Clicked: ${buttonName}`, {
      position: "top-right",
      autoClose: 3000,
      hideProgressBar: false,
      closeOnClick: true,
      pauseOnHover: true,
      draggable: true,
    });
  } catch (error) {
    console.warn('[Web_HSE] Toast notification failed:', error.message);
  }
  
  // Also send to backend
  const url = `${WEB_HSE_API_URL}/api/observation/button-click`;
  
  fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ buttonName }),
  })
  .catch((error) => {
    // Silently log to console only - don't disrupt user experience
    console.warn('[Web_HSE] Failed to send to backend (but logged to console):', error.message);
  });
}

// Commented out - Confirm button handling
// function handleConfirmButton(buttonName) {
//   // Log to browser console immediately (backup in case backend fails)
//   console.log(`ButtonClicked: "${buttonName}"`);
//   
//   // Show toast notification to user
//   toast.info(`Button Clicked: ${buttonName}`, {
//     position: "top-right",
//     autoClose: 3000,
//     hideProgressBar: false,
//     closeOnClick: true,
//     pauseOnHover: true,
//     draggable: true,
//   });
//   
//   // Also send to backend
//   const url = `${WEB_HSE_API_URL}/api/observation/button-click`;
//   
//   fetch(url, {
//     method: 'POST',
//     headers: {
//       'Content-Type': 'application/json',
//     },
//     body: JSON.stringify({ buttonName }),
//   })
//   .catch((error) => {
//     console.warn('[Web_HSE] Failed to send to backend (but logged to console):', error.message);
//   });
// }

// Commented out - Cancel button handling
// function handleCancelButton(buttonName) {
//   // Log to browser console immediately (backup in case backend fails)
//   console.log(`ButtonClicked: "${buttonName}"`);
//   
//   // Show toast notification to user
//   toast.info(`Button Clicked: ${buttonName}`, {
//     position: "top-right",
//     autoClose: 3000,
//     hideProgressBar: false,
//     closeOnClick: true,
//     pauseOnHover: true,
//     draggable: true,
//   });
//   
//   // Also send to backend
//   const url = `${WEB_HSE_API_URL}/api/observation/button-click`;
//   
//   fetch(url, {
//     method: 'POST',
//     headers: {
//       'Content-Type': 'application/json',
//     },
//     body: JSON.stringify({ buttonName }),
//   })
//   .catch((error) => {
//     console.warn('[Web_HSE] Failed to send to backend (but logged to console):', error.message);
//   });
// }

