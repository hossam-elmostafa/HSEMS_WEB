// VehicleAccidentFollowUpCategory.cpp: implementation of the VehicleAccidentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "VehicleAccidentFollowUpCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VehicleAccidentFollowUpCategory::VehicleAccidentFollowUpCategory(CCategory *pCategory):VehicleAccident(pCategory)
{
	m_pCategory = pCategory;
}

VehicleAccidentFollowUpCategory::~VehicleAccidentFollowUpCategory()
{

}
/*************************************************************************************************************/
HRESULT VehicleAccidentFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//409_510~2_1
	//This screen is permitted only if (HSE policy, Vehicle Accident Follow-up  checkbox [16@600])= checked
	bool bIsAccdntFlwUpRqrd=CheckPolicy("HSEPLC_VCLACDNTFLW_UPRQRD");
	if(bIsAccdntFlwUpRqrd==true)
	{
		return VehicleAccident::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	//Else message box displayed to user with this message 
	//"Vehicle Accident Follow-up checkbox must be checked to open this screen"
	else
	{
		ShowMessageEx(IDS_VACR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}

}
/*************************************************************************************************************/
HRESULT VehicleAccidentFollowUpCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//409_510~2_2	
	CString strBtnName(pCustomButtonClicked->Button_Name);
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	if(strBtnName=="VCLACDNTENT_CLS")
	{
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			CloseVehicleAccident(pCustomButtonClicked);
	}
	return VehicleAccident::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT VehicleAccidentFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return VehicleAccident::DisplayScreenReady(pScreenReady);
}
/*************************************************************************************************************/
HRESULT VehicleAccidentFollowUpCategory::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	//To disable NEW and DELETE buttons "Framework bug disable function doesn't work in tabs"
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;
	if(strSubFormTag == "HSE_TGVCLACDNTFLWUPRQDACTN") {
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
	}	
	return VehicleAccident::DisplayRecordRepostion(pRecordRepostion);
}




/************************************************************************************************************
Helper function
************************************************************************************************************/
void VehicleAccidentFollowUpCategory::CloseVehicleAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Update (Record Status[60@510]) of selected record into (6)
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,6,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
	//"	Insert record into (tracing [@51020~2]) tab
	InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",CLOSED);
}

		
	
long VehicleAccidentFollowUpCategory::ShowVehicleAccidentFollowUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCLACDNTFLWUP;
	return ParentManage("HSE_TGVCLACDNTFLWUP",TRUE,TRUE,TRUE,"HSE_VCLACDNTENT",strCriteria);
}