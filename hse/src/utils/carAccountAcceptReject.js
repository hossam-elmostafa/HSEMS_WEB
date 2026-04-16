/**
 * RQ_HSE_12_4_26_00_40 — GAP-9: CAR Not Accepted / Accepted at account level.
 * C++: CarCategory::setCARNotAcptd + sendCARNotAccptdNotification; CarCategory::setCARAcptd.
 * Wire into Actions Received and Actions Under Execution screen handlers.
 */

import { getCurrentRecordPRMKY } from './rejectReasonUtils.js';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-9: CAR Not Accepted → setCARNotAcptd SP + email notification.
 */
export async function handleCarNotAccepted(eventObj, devInterfaceObj) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;
  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;
  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';

  try {
    await executeSQLPromise(`EXECUTE setCARNotAcptd '${escapeSqlString(prmky)}','${escapeSqlString(user)}'`);

    // RQ_HSE_12_4_26_00_40 — GAP-15: email notification
    try {
      const rs = await executeSQLPromise(
        `SELECT CRCTVEACCENT_NO, CRCTVEACCENT_DPRT FROM HSE_CRCTVEACCENT WHERE PrmKy = ${parseInt(prmky, 10)}`
      );
      const row = rs?.recordset?.[0] ?? rs?.[0];
      if (row) {
        const carNo = String(row.CRCTVEACCENT_NO ?? row.crctveaccent_no ?? '').trim();
        const dept = String(row.CRCTVEACCENT_DPRT ?? row.crctveaccent_dprt ?? '').trim();
        if (dept && carNo) {
          const mailRs = await executeSQLPromise(
            `SELECT TOP 1 ISNULL(DEPARTMENT_EMAIL,'') AS EMAIL FROM CMN_DEPARTMENT WHERE DEPARTMENT_CODE = '${escapeSqlString(dept)}'`
          );
          const mailRow = mailRs?.recordset?.[0] ?? mailRs?.[0];
          const email = String(mailRow?.EMAIL ?? mailRow?.email ?? '').trim();
          if (email) {
            await executeSQLPromise(`EXECUTE sendCARNotAccptdNotification '${escapeSqlString(email)}','${escapeSqlString(carNo)}'`);
          }
        }
      }
    } catch (notifErr) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 sendCARNotAccptdNotification:', notifErr);
    }

    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 setCARNotAcptd:', e);
    if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error: CAR Not Accepted'));
  }
  return true;
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-9: CAR Accepted → setCARAcptd SP.
 */
export async function handleCarAccepted(eventObj, devInterfaceObj) {
  const { executeSQLPromise, getUserName, refreshData, AskYesNoMessage } = devInterfaceObj || {};
  if (!executeSQLPromise) return false;
  const prmky = getCurrentRecordPRMKY(eventObj);
  if (!prmky) return false;
  const user = (typeof getUserName === 'function' ? getUserName() : '') || '';

  try {
    await executeSQLPromise(`EXECUTE setCARAcptd '${escapeSqlString(prmky)}','${escapeSqlString(user)}'`);
    if (typeof refreshData === 'function') refreshData('', 'REFRESH_SELECTED');
  } catch (e) {
    console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 setCARAcptd:', e);
    if (AskYesNoMessage) AskYesNoMessage(String(e?.message || 'Error: CAR Accepted'));
  }
  return true;
}
