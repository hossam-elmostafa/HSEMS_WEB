/**
 * Shared utility for setting next serial/line number when NEW is clicked on a tab.
 * C++: HSEMSCommonCategory::DisplayToolBarButtonClicked when strBtnName=="NEW" and getNxtSrl + FormSetField.
 * Use FormSetField(..., 'tab') for tab/subform context.
 */

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Get next serial number for a tab (C++ getNxtSrl).
 * @param {Object} devInterface - executeSQLPromise, getValFromRecordSet
 * @param {Object} opts - { tableName, serialFieldName, linkFieldName, parentTableName, parentKeyFieldName }
 *   For main-form NEW (no link), omit linkFieldName and parentKeyFieldName or pass empty.
 * @param {string} [parentKeyValue] - value from FormGetField(parentTableName, parentKeyFieldName, 'scr')
 * @returns {Promise<number>} Next serial (1-based)
 */
export async function getNextSerialForTab(devInterface, opts, parentKeyValue) {
  const { executeSQLPromise, getValFromRecordSet } = devInterface || {};
  if (!executeSQLPromise || !opts) return 1;
  const { tableName, serialFieldName, linkFieldName, parentKeyFieldName } = opts;
  if (!tableName || !serialFieldName) return 1;

  const key = parentKeyValue != null && parentKeyValue !== '' ? String(parentKeyValue).trim() : '';
  const esc = escapeSqlString(key);
  const where =
    linkFieldName && (esc || key === '')
      ? `WHERE ${linkFieldName} = '${esc}'`
      : '';

  try {
    const sql = `SELECT ISNULL(MAX(${serialFieldName})+1,1) AS New_Serial FROM ${tableName} ${where}`.trim();
    const data = await executeSQLPromise(sql);
    const val =
      data?.recordset?.[0] && getValFromRecordSet
        ? getValFromRecordSet(data, 'New_Serial')
        : data?.recordset?.[0]?.New_Serial;
    const n = parseInt(val, 10);
    return Number.isFinite(n) && n > 0 ? n : 1;
  } catch (e) {
    console.warn('[Web_HSE] getNextSerialForTab:', tableName, e);
    return 1;
  }
}

/**
 * Handle NEW on a tab: get parent key, compute next serial, set field. Use for tab context (FormSetField with 'tab').
 * @param {Object} devInterface - FormGetField, FormSetField, executeSQLPromise, getValFromRecordSet
 * @param {string} tabTableName - table/tag for the tab (FormSetField first arg)
 * @param {string} serialFieldName - field to set (FormSetField second arg)
 * @param {Object} opts - getNextSerialForTab opts: { tableName, serialFieldName, linkFieldName, parentTableName, parentKeyFieldName }
 * @returns {Promise<boolean>} true if ran successfully
 */
export async function setNextSerialOnNewTab(devInterface, tabTableName, serialFieldName, opts) {
  const { FormGetField, FormSetField } = devInterface || {};
  if (!FormGetField || !FormSetField) return false;
  const parentKey =
    opts.parentTableName && opts.parentKeyFieldName
      ? FormGetField(opts.parentTableName, opts.parentKeyFieldName, 'scr')
      : '';
  const key = parentKey != null && parentKey !== '' ? String(parentKey).trim() : '';
  const newSerial = await getNextSerialForTab(devInterface, opts, key);
  FormSetField(tabTableName, serialFieldName, String(newSerial), 'tab');
  return true;
}
