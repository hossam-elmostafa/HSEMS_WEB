// Auditors.cpp: implementation of the Auditors class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "Auditors.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Auditors::Auditors(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

Auditors::~Auditors(){}

HRESULT Auditors::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	////////////////////////////////////////////////////////////////////////////////////////
	//RQ-2-2021(Batch2).19
	/*
		Audit Plan Notification	Select the auditors, qualified for the “audit standard”	"•	In (audits) TAB > (auditors) popup screen :
		o	Filter the (employee No.) by (audits\Audit standard)
		o	Add (Leading The Audit) combo, with fixed values (Yes/No).  default = Yes.  Only (1) auditor in the same “audit” should lead the audit.
	*/
	CString strFieldName  = pFieldChange->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName == "ADTPLNENTAUDITORS_LEADING")
	{
		CString strNewValue=pFieldChange->New_Value;
		if(strNewValue=="Y")
		{
			CString strGroupLink =FormGetField(pFieldChange->Form_Tag,"LINK");
			CString strSql;
			strSql.Format("UPDATE HSE_ADTPLNENTAUDITORS SET ADTPLNENTAUDITORS_Leading='Y' WHERE ADTPLNENTAUDITORS_Leading='Y' AND LINK=%s",strGroupLink);
			long lAffected=ExecuteSQL(strSql);
			if(lAffected>0)
			{
				AfxMessageBox("Only one Auditor can Lead this Audit");
				FormSetField(pFieldChange->Form_Tag,strFieldName,"N",-1);
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

HRESULT Auditors::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	if(strBtnName=="SAVE"&&  pToolBarButtonClicked->iComplete == 1)
	{
		RefreshScreen("",REFRESH_SELECTED);
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT Auditors::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{	
	//HSE BGC-EZZ Requirements_2892020
	/*CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName == "ADTPLNENTAUDITORS_EMPNUM") {				
		CString strJobTitle = FormGetField("HSE_ADTPLNENTAUDITORS","ADTPLNENTAUDITORS_JBTLE");
		if(strJobTitle.IsEmpty())
			strJobTitle=" ";
		CString strSQL;
		strSQL.Format("WHERE (EMPLOYEE_TRADES ='%s')",strJobTitle);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}*/
	////////////////////////////////////////////////////////////////////////////////////////
	//RQ-2-2021(Batch2).19
	/*
		Audit Plan Notification	Select the auditors, qualified for the “audit standard”	"•	In (audits) TAB > (auditors) popup screen :
		o	Filter the (employee No.) by (audits\Audit standard)
		o	Add (Leading The Audit) combo, with fixed values (Yes/No).  default = Yes.  Only (1) auditor in the same “audit” should lead the audit.
	*/
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName == "ADTPLNENTAUDITORS_EMPNUM") {				
		CString strQualityStandard=m_pCategory->GetHSEObj()->getstrDep();
		CString strSQL;
		strSQL.Format("WHERE (AUDTRS_AdtStndndrd ='%s')",strQualityStandard);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}
	////////////////////////////////////////////////////////////////////////////////////////
	return S_OK;
}

long Auditors::ShowAuditors(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTORS;
	return ParentManage("HSE_TGADTPLNENTAUDITORS",TRUE,TRUE,TRUE,"HSE_ADTPLNENTAUDITORS",strCriteria);
}

long Auditors::ShowActualAuditors(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTORS;
	return ParentManage("HSE_TGACTLADTPLNENTAUDITORS",TRUE,TRUE,TRUE,"HSE_ADTPLNENTAUDITORS",strCriteria);
}