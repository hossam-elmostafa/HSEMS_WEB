// PtwConfirmationCategory.cpp: implementation of the PtwConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PtwConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PtwConfirmationCategory::PtwConfirmationCategory(CCategory *pCategory):PtwCategory(pCategory)
{
	m_pCategory = pCategory;
}

PtwConfirmationCategory::~PtwConfirmationCategory(){}
/*************************************************************************************************************/
HRESULT PtwConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//502_440~1_1
	//This screen is permitted only if the (HSE policy, PTW Confirmation checkbox [60]) = checked
	bool bIsPTWCnfrmtnRqrd=CheckPolicy("HSEPLC_PTWSRGSTRCNFRMTNRQRD");
	if(bIsPTWCnfrmtnRqrd==true)
	{
		return PtwCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		//Else message box displayed to user with this message 
		//"PTW Confirmation checkbox checkbox must be checked to open this screen"
		ShowMessageEx(IDS_PTWCR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
/*************************************************************************************************************/
HRESULT PtwConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strBtnName.MakeUpper();	
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	//502_440~1_2
	if(strBtnName=="PTWSRGSTRENT_CNFRM")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ConfirmPTW(pCustomButtonClicked);
	}
	return PtwCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT PtwConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return PtwCategory::DisplayScreenReady(pScreenReady);
}
/*************************************************************************************************************
Helper Function
*************************************************************************************************************/
void PtwConfirmationCategory::ConfirmPTW(tag_CustomButtonClicked* pCustomButtonClicked)
{	
	CString strSQL;
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");
	setOwnerScreenTag(m_strScreenTag);
	CString strPTWNum=FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSNUM");
	bool RjctRsnExist=IsNewRjcRsnExist("PTWSRGSTRENT-L1",strPTWNum); 
	if(RjctRsnExist==1)
	{
		bool bDeleteRjcRsn=showApproveMessageBox("PTWSRGSTRENT-L1",strPTWNum);
		if(bDeleteRjcRsn==true)
		{
			/*deleted Code ,Please Check Source Safe*/			
			strSQL.Format("EXECUTE confirmPTWTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}			
	} else {
		/*deleted Code ,Please Check Source Safe*/			
		strSQL.Format("EXECUTE confirmPTWTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

long PtwConfirmationCategory::ShowPTWRegisterConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTWSRGSTRCNFRMTN;
	return ParentManage("HSE_TGPTWSRGSTRCNFRMTN",TRUE,TRUE,TRUE,"HSE_PTWSRGSTRENT",strCriteria);
}