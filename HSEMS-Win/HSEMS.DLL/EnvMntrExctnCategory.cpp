#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMntrExctnCategory.h"
#include "HSEobj.h"

EnvMntrExctnCategory::EnvMntrExctnCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ENVMNTRPLAN";
	m_ScreenTgName="HSE_TGENVMNTREXCTN";
}

EnvMntrExctnCategory::~EnvMntrExctnCategory(){}


long EnvMntrExctnCategory::ShowEnvMntrExctn(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EnvMntrExctn;
	return ParentManage("HSE_TGENVMNTREXCTN",FALSE,FALSE,FALSE,"HSE_ENVMNTRPLAN",strCriteria);	
}

HRESULT EnvMntrExctnCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strFullRecord(pCustomButtonClicked->Full_Record);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();	
	int Count = 0;
	if(strSubFormTag == "") {
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	}
	CString strPrmKy = FormGetField("HSE_ENVMNTRPLAN","EnvMntrPlan_Yr");
	if(strButtonName=="ENVMNTRPLAN_CLSPLAN")
	{
		closeEnvMntrPlan(strPrmKy);		
	} else if(strButtonName=="ENVMNTREXCTNACTVTS_PLTNTS") {
		CString strSQL;	
		bool bLocked = false;
		CString strMainLinkFeild = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_PRMKY");	
		CString strActivityStatus = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_ACTVSTTS");	
		CString strDefValues;
		setOwnerScreenTag("HSE_TGENVMNTREXCTN");
		strDefValues.Format("ENVMNTRACTVTSPLTNT_LNK|%s|ActivityStatusHolder|%s",strMainLinkFeild,strActivityStatus);	
		strSQL.Format("SELECT * FROM HSE_ENVMNTRACTVTSPLTNT WHERE ENVMNTRACTVTSPLTNT_LNK = %s",strMainLinkFeild);
		ShowScreen(0,"HSE_TGENVMNTRACTVTSPLTNT","Measures",NORMAL_MODE,false,strSQL,strFormTag,strDefValues,"",bLocked);
	} 
	
	else if(strButtonName=="ENVMNTREXCTNACTVTS_CMPLTACTVTY") {
		changeEnvMsrActvtStat(Completed,FormGetField("HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_PrmKy"));
		CString strActvtyStat = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_ACTVSTTS");
		if(strActvtyStat == "5") {
			LockRecord("","HSE_TgENVMNTREXCTNACTVTS",true);
		}
	}
	
	else if(strButtonName=="ENVMNTREXCTNACTVTS_CNCLACTVTY") {
		changeEnvMsrActvtStat(Cancelled,FormGetField("HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_PrmKy"));
		LockRecord("","HSE_TgENVMNTREXCTNACTVTS",false);
	} else if(strButtonName=="ENVMNTREXCTNACTVTS_REOPNACTVTY") {
		changeEnvMsrActvtStat(Pending,FormGetField("HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_PrmKy"));
		LockRecord("","HSE_TgENVMNTREXCTNACTVTS",false);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT EnvMntrExctnCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1 && strBtnName=="SAVETAB")
	{		
		//When save in tab,
		RefreshScreen("",REFRESH_SELECTED);											
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT EnvMntrExctnCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strForm		  = pRecordRepostion->Form_Tag;
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;	
	CString strActvtyStts = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_ACTVSTTS");	
	int nActvtyStts = atoi(strActvtyStts);
	if(nActvtyStts >= 5) {
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);		
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_SAVE,FALSE);		
		
		//HSEMS
		//RQ-1-2021	19
		//If (activity status) = ”completed”, then LOCK the custom-buttons (enter measures, complete activity, cancel activity)
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_PLTNTS",false,true);
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_CMPLTACTVTY",false,true);
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_CNCLACTVTY",false,true);				
		
		LockRecord("",strSubFormTag,true);
	} else {
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);		
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,TRUE);
		EnableToolbar(strSubFormTag,TOOLBAR_SAVE,TRUE);
		
		//HSEMS
		//RQ-1-2021	19
		//If (activity status) = ”completed”, then LOCK the custom-buttons (enter measures, complete activity, cancel activity)
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_PLTNTS",true,true);
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_CMPLTACTVTY",true,true);
		FormEnableButton(2,1,"CUSTOM_ENVMNTREXCTNACTVTS_CNCLACTVTY",true,true);
		
		LockRecord("",strSubFormTag,false);
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);	
}

void EnvMntrExctnCategory::closeEnvMntrPlan(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE closeEnvMntrPlan %s",strPrmryKey);
	long lRetVal = ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvMntrExctnCategory::changeEnvMsrActvtStat(int ActivityNewStatus,CString ActivityPrmKy)
{
	CString strSQL;
	strSQL.Format("EXECUTE changeEnvMsrActvtStat %d,%s",ActivityNewStatus,ActivityPrmKy);	
	long lRetVal = ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

HRESULT EnvMntrExctnCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	////////////////////////////////////////////////////////////////////
	/*RQ-3-2021.13
		Environment\Environmental Measures	
		Similar to (environmental monitoring plan) the ability to insert/repeat the same (activity) in the same (target month)
		as above
	*/

	CString strFieldName(pFieldChange->Field_Name);	
	strFieldName.MakeUpper();
	if(strFieldName.Find("ENVMNTRPLANACTVTS")>=0)
	{
		CString strLineNo = FormGetField("HSE_ENVMNTRPLANACTVTS","ENVMNTRPLANACTVTS_SRIL");
		if(strLineNo == "") {
			getNewSerialNoForATab("HSE_TGENVMNTREXCTN","EnvMntrPlan_Yr","HSE_ENVMNTRPLANACTVTS","EnvMntrPlanActvts_LNK","ENVMNTRPLANACTVTS_SRIL");
			
		}
	}
	////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}