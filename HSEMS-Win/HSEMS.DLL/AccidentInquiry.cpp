// AccidentInquiry.cpp: implementation of the AccidentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccidentInquiry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//polydb_task
/*
AccidentInquiry::AccidentInquiry()
{

}

AccidentInquiry::~AccidentInquiry()
{

}
*/

HRESULT AccidentInquiry::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{

	CString strButtonName(pCustomButtonClicked->Button_Name);
	if(strButtonName=="ACDNTENTINJRDPRSN_INJRDBODYPART")
	{	
		OpenBodyParts();		
	}
	else
	{
		CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);

		if(strButtonName != "SELECT" && strSubScrnTag == "")
		{
			if(strButtonName=="ACDNTENT_VWINCMPLT")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","WHERE (ACDNTENT_RECSTS=1)");
			}
			else if(strButtonName=="ACDNTENT_VWRJCT")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","WHERE (ACDNTENT_RECSTS=3)");
			}
			else if(strButtonName=="ACDNTENT_VWCMPLT")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","WHERE (ACDNTENT_RECSTS=4)");
			}
			else if(strButtonName=="ACDNTENT_VWAPPRV")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","WHERE (ACDNTENT_RECSTS=5)");
			}
			else if(strButtonName=="ACDNTENT_VWCLSD")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","WHERE (ACDNTENT_RECSTS=6)");
			}
			else if(strButtonName=="ACDNTENT_VWALL")
			{
				ChangeCriteria("HSE_TGACDNTINQ","","");
			}

			RefreshScreen("",0);
		}
	}
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long AccidentInquiry::ShowAccident(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTINQ;
	//return ParentManage("HSE_TGACDNTINQ",TRUE,TRUE,TRUE,"HSE_ACDNTENT",strCriteria);
	return ParentManage("HSE_TGACDNTINQ",FALSE,FALSE,FALSE,"HSE_ACDNTENT",strCriteria);
}

