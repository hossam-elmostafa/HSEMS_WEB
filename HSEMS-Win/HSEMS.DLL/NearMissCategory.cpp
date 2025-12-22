// NearMissCategory.cpp: implementation of the NearMissCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissCategory.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NearMissCategory::NearMissCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

NearMissCategory::~NearMissCategory()
{

}

HRESULT NearMissCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName = pFieldChange->Field_Name;
	CString strFieldValue= pFieldChange->New_Value;
	CString strTableName = pFieldChange->Table_Name;
	// To Solve Bug id : 8651 
	// Done by : Mohamed Elghobashi	
	if(strFieldName == "NRSTMISCENT_EMP"){
		if(strFieldValue=="0" || strFieldValue=="False")
		{							
			FormSetField(strTableName,"NrstMiscEnt_Nam","");
			//a.ali,30-6-2020,Ref : Urgent - HSEMS Discovered BUG's 24 Jun 2020
			//FormSetField(strTableName,"EMP_DPRT34","");
		} else {
			FormSetField(strTableName,"NRSTMISCENT_NONEMPDPRT"," ");
			FormSetField(strTableName,"NRSTMISCENT_NONEMPNAM"," ");
		}	
	}	

	else if(strFieldName == "NRSTMISCENT_OBSACC"){
		//////////////////////////////
		//RQ-1-2024.6
		//o   If (Observation Accepted = Yes) then ENABLE the (Require CAR) combo
		//o   If (Observation Accepted = No) then DISABLE the (Require CAR) combo
		/*
			If the user selects (Observation Accepted = Yes), then he will have the option to select (Require CAR) either (Yes) or (No).
			and, If the user selects (Observation Accepted = No), then he has no option with (Require CAR) and it must be locked, and may be with value = No.
			It is also possible for the user to change his choices, therefore, when user change the value of first combo, i guess we must empty the second combo
		*/
		if(strFieldValue=="Y")
		{
			FieldSetAttribSingle("NRSTMISCENT_RQRCR","",-1,"","OPTIONAL","","",-1,"","","","","",-1,-1,"");
			FormSetField(strTableName,"NRSTMISCENT_RQRCR","");
		}
		else if(strFieldValue=="N")
		{
			FieldSetAttribSingle("NRSTMISCENT_RQRCR","",-1,"","ALWAYSLOCKED","","",-1,"","","","","",-1,-1,"");
			FormSetField(strTableName,"NRSTMISCENT_RQRCR",strFieldValue);
		}
		//////////////////////////////

	}
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

HRESULT NearMissCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strTableName(pToolBarButtonClicked->Table_Name);
	CString strMainStatus;
	//polydb_task nRecordStatus = 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(strBtnName=="SAVE")
	{
		if(pToolBarButtonClicked->iComplete==0) {
			//000_2
			//Action : Add new record
			//Condition : If new reject reason for that record exist 
			//Response : Message box inform user that reject reason already exist and cancel add operation 
			//Else Add new record
			if(strSubFormTag=="HSE_TGNRSTMISCFLWUPREJRSN"||strSubFormTag=="HSE_TGNRSTMISCCNFRMTNRJCRSN") {																									
				SetRjctRsnFlag(pToolBarButtonClicked,"NRSTMISCENT_NRSTMISCNUM");								
			}
			//Save new record only without any validation on it.
			//Specs Rules Id : 401_371_2,401_371_3,402_32410_1 and 402_32410_2
			//polydb_task nRecordStatus = 0
			//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE" && pToolBarButtonClicked->nRecordStatus==0)
			strMainStatus = FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_RECSTS");		
			if(strTableName == "HSE_NRSTMISCENTRQDACTN" || strTableName == "HSE_NRSTMISCENTPSSBLNRSTMISCCUSE") {
				SetRecordStatus(strTableName,strMainStatus);
			}
		} else {
			bool bNewRec = m_pCategory->GetHSEObj()->getbNewMode(strFormTag);
			if(bNewRec) {
				getNearMissRootCauses();
				getNearMissHazards();
			}
		}
	}	
	// check if current user can delete or modify current record or not.
	// user can modify or delete the records that inserted by him/her in the same status.
	// this change according HSE_CR12v2_32,HSE_CR12v2_34,HSE_CR12v2_38 and HSE_CR12v2_40
	// Done by : Mohamed Elghobashi
	// Specs Rules Id : 401_371_2,401_371_3,402_32410_1 and 402_32410_2
	//polydb_task nRecordStatus = 0
	//if(pToolBarButtonClicked->iComplete==0 && pToolBarButtonClicked->nRecordStatus==1 && (strBtnName=="DELETE" || strBtnName == "SAVE") &&(strTableName == "HSE_NRSTMISCENTRQDACTN" || strTableName == "HSE_NRSTMISCENTPSSBLNRSTMISCCUSE"))
	if(pToolBarButtonClicked->iComplete==0 && (strBtnName=="DELETE" || strBtnName == "SAVE") 
		&&(strTableName == "HSE_NRSTMISCENTRQDACTN" || strTableName == "HSE_NRSTMISCENTPSSBLNRSTMISCCUSE"))
	{		
		if (DoSaveDeleteValid(strTableName) == false)
		{
			*pToolBarButtonClicked->iCancle=1;
			AfxMessageBox("You are not allowed to Delete/Modify this record.");
			return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
		}
	}
	
	//added by a.ali
	//date : 25/09/2016 
	//Desc : when reposition from record with locked fields , locked attribute become default attributes for fields 
	if(strSubFormTag == "HSE_TGNRSTCNFRMEXPCTDHRZ" || strSubFormTag == "HSE_NRSTFOLLOWEXPCTDHRZ") {
		if (pToolBarButtonClicked->iComplete == 0) {						
			unLockExpctdHzrdFlds(strSubFormTag);			
		}
	}
	
	//added by a.ali
	//date : 25/09/2016 
	//Desc : when reposition from record with locked fields , locked attribute become default attributes for fields 
	if (/*strSubFormTag =="HSE_TGNRSTMISCFLWUPRQRDACC"||*/strSubFormTag == "HSE_TGNRSTMISCCNFRMTNRQRDACC"
	  || strSubFormTag =="HSE_TGNRSTMISCCNFRMTNPSSBLNRSTMISCCUSE" /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE" */
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNENVRNMNTCNDTNS"   /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPENVRNMNTCNDTNS"*/
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMINVSTGTNTEMS"		  /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPINVSTGTNTEMS"*/
	  || strSubFormTag == "FRMSIGNTR_NEARMSCONF_FRMSGNTR"		  || strSubFormTag == "NEARMISSFOLW_FRMSGNTR"
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNATCH"             /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPATCH"*/) {
		if (pToolBarButtonClicked->iComplete==0) {
			// Comments above By Hossam 14/9/2021 RQ-8-2021.5
			EnableRequireedActionFields(strSubFormTag);
		}
	}


	
	if(strBtnName=="NEW")
	{
		if (strSubFormTag =="HSE_TGNRSTMISCENTRQRDACC" || strSubFormTag =="HSE_TGNRSTMISCFLWUPRQRDACC" || strSubFormTag =="HSE_TGNRSTMISCCNFRMTNRQRDACC")
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			/*
				RQ-8-2021.3
				RQ-8-2021.4
				RQ-8-2021.5
				Observation Entry	More control	In the (Recommended Actions) TAB, set default (require CAR) = (HSE policy > CAR > Default, Require CAR)	
			*/
			if(pToolBarButtonClicked->iComplete)
			{
				CString strDefaultValue="0";
				if(checkRequireCAR())
					strDefaultValue="1";			//NRSTMISCENTRQDACTN_RQRCAR , HSE_TGNRSTMISCENT.HSE_TGNRSTMISCENTRQRDACC
				FormSetField(strSubFormTag,"NRSTMISCENTRQDACTN_RQRCAR",strDefaultValue);	
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////
		}
		if (strFormTag == "HSE_TGNRSTMISCENT" && pToolBarButtonClicked->iComplete==1)
		{
			//////////////////////////////////////////////////////
			//RQ-HM-11-10-23.01.06.03
			CString strUserDepartment=getEmpDep(m_pCategory->GetUserLogin(),true);
			if(strUserDepartment!="")
			{
				FormSetField(strFormTag ,"NRSTMISCENT_CNCRNDDEP",strUserDepartment);
				FormSetField(strFormTag ,"NRSTMISCENT_AREA","");
			}
			//////////////////////////////////////////////////////
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
//Enable Confirmation comments in confirmation screens 
HRESULT NearMissCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(strFormTag=="HSE_TGNRSTMISCCNFRMTN"&&pRecordRepostion->iComplete==1)
	{
		SetFieldVisibleAttrib("HSE_TGNRSTMISCCNFRMTN","NRSTMISCENT_CNFRMTNCMNT","NULL");
	}
	if (/*strSubFormTag =="HSE_TGNRSTMISCFLWUPRQRDACC"||*/strSubFormTag == "HSE_TGNRSTMISCCNFRMTNRQRDACC"
	  || strSubFormTag =="HSE_TGNRSTMISCCNFRMTNPSSBLNRSTMISCCUSE" /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE" */
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNENVRNMNTCNDTNS"   /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPENVRNMNTCNDTNS"*/
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMINVSTGTNTEMS"		  /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPINVSTGTNTEMS"*/
	  || strSubFormTag == "FRMSIGNTR_NEARMSCONF_FRMSGNTR"		  || strSubFormTag == "NEARMISSFOLW_FRMSGNTR"
	  || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNATCH"             /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPATCH"*/) {
		if (pRecordRepostion->iComplete==1) {
			EnableRequireedActionFields(strSubFormTag);
		}
	} else if(strSubFormTag == "HSE_TGNRSTCNFRMEXPCTDHRZ" || strSubFormTag == "HSE_NRSTFOLLOWEXPCTDHRZ") {
		if (pRecordRepostion->iComplete == 1) {						
			CString strTXNSts = FormGetField("HSE_NRSTEXPCTDHRZ","TXNSTS");
			if(strSubFormTag == "HSE_TGNRSTCNFRMEXPCTDHRZ" && strTXNSts != "4" && strTXNSts != "") {
				lockExpctdHzrdFlds(strSubFormTag);
			} else if(strSubFormTag == "HSE_NRSTFOLLOWEXPCTDHRZ" && strTXNSts != "5" && strTXNSts != "") {
				lockExpctdHzrdFlds(strSubFormTag);
			} else {
				unLockExpctdHzrdFlds(strSubFormTag);
			}
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

void NearMissCategory::EnableRequireedActionFields(CString strSubFormTag)
{
	bool bEnabled = false;
	CString strTableName = "";
	if(strSubFormTag.Find("RQRDACC")!= -1){
		strTableName = "HSE_NRSTMISCENTRQDACTN";
	}
	else if (strSubFormTag.Find("NRSTMISCCUSE")!= -1){
		strTableName = "HSE_NRSTMISCENTPSSBLNRSTMISCCUSE";	
	}
	CString strEnteredIn = FormGetField(strTableName,"TXNSTS");
	CString strMustAttrib	  = "ALWAYSLOCKED";
	CString strOptionalAttrib = "ALWAYSLOCKED";
	if ((strSubFormTag == "HSE_TGNRSTMISCCNFRMTNRQRDACC" || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNPSSBLNRSTMISCCUSE") && strEnteredIn == "4")
		bEnabled = true;
	else if ((strSubFormTag == "HSE_TGNRSTMISCFLWUPRQRDACC" || strSubFormTag == "HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE") && strEnteredIn == "5")
		bEnabled = true;
	//In poly-db FormGetField for TXNSTS return 0 if no value selected,so add new condition if strEnteredIn == "0" enable fields
	if (bEnabled || strEnteredIn == "" || strEnteredIn == "0") {
		strMustAttrib = "MUST";
		strOptionalAttrib = "NULL";
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);
	} else {
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
	}
	/*
	//The above comment was done as per RQ-8-2021.6
	if(strSubFormTag == "HSE_TGNRSTMISCCNFRMTNRQRDACC" || strSubFormTag == "HSE_TGNRSTMISCFLWUPRQRDACC"){
		
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_ACTNNO",strOptionalAttrib );
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_ACCTYP",strOptionalAttrib );
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_ACCDESC",strMustAttrib );
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_TRGTDAT",strOptionalAttrib );
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_BYDEP",strMustAttrib );
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTRQDACTN_RQRCAR",strOptionalAttrib );
	}
	*/
	if(strSubFormTag == "HSE_TGNRSTMISCCNFRMTNPSSBLNRSTMISCCUSE" /*|| strSubFormTag == "HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE"*/){
		//The above comment was done as per RQ-8-2021.6
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM",strMustAttrib);
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_GRP",strMustAttrib);
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_DTLCOD",strMustAttrib);
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_SUBDTL",strMustAttrib);
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_DESC",strOptionalAttrib);
		SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_INVTBY",strOptionalAttrib);
		//SetFieldVisibleAttrib(strSubFormTag,"NRSTMISCENTPSSBLNRSTMISCCUSE_CMNT",strOptionalAttrib);
	}
	SetFieldVisibleAttrib(strSubFormTag,"ACCPTDBYHSE","");
}

HRESULT NearMissCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID :	402_370~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="NRSTMISCENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectObservation '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	else if(strButton_Name=="NRSTMISCENTPSSBLNRSTMISCCUSE_GETROTCUS") {	//Get Root Cause
		getNearMissRootCauses();
	}
	else if (strButton_Name=="NRSTEXPCTDHRZ_GETNRMISHZRDS") {			//Get Near Miss Hazards
		getNearMissHazards();
	}
	else if(strButton_Name=="NRSTMISCENT_CNCL"){
		//////////////////////////////////////////////////////////////
		//RQ-1-2024.9
		/*
			5- When user clicks (Cancel) button then display message (are you sure to cancel the observation - (Yes/No) )
					- If user clicks (No) then ignore the user action
					- If user clicks (Yes) then change record status into “cancelled" (99) & insert record into tracing Tab, and refresh screen
		*/
		int nRet = MessageBox(NULL,"Are you sure to cancel the observation - (Yes/No)?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		CString strStatus = FormGetField(strForm_Tag,"NRSTMISCENT_RECSTS");	
		if(nRet == IDYES && strStatus!="99") {
			FormSetField(strForm_Tag,"NRSTMISCENT_RECSTS","99");	
			CString strUserName = GetUserLogin();
			CString linkFieldVal = FormGetField(strForm_Tag,"NrstMiscEnt_NrstMiscNum");	
			CString strScreenName="";
			if (strForm_Tag=="HSE_TGNRSTMISCCNFRMTN")
				strScreenName="Observation Review";
			else if (strForm_Tag=="HSE_TGNRSTMISCFLWUP")
				strScreenName="Observation Approval";
			CString strSQL;
			strSQL.Format("insert into HSE_NRSTMISCENTTRC (NRSTMISCENTTRC_DATTIM,NRSTMISCENTTRC_ACCDESC,NRSTMISCENTTRC_LNK,NRSTMISCENTTRC_ENTUSR,SRCSCRN) values (getdate(),'Canceled',%s,'%s','%s')",linkFieldVal,strUserName,strScreenName);
			ExecuteSQL(strSQL);		
			DoToolBarAction(TOOLBAR_SAVE,strForm_Tag,"");
			RefreshScreen("",REFRESH_SELECTED);
		}
		//////////////////////////////////////////////////////////////
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void NearMissCategory::rejectRecord(CString strForm_Tag)
{
	CString strNrstMiscNum=FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	setSrcKyFldNm("NRSTMISCNUM");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGNRSTMISCCNFRMTN")
	{
		Reject("NRSTMISCENT-L1",strNrstMiscNum);
	} else if(strForm_Tag=="HSE_TGNRSTMISCFLWUP") {
		Reject("NRSTMISCENT-L2",strNrstMiscNum);
	}
}

HRESULT NearMissCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	//Rule ID : 402_307070_1,402_307070_2,403_307070~2_1,403_307070~2_2,401_307070_1,401_307070_2
	if(strFieldName=="NRSTEXPCTDHRZ_HZRDCTG")
	{
		CString strHzrdGrp=FormGetField("HSE_NRSTEXPCTDHRZ","NRSTEXPCTDHRZ_HZRDGRP");
		if(strHzrdGrp.IsEmpty())
			strHzrdGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK='%s')",strHzrdGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	} else if(strFieldName=="NRSTEXPCTDHRZ_HRMTYP") {
		CString strTskGrp=FormGetField("HSE_NRSTEXPCTDHRZ","NRSTEXPCTDHRZ_HZRDCTG");
		if(strTskGrp.IsEmpty())
			strTskGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strTskGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	//RQ-8-2021.4
	//By Hossam 16/9/2021 
	//(Action Verified By) combo, optional, browsing list of ""active"" employees of the (HSE policy > owner department)
	else if (strFieldName=="NRSTMISCENTRQDACTN_ACTVRFBY")
	{
		CString strOwnerDept = getOwnerDept("");
		CString strSQL;
		strSQL.Format("WHERE (EMPLOYEE_WSHOP ='%s')",strOwnerDept);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-10-2021.53
	//The following lines moved from the child NearMissEntryCategory to be used with all Near Miss Screens and not the entry only	
	else if(strFieldName=="NRSTMISCENTPSSBLNRSTMISCCUSE_DTLCOD")
	{
		CString strGrp=FormGetField("HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_GRP");
		if(strGrp.IsEmpty())
			strGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	else if(strFieldName=="NRSTMISCENTPSSBLNRSTMISCCUSE_SUBDTL") {
		CString strDtlCode =FormGetField("HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_DTLCOD");								
		if(strDtlCode.IsEmpty())
			strDtlCode=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strDtlCode);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}		
	else if (strFieldName=="NRSTMISCENT_PRVWBY")
	{
		//RQ-9-2023.23
		//browsing “Active” employee records, WHERE (employee > department) = (HSE policy > owner department)
		CString strOwnerDept = getOwnerDept("");
		CString strSQL;
		strSQL.Format("WHERE (EMPLOYEE_WSHOP ='%s' and ACTIVE_REC = 'Y')",strOwnerDept);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

void NearMissCategory::SetRecordStatus(CString strTableName,CString strMainStatus)
{
	if (strMainStatus == "2" || strMainStatus == "3")
		strMainStatus = "1";
	FormSetField(strTableName,"TXNSTS",strMainStatus);	
}

BOOL NearMissCategory::DoSaveDeleteValid(CString strTableName)
{
	// this function was disabled base on mr. samer requirement , 
	// See EnableRequireedActionFields
		return true;
		bool RetVal = true;
		CString strEntrdby = FormGetField(strTableName,"ENTRDBY");
		CString strCurrUser = GetUserLogin();
		CString strRecordStatus =  FormGetField(strTableName,"TXNSTS");
		CString strMainStatus = FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_RECSTS");
		// check if the status saved in the tab record is the same as the main 
		// or (the tab record status is Incomplete && main status is Rejected)
		if(strMainStatus == strRecordStatus || (strRecordStatus == '1' && strMainStatus == '3'))
		{
			// check the current user = user entered the record.
			if(strEntrdby != strCurrUser)
			{
				return RetVal = false;					
			}
		} else {
			return RetVal = false;			
		}
		return RetVal;
}

void NearMissCategory::lockExpctdHzrdFlds(CString strSubFormTag) {
	EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDGRP","ALWAYSLOCKED");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDCTG","ALWAYSLOCKED");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HRMTYP","ALWAYSLOCKED");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDDESC","ALWAYSLOCKED");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_EFCTDESC","ALWAYSLOCKED");
	//SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_STS","NULL");
}

void NearMissCategory::unLockExpctdHzrdFlds(CString strSubFormTag) {
	EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDGRP","MUST");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDCTG","MUST");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HRMTYP","NULL");			
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_HZRDDESC","NULL");			
	//HSE BGC-EZZ Requirements_2892020
	//SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_EFCTDESC","MUST");
	SetFieldVisibleAttrib(strSubFormTag,"NRSTEXPCTDHRZ_EFCTDESC","NULL");
}

void NearMissCategory::getNearMissRootCauses() {
	CString userName = GetUserLogin();
	CString strNearMissEntryKey = FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	CString strNearMissType		= FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRMISTYP");		
	CString strSQL = "EXECUTE getRootCauses '"+userName+"',"+strNearMissEntryKey+",'"+strNearMissType+"'";
	ExecuteSQL(strSQL);
	RefreshScreen("HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE",REFRESH_SCREEN);	
}

void NearMissCategory::getNearMissHazards(){
	CString userName = GetUserLogin();
	CString strNearMissEntryKey = FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	CString strNearMissType		= FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_NRMISTYP");		
	CString strSQL = "EXECUTE getExpectedHazards '"+userName+"',"+strNearMissEntryKey+",'"+strNearMissType+"'";
	ExecuteSQL(strSQL);
	RefreshScreen("HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE",REFRESH_SCREEN);	
}

//*****************************************************************************************************************************************
HRESULT NearMissCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strSubFormTag = pBrowsingSelectedItem->SubForm_Tag;
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;
	CString strReturnValue = pBrowsingSelectedItem->Return_Value;
	CString strFullRecord = pBrowsingSelectedItem->Full_Record;	
	CString strYearFldNam,strSubjectFldNam,strIssueDateFldNam,strYearVal,strSubjectVal,strIssueDateVal;									
	if(strBrowsingField == "NRSTMISCENT_REPDEP") {		
		CString strOccuranceDT = FormGetField("HSE_VWNRSTMISCENT","NRSTMISCENT_ISUDAT");
		getTargetMonthSTD(strReturnValue,strOccuranceDT);
		FormSetField("HSE_VWNRSTMISCENT","NRSTMISCENT_TRGTMNTHSTD",getTargetMonthSTD(strReturnValue,strOccuranceDT));
	}
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}
//*****************************************************************************************************************************************

//HSE Comments 26-08-2020
//The department (target month STD's) should appear once the department is selected & before record "save" 
CString NearMissCategory::getTargetMonthSTD(CString strDep,CString strOccuranceDT) {	
	CString strSQL;
	strSQL.Format("SELECT OBSRVTNTRGTSTRGTS_EXPCTDNOOBSRVTNS FROM HSE_OBSRVTNTRGTSTRGTS WHERE ObsrvtnTrgtsTrgts_Lnk = (SELECT ObsrvtnTrgts_PrmKy FROM HSE_OBSRVTNTRGTS WHERE OBSRVTNTRGTS_YR = YEAR('%s') AND OBSRVTNTRGTS_MNTH = MONTH(CONVERT(DATETIME,'%s',103))) AND OBSRVTNTRGTSTRGTS_DEP = '%s'",strOccuranceDT,strOccuranceDT,strDep);			
	CString strRetVal = GetRecordSetData(strSQL).GetBuffer(0);	
	return strRetVal;

}