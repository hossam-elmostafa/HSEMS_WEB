#include "stdafx.h"
#include "HSEMS.h"
#include "HSEObj.h"
#include "Listlisten.h"
#include "HSEMSApp.h"

#include "IO.h"
#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

#define	MAIN_NEW_NONEMPTY_RECORD_NAVIGATE	-1
#define	MAIN_NEW_RECORD						0
#define	GRID_FREE_NAVIGATE					0
#define	GRID_NEW_EMPTY_RECORD_NAVIGATE		1
#define	GRID_NEW_NONEMPTY_RECORD_NAVIGATE	2
#define	SILENTPOLYDBRECORDSET				4
#define	SILENTPOLYDBRECORDSETF				5
#define CM_SETPANTEXT						WM_USER+1000
#define	LIST_FORM							3
#define	UNLOAD								11
#define	BROWSE_FROM_DETAIL_TABLES			"" // ADD TABLES SEPARATED WITH |

BOOL bReturnFromEvent = FALSE;


BEGIN_EVENTSINK_MAP(CCategory, CCmdTarget)
    //{{AFX_EVENTSINK_MAP(CAboutDlg)
	ON_EVENT(CCategory, -1, 0x1  , OnEventMFieldChanged, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I2 VTS_I2)
	ON_EVENT(CCategory, -1, 0x2  , OnEventSFieldChanged, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I2)
	ON_EVENT(CCategory, -1, 0x3  , SGFieldChanged,VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)
	ON_EVENT(CCategory, -1, 0x4  , OnEventBrowsingAddNewItem, VTS_BSTR VTS_BSTR VTS_BSTR VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0x5  , OnBrowsingSelectedItem, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0x6  , OnBrowsingEditItemSelected, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0x7  , OnBrowsingChangePosition, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CCategory, -1, 0x8  , OnTreeNodeSelected, VTS_BSTR VTS_BSTR)
	ON_EVENT(CCategory, -1, 0xa  , OnCustomButtonClicked, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0xb  , OnEventToolBarButtonClicked,VTS_BSTR VTS_BSTR VTS_I2 VTS_VARIANT VTS_I2 VTS_BSTR)
	ON_EVENT(CCategory, -1, 0xc  , OnEventToolBarButtonClickedDone,VTS_BSTR VTS_BSTR VTS_I2 VTS_VARIANT VTS_I2 VTS_BSTR)
	ON_EVENT(CCategory, -1, 0xd  , OnEventMainSubReposition,VTS_BSTR VTS_I4 VTS_I2 VTS_BSTR)
	ON_EVENT(CCategory, -1, 0xe  , OnGridRowReposition,VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR VTS_BSTR)
	ON_EVENT(CCategory, -1, 0xf  , OnBrowsableCustomButtonClicked,VTS_BSTR VTS_BSTR VTS_VARIANT VTS_I2 VTS_BSTR VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0x10  , OnBeforeBrowseButtonClick,VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CCategory, -1, 0x11  ,OnBrowsingToolBarButtonClicked,VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_VARIANT VTS_DISPATCH VTS_BOOL)
	ON_EVENT(CCategory, -1, 1	  ,RangeNotify,VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CCategory, -1, 0x16 ,BrowseDuplicatedValue,VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_PBOOL)
	ON_EVENT(CCategory, -1, 0x13 ,OnScreenReady,VTS_BSTR)	
	ON_EVENT(CCategory, -1, 0x19 ,SendReport,VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR)
	ON_EVENT(CCategory, -1, 0x1a		 /* Event24 */,OnReletedRecordSelected ,VTS_BSTR VTS_BSTR VTS_BSTR)		
	ON_EVENT(CCategory, -1, 0x1b  , OnEventBrowsingAddNewItemEx, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_DISPATCH)
	ON_EVENT(CCategory, -1, 0x1e  ,OnCreateLikeInvoked,VTS_BSTR VTS_BSTR VTS_BSTR)		

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INTERFACE_IMPL(CCategory,EventSink)
CCategory::CCategory(CString DbPath, CWnd* pParentWnd,CHSEObj *pHSEobj,_DB *pHSEMSSYSPolyDb)
{
	m_bConnected=false; // BY AAZMY
	m_unCurMenuID = 0;
	m_rcFormRect.SetRect(0,0,0,0);
	m_pCommonCategory=NULL;
	m_pListlistener=NULL;
	m_pParentWnd=pParentWnd;
	m_pHSEMSPolyDb = new _DB;
	m_pHSEMSSYSPolyDb = pHSEMSSYSPolyDb;
	m_pObject=m_pHSEMSPolyDb->m_lpDispatch;
	m_strDbPath=DbPath;
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pHSEobj=pHSEobj;
	m_User= m_pHSEobj->m_User;
	m_strDB = m_pHSEobj->m_strDB;
	m_nEngine = m_pHSEobj->m_nCurrentDBEngine;
	m_strConnection = m_pHSEobj->m_strConnection;
	m_nSegLength= m_pHSEobj->m_nSegLength;
	m_strSystemPath=pHSEobj->GetSystemPath();

	EstablishConnectionWithEventSrc();
	m_pToolsListener = new CToolsListener();
	long RetVal=0;
	try
	{
		// BY AAZMY //////////////////
		if (!m_DaoRecSet.m_bConnected)
			return;
		//////////////////////////////

		RetVal = m_pHSEobj->DBInit(m_pHSEMSPolyDb,m_pToolsListener,FALSE);
		// BY AAZMY //////////////////
		if (RetVal == 1)
			m_bConnected = true;
		//////////////////////////////
	}

	catch(CException *e)
	{
		e->ReportError();
		delete m_pHSEMSPolyDb;
		m_pHSEMSPolyDb = NULL;
		return;
	}	
}

CCategory::CCategory(CCategory*pCategory)
{
	m_bConnected=false;	 // BY AAZMY
	m_pCommonCategory=NULL;
	m_pListlistener=NULL;
	m_pParentWnd=pCategory->m_pParentWnd;
	m_pHSEMSPolyDb = new _DB;
	m_pObject=m_pHSEMSPolyDb->m_lpDispatch;
	m_strDbPath=pCategory->m_strDbPath;	
	m_pHSEobj=pCategory->m_pHSEobj;
	m_strSystemPath=pCategory->m_strSystemPath;

	EstablishConnectionWithEventSrc();
	long RetVal =0;
	try
	{
		m_pToolsListener = new CToolsListener();
		RetVal = m_pHSEobj->DBInit(m_pHSEMSPolyDb,m_pToolsListener);
		// BY AAZMY
		if (RetVal == 1)
			m_bConnected = true;
		///////////////////////
	}
	catch (...)
	{
		delete m_pHSEMSPolyDb;
		m_pHSEMSPolyDb = NULL;
		return;
	}
}

CCategory::~CCategory()
{
	try
	{
		m_DaoRecSet.Close();
		EndConnectionWithEventSrc();
		if(m_pHSEMSPolyDb)
		{
			m_pHSEMSPolyDb->FormDoAction(-1,UNLOAD);
			delete m_pHSEMSPolyDb;
			m_pHSEMSPolyDb =NULL;
		}
		if(m_pCommonCategory)
		{
			ATOM ThisScreenAtom=FindAtom(m_pCommonCategory->GetFormTag());
			if(ThisScreenAtom)
				ATOM atomRetVal=DeleteAtom(ThisScreenAtom);
			delete m_pCommonCategory;
		}
		if(m_pToolsListener)
		{
			delete m_pToolsListener;
			m_pToolsListener = NULL;
		}
		m_pCommonCategory=NULL;
		m_pHSEobj->Remove(this);
	}
	catch(...)
	{
		return;
	}
}

bool GetModalState(CString strFormTag)
{
	//Bugs-3-2023.11
	//BUG_HM_2_8_23.02
	if(strFormTag=="HSE_TGCRRCTVACTNSFLWUP" || strFormTag=="HSE_TGCRRCTVACTNS" ||strFormTag=="HSE_TGCRRCTVACTNS_RVW" || strFormTag=="HSE_TgCrRvwInf" || strFormTag=="HSE_TGCRAPPRVLINF" || strFormTag=="HSE_TGCRRCTVACTNS_ACTENT")
		return true;
	return false;
}

CString CCategory::GetFirstSysDBPath()
{
	return m_strSystemPath;
}

_DB *CCategory::GetHSEMSPolyDB()
{
	return m_pHSEMSPolyDb;
}

_DB *CCategory::GetHSEMSSYSPolyDB()
{
	return m_pHSEMSSYSPolyDb;
}

long CCategory::GetParentHWnd()
{
	if(m_pParentWnd == NULL)
		return 0;


	try
	{
		HWND hWnd=m_pParentWnd->GetSafeHwnd();
		return (long)(DWORD)hWnd;
	}
	catch(...)
	{
		return m_pHSEobj->GetParent();
	}
}

void CCategory::SetPaneText(int nPos,char* pText,BOOL bState)
{
	UINT message=CM_SETPANTEXT;
	LPARAM lParam = (LPARAM)pText;
	m_pParentWnd->SendMessage( message, 0, lParam );
}


void CCategory::OnEventSFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Record_Position, short grid_edit_mode)
{	
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;

	CString strFormTag=m_pCommonCategory->GetFormTag();
		if(strFormTag=="")
		strFormTag=Form_Tag;
	CString strTag=strFormTag;
	strTag.MakeUpper();
	CString strFieldName=Field_Name;
	strFieldName.MakeUpper();
	CString strNewVal=New_Value;
	strNewVal.MakeUpper();

	if(m_pCommonCategory)
		m_pCommonCategory->FieldChanged(Form_Tag, Field_Name, old_value, New_Value, full_record, Record_Position, 0, grid_edit_mode);

//	if(m_pCommonCategory)
//		m_pCommonCategory->EventSFieldChanged(strTag,Field_Name,old_value,New_Value,full_record,Record_Position,grid_edit_mode);

	// Manage Synchronous Changes 
	if(!m_pCommonCategory->ValidateOnRecordStatus())
		return;
	//-------------------------------------------------
	if(grid_edit_mode ==GRID_NEW_NONEMPTY_RECORD_NAVIGATE)
		return;
	
	AdjustRelatedTables(strTag,Field_Name,old_value, New_Value,grid_edit_mode);	
}

void CCategory::AdjustRelatedTables(CString strTag,CString strField_Name,CString strOld_value, CString strNew_Value, short grid_edit_mode)
{
	if(strOld_value=="")
		return;
	if(AskForRelatedTables(strTag,strField_Name))
	{
	
		if(grid_edit_mode != GRID_NEW_NONEMPTY_RECORD_NAVIGATE)
		{
			int nRet=0;
			CString FeedBackSql;
			CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
			
			CString FSAString = "SELECT * FROM FSA WHERE BROWSINGTABLE =";
			CString LFAString = "SELECT * FROM LFA WHERE BROWSINGTABLE =";
			CString FullSqlString;
			FullSqlString.Format("%s '%s' AND DATAFIELDNAME <> BROWSINGRETFIELD AND APPLICATIONCODE = %ld AND DATAFIELDNAME <> BROWSINGRETFIELD",FSAString,strTag,APP_CODE);
			// strTag is made equal to the table name
			DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
			BOOL bCheckForRule=TRUE;
			BOOL bRecodeTable;
			if(!DaoRecset.IsEOF())
			{
				DaoRecset.MoveFirst();
				while(!DaoRecset.IsEOF())
				{
					CString TableName;
					CString strUpdateStatement;
					CString FieldName;
					DaoRecset.GetFieldValue("TableName",TableName);
					bRecodeTable = GetTableRecodingStatus(TableName);
					if(bRecodeTable)
					{
						DaoRecset.GetFieldValue("DataFieldName",FieldName);
						if(bCheckForRule)
						{						
							CString strValidationRule;
							strValidationRule.Format("SELECT %s FROM %s WHERE %s ='%s'",FieldName,TableName ,FieldName,strOld_value);
							CPolyDbRecordset DaoRecsetValidation;
							DaoRecsetValidation.Open(AFX_DAO_USE_DEFAULT_TYPE,strValidationRule);
							if(!DaoRecsetValidation.IsEOF())
							{
								bCheckForRule=FALSE;							
								if(m_pHSEobj->GetRecodingStatus())
									if(!IsArabic())
										nRet = MessageBox(NULL,"This field is related with other tables\nAre you sure you want to recoded it ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
									else
										nRet = MessageBox(NULL,"åÐÇ ÇáÍÞá ãÑÊÈØ ÈÈíÇäÇÊ ÃÎÑí .åá ÊÑÛÈ Ýí ÊÚÏíá ÔÇãá ááÞíãÉ¿","ÊäÈíå",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);

								else
								{
									if(!IsArabic())
										MessageBox(NULL,"This field is related with other tables\nYou can't recode it","Prompt",MB_OK | MB_ICONHAND |MB_TASKMODAL);
									else
										MessageBox(NULL,"åÐÇ ÇáÍÞá ãÑÊÈØ ÈÈíÇäÇÊ ÃÎÑí .åá ÊÑÛÈ Ýí ÊÚÏíá ÔÇãá ááÞíãÉ¿","ÊäÈíå",MB_YESNO |MB_RIGHT| MB_ICONEXCLAMATION |MB_TASKMODAL);
									nRet=IDNO;
								}
								if(nRet != IDYES)
								{
									_DB *m_PollyDb = GetHSEMSPolyDB();
									m_PollyDb->FormSetField(strTag,strField_Name,strOld_value,-1);
									return;
								}
								
								strUpdateStatement.Format("UPDATE %s SET %s = '%s' WHERE %s ='%s'",TableName ,FieldName,strNew_Value,FieldName,strOld_value);
								try
								{
									m_pCommonCategory->ExecuteSQL(strUpdateStatement);
								}
								catch(...)
								{
								}
								DaoRecsetValidation.Close();
								
							}
							else
								DaoRecsetValidation.Close();						
						}
					
					else
					{
						strUpdateStatement.Format("UPDATE %s SET %s = '%s' WHERE %s ='%s'",TableName ,FieldName,strNew_Value,FieldName,strOld_value);
						try
						{
							m_pCommonCategory->ExecuteSQL(strUpdateStatement);
						}
						catch(...)
						{
						}
					}
					}
				DaoRecset.MoveNext();
				}
			}

			FullSqlString.Format("%s '%s' AND APPLICATIONCODE = %ld AND DATAFIELDNAME <> BROWSINGRETFIELD",LFAString,strTag,APP_CODE);
			DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
			if(!DaoRecset.IsEOF())
			{
				DaoRecset.MoveFirst();
				while(!DaoRecset.IsEOF())
				{
					CString TableName;
					CString strUpdateStatement;
					CString FieldName;
					DaoRecset.GetFieldValue("LINKNAME",TableName);

					DaoRecset.GetFieldValue("DATAFIELDNAME",FieldName);
					if(bCheckForRule)
					{						
						CString strValidationRule;
						strValidationRule.Format("SELECT %s FROM %s WHERE %s ='%s'",FieldName,TableName ,FieldName,strOld_value);
						CPolyDbRecordset DaoRecsetValidation;
						DaoRecsetValidation.Open(AFX_DAO_USE_DEFAULT_TYPE,strValidationRule);
						if(!DaoRecsetValidation.IsEOF())
						{
							bCheckForRule=FALSE;
							DaoRecsetValidation.Close();
							if(m_pHSEobj->GetRecodingStatus())
								if(!IsArabic())
									nRet = MessageBox(NULL,"This field is related with other tables\nAre you sure you want to recoded it?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
								else
									nRet = MessageBox(NULL,"åÐÇ ÇáÍÞá ãÑÊÈØ ÈÈíÇäÇÊ ÃÎÑí .åá ÊÑÛÈ Ýí ÊÚÏíá ÔÇãá ááÞíãÉ¿","ÊäÈíå",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);							
							else
							{
								if(!IsArabic())
									MessageBox(NULL,"This field is related with other tables\nAre you sure you want to recoded it?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
								else
									MessageBox(NULL,"åÐÇ ÇáÍÞá ãÑÊÈØ ÈÈíÇäÇÊ ÃÎÑí .åá ÊÑÛÈ Ýí ÊÚÏíá ÔÇãá ááÞíãÉ¿","ÊäÈíå",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
								nRet=IDNO;
							}
							if(nRet != IDYES)
							{
								_DB *PollyDb = GetHSEMSPolyDB();
								PollyDb->FormSetField(strTag,strField_Name,strOld_value,-1);
								DaoRecset.Close();
								return;
							}


							strUpdateStatement.Format("UPDATE %s SET %s = '%s' WHERE %s ='%s'",TableName ,FieldName,strNew_Value,FieldName,strOld_value);
							try
							{
								m_pCommonCategory->ExecuteSQL(strUpdateStatement);
							}
							catch(...)
							{
							}
						}
						else
							DaoRecsetValidation.Close();						
					}
					else
					{
						strUpdateStatement.Format("UPDATE %s SET %s = '%s' WHERE %s ='%s'",TableName ,FieldName,strNew_Value,FieldName,strOld_value);
						try
						{
							m_pCommonCategory->ExecuteSQL(strUpdateStatement);
						}
						catch(...)
						{
						}
					}
					DaoRecset.MoveNext();
				}
			}								
			DaoRecset.Close();
		}
	}

}

void CCategory::OnEventMFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Main_Position, long Details_Position, short grid_edit_mode,int EditMode_Area)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	if(m_pCommonCategory)
		m_pCommonCategory->FieldChanged(Form_Tag, Field_Name, old_value, New_Value, full_record, Main_Position, Details_Position, grid_edit_mode);

	// Manage Synchronous Changes 
	if(!m_pCommonCategory->ValidateOnRecordStatus())
		return;
	//-------------------------------------------------
	if(Main_Position == MAIN_NEW_NONEMPTY_RECORD_NAVIGATE || Main_Position == MAIN_NEW_RECORD)
		return;

	AdjustRelatedTables(strFormTag,Field_Name,old_value, New_Value,grid_edit_mode);
}

void CCategory::OnEventBrowsingAddNewItemEx(LPCTSTR Form_Tag, LPCTSTR BrowseForm_Tag, LPCTSTR Table_Name, LPCTSTR Criteria,LPDISPATCH ListRecordset)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	m_pCommonCategory->FillListRecordSet(ListRecordset);
	if(CCommonCategory::m_nShowPos==-1)
	{
		try
		{
			m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);
		}
		catch(...)
		{
		}
		return;
	}
	// TO INVOKE ADD NEW IN POLYDB - AAZMY
	CCommonCategory::m_nShowPos = -1;
	if(m_pCommonCategory && m_pCommonCategory->m_bHasListWindow) {
		CString strFormTag=m_pCommonCategory->GetFormTag();
		if(strFormTag=="")
			strFormTag=Form_Tag;
		bool bModal = m_pCommonCategory->IsModalScreen();
		m_pHSEobj->DoFunction(m_pCommonCategory->m_nCurrentCard,strFormTag,ADD_NEW_MODE,bModal,Criteria,m_pCommonCategory->m_strComesFrom,m_pHSEobj->GetScreenDefauls(),"");
	}
	else
	{
		m_pHSEobj->DoFunction(m_pCommonCategory->m_nCurrentCard,BrowseForm_Tag,ADD_NEW_MODE,(m_pCommonCategory==NULL)?TRUE:m_pCommonCategory->IsModalScreen(),"","",m_pHSEobj->GetScreenDefauls());
	}
}

/**
 * 27/12/2020	
 * Desc : 
 *
 */
void CCategory::OnEventBrowsingAddNewItem(LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Criteria,LPDISPATCH ListRecordset)
{
	return;
}

void CCategory::SGFieldChanged(LPCTSTR Form_Tag, LPCTSTR Panel_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Record_Position)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	OnEventSFieldChanged(Form_Tag,Field_Name,Old_Value,New_Value,Full_Record,Record_Position,-1);
}

void CCategory::OnEventToolBarButtonClicked(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID,LPCTSTR subForm_Tag)
{	
	if(bReturnFromEvent)
		return;
	CString strButton_Name = Button_Name;
	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	CString strForm_Tag = strFormTag;
	CString strSubFormTag=subForm_Tag;
	strButton_Name.MakeUpper();
	if(strButton_Name=="NEXT" || strButton_Name=="PREVIOUS")
		return;

	if(strButton_Name == "DELETE" || strButton_Name == "SAVE")
	{
		// Manage Synchronous Changes 
		if(!m_pCommonCategory->ValidateOnRecordStatus())
			return;
		//-------------------------------------------------
	}
	if(strButton_Name=="DELETE")
	{

	}

	SAFEARRAY *pSAFEARRAY=V_ARRAY(&Full_Record);
	VARIANT* pVar=(VARIANT*)(pSAFEARRAY->pvData);

	int nLoops=0;
	nLoops = pSAFEARRAY->rgsabound->cElements;
	if(nLoops == 1)
		nLoops++;	
	m_pCommonCategory->m_bCancelOperation=TRUE;
	m_pCommonCategory->m_nNumberOfUnDeletedRecords = 0;
	if(!m_pCommonCategory)
		m_pCommonCategory->m_bCancelOperation=FALSE;
	int nLoop =0;	
	for(nLoop =0;nLoop < nLoops -1;nLoop++)
	{
		BSTR str=V_BSTR(&pVar[nLoop]);
		CString strFullRecord(str);
		if(m_pCommonCategory)
			m_pCommonCategory->ConfirmToolBarButtonClicked(strForm_Tag ,Seleted_Tab,strButton_Name ,strFullRecord,nLoop,Toolbar_ID,strSubFormTag);
	}
	if(m_pCommonCategory->m_bCancelOperation)
	{
		short nShort=0;
		m_pHSEMSPolyDb->HandleActionEx(-1,TRUE,nShort);
	}
	if(m_pCommonCategory->m_nNumberOfUnDeletedRecords > 0)
	{
		CString strMessge;
		if(!IsArabic())
		{
			strMessge.Format("Undeleted %d Record(s) Related With Other Tables",m_pCommonCategory->m_nNumberOfUnDeletedRecords);
			MessageBox(NULL,strMessge,"Prompt",MB_OK | MB_ICONEXCLAMATION |MB_TASKMODAL);
		}
		else
		{
			strMessge.Format("ÚÏÏ ÇáÓÌáÇÊ ÇáÛíÑ ãÍÐæÝÉ %d",m_pCommonCategory->m_nNumberOfUnDeletedRecords);
			MessageBox(NULL,strMessge,"ÊäÈíå",MB_OK| MB_RIGHT | MB_ICONEXCLAMATION |MB_TASKMODAL);
		}

	}
}

void CCategory::OnEventToolBarButtonClickedDone(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID,LPCTSTR subForm_Tag)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	CString strButton_Name = Button_Name;
	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	CString strForm_Tag = strFormTag;
	strButton_Name.MakeUpper();
	if(strButton_Name=="EXIT")
	{
		if(m_pCommonCategory)
			m_pCommonCategory->ConfirmToolBarButtonClickedDone(strForm_Tag ,Seleted_Tab,strButton_Name ,"",0,Toolbar_ID,subForm_Tag);
		if(m_pCommonCategory->m_bSubWindowScreen == TRUE)// One to one state or one to many without list
		{
			ATOM ThisScreenAtom=FindAtom(strForm_Tag);
			if(ThisScreenAtom)
			{
				ATOM atomRetVal=DeleteAtom(ThisScreenAtom);
				m_pCommonCategory->m_bSubWindowScreen = FALSE;
			}
			
			m_pCommonCategory->OnScreenExit();
			//	NEXT LINE ONLY MOVED HERE BY AAZMY, AALI
			m_pCommonCategory->ActivateListWindow();
			delete this;
		}
		else
		{
			m_pCommonCategory->m_bMainSubBrowsing=FALSE;
			m_pCommonCategory->m_bEditMode=TRUE;
		}
		return;
	}

	SAFEARRAY *pSAFEARRAY=V_ARRAY(&Full_Record);
	VARIANT* pVar=(VARIANT*)(pSAFEARRAY->pvData);

	int nLoops=0;
	nLoops = pSAFEARRAY->rgsabound->cElements;

	if(nLoops == 1)
		nLoops++;	
//	if(Toolbar_ID == 0)
	{	
		int nLoop =0;
		for(nLoop =0;nLoop < nLoops -1;nLoop++)
		{
			BSTR str=V_BSTR(&pVar[nLoop]);
			CString strFullRecord(str);
			if(m_pCommonCategory)
				m_pCommonCategory->ConfirmToolBarButtonClickedDone(strForm_Tag ,Seleted_Tab,strButton_Name ,strFullRecord,nLoop,Toolbar_ID,subForm_Tag);
		}
	}	
}

void CCategory::OnEventMainSubReposition(LPCTSTR Form_Tag, long Main_Position, short Seleted_Tab,LPCTSTR subForm_Tag)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	if(Main_Position==-1)
		return;

	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	strFormTag.MakeUpper();
	
	if(m_pCommonCategory)
		m_pCommonCategory->MainSubReposition(strFormTag, Main_Position ,Seleted_Tab);

}
	
	
void CCategory::OnBrowsingSelectedItem(LPCTSTR Form_Tag, LPCTSTR Full_Recoed, LPCTSTR Table, LPCTSTR Return_Value ,LPCTSTR pszBrowsingField , LPCTSTR Criteria,long AbsolutePosition,LPDISPATCH ListRecordset)
{
	m_pCommonCategory->WaitCursor();
	CString strForm_Tag = Form_Tag;
	CString strBrowseField = pszBrowsingField;
	if(bReturnFromEvent)
		return;
	
	m_pCommonCategory->FillListRecordSet(ListRecordset);

	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	if(m_pCommonCategory)
	{
		if(m_pCommonCategory->m_bHasListWindow)// && *Return_Value==0)
		{
			m_pCommonCategory->SetCurrentPos(AbsolutePosition);
			m_pCommonCategory->m_bMainSubBrowsing=TRUE;
			//Criteria was added to below line instead of "" on 3/12/21 by Hsm
			//DoFunction(m_pCommonCategory->m_nCurrentCard,strFormTag,EDIT_MODE,false,"",m_pCommonCategory->m_strComesFrom,m_pHSEobj->GetScreenDefauls(),CString(Return_Value));
			bool bModal=GetModalState(strFormTag);
			DoFunction(m_pCommonCategory->m_nCurrentCard,strFormTag,EDIT_MODE,bModal,Criteria,m_pCommonCategory->m_strComesFrom,m_pHSEobj->GetScreenDefauls(),CString(Return_Value));
			return;
		}
		m_pCommonCategory->UpdateAfterBrowse(strFormTag, Full_Recoed, Table, Return_Value,pszBrowsingField,Criteria);
		if(m_pCommonCategory->IsModalScreen())
			m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);
	}
}

void CCategory::OnBrowsingEditItemSelected(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table , LPCTSTR Criteria,long AbsolutePosition,LPDISPATCH ListRecordset)
{
	if(bReturnFromEvent)
		return;

	CString strNewBuild;
	CString strTable = Table;
	strTable.MakeUpper();
	strNewBuild.Format("SELECT * FROM %s ",Table);
	m_pCommonCategory->WaitCursor();
	m_pCommonCategory->SetCurrentPos(AbsolutePosition);
	m_pCommonCategory->FillListRecordSet(ListRecordset);
	switch(m_pCommonCategory->m_nCurrentCard)
	{
		default:
			Table = strTable;
			break;
	}
	m_pHSEobj->DoFunction(m_pCommonCategory->m_nCurrentCard,Table,EDIT_MODE,TRUE);
}

void CCategory::OnBrowsingChangePosition(LPCTSTR Form_Tag, LPCTSTR Full_Recoed, LPCTSTR Table, LPCTSTR Return_Value, LPCTSTR Criteria)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
}

void CCategory::OnTreeNodeSelected(LPCTSTR Table_Name, LPCTSTR Field_Value)
{
	m_pCommonCategory->WaitCursor();
}

void CCategory::OnBrowsableCustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Table, const VARIANT& Record, short CustomButtonID , LPCTSTR Criteria,LPDISPATCH ListRecordset)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	if(m_pCommonCategory)
		m_pCommonCategory->OnBrowsableCustomButtonClicked(strFormTag, Record, CustomButtonID);
}

void CCategory::OnBeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	CMenu* pMenu = m_pParentWnd->GetMenu();
	CString strID;
	CString strForm_Tag=Form_Tag;
	CString strFieldName =FieldName;
	CString strNewTable= BrowsingTable;
	CString strReturnValue=CellContent;
	CString strBrowsingSQL=BrowsingSQL;
	CString strLink_Tag = Link_Tag;
	m_pHSEobj->m_MapMenuIDTOTags.Lookup(strNewTable,strID);
	UINT ui = pMenu->GetMenuState(atol(strID.GetBuffer(NULL)),MF_BYCOMMAND);
	CString strName;
	pMenu->GetMenuString(atol(strID.GetBuffer(NULL)),strName,MF_BYCOMMAND);
	if(ui & MF_DISABLED != 0)
	{
		// DISABLE MODIFICATION IN BROWSE FROM DETAILS CASE
		CString strBrowseFromDetailsTables = BROWSE_FROM_DETAIL_TABLES;
		CString strBrowsingTable(BrowsingTable);
		if (strBrowseFromDetailsTables.Find(strBrowsingTable + "|") != -1)
		{
			m_pCommonCategory->FormEnableButton(BROWSABLE,BASETOOLBAR,NEW_BUTTONNAME,FALSE,FALSE);
			m_pCommonCategory->FormEnableButton(BROWSABLE,BASETOOLBAR,DELETE_BUTTONNAME,FALSE,FALSE);
			m_pCommonCategory->FormEnableButton(BROWSABLE,BASETOOLBAR,EDIT_BUTTONNAME,FALSE,FALSE);
			m_pCommonCategory->FormEnableButton(BROWSABLE,BASETOOLBAR,COMMON_BUTTONNAME,FALSE,FALSE);
		}
	}
	m_pCommonCategory->BeforeBrowseButtonClick(Form_Tag,Link_Tag, FieldName, CellContent, BrowsingTable, BrowsingSQL, BrowsingField, BrowsingRetField, Full_Record);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RQ-3-2021.4 
BOOL gbCustomButton=false;
BOOL bToolBarButton=false;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCategory::OnBrowsingToolBarButtonClicked(LPCTSTR List_Tag, LPCTSTR Button_Key, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, const VARIANT& Record, LPDISPATCH ListRecordset , BOOL bCustomButton)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	if(m_pListlistener)
		return;
	if(m_pCommonCategory==NULL)
		return;

	CString strFormTag=m_pCommonCategory->GetFormTag();
	CString strListFormTag=List_Tag;
	CString strTable_Name = Table_Name;
	CString strButton_Key=Button_Key;

	strTable_Name.MakeUpper();
	if(strFormTag=="")
		strFormTag=List_Tag;
	
	strButton_Key.MakeUpper();
	if(strButton_Key=="DELETE")
	{
		if(m_pCommonCategory)
		{
			if(Record.vt == VT_EMPTY)
				return;
			SAFEARRAY *pSAFEARRAY=V_ARRAY(&Record);
			VARIANT* pVar=(VARIANT*)(pSAFEARRAY->pvData);
			int nLoops=0;
			nLoops = pSAFEARRAY->rgsabound->cElements;
			if(nLoops == 1)
				nLoops++;	
			int nLoop =0;
			for(nLoop =0;nLoop < nLoops -1;nLoop++)
			{
				BSTR str;
				str=V_BSTR(&pVar[nLoop]);
				CString strFullRecord(str);
				m_pCommonCategory->ConfirmToolBarButtonClicked(strTable_Name,0,strButton_Key,strFullRecord,nLoop,0,"");
			}
			if(m_pCommonCategory->m_nNumberOfUnDeletedRecords > 0)
			{
				CString strMessge;
				if(!IsArabic())
				{
					strMessge.Format("Undeleted %d Record(s) Related With Other Tables",m_pCommonCategory->m_nNumberOfUnDeletedRecords);
					MessageBox(NULL,strMessge,"Prompt",MB_OK | MB_ICONEXCLAMATION |MB_TASKMODAL);
				}
				else
				{
					strMessge.Format("ÚÏÏ ÇáÓÌáÇÊ ÇáÛíÑ ãÍÐæÝÉ %d",m_pCommonCategory->m_nNumberOfUnDeletedRecords);
					MessageBox(NULL,strMessge,"ÊäÈíå",MB_OK| MB_RIGHT | MB_ICONEXCLAMATION |MB_TASKMODAL);
				}
				m_pCommonCategory->m_nNumberOfUnDeletedRecords = 0;

			}
		
		}
	}
	else if(strButton_Key=="EXIT")
	{
		if(m_pCommonCategory)
		{
			ATOM ThisScreenAtom=FindAtom(strListFormTag);
			if(ThisScreenAtom)
				ATOM atomRetVal=DeleteAtom(ThisScreenAtom);

			m_pCommonCategory->OnScreenExit();
			//TODO : Check below code usage			
			//if(!m_pCommonCategory->IsModalScreen())
			delete this;
			//Delete the current instance of category if parentmanage has finished		
		}
	}
	else //if(strButton_Key=="SELECT")
	{
		if (Record.vt == VT_EMPTY)
		{
			COleVariant Details_Full_Recoed;
			Details_Full_Recoed.vt = VT_EMPTY;
			if(strButton_Key=="EDIT")
				OnBrowsingEditItemSelected(	List_Tag, "", Table_Name, Criteria,AbsolutePosition,ListRecordset);
			else
				OnCustomButtonClicked(List_Tag, strButton_Key, "", "", Details_Full_Recoed, Table_Name, Criteria, AbsolutePosition, ListRecordset);
		}
		else
		{
			SAFEARRAY *pSAFEARRAY=V_ARRAY(&Record);
			VARIANT* pVar=(VARIANT*)(pSAFEARRAY->pvData);

			int nLoops=0;
			nLoops = pSAFEARRAY->rgsabound->cElements;

			if(nLoops == 1)
				nLoops++;	
			int nLoop =0;
			for(nLoop =0;nLoop < nLoops -1;nLoop++)
			{
				BSTR str=V_BSTR(&pVar[nLoop]);
				CString strFullRecord(str);

				COleVariant Details_Full_Recoed;
				Details_Full_Recoed.vt = VT_EMPTY;
				if(strButton_Key=="EDIT")
					OnBrowsingEditItemSelected(	List_Tag, strFullRecord, Table_Name, Criteria,AbsolutePosition,ListRecordset);
				else
				{
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//RQ-3-2021.4 
					bToolBarButton=true;
					gbCustomButton=bCustomButton;
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					OnCustomButtonClicked(List_Tag, strButton_Key, "", strFullRecord, Details_Full_Recoed, Table_Name, Criteria, AbsolutePosition, ListRecordset);
					bToolBarButton=false;
				}
			}
		}
	}

/*	else if(strButton_Key=="EDIT")
		OnBrowsingEditItemSelected(	List_Tag, "", Table_Name, Criteria,AbsolutePosition,ListRecordset);
	else
		OnCustomButtonClicked(List_Tag, strButton_Key, "", "",COleVariant(""), Table_Name, Criteria, AbsolutePosition, ListRecordset);
*/
}

void CCategory::RangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1_Text, LPCTSTR Mask2_Text)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
}
void CCategory::BrowseDuplicatedValue(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strBrowsingTable, LPCTSTR strBrowsingRetField, LPCTSTR strBrowsingValue, BOOL* bShowList)
{
	m_pCommonCategory->WaitCursor();
	CString strTag = strFormTag;
}
void CCategory::OnScreenReady(LPCTSTR FormTag)
{
	m_pCommonCategory->WaitCursor();
	m_pCommonCategory->OnScreenReady(FormTag);
	m_pCommonCategory->SetStartedTXNStatus();
}
void CCategory::SendReport(BSTR* strTo, BSTR* strCc, BSTR* strSubject, BSTR* strBody)
{

}
void CCategory::AddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)
{

}
void CCategory::DeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)
{

}
void CCategory::ChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID)
{

}

void CCategory::UnloadPlan(BOOL bActionCompleted)
{

}

void CCategory::PlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID)
{

}

void CCategory::NodeSelected(const VARIANT& TableName, LPCTSTR ReturnValue, long UserID)
{

}

void CCategory::RangeEnterd(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2)
{

}


BOOL CCategory::AskForRelatedTables(LPCTSTR TableName, LPCTSTR Field_name)
{
	BOOL bRetVal;
	CString strField_name = Field_name;
	if(strField_name =="")
		Field_name = GetTableKey(TableName);
	CString FeedBackSql;
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	CString FSAString = "SELECT * FROM FSA WHERE BROWSINGTABLE =";
	CString LFAString = "SELECT * FROM LFA WHERE BROWSINGTABLE=";
	CString FullSqlString;
	
	FullSqlString.Format("%s '%s' AND BROWSINGRETFIELD ='%s' AND APPLICATIONCODE =%ld AND DATAFIELDNAME <> BROWSINGRETFIELD",FSAString,TableName,Field_name,APP_CODE);
	DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
	if(DaoRecset.IsEOF())
	{
		FullSqlString.Format("%s '%s' AND BROWSINGRETFIELD='%s' AND APPLICATIONCODE =%ld AND DATAFIELDNAME <> BROWSINGRETFIELD",LFAString,TableName,Field_name,APP_CODE);
		DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
		if(DaoRecset.IsEOF())
			bRetVal=FALSE;
		else
			bRetVal=TRUE;			
	}
	else
		bRetVal=TRUE;
	DaoRecset.Close();
	return bRetVal;
}


CString CCategory::GetDbPath()
{
	return m_strDbPath;
}

CString CCategory::GetScreenDefauls()
{
	return m_pHSEobj->GetScreenDefauls();
}

//a.ali 02/05/2021
void CCategory::SetScreenDefauls(CString strScreenDefaults)
{
	m_pHSEobj->SetScreenDefauls(strScreenDefaults);
}

BOOL CCategory::IsModalPolyDb()
{
	return m_pHSEobj->IsModalPolyDb();
}

void CCategory::SetModalPollyDB(BOOL bStatus)
{
	m_pHSEobj->SetModalPollyDB(bStatus);
}

void CCategory::OnCustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Field_Caption, LPCTSTR Master_Full_Recoed,const VARIANT& Details_Full_Recoed, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset)
{
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	LPCTSTR FieldName =Field_Name;
	CString strForm_Tag =Form_Tag;
	CString strCaption =FieldName;
	strCaption.MakeUpper();
	CString strMessage;
	
	// get the first record in the array
	CString strDetailFullRecord="";

	if (Details_Full_Recoed.vt != VT_EMPTY && Details_Full_Recoed.vt != VT_DISPATCH)
	{
		SAFEARRAY *pSAFEARRAY=V_ARRAY(&Details_Full_Recoed);
		VARIANT* pVar=(VARIANT*)(pSAFEARRAY->pvData);
		
		long nLoop = pSAFEARRAY->rgsabound->cElements;
		if (nLoop > 0)
		{
			BSTR str=V_BSTR(&pVar[0]);
			strDetailFullRecord = CString(str);
		}
	}

	if(strCaption != "REFRESH" && strCaption != "SELECT")
	{
		
		// Manage Synchronous Changes 
		if(!m_pCommonCategory->ValidateOnRecordStatus())
			return;
		//-------------------------------------------------
	}
	if(strCaption.Find("CUSTVIEW_") ==0)
	{
		HandleCustomButton(m_pCommonCategory,Form_Tag, Field_Name, Field_Caption, Master_Full_Recoed,Details_Full_Recoed.bstrVal, Table_Name, Criteria);
	}

	if(m_pCommonCategory)
	{
		m_pCommonCategory->WaitCursor();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//RQ-3-2021.4 
		if(!bToolBarButton)
			gbCustomButton=TRUE;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pCommonCategory->ButtonClicked(FieldName , Form_Tag, Table_Name,Master_Full_Recoed,strDetailFullRecord,Field_Caption,AbsolutePosition);
	}
}

CString	CCategory::Find(CString strAllData,CString strField)
{
	int nPos=strAllData.Find(strField);
	if(nPos<0)
		return "";
	CString strTemp=strAllData.GetBuffer(NULL)+nPos;
	CString strRetVal=strTemp.GetBuffer(NULL)+strField.GetLength()+1;
	char *pNull=strchr(strRetVal.GetBuffer(NULL),',');
	if(pNull)
		*pNull=0;
	return strRetVal;
}

void CCategory::OnGridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record,LPCTSTR subForm_Tag)
{	
	m_pCommonCategory->WaitCursor();
	if(bReturnFromEvent)
		return;
	CString strFormTag=m_pCommonCategory->GetFormTag();
	if(strFormTag=="")
		strFormTag=Form_Tag;
	
	if(m_pCommonCategory)
	{
		m_pCommonCategory->GridRowReposition(strFormTag, Selected_Tab, AbsolutePosition, Full_Record,subForm_Tag);
	}
	
	
}

int CCategory::DoFunction(int nCurrentCard,CString strFunction,int nStatus,BOOL bModal,CString strCriteria,CString strComesFrom,CString strScreenDefaults,CString strReturnBrowseValue)
{
	return m_pHSEobj->DoFunction(nCurrentCard,strFunction,nStatus,bModal,strCriteria,strComesFrom,strScreenDefaults,strReturnBrowseValue);
}

BOOL CCategory::IsArabic()
{
	return m_pHSEobj->IsArabic();
}

BOOL CCategory::IsHijri()
{
	return m_pHSEobj->IsHijri();
}
CWnd* CCategory::GetParentWnd()
{
	return m_pParentWnd;
}
long CCategory::GetParentWndAsLong()
{
	return (long)m_pParentWnd->GetParent();//m_pHSEobj->GetParent();
}


int CCategory::GetDisplayStatus()
{
	return m_pHSEobj->GetDisplayStatus();
}

long CCategory::ExecuteSQL(CString &strSqlStatement,UINT unConnectionDB,BOOL bIgnoreCommit)
{
	m_pCommonCategory->WaitCursor();
	long lRetFieldsCount=0;
	try
	{
		lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql(strSqlStatement,unConnectionDB,FALSE);
	}
	catch(...)
	{
		lRetFieldsCount = 0;
	}
	return lRetFieldsCount;
}

BOOL CCategory::GetTableRecodingStatus(CString &strTableName)
{
	CString strSQL;
	BOOL bRetVal;
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	strSQL.Format("SELECT TRUETABLENAME FROM TABLESINFO WHERE TABLENAME = '%s' AND APPLICATIONCODE = %ld AND TABLENAME = TRUETABLENAME \
					UNION SELECT DISTINCT LINKNAME FROM LSA WHERE (LinkOtherTable <> '' OR LinkOtherTable IS NOT NULL) AND LINKNAME = LinkOtherTable AND LINKNAME = '%s' AND ApplicationCode = %ld",strTableName,APP_CODE,strTableName,APP_CODE);
	long lRetVal = DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecset.Close();
	if(lRetVal <= 0)
		bRetVal = FALSE;
	else
		bRetVal = TRUE;
	return bRetVal;
}

//by a.ali to get Where Clause from tablesinfo table
CString CCategory::GetTableWhereCondition(CString &strTableName)
{
	CString strSQL;
	CString strSQLCriteria;
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	strSQL.Format("SELECT WHERECLAUSE FROM TABLESINFO WHERE TABLENAME = '%s' AND APPLICATIONCODE = %ld",strTableName,APP_CODE);
	DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if(DaoRecset.IsEOF())
	{
		DaoRecset.Close();
		return "";
	}
	DaoRecset.MoveFirst();
	DaoRecset.GetFieldValue("WHERECLAUSE",strSQLCriteria);
	DaoRecset.Close();
	return strSQLCriteria;
}





//////////////// CPolyDb Recordset

INTERFACE_IMPL(CPolyDbRecordset,EventSink)
CPolyDbRecordset::CPolyDbRecordset(UINT unConnectionDB)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb=new _DB;
	extern CHSEMSApp theApp;
	CString strDPath=theApp.GetHSEObj()->GetDataPath();
	CString strSystemPath=theApp.GetHSEObj()->GetSystemPath();
	int nSystemDBType=theApp.GetHSEObj()->GetSystemDBType();

	m_bConnected = false;  // BY AAZMY
	switch(unConnectionDB)
	{
	case PDBRS_LOAD_FROM_GMS_DB:
	{
		long lRetVal = CHSEObj::DBInit(m_pPolyDb,NULL,FALSE);
		m_bDataDB=TRUE;
		// BY AAZMY
		if (lRetVal == 1)
			m_bConnected = true;
		////////////////////////////////
		break;
	}
	case PDBRS_LOAD_FROM_SYS_DB:
		{
			CString strFormat;
//			strFormat.Format("Provider=Microsoft.Jet.OLEDB.4.0;Persist Security Info=False;Data Source=%s;Jet OLEDB:Database Password=First2000",strSystemPath);
			long lRetVal = m_pPolyDb->DBInit(nSystemDBType,strSystemPath,strSystemPath,APP_CODE,theApp.GetHSEObj()->GetUserID(),"",APP_ODBC,"");
			// BY AAZMY
			if (lRetVal == 1)
				m_bConnected = true;
			////////////////////////////////

			m_bDataDB=FALSE;

		}
		break;
	}
}

CPolyDbRecordset::~CPolyDbRecordset()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(m_pPolyDb)
	{
		try
		{
			delete m_pPolyDb;
			m_pPolyDb =NULL;
		}
		catch(...)
		{}
	}
}

long CPolyDbRecordset::Open(int,CString &strSql)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_bEOF=TRUE;
	long lRetVal=0;
	if(m_pPolyDb==NULL)
	{
		m_pPolyDb=new _DB;
		extern CHSEMSApp theApp;
		CString strDPath=theApp.GetHSEObj()->GetDataPath();
		CString strSystemPath=theApp.GetHSEObj()->GetSystemPath();
		CHSEObj::DBInit(m_pPolyDb,NULL);
	}
	try
	{		
		if(m_bDataDB)
			lRetVal = m_pPolyDb->DBBuildSet(SILENTPOLYDBRECORDSET,strSql.GetBuffer(NULL),1,"",NULL,"");
		else
			lRetVal = m_pPolyDb->DBBuildSet(SILENTPOLYDBRECORDSETF,strSql.GetBuffer(NULL),1,"",NULL,"");

		if(lRetVal>0)
		{
			MoveLast();
			MoveFirst();
		}
	}
	catch(...)
	{
	}
	return lRetVal;
}

BOOL CPolyDbRecordset::IsEOF()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(m_bEOF)
		return m_bEOF;
	return (m_pPolyDb->RecSetEOF()==FALSE)?FALSE:TRUE;
}

void CPolyDbRecordset::Close()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	delete m_pPolyDb;
	m_pPolyDb=NULL;
}

void CPolyDbRecordset::GetFieldValue(CString strFieldName,CString &strRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
}

void CPolyDbRecordset::GetFieldValue(CString strFieldName,int &nRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
	strRetValue.MakeUpper();
	if(strRetValue=="" || strRetValue =="FALSE")
		nRetValue=0;
	else if(strRetValue=="TRUE") 
		nRetValue=1;
	else
		nRetValue=atoi(strRetValue);
}

void CPolyDbRecordset::GetFieldValue(CString strFieldName,long &lRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
	if(strRetValue=="")
		lRetValue=0;
	lRetValue=atol(strRetValue);
}
void CPolyDbRecordset::GetFieldValue(CString strFieldName,float &fRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
	if(strRetValue=="")
		fRetValue=0;
	fRetValue=atof(strRetValue);
}

void CPolyDbRecordset::GetFieldValue(CString strFieldName,COleDateTime &OleDateTimeRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
	OleDateTimeRetValue.ParseDateTime(strRetValue);
}

void CPolyDbRecordset::GetFieldValue(CString strFieldName,double &dRetValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strRetValue = m_pPolyDb->RecFieldGet(strFieldName);
	if(strRetValue=="")
		dRetValue=0;
	dRetValue=atof(strRetValue);
}

void CPolyDbRecordset::Move(long lRows)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecMoveAbsolute(-1,lRows);
}

CString CPolyDbRecordset::GetFullRecord(CString strSeparator)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strFullRecord = m_pPolyDb->RecGetFullRecord();

	if (strSeparator != "|")
		strFullRecord.Replace("|",strSeparator);

	return strFullRecord;
}

void CPolyDbRecordset::MoveFirst()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecMoveFirst();
	m_bEOF=FALSE;
}

void CPolyDbRecordset::MoveNext()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecMoveNext();
}

void CPolyDbRecordset::MoveLast()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecMoveLast();
}

void CPolyDbRecordset::AddNew()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecAddNew();
}

void CPolyDbRecordset::Edit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecEdit();
}

void CPolyDbRecordset::Update()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecUpdate();
}

void CPolyDbRecordset::SetFieldValue(LPCTSTR lpszName, LPCTSTR lpszValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecFieldPut(lpszName, lpszValue);
}

BOOL CPolyDbRecordset::Find(long lFindType, LPCTSTR lpszFilter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	short shRetVal=-1;
	switch(lFindType)
	{
	case AFX_DAO_FIRST:
		shRetVal=m_pPolyDb->RecFindFirst(lpszFilter);
		break;
	case AFX_DAO_LAST:
		shRetVal=m_pPolyDb->RecFindLast(lpszFilter);
		break;
	}
	return shRetVal==1;
}

long CPolyDbRecordset::GetAbsolutePosition()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return m_pPolyDb->RecGetAbsolute();
}

void CPolyDbRecordset::SetAbsolutePosition(long lNewPos)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pPolyDb->RecMoveAbsolute(-1, lNewPos);
}

long CPolyDbRecordset::GetRecordCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	MoveLast();
	return GetAbsolutePosition() + 1;
}


short CPolyDbRecordset::Delete()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	return m_pPolyDb->RecDelete();
}


CString CCategory::GetUserLogin()
{
	return m_pHSEobj->m_strLoginUserName;
}

CString CCategory::GetWorkingFolder()
{
	return m_pHSEobj->GetWorkingFolder();
}
CString CCategory::GetUserFullName()
{
	return m_pHSEobj->m_strUserFullName;
}


CString CCategory::GetTableKey(CString TableName)
{
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	CString strKeyField;
	CString strSQL;
	strSQL.Format("SELECT DISTINCT ORDERBYCLAUSE FROM TABLESINFO WHERE TRUETABLENAME ='%s' AND APPLICATIONCODE =3 AND ORDERBYCLAUSE IS NOT NULL",TableName);
	DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);	
	DaoRecset.MoveFirst();
	if(!DaoRecset.IsEOF())
	{
		DaoRecset.GetFieldValue("ORDERBYCLAUSE",strKeyField);
	}
	DaoRecset.Close();
	return strKeyField;
}

void CCategory::HandleCustomButton(CCommonCategory* pCommonCategory,CString Form_Tag, CString Field_Name, CString Field_Caption, CString Master_Full_Recoed,CString Details_Full_Recoed, CString Table_Name, CString Criteria)
{
	CPolyDbRecordset RecSet(PDBRS_LOAD_FROM_SYS_DB);
	CPolyDbRecordset VWRecSet;
	CString strParameterFields;
	CString strDisplayTag;
	CString strScreenTitle;

	CString strSQLStatement;
	CString strCaptionFieldName = "DisplayTagEnglishCaption";
	
	if (IsArabic())
		strCaptionFieldName = "DisplayTagArabicCaption";
	
	CString strSQL;
	strSQL.Format("SELECT * from Handle_CustomButtons WHERE TableName = '%s' AND ButtonsID = '%s'",Form_Tag, Field_Name);
	RecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (RecSet.IsEOF())
	{
		RecSet.Close();
		return;
	}
	else
	{
		RecSet.MoveFirst();
		CString strSqlStatementFieldName;
		BOOL bOpenScreen = FALSE;
		while(!RecSet.IsEOF())
		{
			RecSet.GetFieldValue("ParameterFields",strParameterFields);
			RecSet.GetFieldValue("DisplayTag",strDisplayTag);
			RecSet.GetFieldValue(strCaptionFieldName,strScreenTitle);
			RecSet.GetFieldValue("OpenScreen",bOpenScreen);

			switch(m_nEngine)
			{
				case DB_SQLSERVER:
					strSqlStatementFieldName = "SQLStatement";
					break;
				case DB_ORACLE:
					strSqlStatementFieldName = "OracleStatement";
					break;
			}
			RecSet.GetFieldValue(strSqlStatementFieldName,strSQL);
			CString strParameterValue;
			if(strParameterFields.Find(",")==-1)
			{
				strParameterValue = pCommonCategory->FormGetField(pCommonCategory->GetTrueTableName(),strParameterFields);
				if (strParameterValue == "")
				{
					RecSet.Close();
					return;
				}
				strSQL.Replace("PARAM1",strParameterValue);
			}
			else
			{
				strParameterFields = strParameterFields+",";
				CString strParamName;
				int nParametersCount = strParameterFields.Replace(",",",,");
				for (int nLoop = 0;nLoop<nParametersCount;nLoop++)
				{
					if(nLoop == 0)
						strParameterFields = strtok(strParameterFields.GetBuffer(0),",,");
					else 
						strParameterFields = strtok(NULL,",,");

					strParamName.Format("PARAM%d",nLoop+1);
					strParameterValue = pCommonCategory->FormGetField(pCommonCategory->GetTrueTableName(),strParameterFields);
					if (strParameterValue == "")
					{
						RecSet.Close();
						return;
					}
					strSQL.Replace(strParamName,strParameterValue);
				}
			}
			
			RecSet.MoveNext();
		}
		//disable the list screen function buttons
		VWRecSet.m_pPolyDb->FormEnableButton(BROWSABLE,BASETOOLBAR,NEW_BUTTONNAME,FALSE,FALSE);
		VWRecSet.m_pPolyDb->FormEnableButton(BROWSABLE,BASETOOLBAR,DELETE_BUTTONNAME,FALSE,FALSE);
		VWRecSet.m_pPolyDb->FormEnableButton(BROWSABLE,BASETOOLBAR,"CUSTOM",FALSE,FALSE);
		m_pCommonCategory->FormEnableButton(BROWSABLE,BASETOOLBAR,COMMON_BUTTONNAME,FALSE,FALSE);
		
		//display the generated view
		if(!bOpenScreen)
			VWRecSet.m_pPolyDb->ListShow(strDisplayTag, strScreenTitle,strSQL,"","",0,0,strDisplayTag,-1);
		else
			if(!pCommonCategory->CheckKey(strDisplayTag))
			{
				if (GetHSEObj()->DefineUsersVerfy(-1,strDisplayTag))
					DoFunction(0,strDisplayTag,NORMAL_MODE,TRUE,strSQL,Field_Name);
			}
	}
	
	RecSet.Close();
}

CString CCategory::ShowNotepad (CString strValue,CString strEnCaption,enum TextStyle nStyle,int nType,BOOL bLocked,COLORREF bkColor)
{
	return (char*)GetHSEObj()->ShowNotepad (strValue.AllocSysString(),strEnCaption.AllocSysString(),nStyle,nType,bLocked,bkColor);
	//return "";
}

void CCategory::OnReletedRecordSelected(LPCTSTR strFormTag, LPCTSTR strTableName, LPCTSTR strCriteria)
{
	DoFunction(-1,CString(strFormTag),NORMAL_MODE,false,CString(strCriteria));
}

CString CCategory::GetFastSearchCriteria()
{
	return m_pHSEobj->GetFastSearchCriteria();
}

void CCategory::SetFastSearchCriteria(CString strCriteria)
{
	m_pHSEobj->SetFastSearchCriteria(strCriteria.AllocSysString());
}

void CCategory::OnCreateLikeInvoked(LPCTSTR bstrFormTag, LPCTSTR bstrTableName, LPCTSTR bstrKeyFieldValue)
{
	CString strCriteria;
	CString strFormTag(bstrFormTag);
	CString strKeyFieldValue(bstrKeyFieldValue);
	strFormTag.MakeUpper();
	if (strFormTag == "HSE_TGRSKASSMNTINQ")
	{
		strFormTag = "HSE_TGRSKASSMNTENT";
		strCriteria.Format("WHERE (rskassmntEnt_assmntNo='%s')",strKeyFieldValue);
	} 
	else if (strFormTag == "HSE_TGRSCUPLNINQURY")
	{
		strFormTag = "HSE_TGRSCUPLNENTRY";
		strCriteria.Format("WHERE (RSCUPLN_PRMRYKY='%s')",strKeyFieldValue);
	}		

	//////////////////////////////////////////////////////////////////////////////////
	// RQ-4-2021.5
	else if (strFormTag == "HSE_DRLLPLNINQ")
	{
		strFormTag = "HSE_DrllPlnEntryAtEntry";
	}		
	//////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////
	// RQ-4-2021.8
	else if (strFormTag == "HSE_EQINSPCTNINQRY")
	{
		strFormTag = "HSE_EQInspctnRqust";
	}		
	//////////////////////////////////////////////////////////////////////////////////
	else {
		AfxMessageBox("add code here,Create like Invoked");	
	}
	DoFunction(0,strFormTag,NORMAL_MODE,false,strCriteria,"CREATELIKE");
}

