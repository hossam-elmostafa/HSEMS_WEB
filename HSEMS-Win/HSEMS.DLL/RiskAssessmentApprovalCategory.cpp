// RiskAssessmentApprovalCategory.cpp: implementation of the RiskAssessmentApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "RiskAssessmentApprovalCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RiskAssessmentApprovalCategory::RiskAssessmentApprovalCategory(CCategory *pCategory):RiskAssessmentCategory(pCategory)
{
	m_pCategory = pCategory;
}


RiskAssessmentApprovalCategory::~RiskAssessmentApprovalCategory()
{

}
/**************************************************************************************************************/
HRESULT RiskAssessmentApprovalCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strSQL	=	"";
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
	
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	//102_300~1_2
	if(strButtonName=="RSKASSMNTENT_APRV")
	{		
		if(KeyFieldValue != "")
			ApproveRiskAssessment(pCustomButtonClicked);
	}
	return RiskAssessmentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/**************************************************************************************************************/
HRESULT RiskAssessmentApprovalCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//102_300~1_1
	//Action : Open Screen
	//This screen is permitted only if the (HSE policy, Risk Assessment Approval) checkbox [6@600] = checked
	//Update field HSEPLC_RSKASSMNTAPRRQRD removed from system

	/*bool bIsAssmntApprvRqrd=CheckPolicy("HSEPLC_RSKASSMNTAPRRQRD");
	if(bIsAssmntApprvRqrd==true)
	{
		return RiskAssessmentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);	
	}
	else
	{
		//Else message box displayed to user with this message "Risk Assessment follow-up checkbox In HSE POLICY screen must be checked to open this screen"
		ShowMessageEx(IDS_RAAR,"",IV_NOICON, "Policy change required");
		return S_FALSE;
	}*/
	return RiskAssessmentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);	
}
/**************************************************************************************************************/
HRESULT RiskAssessmentApprovalCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return RiskAssessmentCategory::DisplayScreenReady(pScreenReady);
}
/**************************************************************************************************************/

/**************************************************************************************************************
Helper function
**************************************************************************************************************/
void RiskAssessmentApprovalCategory::ApproveRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//CString strMainKeyFeild = FormGetField("HSE_RSKASSMNTENT" ,"RSKASSMNTENT_ASSMNTNO");
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	

	setOwnerScreenTag(m_strScreenTag);
	//check if user enter rejcet reason before approve selected record
	//note : modified by mr sameer 21/11/2009
	bool RjctRsnExist=IsNewRjcRsnExist("RSKASSMNTENT-L1",KeyFieldValue); 
	if(RjctRsnExist==1)
	{
		bool bDeleteRjcRsn=showApproveMessageBox("RSKASSMNTENT-L1",KeyFieldValue);
		if(bDeleteRjcRsn==true)
		{
			/*deleted Code ,Please Check Source Safe*/			
			strSQL.Format("EXECUTE confirmRskAssmntTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
		
	} else {
		/*//Update (Record Status [25@300]) of selected record into (5)
		deleted Code,Please check Source Safe*/
		strSQL.Format("EXECUTE confirmRskAssmntTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}	
}


long RiskAssessmentApprovalCategory::ShowRiskAssessmentApproval(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKASSMNTAPR;	
	return ParentManage("HSE_TGRSKASSMNTAPR",TRUE,TRUE,TRUE,"HSE_RSKASSMNTENT",strCriteria);
}