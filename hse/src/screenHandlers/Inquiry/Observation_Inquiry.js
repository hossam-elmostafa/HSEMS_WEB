/**
 * Screen handler: Observation Inquiry (HSE_TgNrstMiscInq)
 * Menu path: Inquiry screens -> Observation Inquiry (from HSE.json)
 * Delegates custom buttons to ObservationService and runs observation tab logic for toolbar events.
 * RQ_HSE_5_4_26_14_19 — Comments tab Source Screen = getScreenCaption.
 */

import {
  sendButtonClickToBackend,
  isObservationTabsEnabled,
  manageObservationTabs,
  manageCommentsTabToolBar,
  // RQ_HSE_5_4_26_14_19
  applyObservationCommentsSourceScreen,
} from '../../services/Observation service/ObservationService.js';
import { mapUnknownFieldTypes } from '../../utils/fieldTypeMapper.js';

const OBS_INQ_SCREEN = 'HSE_TgNrstMiscInq';
const OBS_FIELD = 'NRSTMISCENT_RECSTS';
const OBS_FILTERS = {
  NRSTMISCENT_VWINCMP: `WHERE (${OBS_FIELD}=1)`,
  NRSTMISCENT_VWRJCT: `WHERE (${OBS_FIELD}=3)`,
  NRSTMISCENT_VWCMPLT: `WHERE (${OBS_FIELD}=4)`,
  NRSTMISCENT_VWCNFRM: `WHERE (${OBS_FIELD}=5)`,
  NRSTMISCENT_VWCLSD: `WHERE (${OBS_FIELD}=6)`,
  NRSTMISCENT_VWALL: '',
};

export function ButtonClicked(eventObj) {
  const { Button_Name, strScrTag } = eventObj || {};
  const devInterface = eventObj.devInterfaceObj || {};
  const buttonName = Button_Name ? Button_Name.toString().toUpperCase() : '';
  const screenTag = strScrTag ? strScrTag.toString().toUpperCase() : '';
  const criteria = OBS_FILTERS[buttonName];
  if (criteria !== undefined && devInterface?.ChangeCriteria && devInterface?.refreshData) {
    devInterface.ChangeCriteria(OBS_INQ_SCREEN, '', criteria);
    devInterface.refreshData('');
    return;
  }
  sendButtonClickToBackend(buttonName, screenTag, eventObj, devInterface);
}

export async function toolBarButtonClicked(eventObj, callBackFn) {
  const devInterface = eventObj.devInterfaceObj || {};
  let strScrTag = (eventObj.strScrTag && eventObj.strScrTag.toString().toUpperCase()) || '';
  const strTabTag = (eventObj.strTabTag && eventObj.strTabTag.toString().toUpperCase()) || '';
  const strBtnName = (eventObj.strBtnName && eventObj.strBtnName.toString().toUpperCase()) || '';
  const complete = eventObj.complete;
  const completeStr = complete == null ? '' : String(complete).trim();
  const isPostPhase = complete === 1 || completeStr === '1';
  const isPrePhase =
    !isPostPhase &&
    (complete == null ||
      complete === '' ||
      complete === 0 ||
      complete === false ||
      completeStr === '0');

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'SAVE' && isPrePhase && strTabTag && strTabTag.includes('CMNTS') && eventObj.isNewMode) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, eventObj?.fullRecord);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Inquiry SAVE pre Comments SRCSCRN:', e);
  }

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
    console.warn('[Web_HSE] Observation Inquiry toolBarButtonClicked tab management:', error);
  }

  callBackFn(eventObj);

  // RQ_HSE_5_4_26_14_19
  try {
    if (strBtnName === 'NEW' && isPostPhase && strTabTag && strTabTag.includes('CMNTS')) {
      applyObservationCommentsSourceScreen(devInterface, strScrTag, strTabTag, undefined);
    }
  } catch (e) {
    console.warn('[Web_HSE] Observation Inquiry post-callback Comments SRCSCRN:', e);
  }
}

export async function ShowScreen(setScreenDisableBtn, strScrTag, strTabTag, devInterfaceObj) {
  const devInterface = devInterfaceObj || {};
  setScreenDisableBtn(false, false, false);

  try {
    await mapUnknownFieldTypes(strScrTag, strTabTag, devInterface);
  } catch (error) {
    console.warn('[Web_HSE] Observation Inquiry ShowScreen: error mapping unknown field types:', error);
  }

  if (!strTabTag || strTabTag === '') {
    try {
      const { executeSQLPromise, TabEnable } = devInterface;
      if (executeSQLPromise && TabEnable) {
        const tabsEnabled = await isObservationTabsEnabled(executeSQLPromise, devInterface.getValFromRecordSet);
        manageObservationTabs(strScrTag, tabsEnabled, TabEnable);
      }
    } catch (error) {
      console.warn('[Web_HSE] Observation Inquiry ShowScreen: error enabling observation tabs:', error);
    }
  }
}
