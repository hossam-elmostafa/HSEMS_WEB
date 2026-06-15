/**
 * Actions Review (HSE_TGACTNSRVIW): C++ DisplayFieldChange on CRRCTVACTNS_LNKCSS_ACC —
 * Accepted Y → lock reject reason, ACTNSENTRY_CRR_ACTSTT = A; else MUST reject reason, status R.
 * RQ_HSE_12_4_26_00_40
 */

const TAB_ACTNS_CRR = 'HSE_ACTNSENTRY_CRR';
const FLD_LNKCSS_ACC = 'CRRCTVACTNS_LNKCSS_ACC';
/** C++ field name casing for reject reason on corrections line */
const FLD_RJC_RSN = 'CRRCTVACTNS_LnkCss_RjcRsn';
const FLD_ACT_STT = 'ACTNSENTRY_CRR_ACTSTT';

/**
 * @param {object} dev - devInterfaceObj
 * @param {string} fieldName
 * @param {*} fieldVal
 */
export function applyActionsReviewLnkCssAccChange(dev, fieldName, fieldVal) {
  const fn = String(fieldName || '').toUpperCase();
  if (fn !== FLD_LNKCSS_ACC) return;
  const { changeFldAttrb, FormSetField } = dev || {};
  const v = String(fieldVal ?? '').trim().toUpperCase();
  const accepted = v === 'Y';
  try {
    if (typeof changeFldAttrb === 'function') {
      changeFldAttrb(TAB_ACTNS_CRR, FLD_RJC_RSN, accepted ? 'ALWAYSLOCKED' : 'MUST');
    }
    if (typeof FormSetField === 'function') {
      FormSetField(TAB_ACTNS_CRR, FLD_ACT_STT, accepted ? 'A' : 'R', 'tab');
    }
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 Actions Review LNKCSS_ACC:', e);
  }
}
