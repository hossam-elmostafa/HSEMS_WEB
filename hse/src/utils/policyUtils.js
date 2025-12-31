/**
 * Get policy value from GMS_POLICY table
 * @param {Function} executeSQLPromise - Function to execute SQL
 * @param {Function} getValFromRecordSet - Function to extract value from recordset
 * @param {string} policyFldName - Policy field name
 * @param {Function} callBackFn - Callback function
 */
export async function getAppPolicy(executeSQLPromise, getValFromRecordSet, policyFldName, callBackFn) {
  let retVal = "";
  if (policyFldName === undefined || policyFldName === "") {
    return;
  }
  let strSQL = `SELECT TOP(1) ${policyFldName} FROM GMS_POLICY`;
  try {
    const data = await executeSQLPromise(strSQL);
    if (data.recordset.length === 0) {
      return callBackFn(retVal);
    } else {
      retVal = getValFromRecordSet(data, policyFldName) || "";
      retVal = retVal.toString().toUpperCase().trim();
      return callBackFn(retVal);
    }
  } catch (err) {
    console.error(err);
  }
}

/**
 * Get policy value (Promise-based version)
 * @param {Function} executeSQL - Function to execute SQL
 * @param {Function} getValFromRecordSet - Function to extract value from recordset
 * @param {string} policyFldName - Policy field name
 * @returns {Promise<string>} - Policy value
 */
export function getAppPolicyV2(executeSQL, getValFromRecordSet, policyFldName) {
  let strSQL = `SELECT TOP(1) ${policyFldName} FROM GMS_POLICY`;
  return new Promise((resolve, reject) => {
    executeSQL(strSQL, (err, data) => {
      if (err) {
        console.log(err);
        reject("");
      } else {
        let retVal = getValFromRecordSet(data, policyFldName) || "";
        retVal = retVal.toString().toUpperCase().trim();
        resolve(retVal);
      }
    });
  });
}



