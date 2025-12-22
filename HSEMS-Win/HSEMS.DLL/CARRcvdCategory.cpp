// CARRcvdCategory.cpp: implementation of the CARRcvdCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARRcvdCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARRcvdCategory::CARRcvdCategory(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGCRCTVEACCRCVD";
	m_strScreen	   = "Corrective Actions Received";
}

CARRcvdCategory::~CARRcvdCategory()
{

}

HRESULT CARRcvdCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	//TODO : below code will be set as a privilege
	CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),false);
	//char specialChar=2;
	//strEmpDep.Replace(specialChar,',');
	//strEmpDep.Replace(",","','");
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-3-2021.160
	if(strEmpDep!="")
		ChangeCriteria("HSE_CRCTVEACCENT","","WHERE (CRCTVEACCENT_RECSTS = '21') OR (CRCTVEACCENT_DPRT IN("+strEmpDep+") AND CRCTVEACCENT_RECSTS = '20')");	
	else
		ChangeCriteria("HSE_CRCTVEACCENT","","WHERE (CRCTVEACCENT_RECSTS = '21')");	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	return CarCategory::DisplayScreenReady(pScreenReady);
}

HRESULT CARRcvdCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="CRCTVEACCENT_ACPTDEXECTN")
	{		
		AcceptCARExe(pCustomButtonClicked);
		RefreshScreen("",REFRESH_SELECTED);
	}	
	else if(strBtnName=="CRCTVEACCENT_RJCTEXECTN")
	{
		if(OpenReasonsScr("Reject Reasons","3",true)){
			RejectCorrectiveAction(pCustomButtonClicked);
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long CARRcvdCategory::ShowCrctveAccRcvd(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CrctveAccRcvd;	
	return ParentManage("HSE_TgcrctveAccRcvd",FALSE,FALSE,FALSE,"HSE_CRCTVEACCENT",strCriteria);
}

/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void CARRcvdCategory::AcceptCARExe(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE AcceptCARExe '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}

void CARRcvdCategory::RejectCARExe(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE RejectCARExe '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}