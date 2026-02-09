import { handleAuditPlanInquiryFilterButton } from './AuditPlanInquiryButtonHandlers';

/**
 * AuditPlanInquiryService.js
 *
 * Main entry for Audit Plan Inquiry main screen custom buttons.
 * TableName: HSE_TgAdtPlnInq, TrueTableName: HSE_AdtPlnEnt (CARCategory.cpp 188-190)
 * C++: AuditInquiry::DisplayCustomButtonClicked (AuditInquiry.cpp)
 */

const AUDIT_PLAN_INQUIRY_TAGS = ['HSE_TGADTPLNINQ', 'HSE_TgAdtPlnInq'];

const FILTER_BUTTONS = [
  'ADTPLNENT_VWALL',
  'ADTPLNENT_VWINCMPLT',
  'ADTPLNENT_VWRJCT',
  'ADTPLNENT_VWCMPLT',
  'ADTPLNENT_VWAPPRV',
  'ADTPLNENT_VWCLSD',
];

/**
 * Check if screen tag is Audit Plan Inquiry main screen
 * @param {string} screenTag - Screen tag to check
 * @returns {boolean}
 */
export function isAuditPlanInquiryScreen(screenTag) {
  if (!screenTag) return false;
  const normalized = screenTag.toString().toUpperCase();
  return AUDIT_PLAN_INQUIRY_TAGS.some(tag => normalized === tag.toUpperCase()) || normalized.includes('ADTPLNINQ');
}

/**
 * Route custom button clicks for Audit Plan Inquiry screen
 * @param {string} buttonName - Normalized button name (uppercase)
 * @param {string} screenTag - Current screen tag
 * @param {Object} eventObj - Event object
 * @param {Object} devInterface - devInterface functions
 */
export function sendButtonClickToBackend(buttonName, screenTag, eventObj = {}, devInterface = {}) {
  const normalizedButton = (buttonName || '').toString().toUpperCase();
  const normalizedScreenTag = (screenTag || '').toString().toUpperCase();

  if (!isAuditPlanInquiryScreen(normalizedScreenTag)) {
    return;
  }

  if (FILTER_BUTTONS.includes(normalizedButton)) {
    console.log('[Web_HSE] Audit Plan Inquiry button handled:', normalizedButton);
    handleAuditPlanInquiryFilterButton(normalizedButton, screenTag, eventObj, devInterface);
  }
  // Other buttons (AUDITORS, AUDITIES, ADTPLNENTACTVT_ADTCHKLSTFDBCK) can be wired here when needed
}
