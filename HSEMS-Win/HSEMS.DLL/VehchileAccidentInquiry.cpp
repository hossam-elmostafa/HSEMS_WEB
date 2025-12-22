// VehchileAccidentInquiry.cpp: implementation of the VehchileAccidentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "VehchileAccidentInquiry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VehchileAccidentInquiry::VehchileAccidentInquiry(CCategory *pCategory):VehicleAccident(pCategory)
{
	m_pCategory = pCategory;
}

VehchileAccidentInquiry::~VehchileAccidentInquiry()
{

}
HRESULT VehchileAccidentInquiry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString  strFormTag(pCustomButtonClicked->Form_Tag);

	//212_320~2_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//Framework Event : Pressing Button(Close)[35]

	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

	if(strButtonName != "SELECT" && strSubScrnTag == "")
	{
		if(strButtonName=="VCLACDNTENT_VWINCMP")
		{
						   
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","WHERE (VCLACDNTENT_RECSTS=1)");
		}
		if(strButtonName=="VCLACDNTENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","WHERE (VCLACDNTENT_RECSTS=3)");
		}
		if(strButtonName=="VCLACDNTENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","WHERE (VCLACDNTENT_RECSTS=4)");
		}
		if(strButtonName=="VCLACDNTENT_VWCNFRM")
		{
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","WHERE (VCLACDNTENT_RECSTS=5)");
		}
		if(strButtonName=="VCLACDNTENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","WHERE (VCLACDNTENT_RECSTS=6)");
		}
		if(strButtonName=="VCLACDNTENT_VWALL")
		{
			ChangeCriteria("HSE_TGVCLACDNTINQTN","","");
		}

		RefreshScreen("",0);
	}
	return VehicleAccident::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long VehchileAccidentInquiry::ShowVehchileAccidentInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCLACDNTINQ;
	//return ParentManage("HSE_TGVCLACDNTINQTN",TRUE,TRUE,TRUE,"HSE_VCLACDNTENT",strCriteria);
	return ParentManage("HSE_TGVCLACDNTINQTN",FALSE,FALSE,FALSE,"HSE_VCLACDNTENT",strCriteria);
}