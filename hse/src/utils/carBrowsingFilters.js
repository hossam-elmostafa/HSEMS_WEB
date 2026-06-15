/**
 * RQ_HSE_12_4_26_00_40 — GAP-16: Browsing filters for HSEVRFDBY fields.
 * C++: DisplayBeforBrowsingButtonClick — site → getCmpnyNamFrmSit → owner department.
 * Filters browse results for HSE verified-by lookup fields.
 */

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Build WHERE clause for HSEVRFDBY browse: filter by site → company → owner department.
 * @param {object} devInterfaceObj
 * @param {string} siteFieldValue — current site from the form
 * @returns {Promise<string>} WHERE clause or empty string
 */
export async function getHseVerifiedByBrowseFilter(devInterfaceObj, siteFieldValue) {
  const { executeSQLPromise } = devInterfaceObj || {};
  if (!executeSQLPromise || !siteFieldValue) return '';

  try {
    const escSite = escapeSqlString(String(siteFieldValue).trim());
    const companyRs = await executeSQLPromise(
      `SELECT TOP 1 ISNULL(SITE_COMPANY,'') AS C FROM CMN_SITE WHERE SITE_CODE = '${escSite}'`
    );
    const companyRow = companyRs?.recordset?.[0] ?? companyRs?.[0];
    const company = String(companyRow?.C ?? companyRow?.c ?? '').trim();
    if (!company) return '';

    const ownerRs = await executeSQLPromise(
      `SELECT TOP 1 ISNULL(HSEPLC_OWNRDPRT,'') AS D FROM HSE_HSEPLC`
    );
    const ownerRow = ownerRs?.recordset?.[0] ?? ownerRs?.[0];
    const ownerDept = String(ownerRow?.D ?? ownerRow?.d ?? '').trim();

    const parts = [`EMPLOYEE_COMPANY = '${escapeSqlString(company)}'`];
    if (ownerDept) parts.push(`EMPLOYEE_WSHOP = '${escapeSqlString(ownerDept)}'`);
    return parts.join(' AND ');
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-16 getHseVerifiedByBrowseFilter:', e);
    return '';
  }
}
