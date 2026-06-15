/**
 * CAR — server date helper.
 * RQ_HSE_12_4_26_00_40
 *
 * Legacy C++ used GetServerDate() to obtain GETDATE() from SQL Server so that
 * dates written to CRENTRY_RVWDT and ACTNSENTRY_CRR_ACTFLLDT are consistent
 * regardless of client time-zone or clock drift.
 *
 * This module provides the equivalent for the web shell.  All callers MUST fall
 * back to getCurrentFormatedDate() (client clock) if the SQL query fails so that
 * the screen still works in offline/degraded-connectivity scenarios.
 */

/**
 * Fetch the current date from SQL Server (GETDATE()) as an ISO string (YYYY-MM-DD).
 * Falls back to client clock on error.
 *
 * @param {Function|undefined} executeSQLPromise  - shell's executeSQLPromise
 * @param {Function|undefined} getCurrentFormatedDate - shell's getCurrentFormatedDate
 * @returns {Promise<string>} date string (non-empty) or empty string if both sources fail
 */
export async function getCarServerDate(executeSQLPromise, getCurrentFormatedDate) {
  // RQ_HSE_12_4_26_00_40 — prefer SQL Server GETDATE() (timezone-neutral, matches desktop GetServerDate())
  if (typeof executeSQLPromise === 'function') {
    try {
      const rs = await executeSQLPromise(
        `SELECT CONVERT(varchar(10), GETDATE(), 23) AS D`
      );
      const row = rs?.recordset?.[0] ?? rs?.[0];
      const d = String(row?.D ?? row?.d ?? '').trim();
      if (d) return d;
    } catch (_) {
      /* fall through to client clock */
    }
  }
  // Fallback: client clock (getCurrentFormatedDate may return locale-formatted string)
  if (typeof getCurrentFormatedDate === 'function') {
    try {
      const d = getCurrentFormatedDate();
      if (d && String(d).trim()) return String(d).trim();
    } catch (_) {}
  }
  return '';
}
