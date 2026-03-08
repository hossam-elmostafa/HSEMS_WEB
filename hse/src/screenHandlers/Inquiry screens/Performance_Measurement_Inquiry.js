/**
 * Screen handler: Performance Measurement Inquiry (HSE_TgperfMsrmntInq)
 * Menu path: Inquiry screens -> Performance Measurement Inquiry (from HSE.json)
 * C++: PerformanceMEasurementInquiry – SHW_DTL (ShowDetails), filter buttons by PERFMSRMNTENT_RECSTS.
 */

import { sendButtonClickToBackend } from '../../services/Observation service/ObservationService.js';

const SCREEN_TAG = 'HSE_TgperfMsrmntInq';
const FIELD = 'PERFMSRMNTENT_RECSTS';

const BUTTONS = {
  PERFMSRMNTENT_VWINCMPLT: `WHERE (${FIELD}=1)`,
  PERFMSRMNTENT_VWAPPRV: `WHERE (${FIELD}=5)`,
  PERFMSRMNTENT_VWCLSD: `WHERE (${FIELD}=6)`,
  PERFMSRMNTENT_VWCMPLT: `WHERE (${FIELD}=4)`,
  PERFMSRMNTENT_VWRJCT: `WHERE (${FIELD}=3)`,
  PERFMSRMNTENT_VWALL: '',
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
  if (btn === 'SHW_DTL') {
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
    return;
  }
  const criteria = BUTTONS[btn];
  if (criteria !== undefined) applyCriteria(devInterfaceObj, criteria);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
