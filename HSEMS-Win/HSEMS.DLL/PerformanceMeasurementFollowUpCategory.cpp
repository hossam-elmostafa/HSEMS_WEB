// PerformanceMeasurementFollowUpCategory.cpp: implementation of the PerformanceMeasurementFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PerformanceMeasurementFollowUpCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PerformanceMeasurementFollowUpCategory::PerformanceMeasurementFollowUpCategory(CCategory *pCategory):PerformanceMeasurementCategory(pCategory)
{
	m_pCategory = pCategory;
}
PerformanceMeasurementFollowUpCategory::~PerformanceMeasurementFollowUpCategory()
{
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	#define	CHECK_FOR_Performance_Measuremenr_FollowUp_REQUIRED	"HSEPLC_PERFMSRMNTFLW_UPRQRD"
	bool bIsPERFollowRqrd=CheckPolicy(CHECK_FOR_Performance_Measuremenr_FollowUp_REQUIRED);
	if(bIsPERFollowRqrd==true)
	{
		return PerformanceMeasurementCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		ShowMessageEx(IDS_PERFOLLOW,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementFollowUpCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//303_340~2_1
	//Change the (record status [20@340]) to (6)
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strButtonName=="PERFMSRMNTENT_CLSVST") {
		ClosePerformanceMeasure(pCustomButtonClicked);				
	} 
	return PerformanceMeasurementCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return PerformanceMeasurementCategory::DisplayScreenReady(pScreenReady);
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementFollowUpCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="DELETE" && strSubFormTag == "HSE_TGPERFMSRMNTFLWUPRQRDACTN")
	{
		CString strSrcScr = FormGetField("HSE_PERFMSRMNTENTRQRDACTN","PERFMSRMNTENTRQRDACTN_ENTRDIN");
		if(strSrcScr != "Performance Visit Follow-up") {
			*pToolBarButtonClicked->iCancle=1;
			ShowMessageEx(IDS_PrfDlt,"",IV_NOICON,"Warning");
		}
	}
	return PerformanceMeasurementCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/************************************************************************************************************
Helper function
************************************************************************************************************/
void PerformanceMeasurementFollowUpCategory::ClosePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE closePrfrmncMsrmntTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

HRESULT PerformanceMeasurementFollowUpCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	if(strSubFormTag == "HSE_TGPERFMSRMNTFLWUPSECNDEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTFLWUPSECNDEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTFLWUPSECNDEVAL",TOOLBAR_DELETE,FALSE);
	}
	if(strSubFormTag == "HSE_TGPERFMSRMNTFLWUPGRPEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTFLWUPGRPEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTFLWUPGRPEVAL",TOOLBAR_DELETE,FALSE);
	} 
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);
}

long PerformanceMeasurementFollowUpCategory::ShowPerformanceMeasurementFollowUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTFLWUP;
	return ParentManage("HSE_TGPERFMSRMNTFLWUP",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTENT",strCriteria);
}