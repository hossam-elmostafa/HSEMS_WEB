// VehicleAccidentEntryCategory.cpp: implementation of the VehicleAccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "VehicleAccidentEntryCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VehicleAccidentEntryCategory::VehicleAccidentEntryCategory(CCategory *pCategory):VehicleAccident(pCategory)
{
	m_pCategory = pCategory;
}

VehicleAccidentEntryCategory::~VehicleAccidentEntryCategory()
{

}
/************************************************************************************************************/
HRESULT VehicleAccidentEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
//407_510_1
//If (driver license expiry date [19]) < (today date) then display just a warning message.	
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="VCLACDNTENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;

		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompleteVehicleAccident(pCustomButtonClicked);
	}
	return VehicleAccident::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT VehicleAccidentEntryCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	//change form tag from HSE_TGVCLACCDNTQSTNS to HSE_TGVCLACCDNTQSTNSENTRY 
	if(strSubFormTag == "HSE_TGVCLACCDNTQSTNSENTRY")
	{
		EnableToolbar("HSE_TGVCLACCDNTQSTNSENTRY",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGVCLACCDNTQSTNSENTRY",TOOLBAR_DELETE,FALSE);
	}
	//a.ali TODO check why refresh tab on every tab selected?
	//RefreshScreen("HSE_TGVCLACCDNTQSTNS",0);
	return VehicleAccident::DisplayTabSelected(pTabSelected);
}
/************************************************************************************************************/

/************************************************************************************************************
Helper function
************************************************************************************************************/
void VehicleAccidentEntryCategory::CompleteVehicleAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Save record before check for driver license expiry date modified by Eng Hossam
	DoToolBarAction(TOOLBAR_SAVE,"HSE_TGVCLACDNTENT","");
	bool bIsDrvrLsnValid=CheckLicenseExpireDate();
	if(bIsDrvrLsnValid==false)
	{
		ShowMessageEx(IDS_DNV,"",IV_NOICON,"Policy change required");
	}
	else
	{
		//If (Vehicle Accident Confirmation) checkbox [15@600] = "un-checked", then
		bool bIsVhclAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_VCLACDNTCNFRMTNRQRD");
		if(bIsVhclAccdntCnfrmtnRqrd==false)
		{
			//Set (Record Status [60]) = 5				
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
			//Insert record into (tracing) tab [@51020]
			InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",CONFIRM);
		}
		else
		{
			//Set (Record Status [60]) = 4
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
			//Insert record into (tracing) tab [@51020]
			InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",COMPLETE);
		}
	}
}


long VehicleAccidentEntryCategory::ShowVehicleAccidentEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCLACDNTENT;
	return ParentManage("HSE_TGVCLACDNTENT",TRUE,TRUE,TRUE,"HSE_VCLACDNTENT",strCriteria);
}