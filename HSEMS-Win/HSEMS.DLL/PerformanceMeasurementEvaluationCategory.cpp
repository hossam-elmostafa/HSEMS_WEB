// PerformanceMeasurementEvaluationCategory.cpp: implementation of the PerformanceMeasurementEvaluationCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "PerformanceMeasurementEvaluationCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PerformanceMeasurementEvaluationCategory::PerformanceMeasurementEvaluationCategory(CCategory *pCategory):PerformanceMeasurementCategory(pCategory)
{
	m_pCategory = pCategory;
}
PerformanceMeasurementEvaluationCategory::~PerformanceMeasurementEvaluationCategory()
{
}
/*************************************************************************************************************/
HRESULT PerformanceMeasurementEvaluationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	#define	CHECK_FOR_Performance_Measuremenr_Evaluation_REQUIRED "HSEPLC_PERFMSRMNTEVALRQRD"
	bool bIsPERCnfrmnRqrd=CheckPolicy(CHECK_FOR_Performance_Measuremenr_Evaluation_REQUIRED);
	if(bIsPERCnfrmnRqrd==true) {
		return PerformanceMeasurementCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		ShowMessageEx(IDS_PERFEVAL,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/*************************************************************************************************************/
HRESULT PerformanceMeasurementEvaluationCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//302_340~1_1
	//If (performance measurement follow-up) checkbox [10@600] = "un-checked", then
	// Set (Record Status [20@340]) = 6
	CString strButtonName(pCustomButtonClicked->Button_Name);
	if(strButtonName=="PERFMSRMNTENT_ENTCMPLT")
	{
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			CompletePerformanceMeasure(pCustomButtonClicked);
	} 
	return PerformanceMeasurementCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT PerformanceMeasurementEvaluationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return PerformanceMeasurementCategory::DisplayScreenReady(pScreenReady);
}
/*************************************************************************************************************/
HRESULT PerformanceMeasurementEvaluationCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);			
	if(strFieldName=="PERFMSRMNTENTVSTTM_COMPEMP")
	{
		//101_302_1
		//Allow user either to feed the company employee information [4], [5], [6], [7] 
		//or non-company employee information [9], [10], [11], [12] but not both 		
		CString strCompEmp(pFieldChange->New_Value);
		if(strCompEmp=="0")
		{		
			FormSetField("HSE_PERFMSRMNTEVALVSTTM","PERFMSRMNTENTVSTTM_EMPNUM","");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_Dprt34","");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_JobTtl44","");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_Nam24","");
		}
		if(strCompEmp=="1")
		{		
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_NAM"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_COMP"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_DPRT"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_JOBTTL"," ");
		}
		selectEmployeeType(pFieldChange,"HSE_TGPERFMSRMNTENTVSTTM");
	}	
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}
/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void PerformanceMeasurementEvaluationCategory::CompletePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE confirmPrfrmncMsrmntTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

HRESULT PerformanceMeasurementEvaluationCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	if(strSubFormTag == "HSE_TGPERFMSRMNTEVALSECNDEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTEVALSECNDEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTEVALSECNDEVAL",TOOLBAR_DELETE,FALSE);
	}
	if(strSubFormTag =="HSE_TGPERFMSRMNTEVALGRPEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTEVALGRPEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTEVALGRPEVAL",TOOLBAR_DELETE,FALSE);
	} 
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);
}

long PerformanceMeasurementEvaluationCategory::ShowPerformanceMeasurementEvaluation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTEVAL;
	return ParentManage("HSE_TGPERFMSRMNTEVAL",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTENT",strCriteria);
}






