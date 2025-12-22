// Auditess.cpp: implementation of the Auditess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Auditess.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Auditess::Auditess(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

Auditess::~Auditess()
{

}

HRESULT Auditess::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	

	/*CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	

 	CString strCompEmp=FormGetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_COMPEMP");
	CString strEmp=FormGetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_EMPNUM");
	if(strCompEmp=="0")
	{		
		FormSetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_EMPNUM"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","{5}"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","{6}"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","{12}"," ");

	}

	if(strCompEmp=="1")
	{		
		FormSetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_NAM"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_COMP"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_DPRT"," ");
		FormSetField("HSE_ADTPLNENTAUDITEES","ADTPLNENTAUDITEES_JOBTTL"," ");
	}

	selectEmployeeType(pFieldChange,"HSE_TGADTPLNENTAUDITEES"); */


	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

HRESULT Auditess::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);

	if(strBtnName=="SAVE"&&  pToolBarButtonClicked->iComplete == 1)
	{
		RefreshScreen("",REFRESH_SELECTED);
	}

	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT Auditess::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();

	/*if(strFieldName=="ADTPLNENTAUDITEES_EMPNUM")
	{
		CString ActivityKeyField = FormGetField("HSE_ADTPLNENTAUDITEES","LINK");
		CString strSQL=("Select ADTPLNENTACTVT_ADITEDPRT From HSE_ADTPLNENTACTVT WHERE MAINLINK = '"+ActivityKeyField+"'");
		CString AuditeeDept=GetRecordSetData(strSQL,1);
		strSQL.Format("WHERE (EMP_DPRT ='%s')",AuditeeDept);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}*/
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}


long Auditess::ShowAudites(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTEES;
	return ParentManage("HSE_TGADTPLNENTAUDITEES",TRUE,TRUE,TRUE,"HSE_ADTPLNENTAUDITEES",strCriteria);
}

