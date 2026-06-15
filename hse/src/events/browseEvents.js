import { getDevInterface } from './buttonEvents.js';

/**
 * OnBeforeBrowseButtonClick
 * RQ_HSE_12_4_26_00_40 — GAP-3: resolve devInterface via getDevInterface() (set in hse.js) when brwsObj omits it.
 * @param {string} strScrTag header screen tag 
 * @param {string} strTabTag tab tag if screen is tab else this will be ''
 * @param {object} brwsObj 
 * brwsObj structure :
 * {
 *    isOpen,                 // true/false open browse dialog or not,don't change it unless you want tp prevent browse dialog from open!
 *    fieldObj                // Browse field object,will be described in details below  
 *    BrowseTag               // Browse tag which will be opened,you can change it if you want to open another screen based on some conditions in your business
 *    BrowseData              //
 *    BrowsingTable           // Table of Browse screen
 *    scrType                 // 
 *    BrowsingSQL             // Criteria for opened browsing screen,you can change it based on your new criteria
 *    fieldDBType             // Database type of field
 *    isfilterFld             // true/false if field browse from advanced filter then this will be true,else it will be false,Please don't change this.
 *    bAllowMultiSelection    // true/false enable or disable multi-selection in browse
 * }
 * 
 * @param {object} fullRecord full record object "full record of caller screen" {field_Name:field_value,field_Name:field_value,...}
 * @returns {Promise} this function always return promise with browsObj,You can keep browsObj as it or change any attribute and return it.
 */
export function OnBeforeBrowseButtonClick(strScrTag, strTabTag, brwsObj, fullRecord) {
  return new Promise(async (resolve, reject) => { // eslint-disable-line no-async-promise-executor
    strScrTag = strScrTag.toString().toUpperCase();
    // RQ_HSE_12_4_26_00_40 — GAP-3: filter HSEVRFDBY browse fields on CAR screens
    try {
      await applyHseVerifiedByBrowseFilter(strScrTag, brwsObj, fullRecord);
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-3 browse filter:', e);
    }
    resolve(brwsObj);
  });
}

// RQ_HSE_12_4_26_00_40 — GAP-3: HSEVRFDBY browse filtering (C++ DisplayBeforBrowsingButtonClick)
const HSEVRFDBY_SCREENS = new Set([
  'HSE_TGCRCTVEACCJOBVRFCTN', 'HSE_TGACTNSUNDREXEC',
  'HSE_TGCRCTVEACCCNFRMTN', 'HSE_TGCARFLWUPVSTS',
]);

async function applyHseVerifiedByBrowseFilter(scrTag, brwsObj, fullRecord) {
  if (!brwsObj || !HSEVRFDBY_SCREENS.has(scrTag)) return;
  const fieldName = String(brwsObj.fieldObj?.strFieldName || brwsObj.fieldObj?.fieldName || '').toUpperCase();
  if (!fieldName.includes('HSEVRFDBY')) return;

  const siteVal = findFieldCaseInsensitive(fullRecord, [
    'CRCTVEACCENT_SIT', 'CRENTRY_ST', 'ACTNSENTRY_SIT',
  ]);
  if (!siteVal) return;

  const { getHseVerifiedByBrowseFilter } = await import('../utils/carBrowsingFilters.js');
  // RQ_HSE_12_4_26_00_40 — GAP-3: browse framework does not pass devInterface on brwsObj; use same instance as ButtonClicked/ShowScreen
  const devInterface =
    brwsObj._devInterfaceObj ||
    (typeof getDevInterface === 'function' ? getDevInterface() : null) ||
    globalThis.__hseDevInterface;
  if (!devInterface?.executeSQLPromise) return;
  const whereClause = await getHseVerifiedByBrowseFilter(devInterface, siteVal);
  if (whereClause) {
    brwsObj.BrowsingSQL = brwsObj.BrowsingSQL
      ? `${brwsObj.BrowsingSQL} AND ${whereClause}`
      : `WHERE ${whereClause}`;
  }
}

function findFieldCaseInsensitive(record, candidates) {
  if (!record || typeof record !== 'object') return '';
  const keys = Object.keys(record);
  for (const c of candidates) {
    const cu = c.toUpperCase();
    const found = keys.find(k => k.toUpperCase() === cu);
    if (found) {
      const v = String(record[found] ?? '').trim();
      if (v) return v;
    }
  }
  return '';
}

/**
 * UpdateAfterBrowse
 * @param {string} strScrTag              // header screen tag
 * @param {object} selectedBrowseRow      // return row fro browse screen {field_Name:field_value,field_Name:field_value,...}
 * @param {string} strBrowsingTable       // browse table name
 * @param {string} strFldVal              // value for return field from browse 
 * @param {string} strBrowsingField       // Then name of browsing field
 * @param {object} updatedAppGlobalVal    // Object 'Ignore this for now' 
 * 
 * This event doesn't require callback
 */
export function UpdateAfterBrowse(
  strScrTag,
  strTabTag,
  selectedBrowseRow,
  strBrowsingTable,
  strOldFldVal,
  strFldVal,
  strBrowsingField,
  updatedAppGlobalVal
) {
  // Empty implementation - can be extended later
}


