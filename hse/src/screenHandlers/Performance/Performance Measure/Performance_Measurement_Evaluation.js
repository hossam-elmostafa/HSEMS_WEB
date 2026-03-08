/**
 * Screen handler: Performance Measurement Evaluation (HSE_TgperfMsrmntEval)
 * Menu path: Performance -> Performance Measure -> Performance Measurement Evaluation (from HSE.json)
 * C++: PerformanceMeasurementEvaluationCategory – PERFMSRMNTENT_ENTCMPLT (confirmPrfrmncMsrmntTXN); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PERFMSRMNTENT_ENTCMPLT') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
