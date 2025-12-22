#include "stdafx.h"
#include "HSEMS.h"
#include "DrillPlanAprvCategory.h"
#include "HSEobj.h"

DrillPlanAprvCategory::DrillPlanAprvCategory(CCategory *pCategory):DrillEntryCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_DRLLPLN_VW";
	m_ScreenTgName="HSE_DRLLPLNAPPRVLATAPPRVL";
}

DrillPlanAprvCategory::~DrillPlanAprvCategory(){}

HRESULT DrillPlanAprvCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	
	
	//HSE BGC-EZZ Requirements_2892020
	//EnableToolbar("",TOOLBAR_SAVE,FALSE);


	/*int nScreenType = pScreenReady->nScreenType;
	int iScreenMode = pScreenReady->iScreenMode;
	if(allowAspctEntry() == "N"){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		if(iScreenMode == 1) {
			EnableToolbar("",TOOLBAR_SAVE,FALSE);			
		}
		EnableTab("HSE_ASPCTS_ASPCTS",false);
		EnableTab("HSE_ASPCTS_TRCNG",false);
	}*/ 	
	return DrillEntryCategory::DisplayScreenReady(pScreenReady);
}

HRESULT DrillPlanAprvCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	/*CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1 && strBtnName=="NEW")
	{						
		FormSetField("HSE_ASPCTS","ASPCTS_YR",getAspctYr());
		FormSetField("HSE_ASPCTS","ASPCTS_MNTH",getAspctMonth());
	}*/
	return DrillEntryCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


HRESULT DrillPlanAprvCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	/*CString strAspctStatus = FormGetField("HSE_ASPCTS","ASPCTS_ASPCTSTTUS");	
	int nRecordStatus = atoi(strAspctStatus);
	if(nRecordStatus >= 5) {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
	}*/
	return DrillEntryCategory::DisplayRecordRepostion(pRecordRepostion);
}


HRESULT DrillPlanAprvCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	return DrillEntryCategory::DisplayFieldChange(pFieldChange);
}


HRESULT DrillPlanAprvCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="APPROVE_DRILL_PLAN")
	{
		ApproveDrillPlan(pCustomButtonClicked);
	} else if(strButtonName=="REJECT_DRILL_PLAN") {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to reject the drill plan?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			RejectDrillPlan(pCustomButtonClicked);
		}
	}			
	/*if(strButtonName=="PLAN_COMPLETED")
	{
		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompleteDrillPlan(pCustomButtonClicked);
	} else if(strButtonName=="CREATE_UN-PLANNED_DRILL") {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to create new un-planned drill?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			DoToolBarAction(TOOLBAR_NEW,"HSE_DRLLPLNENTRYATENTRY","");
			FormSetField("HSE_DRLLPLN","DRLLPLN_DRLLSTTUS",10);		//TODO:a.azmy
			FormSetField("HSE_DRLLPLN","DRLLPLN_DRLLPLNND","N");	
		}
	} else if(strButtonName=="RE-LOAD_SCENARIO") {
		reloadScenario(pCustomButtonClicked);
	}*/
	return DrillEntryCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long DrillPlanAprvCategory::ShowDrillPlanAprv(CString strCriteria)
{	
	m_nCurrentCard=HSE_DRILL_PLAN_ENTRY_ENTRY;
	return ParentManage("HSE_DRLLPLNAPPRVLATAPPRVL",TRUE,TRUE,TRUE,"HSE_DRLLPLN_VW",strCriteria);	
}

void DrillPlanAprvCategory::ApproveDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strKeyFld = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strSQL;	
	strSQL.Format("EXECUTE ApproveDrillPlan '%s'",strKeyFld);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void DrillPlanAprvCategory::RejectDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strKeyFld = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strSQL;	
	strSQL.Format("EXECUTE RejectDrillPlan '%s'",strKeyFld);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}