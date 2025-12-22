// AccidentReasonCategory.cpp: implementation of the AccidentReasonCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccidentReasonCategory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AccidentReasonCategory::AccidentReasonCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

AccidentReasonCategory::~AccidentReasonCategory()
{

}

HRESULT AccidentReasonCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	// Rule id : CR03-09
	// Done by : Doaa Desouky
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//polydb_task , nRecordStatus always sent 0
	//if(pCustomButtonClicked-> nRecordStatus != 0 && strButtonName=="SUBDETAIL")
	if(strButtonName=="SUBDETAIL")
	{
		OpenSubDetailScreen();
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void AccidentReasonCategory::OpenSubDetailScreen()
{
	CString strScreenTag;
	CString strMainKeyFeild = FormGetField("HSE_ACDNTRSN" ,"ACDNTRSN_GRP");
	int nNumberOfRecords    = getNumberOfRecord("HSE_ACDNTRSNDTL" ,strMainKeyFeild,"ACDNTRSNDTL_LNK");
	if(nNumberOfRecords != 0)
	{		
		CString strSQL ;		
		CString strKeyField = FormGetField("HSE_ACDNTRSNDTL","MAINLINK");				
		CString strDefValues;		
		strSQL.Format("SELECT * FROM HSE_SUBDTL WHERE (LINK = %s)","'"+strKeyField+"'");
		strDefValues.Format("LINK|%s",strKeyField);
		ShowScreen(0,"HSE_TGSUBDTL","Sub Details",NORMAL_MODE,false,strSQL,"",strDefValues,"",false);		
	}	
}

long AccidentReasonCategory::ShowAccidentReason(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTRSN;
	return ParentManage("HSE_TGACDNTRSN",TRUE,TRUE,TRUE,"HSE_ACDNTRSN",strCriteria);
}
