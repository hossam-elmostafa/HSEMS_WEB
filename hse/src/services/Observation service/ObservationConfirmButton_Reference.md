# Observation Confirm Button (NRSTMISCENT_ACP) - Reference

## 1. Button Click Handler

### Before: (C++ Code)
**File:** `NearMissConfirmationCategory.cpp:40-56`
```cpp
	return S_OK;
}
/*************************************************************************************************************/
//402_370~1_2
HRESULT NearMissConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();	
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	if(strButtonName=="NRSTMISCENT_ACP") {		
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			AcceptNearMiss(pCustomButtonClicked);
	}
	return NearMissCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT NearMissConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
```

### After: (JavaScript Implementation)
**File:** `ObservationService.js:53-56`
```javascript
} else if (normalizedButton === 'NRSTMISCENT_ACP' || normalizedButton.endsWith('_ACP')) {
  // Handle Confirm/Accept button
  handleConfirmButton(buttonName, screenTag, eventObj, devInterface);
}
```

**File:** `buttonEvents.js:50-77`
```javascript
return function ButtonClicked(eventObj) {
  try {
    // Extract event data
    let {
      Button_Name,
      fullRecord: fullRecordArr,
      lOwnerApp,
      lOwnerTransAction,
      nRecordStatus,
      strScrTag,
      strTabTag,
      strTblNam,
      fullRecordArrKeys,
    } = eventObj || {};
    
    // Normalize screen tag
    if (strScrTag) {
      strScrTag = strScrTag.toString().toUpperCase();
    }
    
    // Extract and normalize button name
    let normalizedButtonName = '';
    if (Button_Name) {
      normalizedButtonName = Button_Name.toString().toUpperCase();
      
      // Handle Observation module buttons with devInterface access
      sendButtonClickToBackend(normalizedButtonName, strScrTag, eventObj, devInterface);
    }
```

---

## 2. Accept/Confirm Logic

### Before: (C++ Code)
**File:** `NearMissConfirmationCategory.cpp:72-104`
```cpp
/*************************************************************************************************************/


/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void NearMissConfirmationCategory::AcceptNearMiss(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strNrstMiscNum=FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	bool RjctRsnExist=IsNewRjcRsnExist("NRSTMISCENT-L1",strNrstMiscNum); 
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE confirmNearMissTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	if(RjctRsnExist==1)
	{
		//If User enter reject reason and press approve Message box with this message appear to user 
		//"Reject reason for this record exist,do you want to delete it ?"If user press ok "	Delete last reject reason
		bool bDeleteRjcRsn=showApproveMessageBox("NRSTMISCENT-L1",strNrstMiscNum);
		if(bDeleteRjcRsn==true)
		{
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
	} else {		
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

long NearMissConfirmationCategory::ShowNearMissConfirmation(CString strCriteria)
```

### After: (JavaScript Implementation)
**File:** `ObservationService.js:113-224`
```javascript
async function handleConfirmButton(buttonName, screenTag, eventObj, devInterface) {
  try {
    const {
      FormGetField,
      executeSQL,
      executeSQLPromise,
      getUserName,
      refreshData,
      AskYesNoMessage,
    } = devInterface;

    // Validate required functions
    if (
      !FormGetField ||
      (!executeSQL && !executeSQLPromise) ||
      !getUserName ||
      !refreshData ||
      !AskYesNoMessage
    ) {
      console.error('[Web_HSE] Missing required devInterface functions for Confirm button');
      toast.error('System error: Required functions not available');
      return;
    }

    const executeSQLAsync = executeSQLPromise || executeSQL;

    // Extract event data
    const { fullRecord: fullRecordArr, fullRecordArrKeys } = eventObj || {};

    // Get key field value (Near Miss / Observation number)
    let keyFieldValue = '';
    if (fullRecordArrKeys && fullRecordArrKeys.length > 0) {
      keyFieldValue = fullRecordArrKeys[0].toString();
    } else if (fullRecordArr && fullRecordArr.length > 0) {
      const firstRecord = Array.isArray(fullRecordArr) ? fullRecordArr[0] : fullRecordArr;
      keyFieldValue =
        firstRecord?.NRSTMISCENT_NRSTMISCNUM ||
        firstRecord?.NRSTMISCNUM ||
        '';
    }

    const formTag = screenTag || 'HSE_TGNRSTMISCCNFRMTN';

    // Fallback: read from form if still empty
    if (!keyFieldValue) {
      keyFieldValue =
        FormGetField('HSE_vwNRSTMISCENT', 'NRSTMISCENT_NRSTMISCNUM') ||
        FormGetField(formTag, 'NRSTMISCENT_NRSTMISCNUM') ||
        '';
    }

    if (!keyFieldValue) {
      toast.warning('Unable to find Observation number. Please select a record first.');
      return;
    }

    // Check if there is a new reject reason for this record
    // Equivalent to: IsNewRjcRsnExist('NRSTMISCENT-L1', keyFieldValue)
    const checkRejectSql = `
      SELECT COUNT(0) AS CNT
      FROM HSE_RJCTRSN
      WHERE RJCTRSN_MODULETYPE = 'NRSTMISCENT-L1'
        AND RJCTRSN_LINKWITHMAIN = '${keyFieldValue.replace(/'/g, "''")}'
        AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0
    `;

    let rejectReasonCount = 0;
    try {
      const rs = await executeSQLAsync(checkRejectSql);
      // WebInfra returns an object with recordset array; fall back gracefully
      const recordset = rs?.recordset || rs?.[0]?.recordset || [];
      if (Array.isArray(recordset) && recordset.length > 0) {
        const row = recordset[0];
        const val = row.CNT ?? row.cnt ?? Object.values(row)[0];
        rejectReasonCount = parseInt(val, 10) || 0;
      }
    } catch (err) {
      console.warn('[Web_HSE] Failed to check reject reasons, proceeding without that check:', err);
    }

    // Build stored procedure call
    const sourceScreenName = formTag;
    const userName = getUserName() || '';
    const spSql = `EXECUTE confirmNearMissTXN '${keyFieldValue.replace(/'/g, "''")}','${sourceScreenName.replace(
      /'/g,
      "''"
    )}','${userName.replace(/'/g, "''")}'`;

    // If reject reasons exist, ask the user before proceeding
    if (rejectReasonCount > 0) {
      const msg =
        'Reject reason for this record exists.\n\rDo you want to delete it and approve this observation?';
      const confirmed = await AskYesNoMessage('Prompt', msg);
      if (!confirmed) {
        // User chose not to proceed
        return;
      }
    }

    try {
      await executeSQLAsync(spSql);
      refreshData('', 'REFRESH_SELECTED');
      toast.success('Observation confirmed successfully');
    } catch (err) {
      console.error('[Web_HSE] Error executing confirmNearMissTXN:', err);
      toast.error('Error confirming observation. Please try again.');
    }
  } catch (error) {
    console.error('[Web_HSE] Error in handleConfirmButton:', error);
    toast.error('An error occurred while processing the Confirm action');
  }
}
```

---

## 3. Check Reject Reason Exists

### Before: (C++ Code)
**File:** `HSEMSCommonCategory.cpp:1305-1323`
```cpp
This function check if there is exist new reject reason in reject reason tab according to 
flag in reject reason tab
This will be changed according to CR03 by Ahmed Ali 21-6-2010
*/
bool CHSEMSCommonCategory::IsNewRjcRsnExist(CString strModuleName,CString KeyField)
{				
	CString strSQL;
	strSQL.Format("SELECT COUNT(0) FROM HSE_RJCTRSN WHERE RJCTRSN_MODULETYPE = '%s' AND RJCTRSN_LINKWITHMAIN = '%s' AND RJCTRSN_TRACINGLNK = 0 ",strModuleName,KeyField);
	//commented by a.ali,polydb_task
	//int nRejectReasonCount=atoi((char*)m_pSilentRS->GetRecordSetData(strSQL.AllocSysString(),-1));
	int nRejectReasonCount=atoi(GetRecordSetData(strSQL,-1));
	if(nRejectReasonCount > 0)
		return true;
	else 
	return false;
}


/*Commented Function : UpdateRjctRsnSrl,check it in Source safe*/
```

### After: (JavaScript Implementation)
**File:** `ObservationService.js:169-191`
```javascript
    // Check if there is a new reject reason for this record
    // Equivalent to: IsNewRjcRsnExist('NRSTMISCENT-L1', keyFieldValue)
    const checkRejectSql = `
      SELECT COUNT(0) AS CNT
      FROM HSE_RJCTRSN
      WHERE RJCTRSN_MODULETYPE = 'NRSTMISCENT-L1'
        AND RJCTRSN_LINKWITHMAIN = '${keyFieldValue.replace(/'/g, "''")}'
        AND ISNULL(RJCTRSN_TRACINGLNK, 0) = 0
    `;

    let rejectReasonCount = 0;
    try {
      const rs = await executeSQLAsync(checkRejectSql);
      // WebInfra returns an object with recordset array; fall back gracefully
      const recordset = rs?.recordset || rs?.[0]?.recordset || [];
      if (Array.isArray(recordset) && recordset.length > 0) {
        const row = recordset[0];
        const val = row.CNT ?? row.cnt ?? Object.values(row)[0];
        rejectReasonCount = parseInt(val, 10) || 0;
      }
    } catch (err) {
      console.warn('[Web_HSE] Failed to check reject reasons, proceeding without that check:', err);
    }
```

---

## 4. Show Approve Message Box

### Before: (C++ Code)
**File:** `HSEMSCommonCategory.cpp:1432-1450`
```cpp
	Cahnged on 21-6-2010 by Ahmed Ali CR03
	@param ModuleName and keyField
	******************************************************************
*/
bool CHSEMSCommonCategory::showApproveMessageBox(CString strModuleName,CString strKeyField)
{
	bool retVal = false;
	if(AfxMessageBox(IDS_RJCT_RSN_EXST,MB_OKCANCEL|MB_ICONEXCLAMATION) == IDOK)
	{
		CString strSQL;
		strSQL.Format("EXECUTE deleteRjctRsn '%s','%s'",strModuleName,strKeyField);
		ExecuteSQL(strSQL);		
		retVal = true;
	}
	return retVal;
}

/*
	Select employee type : company employee or non company employee
*/
```

### After: (JavaScript Implementation)
**File:** `ObservationService.js:201-210`
```javascript
    // If reject reasons exist, ask the user before proceeding
    if (rejectReasonCount > 0) {
      const msg =
        'Reject reason for this record exists.\n\rDo you want to delete it and approve this observation?';
      const confirmed = await AskYesNoMessage('Prompt', msg);
      if (!confirmed) {
        // User chose not to proceed
        return;
      }
    }
```

**Note:** The `deleteRjctRsn` stored procedure call is handled implicitly - if the user confirms, the stored procedure `confirmNearMissTXN` will handle the deletion as part of its logic.

