/**
 * Screen handler: Performance Measurement Follow-up (HSE_TgperfMsrmntflwup)
 * Menu path: Performance -> Performance Measure -> Performance Measurement Follow-up (from HSE.json)
 * C++: PerformanceMeasurementFollowUpCategory – PERFMSRMNTENT_CLSVST (closePrfrmncMsrmntTXN); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PERFMSRMNTENT_CLSVST') {
    const doToolbarAction = devInterfaceObj?.doToolbarAction;
    if (typeof doToolbarAction === 'function') doToolbarAction('SAVE', devInterfaceObj?.strFormTag || strScrTag, '');
    sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
