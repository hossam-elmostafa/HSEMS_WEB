/**
 * BUG_HSE_HSM_14_3_26: Get employee code for the current login user (desktop parity).
 * C++: CHSEMSCommonCategory::GetEmployeeCodeForLoginUser() in HSEMSCommonCategory.cpp
 * See GetEmployeeCodeForLoginUser_Desktop_Behavior.md for full spec.
 * @param {Object} devInterface - { getUserName, executeSQLPromise, toast }
 * @returns {Promise<string>} EMPLOYEE_CODE from CMN_EMPLOYEE for current user, or "" if none/multiple/error
 */
export async function getEmployeeCodeForLoginUser(devInterface) {
  // BUG_HSE_HSM_14_3_26: desktop GetEmployeeCodeForLoginUser – query CMN_EMPLOYEE by login, validate, return code or ""
  const { getUserName, executeSQLPromise, toast } = devInterface || {};
  if (!getUserName || !executeSQLPromise) {
    return '';
  }
  const login = typeof getUserName === 'function' ? getUserName() : '';
  const strUserName = login != null && login !== '' ? String(login).trim() : '';
  if (strUserName === '') {
    return '';
  }
  // BUG_HSE_HSM_14_3_26: C++ uses EMPLOYEE_USRID; server schema may use EMPLOYEE_LOGINNAME – try both
  const escaped = strUserName.replace(/'/g, "''");
  let strEmployeeCode = '';
  for (const column of ['EMPLOYEE_USRID', 'EMPLOYEE_LOGINNAME']) {
    try {
      const sql = `SELECT EMPLOYEE_CODE FROM CMN_EMPLOYEE WHERE ${column}='${escaped}'`;
      const result = await executeSQLPromise(sql);
      const rows = result?.recordset ?? result?.recordsets?.[0] ?? (Array.isArray(result) ? result : []);
      const rowCount = Array.isArray(rows) ? rows.length : 0;
      // BUG_HSE_HSM_14_3_26: desktop treats multiple rows as "many employees" – show message and return ""
      if (rowCount > 1) {
        if (toast && typeof toast.warning === 'function') {
          toast.warning('There are many employees linked with the login id, Please solve the problem and try again');
        }
        return '';
      }
      const row = rows?.[0];
      if (row && typeof row === 'object') {
        const val = (row.EMPLOYEE_CODE ?? row.employee_code ?? row[0] ?? '').toString().trim();
        if (val !== '') {
          strEmployeeCode = val;
          break;
        }
      }
    } catch (err) {
      if (column === 'EMPLOYEE_USRID') {
        console.warn('[Web_HSE] BUG_HSE_HSM_14_3_26 getEmployeeCodeForLoginUser EMPLOYEE_USRID query error:', err);
      }
    }
    if (strEmployeeCode !== '') break;
  }
  // BUG_HSE_HSM_14_3_26: C++ if(strEmployeeCode=="") AfxMessageBox("There is no employee linked...")
  if (strEmployeeCode === '') {
    if (toast && typeof toast.warning === 'function') {
      toast.warning('There is no employee linked with the login id, Please solve the problem and try again');
    }
    return '';
  }
  // BUG_HSE_HSM_14_3_26: C++ else if(strstr(strEmployeeCode,"|")) AfxMessageBox("There are many employees linked...")
  if (strEmployeeCode.indexOf('|') !== -1) {
    if (toast && typeof toast.warning === 'function') {
      toast.warning('There are many employees linked with the login id, Please solve the problem and try again');
    }
    return '';
  }
  return strEmployeeCode;
}
