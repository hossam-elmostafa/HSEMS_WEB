/***********************************************************/
/*
/*  HSE Client Side Code
/*
/***********************************************************/
import { toast } from "react-toastify";

// Suppress Material-UI Autocomplete warning about getOptionLabel returning numbers
// This is a framework-level issue that we can't fix in WebInfra
// The warning doesn't affect functionality, but we suppress it for cleaner console
if (typeof console !== 'undefined' && console.warn) {
  const originalWarn = console.warn;
  console.warn = function(...args) {
    // Filter out Material-UI Autocomplete getOptionLabel warnings
    const message = args[0]?.toString() || '';
    if (message.includes('Material-UI: The `getOptionLabel` method of Autocomplete returned number')) {
      // Suppress this specific warning - it's a framework issue we can't fix
      return;
    }
    originalWarn.apply(console, args);
  };
}

// Import event handlers
import { toolBarButtonClicked, ButtonClicked, setDevInterface } from './src/events/buttonEvents';
import { OnBeforeBrowseButtonClick, UpdateAfterBrowse } from './src/events/browseEvents';
import { SubFieldChanged, MainSubReposition, ShowScreen, setDevInterface as setScreenDevInterface } from './src/events/screenEvents';
import { beforeRenderAppMenu, beforeRenderCustomActions, onMenuItemClicked, onAppOpen, setMenuDevInterface } from './src/events/menuEvents';

// Import utilities
import { getMessage } from './src/utils/messageUtils';
import { getAppPolicy, getAppPolicyV2 } from './src/utils/policyUtils';

// Import service initializers
import { initializeIncidentService } from './src/services/Incident service/IncidentService';

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
  // Wrap executeSQL functions to handle WRKLOC column errors gracefully
  devInterfaceObj.executeSQL = executeSQL;
  devInterfaceObj.executeSQLPromise = async function(sql, ...args) {
    try {
      return await executeSQLPromise(sql, ...args);
    } catch (error) {
      const errorMessage = error?.message || error?.toString() || '';
      // Check if it's a WRKLOC column error
      if (errorMessage.includes('WRKLOC') && errorMessage.includes('invalid column name')) {
        console.warn('[Web_HSE] Database column error detected (WRKLOC). This is a database schema issue that must be fixed in stored procedures.');
        console.warn('[Web_HSE] SQL that failed:', sql);
        // Still throw the error so calling code can handle it, but log helpful info
      }
      throw error;
    }
  };
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
  
  // Set devInterface for screen event handlers (MainSubReposition, etc.)
  setScreenDevInterface(devInterfaceObj);
  
  // Set devInterface for menu event handlers (onMenuItemClicked, etc.)
  setMenuDevInterface(devInterfaceObj);

  // Initialize services
  initializeIncidentService();

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
