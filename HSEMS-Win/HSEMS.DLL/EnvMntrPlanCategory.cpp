#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMntrPlanCategory.h"
#include "HSEobj.h"

EnvMntrPlanCategory::EnvMntrPlanCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ENVMNTRPLAN";
	m_ScreenTgName="HSE_TGENVMNTRPLAN";
}

EnvMntrPlanCategory::~EnvMntrPlanCategory(){}


long EnvMntrPlanCategory::ShowEnvMntrPlan(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EnvMntrPlan;
	return ParentManage("HSE_TGENVMNTRPLAN",TRUE,TRUE,TRUE,"HSE_ENVMNTRPLAN",strCriteria);	
}
int gCurrentlyInTheFunction=false; //Bugs-10-2021.16
HRESULT EnvMntrPlanCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	if(!gCurrentlyInTheFunction)//Bugs-10-2021.16
	{		
		gCurrentlyInTheFunction=true;
		CString strForm		  = pRecordRepostion->Form_Tag;
		CString strSubFormTag = pRecordRepostion->SubForm_Tag;	
		CString strPlnStts = FormGetField("HSE_ENVMNTRPLAN","ENVMNTRPLAN_PLNSTTS");	
		int nPlnStts = atoi(strPlnStts);
		if(nPlnStts >= 5) {
			EnableToolbar("",TOOLBAR_DELETE,FALSE);		
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			EnableToolbar("",TOOLBAR_SAVE,FALSE);
			LockRecord("","",true);
		} else {
			EnableToolbar("",TOOLBAR_DELETE,TRUE);		
			EnableToolbar("",TOOLBAR_NEW,TRUE);
			EnableToolbar("",TOOLBAR_SAVE,TRUE);
			LockRecord("","",false);
		}
		if(strSubFormTag == "HSE_TGENVMNTRPLANACTVTS"){
			int nActvtyStts = atoi(FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_ACTVSTTS"));	
			if(nActvtyStts >= 5) {
				EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
			}
		}
		gCurrentlyInTheFunction=false;
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);	
}

HRESULT EnvMntrPlanCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "") {
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	}
	CString strPrmKy = FormGetField("HSE_ENVMNTRPLAN","EnvMntrPlan_Yr");
	if(strButtonName=="ENVMNTRPLAN_APRVPLAN")
	{
		approveEnvMntrPlan(strPrmKy);		
	} else if(strButtonName=="ENVMNTRPLAN_REOPNPLN") {
		reOpnEnvMntrPlan(strPrmKy);
	} else if(strButtonName=="ENVMNTRPLAN_RELOADACTVTS") {
		int nPlanStts = atoi(FormGetField("HSE_ENVMNTRPLAN","ENVMNTRPLAN_PLNSTTS"));
		if(nPlanStts < 5) {
			int nRet=0;
			nRet = MessageBox(NULL,"Are you sure to re-load the activity records?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
			if(nRet == IDYES) {
				CString strPrmKy = FormGetField("HSE_ENVMNTRPLAN","EnvMntrPlan_Yr");
				reloadEnvMntrPlnActvty(strPrmKy);
			}
		}
	} else if(strButtonName=="ENVMNTRPLANACTVTS_PLTNTS") {				
		CString strSQL;	
		bool bLocked = true;
		CString strMainLinkFeild = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_PRMKY");	
		CString strDefValues;
		setOwnerScreenTag("HSE_TGENVMNTRPLAN");
		strDefValues.Format("ENVMNTRACTVTSPLTNT_LNK|%s",strMainLinkFeild);	
		strSQL.Format("SELECT * FROM HSE_ENVMNTRACTVTSPLTNT WHERE ENVMNTRACTVTSPLTNT_LNK = %s",strMainLinkFeild);
		ShowScreen(0,"HSE_TGENVMNTRACTVTSPLTNT","Measures",NORMAL_MODE,false,strSQL,strFormTag,strDefValues,"",bLocked);	
	} 
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void EnvMntrPlanCategory::approveEnvMntrPlan(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE approveEnvMntrPlan %s",strPrmryKey);
	long lRetVal = ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvMntrPlanCategory::reOpnEnvMntrPlan(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE reOpnEnvMntrPlan %s",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvMntrPlanCategory::reloadEnvMntrPlnActvty(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE reloadEnvMntrPlnActvty %s",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

HRESULT EnvMntrPlanCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	////////////////////////////////////////////////////////////////////
	/*RQ-3-2021.12
		Environmental Monitoring Plan	
		Ability to insert/repeat the same (activity) in the same (target month)	
		You may need to use (serial No.) as a record key instead
	*/

	CString strFieldName(pFieldChange->Field_Name);	
	strFieldName.MakeUpper();
	if(strFieldName.Find("ENVMNTRPLANACTVTS")>=0)
	{
		CString strLineNo = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_SRIL");
		if(strLineNo == "") {
			getNewSerialNoForATab("HSE_TGENVMNTRPLAN","EnvMntrPlan_Yr","HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_LNK","ENVMNTRPLANACTVTS_SRIL");
			
		}
	}
	////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}