/**
 * Screen handler: Environmental Measures Popup (HSE_TgEnvMntrActvtPltnt)
 * Opened from: Environmental Monitoring Plan (read-only) and Environmental Measures / Execution (editable).
 * C++: EnvMntrMsrPopupCategory – ENVMNTRACTVTSPLTNTMSRAREA_RQRDACTN button, record lock via ActivityStatusHolder.
 *
 * RQ_HSE_23_3_26_21_14: New handler implementing gaps GA-3 and GB-7:
 *   GA-3 – ButtonClicked RQRDACTN: open Required Actions popup (checks RQRNCR flag).
 *   GB-7 – MainSubReposition: record lock when ActivityStatusHolder >= 5.
 */

const SCREEN_TAG = 'HSE_TgEnvMntrActvtPltnt';
const TABLE_MSRAREA = 'HSE_ENVMNTRACTVTSPLTNTMSRAREA';
const TABLE_PLTNT = 'HSE_ENVMNTRACTVTSPLTNT';

const BTN_RQRDACTN = 'ENVMNTRACTVTSPLTNTMSRAREA_RQRDACTN';

// RQ_HSE_23_3_26_21_14: module-level devInterfaceObj for MainSubReposition
let _devInterfaceObj = {};

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-3 – Required Actions popup
// (C++: EnvMntrMsrPopupCategory::DisplayCustomButtonClicked)
// ---------------------------------------------------------------------------

function handleRequiredActions(devInterfaceObj) {
  const openScr = devInterfaceObj?.openScr;
  const FormGetField = devInterfaceObj?.FormGetField;
  const AskYesNoMessage = devInterfaceObj?.AskYesNoMessage;
  if (typeof openScr !== 'function' || !FormGetField) return;

  const rqrNCR = String(FormGetField(TABLE_MSRAREA, 'ENVMNTRACTVTSPLTNTMSRAREA_RQRNCR', 'tab') ?? '').toUpperCase();
  if (rqrNCR === 'Y') {
    const prmKy = String(FormGetField(TABLE_MSRAREA, 'PRMKY', 'tab') ?? '').trim();
    if (!prmKy) return;
    const strCriteria = `SELECT * FROM HSE_ENVMNTRRQRDACTN WHERE (ENVMNTRRQRDACTN_LNK = ${prmKy})`;
    const defValObj = { ENVMNTRRQRDACTN_LNK: prmKy };
    openScr('HSE_TGENVMNTRRQRDACTN', {}, strCriteria, 'edit', false, defValObj, false, false);
  } else {
    if (typeof AskYesNoMessage === 'function') {
      AskYesNoMessage('This measure area does not require an NCR');
    }
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GA-3 – ButtonClicked
// ---------------------------------------------------------------------------

export async function ButtonClicked(eventObj) {
  const { Button_Name, devInterfaceObj } = eventObj || {};
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';

  if (btn === BTN_RQRDACTN) {
    handleRequiredActions(devInterfaceObj);
  }
}

// ---------------------------------------------------------------------------
// RQ_HSE_23_3_26_21_14  GB-7 – Record lock based on ActivityStatusHolder
// (C++: EnvMntrMsrPopupCategory::DisplayRecordRepostion)
// ---------------------------------------------------------------------------

export function MainSubReposition(strFormTag, Main_Position, Seleted_Tab, strSelectedTabTag) {
  const dev = _devInterfaceObj;
  const { FormGetField, setScreenDisableBtn } = dev;
  if (!FormGetField || !setScreenDisableBtn) return;

  // Only apply lock logic on sub-tab navigation (C++: if(strSubFormTag != ""))
  if (!strSelectedTabTag || strSelectedTabTag === '') return;

  const activityStatus = FormGetField(TABLE_PLTNT, 'ActivityStatusHolder', 'scr');
  const n = parseInt(String(activityStatus ?? '').trim(), 10);
  if (Number.isFinite(n) && n >= 5) {
    setScreenDisableBtn(true, true, true);
  } else {
    setScreenDisableBtn(false, false, false);
  }
}

// ---------------------------------------------------------------------------
// ShowScreen
// ---------------------------------------------------------------------------

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  _devInterfaceObj = devInterfaceObj || _devInterfaceObj;
  setScreenDisableBtn(false, false, false);
}
