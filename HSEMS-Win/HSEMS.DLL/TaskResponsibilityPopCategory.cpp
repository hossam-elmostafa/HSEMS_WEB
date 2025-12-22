#include "stdafx.h"
#include "HSEMS.h"
#include "TaskResponsibilityPopCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString gstrKeyField;
//////////////////////////////////////////////////////////////////////////
//RQ-9-2021.1

CTaskResponsibilityPopCategory::CTaskResponsibilityPopCategory(CCategory *pCategory,CString strCriteria):RiskAssessmentCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGTSKRSP";
	m_strScreen			= "Task Responsibility";

}

CTaskResponsibilityPopCategory::~CTaskResponsibilityPopCategory() 
{
}


HRESULT CTaskResponsibilityPopCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);			
	if(strBtnName=="NEW") {			
		FormSetField("HSE_TGTSKRSP","TskRsp_PrntLnk",gstrKeyField);	
	}	
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


////////////////////////////////////////////////////////////////////////////