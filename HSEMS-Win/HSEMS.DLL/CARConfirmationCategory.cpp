// arConfirmationCategory.cpp: implementation of the CarConfirmationCategory class.
// Update :
// Screen name changed to Corrective Action Review
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CarConfirmationCategory::CarConfirmationCategory(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGCRCTVEACCCNFRMTN";
	m_strScreen	   = "Corrective Action Review";
}

CarConfirmationCategory::~CarConfirmationCategory()
{

}
/************************************************************************************************************/
HRESULT CarConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//702_430~1_1
	//This screen is permitted only if the (HSE policy, CAR Confirmation checkbox [90]) = checked
	bool bIsCARCnfrmtnRqrd =CheckPolicy(Is_CAR_Confirmation_Required);
	if(bIsCARCnfrmtnRqrd==true)
	{
		return CarCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{
		//Else message box displayed to user with this message 
		//"CAR Confirmation checkbox must be checked to open this screen"
		ShowMessageEx(IDS_CARCR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
	return S_OK;
}
/************************************************************************************************************/
HRESULT CarConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strBtnName.MakeUpper();	
	
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	//702_430~1_2
	if(strBtnName=="CRCTVEACCENT_CNFRM")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ConfirmCorrectiveAction(pCustomButtonClicked);
	}
	//HSEMS New requirements_27_10_2020 5
	else if (strBtnName=="CRCTVEACCENT_CNFRMTN_RJC") {		
		if(OpenReasonsScr("Reject Reasons","3",true)){
			RejectCorrectiveAction(pCustomButtonClicked);
		}
	}
	//HSEMS New requirements_27_10_2020 5
	else if (strBtnName=="CRCTVEACCENT_CNFRMTN_CNCL") {
		if(OpenReasonsScr("Cancel Reasons","3",true)){
			CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
			if(KeyFieldValue != "") {
				CancelCorrectiveAction(pCustomButtonClicked);
			}
		}
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/************************************************************************************************************/
HRESULT CarConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	//HSEMS New requirements_27_10_2020  , 5 - Disable delete button
	EnableToolbar("",TOOLBAR_DELETE,FALSE);
	return CarCategory::DisplayScreenReady(pScreenReady);
}
/************************************************************************************************************/
				
/************************************************************************************************************
Helper function
************************************************************************************************************/
void CarConfirmationCategory::ConfirmCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strCarModelNum= FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELNUM");
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");
	//If User enter reject reason and press approve Message box with this message appear to user 
	//"Reject reason for this record exist,do you want to delete it ?"If user press ok Delete last reject reason
	bool RjctRsnExist=IsNewRjcRsnExist("CRCTVEACCENT-L1",strCarModelNum); 
	if(RjctRsnExist==1)
	{
		bool bDeleteRjcRsn=showApproveMessageBox("CRCTVEACCENT-L1",strCarModelNum);
		if(bDeleteRjcRsn==true) {
			strSQL.Format("EXECUTE confirmCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
	} else {
		strSQL.Format("EXECUTE confirmCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

// Reject CAR from Confirmation screen
// Change record status to Rejected-Review 2,Insert record into tracing tab
/*void CarConfirmationCategory::RejectCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,m_FSATableName,"");
	CString KeyFieldValue = GetKeyField(m_FSATableName,pCustomButtonClicked->pMultiSelectedRows);
	CString strNewStatus = "2";
	CString strTracingAction = "Rejected By Review";	
	strSQL.Format("EXECUTE rejectCARTXN '%s','%s','%s','%s','%s' ",KeyFieldValue,strUserName,strSourceScreenName,strNewStatus,strTracingAction);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}*/	

/*void CarConfirmationCategory::CancelCorrectiveActionConfirmation(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strFormTag = pCustomButtonClicked->Form_Tag;
	setOwnerScreenTag(strFormTag);	
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
	strSQL.Format("EXECUTE CancelCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}*/

long CarConfirmationCategory::ShowCorrectiveActionConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CRCTVEACCCNFRMTN;
	return ParentManage("HSE_TGCRCTVEACCCNFRMTN",TRUE,TRUE,TRUE,"HSE_CRCTVEACCENT",strCriteria);
}
