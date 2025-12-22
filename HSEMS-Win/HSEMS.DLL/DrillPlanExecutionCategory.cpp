#include "stdafx.h"
#include "HSEMS.h"
#include "DrillPlanExecutionCategory.h"
#include "HSEobj.h"

DrillPlanExecutionCategory::DrillPlanExecutionCategory(CCategory *pCategory):DrillEntryCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_DRLLPLN_VW";
	m_ScreenTgName="HSE_DRLLEXCUTNATEXCUTN";
}

DrillPlanExecutionCategory::~DrillPlanExecutionCategory(){}

HRESULT DrillPlanExecutionCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	EnableToolbar("HSE_DRLLPLN_EMT",TOOLBAR_NEW,FALSE);
	
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

HRESULT DrillPlanExecutionCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
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

//Moved to DrillEntryCategory
/*HRESULT DrillPlanExecutionCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);	
	if(strSubFormTag == "HSE_DRLLPLN_EMT" ||strSubFormTag == "HSE_DRLLPLN_INTRNLRSURCS" ||strSubFormTag == "HSE_DRLLPLN_EXTRNLSUPPRT" || strSubFormTag == "HSE_DRLLEXCUTNATEXCUTN_INVLVDPRTS" || strSubFormTag == "HSE_DRLLPLN_ERT") {
		CString strSrcScr = FormGetField(strSubFormTag,"src_Scr");
		strSrcScr.MakeUpper();
		if(strSrcScr == strFormTag) {
		  EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);
		} else {
		  EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
		}
	}
	return DrillEntryCategory::DisplayRecordRepostion(pRecordRepostion);
}*/

HRESULT DrillPlanExecutionCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strForm_Tag=pFieldChange->Form_Tag;
	CString strField_Name=pFieldChange->Field_Name;
	CString strNewValue=pFieldChange->New_Value;
	//RQ-11-2021.20
	if(strField_Name=="DRLLPLN_ACTULSCNR_STTUS" && strNewValue=="D")
	{
		CString strEstDuration = FormGetField("HSE_DRLLPLN_ACTULSCNR","DRLLPLN_ACTULSCNR_ESTDRTN");
		////////////////////////////////////////////////////////////////////
		//Bugs-11-2021.7
		//FormSetField("HSE_DRLLPLN_ACTULSCNR","DRLLPLN_ACTULSCNR_EXCUTNTM",strEstDuration);		
		FormSetField("HSE_DRLLPLN_ACTULSCNR","DRLLPLN_ACTULSCNR_ACTLDRTN",strEstDuration);
		////////////////////////////////////////////////////////////////////
		
	}
	////////////////////////////////////////////////////////////////////

	return DrillEntryCategory::DisplayFieldChange(pFieldChange);
}

HRESULT DrillPlanExecutionCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strTableName(pCustomButtonClicked->Table_Name);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="CLOSE_DRILL")
	{
		CloseDrillPlan(pCustomButtonClicked);
	} else if(strButtonName=="CANCELL_DRILL") {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to cancel the drill?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			CancelDrill(pCustomButtonClicked);
		}
	} else if(strButtonName=="RE-LOAD_DRILL_SCENARIO") {
		reloadScenario(pCustomButtonClicked);
	} 
	return DrillEntryCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long DrillPlanExecutionCategory::ShowDrillPlanExecution(CString strCriteria)
{	
	m_nCurrentCard=HSE_DRILL_EXECUTION_EXECUTION;
	return ParentManage("HSE_DRLLEXCUTNATEXCUTN",TRUE,TRUE,TRUE,"HSE_DRLLPLN_VW",strCriteria);	
}


void DrillPlanExecutionCategory::CloseDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strKeyFld = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strSQL;	
	strSQL.Format("EXECUTE CloseDrill '%s'",strKeyFld);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void DrillPlanExecutionCategory::CancelDrill(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strKeyFld = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strSQL;	
	strSQL.Format("EXECUTE CancelDrill '%s'",strKeyFld);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void DrillPlanExecutionCategory::reloadScenario(tag_CustomButtonClicked* pCustomButtonClicked) 
{
	CString strKeyFld = FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY");
	CString strSQL;	
	strSQL.Format("EXECUTE INSERT_ACTUAL_SCNR '%s'",strKeyFld);	
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

