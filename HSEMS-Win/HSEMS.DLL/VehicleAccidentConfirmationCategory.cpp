// VehicleAccidentConfirmationCategory.cpp: implementation of the VehicleAccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "VehicleAccidentConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VehicleAccidentConfirmationCategory::VehicleAccidentConfirmationCategory(CCategory *pCategory):VehicleAccident(pCategory)
{
	m_pCategory = pCategory;
}

VehicleAccidentConfirmationCategory::~VehicleAccidentConfirmationCategory()
{

}
/*************************************************************************************************************/
HRESULT VehicleAccidentConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
//408_510~1_1
//This screen is permitted only if the (HSE policy, Vehicle Accident Confirmation checkbox [15@600]) = checked
	bool bIsVhclAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_VCLACDNTCNFRMTNRQRD");
	if(bIsVhclAccdntCnfrmtnRqrd==true)
	{
		return VehicleAccident::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{
		//Else message box displayed to user with this message 
		//"{Vehicle Accident Confirmation  checkbox must be checked to open this screen"
		ShowMessageEx(IDS_VACR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/*************************************************************************************************************/
HRESULT VehicleAccidentConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	strBtnName.MakeUpper();
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	//408_510~1_2
	if(strBtnName=="VCLACDNTENT_CNFRM")
	{	
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ConfirmVehicleAccident(pCustomButtonClicked);
	}
	return VehicleAccident::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT VehicleAccidentConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return VehicleAccident::DisplayScreenReady(pScreenReady);
}
/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void VehicleAccidentConfirmationCategory::ConfirmVehicleAccident(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strVclacdntNo=FormGetField("HSE_VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
	bool RjctRsnExist=IsNewRjcRsnExist("VCLACDNTENT-L1",strVclacdntNo); 
	if(RjctRsnExist==1)
	{
		bool bDeleteRjcRsn=showApproveMessageBox("VCLACDNTENT-L1",strVclacdntNo);
		if(bDeleteRjcRsn==true)
		{
			//Update (Record Status [60@510]) of selected record into (5)
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
			//Insert record into (tracing [@51020~1]) tab
			InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",CONFIRM);		
		}
	}
	else
	{
		//Update (Record Status [60@510]) of selected record into (5)
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
		//Insert record into (tracing [@51020~1]) tab
		InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",CONFIRM);		
	}
}

long VehicleAccidentConfirmationCategory::ShowVehicleAccidentConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCLACDNTCNFRMTN;
	return ParentManage("HSE_TGVCLACDNTCNFRMTN",TRUE,TRUE,TRUE,"HSE_VCLACDNTENT",strCriteria);
}