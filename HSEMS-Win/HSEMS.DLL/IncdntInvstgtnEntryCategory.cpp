#include "stdafx.h" 
#include "HSEMS.h"
#include "IncdntInvstgtnEntryCategory.h"
#include "HSEobj.h"

IncdntInvstgtnEntryCategory::IncdntInvstgtnEntryCategory(CCategory *pCategory):AccidentCategory(pCategory) {
	m_pCategory = pCategory;
}

IncdntInvstgtnEntryCategory::~IncdntInvstgtnEntryCategory(){}


HRESULT IncdntInvstgtnEntryCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{	
	if(allowIncdntInvstgtnEntry()=='Y')
	{
		return AccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{		
		ShowMessageEx(IDS_VACR,"",IV_NOICON,"Policy Change Required");
		return S_FALSE;
	}
}


HRESULT IncdntInvstgtnEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	return AccidentCategory::DisplayScreenReady(pScreenReady);
}

HRESULT IncdntInvstgtnEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strButtonName=="ACDNTENT_ENTRYCMPLTD")
	{
		CompleteAccident(pCustomButtonClicked);
	}
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT IncdntInvstgtnEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{	
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strBtnName(pToolBarButtonClicked->Button_Name);		
	if(strBtnName=="DELETE")
	{
		//TODO : this should by 1 to complete process but I can't get key after record is deleted 
		//so I changed it to be 0 but need review
		if(pToolBarButtonClicked->iComplete == 0) {
			CString strPrmryKey = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
			runAftrDltIncdntInvstgtn(strPrmryKey);
		}
	}
	return AccidentCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

long IncdntInvstgtnEntryCategory::ShowIncdntInvstgtnEntry(CString strCriteria)
{	if(allowIncdntInvstgtnEntry()=='Y') {
		m_nCurrentCard=ID_HSE_AcdntIncdntInvstgtnEntry;
		return ParentManage("HSE_TgIncdntInvstgtnEntry",TRUE,FALSE,FALSE,"HSE_ACDNTENT",strCriteria);	
	} else {		
		ShowMessageEx(IDS_INCDNT_INVSTGTN_RQRD,"",IV_NOICON,"Policy Change Required");
		return -1;
	}
}

CString IncdntInvstgtnEntryCategory::allowIncdntInvstgtnEntry(){
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_ISINCDNTINVSTGTNRQRD,'') FROM HSE_HSEPLC");
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctMonth;
}

void IncdntInvstgtnEntryCategory::runAftrDltIncdntInvstgtn(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE RunAftrDltIncdntInvstgtn '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void IncdntInvstgtnEntryCategory::CompleteAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
	strSQL.Format("EXECUTE completeAccidentTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}