// NearMissConfirmationCategory.cpp: implementation of the NearMissConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissConfirmationCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NearMissConfirmationCategory::NearMissConfirmationCategory(CCategory *pCategory):NearMissCategory(pCategory)
{
	m_pCategory = pCategory;
}

NearMissConfirmationCategory::~NearMissConfirmationCategory()
{
}
/*************************************************************************************************************/
//402_370~1_1
HRESULT NearMissConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//This screen is permitted only if the (HSE policy, Near Miss Confirmation checkbox [11@600]) = checked
	//On 6/12/23 I discovered that the field doesn`t exist anymore for no known reason
	bool bIsNrMssCnfrmtnRqrd=true;//CheckPolicy(Is_Near_Miss_Confirmation_Required);
	
	if(bIsNrMssCnfrmtnRqrd==true)
	{
		return NearMissCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		//Else message box displayed to user with this message 
		ShowMessageEx(IDS_NMCR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
	return S_OK;
}
/*************************************************************************************************************/
//402_370~1_2
HRESULT NearMissConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();	
	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	if(strButtonName=="NRSTMISCENT_ACP") {		
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			AcceptNearMiss(pCustomButtonClicked);
	}
	return NearMissCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT NearMissConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString Tab_tag = pScreenReady->SubForm_Tag;
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	return NearMissCategory::DisplayScreenReady(pScreenReady);
}

HRESULT NearMissConfirmationCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	CString strtag = m_strScreenTag;
	/*Commented_Code Deleted,Please check source safe earlier versions*/
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);

}
/*************************************************************************************************************/


/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void NearMissConfirmationCategory::AcceptNearMiss(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strNrstMiscNum=FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	bool RjctRsnExist=IsNewRjcRsnExist("NRSTMISCENT-L1",strNrstMiscNum); 
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE confirmNearMissTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	if(RjctRsnExist==1)
	{
		//If User enter reject reason and press approve Message box with this message appear to user 
		//"Reject reason for this record exist,do you want to delete it ?"If user press ok "	Delete last reject reason
		bool bDeleteRjcRsn=showApproveMessageBox("NRSTMISCENT-L1",strNrstMiscNum);
		if(bDeleteRjcRsn==true)
		{
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
	} else {		
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

long NearMissConfirmationCategory::ShowNearMissConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NRSTMISCCNFRMTN;
	//RQ-1-2024.9
	return ParentManage("HSE_TGNRSTMISCCNFRMTN",TRUE,TRUE,FALSE,"HSE_vwNRSTMISCENT",strCriteria);
}







