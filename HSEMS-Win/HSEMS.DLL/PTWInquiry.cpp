// PTWInquiry.cpp: implementation of the PTWInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PTWInquiry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PTWInquiry::PTWInquiry(CCategory *pCategory):PtwCategory(pCategory)
{
	m_pCategory = pCategory;
}

PTWInquiry::~PTWInquiry()
{

}


HRESULT PTWInquiry::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);

	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		if(strButtonName=="PTWSRGSTRENT_VWINCMPLT")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","WHERE (PTWSRGSTRENT_RECSTS=1)");
		}
		else if(strButtonName=="PTWSRGSTRENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","WHERE (PTWSRGSTRENT_RECSTS=3)");
		}
		else if(strButtonName=="PTWSRGSTRENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","WHERE (PTWSRGSTRENT_RECSTS=4)");
		}
		else if(strButtonName=="PTWSRGSTRENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","WHERE (PTWSRGSTRENT_RECSTS=5)");
		}
		else if(strButtonName=="PTWSRGSTRENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","WHERE (PTWSRGSTRENT_RECSTS=6)");
		}
		else if(strButtonName=="PTWSRGSTRENT_VWALL")
		{
			ChangeCriteria("HSE_TGPTWSRGSTRINQ","","");
		}
		RefreshScreen("",0);
	}
	return PtwCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long PTWInquiry::ShowPTW(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PPTWINQ;
	//return ParentManage("HSE_TGPTWSRGSTRINQ",TRUE,TRUE,TRUE,"HSE_PTWSRGSTRINQ",strCriteria);
	return ParentManage("HSE_TGPTWSRGSTRINQ",FALSE,FALSE,FALSE,"HSE_PTWSRGSTRENT",strCriteria);
}