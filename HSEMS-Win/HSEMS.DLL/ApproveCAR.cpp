// ApproveCAR.cpp: implementation of the CApproveCAR class.
//
//////////////////////////////////////////////////////////////////////

//RQ-SF-9-1-23_4_1_5 (Approve CAR) Button

#include "stdafx.h"
#include "ApproveCAR.h"
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CApproveCAR::CApproveCAR(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
	setgbUpdateApprovedRecord(false);
}

CApproveCAR::~CApproveCAR()
{

}
/*********************************************************************************************************/
HRESULT CApproveCAR::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{

	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name== "NEW")
	{
		if(CheckForEmptyRecord(pCustomButtonClicked->pMultiSelectedRows->pRow->strFullRecord))
			return S_OK;
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*********************************************************************************************************/
//Helper function
//If user press ok and accept Accept reasons
/*********************************************************************************************************/
void CApproveCAR::ApproveCAROk()
{
		CString strAcceptFrom;
		CString keyFieldPart=getSrcKyFldNm();	
		CString strMainLink=FormGetField("HSE_RJCTRSN","MAINLINK");
		m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);

}
/*********************************************************************************************************/
HRESULT CApproveCAR::DisplayScreenReady(SCREENREADY *pSreenReady)
{
	return CHSEMSCommonCategory::DisplayScreenReady(pSreenReady);
}


HRESULT CApproveCAR::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	int nRecordStatus(pToolBarButtonClicked->nRecordStatus);
	CString strFullRecord = pToolBarButtonClicked->Full_Record;
	CString strTableName= pToolBarButtonClicked->Table_Name;
	CString strSubFormTag = pToolBarButtonClicked->SubForm_Tag;
	CString strFormTag=pToolBarButtonClicked->Form_Tag;
	if(strBtnName=="NEW")
	{
		if(pToolBarButtonClicked->iComplete == 1)
		{
			CString strAccptScr	= m_pCategory->GetHSEObj()->getstrSrcForAccptRsn();
			CString strKeyValForAccptScr = m_pCategory->GetHSEObj()->getstrKeyValForAccptScr();		
			FormSetField("HSE_CRRVWINF","CRRVWINF_LNKWTHMN",strKeyValForAccptScr);				
			FormSetField("HSE_CRRVWINF","CRRVWINF_MDLTYP",strAccptScr);		
		}
		else
		{
			if(CheckForEmptyRecord(strFullRecord))
				return S_OK;
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

void  CApproveCAR::SetScreenAttrib()
{
	bool bOpenAcceptScreenLocked=getgbOpenAcceptScreenLocked();
}
/*********************************************************************************************************/
HRESULT CApproveCAR::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//Get browsing Field name
	CString strFieldName= pBeforBrowsingButtonClick->Field_Name;
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

extern bool gEnableScreen;

long CApproveCAR::ShowApproveCAR(CString strCriteria)
{
//	m_nCurrentCard=ID_HSE_RJCT;
	return ParentManage("HSE_TGCRAPPRVLINF",gEnableScreen,gEnableScreen,gEnableScreen,"HSE_CRAPPRVLINF",strCriteria);
}

bool CApproveCAR::CheckForEmptyRecord(CString strFullRecord)
{
	if(*strFullRecord != '|' && strFullRecord != "")
	{
		AfxMessageBox("Only one record is allowed here");
		m_pHSEMSPolyDb->HandleActionEx(-1,TRUE,0);
		return true;
	}
	return false;
}

extern bool gEnableScreen;
extern CString gstrCncrnDprt="";
extern CString gstrProject="";
void CApproveCAR::OnScreenExit()
{
	CString strKeyValForAccptScr = m_pCategory->GetHSEObj()->getstrKeyValForApprvlScr();		
	CString strSQL;
	strSQL.Format("select count(0) cnt from HSE_CrApprvlInf where CrApprvlInf_LnkWthMn=%s",strKeyValForAccptScr);
	CString strRecords=GetRecordSetData(strSQL,1);		
	int nRecords=atoi(strRecords);
	if(nRecords==1 && gEnableScreen)
	{
		gstrCncrnDprt=FormGetField("HSE_TGCRAPPRVLINF","CRAPPRVLINF_CNCDPR");
		gstrProject=FormGetField("HSE_TGCRAPPRVLINF","CRAPPRVLINF_RLTPRJ");
		setgbUpdateApprovedRecord(true);
	}	
	CCommonCategory::OnScreenExit();
}