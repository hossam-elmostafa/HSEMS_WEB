// AuditPlanFollowUpCategory.cpp: implementation of the AuditPlanFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditPlanFollowUpCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanFollowUpCategory::AuditPlanFollowUpCategory(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanFollowUpCategory::~AuditPlanFollowUpCategory()
{

}
/*************************************************************************************************************/
HRESULT AuditPlanFollowUpCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strBtnName=="ADTPLNENT_CLS")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
		{
			CloseAudit(pCustomButtonClicked);
			RefreshScreen(strFormTag , 0);
		}
	}
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT AuditPlanFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	#define	CHECK_FOR_AUDIT_PLAN_FOLLOWUP_REQUIRED	"HSEPLC_ADTRSLTCNFRMTNRQRD"
	bool bIsADTFollowRqrd=CheckPolicy(CHECK_FOR_AUDIT_PLAN_FOLLOWUP_REQUIRED);
	if(bIsADTFollowRqrd==true)
	{
		return AuditCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	
	else
	{
		ShowMessageEx(IDS_ADTFOLLOW,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/*************************************************************************************************************/
HRESULT AuditPlanFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}
/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void AuditPlanFollowUpCategory::CloseAudit(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,6,"ADTPLNENT","MAINLINK");
	InsertActionRecordIntoTracingTab("ADTPLNENT","MAINLINK",CLOSED);
}

long AuditPlanFollowUpCategory::ShowAuditPlanFlwUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNFLWUP;
	return ParentManage("HSE_TGADTPLNFOLLWUP",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}