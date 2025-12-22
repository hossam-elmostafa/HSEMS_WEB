#include "stdafx.h"
#include "io.h"

#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"
//to use HSEOBJ

#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
///////////INITIALIZE STATIC VARIABLES///////////////

long			 CCommonCategory::m_nShowPos=0;
long			 CCommonCategory::m_nStockCodingWay = 0;
long			 CCommonCategory::m_nEmpSetting =0;
LPDISPATCH		 CCommonCategory::m_pActiveRecordset = NULL;
CPolyDbRecordset *CCommonCategory::m_FirstSysRecSet=NULL;
extern IFirstDesktopPtr	gpFirstDesktopObj;
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#define	EMPTY_SQL											"-1"
CCommonCategory::CCommonCategory(CCategory *pCategory)
{
	m_bOpenList = false;
	m_bOneToMany = false;
	m_bDialogMode = false;
	m_nScreenHelpID = 0;
	m_strComesFrom = "";
	m_bHasListWindow=FALSE;
	m_bModalScreen=TRUE;
	m_bMainSubBrowsing=FALSE;
	int nDisplayStatus=pCategory->GetDisplayStatus();
	m_bAddNewMode=nDisplayStatus==ADD_NEW_MODE;

	m_bEditMode=nDisplayStatus==EDIT_MODE;
	m_bCancelOperation=TRUE;
	m_nNumberOfUnDeletedRecords = 0;
	m_pHSEMSPolyDb = pCategory->GetHSEMSPolyDB();
	m_pHSEMSSYSPolyDb = pCategory->GetHSEMSSYSPolyDB();
	m_pCategory=pCategory;
	m_bLanguage = m_pCategory->IsArabic();
	m_pCategory->SetMyCCategory(this);
	m_pCategory->m_pToolsListener->SetMyCCategory(this);

	m_strFormTag="";
	m_bSubWindowScreen = FALSE;
	if(m_FirstSysRecSet ==NULL)
	{
		m_FirstSysRecSet = new CPolyDbRecordset(PDBRS_LOAD_FROM_SYS_DB);
		CString strFormatSQL;
		strFormatSQL.Format("SELECT * FROM TABLESINFO WHERE APPLICATIONCODE = %d",APP_CODE);
		m_FirstSysRecSet->Open(AFX_DAO_USE_DEFAULT_TYPE,strFormatSQL);

	}

	m_nScreenId=0;
	m_ScreenTgName="";

	m_nCurrentCard =0;
	m_nMaxNoRecord=0;
	m_bModal=false;
	m_bAllowAddNew=true;
	m_bAllowUpdate=true;
	m_bAllowDelete=true;
}

CCommonCategory::~CCommonCategory()
{
	if(m_FirstSysRecSet)
	{
		delete m_FirstSysRecSet;
		m_FirstSysRecSet=NULL;
	}
	try
	{
		m_pCategory=NULL;
	}
	catch(...)
	{
		return;
	}
}

BOOL CCommonCategory::Validate()
{
	BOOL bRetVal=m_pHSEMSPolyDb!=NULL;
	if(!bRetVal)
	{
		if(!m_bLanguage)
			AfxMessageBox("No DB Available");
		else
			AfxMessageBox("áÇ íæÌÏ ãßÊÈÉ");
	}
	return bRetVal;
}

short CCommonCategory::GridShow(LPCTSTR title, BOOL one_to_many, BOOL AllowUpdate, BOOL AllowAdd, BOOL AllowDelete, BOOL lang, LPSTR strTableName,int nOpenState,int nHelpID)
{
	BOOL bModal=(m_pCategory->IsModalPolyDb())?TRUE:FALSE;//The screen must be modal when in sub browsing state;
	short retval=0;
	
	if(m_pHSEMSPolyDb)
	{
		try
		{
			long nShowPos=m_nShowPos;
			m_nShowPos=0;
			m_bAddNewMode=FALSE;
			///////////////
			if (!AllowAdd && nShowPos == -1)
				nShowPos = 0;

			SetCurrentLanguage(m_bLanguage);
			if(one_to_many)
				DisableSubToolBarForLockedScreens(FALSE);
			m_pHSEMSPolyDb->GridShow(title,AllowUpdate, AllowAdd, AllowDelete,strTableName,(bModal)?0:m_pCategory->GetParentHWnd(),nOpenState,nShowPos,nHelpID);
			m_strMenuTitle = title;

			if(!bModal)
			{
				if (gpFirstDesktopObj != NULL && m_strMenuTitle  != "")
				{
					int nMenuID = m_pCategory->GetMenuID();
					gpFirstDesktopObj->AddRecentMenuItem(m_strMenuTitle .AllocSysString(),nMenuID);
				}
				MovePolydbForm();
			}
		}
		catch(...)
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			// Free the buffer.
			LocalFree( lpMsgBuf );
		}
	}
	return retval;
}

UINT CCommonCategory::ParentManage(LPCTSTR strTableName,BOOL bAllowUpdate,BOOL bAllowAddNew,BOOL bAllowDelete,CString strTableNameForSql,LPCTSTR strSelectStatement,BOOL bModal,LPCTSTR report_file ,int nMaxNoRecord ,int nCustomButtons,CString strToolTips,BOOL bIgnoreFilterScreen)
{
	//CString str;
	//str.Format("%d,%d,%d",bAllowUpdate,bAllowAddNew,bAllowDelete);
	//AfxMessageBox(str);
	if(!Validate())
		return -1;
	//m_bLanguage = m_pCategory->IsArabic();
	SetCurrentLanguage(m_bLanguage);
	m_nMaxNoRecord=nMaxNoRecord;
	m_bModal=bModal;
	m_bAllowAddNew = bAllowAddNew;
	m_bAllowUpdate = bAllowUpdate;
	m_bAllowDelete = bAllowDelete;
	CString strCaptTag;

	CString strSQL(strSelectStatement);

	m_strFSA ="";

	CString strTableNamestr=strTableName;
	strTableNamestr.Replace("_DUMMY","");
	strTableName=strTableNamestr.GetBuffer(0);
	
	
	//use this function or the other 2 lines between braces in order of optimization
	{
		m_strFSA   = strTableName;
		if(m_ScreenTgName=="")
			strCaptTag = strTableName;
		else
			strCaptTag = m_ScreenTgName;
	}

	if(m_strFormTag=="")
		m_strFormTag=strCaptTag;
	bModal=m_pCategory->IsModalPolyDb();
	


//	if(!bModal)
//	{
//		ThisScreenAtom=FindAtom(m_strFormTag);// USE ATOMS TO MARK THIS SCREEN AS BEING ALREADY SHOWN
//		if(ThisScreenAtom!=0)
//			return 0;	
//	}
//	ThisScreenAtom=AddAtom(m_strFormTag);
	BOOL bShowEmptyList=FALSE;

	CString strTrueTableName;
	if(m_strScreenCaption != "")
		GetScreenData(m_strScreenCaption ,m_strScreenCaption,m_strReportFileName,m_bOneToMany,m_strUniqueFields,m_strOrderFields,m_nScreenHelpID,m_strKeyField,bShowEmptyList,m_strStatusField,strTrueTableName,m_nMaxNoRecord);
	else
		GetScreenData(strCaptTag,m_strScreenCaption,m_strReportFileName,m_bOneToMany,m_strUniqueFields,m_strOrderFields,m_nScreenHelpID,m_strKeyField,bShowEmptyList,m_strStatusField,strTrueTableName,m_nMaxNoRecord);

	if (!m_bDialogMode && m_bOneToMany && m_bAddNewMode==false && m_bEditMode==false)
		m_bOpenList=true;

	ATOM ThisScreenAtom	=	IsScreenLoaded(strCaptTag,m_bOpenList);
	if(!ThisScreenAtom)
		return -1;

	CString strTitle;
	if(m_bLanguage)
	{
		strTitle.Format("ÞÇÆãÉ %s",m_strScreenCaption);
	}
	else
	{
		strTitle.Format("%s List",m_strScreenCaption);
	}

	if(strTableNameForSql == "")
		strTableNameForSql = m_strFSA;
	
	m_stringTableName= strTableName;	
	m_strTableNameForSql =strTableNameForSql;
	CString strBuildStatement;
	if(m_strWhereCriteria != "" && m_strWhereCriteria.Find("WHERE") != -1 && strncmp(m_strWhereCriteria, m_strWhereCriteria, strlen(m_strWhereCriteria)) ==0 ) {
		if(strSQL== "") {
			strSQL = m_strWhereCriteria;
		} else {				
			CString strTemp;
			CString strWhereCriteria = m_strWhereCriteria;
			strWhereCriteria.Replace("WHERE","");
			strTemp.Format("%s AND %s",strSQL,strWhereCriteria);
			strSQL = strTemp;
		}
	}	
	if(strSQL=="")
	{
		if(m_bOneToMany && !m_bEditMode)//Bugs-4-2022.14 as per A Azmy. When you open a screen in Edit Mode directly GetBrowsingSQL should be called
		{
			strBuildStatement.Format("SELECT * FROM %s",strTableNameForSql,m_strOrderFields);
		}
		else
		{
			//strBuildStatement=GetBrowsingSQL(strTableNameForSql,strSQL);
			strBuildStatement=GetBrowsingSQL(m_stringTableName,strSQL);
		}
		if(m_strOrderFields !="")
		{
			strBuildStatement += " " + m_strOrderFields;
		}

	}
	else
	{
		CString strSelStatement=strSQL;
		strSelStatement.MakeUpper();
		int nSelectPos=strSelStatement.Find("SELECT");
		if(nSelectPos==-1 || nSelectPos>5)	// WRONG IN SELECT STATEMENT
		{
			if(strSelStatement.Find("ORDER BY") ==-1)
			{
				strBuildStatement.Format("SELECT * FROM %s %s ",strTableNameForSql,strSQL);// SHUTEQP
				if(m_strOrderFields !="")
				{
					strBuildStatement+= " " + m_strOrderFields;
				}
			}
			else
				strBuildStatement.Format("SELECT * FROM %s %s",strTableNameForSql,strSQL);// SHUTEQP
		}
		else
		{
			if(m_strOrderFields !="")
			{
				strBuildStatement+= strSelStatement + " " + m_strOrderFields;
			}
			else
				strBuildStatement=strSelStatement;
		}
	}
	
	m_strBuildStatement=strBuildStatement;//Testing Bugs-3-2021.104 "It works when adding the open condition here and not use ChangeCriteria" + " WHERE (CRCTVEACCENT_RECSTS = '26') OR (CRCTVEACCENT_DPRT = '01' AND CRCTVEACCENT_RECSTS = '25')";
	if(m_bEditMode==FALSE)
		m_nShowPos=-1;

//	if (m_stringTableName == "HSE_TGHSEPLC")
//		m_bEditMode = TRUE;
	
	if(m_bOneToMany && m_bDialogMode == FALSE && m_bAddNewMode==FALSE && m_bEditMode==FALSE)
	{
		CString strCaption;
		int nPosOfBrace=strCaption.Find("AS [");
		char *pCaption=strCaption.GetBuffer(NULL)+nPosOfBrace+4;
		char *pSecondBrace=strchr(pCaption,']');
		if(pSecondBrace)
			*pSecondBrace=0;
		
		
		CString strRetVal="";//;"APPTABLEINDEX";
		if (m_strKeyField != "" && m_strKeyField.Find("|")==-1)
			strRetVal =m_strKeyField;

		m_bMainSubBrowsing=TRUE;
		m_nShowPos=0;
		
		//SetModalScreen(FALSE);
		
		//Changed by : a.ali
		//Date : 28/02/2017
		//TODO : Review this code with Eng A.Azmy
		if(bModal)
			SetModalScreen(TRUE);
		else 
			SetModalScreen(FALSE);

		long lRetVal=0;
		FormEnableButton(BROWSABLE,BASETOOLBAR,NEW_BUTTONNAME, m_bAllowAddNew,FALSE);
		FormEnableButton(BROWSABLE,BASETOOLBAR,DELETE_BUTTONNAME, m_bAllowDelete ,FALSE);
		
	
		long lParentHandle = 0;
		if (!bModal)
			lParentHandle = m_pCategory->GetParentHWnd();

		if (bShowEmptyList == TRUE && bIgnoreFilterScreen == FALSE)
		{
			CString strRetSQL = ShowFilterScreen(m_strFSA,m_strScreenCaption,strTableNameForSql,TRUE,m_strBuildStatement);
			if(strRetSQL == EMPTY_SQL) 
			{
				DeleteAtom(ThisScreenAtom);
				delete m_pCategory;
				return -1;
			}
		}

		CString strFastSearchCriteria = m_pCategory->GetFastSearchCriteria();
		m_pCategory->SetFastSearchCriteria("");

		if (strFastSearchCriteria != "")
		{
			m_pHSEMSPolyDb->CreateFilter(m_strFormTag,"Last Search",strFastSearchCriteria,0);
			m_pCategory->SetFastSearchCriteria("");
		}

		m_strScreenCaption=strTitle;
		if(lParentHandle == 0)
		{
			m_bHasListWindow=TRUE;  
			m_bMainSubBrowsing=FALSE;
		}
		OnBeforeListReady(m_stringTableName);

		lRetVal = m_pHSEMSPolyDb->ListShow(m_stringTableName, strTitle ,m_strBuildStatement,strRetVal ,"",TRUE,lParentHandle,m_stringTableName,-1);
		
		if(lParentHandle == 0)
			return 0;

		// fire this event after list show		
		OnScreenReady(m_stringTableName);
	
		UpdateTablesInfoCriteria(m_stringTableName, m_strBuildStatement);
		m_strMenuTitle = strTitle;
		if(!bModal)
		{
			if (gpFirstDesktopObj != NULL && m_strMenuTitle  != "")
			{
				int nMenuID = m_pCategory->GetMenuID();
				gpFirstDesktopObj->AddRecentMenuItem(m_strMenuTitle .AllocSysString(),nMenuID);
			}
		}
		MovePolydbForm();
		
		if(lRetVal<0)
		{
			DeleteAtom(ThisScreenAtom);
			delete m_pCategory;
			return -1;
		}
		else
		{
			m_bHasListWindow=TRUE;
			m_bMainSubBrowsing=FALSE;
			return 0;
			MSG WindowMsg;
			while(GetMessage(&WindowMsg,NULL,NULL,NULL))
			{
				::TranslateMessage(&WindowMsg);
				::DispatchMessage(&WindowMsg);
				if(m_bMainSubBrowsing==FALSE)
					break;
			}
			if(m_nShowPos==-1 && m_bAddNewMode==FALSE)	// one to many cancel
			{
				delete m_pCategory;
				return -1;
			}
		}
		
	}	
	
	return CompleteParentManage();
}

UINT CCommonCategory::CompleteParentManage()
{
	if(m_nShowPos==-1 && m_bAddNewMode==FALSE && m_bEditMode==FALSE)	// if one to one
		m_nShowPos=0;
	m_bEditMode=FALSE;
	long lRetVal=0;
	if(m_bOneToMany)
	{
		//this is a problem in polydb that there is a relation between list and one to many
		if (m_pActiveRecordset != NULL)
			lRetVal = m_pHSEMSPolyDb->DBBuildSet((m_bOneToMany==FALSE)?1:2,NULL,m_nMaxNoRecord,m_strUniqueFields.GetBuffer(NULL),m_pActiveRecordset,m_strFormTag);
		else
			lRetVal = m_pHSEMSPolyDb->DBBuildSet((m_bOneToMany==FALSE)?1:2,m_strBuildStatement.GetBuffer(NULL),m_nMaxNoRecord,m_strUniqueFields.GetBuffer(NULL),m_pActiveRecordset,m_strFormTag);
	} else {


		UpdateTablesInfoCriteria(m_stringTableName, m_strBuildStatement);
		lRetVal = m_pHSEMSPolyDb->DBBuildSet((m_bOneToMany==FALSE)?1:2,m_strBuildStatement.GetBuffer(NULL),m_nMaxNoRecord,m_strUniqueFields.GetBuffer(NULL),NULL,m_strFormTag);
		m_bSubWindowScreen = TRUE; //Added To Solve The Problem Of Maximum No Of Process Exceeded
	}

	if(lRetVal < 0)
	{
		//AfxMessageBox(m_strBuildStatement);
		AfxMessageBox("DBBuildSet Failed");
		return -1;
	}
	lRetVal = m_pHSEMSPolyDb->FieldSetAttrib(m_strFSA.GetBuffer(NULL));
	int nOpenState = 0;
	if(m_bOneToMany)
	{
		lRetVal = m_pHSEMSPolyDb->LinkSetAttrib(m_strFSA.GetBuffer(NULL));
		lRetVal = m_pHSEMSPolyDb->LinkFieldsAttrib(m_strFSA.GetBuffer(NULL));
	}

	SetScreenDefauls();
	
	//m_pCategory->SetModalPollyDB(m_bModal);
	
	// Removed from PolyDB by Ahmed Azmy in 6 Jan 2013
	/*if(m_strReportFileName != "")
	{
		if( (_access( m_strReportFileName, 0 )) != -1 )
			m_pHSEMSPolyDb->ReportEnable((m_bOneToMany==TRUE)?2:1,m_strReportFileName);
	}*/
	
	UINT unRetVal=0;
	int nScreenType = ONETOONE;
	if(m_bOneToMany)
	{
		nScreenType = ONETOMANY;
	}
	FormEnableButton(nScreenType,BASETOOLBAR,NEW_BUTTONNAME, m_bAllowAddNew,FALSE);
	FormEnableButton(nScreenType,BASETOOLBAR,DELETE_BUTTONNAME, m_bAllowDelete ,FALSE);
	unRetVal=GridShow(m_strScreenCaption, m_bOneToMany,m_bAllowUpdate , m_bAllowAddNew, m_bAllowDelete, 0,(LPSTR)m_stringTableName.GetBuffer(NULL),nOpenState,m_nScreenHelpID);
	m_bHasListWindow = FALSE;
	return unRetVal;
}

void CCommonCategory::GetTablesNames(LPCTSTR strTableName,CString &strFSA,CString &strLSA,CString &strLFA,CString &strCaptTag)
{
	char *pHashPos=strchr((char*)strTableName,'#');
	if(pHashPos)
		*pHashPos=0;
	char*pSlash=strchr((char*)strTableName,'\\');
	if(pSlash)
	{
		*pSlash=0;
		strFSA=strTableName;
		pSlash++;
		if(*pSlash=='s')	// NAME\sANYTHING
		{
			pSlash++;
			char *pSlash2=strchr(pSlash,'\\');
			if(pSlash2)
			{
				*pSlash2=0;
				strLSA=pSlash;
				pSlash2++;
				if(*pSlash2=='f')	// NAME\sNAME\fANYTHING
				{
					pSlash2++;
					char *pSlash3=strchr(pSlash2,'\\');
					if(pSlash3)
					{
						*pSlash3=0;
						strLFA=pSlash2;
						pSlash3++;
						if(*pSlash3=='c')	// NAME\sNAME\fNAME\cNAME
						{
							pSlash3++;
							strCaptTag=pSlash3;
						}
						else 
							ASSERT(FALSE);
					}
					else	// NAME\sNAME\fNAME
					{
						strLFA=strCaptTag=pSlash2;
					}
				}
				else	// NAME\sNAME\cANYTHING
				{
					if(*pSlash2=='c')	// NAME\sNAME\cNAME
					{
						pSlash2++;
						strLFA=strLSA;
						strCaptTag=pSlash2;
					}
					else
						ASSERT(FALSE);
				}
			}
			else	// NAME\sNAME
			{
				strLSA=strLFA=strCaptTag=pSlash;
			}
		}
		else	// NAME\fANYTHING || NAME\cANYTHING
		{
			strLSA=strFSA;
			if(*pSlash=='f')// NAME\fANYTHING 
			{				
				pSlash++;
				char *pSlash2=strchr(pSlash,'\\');
				if(pSlash2)	// NAME\fNAME\ANYTHING
				{
					*pSlash2=0;					
					strLFA=pSlash;
					pSlash2++;
					if(*pSlash2=='c')	// NAME\fNAME\cNAME
					{
						pSlash2++;
						strCaptTag=pSlash2;
					}
					else 
						ASSERT(FALSE);
				}
				else	// NAME\fNAME
				{
					strLFA=pSlash;
				}
			}
			else	//NAME\cANYTHING
			{
				strLFA=strLSA;
				if(*pSlash=='c')
				{
					pSlash++;
					strCaptTag=pSlash;
				}
				else
					ASSERT(FALSE);
			}
		}
	}
	else	// NAME
	{
		strFSA=strTableName;
		strLSA=strTableName;
		strLFA=strTableName;
		strCaptTag=strTableName;
	}

}
CString CCommonCategory::GetDbPath()
{
	return m_pCategory->GetDbPath();
}

CString	CCommonCategory::Find(CString strAllData,CString strField,CString strTable)
{
	if(strTable!="")
	{
		CString strCaption=strField;
	}
	CString strAllDatacopy=strAllData;
	strAllDatacopy.MakeUpper();
	CString strFieldCopy=strField;
	strFieldCopy.MakeUpper();
	strFieldCopy+="|";
	int nPos=strAllDatacopy.Find(strFieldCopy);
	if(nPos<0)
		return "";
	CString strTemp=strAllData.GetBuffer(NULL)+nPos;
	CString strRetVal=strTemp.GetBuffer(NULL)+strField.GetLength()+1;
	char *pNull=strchr(strRetVal.GetBuffer(NULL),'|');
	if(pNull)
		*pNull=0;
	return strRetVal;
} 


BOOL CCommonCategory::AskForRelatedTables(LPCTSTR TableName, LPCTSTR Field_name,LPCTSTR Field_Data)
{
	CString strFSA,
			strLSA,
			strLFA,
			strDumy;

	BOOL bRetVal=0;
	CString strLinkedTable="";
	CString strLinkedField="";
	CString strSearchOnLinkedField="";

	CPolyDbRecordset *pDaoRecset=new CPolyDbRecordset(PDBRS_LOAD_FROM_SYS_DB);
	CString FSAString = "SELECT DISTINCT TABLENAME,DATAFIELDNAME,BROWSINGRETFIELD FROM FSA WHERE BROWSINGTABLE =";
	CString LFAString = "SELECT DISTINCT TABLENAME,LINKNAME,DATAFIELDNAME,BROWSINGRETFIELD FROM LFA WHERE BROWSINGTABLE =";
	CString FullSqlString;

	CPolyDbRecordset *pDoaLinkTable=new CPolyDbRecordset;

	CPolyDbRecordset *pDoaTrueTableFrmLSA=new CPolyDbRecordset(PDBRS_LOAD_FROM_SYS_DB);
	CString strField_name = Field_name;
	CString strField_Data =Field_Data;
	if(strField_name!="")
		FullSqlString.Format("%s '%s' AND TABLENAME <> '%s' AND BROWSINGRETFIELD ='%s' AND APPLICATIONCODE = %ld AND FIELDSTYLE <> 11",FSAString,TableName,TableName,strField_name,APP_CODE);
	else
		FullSqlString.Format("%s '%s' AND TABLENAME <> '%s' AND APPLICATIONCODE = %ld AND FIELDSTYLE <> 11",LFAString,TableName,TableName,APP_CODE);

	pDaoRecset->Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
	if(!pDaoRecset->IsEOF())
	{
		while(!pDaoRecset->IsEOF())
		{
			pDaoRecset->GetFieldValue("TABLENAME",strLinkedTable);
			if(strField_name=="")
			{
				pDaoRecset->GetFieldValue("BROWSINGRETFIELD",strField_name);
				strField_Data = Find(strField_Data,strField_name);
			}
			
			if(m_pCategory->GetTableRecodingStatus(strLinkedTable))
			{
				GetTablesNames(strLinkedTable.GetBuffer(NULL),strFSA,strLSA,strLFA,strDumy);
				strLinkedTable=strFSA;

				pDaoRecset->GetFieldValue("DATAFIELDNAME",strLinkedField);
				strSearchOnLinkedField.Format("SELECT %s FROM %s WHERE %s = '%s'",strLinkedField,strLinkedTable,strLinkedField,strField_Data);
				long lRetVal = pDoaLinkTable->Open(AFX_DAO_USE_DEFAULT_TYPE,strSearchOnLinkedField);
				if(lRetVal < ERROROPENTABLE)
				{
					pDaoRecset->MoveNext();
					continue;
				}
				if(!pDoaLinkTable->IsEOF())
				{
					bRetVal = TRUE;
					return bRetVal;
				}
			}
			pDaoRecset->MoveNext();
			bRetVal = FALSE;
		}
	}

	bRetVal=FALSE;
	if(strField_name!="")
		FullSqlString.Format("%s '%s' AND BROWSINGRETFIELD='%s' AND APPLICATIONCODE = %ld AND FIELDSTYLE <> 11",LFAString,TableName,Field_name,APP_CODE);
	else
		FullSqlString.Format("%s '%s' AND APPLICATIONCODE = %ld AND FIELDSTYLE <> 11",LFAString,TableName,APP_CODE);

	pDaoRecset->Open(AFX_DAO_USE_DEFAULT_TYPE,FullSqlString);
	if(pDaoRecset->IsEOF())
		bRetVal=FALSE;
	else
	{
		while(!pDaoRecset->IsEOF())
		{

			pDaoRecset->GetFieldValue("LINKNAME",strLinkedTable);
			pDaoRecset->GetFieldValue("BROWSINGRETFIELD",strField_name);
			
			//Updated by Doaa Desouky
			//Date :17/5/2011
			/*Description
				Solve bug When (use uncoded item in material request and then try to delete Uncoded item
				the item deleted without notify user that there is related record in material request*/
			/*Solution description
				in function in case of tab use linkname from FirstSys (LFA) as the table name
				but in some cases the link name is diffrent with the true table name so i make the function 
				use the true table name from LSA instead of linkname*/
			//These lines added
			CString strTrueTableName;
			CString MainTableName;
			pDaoRecset->GetFieldValue("TABLENAME",MainTableName);
			strTrueTableName.Format("SELECT LINKOTHERTABLE FROM LSA WHERE TABLENAME = '%s' AND LINKNAME ='%s' AND APPLICATIONCODE = %ld" ,MainTableName, strLinkedTable, APP_CODE );
			pDoaTrueTableFrmLSA->Open(AFX_DAO_USE_DEFAULT_TYPE,strTrueTableName);
			pDoaTrueTableFrmLSA->GetFieldValue("LINKOTHERTABLE",strTrueTableName);
/////////////////////////////////////////////////////////////////////////////
			//And these lines commented
			/*GetTablesNames(strLinkedTable.GetBuffer(NULL),strFSA,strLSA,strLFA,strDumy);
			strLinkedTable=strFSA;
			pDaoRecset->GetFieldValue("DATAFIELDNAME",strLinkedField);
			strSearchOnLinkedField.Format("SELECT %s FROM %s WHERE %s = '%s'",strLinkedField,strLinkedTable,strLinkedField,strField_Data);*/
////////////////////////////////////////////////////////////////////
			//these lines are instead of commented lines 
			GetTablesNames(strTrueTableName.GetBuffer(NULL),strFSA,strLSA,strLFA,strDumy);
			strTrueTableName=strFSA;
			pDaoRecset->GetFieldValue("DATAFIELDNAME",strLinkedField);
			strSearchOnLinkedField.Format("SELECT %s FROM %s WHERE %s = '%s'",strLinkedField,strTrueTableName,strLinkedField,strField_Data);
////////////////////////////////////////////////////////////////////////////////////
			pDoaLinkTable->Open(AFX_DAO_USE_DEFAULT_TYPE,strSearchOnLinkedField);
			if(!pDoaLinkTable->IsEOF())
			{				
				bRetVal = TRUE;
				return bRetVal;
			}
			else
			{
				pDaoRecset->MoveNext();
				bRetVal = FALSE;
			}
		
		}
	}

	pDaoRecset->Close();
	delete pDoaLinkTable;
	delete pDaoRecset;
	return bRetVal;
}
void CCommonCategory::DeleteFromRelatedTables(CString strTableName,CString strFullRecord)
{
	CPolyDbRecordset DaoRecSet(PDBRS_LOAD_FROM_SYS_DB);
	CString strOpenLFA;
	strTableName.MakeUpper();
	strOpenLFA.Format("SELECT * FROM LSA WHERE TableName = '%s' AND APPLICATIONCODE = %ld",strTableName,APP_CODE);

	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strOpenLFA);
	if(DaoRecSet.IsEOF())
	{
		DaoRecSet.Close();
		return;
	}
	
	CString strLinkedTableName;
	CString strLinkedLocalField;
	CString strLinkedLocalFieldContents;
	CString strLinkedOtherField;
	CString strFullDeleteSql;
	CString strTabAttrib;
	while(!DaoRecSet.IsEOF())
	{
		DaoRecSet.GetFieldValue("LinkOtherTable",strLinkedTableName);
		DaoRecSet.GetFieldValue("LinkLocalField",strLinkedLocalField);
		DaoRecSet.GetFieldValue("LinkOtherField",strLinkedOtherField);
		DaoRecSet.GetFieldValue("TABATTRIB",strTabAttrib);
		if(strTabAttrib.Find("_VIEW") >=0)
		{
			DaoRecSet.MoveNext();
			continue;
		}
		strLinkedLocalFieldContents = Find(strFullRecord,strLinkedLocalField,strTableName);
		
		if(strLinkedTableName != "")
		{
			strFullDeleteSql.Format("DELETE FROM %s WHERE %s = '%s'",strLinkedTableName,strLinkedOtherField,strLinkedLocalFieldContents);
			ExecuteSQL(strFullDeleteSql);
		}
		DaoRecSet.MoveNext();
	}
	DaoRecSet.Close();
}

void CCommonCategory::SetCurrentPos(long lAbsolutePosition)
{
	m_nShowPos = lAbsolutePosition;
}

long CCommonCategory::ExecuteSQL(CString& strSqlStatement,UINT unConnectionDB,BOOL bIgnoreCommit)
{
	long lRetVal = m_pCategory->ExecuteSQL(strSqlStatement,unConnectionDB,bIgnoreCommit);
	if(lRetVal >0)
		lRetVal = lRetVal;
	return lRetVal;
}

//void CCommonCategory::GetScreenData(CString &strFSA,CString &strScreenCaption,CString &strReportFile,BOOL &m_bOneToMany,CString &strUniqueFields,CString &strOrderFields,int &nHelpID,CString &strKeyField,BOOL &bShowEmptyList,CString &strStatusField,CString &strTrueTableName)
void CCommonCategory::GetScreenData(CString &strFSA,CString &strScreenCaption,CString &strReportFile,BOOL &m_bOneToMany,CString &strUniqueFields,CString &strOrderFields,int &nHelpID,CString &strKeyField,BOOL &bShowEmptyList,CString &strStatusField,CString &strTrueTableName,int &nMaxNoRecord)
{
	CString strFormatSQL;
	CString strTag=strFSA;
	
	strFormatSQL.Format("TABLENAME = '%s'",strTag);
	BOOL bRetVal = m_FirstSysRecSet->Find(AFX_DAO_FIRST,strFormatSQL);
	CString strScreenName,strReportFileName,strSingleMany,strUniqueFieldsCriteria,strWhereCriteria;


	#ifdef	_DEBUG
	try{
		CString strMenuID;
		strMenuID.Format("%ld",m_pCategory->GetMenuID());
		m_FirstSysRecSet->SetFieldValue("MENUID",strMenuID);
		m_FirstSysRecSet->Update();
	}
	catch(...)
	{
	}
	#endif
	
	//HSEMS_RV : TODO : Check this with old screens
	//Cganged from "1" to "" by Hossam since it yields a wrong order by clause
	//strOrderFields = "1";
	if(bRetVal==FALSE)
	{
		strScreenCaption = "";
		strReportFile = "";
		m_bOneToMany=FALSE;
		return;
	}

	// Get screen caption from the menu (may be change from the privilege)
	//GMS Integration
	try {
	CMenu* pMenu = m_pCategory->GetParentWnd()->GetMenu();
	pMenu->GetMenuString(m_pCategory->GetMenuID(),strScreenName,MF_BYCOMMAND);
	strScreenName.Replace("&","");
	if (strScreenName.Find("  ") != -1)
		strScreenName = strScreenName.Mid(0,strScreenName.Find("  "));
	}
	catch (...) {

	}

	if (strScreenName == "")
	{
		if(!m_bLanguage)
			m_FirstSysRecSet->GetFieldValue("ENGLISHCAPTION",strScreenName);
		else
			m_FirstSysRecSet->GetFieldValue("ARABICCAPTION",strScreenName);
	}
	m_FirstSysRecSet->GetFieldValue("REPORTFILENAME",strReportFileName);
	m_FirstSysRecSet->GetFieldValue("UNIQUEFIELDS",strUniqueFieldsCriteria);
	m_FirstSysRecSet->GetFieldValue("SINGLEMANY",strSingleMany);
	m_FirstSysRecSet->GetFieldValue("ORDERBYCLAUSE",strOrderFields);//LOAD THE SORT ORDER CRITERIA
	m_FirstSysRecSet->GetFieldValue("HELPCONTEXTID",nHelpID);
	m_FirstSysRecSet->GetFieldValue("KEYFIELD",strKeyField);
	m_FirstSysRecSet->GetFieldValue("SHOWEMPTYLIST",bShowEmptyList);
	m_FirstSysRecSet->GetFieldValue("STATUSFIELD",strStatusField);
	m_FirstSysRecSet->GetFieldValue("TRUETABLENAME",strTrueTableName);
	m_FirstSysRecSet->GetFieldValue("WHERECLAUSE",strWhereCriteria);
	m_FirstSysRecSet->GetFieldValue("MaxRecords",nMaxNoRecord);
	

	if(strOrderFields == "")
		m_strOrderFields = "";

	if(strSingleMany == "2" || strSingleMany == "3")
	{
		m_bOneToMany = TRUE;
		m_bSubWindowScreen = TRUE;
	}
	else
		m_bOneToMany = FALSE;

	if (strSingleMany == "3")
		m_bDialogMode = TRUE;

	if(strWhereCriteria != "") {
		m_strWhereCriteria = strWhereCriteria;
	}

	m_nMaxNoRecord = nMaxNoRecord;

	strScreenCaption = strScreenName;
	strUniqueFields = strUniqueFieldsCriteria;
//	strUniqueFields	="";
	//I REMOVE IT IN ORDER TO ENABLE TXN TRACING
/*	strFormatSQL.Format("SELECT REPORTSOURCE FROM REPORTS WHERE REPORTFILENAME = '%s'",strReportFileName);
	CPolyDbRecordset DaoDBRecSet;
	DaoDBRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strFormatSQL);
	if(DaoDBRecSet.IsEOF())
	{
		strReportFile = "";
	}
	DaoDBRecSet.GetFieldValue("REPORTSOURCE",strReportFileName);
	strReportFile = strReportFileName;
	DaoDBRecSet.Close();
*/
}

CString CCommonCategory::GetCurrentDate(CString &strFormat)
{
	CString strRetDate;
	CString strDate = m_pHSEMSPolyDb->GetServerDate();
	COleDateTime OleDate;
	OleDate.ParseDateTime(strDate);

	if(strFormat != "MONTHDAYYEAR")
		strRetDate = OleDate.Format("%d/%m/%Y");
	else
		strRetDate = OleDate.Format("%m/%d/%Y");
	return strRetDate;
}
void CCommonCategory::WaitCursor()
{
	AfxGetApp()->BeginWaitCursor();
	return;
}
BOOL CCommonCategory::AskYesNoMessage(CString &strTitle,CString &strMessage)
{
	BOOL bRetVal;
	if(m_bLanguage)
		strTitle = "ÊäÈíå";
	if(MessageBox(NULL,strMessage,strTitle,MB_YESNO | MB_ICONEXCLAMATION | MB_TASKMODAL) == IDYES)
		bRetVal =  TRUE;
	else 
		bRetVal =  FALSE;
	return bRetVal;
}
void CCommonCategory::FillListRecordSet(LPDISPATCH	pActiveRecordset)
{
	m_pActiveRecordset = pActiveRecordset;
}

void CCommonCategory::DisableSubToolBarForLockedScreens(BOOL bDelete)
{
	FormEnableButton(ONETOMANY,SUBTOOLBAR,COPY_BUTTONNAME,FALSE,FALSE);
	FormEnableButton(ONETOMANY,SUBTOOLBAR,COMMON_BUTTONNAME,FALSE,FALSE);
	if(bDelete)
		FormEnableButton(ONETOMANY,SUBTOOLBAR,DELETE_BUTTONNAME,FALSE,FALSE);
}

void CCommonCategory::ActivateListWindow()
{
	ATOM ScrAtom = FindAtom(m_strFormTag);
	if(ScrAtom!=0)
	{
		DeleteAtom(ScrAtom);
	}
	m_bHasListWindow = TRUE;
	m_pActiveRecordset = NULL; //reset the Active RecordSet
}

CString CCommonCategory::GetTrueTableName()
{
	return m_strTableNameForSql;
}

BOOL CCommonCategory::CheckKey(CString &strTableName)
{
	ATOM ThisScreenAtom=FindAtom(strTableName);
	if(ThisScreenAtom)
	{
		MessageBox(NULL,"This Screen is already opened","Prompt",0);
		return TRUE;
	}
	return FALSE;
}

COleDateTime CCommonCategory::GetGregDate(CString &strDate)
{
	COleDateTime GergDate;
	GergDate.ParseDateTime(strDate);
	if(m_pCategory->IsArabic())
	{
		COleDateTime oleHDate;
		UDATE uHijriiDate;
		HRESULT hr=E_INVALIDARG;
		LONG lCounter=0;
		int iDay=0;
		int iMonth=0;
		int iYear=0;
		iDay=__min(GergDate.GetDay(),30);
		iMonth=GergDate.GetMonth();
		iYear=GergDate.GetYear();
		oleHDate.SetStatus(COleDateTime::invalid);
		while (oleHDate.m_status == COleDateTime::invalid && lCounter < 10)
		{
			oleHDate.SetDateTime(iYear,iMonth,iDay-lCounter,0,0,0);
			oleHDate.GetAsSystemTime(uHijriiDate.st );
			hr=VarDateFromUdate(&uHijriiDate,VARIANT_CALENDAR_HIJRI,&oleHDate.m_dt);
			lCounter ++;
		}
		lCounter --;
		if(hr == S_OK)
			GergDate.m_dt=oleHDate.m_dt - lCounter;
		CString strMyDate = GergDate.Format("%d/%m/%Y");
	}
	return GergDate;
}

CString CCommonCategory::GetTXNIndex(CString strTable,CString strRetField,CString strRefField,CString strRefValue,CString strWhereClause)
{
	CString strSQL,strTXNIndex;
	if(strWhereClause=="")
		strSQL.Format("SELECT %s FROM %s WHERE %s = '%s'",strRetField,strTable,strRefField,strRefValue);
	else
	{
		if(strWhereClause.Find("WHERE ")==0)
		{
			strSQL.Format("SELECT %s FROM %s %s = '%s' AND %s",strRetField,strTable,strRefField,strRefValue,strWhereClause);
		}
		else
			strSQL.Format("SELECT %s FROM %s WHERE %s = '%s' AND %s",strRetField,strTable,strRefField,strRefValue,strWhereClause);
	}
	CPolyDbRecordset DaoRecSet;
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecSet.GetFieldValue(strRetField,strTXNIndex);
	DaoRecSet.Close();
	return strTXNIndex;
}

int CCommonCategory::GetCount(CString strTableName,CString strLinkField,CString strIndex,BOOL bString,CString strAddSQL)
{
	CString strSQL;
	int nCount = 0;
	if(bString==TRUE)//means string format
		strSQL.Format("SELECT COUNT (1) AS NOOFRECORDS FROM %s WHERE %s = '%s'",strTableName,strLinkField,strIndex);
	else
		strSQL.Format("SELECT COUNT (1) AS NOOFRECORDS FROM %s WHERE %s = %s",strTableName,strLinkField,strIndex);
	if(strAddSQL !="")
	{
		if (strAddSQL.Find("WHERE") == 0)
		{
			strAddSQL = strAddSQL.Mid(5);
		}
		strSQL+=" AND "+strAddSQL;
	}
	nCount = GetRetVal(strSQL);
	return nCount ;
}

long CCommonCategory::GetRetVal(CString &strSQL)
{
	CPolyDbRecordset DaoRecSet;
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecSet.MoveFirst();
	long lCount =-1;
	DaoRecSet.GetFieldValue("NOOFRECORDS",lCount);
	DaoRecSet.Close();
	return lCount;
}
double CCommonCategory::GetRetDVal(CString &strSQL)
{
	CPolyDbRecordset DaoRecSet;
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecSet.MoveFirst();
	double lCount =-1;
	DaoRecSet.GetFieldValue("NOOFRECORDS",lCount);
	DaoRecSet.Close();
	return lCount;
}

CString CCommonCategory::FormatValueForSQL(CString strValue)
{
	strValue.Replace("'","''");
	return strValue;
}

int CCommonCategory::GetNextTXNLineNumbers(CString strTableName,CString strIndexField,CString strIndexValue,CString strItemFieldName,CString strOrderCriteria,CString strLineField,CString strRefField,CString strRefValue,BOOL bString)
{
	CString strSQL;
	//start getting the last line number from the previous transaction
	int nLineNO= 0;
	//Format String Reference
	if (bString)
		strIndexValue = "'" +strIndexValue+ "'";

	if(strRefValue !="")
	{
		strSQL.Format("SELECT MAX(%s) AS NOOFRECORDS FROM %s WHERE %s = %s AND (%s <> '%s')",strLineField,strTableName,strIndexField,strIndexValue,strRefField,strRefValue);
		nLineNO = GetRetVal(strSQL);
	}
	nLineNO++;

	return nLineNO;
}

//this function to get the sort order of the items tab in order to 
//create the line numbers correct
CString CCommonCategory::GetOrderCriteria(CString &strTableName)
{
	CPolyDbRecordset DaoRecSet(PDBRS_LOAD_FROM_SYS_DB);
	CString strSQL;
	strSQL.Format("SELECT DISTINCT LINKSQL FROM LSA WHERE LINKOTHERTABLE = '%s'",strTableName);
	CString strSortOrder = "ORDER BY 1";
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if(DaoRecSet.IsEOF())
	{
		DaoRecSet.Close();
		return strSortOrder;
	}

	DaoRecSet.MoveFirst();
	DaoRecSet.GetFieldValue("LINKSQL",strSortOrder);
	DaoRecSet.Close();

	int nPos = strSortOrder.Find("ORDER BY");
	if (nPos >=0)
		strSortOrder = strSortOrder.Mid(nPos);
	return strSortOrder;
}
CString CCommonCategory::GetSegment(CString strScreenTag)
{
	CString strGeneratedCode;
	CString strSQL;
	strGeneratedCode= m_pCategory->m_pToolsListener->m_pToolsObj->GetSegmentedCode(APP_CODE,strScreenTag,"",FALSE);
	if (strGeneratedCode != "")
	{
		m_pCategory->m_pToolsListener->m_pToolsObj->IncreaseSegment(APP_CODE,strScreenTag,"");
	}
	return strGeneratedCode;
}

CString CCommonCategory::GetFieldAttributes(CString &strTableName,CString &strFieldName)
{
	CString strAttrib;
	CPolyDbRecordset DaoRecSet(PDBRS_LOAD_FROM_SYS_DB);
	CString strFormatSQL;
	strFormatSQL.Format("SELECT ValidationRule FROM FSA WHERE APPLICATIONCODE = %ld and TableName ='%s' and DataFieldName = '%s'",APP_CODE,strTableName,strFieldName);
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strFormatSQL);
	DaoRecSet.GetFieldValue("ValidationRule",strAttrib);
	return strAttrib;
}

void CCommonCategory::RefreshScreen()
{
	FormDoAction(GetFormType(),SAVE_BUTTON);
	FormMakeRefresh(GetFormType());
}

CString CCommonCategory::GetBrowsingSQL(CString TableName, LPCTSTR WhereClause)
{
	return m_pHSEMSPolyDb->GetBrowsingSQL(TableName,WhereClause);
}

void CCommonCategory::SetCurrentLanguage(BOOL bNewValue)
{
	m_pHSEMSPolyDb->SetCurrentLanguage(bNewValue);
}

void CCommonCategory::SubMakeRefresh()
{
	m_pHSEMSPolyDb->SubMakeRefresh();
}
short CCommonCategory::GetFormType()
{
	return m_pHSEMSPolyDb->GetFormType();
}
short CCommonCategory::FormEnableButton(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, BOOL State, BOOL bForCurrTabOnly)
{
	if (Form_Type == UNKNOWN)
		Form_Type = m_pHSEMSPolyDb->GetFormType();
	return m_pHSEMSPolyDb->FormEnableButton(Form_Type, ToolbarID, ButtonKey, State, bForCurrTabOnly);
}
void CCommonCategory::LockScreen(short Form_Type, BOOL bLockMain, BOOL bLockSubs)
{
	m_pHSEMSPolyDb->LockScreen(Form_Type,bLockMain,bLockSubs);
}

short CCommonCategory::LinkFieldsAttribSingle(LPCTSTR Link_Name, LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, 
		LPCTSTR Field_F_SQL, LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields)
{
	return m_pHSEMSPolyDb->LinkFieldsAttribSingle(Link_Name, Field_Name, Field_Caption, Max_Length, Field_Help, Field_Attrib_Rules, Field_Mask, Field_Default, Field_Style, Field_F_Table, 
		Field_F_SQL, Field_F_Field, Field_Ret_Field, strBrowsingArray, ArabicHelpContextID, EnglishHelpContextID, strBrowsingHelpFields);
}
short CCommonCategory::FieldSetAttribSingle(LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, LPCTSTR Field_F_SQL, 
		LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields)
{
	return m_pHSEMSPolyDb->FieldSetAttribSingle(Field_Name, Field_Caption, Max_Length, Field_Help, Field_Attrib_Rules, Field_Mask, Field_Default, Field_Style, Field_F_Table, Field_F_SQL, 
		Field_F_Field, Field_Ret_Field, strBrowsingArray, ArabicHelpContextID, EnglishHelpContextID, strBrowsingHelpFields);
}
CString CCommonCategory::ShowFilterScreen(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria)
{
	return m_pHSEMSPolyDb->ShowFilterScreen(strTableName, strCaption, strTrueTableName, bSaveFilter, strBaseCriteria);
}
CString CCommonCategory::FormChangeCaption(short Form_Type, LPCTSTR strCaption)
{
	return m_pHSEMSPolyDb->FormChangeCaption(Form_Type, strCaption);
}
//Poly Db Encapsulation
CString CCommonCategory::ListShowEx(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, LPCTSTR strAttribTableName, long lHelpID)
{
	_DB pPolyDb;
	m_pCategory->GetHSEObj()->DBInit(&pPolyDb,NULL,FALSE);
	pPolyDb.FormEnableButton(BROWSABLE,BASETOOLBAR,NEW_BUTTONNAME, FALSE,FALSE);
	pPolyDb.FormEnableButton(BROWSABLE,BASETOOLBAR,DELETE_BUTTONNAME, FALSE,FALSE);
	pPolyDb.FormEnableButton(BROWSABLE,BASETOOLBAR,COMMON_BUTTONNAME, FALSE,FALSE);
	return pPolyDb.ListShowEx(List_Tag, List_Title,SQL_Statement,Return_Field ,Old_Value,MultiSelection,strAttribTableName,lHelpID);
}

void CCommonCategory::FormUpdateGrid(CString Table_Name, int nFormType)
{
	m_pHSEMSPolyDb->FormUpdateGrid(Table_Name, nFormType);
}

void CCommonCategory::FormSetFieldEx(CString strFormTag ,CString Field,CString Value,int nFormType)
{
	m_pHSEMSPolyDb->FormSetField(strFormTag,Field,Value,nFormType);
}

void CCommonCategory::FormSetField(CString strTableName,CString Field,CString Value,int nFormType)
{
	m_pHSEMSPolyDb->FormSetField(strTableName,Field,Value,nFormType);
}

void CCommonCategory::FormDoAction(short Form_Type, short Button_Index)
{
	m_pHSEMSPolyDb->FormDoAction(Form_Type, Button_Index);
}
short CCommonCategory::FormMakeRefresh(short Form_Type)
{
	return m_pHSEMSPolyDb->FormMakeRefresh(Form_Type);
}

long CCommonCategory::ChangeFormSQL(CString strFullSQL,short Form_Type)
{
	return m_pHSEMSPolyDb->ChangeFormSQL(Form_Type, strFullSQL);
}

CString CCommonCategory::FormGetFieldEx(CString strFormTag ,CString Field,int nFormType)
{
	CString strValue;
	strValue = m_pHSEMSPolyDb->FormGetFieldEx(strFormTag,Field,nFormType);
	if (strValue == "/  /")
		strValue = "";

	return strValue; 
}

CString CCommonCategory::FormGetField(CString strTableName,CString Field,int nFormType)
{
	CString strValue;
	strValue = m_pHSEMSPolyDb->FormGetField(strTableName,Field,nFormType);
	
	if (strValue == "True")
		strValue = "1";

	else if (strValue == "False")
		strValue = "0";

	else if (strValue == "/  /")
		strValue = "";

	return strValue; 
}
void CCommonCategory::HandleActionEx(short Form_Type, BOOL Cancel, short Selected_Records)
{
	m_pHSEMSPolyDb->HandleActionEx(Form_Type, Cancel, Selected_Records);
}

short CCommonCategory::TabEnableEx(LPCTSTR strSubTag, BOOL State)
{
	return m_pHSEMSPolyDb->TabEnableEx(strSubTag, State);
}

void CCommonCategory::MovePolydbForm()
{
	CRect rcRect;
	CWnd::FromHandle((HWND)m_pCategory->GetParentHWnd())->GetClientRect(&rcRect);

	CRect rc=m_pCategory->GetFormRect();
	if(rc.bottom == 0)
		return;
	m_pHSEMSPolyDb->MoveForm(-1,rc.left,rc.top,rc.Width()+rc.left,rc.Height()+rc.top);
	m_pCategory->m_pToolsListener->m_pToolsObj->MoveTree(rc.left,rc.top,rc.Width()+rc.left,rc.Height()+rc.top);
}
void CCommonCategory::OnScreenExit()
{
	if (gpFirstDesktopObj != NULL && (m_bOpenList || m_bOneToMany == FALSE) )
	{
		int nMenuID = m_pCategory->GetMenuID();
		gpFirstDesktopObj->RemoveRecentMenuItem(m_strScreenCaption.AllocSysString(),nMenuID);
	}
}

void CCommonCategory::BringWindowToTop()
{
	if (gpFirstDesktopObj != NULL)
		gpFirstDesktopObj->ActivateMenuItem(m_strMenuTitle.AllocSysString());

	m_pHSEMSPolyDb->BringFormToTop(-1);
}

void CCommonCategory::UpdateTablesInfoCriteria(CString strFormTag, CString strSQL)
{
	// THIS FUNCTION WAS USED ONCE TO UPDATE [WhereClause] IN [TablesInfo] TABLE UNDER FIRSTSYSDB.MDB
	// THE [WhereClause] FIELD IS VERY ESSENTIAL IN FIELD DEPENDENCY FEATURE IN POLYDB 
	// BY AHMED AZMY
	return;

	CString strWhere = "";
	
	strSQL.MakeUpper();
	
	if (strSQL.Find("ORDER BY ") != -1)
		strSQL = strSQL.Left(strSQL.Find("ORDER BY "));

	if (strSQL.Find("WHERE ") != -1)
		strWhere = strSQL.Mid(strSQL.Find("WHERE "));

	if (strWhere != "")
	{
		//AfxMessageBox(strWhere);
		strWhere.Replace("'","''");
		CString strUpdateSQL;
		strUpdateSQL.Format("UPDATE TablesInfo SET WhereClause='%s' WHERE (WhereClause='' OR WhereClause IS NULL) AND ApplicationCode=%ld AND TableName='%s'",strWhere,APP_CODE,strFormTag);
		long lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql((LPCSTR)strUpdateSQL,PDBRS_LOAD_FROM_SYS_DB,0);
		if (lRetFieldsCount != 1)
			AfxMessageBox("ERROR");
	}
}

ATOM CCommonCategory::IsScreenLoaded(CString strFormTag, bool bList)
{
	int nSkip=0;
	ATOM ThisScreenAtom=0;
	try
	{
		BOOL bModal=m_pCategory->IsModalPolyDb();
		if(!bModal)
		{
			ThisScreenAtom=FindAtom(strFormTag);
			if(ThisScreenAtom!=0)
			{
				!bList ? nSkip=1:nSkip=0;
				m_pCategory->GetHSEObj()->ActivateForm(strFormTag.AllocSysString(),true,nSkip,true);
				ThisScreenAtom=0;
				return 0;	
			}
		}
		ThisScreenAtom=AddAtom(strFormTag);
	}
	catch(...)
	{
		
	}
	return ThisScreenAtom;
}

CString CCommonCategory::GetSelectedRecordCriteria()
{
	CString strCriteria;
	CString strKeyFieldValue = m_pHSEMSPolyDb->FormGetField(m_strTableNameForSql,m_strKeyField,-1);
	strCriteria.Format("%s='%s'", m_strKeyField, strKeyFieldValue);
	return strCriteria;
}

void CCommonCategory::FindRecord(CString strCriteria)
{
	m_pHSEMSPolyDb->FindRecord(strCriteria,-1);
}

void CCommonCategory::SetScreenDefauls()
{
	CString strScreenDefauls =m_pCategory->GetScreenDefauls();
	
	if (strScreenDefauls == "")
		return;


	CString strFieldName=strtok(strScreenDefauls.GetBuffer (0),"|");
	CString strValue=strtok(NULL,"|");
	
	FieldSetAttribSingle(strFieldName,"",-1,"","","",strValue,-1,"","","","""","",0,0,"");	

	while (strValue != "")
	{
		strFieldName=strtok(NULL,"|");
		strValue=strtok(NULL,"|");
		if (strFieldName != "")
			FieldSetAttribSingle(strFieldName,"",-1,"","","",strValue,-1,"","","","""","",0,0,"");	
	}
	
}
void CCommonCategory::OnBeforeListReady(LPCTSTR strFormTag){}
void CCommonCategory::OnScreenReady(LPCTSTR strFormTag){}

//---START---------Manage Synchronous Changes ---------By Nehad in 15 April 2014-----------------------
// This Function used in category class to save the record status in the start
void CCommonCategory::SetStartedTXNStatus()
{
	if(GetStatusFieldName() == "")
		return;

	m_strStatusVal = FormGetField(GetTrueTableName(),GetStatusFieldName());
}
// This Helper function to read current record status at any action on this screen (field change ,toolbar or custom button)
CString CCommonCategory::ReadTXNStatus()
{
	if(GetStatusFieldName() == "")
		return "";

	CString strKeyFieldVal = FormGetField(GetTrueTableName(),m_strKeyField);	
	CString strStatus = GetTXNIndex(GetTrueTableName(),GetStatusFieldName(),m_strKeyField,strKeyFieldVal,"");
	return strStatus;
}
// This Helper function to check if the record status changed or not.
BOOL CCommonCategory::IsStatusChanged()
{
	if(GetStatusFieldName() == "")
		return FALSE;

	CString strCurrentStatus = ReadTXNStatus();
	if(m_strStatusVal != "" && strCurrentStatus != "" && strCurrentStatus != m_strStatusVal)
	{
		return TRUE;
	}
	return FALSE;
}
// This Function used in category class to perform action if the record status changed
BOOL CCommonCategory::ValidateOnRecordStatus()
{
	if(GetStatusFieldName() == "")
		return TRUE;		

	if(IsStatusChanged())
	{
		AfxMessageBox("Sorry, This record was changed by another user.");
		m_bCancelOperation = TRUE;
		short nShort=0;
		try
		{
			HandleActionEx(-1,TRUE,nShort);
		}
		catch(...)
		{

		}
		FormMakeRefresh(GetFormType());
		
		return FALSE;
	}
	return TRUE;
}

CString CCommonCategory::GetPolicySettingValue(CString strSettingName)
{
	IRegHandlePtr pRegHandle;
	pRegHandle.CreateInstance(__uuidof(RegHandle));//create instance from registry handle
	pRegHandle->SetSettingMode(WM_DB); 
	pRegHandle->SetProfileName(APP_NAME);
	pRegHandle->SetConnectionString(m_pCategory->GetConnectionString().AllocSysString());

	CString strValue = (char*)pRegHandle->GetSetting(APP_NAME,APP_NAME,strSettingName.AllocSysString(),"");
	pRegHandle.Release();
	return strValue;
}

CString CCommonCategory::GetValueFromSQL(CString strSQL,CString strField)
{
	CString strRetVal;
	CPolyDbRecordset DaoRecSet;
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecSet.GetFieldValue(strField,strRetVal);
	DaoRecSet.Close();
	return strRetVal;
}

CString CCommonCategory::GetResourceCaption(int nID,BOOL bLang)
{
	CString strCaption;

	strCaption.LoadString(nID + bLang);
	
	return strCaption;
}


CString CCommonCategory::getScreenFldVal(CString strFldNam,CString strFormTag) 
{
	CString strTableName;
	if(strFormTag == "") {
		strTableName = GetTrueTableName();
	} else {
		strFormTag.MakeUpper();
		strFormTag.Replace("HSE_TG","HSE_");
		strTableName = strFormTag;
	}
	strTableName.MakeUpper();
	
	//first try to get value from sent tag
	CString strFldVal  = FormGetField(strTableName,strFldNam);
	//then try to get value from member tag (handle case for calling function from tab , but we try to get header value)
	if(strFldVal == "") {
		strTableName = GetTrueTableName();
		strTableName.MakeUpper();		
		strFldVal  = FormGetField(strTableName,strFldNam);
	}
	return strFldVal;
}

CString CCommonCategory::GeneralizeDate(CString strCARDateInitial)
{
	CString strDay=strtok(strCARDateInitial.GetBuffer(0),"/");
	CString strMonth=strtok(NULL,"/");
	CString strYear=strtok(NULL,"/");
	CString strMonthInText;
	switch(atoi(strMonth))
	{
	case 1:
		strMonthInText="JAN";
		break;
	case 2:
		strMonthInText="FEB";
		break;
	case 3:
		strMonthInText="MAR";
		break;
	case 4:
		strMonthInText="ABR";
		break;
	case 5:
		strMonthInText="MAY";
		break;
	case 6:
		strMonthInText="JUN";
		break;
	case 7:
		strMonthInText="JUL";
		break;
	case 8:
		strMonthInText="AUG";
		break;
	case 9:
		strMonthInText="SEP";
		break;
	case 10:
		strMonthInText="OCT";
		break;
	case 11:
		strMonthInText="NOV";
		break;
	case 12:
		strMonthInText="DEC";
		break;
	}

	CString strRetVal;
	strRetVal.Format("%s %s %s",strDay,strMonthInText,strYear);
	return strRetVal;
}

