// AccidentFollowUpCategory.cpp: implementation of the AccidentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AccidentFollowUpCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AccidentFollowUpCategory::AccidentFollowUpCategory(CCategory *pCategory):AccidentCategory(pCategory)
{
	m_pCategory = pCategory;
}

AccidentFollowUpCategory::~AccidentFollowUpCategory()
{

}
/*************************************************************************************************************/
HRESULT AccidentFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//update : checkbox removed from policy screen
	//406_400~2_1
	//This screen is permitted only if (HSE policy, Accident Follow-up  checkbox [14@600])= checked
	//bool bIsAccdntFlwUpRqrd=CheckPolicy("HSEPLC_ACDNTFLW_UPRQRD");
	//if(bIsAccdntFlwUpRqrd==true)
	//{
	//	return AccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	//}
	//Else message box displayed to user with this message 
	//"Accident Follow-up checkbox must be checked to open this screen"
	//else
	//{
	//	ShowMessageEx(IDS_AFR,"",IV_NOICON,"Policy change required");
	//	return S_FALSE;
	//}		
	return AccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
}
/*************************************************************************************************************/
HRESULT AccidentFollowUpCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	//406_400~2_2
	CString strBtnName(pCustomButtonClicked->Button_Name);
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	if(strBtnName == "ACDNTENT_CLS") {	
		/*CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		bool bPendingRqrdActnsExist = false;
		if(KeyFieldValue != "") {
			bPendingRqrdActnsExist = hasPendingRequiredActions("HSE_ACDNTENTRECMNDACC",KeyFieldValue);
			if(!bPendingRqrdActnsExist) {
				CloseAccident(pCustomButtonClicked);
				updtAccidentTypeRotCusLib(KeyFieldValue);
			}
		}*/
		CloseAccident(pCustomButtonClicked);
	} else if(strBtnName == "ACDNTENTINJRDPRSN_INJRDBODYPART") {	
		OpenBodyParts();		
	}
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT AccidentFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString Tab_tag = pScreenReady->SubForm_Tag;
	EnableToolbar("",TOOLBAR_NEW,FALSE);		
	return AccidentCategory::DisplayScreenReady(pScreenReady);
}

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void AccidentFollowUpCategory::CloseAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE closeAccidentTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
	ExecuteSQL(strSQL);
	//DoToolBarAction(TOOLBAR_COPY,"HSE_TGACDNTFOLLWUPDT","");
	//DoToolBarAction(TOOLBAR_NEW,"HSE_TGACDNTFOLLWUPDT","");	
	//DoToolBarAction(TOOLBAR_PASTE,"HSE_TGACDNTFOLLWUPDT","");
	//DoToolBarAction(TOOLBAR_SAVE,"HSE_TGACDNTFOLLWUPDT","");
	RefreshScreen("",REFRESH_SELECTED);
}

long AccidentFollowUpCategory::ShowAccidentFollowup(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTFOLLWUPDT;
	return ParentManage("HSE_TGACDNTFOLLWUPDT",TRUE,TRUE,TRUE,"HSE_ACDNTENT",strCriteria);
}

//Not Used
void AccidentFollowUpCategory::updtAccidentTypeRotCusLib(CString strAccidentKey) {
	CString strSQL = "EXECUTE updateAccidentTypeRootCauseLib "+strAccidentKey;
	ExecuteSQL(strSQL);	
}







