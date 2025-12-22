/***********************************************************/
/*
/*  HSE Client Side Code
/*
/***********************************************************/
import { toast } from "react-toastify";

// Import event handlers
import { toolBarButtonClicked, ButtonClicked, setDevInterface } from './src/events/buttonEvents';
import { OnBeforeBrowseButtonClick, UpdateAfterBrowse } from './src/events/browseEvents';
import { SubFieldChanged, MainSubReposition, ShowScreen } from './src/events/screenEvents';
import { beforeRenderAppMenu, beforeRenderCustomActions, onMenuItemClicked, onAppOpen } from './src/events/menuEvents';

// Import utilities
import { getMessage } from './src/utils/messageUtils';
import { getAppPolicy, getAppPolicyV2 } from './src/utils/policyUtils';

export default function useHSE(useDevInterfaceFun) {
  const {
    executeSQL,
    executeSQLPromise,
    ShowNotepad,
    openCustomDlg,
    getValFromRecordSet,
    getFldValFrmFullRec,
    getUserName,
    getUserFullName,
    openScr,
    LinkFieldsAttribSingle,
    getCurrentLang,
    LinkSetAttribSingle,
    FormEnableButton,
    showProgressbarFn,
    FormSetField,
    FormGetField,
    changeFldAttrb,
    getScrNxtSegment,
    getScrNxtSegmentPromise,
    refreshData,
    getCurrentFormatedDate,
    setScreenDisableBtn,    
    downloadReportFile,
    doToolbarAction,
    updateScrRow,
    setScrLockedAttrb,
    downloadServerFile,
    uploadEquipmentData,
    changeFldObj,
    IsScrHaveSegment,
    updateRecordsSegment,
    getCurrentUserObj,
    runToolsForWebFn,
    AskYesNoMessage,
    TabEnable,
    openAdvancedFilter,
    changeCustomButtonProperty
  } = useDevInterfaceFun();

  let devInterfaceObj = {};

  // Build devInterfaceObj with all functions from useDevInterfaceFun
  devInterfaceObj.executeSQL = executeSQL;
  devInterfaceObj.executeSQLPromise = executeSQLPromise;
  devInterfaceObj.ShowNotepad = ShowNotepad;
  devInterfaceObj.openCustomDlg = openCustomDlg;
  devInterfaceObj.getValFromRecordSet = getValFromRecordSet;
  devInterfaceObj.getFldValFrmFullRec = getFldValFrmFullRec;
  devInterfaceObj.getUserName = getUserName;
  devInterfaceObj.getUserFullName = getUserFullName;
  devInterfaceObj.openScr = openScr;
  devInterfaceObj.LinkFieldsAttribSingle = LinkFieldsAttribSingle;
  devInterfaceObj.getCurrentLang = getCurrentLang;
  devInterfaceObj.LinkSetAttribSingle = LinkSetAttribSingle;
  devInterfaceObj.FormEnableButton = FormEnableButton;
  devInterfaceObj.showProgressbarFn = showProgressbarFn;
  devInterfaceObj.FormSetField = FormSetField;
  devInterfaceObj.FormGetField = FormGetField;
  devInterfaceObj.changeFldAttrb = changeFldAttrb;
  devInterfaceObj.getScrNxtSegment = getScrNxtSegment;
  devInterfaceObj.getScrNxtSegmentPromise = getScrNxtSegmentPromise;
  devInterfaceObj.refreshData = refreshData;
  devInterfaceObj.getCurrentFormatedDate = getCurrentFormatedDate;
  devInterfaceObj.setScreenDisableBtn = setScreenDisableBtn;
  devInterfaceObj.downloadReportFile = downloadReportFile;
  devInterfaceObj.doToolbarAction = doToolbarAction;
  devInterfaceObj.updateScrRow = updateScrRow;
  devInterfaceObj.setScrLockedAttrb = setScrLockedAttrb;
  devInterfaceObj.downloadServerFile = downloadServerFile;
  devInterfaceObj.uploadEquipmentData = uploadEquipmentData;
  devInterfaceObj.changeFldObj = changeFldObj;
  devInterfaceObj.IsScrHaveSegment = IsScrHaveSegment;
  devInterfaceObj.updateRecordsSegment = updateRecordsSegment;
  devInterfaceObj.getCurrentUserObj = getCurrentUserObj;
  devInterfaceObj.getAppPolicy = (policyFldName, callBackFn) => getAppPolicy(executeSQLPromise, getValFromRecordSet, policyFldName, callBackFn);
  devInterfaceObj.getAppPolicyV2 = (policyFldName) => getAppPolicyV2(executeSQL, getValFromRecordSet, policyFldName);
  devInterfaceObj.getMessage = (key, paramArr = []) => getMessage(getCurrentLang, key, paramArr);
  devInterfaceObj.runToolsForWebFn = runToolsForWebFn;  
  devInterfaceObj.AskYesNoMessage = AskYesNoMessage;
  devInterfaceObj.TabEnable = TabEnable;
  devInterfaceObj.openAdvancedFilter = openAdvancedFilter;
  devInterfaceObj.changeCustomButtonProperty = changeCustomButtonProperty;

  // Set devInterface for ButtonClicked handler
  setDevInterface(devInterfaceObj);

  // Return all event handlers and devInterfaceObj
  return {
    toolBarButtonClicked,
    ButtonClicked,
    OnBeforeBrowseButtonClick,
    UpdateAfterBrowse,
    SubFieldChanged,
    MainSubReposition,
    ShowScreen: (strScrTag, strTabTag) => ShowScreen(setScreenDisableBtn, strScrTag, strTabTag),
    beforeRenderAppMenu,
    beforeRenderCustomActions,
    onMenuItemClicked,
    onAppOpen
  }
}
