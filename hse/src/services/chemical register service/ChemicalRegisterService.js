import { CHEMICAL_REGISTER_SCREEN_TAGS } from '../../config/constants';

// Import button handlers from ChemicalRegisterButtonHandlers
import {
  handleViewAllActiveItemsButton,
  handleViewAllInActiveItemsButton,
} from './ChemicalRegisterButtonHandlers';

/**
 * ChemicalRegisterService.js
 * 
 * Main entry point for Chemical Register module functionality.
 * Routes button clicks to appropriate handlers and provides utility functions.
 * 
 * This file has been refactored to delegate to:
 * - ChemicalRegisterButtonHandlers.js: All button click handlers
 */

/**
 * Check if a screen tag belongs to the Chemical Register module
 * @param {string} screenTag - The screen tag to check
 * @returns {boolean} - True if the screen is a Chemical Register screen
 */
export function isChemicalRegisterScreen(screenTag) {
  if (!screenTag) {
    console.log('[Web_HSE Debug] isChemicalRegisterScreen: screenTag is empty');
    return false;
  }
  
  const normalizedTag = screenTag.toString().toUpperCase();
  const originalTag = screenTag.toString();
  console.log('[Web_HSE Debug] isChemicalRegisterScreen: checking tag:', originalTag, 'normalized:', normalizedTag);
  
  // First check exact matches in the CHEMICAL_REGISTER_SCREEN_TAGS array (case-insensitive)
  const matches = CHEMICAL_REGISTER_SCREEN_TAGS.some(tag => 
    normalizedTag === tag.toUpperCase() || 
    originalTag === tag ||
    normalizedTag.includes(tag.toUpperCase()) ||
    originalTag.includes(tag)
  );
  
  if (matches) {
    console.log('[Web_HSE Debug] isChemicalRegisterScreen: matched in CHEMICAL_REGISTER_SCREEN_TAGS');
    return true;
  }
  
  // Also check if the tag contains Chemical Register-related keywords (case-insensitive)
  // Check both normalized and original case
  const hasKeyword = 
    normalizedTag.includes('CHMCLRGSTR') || 
    originalTag.includes('ChmclRgstr') ||
    originalTag.includes('CHMCLRGSTR') ||
    normalizedTag.startsWith('HSE_TGCHMCLRGSTR') ||
    normalizedTag.startsWith('HSE_CHMCLRGSTR') ||
    normalizedTag === 'HSE_CHMCLRGSTR' ||
    originalTag === 'HSE_ChmclRgstr' ||
    originalTag === 'HSE_CHMCLRGSTR';
  
  if (hasKeyword) {
    console.log('[Web_HSE Debug] isChemicalRegisterScreen: matched by keyword');
    return true;
  }
  
  console.log('[Web_HSE Debug] isChemicalRegisterScreen: no match');
  return false;
}

/**
 * Send button click event to Web_HSE backend
 * Handles Chemical Register button clicks on Chemical Register module screens
 * 
 * This is the main routing function that delegates to specific button handlers.
 * 
 * @param {string} buttonName - The button name/ID to log
 * @param {string} screenTag - The screen tag to check if it's a Chemical Register screen
 * @param {Object} eventObj - The full event object containing fullRecord, strScrTag, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 */
export function sendButtonClickToBackend(buttonName, screenTag, eventObj = {}, devInterface = {}) {
  // Debug logging to see what's being passed
  const normalizedButton = buttonName ? buttonName.toString().toUpperCase() : '';
  const originalScreenTag = screenTag ? screenTag.toString() : '';
  const normalizedScreenTag = originalScreenTag.toUpperCase();
  
  console.log('[Web_HSE] ===== Chemical Register Button Click Handler =====');
  console.log('[Web_HSE] Button Name:', normalizedButton);
  console.log('[Web_HSE] Screen Tag (original):', originalScreenTag);
  console.log('[Web_HSE] Screen Tag (normalized):', normalizedScreenTag);
  console.log('[Web_HSE] Event Object:', eventObj);
  
  // Handle buttons: View All Active Items
  // Button ID: VIEW_ALL_ACTIVE_ITEMS
  // RQ_HSM_27_01_26_22_57.03: View All Active Items
  // This button should work on all screens, not just Chemical Register screens
  if (normalizedButton === 'VIEW_ALL_ACTIVE_ITEMS') {
    console.log('[Web_HSE] ✓ View All Active Items button matched:', normalizedButton);
    // Pass the screen tag from eventObj if available, or use the provided screenTag
    // The handler will determine the correct Chemical Register screen tag to use
    const actualScreenTag = eventObj?.strScrTag || screenTag || 'HSE_ChmclRgstr';
    handleViewAllActiveItemsButton(buttonName, actualScreenTag, eventObj, devInterface);
  }
  // Handle buttons: View All In-Active Items
  // Button ID: VIEW_ALL_IN-ACTIVE_ITEMS (note the hyphen)
  // RQ_HSM_27_01_26_22_57.04: View All In-Active Items
  // This button should work on all screens, not just Chemical Register screens
  else if (normalizedButton === 'VIEW_ALL_IN-ACTIVE_ITEMS' || normalizedButton === 'VIEW_ALL_INACTIVE_ITEMS') {
    console.log('[Web_HSE] ✓ View All In-Active Items button matched:', normalizedButton);
    // Pass the screen tag from eventObj if available, or use the provided screenTag
    // The handler will determine the correct Chemical Register screen tag to use
    const actualScreenTag = eventObj?.strScrTag || screenTag || 'HSE_ChmclRgstr';
    handleViewAllInActiveItemsButton(buttonName, actualScreenTag, eventObj, devInterface);
  } else {
    // Other buttons are not handled
    console.log('[Web_HSE] Chemical Register Button not handled. Button name:', normalizedButton);
  }
}
