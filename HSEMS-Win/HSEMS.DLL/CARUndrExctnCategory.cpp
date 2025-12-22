// CARUndrExctnCategory.cpp: implementation of the CARUndrExctnCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARUndrExctnCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARUndrExctnCategory::CARUndrExctnCategory(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGCRCTVEACCUNDREXCTN";
	m_strScreen	   = "Corrective Actions Under Execution";
}

CARUndrExctnCategory::~CARUndrExctnCategory()
{

}

HRESULT CARUndrExctnCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);	
	if(strBtnName=="CRCTVEACCENT_JOBCMPLTD")
	{		
		CARJobCmpltd(pCustomButtonClicked);
		RefreshScreen("",REFRESH_SELECTED);
	}	
	else if(strBtnName=="CRCTVEACCENT_RJCT_UNDREXCTN")
	{
		if(OpenReasonsScr("Reject Reasons","3",true)){
			RejectCorrectiveAction(pCustomButtonClicked);
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	else if(strBtnName=="CRCTVEACCENT_JOBPNDNG")
	{
		CString strCARState = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_RECSTS");
		if(strCARState != "26") {
			if(OpenReasonsScr("Pending Reasons","3",true)){
				CARJobPndng(pCustomButtonClicked);
				RefreshScreen("",REFRESH_SELECTED);
			}
		}
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT CARUndrExctnCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	//TODO : below code will be set as a privilege
	CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),false);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-3-2021.160
	if(strEmpDep!="")
		ChangeCriteria("HSE_CRCTVEACCENT","","WHERE ((CRCTVEACCENT_RECSTS = '26') OR (CRCTVEACCENT_DPRT IN ("+strEmpDep+") AND CRCTVEACCENT_RECSTS = '25'))");	
	else
		ChangeCriteria("HSE_CRCTVEACCENT","","WHERE (CRCTVEACCENT_RECSTS = '26')");	
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	
	return CarCategory::DisplayScreenReady(pScreenReady);
}

long CARUndrExctnCategory::ShowCrctveAccUndrExctn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CrctveAccUndrExctn;	
	return ParentManage("HSE_TgcrctveAccUndrExctn",TRUE,FALSE,FALSE,"HSE_CRCTVEACCENT",strCriteria);
}

/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void CARUndrExctnCategory::CARJobCmpltd(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE CARJobCmpltd '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}

void CARUndrExctnCategory::CARJobPndng(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE CARJobPndng '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}