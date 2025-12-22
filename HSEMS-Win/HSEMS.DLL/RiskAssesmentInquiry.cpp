// RiskAssesmentInquiry.cpp: implementation of the RiskAssesmentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RiskAssesmentInquiry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RiskAssesmentInquiry::RiskAssesmentInquiry(CCategory *pCategory):RiskAssessmentCategory(pCategory)
{
	m_pCategory = pCategory;
}

RiskAssesmentInquiry::~RiskAssesmentInquiry()
{

}

HRESULT RiskAssesmentInquiry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
	strButtonName.MakeUpper();
	if(strButtonName != "SELECT" && strButtonName != "CREATELIKE" && strSubScrnTag == "")
	{	
		if(strButtonName=="RSKASSMNTENT_VWINCMPLT")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","WHERE (RSKASSMNTENT_RECSTS=1)");
			RefreshScreen("",0);
		}
		else if(strButtonName=="RSKASSMNTENT_VWAPPRV")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","WHERE (RSKASSMNTENT_RECSTS=5)");
			RefreshScreen("",0);
		}
		else if(strButtonName=="RSKASSMNTENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","WHERE (RSKASSMNTENT_RECSTS=6)");
			RefreshScreen("",0);
		}
		else if(strButtonName=="RSKASSMNTENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","WHERE (RSKASSMNTENT_RECSTS=4)");
			RefreshScreen("",0);
		}
		else if(strButtonName=="RSKASSMNTENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","WHERE (RSKASSMNTENT_RECSTS=3)");
			RefreshScreen("",0);
		}
		else if(strButtonName=="RSKASSMNTENT_VWALL")
		{
			ChangeCriteria("HSE_TGRSKASSMNTINQ","","");
			RefreshScreen("",0);
		}				
	}	
	return RiskAssessmentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long RiskAssesmentInquiry::ShowRiskInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKASSMNTINQ;
	return ParentManage("HSE_TGRSKASSMNTINQ",FALSE,FALSE,FALSE,"HSE_RSKASSMNTENT",strCriteria);
}
