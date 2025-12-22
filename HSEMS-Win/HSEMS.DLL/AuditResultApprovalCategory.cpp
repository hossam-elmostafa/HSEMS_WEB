// AuditResultApprovalCategory.cpp: implementation of the AuditResultApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditResultApprovalCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditResultApprovalCategory::AuditResultApprovalCategory(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditResultApprovalCategory::~AuditResultApprovalCategory()
{

}


HRESULT AuditResultApprovalCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//604_460~3_1
	//Framework Event : Open screen
	//This screen is permitted only if the (HSE policy, Audit Result Approval checkbox [22@600]) = checked
	bool IsAdtRsltAprvlRqrd=CheckPolicy(Is_Audit_Result_Approval_Required);
	if(IsAdtRsltAprvlRqrd==true)
	{
		return AuditCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{
		//Else message box displayed to user with this message 
		//"{Audit Result Approval checkbox checkbox must be checked to open this screen"
		ShowMessageEx(IDS_ARAR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}

}


HRESULT AuditResultApprovalCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strBtnName.MakeUpper();
	//604_460~3_2
	//Framework Event : Press custom button
	//Condition : Button Pressed is (Approve)[4] 
	if(strBtnName=="ADTPLNENT_APPRV")
	{		
		//Update (Audit Result Status [12@460]) of selected record into (5)
		//UpdateStatusForAudit(pCustomButtonClicked->pMultiSelectedRows,5,"ADTPLNENT_ADTYR",strFormTag);
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"ADTPLNENT","MAINLINK");
		//Insert record into (tracing [@46020~3]) tab
		InsertActionRecordIntoTracingTab("ADTPLNENT","ADTYR",CONFIRM);	
	}
	//Condition : Button Pressed is (Reject) [5]
	else if(strBtnName=="ADTPLNENT_RJC")
	{
		//Check if there is new reject reason exist in reject reason tab if there
			//Make sure that a reject reason with serial 0 exist "new reject reason"
		bool RjctRsnExist=IsNewRjcRsnExist("HSE_ADTRSLTENTREJRES","ADTYR");
		if(RjctRsnExist==true)
		{
			//Update (Record Status [30@430]) of selected record into (3)
			/*deleted Code,Please Check Source Safe*/
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"ADTPLNENT","MAINLINK");
			//Insert record into (tracing [@43020~1]) tab
			InsertActionRecordIntoTracingTab("ADTPLNENT","ADTYR",REJECT);	
		}
		else
		{
			//Ask user to insert reject reason in the reject reason tab [@46010~3] before completing rejection process
			ShowMessageEx(IDS_RjctRsnRqrd,"",IV_NOICON,"Error");
			return S_FALSE;		
		}
	}
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}



HRESULT AuditResultApprovalCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}
	