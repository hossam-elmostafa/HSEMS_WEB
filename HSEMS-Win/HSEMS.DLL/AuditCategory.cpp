// AuditCategory.cpp: implementation of the AuditCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditCategory.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditCategory::AuditCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditCategory::~AuditCategory()
{

}

/*
Lock Screen "Audit plan Entry" According to status
HSE_CR09_1 : Screen "Audit plan Entry" changed to Audit Plan Preparation
and all tabs
*/
void AuditCategory::LockOnStatus()
{
	CString strAdtPlnStats=FormGetField(Audit_Plan_Entry_Table,"ADTPLNENT_ADTPLNSTS");
	int nAdtPlnStats=atoi(strAdtPlnStats);
	if(nAdtPlnStats>3)
	{		
		LockActivityTab();
	}
	else
	{
		UnLockActivityTab();
	}
}

HRESULT AuditCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	//polydb_task , nRecordStatus always sent 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{						
		//000_2
		//Action : Add new record
		//Condition : If new reject reason for that record exist 
		//Response : Message box inform user that reject reason already exist and cancel add operation 
		//Else Add new record
		if(strSubFormTag=="HSE_TGADTPLNFOLLWUPRJCRSN"||strSubFormTag=="HSE_TGADTPLNAPRRJCRSN"||strSubFormTag=="HSE_TGADTRSLTENTREJRES")																									
		SetRjctRsnFlag(pToolBarButtonClicked,"ADTPLNENT_ADTYR");								
	} else if (pToolBarButtonClicked->iComplete==1 && strFormTag == "HSE_TGADTPLNENT" && strSubFormTag == "" && strBtnName=="NEW") {
		CString strSQL="SELECT TOP 1 CompPrf_Cod FROM HSE_COMPPRF";
		CString strCompNam=GetRecordSetData(strSQL,1);
		FormSetField("HSE_ADTPLNENT","ADTPLNENT_CMPNYNAM",strCompNam);
	//TODO : Check this,When I add Planned Audit year and enable save button only,after save I found that new button
	//is enabled,Note that save button is disabled,I write below code to handle this problem
	} else if(strSubFormTag=="HSE_TGADTPLNADTRSADTSACTV" && strBtnName == "NEW" && pToolBarButtonClicked->iComplete==0) {
		*pToolBarButtonClicked->iCancle = 1;
		return S_FALSE;
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


//Enable Confirmation comments in confirmation screens 
HRESULT AuditCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);	
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGADTPLNAPR"||strFormTag=="HSE_TGADTRSLTAPR")
		{
			SetFieldVisibleAttrib("HSE_TGADTRSLTAPR","ADTPLNENT_APRCMNT","NULL");		
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT AuditCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 602_460~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="ADTPLNENT_RJC" && (strForm_Tag=="HSE_TGADTPLNAPR" || strForm_Tag=="HSE_TGADTPLNFOLLWUP"))
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			//UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"ADTPLNENT","MAINLINK");		
			//RefreshScreen("",REFRESH_SELECTED);
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectAudit '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);
		}
	} else if(strButton_Name == "ADTCHKLST_VWADTCHCKLST") {
		///////////////////////////////////////////////////////////
		//Bugs-10-2021.58
		if(pCustomButtonClicked->AbsolutePosition!=-1)
		///////////////////////////////////////////////////////////
		{

			CString strSQL;
			CString strAuditeeGrp = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT");
			CString strAuditee	  = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM");

			CString strAuditKey = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");	
			
			//strSQL.Format("SELECT * FROM HSE_AdtChcklst WHERE AdtChcklst_LNKWTHDTL = (SELECT MAINLINK FROM HSE_AdtGrpAdte WHERE AdtGrpAdte_COD = '%s' AND DETAILLINK = '%s')",strAuditee,strAuditeeGrp);		
			
			//Bugs-2-2021.61
			//"•        In (audits) TAB > (actual checklist) popup > (audit checklist result) popup > (required actions) popup :
			//o        The (serial No.) of (recommended actions) is not correct, please check (Bug"
			strSQL.Format("SELECT * FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_LNK = '%s' Order by ADTCHCKLSTPLN_TSKNUM",strAuditKey);		
			
			//ShowScreen(0,"HSE_TGADTCHCKLST","Audit Checklist",NORMAL_MODE,true,strSQL,"","","",true);
			ShowScreen(0,"HSE_TGADTCHCKLSTPLN","Audit Checklist",NORMAL_MODE,true,strSQL,"","","",true);
		}


	} else if(strButton_Name == "AUDITORS" && strForm_Tag != "HSE_TGADTPLNADTRSANDADTS" && strForm_Tag != "HSE_TGADTPLNCHKLSTFDBCK"){
		///////////////////////////////////////////////////////////
		//Bugs-10-2021.58
		if(pCustomButtonClicked->AbsolutePosition!=-1)
		///////////////////////////////////////////////////////////
			OpenAuditorsScreen(strForm_Tag,1);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AuditCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{		
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	//Filter Auditee on Auditee Group,Note : ADTPLNENTACTVT_REFNUM is Auditee field
	//update by a.ali , 12-8-2020 Below code changes as ADTPLNENTACTVT_REFNUM browse from another table HSE_HSEPRCDRLISTADTS
	/*if (strFieldName == "ADTPLNENTACTVT_REFNUM") {
		CString strAdtGrp = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT");
		CString strSQL;
		strSQL.Format("SELECT * FROM HSE_ADTGRPADTE WHERE (DETAILLINK ='%s')",strAdtGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL = strSQL.AllocSysString();
	}*/
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT AuditCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strSubFormTag = pFieldChange->SubForm_Tag;
	CString strFldName    = pFieldChange->Field_Name;	
	if(strFldName == "ADTPLNENTACTVT_ADITEDPRT") {		
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_RefNum","");	//Empty Auditee field if Auditee group changed
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT AuditCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;		
	if(strBrowsingField == "ADTPLNENTACTVT_ADITEDPRT") {				
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_RefNum","");		
	} 
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}


void AuditCategory::LockActivityTab()
{
	EnableToolbar("HSE_TGADTPLNENT","",TOOLBAR_DELETE,false);
	//polydb_task
	//LockTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",true);
	EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);	
}

void AuditCategory::UnLockActivityTab()
{
	EnableToolbar("HSE_TGADTPLNENT","",TOOLBAR_DELETE,true);
	//polydb_task
	//LockTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);
	//EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);
	EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",true);
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ACTVNUM","ALWAYSLOCKED UNIQUE");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_VRSNNUM","ALWAYSLOCKED");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM","ALWAYSLOCKED");		//commented by a.ali,11-8-2020,field deleted
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT","MUST");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM","MUST");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM","NULL");				//commented by a.ali,11-8-2020,field deleted
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_CMNT","NULL");
	//this field is always hidden
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ACTVTSTS","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ADTCNCLSN","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PLND","ALWAYSLOCKED");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_CNTCTPRSN","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_SITE","MUST");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_WRKLOC","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_LOCTYPE","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_AREA","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_TODATE","MUST");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_FRMDATE","MUST");
	
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_FRMTM","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_TOTM","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PLNDFLWUPDATE","NULL");
}

void AuditCategory::rejectRecord(CString strForm_Tag)
{
	setSrcKyFldNm("MAINLINK");
	setOwnerScreenTag(m_strScreenTag);
	CString strMAINLINK=FormGetField("HSE_ADTPLNENT","MAINLINK");
	if(strForm_Tag=="HSE_TGADTPLNAPR")
	{
		Reject("ADTPLNENT-L1",strMAINLINK);
	}
	else if(strForm_Tag=="HSE_TGADTPLNFOLLWUP")
	{
		Reject("ADTPLNENT-L2",strMAINLINK);
	}
}

/**
 *	Used by
 *  Audit Plan Resource -> Audits tab "HSE_TGADTPLNADTRSANDADTS.HSE_TGADTPLNADTRSADTSACTV"
 *  Audit Check list	-> Audits tab "HSE_TGADTPLNPRPRCHKLST.HSE_TGADTPLNPRPCHKLSTACTV"
 */
void AuditCategory::OpenAuditCheckListScreen(CString ScreenTag)
{
	CString strMainKeyFeild = FormGetField("HSE_ADTPLNENT" ,"MAINLINK");
	int nNumberOfRecords    = getNumberOfRecord("HSE_ADTPLNENTACTVT" , strMainKeyFeild,"ADTPLNENTACTVT_LNK");
	CString strSQL ;
	//Audit Checklist without any record in the Activities screen shouldn't be opened
	if(nNumberOfRecords != 0)
	{
		//Response : "	Display the (Audit Checklist [@481]) pop-up screen"	Allow user to edit all records			
		CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");	
		CString strDefValues;				
		bool bLocked = false;		
		//Bugs-2-2021.61
		//"•        In (audits) TAB > (actual checklist) popup > (audit checklist result) popup > (required actions) popup :
		//o        The (serial No.) of (recommended actions) is not correct, please check (Bug"
		strSQL.Format("SELECT * FROM HSE_ADTCHCKLSTPLN WHERE (ADTCHCKLSTPLN_LNK = %s) Order by ADTCHCKLSTPLN_TSKNUM",strKeyField);
		if(ScreenTag == "HSE_TGADTPLNINQ") {
			bLocked = true;
		}
		strDefValues.Format("HMNBODYPART_LNK|%s",strKeyField);
		ShowScreen(0,"HSE_TGADTCHCKLSTPLN","Check List Entry",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);		
	}
}


/**
 *
 *
 */
BOOL AuditCategory::openAuditChecklistResult(CString ScreenTag)
{
	CString strSQL;
	CString strMsg;
	CString strDefValues;
	CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");
	strDefValues.Format("ADTCHCKLSTPLN_LNK|%s",strKeyField);	
		//Bugs-2-2021.61
		//"•        In (audits) TAB > (actual checklist) popup > (audit checklist result) popup > (required actions) popup :
		//o        The (serial No.) of (recommended actions) is not correct, please check (Bug"
	strSQL.Format("SELECT * FROM HSE_AdtchckLstPln WHERE (ADTCHCKLSTPLN_LNK = %s) Order by ADTCHCKLSTPLN_TSKNUM",strKeyField);
	m_strScreenTag = "HSE_TGADTCHCKLSTRSLT";
	if(ScreenTag == "HSE_TGADTPLNINQ")
	{		
		//Bugs-3-2021.67
		//Show the screen modeless
		ShowScreen(0,"HSE_TGADTCHCKLSTRSLT","Check List Feedback",NORMAL_MODE,false,strSQL,"",strDefValues,"",true);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		return true;
	} else {		
		//Bugs-3-2021.67
		//Show the screen modeless
		ShowScreen(0,"HSE_TGADTCHCKLSTRSLT","Check List Feedback",NORMAL_MODE,false,strSQL,"",strDefValues,"",false);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		return true;
		strSQL.Format("SELECT ADTCHCKLSTPLN_TSKNUM FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_LNK  = '%s'",strKeyField);						
		CString strTaskNum = GetRecordSetData(strSQL.AllocSysString(),-1);		
		/*CString Task = strtok(strTaskNum.GetBuffer(0),"\2");		
		int i = 0;
		while (Task != "")
		{
			//strSQL.Format("SELECT ISNULL(ADTCHCKLSTPLN_FNDNG,''),ADTCHCKLSTPLN_TSKSTS FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_TSKNUM = '%s' and ADTCHCKLSTPLN_LNK  = '%s'",Task ,strKeyField );
			strSQL.Format("SELECT ISNULL(ADTCHCKLSTPLN_FNDNG,'')  FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_TSKNUM = '%s' and ADTCHCKLSTPLN_LNK  = '%s'",Task ,strKeyField );
			CString Findings =GetRecordSetData(strSQL.AllocSysString(),-1);
			strSQL.Format("SELECT ISNULL(ADTCHCKLSTPLN_TSKSTS,'') FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_TSKNUM = '%s' and ADTCHCKLSTPLN_LNK  = '%s'",Task ,strKeyField );
			CString strCheckListsts = GetRecordSetData(strSQL.AllocSysString(),-1);
			if(Findings=="" && strCheckListsts != "3")
			{
				strMsg.Format("You didn't Enter feedback in task number [%s]  please fill it to can update actual From and To date ",Task);
				AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
				return false;
			}			
			Task = strtok(NULL,"\2");
		}*/
		
		CString Task=_T("");
		int i = 0; // substring index to extract
		while (AfxExtractSubString(Task, strTaskNum, i,'\2'))
		{   
		    strSQL.Format("SELECT ISNULL(ADTCHCKLSTPLN_FNDNG,'')  FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_TSKNUM = '%s' and ADTCHCKLSTPLN_LNK  = '%s'",Task ,strKeyField );
			CString Findings =GetRecordSetData(strSQL.AllocSysString(),-1);
			strSQL.Format("SELECT ISNULL(ADTCHCKLSTPLN_TSKSTS,'') FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_TSKNUM = '%s' and ADTCHCKLSTPLN_LNK  = '%s'",Task ,strKeyField );
			CString strCheckListsts = GetRecordSetData(strSQL.AllocSysString(),-1);
			if(Findings=="" && strCheckListsts != "3")
			{
				strMsg.Format("You didn't Enter feedback in task number [%s]  please fill it to can update actual From and To date ",Task);
				AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
				return false;
			}
		    i++;
		}
		m_strScreenTag = "HSE_TGADTPLNCHKLSTFDBCK";
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLFRMDATES","MUST");
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLTODATE","MUST");
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CARNUM","3");
		return true;
	}
}

void AuditCategory::OpenCncrndDept(CString ScreenTag)
{
	CString strMainKeyFeild = FormGetField("HSE_ADTPLNENT" ,"MAINLINK");
	int nNumberOfRecords = getNumberOfRecord("HSE_ADTPLNENTACTVT" , strMainKeyFeild,"ADTPLNENTACTVT_LNK");
	CString strSQL ;

	//Audit Checklist without any record in the Activities screen shouldn't be opened
	if(nNumberOfRecords != 0)
	{
		bool bLocked = false;
		CString strDefValues;
		//Response : "	Display the (Concerned department pop-up screen"Allow user to edit all records			
		CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");				
		strDefValues.Format("CNCRNDPT_LNK|%s",strKeyField);
		strSQL.Format("SELECT * FROM HSE_CNCRNDPT WHERE (CNCRNDPT_LNK = %s)",strKeyField);
		if(ScreenTag == "HSE_TGADTPLNINQ") {
			bLocked = true;
		}
		ShowScreen(0,"HSE_TGCNCRNDPT","Concrned department",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
	}
}

/**
 * Open Auditors screen (Planned , Actual) 
 * @param CString ScreenTag 
 * @param int ScreenMode 1 -> planned , 2-> Actual
 * 
 * 1 -> is the initial screen (not containing all fields)
 * 2 -> full fields screen after audit
 */
void AuditCategory::OpenAuditorsScreen(CString ScreenTag,int ScreenMode)
{
	CString strSQL;
	CString strDefValues;
	//According to CR03
	CString strMainLinkFeild = FormGetField("HSE_ADTPLNENTACTVT" ,"MAINLINK");
	CString strDep = FormGetField("HSE_ADTPLNENTACTVT" ,"ADTPLNENTACTVT_ADITEDPRT");
	////////////////////////////////////////////////////////////////////////////////////////
	//RQ-2-2021(Batch2).19
	/*
		Audit Plan Notification	Select the auditors, qualified for the “audit standard”	"•	In (audits) TAB > (auditors) popup screen :
		o	Filter the (employee No.) by (audits\Audit standard)
		o	Add (Leading The Audit) combo, with fixed values (Yes/No).  default = Yes.  Only (1) auditor in the same “audit” should lead the audit.
	*/
	CString strAuditStandard = FormGetField("HSE_ADTPLNENTACTVT" ,"ADTPLNENTACTVT_QLTYSTNDRD");
	//m_pCategory->GetHSEObj()->setstrDep(strDep);
	m_pCategory->GetHSEObj()->setstrDep(strAuditStandard);
	////////////////////////////////////////////////////////////////////////////////////////

	strDefValues.Format("LINK|%s",strMainLinkFeild);						    
	strSQL.Format("SELECT * FROM HSE_AdtPlnEntAuditors WHERE (LINK= %s)",strMainLinkFeild);	
	if(ScreenTag == "HSE_TGADTPLNINQ") {	
		ShowScreen(0,"HSE_TGACTLADTPLNENTAUDITORS","Auditors",NORMAL_MODE,true,strSQL,"",strDefValues,"",true);
	} else {
		if(ScreenMode == 1) {
			ShowScreen(0,"HSE_TGADTPLNENTAUDITORS","Auditors",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
		} else {		
			ShowScreen(0,"HSE_TGACTLADTPLNENTAUDITORS","Auditors",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
		}
	}
}

/***************************************************************************
helper function ->Rule ID :
	--Open Auditees Screen
***************************************************************************/
void AuditCategory::OpenAuditeesScreen(CString ScreenTag,int ScreenMode)
{
	//According to CR03
	CString strSQL ;
	CString strDefValues;
	CString strMainLinkFeild = FormGetField("HSE_ADTPLNENTACTVT" ,"MAINLINK");	
	strDefValues.Format("LINK|%s",strMainLinkFeild);
	strSQL.Format("SELECT * FROM HSE_AdtPlnEntAuditees WHERE (LINK= %s)",strMainLinkFeild);	
	if(ScreenTag == "HSE_TGADTPLNINQ")		
		ShowScreen(0,"HSE_TGADTPLNENTAUDITEES","Auditees",NORMAL_MODE,true,strSQL,"",strDefValues,"",true);
	else{
		if(ScreenMode == 1)			
			ShowScreen(0,"HSE_TGADTPLNENTAUDITEES","Auditees",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
		else			
			ShowScreen(0,"HSE_TGACTLADTPLNENTAUDITEES","Auditees",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
	}
}

CString AuditCategory::getRecordStatus()
{
	//CString strSQL;
	//CString strKeyField="";
	//strKeyField=FormGetField("HSE_ADTPLNENT","MAINLINK");	
	//strSQL="SELECT ADTPLNENT_ADTPLNSTS FROM HSE_ADTPLNENT WHERE MAINLINK = '"+strKeyField+"'" ;	
	//CString EntryStatus = GetRecordSetData(strSQL,1);
	CString EntryStatus = FormGetField("HSE_ADTPLNENT","ADTPLNENT_ADTPLNSTS");	
	return EntryStatus;
}
