// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _DB wrapper class

class _DB : public COleDispatchDriver
{
public:
	_DB();
	~_DB();
	_DB(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DB(const _DB& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long BuildTree(LPCTSTR Table_Name, LPCTSTR Tree_Title, LPCTSTR Index, LPCTSTR Displayed_Field, LPCTSTR Return_Field, LPCTSTR Link_Table, LPCTSTR Link_Field, BOOL finish);
	long BuildTreeSingleTable(LPCTSTR Table_Name, LPCTSTR Form_Title, LPCTSTR Item_Key, LPCTSTR Parent_Key, LPCTSTR Displayed_Field, LPCTSTR Return_Field, short Style, LPCTSTR SelectedItemKey);
	void HandleAction(short Form_Type, BOOL Cancel, short Selected_Records, LPCTSTR Selection_Formula);
	long RecGetAbsolute();
	short FormSetField(LPCTSTR Table_Name, LPCTSTR Field_Name, LPCTSTR Field_Value, short Form_Type);
	short RecSetBOF();
	BOOL RecSetEOF();
	short RecUpdate();
	short RecClose();
	short PositionMaskSingle(short Relative_Row, short Relative_Column);
	long ReportPrint(LPCTSTR Report_File, LPCTSTR Report_Title, LPCTSTR Selection_Formula, LPCTSTR Formulas, long Window_State, BOOL bExport, long Parent_Handle);
	long ListShow(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, long Parent_Handle, LPCTSTR strAttribTableName, long lHelpID);
	BOOL GridSetFocus(LPCTSTR Field_Name, BOOL Cancel_Update);
	LPDISPATCH DBCloneSet();
	short ReportEnable(short Form_Type, LPCTSTR Report_File);
	CString FormGetField(LPCTSTR Table_Name, LPCTSTR Field_Name, short Form_Type);
	short FormUpdateGrid(LPCTSTR Table_Name, short Form_Type);
	CString FormCancelUpdate(LPCTSTR Table_Name, short Form_Type);
	short LinkFieldsAttribSingle(LPCTSTR Link_Name, LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, 
		LPCTSTR Field_F_SQL, LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields);
	short LinkFieldsAttrib(LPCTSTR Table_Name);
	short LinkSetAttribSingle(LPCTSTR LinkName, LPCTSTR LinkCaption, LPCTSTR LinkLocalField, LPCTSTR LinkOtherTable, LPCTSTR LinkOtherField, LPCTSTR LinkSQL, short LinkMaxRecs, LPCTSTR UniqueFields, long ArabicHelpContextID, 
		long EnglishHelpContextID, LPCTSTR strTabAttribute);
	short LinkSetAttrib(LPCTSTR Table_Name);
	short FieldSetAttribSingle(LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, LPCTSTR Field_F_SQL, 
		LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields);
	short FieldSetAttrib(LPCTSTR Table_Name);
	LPDISPATCH GridShow(LPCTSTR Title, BOOL AllowUpdate, BOOL AllowAdd, BOOL AllowDelete, LPCTSTR Form_Tag, long Parent_Handle, short Window_State, long StartingFromPosition, long HelpID);
	short RecAddNew();
	short RecFieldPut(LPCTSTR Field_Name, LPCTSTR Value);
	short RecEdit();
	short RecDelete();
	long DBExecSql(LPCTSTR SQL_Statement, short nFirstsysdbFlag, BOOL bIgnoreBeginCommit);
	CString RecFieldGet(LPCTSTR FieldName);
	long RecRefresh();
	short RecFindLast(LPCTSTR Where_Statement);
	short RecFindFirst(LPCTSTR Where_Statement);
	short RecMoveAbsolute(short Old_Position, long New_Position);
	long DBBuildSet(short Form_Style, LPCTSTR SQL_Statement, short Max_Records, LPCTSTR UniqueFields, LPDISPATCH ActiveRecordset, LPCTSTR strFormTag);
	long DBInit(short DB_Type, LPCTSTR strConnectionString, LPCTSTR DB_SystemName, short ApplicationCode, short UserIDNumber, LPCTSTR HelpFileName, LPCTSTR ODBCName, LPCTSTR strProfileName);
	short RecMovePrevious();
	short RecMoveNext();
	short RecMoveLast();
	short RecMoveFirst();
	short FormDoAction(short Form_Type, short Button_Index);
	short FormMakeRefresh(short Form_Type);
	short TabEnable(short TabIndex, BOOL State);
	BOOL GetCurrentLanguage();
	void SetCurrentLanguage(BOOL bNewValue);
	short FormEnableButton(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, BOOL State, BOOL bForCurrTabOnly);
	VARIANT ShowCustomizedInputBox(LPCTSTR MainTitle, LPCTSTR SubTitle, const VARIANT& ArrayOfProperties);
	long RecCount();
	CString GetBrowsingSQL(LPCTSTR TableName, LPCTSTR WhereClause);
	CString GetFieldCaption(LPCTSTR TableName, LPCTSTR FieldName);
	VARIANT GetBrowsingArray(LPCTSTR Table_Name, LPCTSTR Field_Name);
	VARIANT GetFieldAttributes(LPCTSTR Table_Name, LPCTSTR Field_Name);
	VARIANT GetTableAttributes(LPCTSTR Table_Name);
	BOOL CreateQuery(LPCTSTR Query_Name, LPCTSTR SQLStatement);
	LPDISPATCH GetListRecordset();
	BOOL FormFindFirst(short Form_Type, LPCTSTR WhereClause);
	short SubMakeRefresh();
	CString RecGetFullRecord();
	CString FormChangeCaption(short Form_Type, LPCTSTR strCaption);
	void CloseConnection();
	VARIANT CancelAction(short Form_Type, LPCTSTR strRecords);
	VARIANT CreateListFilter(LPCTSTR strFilterCaption, LPCTSTR strFilterCriteria, LPCTSTR strIcon);
	VARIANT SetCalendarType(BOOL bHijriState);
	CString GetGregDate(LPCTSTR strHijriDate);
	short RecFieldAppendChunk(LPCTSTR Field_Name, const VARIANT& Value);
	CString ShowFilterScreen(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria);
	void LockScreen(short Form_Type, BOOL bLockMain, BOOL bLockSubs);
	CString GetServerDate();
	long ChangeFormSQL(short Form_Type, LPCTSTR strFullSQL);
	CString GetFormSQL(short Form_Type);
	void RefreshMemoScreen(short Form_Type, LPCTSTR strMemoTag);
	short ChangeCustomButtonProperty(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, LPCTSTR strPropertyName, LPCTSTR strPropertyValue);
	long BringFormToTop(short Form_Type);
	short GetFormType();
	BOOL CheckTabsIntegrity(LPCTSTR strFormTag, LPCTSTR strKeyValue, BOOL bShowMessage);
	long DBInitEx(LPCTSTR strConnectionString);
	CString ShowFilterScreenEx(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria, BSTR* strReadableCriteria);
	short TabEnableEx(LPCTSTR strSubTag, BOOL State);
	long ShowLightReport(LPCTSTR Report_File, LPCTSTR Report_Title, LPCTSTR Selection_Formula, LPCTSTR Formulas, long Window_State, BOOL bExport, long Parent_Handle);
	long GetScreenHWnd(short Form_Type);
	VARIANT ShowMailSetting();
	VARIANT SendMail();
	CString ListShowEx(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, LPCTSTR strAttribTableName, long lHelpID);
	void HandleActionEx(short Form_Type, short Cancel, short Selected_Records);
	BOOL ValidateEmptyFields(LPCTSTR strFormTag, LPCTSTR strKeyField, LPCTSTR strKeyValue, LPCTSTR strMessage, BOOL bShowMessage);
	void SetProfileName(LPCTSTR strProfileName);
	void LockRecord(short nFormType, BOOL bMainArea, BOOL bLock);
	long SetWindowState(short Form_Type, long iWindowState);
	BOOL IsErrorFlagRaised();
	long ShowAutoMailSetupScreen(long lParentHandle);
	long SendMailByAction(LPCTSTR strFormTag, LPCTSTR strButtonID, LPCTSTR strCriteria, LPCTSTR strTableName);
	VARIANT CreateFilter(LPCTSTR strFormTag, LPCTSTR strFilterCaption, LPCTSTR strFilterCriteria, BOOL bAsDefault);
	void ShowMailWizard();
	void ShowMailServerSetting();
	CString SelectFieldToExport(LPCTSTR strFormTag, LPCTSTR strFormCaption, LPCTSTR strTableName, LPCTSTR strCriteria, LPCTSTR strSelectedFields, LPCTSTR strRect);
	CString ExportToExcel(LPCTSTR strFormTag, LPCTSTR strFormCaption, LPCTSTR strTableName, LPCTSTR strCriteria, LPCTSTR strSelectedFields, LPCTSTR strRect, BOOL bSilent, LPCTSTR strExportedFile);
	long ShowUserDefinedChecks();
	CString BuildSQLFromReadableCriteria(LPCTSTR strTableName, LPCTSTR strBaseCriteria, LPCTSTR strReadableCriteria, LPCTSTR strTrueTableName);
	long ShowTransactionTypes();
	long ShowSegmentsSystem(long lParentHandle);
	LPDISPATCH GetAppConnection();
	short FormSetFieldEx(LPCTSTR FormTag, LPCTSTR Field_Name, LPCTSTR Field_Value, short Form_Type);
	CString FormGetFieldEx(LPCTSTR FormTag, LPCTSTR Field_Name, short Form_Type);
	void MoveForm(short Form_Type, short nLeft, short nTop, short nWidth, short nHeight);
	long FindRecord(LPCTSTR strCriteria, short nFormType);
	long GetTXNDependencyInfo(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strFieldValue);

	long ShowAutomaticEmails(long lParentHandle);
	CString GetScreenCaption(LPCTSTR strScreenTag);
	short SetActiveTab(LPCTSTR strTabTag);
	VARIANT CancelUploadImage(LPCTSTR strFieldName);
	CString GetVersion();
	BOOL ValidateTabEmptyFields(LPCTSTR strFormTag, LPCTSTR strKeyField, LPCTSTR strKeyValue, LPCTSTR strTabFormTag, LPCTSTR strTabKeyField, LPCTSTR strTabKeyValue, LPCTSTR strMessage, BOOL bShowMessage);

};

/////////////////////////////////////////////////////////////////////////////
// __DB wrapper class

class __DB : public COleDispatchDriver
{
public:
	__DB() {}		// Calls COleDispatchDriver default constructor
	__DB(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	__DB(const __DB& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void MFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Main_Position, long Details_Position, short Grid_Edit_Mode, short EditMode_Area);
	void SFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Record_Position, short Grid_Edit_Mode);
	void SGFieldChanged(LPCTSTR Form_Tag, LPCTSTR Panel_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Record_Position);
	void BrowsingAddNewItem(LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Criteria, LPDISPATCH ListRecordset);
	void BrowsingSelectedItem(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Return_Value, LPCTSTR Browsing_Field, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset);
	void BrowsingEditItemSelected(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset);
	void BrowsingChangePosition(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Return_Value, LPCTSTR Criteria);
	void TreeNodeSelected(LPCTSTR Table_Name, LPCTSTR Field_Value);
	void UniqueConflict(LPCTSTR Form_Tag, LPCTSTR LinkName, LPCTSTR Field_Name, LPCTSTR Field_Value, short Grid_Edit_Mode);
	void CustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Field_Caption, LPCTSTR Master_Full_Record, const VARIANT& Details_Full_Record, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset);
	void ToolBarButtonClicked(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID, LPCTSTR strSelectedTabTag);
	void ToolBarButtonClickedDone(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID, LPCTSTR strSelectedTabTag);
	void MainSubReposition(LPCTSTR Form_Tag, long Main_Position, short Seleted_Tab, LPCTSTR strSelectedTabTag);
	void GridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record, LPCTSTR strSelectedTabTag);
	void BrowsableCustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Table, const VARIANT& Record, short CustomButtonID, LPCTSTR Criteria, LPDISPATCH ListRecordset);
	void BeforeBrowseButtonClick(LPCTSTR Form_Tag, LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTag, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record);
	void BrowsingToolBarButtonClicked(LPCTSTR List_Tag, LPCTSTR Button_Key, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, const VARIANT& Record, LPDISPATCH ListRecordset, BOOL bCustomButton);
	void KeyCheck(BOOL Dongle_Exist);
	void ScreenReady(LPCTSTR Form_Tag);
	void ListCommonDone(LPCTSTR List_Tag, const VARIANT& vAffectedRecords);
	void BeforeBrowsing(LPCTSTR strFormTag, LPCTSTR strFieldName, BSTR* strBrowsingField, BSTR* strBrowsingRetField, BSTR* strBrowsingSQL);
	void BrowseDuplicatedValue(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strBrowsingTable, LPCTSTR strBrowsingRetField, LPCTSTR strBrowsingValue, BOOL* bShowList);
	void FieldChangeDone(LPCTSTR Form_Tag, LPCTSTR TableName, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Main_Position, long Details_Position, short Grid_Edit_Mode, short EditMode_Area);
	void BeforeSendMail(LPCTSTR strFormTag, LPCTSTR strButtonID, const VARIANT& vRecordsArray, LPCTSTR strTableName, LPCTSTR strKeyField, BSTR* strOtherInfo);
	void SendReport(BSTR* strTo, BSTR* strCc, BSTR* strSubject, BSTR* strBody);
	void ReletedRecordSelected(LPCTSTR strFormTag, LPCTSTR strTableName, LPCTSTR strCriteria);
	long GetTXNDependencyInfo(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strFieldValue);
	void BrowsingAddNewItemEx(LPCTSTR Form_Tag, LPCTSTR BrowseForm_Tag, LPCTSTR Table_Name, LPCTSTR Criteria, LPDISPATCH ListRecordset);
	void CreateLikeInvoked(LPCTSTR strFormTag, LPCTSTR strTableName, LPCTSTR strKeyFieldValue);
};
