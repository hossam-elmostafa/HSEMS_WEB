#include "stdafx.h"
#include "HSEMS.h"
#include "RiskAssessmentHzrdPopCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString gstrKeyField;
//////////////////////////////////////////////////////////////////////////
//RQ-9-2021.1

CRiskAssessmentHzrdPopCategory::CRiskAssessmentHzrdPopCategory(CCategory *pCategory,CString strCriteria):RiskAssessmentCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGPTNLHZRD";
	m_strScreen			= "Potential Hazards";

}

CRiskAssessmentHzrdPopCategory::~CRiskAssessmentHzrdPopCategory() 
{
}


HRESULT CRiskAssessmentHzrdPopCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);			
	if(strBtnName=="NEW") {			
		FormSetField("HSE_TGPTNLHZRD","PtnlHzrd_PrntLnk",gstrKeyField);	
	}	
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


////////////////////////////////////////////////////////////////////////////