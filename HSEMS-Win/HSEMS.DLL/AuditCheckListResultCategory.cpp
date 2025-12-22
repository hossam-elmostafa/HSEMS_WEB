// AuditCheckListResultCategory.cpp: implementation of the AuditCheckListResultCategory class.
// Important Note : This class handle Audit checklist opened from Audit Plan resource.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditCheckListResultCategory.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditCheckListResultCategory::AuditCheckListResultCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditCheckListResultCategory::~AuditCheckListResultCategory()
{

}

HRESULT AuditCheckListResultCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strSQL="";
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	if(strBtnName == "ADTCHCKLSTPLN_GETCHCKLST") {		
		int nAuditesKeyFld = m_pCategory->GetHSEObj()->getnAuditsKeyFld();
		CString strSQL;
		strSQL.Format("EXECUTE getAuditCheckList %d",nAuditesKeyFld);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SCREEN);		
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AuditCheckListResultCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(strBtnName=="NEW")
	{									  
		FormSetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_PLN","0");
		SetFieldVisibleAttrib("HSE_TGADTCHCKLSTRSLT","ADTCHCKLSTPLN_ADTPNT","NULL");
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


long AuditCheckListResultCategory::ShowAuditCheckListResult(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AUDITCHECKLIST;
	return ParentManage("HSE_TGADTCHCKLSTPLN",TRUE,TRUE,TRUE,"HSE_ADTCHCKLSTPLN",strCriteria);
}


