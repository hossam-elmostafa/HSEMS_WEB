/**
 * Screen handler: Chemical Register (HSE_ChmclRgstr)
 * Menu path: Environment -> Chemical Register -> Chemical Register (from HSE.json)
 * C++: ChemicalRegisterCategory – VIEW_ALL_ACTIVE_ITEMS, VIEW_ALL_IN-ACTIVE_ITEMS, VIEW_DEPARTMENT_ITEMS (change criteria), UPDT (subform – delegate to backend).
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

const SCREEN_TAG = 'HSE_ChmclRgstr';
const TABLE_MAIN = 'HSE_CHMCLRGSTR';
const SUBFORM_TAG = 'HSE_CHMCLRGSTR_QTYCRD';

const BTN_VIEW_ACTIVE = 'VIEW_ALL_ACTIVE_ITEMS';
const BTN_VIEW_INACTIVE = 'VIEW_ALL_IN-ACTIVE_ITEMS';
const BTN_VIEW_DEPARTMENT = 'VIEW_DEPARTMENT_ITEMS';
const BTN_UPDT = 'UPDT';

function escapeSqlString(val) {
  if (val == null) return '';
  return String(val).replace(/'/g, "''");
}

/**
 * Change criteria and refresh (C++ ChangeCriteria).
 */
function applyCriteria(devInterfaceObj, criteria) {
  const ChangeCriteria = devInterfaceObj?.ChangeCriteria;
  const refreshData = devInterfaceObj?.refreshData;
  if (typeof ChangeCriteria === 'function' && criteria) {
    ChangeCriteria(SCREEN_TAG, '', criteria);
  }
  if (typeof refreshData === 'function') refreshData('');
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, strSubFormTag, strTabTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const subForm = (strSubFormTag || strTabTag || '').toString().toUpperCase();

  if (btn === BTN_VIEW_ACTIVE) {
    applyCriteria(devInterfaceObj, "WHERE (ChmclRgstr_Actv = 'Y')");
    return;
  }
  if (btn === BTN_VIEW_INACTIVE) {
    applyCriteria(devInterfaceObj, "WHERE (ChmclRgstr_Actv = 'N')");
    return;
  }
  if (btn === BTN_VIEW_DEPARTMENT) {
    const FormGetField = devInterfaceObj?.FormGetField;
    if (FormGetField) {
      const dprtmnt = FormGetField(TABLE_MAIN, 'ChmclRgstr_DprtmntNm', 'scr');
      const val = (dprtmnt != null && dprtmnt !== '') ? escapeSqlString(dprtmnt) : '';
      if (val) applyCriteria(devInterfaceObj, `WHERE (ChmclRgstr_DprtmntNm = '${val}')`);
    }
    return;
  }
  if (subForm === SUBFORM_TAG && btn === BTN_UPDT) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
