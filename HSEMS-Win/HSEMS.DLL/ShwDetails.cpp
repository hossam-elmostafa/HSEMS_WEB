// ShwDetails.cpp: implementation of the ShwDetails class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShwDetails.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ShwDetails::ShwDetails(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

ShwDetails::~ShwDetails()
{

}

HRESULT ShwDetails::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	
	ChkBoxYesNoValidation();
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT ShwDetails::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	ChkBoxYesNoValidation();
	
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

void ShwDetails::ChkBoxYesNoValidation()
{
	CString strYESChecked=FormGetField("HSE_DTL","CHK_YES");
	CString strNOChecked=FormGetField("HSE_DTL","CHK_NO");
	if(strYESChecked=="1"&&strNOChecked=="0")
	{
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_NO","ALWAYSLOCKED");
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_YES","NULL");
	}
	else if(strNOChecked=="1"&&strYESChecked=="0")
	{
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_YES","ALWAYSLOCKED");
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_NO","NULL");
	}	
	else if(strNOChecked=="0"&&strYESChecked=="0")
	{
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_NO","NULL");
		SetFieldVisibleAttrib("HSE_TGCPYDTL","CHK_YES","NULL");
	}
}

long ShwDetails::ShowCopyofdetail(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_DTLCPY;
	return ParentManage("HSE_TGCPYDTL",TRUE,TRUE,TRUE,"HSE_CPYDTL",strCriteria);
}