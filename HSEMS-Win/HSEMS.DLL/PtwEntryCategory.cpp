// PtwEntryCategory.cpp: implementation of the PtwEntryCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "PtwEntryCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PtwEntryCategory::PtwEntryCategory(CCategory *pCategory):PtwCategory(pCategory)
{
	m_pCategory = pCategory;
}
PtwEntryCategory::~PtwEntryCategory(){}
/*************************************************************************************************************/
HRESULT PtwEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//501_440_1
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="PTWSRGSTRENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompletePTW(pCustomButtonClicked);
	}
	return PtwCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT PtwEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//filter detail browsing field according to main browsing field 
	//note : not in specs but its logical
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	//501_440_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="PTWSRGSTRENT_LOCTYP") {
		CString strSite=FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_SIT");								
		if(strSite.IsEmpty())
			strSite=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSite);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//501_440_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="PTWSRGSTRENT_WRKLOC") {
		CString strLocTyp=FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_LOCTYP");
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	} else if(strFieldName=="PTWSRGSTRENTRELPTWS_RELPTWSNUM") {
		CString strRelPtwsNum=FormGetField("HSE_PTWSRGSTRENTRELPTWS","PTWSRGSTRENTRELPTWS_LNK");
		CString strSQL;
		strSQL.Format("WHERE (PTWSRGSTRENT_PTWSNUM <>'%s' and (PTWSRGSTRENT_RECSTS = 5 or PTWSRGSTRENT_RECSTS = 6))",strRelPtwsNum);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}
	return PtwCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT PtwEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{		
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	/*if(strFieldName == "PTWSRGSTRENT_PRFRMRPRTY")
	{
		FormSetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_DEPT","");
		FormSetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_CNTRCTORTHRDPRT","");
		if((FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PRFRMRPRTY") == "2")|| (FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PRFRMRPRTY") == "3") ) {
			SetFieldVisibleAttrib("HSE_TGPTWSRGSTRENT","ABC[PTWSRGSTRENT_DEPT=HSE_DPRT.DPRT_COD][DPRT_DESC]","HIDDEN");
		} else {
			SetFieldVisibleAttrib("HSE_TGPTWSRGSTRENT","ABC[PTWSRGSTRENT_DEPT=HSE_DPRT.DPRT_COD][DPRT_DESC]","ALWAYSLOCKED");
			FormSetField("HSE_PTWSRGSTRENT","ABC[PTWSRGSTRENT_DEPT=HSE_DPRT.DPRT_COD][DPRT_DESC]","");
		}
	}*/
	return PtwCategory::DisplayFieldChange (pFieldChange);
}
/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void PtwEntryCategory::CompletePTW(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	/*Deleted Code,Please Check Source Safe*/
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE completePTWTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

long PtwEntryCategory::ShowPTWRegisterEntry(CString strCriteria,CString strScreenDefaults)
{
	m_nCurrentCard=ID_HSE_PTWSRGSTRENT;		
	if(strScreenDefaults != "") {
		m_bEditMode=true;
		//m_bAddNewMode=true;
		//m_bOpenList=false;
		m_pCategory->SetScreenDefauls(strScreenDefaults);
	}	
	return ParentManage("HSE_TGPTWSRGSTRENT",TRUE,TRUE,TRUE,"HSE_PTWSRGSTRENT",strCriteria);
}