// RiskAssessmentFollowUpCategory.cpp: implementation of the RiskAssessmentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "RiskAssessmentFollowUpCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RiskAssessmentFollowUpCategory::RiskAssessmentFollowUpCategory(CCategory *pCategory):RiskAssessmentCategory(pCategory)
{
	m_pCategory = pCategory;
}

RiskAssessmentFollowUpCategory::~RiskAssessmentFollowUpCategory()
{

}
/************************************************************************************************************/
HRESULT RiskAssessmentFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//103_300~2_1
	//This screen is permitted only if the (HSE policy, Potential Hazard Confirmation checkbox [8@600]) = checked	
	//update : field removed from system
	/*bool bIsAssmntFlwUpRqrd=CheckPolicy(CHECK_FOR_RISK_ASSESSMENT_FOLLOWUP_REQUIRED);
	if(bIsAssmntFlwUpRqrd==true)
	{
		return RiskAssessmentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);	
	}
	else
	{
		//Else message box displayed to user with this message "Risk Assessment follow-up checkbox In HSE POLICY screen must be checked to open this screen"
		ShowMessageEx(IDS_RAFM,"",IV_NOICON, "Policy change required");
		return S_FALSE;
	}*/
	return RiskAssessmentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);	
}
/************************************************************************************************************/
HRESULT RiskAssessmentFollowUpCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
	//103_300~2_2
	if(strButtonName=="RSKASSMNTENT_CLS")
	{		
		/*Deleted Code,Please Check Source Safe*/
		CString strSQL;
		CString strUserName = GetUserLogin();
		CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
		strSQL.Format("EXECUTE closeRskAssmntTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
	return RiskAssessmentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/************************************************************************************************************/
HRESULT RiskAssessmentFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	return RiskAssessmentCategory::DisplayScreenReady(pScreenReady);
}
/************************************************************************************************************/
HRESULT RiskAssessmentFollowUpCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{				
		if(strSubFormTag=="HSE_TGRSKASSMNTFOLLWUPDTRJCRSN")
		{																						
			SetRjctRsnFlag(pToolBarButtonClicked,"RSKASSMNTENT_ASSMNTNO");					
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/************************************************************************************************************/
HRESULT RiskAssessmentFollowUpCategory::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	//To disable NEW and DELETE buttons "Framework bug disable function doesn't work in tabs"
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;
	if(strSubFormTag == "HSE_TGRSKASSMNTFOLLWUPDTRQRDACTN") {
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
	}	
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}
/************************************************************************************************************
helper function
************************************************************************************************************/
//@deprecated
void RiskAssessmentFollowUpCategory::CloseRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Update (Record Status[25]) of selected record = 6
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,6,"RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
	//Insert record into (tracing [@30020~3]) tab
	InsertActionRecordIntoTracingTab("RSKASSMNTENT","ASSMNTNO",CLOSED);
	RefreshScreen("",REFRESH_SELECTED);
}

long RiskAssessmentFollowUpCategory::ShowRiskAssessmentFollowUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKASSMNTFOLLWUPDT;		
	return ParentManage("HSE_TGRSKASSMNTFOLLWUPDT",TRUE,TRUE,TRUE,"HSE_RSKASSMNTENT",strCriteria);	
}
