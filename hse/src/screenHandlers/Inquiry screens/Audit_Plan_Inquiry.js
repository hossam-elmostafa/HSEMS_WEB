/**
 * Screen handler: Audit Plan Inquiry (HSE_TgAdtPlnInq)
 * Menu path: Inquiry screens -> Audit Plan Inquiry (from HSE.json)
 * C++: AuditInquiry – filter buttons by ADTPLNENT_ADTPLNSTS; AUDITORS, AUDITIES, ADTPLNENTACTVT_* delegate to backend.
 */

import { sendButtonClickToBackend } from '../../services/Observation service/ObservationService.js';

const SCREEN_TAG = 'HSE_TgAdtPlnInq';
const FIELD = 'ADTPLNENT_ADTPLNSTS';

const FILTER_BUTTONS = {
  ADTPLNENT_VWINCMPLT: `WHERE (${FIELD}=1)`,
  ADTPLNENT_VWRJCT: `WHERE (${FIELD}=3)`,
  ADTPLNENT_VWCMPLT: `WHERE (${FIELD}=4)`,
  ADTPLNENT_VWAPPRV: `WHERE (${FIELD}=5)`,
  ADTPLNENT_VWCLSD: `WHERE (${FIELD}=6)`,
  ADTPLNENT_VWALL: '',
};

function applyCriteria(devInterfaceObj, criteria) {
  const ChangeCriteria = devInterfaceObj?.ChangeCriteria;
  const refreshData = devInterfaceObj?.refreshData;
  if (typeof ChangeCriteria === 'function') ChangeCriteria(SCREEN_TAG, '', criteria);
  if (typeof refreshData === 'function') refreshData('');
}

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  const criteria = FILTER_BUTTONS[btn];
  if (criteria !== undefined) {
    applyCriteria(devInterfaceObj, criteria);
    return;
  }
  if (['AUDITORS', 'AUDITIES', 'ADTPLNENTACTVT_ADTCHKLSTFDBCK'].includes(btn)) {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
