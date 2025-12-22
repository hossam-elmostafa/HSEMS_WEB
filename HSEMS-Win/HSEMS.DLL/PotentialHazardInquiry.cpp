// PotentialHazardInquiry.cpp: implementation of the PotentialHazardInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PotentialHazardInquiry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PotentialHazardInquiry::PotentialHazardInquiry(CCategory *pCategory):PotentialHazardCategory(pCategory)
{
	m_pCategory = pCategory;
}

PotentialHazardInquiry::~PotentialHazardInquiry() 
{

}

HRESULT PotentialHazardInquiry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString  strFormTag(pCustomButtonClicked->Form_Tag);

	//212_320~2_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//Framework Event : Pressing Button(Close)[35]
	
	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		if(strButtonName=="PTNLHZRDENT_VWINCMPLT")
		{
						   
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","WHERE (PTNLHZRDENT_RECSTS=1)");
		}
		if(strButtonName=="PTNLHZRDENT_VWREJC")
		{
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","WHERE (PTNLHZRDENT_RECSTS=3)");
		}
		if(strButtonName=="PTNLHZRDENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","WHERE (PTNLHZRDENT_RECSTS=4)");
		}
		if(strButtonName=="PTNLHZRDENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","WHERE (PTNLHZRDENT_RECSTS=5)");
		}
		if(strButtonName=="PTNLHZRDENT_VWCLS")
		{
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","WHERE (PTNLHZRDENT_RECSTS=6)");
		}
		if(strButtonName=="PTNLHZRDENT_VWALL")
		{
			ChangeCriteria("HSE_TGPTNLHZRDINQ","","");
		}
		RefreshScreen("",0);
	}
	return PotentialHazardCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long PotentialHazardInquiry::ShowPotentialhazardinquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTNLHZRDINQ;
	return ParentManage("HSE_TGPTNLHZRDINQ",FALSE,FALSE,FALSE,"HSE_PTNLHZRDENT",strCriteria);
}
