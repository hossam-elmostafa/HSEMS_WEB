/**
 * HSE Application Constants
 */
export const CURRENT_APP_CODE = 66;

// Backend API URL - can be overridden via environment variable
export const WEB_HSE_API_URL = process.env.REACT_APP_WEB_HSE_API || 'http://localhost:7070';

// Observation screen tags - screens that belong to the Observation module
export const OBSERVATION_SCREEN_TAGS = [
   'HSE_TGNrMisTypes',        // Observation Type
  'HSE_TgObsrvtnTrgts',      // Observation Targets
  'HSE_TGNRSTMISCENT',       // Observation Entry (Near Miss Entry)
  'HSE_TgNrstMiscEnt',        // Observation Entry (alternative tag format)
  'HSE_TgNrstMisccnfrmtn',   // Observation Review
   'HSE_TgNrstMiscflwup',     // Observation Approval
   'HSE_TgNrMisRward',        // Observation Reward
   'HSE_TgNrstMiscInq',       // Observation Inquiry
];

// CAR screen tags - screens that belong to the CAR module
export const CAR_SCREEN_TAGS = [
  // Old CAR module screens
  'HSE_TGCRCTVEACCENT',      // CAR Entry
  'HSE_TGCRCTVEACCCNFRMTN',  // CAR Review/Confirmation
  'HSE_TGCRCTVEACCFLWUP',    // CAR Follow-up
  'HSE_TGCRCTVEACCINQ',      // CAR Inquiry
  'HSE_TGCARFLWUPVSTS',      // CAR Follow-up Visits
  'HSE_TGCRCTVEACCRCVD',     // CAR Received
  'HSE_TGCRCTVEACCUNDREXCTN', // CAR Under Execution
  'HSE_TGCRCTVEACCJOBVRFCTN', // CAR Job Verification
  
  // New CAR module screens (Actions-based)
  'HSE_TGACTNSENTRY',        // Actions Entry
  'HSE_TGACTNSRVIW',         // Actions Review
  'HSE_TGCRRVW',             // CAR Review
  'HSE_TGCRAPRVL',           // CAR Approval
  'HSE_TGCRFLOUP',           // CAR Follow-up
  'HSE_TGCREDTNG',           // CAR Editing
  'HSE_TGCRENTRY',           // CAR Entry (new)
  
  // New CAR module screens (from menu JSON - exact TagNames)
  'HSE_TgCrEntry',           // CAR Entry
  'HSE_TgCrRvw',             // CAR Review
  'HSE_TgCrAprvl',           // CAR Approval
  'HSE_TgActnsEntry',        // Actions Entry
  'HSE_TgActnsRviw',         // Actions Review
  'HSE_TgActnsRcvd',         // Actions Received
  'HSE_TgActnsUndrExec',     // Actions Under Execution
  'HSE_TgCrFloUp',           // CAR Follow Up
  'HSE_TgCrEdtng',           // CAR Editing
  
  // CAR Model Basis
  'HSE_TGCARMODELBAS',       // CAR Model Basis
];

// Chemical Register screen tags - screens that belong to the Chemical Register module
export const CHEMICAL_REGISTER_SCREEN_TAGS = [
  'HSE_CHMCLRGSTR',          // Chemical Register (table name - uppercase)
  'HSE_TGCHMCLRGSTR',        // Chemical Register (screen tag - uppercase)
  'HSE_ChmclRgstr',          // Chemical Register (alternative tag format from menu - mixed case)
  'HSE_ChmclRgstr'.toUpperCase(), // Also include uppercase version
];

// Messages object for internationalization (placeholder)
export const messages = {
  // Add message keys here as needed
  // Example: 
  // "SUBMIT_SUCCESS": { en: "Submitted successfully", ar: "تم التقديم بنجاح" }
};

