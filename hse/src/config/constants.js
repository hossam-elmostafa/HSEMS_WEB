/**
 * HSE Application Constants
 */
export const CURRENT_APP_CODE = 66;

// Backend API URL - can be overridden via environment variable
export const WEB_HSE_API_URL = process.env.REACT_APP_WEB_HSE_API || 'http://localhost:7070';

// Observation screen tags - screens that belong to the Observation module
export const OBSERVATION_SCREEN_TAGS = [
//   'HSE_TGNrMisTypes',        // Observation Type
//   'HSE_TgObsrvtnTrgts',      // Observation Targets
//   'HSE_TgNrstMiscEnt',       // Observation Entry
  'HSE_TgNrstMisccnfrmtn',   // Observation Review
//   'HSE_TgNrstMiscflwup',     // Observation Approval
//   'HSE_TgNrMisRward',        // Observation Reward
//   'HSE_TgNrstMiscInq',       // Observation Inquiry
];

// Messages object for internationalization (placeholder)
export const messages = {
  // Add message keys here as needed
  // Example: 
  // "SUBMIT_SUCCESS": { en: "Submitted successfully", ar: "تم التقديم بنجاح" }
};

