////CommonCategory.h
#ifndef _COMMONCATEGORY_H
#define _COMMONCATEGORY_H

class _DB;
class CCategory;

#define NEW_BUTTON		1
#define SAVE_BUTTON		2	
#define CANCEL_BUTTON	3
#define DELETE_BUTTON	4
#define NEXT_BUTTON		5
#define PREVIOUS_BUTTON	6
#define FIND_BUTTON		7
#define PRINT_BUTTON 	8
#define SORT_BUTTON		9
#define COPY_BUTTON		10
#define EXIT			11

#define UNKNOWN			-1
#define ONETOONE		1
#define ONETOMANY		2
#define BROWSABLE		3
#define ERROROPENTABLE	0
#define BASETOOLBAR			0
#define SUBTOOLBAR			1

#define NEW_BUTTONNAME		"NEW"
#define SAVE_BUTTONNAME		"SAVE"	
#define CANCEL_BUTTONNAME	"CANCEL"
#define DELETE_BUTTONNAME	"DELETE"
#define NEXT_BUTTONNAME		"NEXT"
#define PREVIOUS_BUTTONNAME	"PREVIOUS"
#define FIND_BUTTONNAME		"FIND"
#define PRINT_BUTTONNAME 	"PRINT"
#define SORT_BUTTONNAME		"SORT"
#define COPY_BUTTONNAME		"COPY"
#define EXITNAME			"EXIT"
#define COMMON_BUTTONNAME	"COMMON"
#define DETAILS_BUTTONNAME	"DETAILS"
#define SELECT_BUTTONNAME	"SELECT"
#define PASTE_BUTTONNAME	"PASTE"
#define ADVFILTER_BUTTONNAME "FILTERSCREEN"
#define EDIT_BUTTONNAME		"EDIT"
#define SELCURRENT_BUTTONNAME "SELECTCURRENT"
#define FILTER_BUTTONNAME	"FILTER"
#define REMFILTER_BUTTONNAME "REMOVEFILTER"
#define EDIT_BUTTONNAME		"EDIT"

enum
{
	HSEMSPASSW = 0,
	VCLTYPE
};

class CCommonCategory:public CToolsCategory
{
public:
	CCommonCategory(){m_nCurrentCard=-1;m_bModal=FALSE;m_bAllowAddNew=TRUE;m_bAllowDelete=TRUE;m_bAllowUpdate=TRUE;};
	CCommonCategory(CCategory *pCategory);
	virtual ~CCommonCategory();
	
	virtual void OnScreenReady(LPCTSTR strFormTag);
	virtual void OnBeforeListReady(LPCTSTR strFormTag);
	virtual void UpdateAfterBrowse(LPCTSTR Form_Tag, LPCTSTR Full_Recoed, LPCTSTR Table, LPCTSTR Return_Value,LPCTSTR Browsing_Field, LPCTSTR Criteria){};
	virtual void ConfirmToolBarButtonClicked(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubFormTag){m_bCancelOperation=FALSE;};
	virtual void ConfirmToolBarButtonClickedDone(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString subForm_Tag){m_bCancelOperation=FALSE;};
	virtual void FieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Main_Position, long Details_Position, short grid_edit_mode){};
//	virtual void EventSFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Record_Position, short grid_edit_mode){};
	virtual void OnTreeSelected(LPCTSTR Table_Name, LPCTSTR Field_Value){};
	virtual void ButtonClicked(LPCTSTR Caption, LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Master_Full_Recoed,LPCTSTR Details_Full_Recoe,LPCTSTR Field_Caption,long AbsolutePosition){};
    virtual void MainSubReposition(CString strFormTag, long Main_Position, short Seleted_Tab){};
	virtual void GridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record,LPCTSTR subForm_Tag){};
	virtual void OnBrowsableCustomButtonClicked(LPCTSTR Table, const VARIANT& Record, short CustomButtonID){};
	virtual void BeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record){};
	virtual void OnAddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID){};
	virtual void OnDeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID){};
	virtual void OnChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID){};
	virtual void OnUnloadPlan(BOOL bActionCompleted){};
	virtual void OnPlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID){};
	virtual void OnNodeSelected(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData){};
	virtual void OnNodeExpanded(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData){};
	virtual void OnNodeMenuPulled( LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID,LPCTSTR strData ){};
	virtual void OnNodeMenuItemClicked(LPCTSTR MenuID,LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData){};
	virtual void OnRangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2){};
	virtual void OnColDblClicked(LPCTSTR ColValue){};
	virtual void OnNodeDeselect (){};
	virtual void OnTreeTerminated (){};

	CString GetResourceCaption(int nID,BOOL bLang);

	CString GetSegment(CString strScreenTag);
	void	SetScreenDefauls();
	void	SetCurrentPos(long lAbsolutePosition);
	void	FillListRecordSet(LPDISPATCH	pActiveRecordset);
	void	ActivateListWindow();
	void DisableSubToolBarForLockedScreens(BOOL bDelete);
	long	ExecuteSQL(CString& strSqlStatement,UINT unConnectionDB=PDBRS_LOAD_FROM_GMS_DB,BOOL bIgnoreCommit = FALSE);
	//void	GetScreenData(CString &strFSA,CString &strScreenCaption,CString &strReportFile,BOOL &m_bOneToMany,CString &strUniqueFields,CString &strOrderFields,int &nHelpID,CString &strKeyField,BOOL &bShowEmptyList,CString &strStatusField,CString &strTrueTableName);
	void	GetScreenData(CString &strFSA,CString &strScreenCaption,CString &strReportFile,BOOL &m_bOneToMany,CString &strUniqueFields,CString &strOrderFields,int &nHelpID,CString &strKeyField,BOOL &bShowEmptyList,CString &strStatusField,CString &strTrueTableName,int &nMaxNoRecord);
	void	WaitCursor();
	BOOL	AskYesNoMessage(CString &strTitle,CString &strMessage);
	UINT	CompleteParentManage();
	CString GetTrueTableName();
	
	BOOL	IsModalScreen(){return m_bModalScreen;};
	CString GetFormTag(){return m_strFormTag;};
	COleDateTime GetGregDate(CString &strDate);
	int GetNextTXNLineNumbers(CString strTableName,CString strIndexField,CString strIndexValue,CString strItemFieldName,CString strOrderCriteria,CString strLineField,CString strRefField,CString strRefValue,BOOL bString=FALSE);

	CString GetOrderCriteria(CString &strTableName);
	CString GetLinkField(){return m_strKeyField;};
	virtual void OnScreenExit();
	void BringWindowToTop();

	CString PrepareValidation(CString Link_Name, CString Field_Name,CString Field_Attrib_Rules);
	

	//Begin general work for the HSEMS Module
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	ATOM IsScreenLoaded(CString strFormTag, bool bList);
	CString GenerateItem(CString strDesc,CString strUnit,CString strStore="",CString strMin="",CString strMax="",CString strROL="");
	CString ListShowEx(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, LPCTSTR strAttribTableName, long lHelpID=0);
	void	FormSetField(CString strTableName,CString Field,CString Value,int nFormType=-1);
	void	FormUpdateGrid(CString Table_Name, int nFormType=-1);
	void	FormSetFieldEx(CString strFormTag ,CString Field,CString Value,int nFormType=-1);
	void	FormDoAction(short Form_Type, short Button_Index);
	short	FormMakeRefresh(short Form_Type);
	long	ChangeFormSQL(CString strFullSQL,short Form_Type=-1);
	CString FormGetField(CString strTableName,CString Field,int nFormType=-1);
	CString FormGetFieldEx(CString strFormTag ,CString Field,int nFormType=-1);
	CString CCommonCategory::GetBrowsingSQL(CString TableName, LPCTSTR WhereClause);
	void HandleActionEx(short Form_Type, BOOL Cancel, short Selected_Records);
	short TabEnableEx(LPCTSTR strSubTag, BOOL State);
	void SetCurrentLanguage(BOOL bNewValue);
	void SubMakeRefresh();
	short GetFormType();
	short FormEnableButton(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, BOOL State, BOOL bForCurrTabOnly);
	void LockScreen(short Form_Type, BOOL bLockMain, BOOL bLockSubs);
	short LinkFieldsAttribSingle(LPCTSTR Link_Name, LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, 
		LPCTSTR Field_F_SQL, LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields);
	short FieldSetAttribSingle(LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, LPCTSTR Field_F_SQL, 
		LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields);
	CString ShowFilterScreen(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria);
	CString FormChangeCaption(short Form_Type, LPCTSTR strCaption);
		/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	CString GetSelectedRecordCriteria();
	void FindRecord(CString strCriteria);
	void UpdateTablesInfoCriteria(CString strFormTag, CString strSQL);
	CString GetTXNIndex(CString strTable,CString strRetField,CString strRefField,CString strRefValue,CString strWhereClause= "");
	CString GetUniqueFields()
	{
		return m_strUniqueFields;
	};
	CString	Find(CString strAllData,CString strField,CString strTable="");
	int GetCount(CString strTableName,CString strLinkField,CString strIndex,BOOL bString=FALSE,CString strAddSQL="");
	long GetRetVal(CString &strSQL);
	double GetRetDVal(CString &strSQL);
	CString FormatValueForSQL(CString strValue);
	BOOL CheckKey(CString &strTableName);
	
	CString GetValueFromSQL(CString strSQL,CString strField);
	CString GetPolicySettingValue(CString strSettingName);
	CString GetScreenCaption(){return m_strScreenCaption;}
	CString GetFieldAttributes(CString &strTableName,CString &strFieldName);
	void RefreshScreen();
	void MovePolydbForm();

	//End general work for the HSEMS Module
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	
	//---START---------Manage Synchronous Changes ---------By Nehad in 15 April 2014-----------------------
	void	SetStartedTXNStatus();
	CString ReadTXNStatus();
	BOOL	IsStatusChanged();
	BOOL	ValidateOnRecordStatus();
	CString GetStatusFieldName(){return m_strStatusField;}


	CString getScreenFldVal(CString strFldNam,CString strFormTag);

	UINT	ParentManage(LPCTSTR strTableName,BOOL bAllowUpdate =TRUE,BOOL bAllowAddNew =TRUE,BOOL bAllowDelete =TRUE,CString strTableNameForSql = "",LPCTSTR strSelectStatement= NULL,BOOL bModal=FALSE,LPCTSTR report_file = NULL,int nMaxNoRecord =0,int nCustomButtons=0,CString strToolTips="",BOOL bIgnoreFilterScreen=FALSE);


	protected:
		CString				m_strStatusVal,m_strStatusField;
	//---END---------Manage Synchronous Changes ---------By Nehad in 15 April 2014-----------------------

protected:
	BOOL	Validate();
//	COleDateTime FromStringToDate(CString &StringDate);
	short	GridShow(LPCTSTR title, BOOL one_to_many, BOOL AllowUpdate, BOOL AllowAdd, BOOL AllowDelete, BOOL lang, LPSTR strTableName,int nOpenState,int nHelpID);
	void	GetTablesNames(LPCTSTR strTableName,CString &strFSA,CString &strLSA,CString &strLFA,CString &strCaptTag);
	CString GetDbPath();
	BOOL	AskForRelatedTables(LPCTSTR TableName, LPCTSTR Field_name,LPCTSTR Field_Data);
	void	DeleteFromRelatedTables(CString strTableName,CString strFullRecord);
	void	SetModalScreen(BOOL bState)	{m_bModalScreen=bState;};

	CString GetCurrentDate(CString &strFormat);
	CString GeneralizeDate(CString strCARDateInitial);
	
public:
	CString				m_strMenuTitle,
						m_strWhereCriteria;	//a.ali
	BOOL				m_bCancelOperation,
						m_bSubWindowScreen,
						m_bOneToMany,
						m_bDialogMode;
	int					m_nNumberOfUnDeletedRecords,
						m_nCurrentCard;
	static long			m_nShowPos,
						m_nStockCodingWay,		// WRONG TO BE PUT HERE
						m_nEmpSetting ;
	static CPolyDbRecordset	*m_FirstSysRecSet;

	BOOL				m_bMainSubBrowsing,
						m_bAddNewMode,
						m_bEditMode,m_bLanguage;

	BOOL				m_bHasListWindow;
	CString				m_strComesFrom;

protected:
	bool				m_bOpenList;	
					
	CCategory			*m_pCategory;
	_DB					*m_pHSEMSPolyDb;
	_DB					*m_pHSEMSSYSPolyDb;
							
	CString				m_strBuildStatement,
						m_strFormTag;

	BOOL				m_bModalScreen;
	CString				m_strFSA,
						m_strReportFileName,
						m_strScreenCaption,
						m_strKeyField,
						m_stringTableName,
						m_strUniqueFields,
						m_strOrderFields,
						m_strTableNameForSql,
						m_ScreenTgName;

	int					m_nMaxNoRecord,
						m_nScreenHelpID;
	BOOL				m_bModal,
						m_bAllowAddNew,
						m_bAllowUpdate,
						m_bAllowDelete;
	static LPDISPATCH	m_pActiveRecordset;
	int					m_nScreenId;
};

#endif
