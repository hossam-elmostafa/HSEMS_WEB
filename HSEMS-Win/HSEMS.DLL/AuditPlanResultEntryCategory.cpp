// AuditPlanResultEntryCategory.cpp: implementation of the AuditPlanResultEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditPlanResultEntryCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanResultEntryCategory::AuditPlanResultEntryCategory(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanResultEntryCategory::~AuditPlanResultEntryCategory()
{

}


HRESULT AuditPlanResultEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
//603_460~2_1

{

	CString strSubFormTag=(pCustomButtonClicked->Button_Name);
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strKeyField=FormGetField("HSE_ADTRSLTENTADTCHCKLST","ADTRSLTENTADTCHCKLST_TSKNUM");
	if(strBtnName=="ADTPLNENT_ENTCMPLT")
	{
		//Set (Audit Result Status = 4)
		//UpdateStatusForAudit(pCustomButtonClicked->pMultiSelectedRows,4,"ADTPLNENT_ADTYR",strFormTag);
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"ADTPLNENT","MAINLINK");
		//Insert record into (tracing [@46020~2]) tab
		InsertActionRecordIntoTracingTab("ADTPLNENT","ADTYR",COMPLETE);
	}
	//603_481_2
	//Action : Press (Auditors) Button
	else if(strBtnName=="ADTRSLTENTADTCHCKLST_AUDTOR")
	{		
		//Display Auditors [@302~3] popup screen
		//Allow user to feed/edit records as normal		
		m_strScreenTag = "HSE_TGAUDITORS";
		InitScreenDefaultValue(0,"HSE_TGAUDITORS",1);
		AddFieldDefaultValue(0,0,"AUDITORS_LNK",strKeyField);
		return OpenScreen("Auditors","",ID_HSE_AUDITORS,0);
	}
	//603_481_3
	//Action : Press (Auditees) Button
	else if("ADTRSLTENTADTCHCKLST_ADITE")
	{
		
		//Display Auditees [@302~4] popup screen
		//Allow user to feed/edit records as normal
		m_strScreenTag = "HSE_TGAUDITEES";		
		InitScreenDefaultValue(0,"HSE_TGAUDITEES",1);
		AddFieldDefaultValue(0,0,"AUDITEES_LNK",strKeyField);		
		return OpenScreen("Auditees","",ID_HSE_AUDITEES,0);
	}
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


HRESULT AuditPlanResultEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}
	

