// PtwCategory.cpp: implementation of the PtwCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PtwCategory.h"
#include "HSEMSCommonCategory.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PtwCategory::PtwCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


PtwCategory::~PtwCategory()
{

}

HRESULT PtwCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{	
	CString strFormTag  = CString(pScreenReady->Form_Tag);	
	CString SubForm_Tag;	
	if(strFormTag == "HSE_TGPTWSRGSTRENT") {
		SubForm_Tag = "HSE_TGPTWSRGSTRENTLINKEDWOS";
	} else if (strFormTag == "HSE_TGPTWSRGSTRCNFRMTN") {
		SubForm_Tag = "HSE_TGPTWSRGSTRCNFRMTNLINKEDWOS";
	} else if (strFormTag == "HSE_TGPTWSRGSTRCLS") {
		SubForm_Tag = "HSE_TGPTWSRGSTRCLSLINKEDWOS";
	} else if (strFormTag == "HSE_TGPTWSRGSTRINQ") {
		SubForm_Tag = "HSE_TGPTWSRGSTRINQLINKEDWOS";
	}			
	//GMS-HSE Integration-Phase-1-WO-PTW-To IS
	bool bGMSIntegrationEnabled = isGMSIntegrationEnabled();	
	if(!bGMSIntegrationEnabled) {				
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_Remarks","HIDDEN","NewValidationAttrib");		
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_SubOrgnatgDprt","HIDDEN","NewValidationAttrib");		
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_SubDept","HIDDEN","NewValidationAttrib");
		UpdateTabAttrib("66",strFormTag,SubForm_Tag,"NewTabAttrib","HIDDEN");		
	} else {
		CString strValidationAttr = "ALWAYSLOCKED";
		if(strFormTag == "HSE_TGPTWSRGSTRENT") {
			strValidationAttr = "NULL";
		}
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_Remarks",strValidationAttr,"NewValidationAttrib");	
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_SubOrgnatgDprt",strValidationAttr,"NewValidationAttrib");
		UpdateValidation("66",strFormTag,"","PTWsrgstrEnt_SubDept",strValidationAttr,"NewValidationAttrib");
		UpdateTabAttrib("66",strFormTag,SubForm_Tag,"NewTabAttrib","NULL");		
	}	

	//Check if caller app is GMS
	int callerApp = m_pCategory->GetHSEObj()->m_callerAppCode;
	if(callerApp != APPCODE) {
		
		if (strFormTag.Find("HSE_TGPTWSRGSTRENT")>-1) {
			FormEnableButton(UNKNOWN,BASETOOLBAR,"CUSTOM_PTWsrgstrEnt_EntCmplt",FALSE,FALSE);			
		} else if (strFormTag == "HSE_TGPTWSRGSTRCNFRMTN") {  
			FormEnableButton(UNKNOWN,BASETOOLBAR,"CUSTOM_PTWsrgstrEnt_cnfrm",FALSE,FALSE);
			FormEnableButton(UNKNOWN,BASETOOLBAR,"CUSTOM_PTWsrgstrEnt_Rjc",FALSE,FALSE);
		} else if (strFormTag == "HSE_TGPTWSRGSTRCLS"){
			FormEnableButton(UNKNOWN,BASETOOLBAR,"CUSTOM_PTWsrgstrEnt_Rjc",FALSE,FALSE);
			FormEnableButton(UNKNOWN,BASETOOLBAR,"CUSTOM_PTWsrgstrEnt_CLSPTW",FALSE,FALSE);
		}
	}
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT PtwCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	//polydb_task nRecordStatus = 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(strBtnName=="SAVE") {
		if(pToolBarButtonClicked->iComplete==0)
		{						
				//000_2
				//Action : Add new record
				//Condition : If new reject reason for that record exist 
				//Response : Message box inform user that reject reason already exist and cancel add operation 
				//Else Add new record
			if(strSubFormTag=="HSE_TGPTWSRGSTRENTRJCRSN"||strSubFormTag=="HSE_TGPTWSRGSTRCNFRMTNRJCRSN") {																									
					SetRjctRsnFlag(pToolBarButtonClicked,"PTWSRGSTRENT_PTWSNUM");						
			}
		}
		else {
			if(strSubFormTag=="HSE_TGPTWSRGSTRENTSPNDLG"||strSubFormTag=="HSE_TGPTWSRGSTRENTRNWLG") {
				RefreshScreen("HSE_TGPTWSRGSTRENT",REFRESH_SELECTED);
			}
		}
	}
	else if(strBtnName=="NEW") {
		
			if(strSubFormTag == "HSE_TGPTWSRGSTRENTRNWLG"){
				CString strPTWType = FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSTYP");	
				if(strPTWType == "CONF-SPACE") {
					*pToolBarButtonClicked->iCancle = 1;				
					return S_FALSE;
				}
			}
		
		if(strSubFormTag == "HSE_TGPTWSRGSTRENTRNWLG" || strSubFormTag == "HSE_TGPTWSRGSTRCNFRMTNRNWLG") {
			FormSetField("HSE_PTWSRGSTRENTRNWLG","PTWSRGSTRENTRNWLG_CRNTEXPCTDCMPDAT",FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_NWEXPCTDCMPLTNDAT"));
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

//Enable Confirmation comments in confirmation screens 
HRESULT PtwCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGPTWSRGSTRCNFRMTN")
		{
			SetFieldVisibleAttrib("HSE_TGPTWSRGSTRCNFRMTN","PTWSRGSTRENT_CNFRMTNCMNT","NULL");		
		}
		//a.ali,05/08/2018
		//Renew is not allowed for type : [PTW Type] = Confined Space Entry		
		CString strPTWType = FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSTYP");	
		if(strPTWType == "CONF-SPACE") {
			EnableTab("HSE_TGPTWSRGSTRENTRNWLG",false);
		} else {
			EnableTab("HSE_TGPTWSRGSTRENTRNWLG",true);
		}			
	}	
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT PtwCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 502_440~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="PTWSRGSTRENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			//UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"PTWSRGSTRENT","PTWSRGSTRENT_PTWSNUM");	
			//RefreshScreen("",REFRESH_SELECTED);
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectPTW '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT PtwCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{		
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	if(strFieldName.Find("PTWSRGSTRENTRNWLG") != -1) {		
		//get PTW TXN number
		CString strPTWNo = FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSNUM");
		CString strSite  = FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_SIT");	
		CString strCompany = getCmpnyNamFrmSit(strSite);
		bool bAccepted = isRenewAccepted(strCompany,atoi(strPTWNo));
		if(!bAccepted) {
			AfxMessageBox(IDS_INVALID_RENEW_PTW);
			FormSetField("HSE_PTWSRGSTRENTRNWLG","PTWSRGSTRENTRNWLG_RNWDAT","");			
		}
	}
	if(strFieldName == "PTWSRGSTRENT_JOBCMPLTTIM") {
		FormSetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_NWEXPCTDCMPLTNDAT", (pFieldChange->New_Value));
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

void PtwCategory::rejectRecord(CString strForm_Tag)
{
	setSrcKyFldNm("PTWSNUM");
	setOwnerScreenTag(m_strScreenTag);
	CString strPTWNum=FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSNUM");
	if(strForm_Tag=="HSE_TGPTWSRGSTRCNFRMTN")
	{
		Reject("PTWSRGSTRENT-L1",strPTWNum);
	}
	else if(strForm_Tag=="HSE_TGPTWSRGSTRCLS"){
		Reject("PTWSRGSTRENT-L2",strPTWNum);
	}
}

bool PtwCategory::isRenewAccepted(CString strCompanyNam,int ptwNO){
	CString strSQL;
	strSQL.Format("SELECT [DBO].[ISRENEWACCEPTED] (%d,'%s') AS ACCEDPTED",ptwNO,strCompanyNam);	
	CString strAccepted = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strAccepted == "1")
		return true;
	else
		return false;
}

bool PtwCategory::isGMSIntegrationEnabled(){
	bool bGMSIntegrationEnabled = false;
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_GMSINTGRTN,'') FROM HSE_HSEPLC");
	CString strGMSIntegrationEnabled = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strGMSIntegrationEnabled == "Yes")
		bGMSIntegrationEnabled = true;
	return bGMSIntegrationEnabled;
}

