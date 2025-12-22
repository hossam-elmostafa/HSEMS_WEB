// CARInquiry.cpp: implementation of the CCARInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARInquiry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARInquiry::CARInquiry(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
}

CARInquiry::~CARInquiry()
{

}

HRESULT CARInquiry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString  strFormTag(pCustomButtonClicked->Form_Tag);
	//212_320~2_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//Framework Event : Pressing Button(Close)[35]
	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);
	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		if(strButtonName=="CRCTVEACCENT_VWINCMP")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_RECSTS=1)");
		}
		if(strButtonName=="CRCTVEACCENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_RECSTS=3)");
		}
		if(strButtonName=="CRCTVEACCENT_VWCMP")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_RECSTS=4)");
		}
		if(strButtonName=="CRCTVEACCENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_RECSTS=5)");
		}
		if(strButtonName=="CRCTVEACCENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","SELECT * FROM HSE_CRCTVEACCENT WHERE (CRCTVEACCENT_RECSTS=6)");
		}
		if(strButtonName=="CRCTVEACCENT_VWALL")
		{
			ChangeCriteria("HSE_TGCRCTVEACCINQ","","");
		}

		RefreshScreen("",0);
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long CARInquiry::ShowCAR(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CRCTVEACCINQ;	
	return ParentManage("HSE_TGCRCTVEACCINQ",FALSE,FALSE,FALSE,"HSE_CRCTVEACCENT",strCriteria);
}