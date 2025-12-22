#include "stdafx.h"
#include "HSEMS.h"
#include "SitSrvyFlwupCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SitSrvyFlwupCategory::SitSrvyFlwupCategory(CCategory *pCategory):SitSrvyCategory(pCategory) {
	m_pCategory = pCategory;
}

SitSrvyFlwupCategory::~SitSrvyFlwupCategory() {}

long SitSrvyFlwupCategory::ShowSitSrvyFlwup(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SitSrvyFlwup;
	return ParentManage("HSE_TgSitSrvyFlwup",TRUE,TRUE,TRUE,"HSE_SitSrvyEnt",strCriteria);
}

HRESULT SitSrvyFlwupCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return SitSrvyCategory::DisplayScreenReady(pScreenReady);
}

HRESULT SitSrvyFlwupCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strButtonName=="SITSRVYENT_CLS") {
		CloseSiteSurveyTXN(pCustomButtonClicked);				
	} 
	return SitSrvyCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void SitSrvyFlwupCategory::CloseSiteSurveyTXN(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE closeSiteSurveyTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}