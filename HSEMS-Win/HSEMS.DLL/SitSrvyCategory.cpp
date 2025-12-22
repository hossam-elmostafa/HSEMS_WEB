#include "stdafx.h"
#include "HSEMS.h"
#include "SitSrvyCategory.h"
#include "HSEobj.h"

SitSrvyCategory::SitSrvyCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
}

SitSrvyCategory::~SitSrvyCategory(){}

HRESULT SitSrvyCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	/*
	 * by : a.ali , 19-09-2017
	 * Field (SITSRVYENTVSTTM_COMPEMP) removed from tab
	 *
 	String strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	if(strFieldName=="SITSRVYENTVSTTM_COMPEMP") {
		CString strCompEmp(pFieldChange->New_Value);
		CString strEmp=FormGetField("HSE_SITSRVYENTVSTTM","SITSRVYENTVSTTM_EMPNUM");
		if(strCompEmp=="0") {		
			FormSetField("HSE_SITSRVYENTVSTTM","SITSRVYENTVSTTM_EMPNUM","");					
			FormSetField("HSE_SITSRVYENTVSTTM","Emp_Dprt34"," ");
			FormSetField("HSE_SITSRVYENTVSTTM","Emp_JobTtl44"," ");
			FormSetField("HSE_SITSRVYENTVSTTM","Emp_Nam24"," ");
		} else if(strCompEmp=="1") {		
			FormSetField("HSE_SITSRVYENTVSTTM","SITSRVYENTVSTTM_NAM","");
			FormSetField("HSE_SITSRVYENTVSTTM","SITSRVYENTVSTTM_COMP","");
		}		
	}*/

	////////////////////////////////////////////////////////////////////
	/*RQ-2-2021(Batch2).30
		Site Survey Entry	There is a need to identify records for referencing. Such as when refering to the record with a colleage or in an email it should have some simple unique identifier	"•	In (findings) TAB
		o	Add (serial No.) locked & auto No.
		"
	*/

	CString strFieldName(pFieldChange->Field_Name);	
	strFieldName.MakeUpper();
	if(strFieldName.Find("SITSRVYENTFNDNG")>=0)
	{
		CString strLineNo = FormGetField("HSE_TGSITSRVYENTFNDNG","SITSRVYENTFNDNG_SRIL");
		if(strLineNo == "") {
			getNewSerialNoForATab("HSE_SITSRVYENT","SitSrvyEnt_SitSrvyNo","HSE_SITSRVYENTFNDNG","SitSrvyEntFndng_LNK","SITSRVYENTFNDNG_SRIL");
			
		}
	}
	////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////
	/*RQ-2-2021(Batch2).33
			"•        In (findings) TAB
			o        In (Required actions) popup :
			?        Set (action No.) = auto-serial No. & locked"
	*/
	CString strFormTag(pFieldChange->Form_Tag);
	strFormTag.MakeUpper();
	if(strFieldName="HSE_TGSITSRVYRQRDACTN")
	{
		
		CString strLineNo = FormGetField(strFormTag,"SITSRVYRQRDACTN_ACTNNO");
		if(strLineNo == "") {
			//Bugs-3-2021.95
			getNewSerialNoForATab("HSE_SITSRVYRQRDACTN","SitSrvyRqrdActn_LNK","HSE_SITSRVYRQRDACTN","SitSrvyRqrdActn_LNK","SITSRVYRQRDACTN_ACTNNO");
			
		}
	}

	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}



HRESULT SitSrvyCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	/*
	 *	Tab removed visits , fields moved to header screen
	 * 
	 if(strFieldName=="SITSRVYENTVSTLOC_LOCNAM")
	{
		CString strSiteName=FormGetField("HSE_SITSRVYENT","SITSRVYENT_SIT");
		if(strSiteName.IsEmpty())
			strSiteName=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSiteName);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}*/
	
	/*
	 * Update : 16-08-2020 , Moved to firstSysDB
	  if(strFieldName == "SITSRVYENT_LOCNAM")
	{
		CString strSiteName=FormGetField("HSE_SITSRVYENT","SITSRVYENT_SIT");
		if(strSiteName.IsEmpty())
			strSiteName=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSiteName);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}*/
	/*
	 * Tab removed visits , fields moved to header screen
	 *
	if(strFieldName=="SITSRVYENTVSTLOC_AREANAM")
	{
		CString strLocationName=FormGetField("HSE_SITSRVYENTVSTLOC","SITSRVYENTVSTLOC_LOCNAM");
		if(strLocationName.IsEmpty())
			strLocationName=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocationName);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}*/
	/* Update : 16-08-2020 , Moved to firstSysDB
	 *
	if(strFieldName=="SITSRVYENT_AREANAM")
	{
		CString strLocationName=FormGetField("HSE_SITSRVYENT","SITSRVYENT_LOCNAM");
		if(strLocationName.IsEmpty())
			strLocationName=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocationName);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}*/
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT SitSrvyCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButton_Name = pCustomButtonClicked->Button_Name;
	CString strForm_Tag    = pCustomButtonClicked->Form_Tag;
	if(strButton_Name == "SITSRVYENT_RJCT") {
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord)
		{			
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectSitSrvy '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);
		}
	} 	
	else if(strButton_Name == "SITSRVYRQRDACTN_RQRDACTNBTN") {		//Open Required Action Screen
		bool bLocked = true;
		m_strScreenTag = "";	
		CString strSQL="";
		CString strDefValues,strDefSrcScr;

		CString strKeyField=FormGetField("HSE_SITSRVYENTFNDNG","MAINLINK");			
		strSQL.Format("SELECT * FROM HSE_SITSRVYRQRDACTN WHERE (SITSRVYRQRDACTN_LNK = '%s')",strKeyField);
		m_strScreenTag = "HSE_TGSITSRVYRQRDACTN";
		if(strForm_Tag == "HSE_TGSITSRVYENT" || strForm_Tag == "HSE_TGSITSRVYCNFRMTN" || strForm_Tag == "HSE_TGSITSRVYINQ") {		
			if(strForm_Tag != "HSE_TGSITSRVYINQ") {
				bLocked = false;			
			}
			m_pCategory->GetHSEObj()->setDisableSitSrvyRqrdActnBtn(TRUE);
		} else {
			m_pCategory->GetHSEObj()->setDisableSitSrvyRqrdActnBtn(FALSE);
		}
		if(strForm_Tag == "HSE_TGSITSRVYENT") {
			strDefSrcScr.Format("TXNSTS|%s","1");
		} else if (strForm_Tag == "HSE_TGSITSRVYCNFRMTN") {
			strDefSrcScr.Format("TXNSTS|%s","4");
		}
		strDefValues.Format("SITSRVYRQRDACTN_LNK|%s|%s",strKeyField,strDefSrcScr);
		//Bugs-3-2021.96
		bool blocked=false;
		if(strForm_Tag=="HSE_TGSITSRVYINQ")
			blocked=true;
		////////////////////////////////////////////////////////////////////////////////
		ShowScreen(0,"HSE_TGSITSRVYRQRDACTN","Required Actions",NORMAL_MODE,true,strSQL,"",strDefValues,"",blocked);	
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void SitSrvyCategory::rejectRecord(CString strForm_Tag)
{
	CString strSrvyNo=FormGetField("HSE_SITSRVYENT","SITSRVYENT_SITSRVYNO");	
	if(strSrvyNo=="")	//if no record selected
		return;
	setSrcKyFldNm("SITSRVYNO");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGSITSRVYCNFRMTN") {			
		Reject("SITSRVYENT-L1",strSrvyNo);			
	}
	else if(strForm_Tag=="HSE_TGSITSRVYFLWUP") {	
		Reject("SITSRVYENT-L2",strSrvyNo);		
	}
}



