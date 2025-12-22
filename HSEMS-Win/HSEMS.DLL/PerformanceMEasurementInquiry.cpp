// PerformanceMEasurementInquiry.cpp: implementation of the PerformanceMEasurementInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PerformanceMEasurementInquiry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PerformanceMEasurementInquiry::PerformanceMEasurementInquiry(CCategory *pCategory):PerformanceMeasurementCategory(pCategory)
{
	m_pCategory = pCategory;
}

PerformanceMEasurementInquiry::~PerformanceMEasurementInquiry()
{

}

HRESULT PerformanceMEasurementInquiry::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);

	
	if(strButtonName=="SHW_DTL")
	{

		ShowDetails(pCustomButtonClicked);
	}

	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		 if(strButtonName=="PERFMSRMNTENT_VWINCMPLT")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","WHERE (PERFMSRMNTENT_RECSTS=1)");
		}
		else if(strButtonName=="PERFMSRMNTENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","WHERE (PERFMSRMNTENT_RECSTS=3)");
		}
		else if(strButtonName=="PERFMSRMNTENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","WHERE (PERFMSRMNTENT_RECSTS=4)");
		}
		else if(strButtonName=="PERFMSRMNTENT_VWAPPRV")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","WHERE (PERFMSRMNTENT_RECSTS=5)");
		}
		else if(strButtonName=="PERFMSRMNTENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","WHERE (PERFMSRMNTENT_RECSTS=6)");
		}
		else if(strButtonName=="PERFMSRMNTENT_VWALL")
		{
			ChangeCriteria("HSE_TGPERFMSRMNTINQ","","");
		}
		RefreshScreen("",0);
	}
	return PerformanceMeasurementCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long PerformanceMEasurementInquiry::ShowPerformanceInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTINQ;
	//return ParentManage("HSE_TGPERFMSRMNTINQ",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTENT",strCriteria);
	return ParentManage("HSE_TGPERFMSRMNTINQ",FALSE,FALSE,FALSE,"HSE_PERFMSRMNTENT",strCriteria);
}