// AuditPlanADTRSNADTES.cpp: implementation of the AuditPlanADTRSNADTES class.
//
// Note : This class will handle screen Audit Plan Assign Resources
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "AuditPlanADTRSNADTES.h"
#include "EMailMsgHandler.h"
#include "HSEobj.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanADTRSNADTES::AuditPlanADTRSNADTES(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanADTRSNADTES::~AuditPlanADTRSNADTES()
{

}

HRESULT AuditPlanADTRSNADTES::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSQL;	
	m_strScreenTag = pCustomButtonClicked->Form_Tag;	
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
	if(KeyFieldValue != "" && strBtnName != "SELECT")
	{
		CString strCmpnyName = FormGetField("HSE_ADTPLNENT","ADTPLNENT_CMPNYNAM");
		CString strAdtFrmDat = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_FRMDATE");
		bool bValidAdtForAsgnRes = IsAuditValidToAssignRes(strAdtFrmDat,strCmpnyName);		
		if(strBtnName=="AUDITORS")
		{
			//if(bValidAdtForAsgnRes) {
				OpenAuditorsScreen(strFormTag,2);
				return S_OK;
			//}  else {
			//	CString strMsg="According to policy,You can't Add Auditors to this Audit.\nPlease check 'Advance Days to Assign Audit Resources.' and 'Audit Min Notification Days'";
			//	AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
			//}
		}
		else if(strBtnName=="AUDITIES")
		{
			//if(bValidAdtForAsgnRes) {
				OpenAuditeesScreen(strFormTag,1);
			//}  else {
			//	CString strMsg="According to policy,You can't Add Audities to this Audit.\nPlease check 'Advance Days to Assign Audit Resources.' and 'Audit Min Notification Days' ";
			//	AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
			//}
		}
		//Added by a.ali 16-09-2012		
		//specs : 601_461_3
		//HSE_CR09_7 : Delete screen audit plan checklist entry
		//and open it's pop up screen planned check list from activity tab under Audit plan resources screen
		else if(strBtnName == "ADTPLNENTACTVT_ADTCHCKLST")
		{
			//if(bValidAdtForAsgnRes) {
				openAuditCheckList();
			//} else {
			//	CString strMsg="According to policy,You can't Add Checklist to this Audit.\nPlease check 'Advance Days to Assign Audit Resources.";
			//	AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
			//}
		}		
		/*else if(strBtnName=="ENTRYCMPLTSNDCYCL")
		{
			CompleteAuditWthAdtrsNAdtees(pCustomButtonClicked);	
			//RefreshScreen("" ,REFRESH_SELECTED);
		}*/
		//Added by a.ali 17-09-2012
		//HSE_CR09_16
		//spec : 601_461_4
		else if(strBtnName == "RSORCCMPLT"){
			//call stored procedure to check Auditors Audittes and check list and change activity status to complete 
			//and audit plan to partial assign
			CString strActivtyKeyValue = FormGetField("HSE_ADTPLNENTACTVT" ,"MAINLINK");			
			CString strSenderName  = "HSEMS System";	
			CString strMailSubject = "Audit Notification , Auto E-mail From HSEMS";
			CString strAdtGrp = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ADITEDPRT");
			CString strAdtCOD = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_REFNUM");
			CString strHSEDepMail  = getAuditeeMail(strAdtGrp,strAdtCOD);
			bool bActivityComplete = CompleteActivity(KeyFieldValue,strActivtyKeyValue);
			if(bActivityComplete) {			
				EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
				CString strMailBody = eMailMsgHandler->FormatMailBody(ID_MAIL_Auditee_Complete,"HSE_ADTPLNENTACTVT");
				m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strHSEDepMail,strMailBody,"");
			}
		}
	}
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


HRESULT AuditPlanADTRSNADTES::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	//TODO : Should be moved to FirstSysDB,Problem disable New is not working in FirstSysDB
	EnableToolbar("HSE_TGADTPLNADTRSADTSACTV",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}


//Open Audit plan check list screen -HSE_CR09_7
void AuditPlanADTRSNADTES::openAuditCheckList(){
	CString strMainKeyFeild = FormGetField("HSE_ADTPLNENT" ,"MAINLINK");
	int nNumberOfRecords = getNumberOfRecord("HSE_ADTPLNENTACTVT" , strMainKeyFeild,"ADTPLNENTACTVT_LNK");
	CString strSQL ;

	//Audit Checklist without any record in the Activities screen shouldn't be opened
	if(nNumberOfRecords != 0)
	{
		CString strDefValues;
		//Response : "	Display the (Audit Checklist [@481]) pop-up screen"	Allow user to edit all records			
		CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");			
		m_pCategory->GetHSEObj()->setnAuditsKeyFld(atoi(strKeyField));		
		strDefValues.Format("ADTCHCKLSTPLN_LNK|%s",strKeyField);		
		//Bugs-2-2021.61
		//"•        In (audits) TAB > (actual checklist) popup > (audit checklist result) popup > (required actions) popup :
		//o        The (serial No.) of (recommended actions) is not correct, please check (Bug"
		strSQL.Format("SELECT * FROM HSE_ADTCHCKLSTPLN WHERE (ADTCHCKLSTPLN_LNK = %s) Order by ADTCHCKLSTPLN_TSKNUM",strKeyField);				
		ShowScreen(0,"HSE_TGADTCHCKLSTPLN","Audit Check List",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
	}
}

/*deleted Code , Please check Source Safe*/
bool AuditPlanADTRSNADTES::CompleteActivity(CString strAuditKeyFieldValue,CString strActivtyKeyValue){	
	bool bAuditComplete = false;	
	int nAuditorsCount = getNumberOfRecord("HSE_ADTPLNENTAUDITORS" , strActivtyKeyValue,"LINK");
	//int nAudittesCount = getNumberOfRecord("HSE_ADTPLNENTAUDITEES" , strActivtyKeyValue,"LINK");
	int nCheckListCount = getNumberOfRecord("HSE_ADTCHCKLSTPLN" , strActivtyKeyValue,"ADTCHCKLSTPLN_LNK");

	//if(nAuditorsCount > 0 && nAudittesCount > 0 && nCheckListCount > 0){
	if(nAuditorsCount > 0 && nCheckListCount > 0){
		//Change activity status to Complete Resource 
		CString strSQL = "EXECUTE sp_complete_activity '"+strAuditKeyFieldValue+"','"+strActivtyKeyValue+"'";
		long lRetVal = ExecuteSQL(strSQL);
		if(lRetVal != 0) {
			bAuditComplete = true;
		}
		RefreshScreen("HSE_TGADTPLNADTRSADTSACTV",REFRESH_SELECTED);
	}
	else{ 
		ShowMessageEx(IDS_CmpltAdtNAudt,"",IV_NOICON,"Complete Activity");
		return bAuditComplete;
	}
	return bAuditComplete;
}

long AuditPlanADTRSNADTES::ShowAuditPlanAuditorsandAuditees(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNADTRSNADTES;
	return ParentManage("HSE_TGADTPLNADTRSANDADTS",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}


