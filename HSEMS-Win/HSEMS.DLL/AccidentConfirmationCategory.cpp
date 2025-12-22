// AccidentConfirmationCategory.cpp: implementation of the AccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AccidentConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AccidentConfirmationCategory::AccidentConfirmationCategory(CCategory *pCategory):AccidentCategory(pCategory)
{
	m_pCategory = pCategory;
}

AccidentConfirmationCategory::~AccidentConfirmationCategory()
{
}
/************************************************************************************************************/
HRESULT AccidentConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//405_400~1_1
	//This screen is permitted only if the (HSE policy, Accident Confirmation checkbox [13@600]) = checked
	bool bIsAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_ACDNTCNFRMTNRQRD");
	if(bIsAccdntCnfrmtnRqrd==true) {
		return AccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		//Else message box displayed to user with this message 
		//"Accident Confirmation  checkbox must be checked to open this screen"
		ShowMessageEx(IDS_ACR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/************************************************************************************************************/
HRESULT AccidentConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	//405_400~1_2	
	if(strButtonName=="ACDNTENT_CNFRM") {	
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "") {
			ConfirmAccident(pCustomButtonClicked);
			RefreshScreen("",REFRESH_SELECTED);
		}
	} else if(strButtonName=="ACDNTENTINJRDPRSN_INJRDBODYPART") {	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-10-2021.58
		if (pCustomButtonClicked->AbsolutePosition!=-1)//There is no record i.e the tab is empty
	///////////////////////////////////////////////////////////////////////////////////////////////////
			OpenBodyParts();		
	}
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/************************************************************************************************************/
HRESULT AccidentConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString Tab_tag = pScreenReady->SubForm_Tag;
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	return AccidentCategory::DisplayScreenReady(pScreenReady);
}

/************************************************************************************************************
Helper function
************************************************************************************************************/
void AccidentConfirmationCategory::ConfirmAccident(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strAcdntNum=FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
	bool RjctRsnExist=IsNewRjcRsnExist("ACDNTENT-L1",strAcdntNum); 
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE confirmAccidentTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	if(RjctRsnExist==1) {
		bool bDeleteRjcRsn=showApproveMessageBox("ACDNTENT-L1",strAcdntNum);
		if(bDeleteRjcRsn==true) {
			ExecuteSQL(strSQL);
			//RefreshScreen("",REFRESH_SELECTED);
		}
	} else {
			ExecuteSQL(strSQL);
			//RefreshScreen("",REFRESH_SELECTED);
	}
}

long AccidentConfirmationCategory::ShowAccidentConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTCNFRMTN;
	return ParentManage("HSE_TGACDNTCNFRMTN",TRUE,TRUE,TRUE,"HSE_ACDNTENT",strCriteria);
}

