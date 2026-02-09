import { toast } from 'react-toastify';

/**
 * AuditPlanInquiryButtonHandlers.js
 *
 * Handles custom button clicks on the Audit Plan Inquiry main screen (HSE_TgAdtPlnInq).
 * Implements the logic from AuditInquiry::DisplayCustomButtonClicked (AuditInquiry.cpp).
 *
 * Requirements:
 * - RQ_HSM_27_01_26_23_00.06: Audit Plan Inquiry Custom Buttons
 * - RQ_HSM_27_01_26_23_00.08: View All
 * - RQ_HSM_27_01_26_23_01.01: View Approved
 * - RQ_HSM_27_01_26_23_01.02: View Closed
 * - RQ_HSM_27_01_26_23_01.03: View Completed
 * - RQ_HSM_27_01_26_23_01.04: View Incomplete
 * - RQ_HSM_27_01_26_23_01.05: View Rejected
 *
 * C++ Reference: AuditInquiry.cpp lines 66-93
 * TableName: HSE_TgAdtPlnInq, TrueTableName: HSE_AdtPlnEnt (CARCategory.cpp 188-190)
 */

/** Screen tag used for ChangeCriteria in C++ (AuditInquiry.cpp) */
const AUDIT_PLAN_INQUIRY_FORM_TAG = 'HSE_TGADTPLNINQ';

/**
 * Map button key (C++) to WHERE clause for ADTPLNENT_ADTPLNSTS.
 * Empty string = View All (no filter).
 */
const BUTTON_TO_CRITERIA = {
  ADTPLNENT_VWALL: '',
  ADTPLNENT_VWINCMPLT: 'WHERE (ADTPLNENT_ADTPLNSTS=1)',
  ADTPLNENT_VWRJCT: 'WHERE (ADTPLNENT_ADTPLNSTS=3)',
  ADTPLNENT_VWCMPLT: 'WHERE (ADTPLNENT_ADTPLNSTS=4)',
  ADTPLNENT_VWAPPRV: 'WHERE (ADTPLNENT_ADTPLNSTS=5)',
  ADTPLNENT_VWCLSD: 'WHERE (ADTPLNENT_ADTPLNSTS=6)',
};

const BUTTON_LABELS = {
  ADTPLNENT_VWALL: 'View All',
  ADTPLNENT_VWINCMPLT: 'View Incomplete',
  ADTPLNENT_VWRJCT: 'View Rejected',
  ADTPLNENT_VWCMPLT: 'View Completed',
  ADTPLNENT_VWAPPRV: 'View Approved',
  ADTPLNENT_VWCLSD: 'View Closed',
};

/**
 * Handle Audit Plan Inquiry filter button click (View All, View Incomplete, etc.)
 * C++: ChangeCriteria("HSE_TGADTPLNINQ","", criteria); RefreshScreen("",0);
 *
 * @param {string} buttonName - Button key (e.g. ADTPLNENT_VWALL, ADTPLNENT_VWAPPRV)
 * @param {string} screenTag - Current screen tag
 * @param {Object} eventObj - Event object (strScrTag, etc.)
 * @param {Object} devInterface - { openScr, refreshData }
 */
export async function handleAuditPlanInquiryFilterButton(buttonName, screenTag, eventObj, devInterface) {
  const normalizedButton = (buttonName || '').toString().toUpperCase();
  const criteria = BUTTON_TO_CRITERIA[normalizedButton];
  if (criteria === undefined) {
    console.log('[Web_HSE] Audit Plan Inquiry: unknown filter button:', buttonName);
    return;
  }

  const label = BUTTON_LABELS[normalizedButton] || normalizedButton;
  try {
    console.log('[Web_HSE] Audit Plan Inquiry filter button:', { buttonName: normalizedButton, criteria, label });
    toast.info(`${label} – applying filter...`);

    const { openScr, refreshData } = devInterface || {};
    if (!openScr) {
      console.error('[Web_HSE] Audit Plan Inquiry: openScr not available');
      toast.error('Cannot apply filter: openScr not available');
      return;
    }

    // C++: ChangeCriteria("HSE_TGADTPLNINQ","", criteria);
    openScr(AUDIT_PLAN_INQUIRY_FORM_TAG, {}, criteria || '', 'list', false, {});

    // C++: RefreshScreen("",0);
    if (refreshData) {
      setTimeout(() => {
        refreshData('', 'REFRESH_ALL');
        toast.success(`Showing ${label.toLowerCase()}`);
      }, 200);
    } else {
      toast.success(`Showing ${label.toLowerCase()}`);
    }
  } catch (error) {
    console.error('[Web_HSE] Audit Plan Inquiry filter error:', error);
    toast.error('Unable to apply filter: ' + (error.message || 'Unknown error'));
  }
}
