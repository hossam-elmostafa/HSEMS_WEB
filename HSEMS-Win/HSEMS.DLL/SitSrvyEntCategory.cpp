#include "stdafx.h"
#include "HSEMS.h"
#include "SitSrvyEntCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SitSrvyEntCategory::SitSrvyEntCategory(CCategory *pCategory):SitSrvyCategory(pCategory) {
	m_pCategory = pCategory;
}

SitSrvyEntCategory::~SitSrvyEntCategory() {}

long SitSrvyEntCategory::ShowSitSrvyEnt(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SitSrvyEnt;
	return ParentManage("HSE_TgSitSrvyEnt",TRUE,TRUE,TRUE,"HSE_SitSrvyEnt",strCriteria);
}

HRESULT SitSrvyEntCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{			
	CString strBtnName(pCustomButtonClicked->Button_Name);
	if(strBtnName=="SITSRVYENT_ENTCMPLT") {
		int Count = 0;		
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompleteSitSrvy(pCustomButtonClicked);
	} 
	return SitSrvyCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/***********************************************************************************************************
 *Helper functions
 ***********************************************************************************************************/
void SitSrvyEntCategory::CompleteSitSrvy(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	//Correct bug Bugs-2-2021.39
	//The name of the SP was missing in the following line
	strSQL.Format("EXECUTE	completeSitSrvyTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

