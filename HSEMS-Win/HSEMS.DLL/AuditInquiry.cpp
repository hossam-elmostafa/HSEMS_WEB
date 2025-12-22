// AuditInquiry.cpp: implementation of the AuditInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuditInquiry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditInquiry::AuditInquiry(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditInquiry::~AuditInquiry()
{

}


HRESULT AuditInquiry::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);

	if(strButtonName=="AUDITORS")
	{
		OpenAuditorsScreen(strFormTag,2);			//here 2 is not needed as function check if open screen is Inquiry screen					
	}
	else if(strButtonName=="AUDITIES")
	{
		OpenAuditeesScreen(strFormTag,2);
	}	
	else if(strButtonName=="ADTPLNENTACTVT_ADTCHKLSTFDBCK")
	{
		openAuditChecklistResult(strFormTag);
	}	
	/**
	 *	Update 13-8-2020
	 *	Below buttons are no longer used
	 *
		else if(strButtonName=="ADTPLNENTACTVT_ADTCHCKLST")
		{
			OpenAuditCheckListScreen(strFormTag);	
		}
		else if(strButtonName=="ADTPLNENTACTVT_CONCRNDPT")
		{
			OpenCncrndDept(strFormTag);
		}
	*/
	

	/***********************************************************
	 *	
	 *	Ref : HSE_CR11_17
	 *  Commented by a.ali
	 *	Date: 29-05-2013
	 *	Ref : HSE_CR12v2_99
	 *	else if(strButtonName == "ADTPLNENTACTVT_RQRDACTNBTN"){
	 *		OpenRequiredActionScreen(strFormTag,1);
	 *	}
	 ***********************************************************/

	else if (strButtonName != "SELECT" )
	{
		if(strButtonName=="ADTPLNENT_VWINCMPLT")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=1)");
		}
		else if(strButtonName=="ADTPLNENT_VWRJCT")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=3)");
		}
		else if(strButtonName=="ADTPLNENT_VWCMPLT")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=4)");
		}
		else if(strButtonName=="ADTPLNENT_VWAPPRV")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=5)");
		}
		else if(strButtonName=="ADTPLNENT_VWCLSD")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","WHERE (ADTPLNENT_ADTPLNSTS=6)");
		}
		else if(strButtonName=="ADTPLNENT_VWALL")
		{
			ChangeCriteria("HSE_TGADTPLNINQ","","");
		}	
		RefreshScreen("",0);
	}
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);


}

long AuditInquiry::ShowAudit(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNINQ;
	//return ParentManage("HSE_TGADTPLNINQ",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
	return ParentManage("HSE_TGADTPLNINQ",FALSE,FALSE,FALSE,"HSE_ADTPLNENT",strCriteria);
}