// LossAccidentConfirmationCategory.cpp: implementation of the LossAccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "LossAccidentConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LossAccidentConfirmationCategory::LossAccidentConfirmationCategory(CCategory *pCategory):LossAccidentCategory(pCategory)
{
	m_pCategory = pCategory;
}
LossAccidentConfirmationCategory::~LossAccidentConfirmationCategory()
{

}
/************************************************************************************************************/
HRESULT LossAccidentConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//204_330~1_1
	//This screen is permitted only if the (HSE policy, Loss Accident Confirmation  checkbox [9@600]) = checked
	bool bIslossAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_ACTHZRDCNFRMTNRQRD");
	if(bIslossAccdntCnfrmtnRqrd==1)
	{
		return LossAccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{
		//Else message box displayed to user with this message 
		//" Loss Accident Confirmation  checkbox must be checked to open this screen "
		ShowMessageEx(IDS_LACR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}	
}
/************************************************************************************************************/
HRESULT LossAccidentConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//204_330~1_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strButtonName.MakeUpper();
		
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strButtonName=="LOSSACDNTENT_CNFRMD")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ConfirmLossAccident(pCustomButtonClicked);
	}

	return LossAccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/************************************************************************************************************/
HRESULT LossAccidentConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return LossAccidentCategory::DisplayScreenReady(pScreenReady);
}
/************************************************************************************************************/

/************************************************************************************************************
Helper function
************************************************************************************************************/
void LossAccidentConfirmationCategory::ConfirmLossAccident(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Get Key Field 
	CString strActHzrdNo= FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
	bool RjctRsnExist=IsNewRjcRsnExist("LOSSACDNTENT-L1",strActHzrdNo);
	if(RjctRsnExist==1)
	{
		//If User enter reject reason and press approve Message box with this message appear to user 
		//"Reject reason for this record exist,do you want to delete it ?"If user press ok "	Delete last reject reason
		bool bDeleteRjcRsn=showApproveMessageBox("HSE_RSKASSMNTENTREJRSN",strActHzrdNo);
		if(bDeleteRjcRsn==true)
		{
			//Update (Record Status [41@330]) of selected record into (5)
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
			//Insert record into (tracing [@30320~1]) tab
			InsertActionRecordIntoTracingTab("LOSSACDNTENT","ACTHZRDNO",CONFIRM);
		}
	}
	else
	{
		//Update (Record Status [41@330]) of selected record into (5)
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
		//Insert record into (tracing [@30320~1]) tab
		InsertActionRecordIntoTracingTab("LOSSACDNTENT","ACTHZRDNO",CONFIRM);	
	}
}

long LossAccidentConfirmationCategory::ShowLossAccidentConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOSSACDNTCNFRMTN;
	return ParentManage("HSE_TGLOSSACDNTCNFRMTN",TRUE,TRUE,TRUE,"HSE_LOSSACDNTENT",strCriteria);
}





