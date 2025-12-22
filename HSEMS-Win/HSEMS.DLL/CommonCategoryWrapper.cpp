// CommonCategoryWrapper.cpp: implementation of the CCommonCategoryWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CCommonCategoryWrapper::OnBeforeListReady(LPCTSTR FormTag)
{

}

/////////////////////////////////////////////////////////////////////////////
///// POLYDB EVENTS /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CCommonCategoryWrapper::OnScreenReady(LPCTSTR FormTag)
{
	// DONE	
	m_strScreenTag = m_strFormTag;
	tag_ScreenReady *pScreenReady = new tag_ScreenReady;
	pScreenReady->Form_Tag = CString (FormTag).AllocSysString();
	pScreenReady->SubForm_Tag = CString ("").AllocSysString();
	pScreenReady->iScreenMode = m_bEditMode;
	DisplayScreenReady(pScreenReady);
	delete pScreenReady;
}

void CCommonCategoryWrapper::UpdateAfterBrowse(LPCTSTR Form_Tag, LPCTSTR Full_Recoed, LPCTSTR Table, LPCTSTR Return_Value,LPCTSTR Browsing_Field, LPCTSTR Criteria)
{
	// DONE

	m_strScreenTag = m_strFormTag;

	tag_BrowsingSelectedItem *pBrowsingSelectedItem = new tag_BrowsingSelectedItem;
	
    pBrowsingSelectedItem->Form_Tag				= CString(Form_Tag).AllocSysString();
    pBrowsingSelectedItem->SubForm_Tag			= CString("").AllocSysString();
    pBrowsingSelectedItem->BrowsingTable		= CString(Table).AllocSysString();
    pBrowsingSelectedItem->BrowsingField		= CString(Browsing_Field).AllocSysString();
    pBrowsingSelectedItem->Return_Value			= CString(Return_Value).AllocSysString();
    pBrowsingSelectedItem->Full_Record			= CString(Full_Recoed).AllocSysString();
    pBrowsingSelectedItem->Criteria				= CString(Criteria).AllocSysString();
    pBrowsingSelectedItem->lCategoryID			= 0;
    pBrowsingSelectedItem->lOwnerApp			= 0;
    pBrowsingSelectedItem->lOwnerTransAction	= 0;
	
    pBrowsingSelectedItem->pMultiSelectedRows = new MultiSelectedRows;
	pBrowsingSelectedItem->pMultiSelectedRows->lCount=1;
	pBrowsingSelectedItem->pMultiSelectedRows->pRow = new SelectedRow[1];
	pBrowsingSelectedItem->pMultiSelectedRows->pRow[0].strFullRecord = CString(Full_Recoed).AllocSysString();
	
	DisplayBrowsingSelectedItem(pBrowsingSelectedItem);

	delete pBrowsingSelectedItem->pMultiSelectedRows->pRow;
	delete pBrowsingSelectedItem->pMultiSelectedRows;
	delete pBrowsingSelectedItem;
}


void CCommonCategoryWrapper::FieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Main_Position, long Details_Position, short grid_edit_mode)
{
	// DONE
	m_strScreenTag = m_strFormTag;
	CString strSubFormTag = "";
	tag_FieldChange *pFieldChange = new tag_FieldChange;
	long iCancle=0;	
	CString strFormTag				= CString(Form_Tag).AllocSysString();
	strFormTag.MakeUpper();
    pFieldChange->Form_Tag			= strFormTag.AllocSysString();	
	pFieldChange->SubForm_Tag		= CString("").AllocSysString();
	CString strFieldName			= CString(Field_Name).AllocSysString();
	strFieldName.MakeUpper();
    pFieldChange->Field_Name		= strFieldName.AllocSysString();;
    pFieldChange->Old_Value			= CString(old_value).AllocSysString();
    pFieldChange->New_Value			= CString(New_Value).AllocSysString();
    pFieldChange->Full_Record		= CString(full_record).AllocSysString();	
	//pFieldChange->Table_Name		= CString("").AllocSysString();    	
	if(Details_Position != -1) 
	{
		strSubFormTag = GetTabTag(m_strFormTag,Details_Position).AllocSysString();
		strSubFormTag.MakeUpper();
	}
	pFieldChange->Table_Name		= GetTrueTableName(APP_CODE,strFormTag,strSubFormTag.AllocSysString()).AllocSysString();
	pFieldChange->AbsolutePosition	= Main_Position;
    pFieldChange->AddNew_Mode		= grid_edit_mode;
    pFieldChange->iComplete			= 1;
    pFieldChange->lCategoryID		= 0;
    pFieldChange->lOwnerApp			= 0;
    pFieldChange->lOwnerTransAction = 0;
	pFieldChange->iCancle			= &iCancle;
	DisplayFieldChange(pFieldChange);
	if (iCancle == 1)
	{
		// ACTION SHOULD BE CANCELED - BUT NOT ALLOWED IN POLYDB
	}
	delete pFieldChange;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//RQ-3-2021.4 
extern BOOL gbCustomButton;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommonCategoryWrapper::ButtonClicked(LPCTSTR Caption, LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Master_Full_Recoed,LPCTSTR Details_Full_Recoe,LPCTSTR Field_Caption,long AbsolutePosition)
{
	// DONE

	m_strScreenTag = m_strFormTag;

    CString strMasterFull_Record(Master_Full_Recoed);
	CString strDetailsFull_Record(Details_Full_Recoe);
	CString strCaption(Caption);

	CString strFulRecord = strMasterFull_Record + "|" + strDetailsFull_Record;
	strCaption.Replace("[","");
	strCaption.Replace("]","");
	strCaption.MakeUpper();


	tag_CustomButtonClicked *pCustomButtonClicked = new tag_CustomButtonClicked;
	

	CString strFormTag						= CString(Form_Tag).AllocSysString();
    strFormTag.MakeUpper();
	pCustomButtonClicked->Form_Tag			= strFormTag.AllocSysString();

	CString strSubFormTag					= GetTabTag(Form_Tag,Table_Name).AllocSysString();
	strSubFormTag.MakeUpper();
    pCustomButtonClicked->SubForm_Tag		= strSubFormTag.AllocSysString();
    pCustomButtonClicked->Button_Name		= strCaption.AllocSysString();
    
	CString strTableName					= CString(Table_Name).AllocSysString();
	strTableName.MakeUpper();
	pCustomButtonClicked->Table_Name		= strTableName.AllocSysString();
	
	pCustomButtonClicked->Full_Record		= strFulRecord.AllocSysString();
    pCustomButtonClicked->lRecordCount		= 1;
    pCustomButtonClicked->nRecordStatus		= 0;
    pCustomButtonClicked->lCategoryID		= 0;
    pCustomButtonClicked->lOwnerApp			= 0;
    pCustomButtonClicked->lOwnerTransAction = 0;

    pCustomButtonClicked->pMultiSelectedRows = new MultiSelectedRows;
	pCustomButtonClicked->pMultiSelectedRows->lCount=1;
	pCustomButtonClicked->pMultiSelectedRows->pRow = new SelectedRow[1];
	pCustomButtonClicked->pMultiSelectedRows->pRow[0].strFullRecord = strFulRecord.AllocSysString();
	pCustomButtonClicked->AbsolutePosition=AbsolutePosition;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//RQ-3-2021.4 observation approval	It is important to ensure the completeness of generated CAR records	In (main screen), change (location) & (area) to be MUST
	// Part of the implementation is to make sure that the Must fields are filled before completing the action
	bool bHandleCustomButton=true;
	if(gbCustomButton==TRUE)
	{
		CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
		CString strSQL;
		strSQL.Format("select KeyField from TablesInfo where TableName='%s' and ApplicationCode=%d",m_strScreenTag,APP_CODE);
		long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
		if (lCount != 0)
		{
			CString strKeyName;
			FirstSysRecSet.GetFieldValue("KeyField",strKeyName);
			CString strKeyValue=FormGetField(m_strScreenTag,strKeyName);
			//I uncommented below line because of Bugs-6-2021.6 
			//TODO : review it with Eng Hossam
			//if(strSubFormTag=="") // A Master Screen //26-12-2021
			{
				BOOL bIsValidRecord=m_pHSEMSPolyDb->ValidateEmptyFields(m_strScreenTag, strKeyName, strKeyValue, "", true);
				if(!bIsValidRecord)
				{
					AfxMessageBox("Please complete Master record before pressing button");
					bHandleCustomButton=false;
				}
			}
			//Also uncommented below line because of Bugs-6-2021.6 
			//else //A Detail Screen
			if(strSubFormTag!="")
			{
				CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
				CString strSQL;
				strSQL.Format("select LinkOtherField,UniqueFields from LSA where TableName='%s' and LinkName='%s' and ApplicationCode=%d",m_strScreenTag,strSubFormTag,APP_CODE);
				long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
				if (lCount != 0)
				{
					CString strLnkKeyName;
					FirstSysRecSet.GetFieldValue("LinkOtherField",strLnkKeyName);
					CString strUniqueFields;
					FirstSysRecSet.GetFieldValue("UniqueFields",strUniqueFields);
					CString strLnkKeyValue=FormGetField(strSubFormTag,strLnkKeyName);
					strUniqueFields.Replace(strLnkKeyName,"");
					strUniqueFields.Replace("|","");
					CString strUniqueValue="";
					if(strUniqueFields!="")
						strUniqueValue=FormGetField(strSubFormTag,strUniqueFields);
					if(strUniqueValue!="")
					{
						if (strSubFormTag!="HSE_TGENVMNTRPLANACTVTS" && strSubFormTag!="HSE_CHMCLRGSTR_QTYCRD" && strSubFormTag!="HSE_TGAWRNSPLNENTRY_CRS" )//Temp solution to bug Bugs-10-2021.16
						{
							//The function m_pHSEMSPolyDb->ValidateTabEmptyFields is not working correct Bugs-10-2021.42
							BOOL bIsValidRecord=true;//m_pHSEMSPolyDb->ValidateTabEmptyFields(m_strScreenTag, strKeyName, strKeyValue,strSubFormTag, strUniqueFields, strUniqueValue, "", true);
							if(!bIsValidRecord)
							{
								AfxMessageBox("Please complete record before pressing button");
								bHandleCustomButton=false;
							}
						}
					}
				}
			}
		}
		/*
		if(strSubFormTag=="")//You must check for master even if details is concerned
		{
			CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
			CString strSQL;
			strSQL.Format("select KeyField from TablesInfo where TableName='%s' and ApplicationCode=%d",m_strScreenTag,APP_CODE);
			long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);


			if (lCount != 0)
			{
				CString strKeyName;
				FirstSysRecSet.GetFieldValue("KeyField",strKeyName);
				CString strKeyValue=FormGetField(m_strScreenTag,strKeyName);
				BOOL bIsValidRecord=m_pHSEMSPolyDb->ValidateEmptyFields(m_strScreenTag, strKeyName, strKeyValue, "", true);
				if(!bIsValidRecord)
				{
					AfxMessageBox("Please complete record before pressing button");
					bHandleCustomButton=false;
				}
			}
		}
		else
		{
			/*
			Waiting for PolyDB new function to check for empty fields in a tab
			
			CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
			CString strSQL;
			strSQL.Format("select LinkOtherField from LSA where TableName='%s' and LinkName='%s' and ApplicationCode=%d",m_strScreenTag,strSubFormTag,APP_CODE);
			long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
			if (lCount != 0)
			{
				CString strKeyName;
				FirstSysRecSet.GetFieldValue("LinkOtherField",strKeyName);
				CString strKeyValue=FormGetField(strSubFormTag,strKeyName);
				BOOL bIsValidRecord=m_pHSEMSPolyDb->ValidateTabEmptyFields(strSubFormTag, strKeyName, strKeyValue, "", false);
				if(!bIsValidRecord)
				{
					AfxMessageBox("Please complete record before pressing button");
					bHandleCustomButton=false;
				}
			}

		}*/
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if(bHandleCustomButton)
		DisplayCustomButtonClicked(pCustomButtonClicked);

	delete pCustomButtonClicked->pMultiSelectedRows->pRow;
	delete pCustomButtonClicked->pMultiSelectedRows;
	delete pCustomButtonClicked;
}

void CCommonCategoryWrapper::BeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record)
{
	// DONE

	m_strScreenTag = m_strFormTag;

	CString strForm_Tag(Form_Tag);
	CString strLink_Tag(Link_Tag);
	CString strFieldName(FieldName);
	CString strCellContent(CellContent);
	CString strBrowsingTable(BrowsingTable);
	CString strBrowsingSQL(BrowsingSQL);
	CString strBrowsingField(BrowsingField);
	CString strBrowsingRetField(BrowsingRetField);
	CString strFull_Record(Full_Record);

	tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick = new tag_BeforBrowsingButtonClick;
	long iCancle=0;

    pBeforBrowsingButtonClick->Browse_Form_Tag	= CString("").AllocSysString();
    pBeforBrowsingButtonClick->Form_Tag			= strForm_Tag.AllocSysString();
    pBeforBrowsingButtonClick->SubForm_Tag		= strLink_Tag.AllocSysString();
	CString strFldName							= strFieldName.AllocSysString();
	strFldName.MakeUpper();
    pBeforBrowsingButtonClick->Field_Name		= strFldName.AllocSysString();
    pBeforBrowsingButtonClick->Current_Value	= strCellContent.AllocSysString();
    pBeforBrowsingButtonClick->Full_Record		= strFull_Record.AllocSysString();
    pBeforBrowsingButtonClick->iNavigate		= 0;
    pBeforBrowsingButtonClick->lCategoryID		= 0;
    pBeforBrowsingButtonClick->lOwnerApp		= 0;
    pBeforBrowsingButtonClick->lOwnerTransAction= 0;
    pBeforBrowsingButtonClick->iScreenMode		= 0;
	pBeforBrowsingButtonClick->iCancle			= &iCancle;

	pBeforBrowsingButtonClick->pBrowsingInfo = new tag_BROWSINGINFO;
    pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingTable		= strBrowsingTable.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->MainCriteria		= strBrowsingSQL.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL		= strBrowsingSQL.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->FilterSQL			= strBrowsingSQL.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingField		= strBrowsingField.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingRetField	= strBrowsingRetField.AllocSysString();
    pBeforBrowsingButtonClick->pBrowsingInfo->lCategoryID		= 0;
    pBeforBrowsingButtonClick->pBrowsingInfo->lOwnerApp			= 0;
    pBeforBrowsingButtonClick->pBrowsingInfo->lOwnerTransAction = 0;

	DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);

	if (iCancle == 1)
	{
		// ACTION SHOULD BE CANCELED - BUT NOT ALLOWED IN POLYDB
	}
	else if (CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL) != strBrowsingSQL)
	{
		if (strLink_Tag != "")
			LinkFieldsAttribSingle(strLink_Tag,	strFieldName,"",-1,"","","","",-1,CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingTable).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingField).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingRetField).GetBuffer(NULL),"",0,0,"");	
		else
			FieldSetAttribSingle(				strFieldName,"",-1,"","","","",-1,CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingTable).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingField).GetBuffer(NULL),CString(pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingRetField).GetBuffer(NULL),"",0,0,"");	
	}
	
	delete pBeforBrowsingButtonClick->pBrowsingInfo;
	delete pBeforBrowsingButtonClick;
}

void CCommonCategoryWrapper::MainSubReposition(CString strFormTag, long Main_Position, short Seleted_Tab)
{
	// DONE

	m_strScreenTag = m_strFormTag;

	tag_RecordRepostion *pRecordRepostion = new tag_RecordRepostion;
	
	long iCancle=0;
    pRecordRepostion->Form_Tag			= strFormTag.AllocSysString();
	
	//a.ali
    //pRecordRepostion->SubForm_Tag		= CString("").AllocSysString();=
	CString strSubFormTag = "";
	if(Seleted_Tab != -1) {
		strSubFormTag = GetTabTag(strFormTag.AllocSysString(),Seleted_Tab).AllocSysString();
		strSubFormTag.MakeUpper();
		pRecordRepostion->SubForm_Tag = strSubFormTag.AllocSysString();
	} else {
		pRecordRepostion->SubForm_Tag = CString("").AllocSysString();
	}
    pRecordRepostion->Full_Record		= CString("").AllocSysString();
    pRecordRepostion->Position			= Main_Position;
    pRecordRepostion->iActiveView		= 0;
    pRecordRepostion->iComplete			= 0;
    pRecordRepostion->iCancle			= &iCancle;
    pRecordRepostion->lCategoryID		= 0;
    pRecordRepostion->lOwnerApp			= 0;
    pRecordRepostion->lOwnerTransAction	= 0;
	
	//a.ali
	if(strSubFormTag != "" && m_strSlctdTab != strSubFormTag.AllocSysString()) {
		m_strSlctdTab = strSubFormTag.AllocSysString();
		tag_TabSelected *pTabSelected = new tag_TabSelected;
		pTabSelected -> SubForm_Tag   = strSubFormTag.AllocSysString();
		DisplayTabSelected(pTabSelected);
		delete pTabSelected;
	}


	// Fire Event Before
	DisplayRecordRepostion(pRecordRepostion);
	if (iCancle == 1)
	{
		// ACTION SHOULD BE CANCELED - BUT NOT ALLOWED IN POLYDB
	}
	else
	{
		// Fire Event After
		pRecordRepostion->iComplete			= 1;
		DisplayRecordRepostion(pRecordRepostion);
	}

	delete pRecordRepostion;
}

void CCommonCategoryWrapper::GridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record,LPCTSTR subForm_Tag)
{
	// DONE
	m_strScreenTag = m_strFormTag;
	if (AbsolutePosition == -1) // SIMULATE ADD NEW IN THE GRID
	{
		ConfirmToolBarButtonClicked(Form_Tag, Selected_Tab,"NEW",Full_Record,0,SUBTOOLBAR,subForm_Tag);
		ConfirmToolBarButtonClickedDone(Form_Tag, Selected_Tab,"NEW",Full_Record,0,SUBTOOLBAR,subForm_Tag);
	} else {
		tag_RecordRepostion *pRecordRepostion = new tag_RecordRepostion;
		long iCancle=0;												
		CString strFormTag                  = CString(Form_Tag).AllocSysString();
		strFormTag.MakeUpper();
		pRecordRepostion->Form_Tag			= strFormTag.AllocSysString();
		CString strSubFormTag				= CString(subForm_Tag).AllocSysString();		
		strSubFormTag.MakeUpper();
		pRecordRepostion->SubForm_Tag		= strSubFormTag.AllocSysString();
		pRecordRepostion->Full_Record		= CString(Full_Record).AllocSysString();
		pRecordRepostion->Position			= AbsolutePosition;
		pRecordRepostion->iActiveView		= 0;
		pRecordRepostion->iComplete			= 0;
		pRecordRepostion->iCancle			= &iCancle;
		pRecordRepostion->lCategoryID		= 0;
		pRecordRepostion->lOwnerApp			= 0;
		pRecordRepostion->lOwnerTransAction	= 0;

		// Fire Event Before
		DisplayRecordRepostion(pRecordRepostion);
		
		if (iCancle == 1)
		{
			// ACTION SHOULD BE CANCELED - BUT NOT ALLOWED IN POLYDB
		} else {
			// Fire Event After
			pRecordRepostion->iComplete			= 1;
			DisplayRecordRepostion(pRecordRepostion);
		}
		delete pRecordRepostion;
	}
}

void CCommonCategoryWrapper::OnBrowsableCustomButtonClicked(LPCTSTR Table, const VARIANT& Record, short CustomButtonID)
{
	m_strScreenTag = m_strFormTag;

}

void CCommonCategoryWrapper::ConfirmToolBarButtonClicked(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubFormTag="")
{
	// DONE
	m_bCancelOperation=FALSE;
	m_strScreenTag = m_strFormTag;
	//CString strSubFormTag = "";
	if (Toolbar_ID == 0)
		Seleted_Tab = 0;
	long iCancle=0;
	tag_ToolBarButtonClicked *pToolBarButtonClicked = new tag_ToolBarButtonClicked;
	CString strFormTag                  = CString(strForm_Tag).AllocSysString();
	strFormTag.MakeUpper();
	//pToolBarButtonClicked->Form_Tag		= strFormTag.AllocSysString();
	pToolBarButtonClicked->Form_Tag		= m_strScreenTag.AllocSysString();
	if(Seleted_Tab != -1) 
	{
		CString strSubFormTagTemp = GetTabTag(m_strFormTag,Seleted_Tab).AllocSysString();//Bugs-10-2021.12
		if(strSubFormTagTemp=="") //Hossam 6/9/21 to solve the problem of getting wrong subform tag when there is already a subform
			strSubFormTag="";
		strSubFormTag.MakeUpper();
	}		
	pToolBarButtonClicked->SubForm_Tag  = strSubFormTag.AllocSysString();
	CString strBtnName                  = strButton_Name.AllocSysString();
	strBtnName.MakeUpper();
	pToolBarButtonClicked->Button_Name  = strBtnName.AllocSysString();	    			
	//a.ali
	//pToolBarButtonClicked->Table_Name			= CString("").AllocSysString();
	CString strTableName						= GetTrueTableName(APP_CODE,m_strFormTag.AllocSysString(),strSubFormTag.AllocSysString());
	pToolBarButtonClicked->Table_Name			= strTableName.AllocSysString();
    pToolBarButtonClicked->Full_Record			= strFullRecord.AllocSysString();
    pToolBarButtonClicked->iComplete			= 0;
    pToolBarButtonClicked->iCancle				= &iCancle;
    pToolBarButtonClicked->lCategoryID			= 0;
    pToolBarButtonClicked->lOwnerApp			= 0;
    pToolBarButtonClicked->lOwnerTransAction	= 0;
    pToolBarButtonClicked->nRecordStatus		= 0;
    pToolBarButtonClicked->pMultiSelectedRows = new MultiSelectedRows;
	pToolBarButtonClicked->pMultiSelectedRows->lCount=1;
	pToolBarButtonClicked->pMultiSelectedRows->pRow = new SelectedRow[1];
	pToolBarButtonClicked->pMultiSelectedRows->pRow[0].strFullRecord = strFullRecord.AllocSysString();
	DisplayToolBarButtonClicked(pToolBarButtonClicked);
	if (iCancle == 1)
	{
		HandleActionEx(UNKNOWN,TRUE,0);
		// ACTION SHOULD BE CANCELED
	}
	delete pToolBarButtonClicked->pMultiSelectedRows->pRow;
	delete pToolBarButtonClicked->pMultiSelectedRows;
	delete pToolBarButtonClicked;
}

void CCommonCategoryWrapper::ConfirmToolBarButtonClickedDone(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubForm_Tag )
{
	// DONE

	m_bCancelOperation=FALSE;
	m_strScreenTag = m_strFormTag;
//	CString strSubForm_Tag = "";

	if (Toolbar_ID == 0)
		Seleted_Tab = 0;

	tag_ToolBarButtonClicked *pToolBarButtonClicked = new tag_ToolBarButtonClicked;
	
	long iCancle=0;
    pToolBarButtonClicked->Button_Name			= strButton_Name.AllocSysString();
	
	//a.ali
	CString strFormTag = strForm_Tag.AllocSysString();
	strFormTag.MakeUpper();
    pToolBarButtonClicked->Form_Tag				= strFormTag.AllocSysString();
	
	//a.ali
	strSubForm_Tag.MakeUpper();
	if(Seleted_Tab != -1) {
		CString strSubFormTagTemp = GetTabTag(strForm_Tag,Seleted_Tab).AllocSysString();//Bugs-10-2021.12
		if(strSubFormTagTemp=="") //Hossam 6/9/21 to solve the problem of getting wrong subform tag when there is already a subform
			strSubForm_Tag="";
		strSubForm_Tag.MakeUpper();
	}
	pToolBarButtonClicked->SubForm_Tag			= strSubForm_Tag.AllocSysString();

    //a.ali
	//pToolBarButtonClicked->Table_Name			= CString("").AllocSysString();
	CString strTableName						= GetTrueTableName(APP_CODE,m_strFormTag.AllocSysString(),strSubForm_Tag.AllocSysString());
	pToolBarButtonClicked->Table_Name			= strTableName.AllocSysString();

    pToolBarButtonClicked->Full_Record			= strFullRecord.AllocSysString();
    pToolBarButtonClicked->iComplete			= 1;
    pToolBarButtonClicked->iCancle				= &iCancle;
    pToolBarButtonClicked->lCategoryID			= 0;
    pToolBarButtonClicked->lOwnerApp			= 0;
    pToolBarButtonClicked->lOwnerTransAction	= 0;
    pToolBarButtonClicked->nRecordStatus		= 0;

    pToolBarButtonClicked->pMultiSelectedRows = new MultiSelectedRows;
	pToolBarButtonClicked->pMultiSelectedRows->lCount=1;
	pToolBarButtonClicked->pMultiSelectedRows->pRow = new SelectedRow[1];
	pToolBarButtonClicked->pMultiSelectedRows->pRow[0].strFullRecord = strFullRecord.AllocSysString();
	
	DisplayToolBarButtonClicked(pToolBarButtonClicked);

	delete pToolBarButtonClicked->pMultiSelectedRows->pRow;
	delete pToolBarButtonClicked->pMultiSelectedRows;
	delete pToolBarButtonClicked;
}

/////////////////////////////////////////////////////////////////////////////
///// HELPER FUNCTIONS //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CString CCommonCategoryWrapper::GetCurrentYear()
{
	CString strYear;
	int nYear = GetServerDate_DATE().GetYear();
	strYear.Format("%d",nYear);
	return strYear;
}

COleDateTime CCommonCategoryWrapper::GetServerDate_DATE()
{
	CString strSQL;
	CString strRetVal;

	CPolyDbRecordset DaoRecSet(PDBRS_LOAD_FROM_GMS_DB);
	
	strSQL.Format("SELECT CONVERT(VARCHAR(10), GETDATE(), 103) + ' ' + CONVERT(VARCHAR(10), GETDATE(), 108) TODAY");
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	DaoRecSet.GetFieldValue("TODAY",strRetVal);
	DaoRecSet.Close();

	COleDateTime dtDate;
	ParseDateTime(&dtDate,strRetVal);
	return dtDate;
}

HRESULT CCommonCategoryWrapper::SetValidationRule(CString strFormTag,CString strFieldName,CString ValidationRule)
{
	DISPLAYFIELDATTRIBINFO *tagFieldAttrib;
	tagFieldAttrib = new DISPLAYFIELDATTRIBINFO;

	if (m_strScreenTag == strFormTag)
		strFormTag = ""; //for tab

	tagFieldAttrib->AppCode = APP_CODE;
	tagFieldAttrib->TableName = m_strScreenTag.AllocSysString();
	tagFieldAttrib->strTagName = strFormTag.AllocSysString();
	tagFieldAttrib->FieldName = strFieldName.AllocSysString();
	tagFieldAttrib->ValidationRule=ValidationRule.AllocSysString();

	SetFieldAttrib(tagFieldAttrib);
	delete tagFieldAttrib;
	return S_OK;
}

void CCommonCategoryWrapper::SetFieldAttrib(DISPLAYFIELDATTRIBINFO *pFieldAttrib)
{
	pFieldAttrib->AppCode = APP_CODE;

	CString strTabTag(pFieldAttrib->strTagName);
	CString strFieldName(pFieldAttrib->FieldName);
	CString strAttribRule(pFieldAttrib->ValidationRule);

	if (strTabTag != "")
		LinkFieldsAttribSingle(strTabTag,	strFieldName,"",-1,"",strAttribRule,"","",-1,"","","","","",0,0,"");	
	else
		FieldSetAttribSingle(				strFieldName,"",-1,"",strAttribRule,"","",-1,"","","","","",0,0,"");	
}

CString CCommonCategoryWrapper::GetFieldValue(CString strFullRecord, CString strFieldName)
{
	CString strValue;
	if (strFullRecord == "")
		strValue = FormGetFieldEx(m_strFormTag,strFieldName);	
	else
		strValue = Find(strFullRecord, strFieldName);

	if (strValue == "True")
		strValue = "1";

	else if (strValue == "False")
		strValue = "0";

	//return strValue;
	return strValue.GetBuffer(0);
}

void CCommonCategoryWrapper::RefreshScreen(CString strSubFormTag,int nRefreshMode)
{
	if (strSubFormTag != "")
		SubMakeRefresh();
	else
		FormMakeRefresh(UNKNOWN);
}

void CCommonCategoryWrapper::LockRecord(CString strMainFormTag,CString strScreenTag,bool bLocked)
{
	if (strScreenTag == "")
		m_pHSEMSPolyDb->LockScreen(UNKNOWN,bLocked,bLocked);
	else
		m_pHSEMSPolyDb->LockRecord(UNKNOWN,FALSE,bLocked);
}

long CCommonCategoryWrapper::ShowMessageEx(UINT nMessageID, CString strButtons, int IconVal, CString strTitle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strMessage = GetResourceCaption(nMessageID,m_bLanguage);

	return ShowMessage(strMessage, strButtons, IconVal, "");
}

CString CCommonCategoryWrapper::GetConectionString()
{
	return m_pCategory->GetConnectionString();
}

bool CCommonCategoryWrapper::ParseDateTime(COleDateTime *pdtDate,CString strDate)
{
	return pdtDate->ParseDateTime(strDate,0,LANG_ARABIC);
}

CString CCommonCategoryWrapper::FormatDate(COleDateTime dtDate)
{
	return dtDate.Format("%d %b %Y");
}

CString CCommonCategoryWrapper::FormatDate(CString strDate)
{
	if (strDate == "")
		return "";

	COleDateTime dt;

	if (ParseDateTime(&dt,strDate))
		if (strDate.Find(":") != -1)// time supported
			return dt.Format("%d %b %Y %H:%M");
		else
			return dt.Format("%d %b %Y");
	else
		return "";
}

CString CCommonCategoryWrapper::GetRecordSetData (CString strSQL,long lRecordsCount)
{
	CString strRetVal="";
	CPolyDbRecordset PolydbRecSet(PDBRS_LOAD_FROM_GMS_DB);
	long lCount = PolydbRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lCount == 0)
		return strRetVal;
	
	while (!PolydbRecSet.IsEOF())
	{
		CString strRecord = PolydbRecSet.GetFullRecord();	
		if (strRecord.Right(1) == "|")
			strRecord = "ALIAS" + strRecord;
		CString strFieldName=strtok(strRecord.GetBuffer (0),"|");
		CString strValue=strtok(NULL,"|");	
		while (strValue != "")
		{
			strRetVal = strRetVal + strValue + "\1";
			strFieldName=strtok(NULL,"|");
			strValue=strtok(NULL,"|");
		}
		if (strRetVal.Right(1) == "\1")
			strRetVal = strRetVal.Left(strRetVal.GetLength()-1);		
		PolydbRecSet.MoveNext();
		if(!PolydbRecSet.IsEOF())
			strRetVal += "\2";
	}
	return strRetVal;
}


long CCommonCategoryWrapper::GetCharCount(CString strString,CString strSubString)
{
	return strString.Replace(strSubString,"");
}

int CCommonCategoryWrapper::ShowScreen(int nCurrentCard,CString strTableName,CString strTitle,int nStatus,BOOL bModal,CString strCriteria,CString strComesFrom,CString strScreenDefauls,CString strReturnBrowseValue,bool blocked,void* msg,int nScreenID,CCommonCategoryWrapper **pCommonCategoryWrapper)
{
	if(blocked) 
		return ShowListScreen(strTableName,strTitle,strCriteria,"","",FALSE,NULL,"");
	else 
 		return m_pCategory->GetHSEObj()->DoFunction(nCurrentCard,strTableName,nStatus,bModal,strCriteria,strComesFrom,strScreenDefauls,strReturnBrowseValue,nScreenID,msg,pCommonCategoryWrapper);	
}

VOID CCommonCategoryWrapper::ShowList(UINT unMenuId,CString strSQL,long lParam,CString strDefValues)
{
	// DONE
	CString strFormTag="";
	if (unMenuId == 0)
	{
		strFormTag = strSQL;
	}

	BOOL bModal = FALSE;
	//this mean we called internal from show list method
	if(lParam > 1) 
	{
		//this mean we has SQL for open these screen and open in list mode not in filter mode
		if(lParam > 3) 
			strSQL = *((CString*)lParam);
		bModal = TRUE;
	}
	m_pCategory->GetHSEObj()->DoFunction(unMenuId,strFormTag,NORMAL_MODE,bModal,strSQL,"",strDefValues,"");	
}

bool CCommonCategoryWrapper::DoToolBarAction(long lButtonKey,CString strScreenTag,CString strSubFormTag)
{	
	if(strScreenTag.IsEmpty())
		strScreenTag	=	m_strScreenTag;

	if (strSubFormTag != "")
	{
		strSubFormTag = "DUMMY STRING"; //POLYDB WILL UPDATE THE CURRENT GRID ANYWAY
		FormUpdateGrid(strSubFormTag,GetFormType());
		FormDoAction(-2, GetButtonID(lButtonKey));
	}
	else
		FormDoAction(GetFormType(), GetButtonID(lButtonKey));

	return 0;
}

void CCommonCategoryWrapper::EnableTab(CString strTabTag,BOOL bStatus)
{
	TabEnableEx(strTabTag,bStatus);
}

void CCommonCategoryWrapper::EnableToolbar(CString strSubFormTag,long lButtonID,bool bEnabled)
{
	int nToolbar = 0;
	if (strSubFormTag != "")
	{
		int nForType=GetFormType();
		if(nForType==3)//Trying to handle toolbar of details while still in the list mode
			return;
		nToolbar=1;
	}
	
	FormEnableButton(UNKNOWN, nToolbar, GetButtonName(lButtonID), bEnabled, TRUE);
//	FormEnableButton(BROWSABLE, nToolbar, GetButtonName(lButtonID), bEnabled, TRUE);
//	FormEnableButton(ONETOMANY, nToolbar, GetButtonName(lButtonID), bEnabled, TRUE);
}

void CCommonCategoryWrapper::EnableToolbar(CString strScreenTag,CString strSubFormTag,long lButtonID,bool bEnabled)
{
	int nToolbar = 0;
	if (strSubFormTag != "")
		nToolbar=1;

	if (nToolbar == 0)
		FormEnableButton(BROWSABLE, nToolbar, GetButtonName(lButtonID), bEnabled, TRUE);
	FormEnableButton(ONETOMANY, nToolbar, GetButtonName(lButtonID), bEnabled, TRUE);
}

void CCommonCategoryWrapper::SetScreenCaption(CString strTag,CString strCaption)
{
	FormChangeCaption(UNKNOWN,strCaption);
}

CString CCommonCategoryWrapper::GetFullDateTime(CString strFormat)
{
	CString strDateTime;
	strDateTime = GetServerDate_STRING(strFormat) + " " + GetServerTime();
	return strDateTime;
}

CString CCommonCategoryWrapper::GetServerDate_STRING(CString strFormat)
{
	CString strDate;
	COleDateTime dt = GetServerDate_DATE();
	
	if(dt.m_status == 0 )
		strDate = dt.Format(strFormat);

	return strDate;
}

double CCommonCategoryWrapper::GetServerDate_DOUBLE()
{
	COleDateTime dtDate = GetServerDate_DATE();
	return dtDate.m_dt;
}

CString CCommonCategoryWrapper::GetServerTime()
{
	CString strTime;
	COleDateTime dtDate = GetServerDate_DATE();

	strTime.Format("%d:%d:%d", dtDate.GetHour(),dtDate.GetMinute(),dtDate.GetSecond());

	return strTime;
}

void CCommonCategoryWrapper::ChangeCriteria(CString strTag,CString strSubTag,CString strCriteria)
{
	//Testing Bugs-3-2021.104 "It works when commenting the next line and use m_strBuildStatement in CCommonCategory::ParentManage"
	ChangeFormSQL(strCriteria);
}

HRESULT CCommonCategoryWrapper::ShowListScreen(CString strList_Tag,CString strList_Title,CString strSQL_Statement,CString strKeyField,CString strRecKeyValue,BOOL bMultiSelection,tagMultiSelectedRows* pstrFieldsValues,CString strScreenAttribute)
{
		/*	if (strScreenAttribute.Find("LOCKED") == -1)
			{
				ShowList(0,strList_Tag,(long)&strSQL_Statement,"");
			}
			else
			{
		*/		
		//pstrFieldsValues->lCount = 0;
		int nHelpID;
		if (strSQL_Statement == "")
		{
			CString strReportFile,strStatusField,strTrueTableName,strUniqueFields,strOrderFields;
			BOOL bShowEmptyList,m_bOneToMany;
			//GetScreenData(strList_Tag,strList_Title,strReportFile,m_bOneToMany,strUniqueFields,strOrderFields,nHelpID,strKeyField,bShowEmptyList,strStatusField,strTrueTableName);
			int nMaxRec = 0;
			GetScreenData(strList_Tag,strList_Title,strReportFile,m_bOneToMany,strUniqueFields,strOrderFields,nHelpID,strKeyField,bShowEmptyList,strStatusField,strTrueTableName,nMaxRec);
			strSQL_Statement.Format("SELECT * FROM %s ORDER BY %s",strTrueTableName,strKeyField);
		}
		CString strValues = ListShowEx(strList_Tag, strList_Title, strSQL_Statement, strKeyField, strRecKeyValue, bMultiSelection, strList_Tag,nHelpID);
		return S_FALSE;
		/*if (strValues == "")
			return S_FALSE;

		if (strValues.Right(1) == "|")
			strValues = strValues.Left(strValues.GetLength()-1);

		int nCount = GetCharCount(strValues,"|") + 1;

		strValues = "|" + strValues;
		strValues.Replace("|","|" + strKeyField + "|");
		strValues = strValues.Mid(1);

		pstrFieldsValues->lCount = nCount;
		pstrFieldsValues->pRow  = new tagSelectedRow[nCount]; 
		
		int nCounter=0;
		CString strFieldName=strtok(strValues.GetBuffer (0),"|");
		CString strValue=strtok(NULL,"|");
		while (strValue != "")
		{
			CString strFullRecord = strFieldName + "|" + strValue + "|";
			pstrFieldsValues->pRow[nCounter].strFullRecord = strFullRecord.AllocSysString();
			strFieldName=strtok(NULL,"|");
			strValue=strtok(NULL,"|");
			nCounter++;
		}*/
//	}
}

long CCommonCategoryWrapper::ShowMessage(CString strMessage, CString strButtons, int IconVal, CString strTitle)
{
	if (strMessage == "")
		return 0;
	if (m_bLanguage)
		strTitle = "(HSEMS®)ÈÑäÇãÌ ÇÏÇÑÉ ÇáÓáÇãÉ æ ÇáÕÍÉ ÇáãåäíÉ";
	else
		strTitle = "Health, Safety and Environmental Management System (HSEMS®)";
	strButtons.MakeUpper();
	long lMessageType;
	if (strButtons == "YES|NO")
		lMessageType = MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL;
	else
		lMessageType = MB_OK | MB_ICONINFORMATION | MB_TASKMODAL;
	return MessageBox(NULL,strMessage,strTitle,lMessageType);
}

CString CCommonCategoryWrapper::GetSetting(CString strKey, CString strSubKey, CString strValueName,CString strDefaultData , CString strProfile)
{
	CString strValue = (char*)m_pCategory->GetHSEObj()->m_pRegHandle->GetSetting(strKey.AllocSysString(),strSubKey.AllocSysString(),strValueName.AllocSysString(),strDefaultData.AllocSysString());

	return strValue;
}

CString CCommonCategoryWrapper::itoa(long iValue)
{
	CString strValue;
	strValue.Format("%ld",iValue);
	return strValue;
}

CString CCommonCategoryWrapper::GetUserLogin()
{
	return m_pCategory->GetUserLogin();
}

void CCommonCategoryWrapper::SaveSetting(CString strKey, CString strSubKey, CString strValueName, CString strData, CString strProfile)
{
	m_pCategory->GetHSEObj()->m_pRegHandle->SaveSetting(strKey.AllocSysString(),strSubKey.AllocSysString(),strValueName.AllocSysString(),strData.AllocSysString());
}

CString CCommonCategoryWrapper::GetTabTag(CString strForm_Tag,CString strTabTableName)
{
	CString strTabTag;
	CString strSQL;
	
	strSQL.Format("SELECT LinkName FROM LSA WHERE TableName='%s' AND LinkOtherTable='%s' AND ApplicationCode=%d",strForm_Tag,strTabTableName,APP_CODE);

	CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);

	long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);

	if (lCount == 0)
		return "";

	FirstSysRecSet.GetFieldValue("LINKNAME",strTabTag);

	return strTabTag;
}

CString CCommonCategoryWrapper::GetTabTag(CString strForm_Tag,int nTabIndex)
{
	if (nTabIndex == 0)
		return "";

	CString strTabTag;
	CString strSQL;
	
	strSQL.Format("SELECT LINKNAME,TABLEINDEX FROM LSA WHERE TABLENAME = '%s' AND APPLICATIONCODE = %d ORDER BY TableIndex",strForm_Tag,APP_CODE);

	CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);

	long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);

	if (nTabIndex > lCount)
		return "";

	FirstSysRecSet.SetAbsolutePosition(nTabIndex-1);
	FirstSysRecSet.GetFieldValue("LINKNAME",strTabTag);

	return strTabTag;
}

CString CCommonCategoryWrapper::GetButtonName(long lButtonID)
{
	CString strButtonName;

	switch(lButtonID)
	{
		case TOOLBAR_NEW:
			strButtonName = "NEW";
			break;
		case TOOLBAR_SAVE:		
			strButtonName = "SAVE";
			break;
		case TOOLBAR_CANCEL:		
			strButtonName = "CANCEL";
			break;
		case TOOLBAR_DELETE:		
			strButtonName = "DELETE";
			break;
		case TOOLBAR_PRINT:		
			strButtonName = "PRINT";
			break;
		case TOOLBAR_MOVEFIRST:	
			strButtonName = "FIRST";
			break;
		case TOOLBAR_MOVEPREV:	
			strButtonName = "PREVIOUS";
			break;
		case TOOLBAR_MOVENEXT:	
			strButtonName = "NEXT";
			break;
		case TOOLBAR_MOVELAST:	
			strButtonName = "LAST";
			break;
		case TOOLBAR_FIND:		
			strButtonName = "FIND";
			break;
		case TOOLBAR_UNFILTER:	
			strButtonName = "REMOVEFILTER";
			break;
		case TOOLBAR_FILTER:		
			strButtonName = "FILTER";
			break;
		case TOOLBAR_STOP:		
			strButtonName = "";
			break;
		case TOOLBAR_CHANGECOLOR:	
			strButtonName = "";
			break;
		case TOOLBAR_EXIT:		
			strButtonName = "EXIT";
			break;
		case TOOLBAR_SELECT:		
			strButtonName = "SELECT";
			break;
		case TOOLBAR_SHOW_DETAILS:
			strButtonName = "";
			break;
		case TOOLBAR_FILTERCURRENT:
			strButtonName = "SELECTCURRENT";
			break;
		case TOOLBAR_SORT:		
			strButtonName = "SORT";
			break;
		case TOOLBAR_COPY:		
			strButtonName = "COPY";
			break;
		case TOOLBAR_PASTE:		
			strButtonName = "PASTE";
			break;
	}
	return strButtonName;
}

long CCommonCategoryWrapper::GetButtonID(long lButtonID)
{
	switch(lButtonID)
	{
		case TOOLBAR_NEW:
			lButtonID = NEW_BUTTON;
			break;
		case TOOLBAR_SAVE:		
			lButtonID = SAVE_BUTTON;
			break;
		case TOOLBAR_CANCEL:		
			lButtonID = CANCEL_BUTTON;
			break;
		case TOOLBAR_DELETE:		
			lButtonID = DELETE_BUTTON;
			break;
		case TOOLBAR_PRINT:		
			lButtonID = PRINT_BUTTON;
			break;
		case TOOLBAR_MOVEFIRST:	
			lButtonID = 0;
			break;
		case TOOLBAR_MOVEPREV:	
			lButtonID = PREVIOUS_BUTTON;
			break;
		case TOOLBAR_MOVENEXT:	
			lButtonID = NEXT_BUTTON;
			break;
		case TOOLBAR_MOVELAST:	
			lButtonID = 0;
			break;
		case TOOLBAR_FIND:		
			lButtonID = FIND_BUTTON;
			break;
		case TOOLBAR_UNFILTER:	
			lButtonID = 0;
			break;
		case TOOLBAR_FILTER:		
			lButtonID = 0;
			break;
		case TOOLBAR_STOP:		
			lButtonID = 0;
			break;
		case TOOLBAR_CHANGECOLOR:	
			lButtonID = 0;
			break;
		case TOOLBAR_EXIT:		
			lButtonID = EXIT;
			break;
		case TOOLBAR_SELECT:		
			lButtonID = 0;
			break;
		case TOOLBAR_SHOW_DETAILS:
			lButtonID = 0;
			break;
		case TOOLBAR_FILTERCURRENT:
			lButtonID = 0;
			break;
		case TOOLBAR_SORT:		
			lButtonID = SORT_BUTTON;
			break;
		case TOOLBAR_COPY:		
			lButtonID = COPY_BUTTON;
			break;
		case TOOLBAR_PASTE:		
			lButtonID = 0;
			break;
	}
	return lButtonID;
}

//a.ali
CString CCommonCategoryWrapper::GetTrueTableName(int nAppCode,CString strFormTag,CString strSubFormTag) {		
	CString strTableName;
	CString strSQL;
	
	if(strSubFormTag == "") {
		strSQL.Format("SELECT TrueTableName AS TABLE_NAME FROM TablesInfo WHERE TableName='%s' AND ApplicationCode=%d",strFormTag,nAppCode);		
	} else {
		strSQL.Format("SELECT LinkOtherTable AS TABLE_NAME FROM LSA WHERE LinkName='%s' AND ApplicationCode=%d",strSubFormTag,nAppCode);
	}
	CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
	long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lCount == 0)
		return "";
	FirstSysRecSet.GetFieldValue("TABLE_NAME",strTableName);
	return strTableName;
}

//a.ali 
//get screen/tab caption by Form tag or sub Form Tag
CString CCommonCategoryWrapper::GetScrCptnByTag(int nAppCode,CString strFormTag,CString strSubFormTag) {		
	CString strCaption;
	CString strSQL;	
	if(strSubFormTag == "") {
		strSQL.Format("SELECT EnglishCaption AS caption FROM TablesInfo WHERE TableName='%s' AND ApplicationCode=%d",strFormTag,nAppCode);		
	} else {
		strSQL.Format("SELECT LinkName AS caption FROM LSA WHERE LinkName='%s' AND ApplicationCode=%d",strSubFormTag,nAppCode);
	}
	CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
	long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lCount == 0)
		return "";
	FirstSysRecSet.GetFieldValue("caption",strCaption);
	return strCaption;
}

//a.ali
//Check if field exist in header screen
//@param int nAppCode
//@param strFormTag
//return true if field found,false otherwise
bool CCommonCategoryWrapper::IsFieldExist(int nAppCode,CString strFormTag,CString strSubFormTag,CString strFieldName) {		
	bool bFieldExist = false;
	CString strSQL;
	if(strSubFormTag == "") {
		strSQL.Format("SELECT DataFieldName AS fieldExist FROM FSA WHERE TableName='%s' AND ApplicationCode=%d AND DataFieldName='%s'",strFormTag,nAppCode,strFieldName);		
	} else {
		strSQL.Format("SELECT LinkName AS fieldExist FROM LFA WHERE LinkName='%s' AND ApplicationCode=%d AND LinkName='%s'",strSubFormTag,nAppCode,strFieldName);
	}
	CPolyDbRecordset FirstSysRecSet(PDBRS_LOAD_FROM_SYS_DB);
	long lCount = FirstSysRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lCount != 0) {
		bFieldExist = true;
	}
	return bFieldExist;
}

void CCommonCategoryWrapper::InitDefaultValue(int nScreenCount)
{/*no need for this method*/}

void CCommonCategoryWrapper::InitScreenDefaultValue(int nScreenIndex,CString strScreenTag,int nFieldsCount)
{
	m_strDefaultValueScreenTag = strScreenTag;
}

void CCommonCategoryWrapper::AddFieldDefaultValue(int nScreenIndex,int nFieldIndex,CString strFieldName,CString strDefaultValue)
{
	SetDefaultValue(m_strDefaultValueScreenTag,strFieldName,strDefaultValue);
}

void CCommonCategoryWrapper::SetDefaultValue(CString strFormTag,CString strFieldName,CString strFieldValue)
{
	if (m_strScreenTag == m_strDefaultValueScreenTag)
		FieldSetAttribSingle(								strFieldName,"",-1,"","","",strFieldValue,-1,"","","","","",0,0,"");
	else
		LinkFieldsAttribSingle(m_strDefaultValueScreenTag,	strFieldName,"",-1,"","","",strFieldValue,-1,"","","","","",0,0,"");
}

void CCommonCategoryWrapper::SetFieldVisibleAttrib(CString strScreenTag,CString strFieldName,CString strAttribute)
{
	m_strScreenTag.MakeUpper();
	strScreenTag.MakeUpper();

	if (m_strScreenTag == strScreenTag)
		FieldSetAttribSingle(				strFieldName,"",-1,"",strAttribute,"","",-1,"","","","","",0,0,"");
	else
		LinkFieldsAttribSingle(strScreenTag,strFieldName,"",-1,"",strAttribute,"","",-1,"","","","","",0,0,"");
}

void CCommonCategoryWrapper::EnableTab(CString strScreenTag,CString strSubFormTag,bool bEnabled)
{
	EnableToolbar(strScreenTag,strSubFormTag,TOOLBAR_NEW,bEnabled);
	EnableToolbar(strScreenTag,strSubFormTag,TOOLBAR_DELETE,bEnabled);
	EnableToolbar(strScreenTag,strSubFormTag,TOOLBAR_SAVE,bEnabled);
	EnableToolbar(strScreenTag,strSubFormTag,TOOLBAR_COMMON,bEnabled);
}


void CCommonCategoryWrapper::UpdateValidation(CString strApp,CString strTag,CString strLinkName,CString strField,CString strValidation,CString strUpdatedFieldName)
{
	//_DB * pdb;
	strValidation.Replace("  " ," ");	
	CString strUpdatedTable,strUpdateSQL;
	if(strLinkName == "") {
		strUpdatedTable = "ClientFSA";
		strUpdateSQL.Format("UPDATE %s SET %s = '%s' WHERE ApplicationCode = %s AND TableName = '%s' AND DataFieldName = '%s'",strUpdatedTable,strUpdatedFieldName,strValidation,strApp,strTag,strField);
	} else {
		strUpdatedTable = "ClientLFA";
		strUpdateSQL.Format("UPDATE %s SET %s = '%s' WHERE ApplicationCode = %s AND TableName = '%s' AND LinkName = '%s' AND DataFieldName = '%s'",strUpdatedTable,strUpdatedFieldName,strValidation,strApp,strTag,strLinkName,strField);
	}
	long lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql((LPCSTR)strUpdateSQL,PDBRS_LOAD_FROM_SYS_DB,0);
	if (lRetFieldsCount == 0)
	{
		if(strLinkName == "") {
			strUpdateSQL.Format("INSERT INTO %s(ApplicationCode,TableName,DataFieldName,%s) VALUES('%s','%s','%s','%s')",strUpdatedTable,strUpdatedFieldName,strApp,strTag,strField,strValidation);
		} else {
			strUpdateSQL.Format("INSERT INTO %s(ApplicationCode,TableName,LinkName,DataFieldName,%s) VALUES('%s','%s','%s','%s','%s')",strUpdatedTable,strUpdatedFieldName,strApp,strTag,strLinkName,strField,strValidation);
		}
		lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql((LPCSTR)strUpdateSQL,PDBRS_LOAD_FROM_SYS_DB,0);
	}
}

void CCommonCategoryWrapper::UpdateTabAttrib(CString strApp,CString strTag,CString strLinkName,CString strField,CString strUpdatedFieldVal) {
	CString strUpdateSQL;
	strUpdateSQL.Format("UPDATE ClientLSA SET %s = '%s' WHERE ApplicationCode = %s AND TableName = '%s' AND LinkName = '%s'",strField,strUpdatedFieldVal,strApp,strTag,strLinkName);
	long lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql((LPCSTR)strUpdateSQL,PDBRS_LOAD_FROM_SYS_DB,0);
	if (lRetFieldsCount == 0)
	{
		strUpdateSQL.Format("INSERT INTO ClientLSA(ApplicationCode,TableName,LinkName,%s) VALUES('%s','%s','%s','%s')",strField,strApp,strTag,strLinkName,strUpdatedFieldVal);
		lRetFieldsCount = m_pHSEMSPolyDb->DBExecSql((LPCSTR)strUpdateSQL,PDBRS_LOAD_FROM_SYS_DB,0);
	}
}