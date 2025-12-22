#include "stdafx.h"
#include "HSEMS.h"
#include "SitSrvyCnfrmtnCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SitSrvyCnfrmtnCategory::SitSrvyCnfrmtnCategory(CCategory *pCategory):SitSrvyCategory(pCategory) {
	m_pCategory = pCategory;
}

SitSrvyCnfrmtnCategory::~SitSrvyCnfrmtnCategory() {}

long SitSrvyCnfrmtnCategory::ShowSitSrvyCnfrmtn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SitSrvyCnfrmtn;
	return ParentManage("HSE_TgSitSrvyCnfrmtn",TRUE,TRUE,TRUE,"HSE_SitSrvyEnt",strCriteria);
}

HRESULT SitSrvyCnfrmtnCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	if(strButtonName=="SITSRVYENT_CNFRM")
	{
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			confirmSiteSrvy(pCustomButtonClicked);
	} 
	return SitSrvyCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT SitSrvyCnfrmtnCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return SitSrvyCategory::DisplayScreenReady(pScreenReady);
}

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void SitSrvyCnfrmtnCategory::confirmSiteSrvy(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE confirmSiteSrvy '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}