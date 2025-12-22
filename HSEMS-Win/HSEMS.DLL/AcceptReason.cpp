// AcceptReason.cpp: implementation of the CAcceptReason class.
//
//////////////////////////////////////////////////////////////////////

//RQ-SF-9-1-23_3_1_4 (Accept CAR) Button

#include "stdafx.h"
#include "AcceptReason.h"
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptReason::CAcceptReason(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
	setgbUpdateAcceptedRecord(false);
}

CAcceptReason::~CAcceptReason()
{

}
/*********************************************************************************************************/
HRESULT CAcceptReason::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
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
void CAcceptReason::AcceptReasonOk()
{
		CString strAcceptFrom;
		CString keyFieldPart=getSrcKyFldNm();	
		CString strMainLink=FormGetField("HSE_RJCTRSN","MAINLINK");
		m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);

}
/*********************************************************************************************************/
HRESULT CAcceptReason::DisplayScreenReady(SCREENREADY *pSreenReady)
{
	return CHSEMSCommonCategory::DisplayScreenReady(pSreenReady);
}


HRESULT CAcceptReason::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
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

void  CAcceptReason::SetScreenAttrib()
{
	bool bOpenAcceptScreenLocked=getgbOpenAcceptScreenLocked();
}
/*********************************************************************************************************/
HRESULT CAcceptReason::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//Get browsing Field name
	CString strFieldName= pBeforBrowsingButtonClick->Field_Name;
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

extern bool gEnableScreen;

long CAcceptReason::ShowAcceptreason(CString strCriteria)
{
//	m_nCurrentCard=ID_HSE_RJCT;
	return ParentManage("HSE_TgCrRvwInf",gEnableScreen,gEnableScreen,gEnableScreen,"HSE_CrRvwInf",strCriteria);
}

bool CAcceptReason::CheckForEmptyRecord(CString strFullRecord)
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
void CAcceptReason::OnScreenExit()
{
	CString strKeyValForAccptScr = m_pCategory->GetHSEObj()->getstrKeyValForAccptScr();		
	CString strSQL;
	strSQL.Format("select count(0) cnt from HSE_CrRvwInf where CrRvwInf_LnkWthMn=%s",strKeyValForAccptScr);
	CString strRecords=GetRecordSetData(strSQL,1);		
	int nRecords=atoi(strRecords);
	if(nRecords==1 && gEnableScreen)
	{
		setgbUpdateAcceptedRecord(true);
	}	
	CCommonCategory::OnScreenExit();
}