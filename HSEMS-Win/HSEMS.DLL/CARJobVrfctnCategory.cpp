// CARJobVrfctnCategory.cpp: implementation of the CARJobVrfctnCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARJobVrfctnCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARJobVrfctnCategory::CARJobVrfctnCategory(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGCRCTVEACCJOBVRFCTN";
	m_strScreen	   = "Corrective Action Job Verification";
}

CARJobVrfctnCategory::~CARJobVrfctnCategory()
{

}

long CARJobVrfctnCategory::ShowCrctveAccJobVrfctn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CrctveAccJobVrfctn;	
	return ParentManage("HSE_TgcrctveAccJobVrfctn",TRUE,FALSE,FALSE,"HSE_CRCTVEACCENT",strCriteria);
}

HRESULT CARJobVrfctnCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	
	if(strBtnName!="SELECT") //Select is not a custom Button
	{
		CString strCarmodelNum=FormGetField(strFormTag,"crctveAccEnt_CarmodelNum");								
		BOOL bIsValidRecord=m_pHSEMSPolyDb->ValidateEmptyFields(strFormTag, "crctveAccEnt_CarmodelNum", strCarmodelNum, "", false);
		if(bIsValidRecord!=0)
		{

			if (strBtnName=="CRCTVEACCENT_JOBACPTD_JOBVRFCTN"){
				CARJobAcptd(pCustomButtonClicked);
				RefreshScreen("",REFRESH_SELECTED);
			} 
			else if (strBtnName=="CRCTVEACCENT_JOBRJCTD_JOBVRFCTN"){
				if(OpenReasonsScr("Reject Reasons","3",true)){
					RejectCorrectiveAction(pCustomButtonClicked);
					RefreshScreen("",REFRESH_SELECTED);
				}
			}   
			else if (strBtnName=="CRCTVEACCENT_CNCL_JOBVRFCTN"){
				if(OpenReasonsScr("Cancel Reasons","3",true)){
					CARJobCncld(pCustomButtonClicked);
					RefreshScreen("",REFRESH_SELECTED);
				}
			}
		}
		else
		{
			AfxMessageBox("Please complete the record before applying function");
		}
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void CARJobVrfctnCategory::CARJobAcptd(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE CARJobAcptd '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}

void CARJobVrfctnCategory::CARJobCncld(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE CARJobCncld '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}