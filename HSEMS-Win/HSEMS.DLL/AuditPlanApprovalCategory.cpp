// AuditPlanApprovalCategory.cpp: implementation of the AuditPlanApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditPlanApprovalCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanApprovalCategory::AuditPlanApprovalCategory(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanApprovalCategory::~AuditPlanApprovalCategory()
{

}


HRESULT AuditPlanApprovalCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//602_460~1_1
	//This screen is permitted only if the (HSE policy, Audit Plan Approval) checkbox [22@600] = checked
	bool bIsAdtPlanAprvlRqrd=CheckPolicy(Is_Audit_Plan_Approval_Required);
	if(bIsAdtPlanAprvlRqrd==true)
	{
		return AuditCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	else
	{
		//Else message box displayed to user with this message 
		//"Audit Plan Approval checkbox must be checked to open this screen"
		ShowMessageEx(IDS_APAR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
	return S_OK;
}

HRESULT AuditPlanApprovalCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Changed by Ahmed Ali 21-6-2010 Why to change member variable m_strScreenTag ?
	//m_strScreenTag = pCustomButtonClicked->Form_Tag;
	CString strBtnName(pCustomButtonClicked->Button_Name);	
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strBtnName.MakeUpper();		
	CString strAdtyr=FormGetField("HSE_ADTPLNENT","MAINLINK");
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strBtnName=="ADTPLNENT_APPRV")
	{
		//602_460~1_2
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
		{
			ApproveAudit(strAdtyr,pCustomButtonClicked);
			//DoToolBarAction(TOOLBAR_UNFILTER,strFormTag,"");
			//RefreshScreen("",REFRESH_SELECTED);
		}
	}
	else if(strBtnName=="ADTPLNENTACTVT_ADTCHCKLST")
	{
		openAuditChecklistResult();
	}
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AuditPlanApprovalCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}
	
/*
	------------------------------
	helper function
	------------------------------
*/
void AuditPlanApprovalCategory::openAuditChecklistResult()
{
	CString strSQL;
	CString strDefValues;
	CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");		
	strDefValues.Format("ADTCHCKLSTPLN_LNK|%s",strKeyField);	
		//Bugs-2-2021.61
		//"•        In (audits) TAB > (actual checklist) popup > (audit checklist result) popup > (required actions) popup :
		//o        The (serial No.) of (recommended actions) is not correct, please check (Bug"
	strSQL.Format("SELECT * FROM HSE_ADTCHCKLSTPLN WHERE (ADTCHCKLSTPLN_LNK = '%s') Order by ADTCHCKLSTPLN_TSKNUM",strKeyField);
	m_strScreenTag = "HSE_TGADTCHCKLSTRSLT";	
	ShowScreen(0,"HSE_TGADTCHCKLSTRSLT","Audit Check List",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
}

/*
	------------------------------
	helper function
	------------------------------	
*/
void AuditPlanApprovalCategory::ApproveAudit(CString strAdtyr,tag_CustomButtonClicked* pCustomButtonClicked)
{
		setOwnerScreenTag(m_strScreenTag);
		//need to be reviewed.	
		bool RjctRsnExist=IsNewRjcRsnExist("ADTPLNENT-L1",strAdtyr); 
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		CString strSQL;
		CString strUserName = GetUserLogin();
		CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");
		if(RjctRsnExist==1)
		{
			bool bDeleteRjcRsn=showApproveMessageBox("ADTPLNENT-L1",strAdtyr);
			if(bDeleteRjcRsn==true)
			{
				/*//"	Update (Audit Plan Status [2@460]) of selected record into (5)
				UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"ADTPLNENT","MAINLINK");
				//Insert record into (tracing [@46020~1]) tab
				InsertActionRecordIntoTracingTab("ADTPLNENT","MAINLINK",APPROVE);*/
				strSQL.Format("EXECUTE confirmAuditTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
				ExecuteSQL(strSQL);
				RefreshScreen("",REFRESH_SELECTED);
			}
		} else {
			/*//Update (Audit Plan Status [2@460]) of selected record into (5)
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"ADTPLNENT","MAINLINK");
			//Insert record into (tracing [@46020~1]) tab
			InsertActionRecordIntoTracingTab("ADTPLNENT","MAINLINK",APPROVE);*/
			strSQL.Format("EXECUTE confirmAuditTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
}

long AuditPlanApprovalCategory::ShowAuditPlanApproval(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNAPR;
	return ParentManage("HSE_TGADTPLNAPR",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}




