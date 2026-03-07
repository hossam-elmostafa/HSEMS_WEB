/**
 * Screen handlers registry
 * Maps screen tags to per-screen handler modules.
 * Add handler modules and register them in SCREEN_HANDLERS below.
 */

import * as HSE_TgNrstMiscEnt from './Safety/Observation/Observation_Entry.js';
import * as HSE_TgNrstMisccnfrmtn from './Safety/Observation/Observation_Review.js';
import * as HSE_TgNrstMiscflwup from './Safety/Observation/Observation_Approval.js';
import * as HSE_TgNrMisRward from './Safety/Observation/Observation_Reward.js';
import * as HSE_TGNrMisTypes from './Setup/Observations/Observation_Type.js';
import * as HSE_TgObsrvtnTrgts from './Setup/Observations/Observation_Targets.js';
import * as HSE_TgNrstMiscInq from './Inquiry/Observation_Inquiry.js';

const SCREEN_HANDLERS = {
  'HSE_TgNrstMiscEnt': HSE_TgNrstMiscEnt,
  'HSE_TGNrMisTypes': HSE_TGNrMisTypes,
  'HSE_TgObsrvtnTrgts': HSE_TgObsrvtnTrgts,
  'HSE_TgNrstMisccnfrmtn': HSE_TgNrstMisccnfrmtn,
  'HSE_TgNrstMiscflwup': HSE_TgNrstMiscflwup,
  'HSE_TgNrMisRward': HSE_TgNrMisRward,
  'HSE_TgNrstMiscInq': HSE_TgNrstMiscInq,
};

// Uppercase-keyed map for case-insensitive lookup (SCREEN_HANDLERS keys may be mixed/camel case)
const SCREEN_HANDLERS_BY_UPPERCASE = Object.fromEntries(
  Object.entries(SCREEN_HANDLERS).map(([tag, handler]) => [tag.toUpperCase(), handler])
);

/**
 * Get screen handler for screen tag, if any.
 * Lookup is case-insensitive: registered keys (camel/mixed case) are normalized to uppercase for matching.
 * @param {string} strScrTag - Screen tag (any case)
 * @returns {Object|null} Handler module or null
 */
export function getScreenHandler(strScrTag) {
  if (!strScrTag) return null;
  const normalized = strScrTag.toString().trim().toUpperCase();
  return SCREEN_HANDLERS_BY_UPPERCASE[normalized] ?? null;
}

export { SCREEN_HANDLERS };
