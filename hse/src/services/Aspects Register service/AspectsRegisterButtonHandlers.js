import { toast } from "react-toastify";

/**
 * AspectsRegisterButtonHandlers.js
 * 
 * Handles all button click events for Aspects Register module screens
 * 
 * C++ Reference Files:
 * - EnvrnmntAspctCategory.cpp
 * 
 * Requirements:
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
 * - RQ_HSM_27_01_26_22_56.02: View Current Month
 *   Custom Button ID: VIEW_CURRENT_MONTH
 * - RQ_HSM_27_01_26_22_56.03: View History
 *   Custom Button ID: VIEW_HISTORY
 */

/**
 * Safe wrapper for FormGetField that handles errors gracefully
 * @param {Function} FormGetField - The FormGetField function
 * @param {string} formTag - The form tag to read from
 * @param {string} fieldName - The field name to read
 * @returns {string} - The field value or empty string if error
 */
function safeFormGetField(FormGetField, formTag, fieldName) {
  try {
    if (!FormGetField || !formTag || !fieldName) {
      return '';
    }
    const value = FormGetField(formTag, fieldName);
    return value || '';
  } catch (error) {
    // Silently return empty string if form tag or field doesn't exist
    return '';
  }
}

/**
 * Handle Entry Completed button click for Aspects Entry
 * 
 * Requirement:
 * - RQ_HSM_22_01_26_10_37.02: Entry Completed
 *   Custom Button ID: ENTRY_COMPLETED
 * 
 * This function implements the "Entry Completed" functionality for Aspects Entry screen.
 * When the user clicks the "Entry Completed" button, it:
 * 1. Gets the primary key from the selected record
 * 2. Executes the EnvAspctEntryComplete stored procedure to:
 *    - Complete the aspects entry
 *    - Update the status accordingly
 * 3. Refreshes the screen to show updated status
 * 
 * C++ Reference: EnvrnmntAspctCategory.cpp line 152-164: ENTRY_COMPLETED button handler
 * C++ Reference: EnvrnmntAspctCategory.cpp line 204-210: completeEntry() helper function
 * 
 * @param {string} buttonName - The button name (e.g. ENTRY_COMPLETED)
 * @param {string} screenTag - The screen tag (e.g. HSE_AspctsEntryAtEntry)
 * @param {Object} eventObj - The full event object containing fullRecord, fullRecordArrKeys, etc.
 * @param {Object} devInterface - Object containing devInterface functions (FormGetField, executeSQL, etc.)
 * 
 * @example
 * // Called when user clicks "Entry Completed" button on Aspects Entry screen
 * await handleEntryCompletedButton('ENTRY_COMPLETED', 'HSE_AspctsEntryAtEntry', eventObj, devInterface);
 */
export async function handleEntryCompletedButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      refreshData,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !refreshData
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Entry Completed button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;
    const formTag = screenTag || 'HSE_AspctsEntryAtEntry';

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // C++: CString strPrmryKy = FormGetField("HSE_ASPCTS_VIEW","PrmryKy");
    // Get primary key from the form
    let primaryKey = '';
    
    // Try to get from event object first
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0 && fullRecordArrKeys[0] != null && fullRecordArrKeys[0] !== undefined) {
      primaryKey = String(fullRecordArrKeys[0]).trim();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      if (firstRecord) {
        primaryKey = String(
          firstRecord.PrmryKy ||
          firstRecord.PRMRYKY ||
          firstRecord.ASPCTS_PRMKY ||
          firstRecord.ASPCTS_PrmryKy ||
          ''
        ).trim();
      }
    }

    // Fallback: read from form if still empty
    if (!primaryKey) {
      primaryKey =
        safeFormGetField(FormGetField, 'HSE_ASPCTS_VIEW', 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_Aspcts_VIEW', 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_ASPCTSENTRYATENTRY', 'PrmryKy') ||
        safeFormGetField(FormGetField, formTag, 'PrmryKy') ||
        safeFormGetField(FormGetField, 'HSE_ASPCTS_VIEW', 'PRMRYKY') ||
        safeFormGetField(FormGetField, formTag, 'PRMRYKY') ||
        '';
      primaryKey = String(primaryKey).trim();
    }

    if (!primaryKey || primaryKey === 'undefined' || primaryKey === 'null') {
      toast.warning('Unable to find primary key. Please select a record first.');
      return;
    }

    // C++: void EnvrnmntAspctCategory::completeEntry(CString strPrmryKey)
    // C++: strSQL.Format("EXECUTE EnvAspctEntryComplete '%s'",strPrmryKey);
    const completeSql = `EXECUTE EnvAspctEntryComplete '${primaryKey.replace(/'/g, "''")}'`;
    
    // Log parameter values for debugging
    console.log('[Web_HSE] Aspects Entry Completed parameters:', {
      primaryKey,
    });

    console.log('[Web_HSE] Completing Aspects entry:', completeSql);
    try {
      await executeSQLAsync(completeSql);
      console.log('[Web_HSE] ✓ Aspects entry completed successfully');
    } catch (dbError) {
      const errorMessage = dbError?.message || dbError?.toString() || '';
      
      // Check if error is about invalid column name (database schema issue)
      if (errorMessage.includes('invalid column name') || errorMessage.includes('Invalid column name')) {
        console.error('[Web_HSE] Database schema error - invalid column name:', errorMessage);
        const columnMatch = errorMessage.match(/invalid column name ['"]([^'"]+)['"]/i) || 
                           errorMessage.match(/Invalid column name ['"]([^'"]+)['"]/i);
        const columnName = columnMatch ? columnMatch[1] : 'unknown column';
        toast.error(`Database error: Invalid column name '${columnName}' detected in stored procedure. Please check database schema and stored procedure 'EnvAspctEntryComplete' definition.`);
        console.error('[Web_HSE] Stored procedure called: EnvAspctEntryComplete');
        console.error('[Web_HSE] Please check the stored procedure definition and update column references if needed.');
        console.error('[Web_HSE] Common issues:');
        console.error('[Web_HSE] - Column may have been renamed or removed');
        console.error('[Web_HSE] - Column name may have a typo in the stored procedure');
        console.error('[Web_HSE] - Database schema may need to be updated');
        return;
      }
      
      // Generic database error
      console.error('[Web_HSE] Database error completing Aspects entry:', errorMessage);
      toast.error(`Error completing Aspects entry: ${errorMessage}`);
      return;
    }

    // Refresh screen
    // C++: RefreshScreen("",REFRESH_SELECTED);
    refreshData('', 'REFRESH_SELECTED');
    toast.success(`Aspects entry ${primaryKey} completed successfully`);
  } catch (error) {
    console.error('[Web_HSE] Error in handleEntryCompletedButton:', error);
    toast.error('An error occurred while completing the Aspects entry');
  }
}

/**
 * Handle View Current Month button click for Aspects Entry
 *
 * Requirement:
 * - RQ_HSM_27_01_26_22_56.02: View Current Month
 *   Custom Button ID: VIEW_CURRENT_MONTH
 *
 * Updates the screen criteria to show only records for the selected year/month.
 * Matches desktop: reads ASPCTS_YR and ASPCTS_MNTH from form HSE_ASPCTS_VIEW (current record / aspect period).
 * If form has no year/month, falls back to current calendar date.
 * Criteria matches both month formats ('2' and '02') for ASPCTS_MNTH.
 *
 * C++ Reference: EnvrnmntAspctCategory.cpp line 140-152: FormGetField("HSE_ASPCTS_VIEW","ASPCTS_YR/MNTH")
 *
 * @param {string} buttonName - The button name (e.g. VIEW_CURRENT_MONTH)
 * @param {string} screenTag - The screen tag (e.g. HSE_AspctsEntryAtEntry)
 * @param {Object} eventObj - The full event object
 * @param {Object} devInterface - Object containing openScr, refreshData
 */
function getYearMonthFromRecord(record) {
  if (!record || typeof record !== 'object') return { year: '', month: '' };
  const y = (record.ASPCTS_YR ?? record.ASPCTS_Yr ?? record.aspcts_yr ?? '').toString().trim();
  const m = (record.ASPCTS_MNTH ?? record.ASPCTS_Mnth ?? record.aspcts_mnth ?? '').toString().trim();
  return { year: y, month: m };
}

/** Treat empty or 0 as invalid so we fall back to current date instead of showing "0-00". */
function isValidYearMonth(val) {
  const s = String(val ?? '').trim();
  if (s === '' || s === '0') return false;
  const n = parseInt(s, 10);
  return Number.isFinite(n) && n > 0;
}

/** C++ view name: ParentManage(..., "HSE_ASPCTS_VIEW", ...) - EnvrnmntAspctCategory.cpp line 171 */
const ASPCTS_VIEW_NAME = 'HSE_ASPCTS_VIEW';

/** Get recordset array from executeSQLPromise result (handles recordset / recordsets[0] / array / response.data). */
function getRecordsetArray(rs) {
  if (!rs) return [];
  if (Array.isArray(rs)) return rs;
  if (rs.data !== undefined && (rs.data?.recordset !== undefined || rs.data?.recordsets !== undefined || Array.isArray(rs.data))) rs = rs.data;
  const set = rs.recordset ?? rs.recordsets?.[0];
  return Array.isArray(set) ? set : [];
}

/** Read first value from a single-column query result (C++ GetRecordSetData). Tries getValFromRecordSet, then named columns, then first column of first row. */
function readFirstValue(rs, getValFromRecordSet, columnNames) {
  if (!rs) return '';
  // Unwrap axios-style response (response.data) or use as-is
  if (rs.data !== undefined && (rs.data?.recordset !== undefined || rs.data?.recordsets !== undefined)) rs = rs.data;
  const names = Array.isArray(columnNames) ? columnNames : [columnNames];
  if (getValFromRecordSet && typeof getValFromRecordSet === 'function') {
    for (const col of names) {
      try {
        const v = getValFromRecordSet(rs, col);
        if (v !== undefined && v !== null && String(v).trim() !== '') return String(v).trim();
      } catch (_) {}
    }
  }
  const recordset = rs?.recordset ?? rs?.recordsets?.[0];
  const row = Array.isArray(rs) ? rs[0] : Array.isArray(recordset) ? recordset[0] : rs?.recordset?.[0] ?? recordset?.[0] ?? rs;
  if (row && typeof row === 'object') {
    for (const col of names) {
      const v = row[col] ?? row[col.toLowerCase()];
      if (v !== undefined && v !== null && String(v).trim() !== '') return String(v).trim();
    }
    // Fallback: first column value (handles different DB column names/casing)
    for (const key of Object.keys(row)) {
      const v = row[key];
      if (v !== undefined && v !== null && String(v).trim() !== '') return String(v).trim();
    }
  }
  return '';
}

/**
 * View History button (RQ_HSM_27_01_26_22_56.03).
 * Shows all aspects history: admin sees all departments, non-admin sees only their department(s).
 * C++ Reference: EnvrnmntAspctCategory.cpp lines 131-139 (VIEW_HISTORY).
 */
/** Fallback form tag when current screen tag is not an Aspects screen. */
const ASPCTS_FORM_TAG = 'HSE_ASPCTS_VIEW';

export async function handleViewHistoryButton(buttonName, screenTag, eventObj, devInterface) {
  const label = 'View History';
  console.log(`[Web_HSE] ${label}: handler running`);
  try {
    const { executeSQLPromise, getUserName, isAdminUser, getIsAdminUser, openScr, refreshData } = devInterface || {};

    if (!executeSQLPromise) {
      console.error(`[Web_HSE] ${label}: Missing executeSQLPromise`);
      toast.error('System error: Required functions not available');
      return;
    }

    const isAdmin = isAdminUser === true || (typeof getIsAdminUser === 'function' && getIsAdminUser() === true);
    let whereClause;

    if (isAdmin) {
      whereClause = "(ASPCTS_DPRTMNT LIKE '%')";
      console.log(`[Web_HSE] ${label}: admin user, showing all departments`);
    } else {
      const userName = typeof getUserName === 'function' ? getUserName() : '';
      const login = String(userName || '').trim();
      if (!login) {
        console.warn(`[Web_HSE] ${label}: no user name, falling back to all departments`);
        whereClause = "(ASPCTS_DPRTMNT LIKE '%')";
      } else {
        const escaped = login.replace(/'/g, "''");
        const sqlDep = `SELECT ISNULL(EMPLOYEE_WSHOP,'') AS EMPLOYEE_WSHOP FROM CMN_EMPLOYEE WHERE (EMPLOYEE_USRID = '${escaped}' OR EMPLOYEE_LOGINNAME = '${escaped}')`;
        let depList = [];
        try {
          const result = await executeSQLPromise(sqlDep);
          const arr = getRecordsetArray(result);
          const col = 'EMPLOYEE_WSHOP';
          const seen = new Set();
          for (const row of arr || []) {
            const d = (row && (row[col] ?? row.EMPLOYEE_WSHOP ?? row.employee_workshop)) != null
              ? String(row[col] ?? row.EMPLOYEE_WSHOP ?? row.employee_workshop).trim()
              : '';
            if (d && !seen.has(d)) {
              seen.add(d);
              depList.push("'" + d.replace(/'/g, "''") + "'");
            }
          }
        } catch (e) {
          console.warn(`[Web_HSE] ${label}: getEmpDep failed`, e);
        }
        if (depList.length === 0) {
          whereClause = "(ASPCTS_DPRTMNT LIKE '%')";
        } else {
          whereClause = `(ASPCTS_DPRTMNT IN (${depList.join(',')}))`;
        }
        console.log(`[Web_HSE] ${label}: non-admin, department filter (${depList.length} dept(s))`);
      }
    }

    const viewNames = [ASPCTS_VIEW_NAME, 'HSE_Aspcts_VIEW'];
    let rows = [];
    for (const viewName of viewNames) {
      try {
        const sql = `SELECT * FROM ${viewName} WHERE ${whereClause}`;
        const result = await executeSQLPromise(sql);
        rows = getRecordsetArray(result);
        break;
      } catch (err) {
        const msg = err?.message || String(err);
        if (msg.includes('invalid object name') && viewNames.indexOf(viewName) < viewNames.length - 1) continue;
        console.error(`[Web_HSE] ${label}: query failed`, err);
        toast.error('Could not load history: ' + (msg.slice(0, 80)));
        return;
      }
    }

    const count = rows.length;
    toast.success(`${count} record${count !== 1 ? 's' : ''} (View History)`);

    // Use existing list/grid: open list with criteria so the normal grid shows the data
    if (openScr && typeof openScr === 'function') {
      try {
        const criteria = 'WHERE ' + whereClause;
        const formTag = (screenTag && String(screenTag).toUpperCase().includes('ASPCTS')) ? screenTag : ASPCTS_FORM_TAG;
        openScr(formTag, {}, criteria, 'list', false, {}, undefined, undefined, true);
        if (refreshData && typeof refreshData === 'function') {
          setTimeout(() => {
            try { refreshData('', 'REFRESH_ALL'); } catch (_) {}
          }, 300);
        }
      } catch (openErr) {
        console.warn('[Web_HSE] View History: openScr/refresh failed:', openErr);
      }
    }
  } catch (error) {
    console.error(`[Web_HSE] Error in handleViewHistoryButton:`, error);
    toast.error('An error occurred while loading history');
  }
}

export async function handleViewCurrentMonthButton(buttonName, screenTag, eventObj, devInterface) {
  const label = (buttonName || '').toUpperCase() === 'VIEW_HISTORY' ? 'View History' : 'View Current Month';
  console.log(`[Web_HSE] ${label}: handler running`);
  try {
    const { FormGetField, executeSQLPromise, getValFromRecordSet } = devInterface || {};

    if (!executeSQLPromise) {
      console.error(`[Web_HSE] ${label}: Missing executeSQLPromise`);
      toast.error('System error: Required functions not available');
      return;
    }

    const now = new Date();
    let year = '';
    let month = '';
    let source = '';

    // 1) HSE_HSEPLC first (C++ uses getAspctYr/getAspctMonth when form is empty - no selection required)
    if (executeSQLPromise) {
      try {
        const sqlPlc = "SELECT TOP 1 ISNULL(HSEPLC_ASPCTYR,'') AS HSEPLC_ASPCTYR, ISNULL(HSEPLC_ASPCTMNTH,'') AS HSEPLC_ASPCTMNTH FROM HSE_HSEPLC";
        const rsPlc = await executeSQLPromise(sqlPlc);
        const arr = getRecordsetArray(rsPlc);
        const firstRow = arr && arr[0];
        if (firstRow && typeof firstRow === 'object') {
          const rawYr = firstRow.HSEPLC_ASPCTYR ?? firstRow.HSEPLC_ASPCTyr ?? firstRow.hseplc_aspctyr ?? '';
          const rawMn = firstRow.HSEPLC_ASPCTMNTH ?? firstRow.HSEPLC_ASPCTMnth ?? firstRow.hseplc_aspctmnth ?? '';
          let plcYr = (rawYr !== null && rawYr !== undefined ? String(rawYr) : '').trim();
          let plcMn = (rawMn !== null && rawMn !== undefined ? String(rawMn) : '').trim();
          if (!plcYr || !plcMn) {
            const vals = Object.values(firstRow).filter(v => v !== null && v !== undefined && String(v).trim() !== '');
            if (vals.length >= 2) {
              if (!plcYr) plcYr = String(vals[0]).trim();
              if (!plcMn) plcMn = String(vals[1]).trim();
            }
          }
          console.log(`[Web_HSE] ${label}: HSE_HSEPLC returned year="${plcYr}" month="${plcMn}"`);
          if (isValidYearMonth(plcYr) && isValidYearMonth(plcMn)) {
            year = plcYr;
            month = plcMn;
            source = 'HSE_HSEPLC';
          }
        }
        if (!source || source !== 'HSE_HSEPLC') {
          const plcYr = readFirstValue(rsPlc, getValFromRecordSet, ['HSEPLC_ASPCTYR', 'YR']);
          const plcMn = readFirstValue(rsPlc, getValFromRecordSet, ['HSEPLC_ASPCTMNTH', 'MNTH']);
          if (isValidYearMonth(plcYr) && isValidYearMonth(plcMn)) {
            year = String(plcYr).trim();
            month = String(plcMn).trim();
            source = 'HSE_HSEPLC';
          }
        }
      } catch (e) {
        console.warn(`[Web_HSE] ${label}: HSE_HSEPLC failed`, e);
      }
    }

    // 2) Form overrides (current record on screen - C++ FormGetField). Include current screen tag so we read from the open form.
    const viewFormTags = [screenTag, 'HSE_ASPCTS_VIEW', 'HSE_Aspcts_VIEW', 'HSE_ASPCTSENTRYATENTRY'].filter(Boolean);
    const seen = new Set();
    for (const tag of viewFormTags) {
      const key = (tag || '').toUpperCase();
      if (seen.has(key)) continue;
      seen.add(key);
      const fy = safeFormGetField(FormGetField, tag, 'ASPCTS_YR') || safeFormGetField(FormGetField, tag, 'ASPCTS_Yr');
      const fm = safeFormGetField(FormGetField, tag, 'ASPCTS_MNTH') || safeFormGetField(FormGetField, tag, 'ASPCTS_Mnth');
      if (isValidYearMonth(fy) && isValidYearMonth(fm)) {
        year = String(fy).trim();
        month = String(fm).trim();
        source = 'form';
        break;
      }
    }

    // 3) Selected row overrides (when user clicked with a row selected)
    if (eventObj) {
      const rec = eventObj.fullRecord?.[0] ?? eventObj.fullRecord;
      const fromRec = getYearMonthFromRecord(rec);
      if (isValidYearMonth(fromRec.year) && isValidYearMonth(fromRec.month)) {
        year = String(fromRec.year).trim();
        month = String(fromRec.month).trim();
        source = 'selectedRecord';
      }
    }

    // 4) Fallback to current date (also when form/HSEPLC returned 0)
    if (!year || !isValidYearMonth(year)) year = String(now.getFullYear());
    else year = String(year).trim();
    if (!month || !isValidYearMonth(month)) month = String(now.getMonth() + 1);
    else month = String(month).trim();
    const monthNum = parseInt(month, 10);
    if (monthNum < 1 || monthNum > 12) month = String(now.getMonth() + 1);
    if (!source) source = 'currentDate';

    const monthPadded = month.padStart(2, '0');

    const y = year.replace(/'/g, "''");
    const m = month.replace(/'/g, "''");
    const mp = monthPadded.replace(/'/g, "''");
    const whereClause = `(ASPCTS_YR = '${y}' AND (ASPCTS_MNTH = '${m}' OR ASPCTS_MNTH = '${mp}'))`;

    console.log(`[Web_HSE] ${label}: using year="${year}" month="${month}" source=${source}`);

    // Query views that exist in DB (HSE_ASPCTSENTRYATENTRY is a grid name in C++, not always a DB object)
    const viewNames = [ASPCTS_VIEW_NAME, 'HSE_Aspcts_VIEW'];
    let rows = [];
    for (const viewName of viewNames) {
      try {
        const sql = `SELECT * FROM ${viewName} WHERE ${whereClause}`;
        const result = await executeSQLPromise(sql);
        rows = getRecordsetArray(result);
        break;
      } catch (err) {
        const msg = err?.message || String(err);
        if (msg.includes('invalid object name') && viewNames.indexOf(viewName) < viewNames.length - 1) continue;
        console.error('[Web_HSE] View Current Month: query failed', err);
        toast.error('Could not load data: ' + (msg.slice(0, 80)));
        return;
      }
    }

    const count = rows.length;
    toast.success(`${count} record${count !== 1 ? 's' : ''} for ${year}-${monthPadded}`);
    if (count > 0) console.log('[Web_HSE] View Current Month rows:', rows);
    // Note: openScr not used here - it triggers FirstMainDrawer loadActions which expects different data and throws (undefined.toString)
  } catch (error) {
    console.error('[Web_HSE] Error in handleViewCurrentMonthButton:', error);
    toast.error('An error occurred while applying current month filter');
  }
}
