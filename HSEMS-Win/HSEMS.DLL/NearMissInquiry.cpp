// NearMissInquiry.cpp: implementation of the NearMissInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissInquiry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NearMissInquiry::NearMissInquiry(CCategory *pCategory):NearMissCategory(pCategory)
{
	m_pCategory = pCategory;
}


NearMissInquiry::~NearMissInquiry()
{

}
HRESULT NearMissInquiry::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{

	CString strButtonName(pCustomButtonClicked->Button_Name);

	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
	
		if(strButtonName=="NRSTMISCENT_VWINCMP")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","WHERE (NRSTMISCENT_RECSTS=1)");
		}
		else if(strButtonName=="NRSTMISCENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","WHERE (NRSTMISCENT_RECSTS=3)");
		}
		else if(strButtonName=="NRSTMISCENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","WHERE (NRSTMISCENT_RECSTS=4)");
		}
		else if(strButtonName=="NRSTMISCENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","WHERE (NRSTMISCENT_RECSTS=5)");
		}
		else if(strButtonName=="NRSTMISCENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","WHERE (NRSTMISCENT_RECSTS=6)");
		}
		else if(strButtonName=="NRSTMISCENT_VWALL")
		{
			ChangeCriteria("HSE_TGNRSTMISCINQ","","");
		}

		RefreshScreen("",0);
	}
	return NearMissCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long NearMissInquiry::ShowNearMissInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NRSTMISCINQ;
	return ParentManage("HSE_TGNRSTMISCINQ",FALSE,FALSE,FALSE,"HSE_vwNRSTMISCENT",strCriteria);
}