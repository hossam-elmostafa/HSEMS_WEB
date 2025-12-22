// AuditPlanEntryCategory.cpp: implementation of the AuditPlanEntryCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "AuditPlanEntryCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AuditPlanEntryCategory::AuditPlanEntryCategory(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanEntryCategory::~AuditPlanEntryCategory()
{
}
/**************************************************************************************************************************************************/
HRESULT AuditPlanEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strForm_Tag(pRecordRepostion->Form_Tag);
	CString strSubForm_Tag(pRecordRepostion->SubForm_Tag);
	if((*pRecordRepostion).iComplete == 1) {
		//if screen is main screen.
		if(strForm_Tag=="HSE_TGADTPLNENT" && strSubForm_Tag=="") {
			LockOnStatus();
		}
		if(strSubForm_Tag == "HSE_TGADTPLNENTACTVT") {
			CString RecordStatus = getRecordStatus();
			if(RecordStatus == "5") {//partial assigned					
				/*Commented_Code Deleted,Please check source safe earlier versions*/
				EnableToolbar("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",TOOLBAR_NEW,false);
			}
			else // Added By Hossam 14/1/2021 because if the tool bar is disabled because of RecordStatus == "5" 'partial assigned' it will never be enabled again 
			{
				EnableToolbar("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",TOOLBAR_NEW,true);
			}
		}		
	}
	return AuditCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT AuditPlanEntryCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	CString strtag = m_strScreenTag;
	if(strSubFormTag == "HSE_TGADTPLNENTACTVT") {
		LockOnStatus();
	}
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);
}
/**************************************************************************************************************************************************/
HRESULT AuditPlanEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strSQL;
	//why?
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	//Get record status
	CString EntryStatus=getRecordStatus();
	int Count = 0;
	if(strSubFormTag == "") {
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}
	}
	if(strBtnName=="ADTPLNENT_ENTCMPLT" && (EntryStatus == "1" || EntryStatus == "3")) {
		//601_460_2
		//The (activities [@461]) tab must include at least 1 record
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}
		bool bAuditComplete = CompleteAudit(pCustomButtonClicked);
		if(bAuditComplete)
			RefreshScreen("",REFRESH_SELECTED);
	} else if (strBtnName=="ADTPLNENTACTVT_ADNLUNPLANDACTVT") {
		//Rule-ID : 601_461_2
		//Update by a.ali add unplanned only to approved audits			
		if(EntryStatus == "4")
			AddUnplanedActivity();
		else
			AfxMessageBox("You can add unplanned activiy to approved audits only");
	} else if(strBtnName=="ADTPLNENTACTVT_CONCRNDPT") {
		if(EntryStatus == "1")
			OpenCncrndDept("HSE_TGADTPLNENT");
	}
	//601_461_3
	//Action : Press (Add New Version [10]) button
	//Update by a.ali add new version only to approved audits
	else if(strBtnName=="ADTPLNENTACTVT_ADNLNSCRVRSN") {		
		if(EntryStatus == "4") {			
			EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);
	 		DuplicateActvtTab();
			EnableToolbar("HSE_TGADTPLNENT","",TOOLBAR_DELETE,true);			
			EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);						
			RturnActvtyTabToDfultBhv();
		}
		else
			AfxMessageBox("You can add new activity version to activities under approved audits only");
	} 
	/*Commented_Code Deleted,Please check source safe earlier versions*/
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/**************************************************************************************************************************************************/
HRESULT AuditPlanEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,true);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}
/**************************************************************************************************************************************************/
HRESULT AuditPlanEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{		
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	// Id : CR03-16
	//Done by : Doaa Desouky
	//Date : 10/06/2010
	// Requirement : Open screen work locations [@020] with all record filterd on selected site 
	if(strFieldName=="ADTPLNENTACTVT_LOCTYPE") {
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL = OpenWorkLocationFilteredOnSite().AllocSysString();
	}
	
	/*Commented_Code Deleted,Please check source safe earlier versions*/

	//Filter Site in Audits tab according to selected company
	else if (strFieldName == "ADTPLNENTACTVT_SITE") {
		CString strCompNam = FormGetField("HSE_ADTPLNENT","ADTPLNENT_CMPNYNAM");
		CString strSQL;
		strSQL.Format("WHERE (STE_CMPNYNAME ='%s')",strCompNam);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL = strSQL.AllocSysString();
	}
	return AuditCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
/**************************************************************************************************************************************************/

/************************************************************************
helper function
	--Filter work location aaccording to selected site
*************************************************************************/
CString AuditPlanEntryCategory::OpenWorkLocationFilteredOnSite()
{
	CString strSQL;
	CString strSite=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_SITE");								
	if(strSite.IsEmpty())			
		strSite =" ";
	strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSite);
	return strSQL;
}
/************************************************************************
helper function
	--Filter work location tab on location type
*************************************************************************/
CString AuditPlanEntryCategory::OpenWotkLocationsTabFilteredOnLocationType()
{
	CString strSQL;
	CString strLocTyp=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_LOCTYPE");
	if(strLocTyp.IsEmpty())		
		strLocTyp=" ";
	strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
	return strSQL;
}
/************************************************************************
helper function
	--Complete Audit record
*************************************************************************/
bool AuditPlanEntryCategory::CompleteAudit(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	bool bAuditComplete = false;
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE completeAuditTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	long lRetVal = ExecuteSQL(strSQL);
	if(lRetVal != 0)
		bAuditComplete = true;	
	return bAuditComplete;
}


/***************************************************************************
helper function 
	--Set Activity tab controls to the behaviour in the application manager
***************************************************************************/
void AuditPlanEntryCategory::RturnActvtyTabToDfultBhv()
{
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ACTVNUM","ALWAYSLOCKED UNIQUE");	
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM","ALWAYSLOCKED");		//commented by a.ali,11-8-2020,field deleted
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT","MUST");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM","NULL");				//commented by a.ali,11-8-2020,field deleted
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_CMNT","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ACTVTSTS","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_ADTCNCLSN","NULL");		
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_SITE","MUST");	
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_LOCTYPE","NULL");
	//SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_AREA","NULL");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_TODATE","MUST");
	SetFieldVisibleAttrib("HSE_TGADTPLNENTACTVT","ADTPLNENTACTVT_FRMDATE","MUST");
}
/***************************************************************************
helper function ->Rule-ID : 601_461_2
	--Add Unplaned Activity
***************************************************************************/
void AuditPlanEntryCategory::AddUnplanedActivity()
{
	//Action   : Press (Add Un-planned Activity [9]) button
	//Response : "	Allow user to feed new record after last row in activitis tab"	
	EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);
	DoToolBarAction(TOOLBAR_NEW,"HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT");
	EnableToolbar("HSE_TGADTPLNENT","",TOOLBAR_DELETE,true);;
	EnableTab("HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT",false);		
	RturnActvtyTabToDfultBhv();
}
/************************************************************************
	Hepler function This function duplicate record in activity tab 
	increment version number [2] by 1
*************************************************************************/
void AuditPlanEntryCategory::DuplicateActvtTab()
{
		//"	Duplicate the current record, with incremented (version number [2]) value
		//Allow user to edit the newly inserted record
		//The (planned [20] checkbox should be "un-checked" for all inserted records
		//DoToolBarAction(TOOLBAR_NEW,"HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT");
		CString strActvNum=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTVNUM");		
		CString strRfrnceNumber=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM");
		//CString strProcdureNumber=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM");		//commented by a.ali,11-8-2020,field deleted
		CString strAudtDprt=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT");
		CString strCmmnt=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CMNT");
		CString strActvSts=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTVTSTS");
		CString strAdtCnclsn=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADTCNCLSN");
		CString strCarNum=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CARNUM");
		CString strCarYr=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CARYR");		
		CString strSite=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_SITE");
		CString strLocTyp=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_LOCTYPE");		
		CString strFrmDat=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_FRMDATE");
		CString strToDat=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_TODATE");		
		//CString strArea=FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_AREA");		
		DoToolBarAction(TOOLBAR_NEW,"HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT");	
		/*int nVrsnNumber=atoi(strVrsnNumber);
		nVrsnNumber++;
		CString strNewVrsnNum=itoa(nVrsnNumber);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_VRSNNUM",strNewVrsnNum);*/
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTVNUM",strActvNum);		
		if(strActvNum == "")
		{
			DoToolBarAction(TOOLBAR_CANCEL,"HSE_TGADTPLNENT","HSE_TGADTPLNENTACTVT");
			return;
		}
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM",strRfrnceNumber);
		//FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_PRCDRCHCKLSTNAM",strProcdureNumber);				//commented by a.ali,11-8-2020,field deleted
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT",strAudtDprt);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CMNT",strCmmnt);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTVTSTS",strActvSts);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADTCNCLSN",strAdtCnclsn);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CARNUM",strCarNum);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_CARYR",strCarYr);		
		//FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_AREA",strArea);		
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_SITE",strSite);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_LOCTYPE",strLocTyp);		
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_FRMDATE",strFrmDat);
		FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_TODATE",strToDat);		
		m_bDisableSerial = TRUE;
}

long AuditPlanEntryCategory::ShowAuditPlanEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNENT;
	return ParentManage("HSE_TGADTPLNENT",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}
