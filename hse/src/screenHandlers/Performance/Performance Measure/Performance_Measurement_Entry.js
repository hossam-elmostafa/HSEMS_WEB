/**
 * Screen handler: Performance Measurement Entry (HSE_TgperfMsrmntEnt)
 * Menu path: Performance -> Performance Measure -> Performance Measurement Entry (from HSE.json)
 * C++: PerformanceMeasurementCategory – PERFMSRMNTENT_RJCT (reject/Reject popup), SHW_DTL (ShowDetails); delegate to backend.
 */

import { sendButtonClickToBackend } from '../../../services/Observation service/ObservationService.js';

export async function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag, devInterfaceObj } = eventObj || {};
  const btn = (Button_Name && String(Button_Name).toUpperCase()) || '';
  if (btn === 'PERFMSRMNTENT_RJCT' || btn === 'SHW_DTL') sendButtonClickToBackend(btn, strScrTag, eventObj, devInterfaceObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  setScreenDisableBtn(false, false, false);
}
