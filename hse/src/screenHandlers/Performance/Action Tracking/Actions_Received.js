/**
 * Screen handler: Actions Received (HSE_TgActnsRcvd)
 * Menu path: Performance -> Action Tracking -> Actions Received (from HSE.json)
 * C++: CAR/Action Tracking – custom buttons via handleCarButton; unhandled delegate to backend.
 * RQ_HSE_12_4_26_00_40 — GAP-13: department-filtered work queue via onMenuItemClicked (C++ ChangeToDefaultCriteria).
 */

import { handleCarButton } from '../../../utils/carCustomButtons.js';
import { handleCarFoundationTxnButtons } from '../../../utils/carFoundationTxnButtons.js'; // RQ_HSE_30_3_26_14_57
import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';
import { getDevInterface } from '../../../events/buttonEvents.js'; // RQ_HSE_12_4_26_00_40

function escSql(v) { return String(v ?? '').replace(/'/g, "''"); }

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const handledFoundation = await handleCarFoundationTxnButtons(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (handledFoundation) return;
  const handled = await handleCarButton(Button_Name, strScrTag, eventObj, devInterfaceObj);
  if (!handled) sendButtonClickToBackend((Button_Name || '').toUpperCase(), strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}

/**
 * RQ_HSE_12_4_26_00_40 — GAP-13: C++ CARRcvdCategory::ChangeToDefaultCriteria:
 * RECSTS=21 OR (CRCTVEACCENT_DPRT IN user's departments AND RECSTS=20).
 */
export async function onMenuItemClicked(userObj, strScrTag, callBackFn) {
  const dev = getDevInterface() || {};
  const exec = dev.executeSQLPromise || dev.executeSQL;
  let strScrCriteria = '';

  if (typeof exec === 'function') {
    try {
      const login = (typeof dev.getUserName === 'function' ? dev.getUserName() : '') || userObj?.LogIn || '';
      if (login) {
        const deptRs = await exec(`SELECT EMPLOYEE_WSHOP FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '${escSql(login)}' OR EMPLOYEE_LOGINNAME = '${escSql(login)}'`);
        const rows = deptRs?.recordset ?? [];
        const depts = rows.map(r => String(r.EMPLOYEE_WSHOP ?? '').trim()).filter(Boolean);
        if (depts.length > 0) {
          const deptList = depts.map(d => `'${escSql(d)}'`).join(',');
          strScrCriteria = `WHERE CRCTVEACCENT_RECSTS = '21' OR (CRCTVEACCENT_DPRT IN (${deptList}) AND CRCTVEACCENT_RECSTS = '20')`;
        }
      }
    } catch (e) {
      console.warn('[Web_HSE] RQ_HSE_12_4_26_00_40 GAP-13 Actions Received onMenuItemClicked:', e);
    }
  }

  callBackFn({ isAllowed: true, strScrCriteria, strMsg: '' });
}
