// PotentialHazardConfirmationCategory.cpp: implementation of the PotentialHazardConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PotentialHazardConfirmationCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PotentialHazardConfirmationCategory::PotentialHazardConfirmationCategory(CCategory *pCategory):PotentialHazardCategory(pCategory)
{
	m_pCategory = pCategory;
}

PotentialHazardConfirmationCategory::~PotentialHazardConfirmationCategory()
{

}

HRESULT PotentialHazardConfirmationCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//202_320~1_1	
	//This screen is permitted only if the (HSE policy, Potential Hazard Confirmation checkbox [8@600]) = checked
	#define	CHECK_FOR_POTENTIAL_HAZARD_CONFIRMATION_REQUIRED	"HSEPLC_PTNLHZRDCNFRMTNRQRD"
	bool bIsHzrdCnfrmnRqrd=CheckPolicy(CHECK_FOR_POTENTIAL_HAZARD_CONFIRMATION_REQUIRED);
	if(bIsHzrdCnfrmnRqrd==true)
	{
		return PotentialHazardCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	//Else message box displayed to user with this message "Potential Hazard Confirmation checkbox must be checked to open this screen "
	else
	{
		ShowMessageEx(IDS_PHCM,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}
HRESULT PotentialHazardConfirmationCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strButtonName.MakeUpper();
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strButtonName=="PTNLHZRDENT_CNF")
	{
		//202_303~1_2
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ConfirmPotential(pCustomButtonClicked);		
	}
	return PotentialHazardCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT PotentialHazardConfirmationCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return PotentialHazardCategory::DisplayScreenReady(pScreenReady);
}

/*************************************************************************************************
Helper function Rule - ID : 202_303~1_2
*************************************************************************************************/
void PotentialHazardConfirmationCategory::ConfirmPotential(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strPtnlHzrdNo=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
	//If User enter reject reason and press Confirm 
	bool RjctRsnExist=IsNewRjcRsnExist("PTNLHZRDENT-L1",strPtnlHzrdNo); 
	if(RjctRsnExist==1)
	{
		//Message box with this message appear to user "Reject reason for this record exist,do you want to delete it ?"If user press ok 
		bool bDeleteRjcRsn=showApproveMessageBox("PTNLHZRDENT-L1",strPtnlHzrdNo);
		if(bDeleteRjcRsn==true)
		{
			//Update (Record Status [41@320]) of selected record into (5)	
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
			//Insert record into (tracing [@32020~1]) tab
			InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",CONFIRM);
			RefreshScreen("",0);
		}
	}
	else
	{
		//Update (Record Status [41@320]) of selected record into (5)	
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
		//Insert record into (tracing [@32020~1]) tab
		InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",CONFIRM);
	}
}



long PotentialHazardConfirmationCategory::ShowPotentialHazardConfirmation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTNLHZRDCNFRMTN;
	return ParentManage("HSE_TGPTNLHZRDCNFRMTN",TRUE,TRUE,TRUE,"HSE_PTNLHZRDENT",strCriteria);
}
