#include "stdafx.h"
#include "HSEMS.h"
#include "DrillPlanEntryCategory.h"
#include "HSEobj.h"

DrillPlanEntryCategory::DrillPlanEntryCategory(CCategory *pCategory):DrillEntryCategory(pCategory) {
	m_pCategory = pCategory;
	//m_FSATableName="HSE_DRLLPLN";
	m_FSATableName="HSE_DRLLPLN_VW";
	m_ScreenTgName="HSE_DRLLPLNENTRYATENTRY";
}

DrillPlanEntryCategory::~DrillPlanEntryCategory(){}

HRESULT DrillPlanEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	/*if(allowDrillEntry() == "N"){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		EnableToolbar("",TOOLBAR_SAVE,FALSE);
		LockRecord("","",true);
	} else {
		CString strDrllYr = getPolicyDrillYr();	
		ChangeCriteria("HSE_DRLLPLNENTRYATENTRY","","WHERE (DRLLPLN_DRLLYR="+strDrllYr+" AND DRLLPLN_DRLLSTTUS < 5)");
	}*/	
	return DrillEntryCategory::DisplayScreenReady(pScreenReady);
}

extern bool gCustomButtonClicked;
HRESULT DrillPlanEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strFullRecord(pToolBarButtonClicked->Full_Record);
	if(pToolBarButtonClicked->iComplete==1)
	{	
		if(strBtnName=="NEW") {
			FormSetField("HSE_DRLLPLN_VW","DRLLPLN_DRLLYR",getPolicyDrillYr());
		} else if(strBtnName=="SAVE") {
			if(strSubFormTag == "") {
				FormSetField("HSE_DRLLPLN_VW","DRLLPLN_LSTMDFYDT",GetFullDateTime("%m/%d/%Y"));
				FormSetField("HSE_DRLLPLN_VW","DRLLPLN_LSTMDFYBY",GetUserLogin());
			}
		}
	}

	if(pToolBarButtonClicked->iComplete==0)
	{
		//////////////////////////////////////////////
		//RQ_HM_20_3_23_1 
		if(strBtnName=="NEW") {
			if(strFormTag=="HSE_DRLLPLNENTRYATENTRY"){
				if(allowDrillEntry()=="N" && !gCustomButtonClicked){
					//AfxMessageBox("HSE Policy prevents adding new Planned Drills. Kindly use Ceate Un-planned Drill instead");
					MessageBox(0,"HSE Policy prevents adding new Planned Drills. \nKindly use \"Create Un-planned Drill\" instead","Not Allowed",0);
					*pToolBarButtonClicked->iCancle = 1;
				}
			}
		}
	}
	if(pToolBarButtonClicked->iComplete==1)
	{
		if(strBtnName=="NEW") {
			if(strFormTag=="HSE_DRLLPLNENTRYATENTRY"){
				//RQ-6-2023.1
				CString strSQL;
				strSQL.Format("exec GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST'");
				CString strDefaultSite= GetRecordSetData(strSQL).GetBuffer(0);
				FormSetField("HSE_DRLLPLNENTRYATENTRY","DRLLPLN_ST",strDefaultSite);
				//////////////////////////////////////////////
			}
		}
		
	}
	return DrillEntryCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT DrillPlanEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	/*CString strAspctStatus = FormGetField("HSE_ASPCTS","ASPCTS_ASPCTSTTUS");	
	int nRecordStatus = atoi(strAspctStatus);
	if(nRecordStatus >= 5) {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);*/
	return S_OK;
}


HRESULT DrillPlanEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	return DrillEntryCategory::DisplayFieldChange(pFieldChange);
}

extern bool gCustomButtonClicked=false;

HRESULT DrillPlanEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="PLAN_COMPLETED")
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
			gCustomButtonClicked=true;
			DoToolBarAction(TOOLBAR_NEW,"HSE_DRLLPLNENTRYATENTRY","");
			FormSetField("HSE_DRLLPLN_VW","DRLLPLN_DRLLSTTUS","10");		//TODO:a.azmy
			FormSetField("HSE_DRLLPLN_VW","DRLLPLN_DRLLPLNND","N");	
			gCustomButtonClicked=false;
		}
	} else if(strButtonName=="RE-LOAD_SCENARIO") {
		reloadScenario(pCustomButtonClicked);
	}
	return DrillEntryCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

//*********************************************************************************************************
long DrillPlanEntryCategory::ShowDrillPlanEntry(CString strCriteria)
{	
	m_nCurrentCard=HSE_DRILL_PLAN_ENTRY_ENTRY;
	if(allowDrillEntry() == "N"){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		EnableToolbar("",TOOLBAR_SAVE,FALSE);
		LockRecord("","",true);
	} else {
		CString strDrllYr = getPolicyDrillYr();
		if(strDrllYr != "") {
			if(strCriteria != "") 
				strCriteria += "AND ";
			else 
				strCriteria += "WHERE ";
			strCriteria += "(DRLLPLN_DRLLYR="+strDrllYr+" AND DRLLPLN_DRLLSTTUS < 5)";
		} else {
			AfxMessageBox("Please insert HSE policy- Emergincy Drills al fields first.");
			return -1;
		}
	}
	return ParentManage("HSE_DRLLPLNENTRYATENTRY",TRUE,TRUE,TRUE,"HSE_DRLLPLN_VW",strCriteria);	
}

CString DrillPlanEntryCategory::allowDrillEntry(){
	CString strSQL;
	//RQ-6-2023.10
	//strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_ALWDRLPLNENTRY,'') FROM HSE_HSEPLC");
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_EMRDRL_ALLDRLPLNENT,'') FROM HSE_HSEPLC_EMRDRL");
	//////////////////////////////////////////////////////////////
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctMonth;
}

CString DrillPlanEntryCategory::getPolicyDrillYr(){
	CString strSQL;
	//RQ-6-2023.10
	//strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_DRLYR,'') FROM HSE_HSEPLC");
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_EMRDRL_DRLYR,'') FROM HSE_HSEPLC_EMRDRL");
	////////////////////////////////////////////////////////////////////////////
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strAspctMonth=="0")
		AfxMessageBox("System policy is missing Drill Year");
	return strAspctMonth;
}

void DrillPlanEntryCategory::CompleteDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strSQL;	
	CString strKeyField = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	strSQL.Format("EXECUTE COMPLETE_DRILL_PLAN '%s'",strKeyField);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}


void DrillPlanEntryCategory::reloadScenario(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strSQL;	
	CString strKeyField = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strDTLDrillTyp = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_DTLDRLTYP");
	strSQL.Format("EXECUTE RELOAD_SCENARIO '%s','%s'",strKeyField,strDTLDrillTyp);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);
}