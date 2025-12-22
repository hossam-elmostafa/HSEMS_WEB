#include "stdafx.h" 
#include "HSEMS.h"
#include "IncdntInvstgtnAprvCategory.h"
#include "HSEobj.h"

IncdntInvstgtnAprvCategory::IncdntInvstgtnAprvCategory(CCategory *pCategory):AccidentCategory(pCategory) {
	m_pCategory = pCategory;
}

IncdntInvstgtnAprvCategory::~IncdntInvstgtnAprvCategory(){}

HRESULT IncdntInvstgtnAprvCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	return AccidentCategory::DisplayScreenReady(pScreenReady);
}

HRESULT IncdntInvstgtnAprvCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strButtonName=="ACDNTENT_CLS")
	{
		CloseAccident(pCustomButtonClicked);
	} else if (strButtonName=="ACDNTENT_RJC") {
		RejectAccident(pCustomButtonClicked); 
	}
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT IncdntInvstgtnAprvCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strSubFormTag=="HSE_TGINCDNTINVSTGTNAPRVLRECMNDACC")
		{	
			CString strCARNo = FormGetField("HSE_ACDNTENTRECMNDACC","ACDNTENTRECMNDACC_CARNO");
			if(strCARNo != "0") {
				EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,FALSE);
				LockRecord(strFormTag,strSubFormTag,true);
			}
		}	
	}
	return AccidentCategory::DisplayRecordRepostion(pRecordRepostion);
}

long IncdntInvstgtnAprvCategory::ShowIncdntInvstgtnAprv(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_AcdntIncdntInvstgtnAprv;
	return ParentManage("HSE_TGINCDNTINVSTGTNAPRVL",FALSE,FALSE,FALSE,"HSE_ACDNTENT",strCriteria);	
}

void IncdntInvstgtnAprvCategory::CloseAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");//GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE closeAccidentTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void IncdntInvstgtnAprvCategory::RejectAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked) 
{
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");
	CString KeyFieldValue = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");//GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE rejectIncident '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);		
	RefreshScreen("",REFRESH_SELECTED);	
}