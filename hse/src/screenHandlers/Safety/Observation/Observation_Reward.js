/**
 * Screen handler: Observation Reward (HSE_TgNrMisRward)
 * Menu path: Safety -> Observation -> Observation Reward (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
 */

import {
  sendButtonClickToBackend,
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
} from '../../../services/Observation service/ObservationService.js';
import { mapUnknownFieldTypes } from '../../../utils/fieldTypeMapper.js';

export function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag } = eventObj || {};
  const devInterface = eventObj.devInterfaceObj || {};
  const buttonName = Button_Name ? Button_Name.toString().toUpperCase() : '';
  const screenTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  sendButtonClickToBackend(buttonName, screenTag, eventObj, devInterface);
}

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  let strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const complete = eventObj.complete;

  try {
    if (complete === 1) {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
      if (strTabTag && strTabTag.includes('CMNTS')) {
        manageCommentsTabToolBar(strScrTag, strTabTag, devInterface);
      }
    }
  } catch (error) {
    console.warn('[Web_HSE] Observation Reward toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Reward ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Reward ShowScreen: error enabling observation tabs:', error);
    }
  }
}
