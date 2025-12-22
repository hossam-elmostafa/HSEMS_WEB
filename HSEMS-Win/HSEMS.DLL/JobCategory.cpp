#include "stdafx.h"
#include "HSEMS.h"
#include "JobCategory.h"


JobCategory::JobCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

JobCategory::~JobCategory()
{

}

HRESULT JobCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{	
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	m_strScreenTag = pCustomButtonClicked->Form_Tag;	
	/* commented by a.ali
	   Ref : TG-PD-WB_15,16
	if(strBtnName=="TSKDTLS_PTNTLHZRDS")
	{		
		CString strDefValues;	
		CString strSQL;				
		strDefValues.Format("PTNTLHZRDSLIB_LNK|%s",strKeyField);					
		strSQL.Format("SELECT * FROM HSE_PTNTLHZRDSLIB WHERE ( PTNTLHZRDSLIB_LNK = %s) ",strKeyField);		
		ShowScreen(0,"HSE_TgPtntlHzrdsLib","Potential Hazards Library",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);	
	}*/	
	if(strBtnName=="TSKDTLS_STEPS")
	{
		CString strKeyField = FormGetField("HSE_TSKDTLS","MAINLINK");	
		CString strDefValues;	
		CString strSQL;				
		strDefValues.Format("JOBSTP_LNKWTHDTL|%s",strKeyField);					
		strSQL.Format("SELECT * FROM HSE_JOBSTP WHERE ( JOBSTP_LNKWTHDTL = %s) ",strKeyField);		
		ShowScreen(0,"HSE_TGJOBSTP","Job Steps",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long JobCategory::ShowJob(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TSK;
	return ParentManage("HSE_TGTSK",TRUE,TRUE,TRUE,"HSE_TSK",strCriteria);
}
