// RiskAssesmentInquiry.cpp: implementation of the RiskAssesmentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AreaHzrdsCategory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AreaHzrdsCategory::AreaHzrdsCategory(CCategory *pCategory):RiskAssessmentCategory(pCategory)
{
	m_pCategory = pCategory;
}

AreaHzrdsCategory::~AreaHzrdsCategory()
{

}

long AreaHzrdsCategory::ShowAreaHzrds(CString strCriteria)
{
	m_nCurrentCard=ID_Area_Hzrds;
	return ParentManage("HSE_TGAREAHZRDS",TRUE,TRUE,TRUE,"HSE_AREAHZRDS",strCriteria);
}

HRESULT AreaHzrdsCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strSQL = "";
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="AREAHZRDSPTNTLHZRD_CNTRLMSR") {		
		OpenAreaHzrdsCntrlMsure(strFormTag);
	}
	else if (strButtonName=="AREAHZRDSPTNTLHZRD_STPEFFCT") {		
		OpnAreaHzrdsEffectsScr(strFormTag);
	}

	return RiskAssessmentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long AreaHzrdsCategory::ShowAHzrdsPtntlHzrdCtrlMsr(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AHzrdsPtntlHzrdCtrlMsr;
	return ParentManage("HSE_TGAHZRDSPTNTLHZRDCTRLMSR",TRUE,TRUE,TRUE,"HSE_AHZRDSPTNTLHZRDCTRLMSR",strCriteria);
}

long AreaHzrdsCategory::ShowAreaHzrdsPtntlHzrdEffct(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AreaHzrdsPtntlHzrdEffct;
	return ParentManage("HSE_TGAREAHZRDSPTNTLHZRDEFFCT",TRUE,TRUE,TRUE,"HSE_AREAHZRDSPTNTLHZRDEFFCT",strCriteria);
}

void AreaHzrdsCategory::OpenAreaHzrdsCntrlMsure(CString strFormTag) {		
	CString strSQL,
			strDefValues,
			strKeyField;			;		 
	strKeyField	= FormGetField("HSE_AreaHzrdsPtntlHzrd_VIEW","AreaHzrdsPtntlHzrd_MAINLINK");	
	strDefValues.Format("AHzrdsPtntlHzrdCtrlMsr_Lnk|%s",strKeyField);	
	strSQL.Format("SELECT * FROM HSE_AHzrdsPtntlHzrdCtrlMsr WHERE (AHzrdsPtntlHzrdCtrlMsr_Lnk = %s)",strKeyField);
	ShowScreen(0,"HSE_TgAHzrdsPtntlHzrdCtrlMsr","Control Measures",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);		
}

void AreaHzrdsCategory::OpnAreaHzrdsEffectsScr(CString strFormTag) {
	CString strSQL,
		    strDefValues,
			strKeyField;
	strKeyField = FormGetField("HSE_AreaHzrdsPtntlHzrd_VIEW","AreaHzrdsPtntlHzrd_MAINLINK");
	strDefValues.Format("AreaHzrdsPtntlHzrdEffct_lnk|%s",strKeyField);	
	strSQL.Format("SELECT * FROM HSE_AreaHzrdsPtntlHzrdEffct WHERE (AreaHzrdsPtntlHzrdEffct_lnk = '%s')",strKeyField);						
	ShowScreen(0,"HSE_TgAreaHzrdsPtntlHzrdEffct","Effects",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
}