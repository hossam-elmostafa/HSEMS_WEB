// LossAccidentInq.cpp: implementation of the LossAccidentInq class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "LossAccidentInq.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LossAccidentInq::LossAccidentInq(CCategory *pCategory):LossAccidentCategory(pCategory)
{
	m_pCategory = pCategory;
}

LossAccidentInq::~LossAccidentInq()
{

}

HRESULT LossAccidentInq::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString  strFormTag(pCustomButtonClicked->Form_Tag);

	//212_320~2_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//Framework Event : Pressing Button(Close)[35]
	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		if(strButtonName=="LOSSACDNTENT_VWINCMPLT")
		{
						   
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","WHERE (LOSSACDNTENT_RECSTS=1)");
		}
		if(strButtonName=="LOSSACDNTENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","WHERE (LOSSACDNTENT_RECSTS=3)");
		}
		if(strButtonName=="LOSSACDNTENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","WHERE (LOSSACDNTENT_RECSTS=4)");
		}
		if(strButtonName=="LOSSACDNTENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","WHERE (LOSSACDNTENT_RECSTS=5)");
		}
		if(strButtonName=="LOSSACDNTENT_VWCLOSD")
		{
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","WHERE (LOSSACDNTENT_RECSTS=6)");
		}
		if(strButtonName=="LOSSACDNTENT_VWALL")
		{
			ChangeCriteria("HSE_TGLOSSACDNTINQ","","");
		}

		RefreshScreen("",0);
	}
	return LossAccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long LossAccidentInq::ShowLossAccidentInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOSSACCDNTINQ;
	return ParentManage("HSE_TGLOSSACDNTINQ",FALSE,FALSE,FALSE,"HSE_LOSSACDNTENT",strCriteria);
}

