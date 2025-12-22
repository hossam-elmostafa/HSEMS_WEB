// VehicleAccident.cpp: implementation of the VehicleAccident class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "VehicleAccident.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VehicleAccident::VehicleAccident(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

VehicleAccident::~VehicleAccident()
{

}


/*
check if driver license expire date > today
return false 
else 
return true 
*/
bool VehicleAccident::CheckLicenseExpireDate()
{
	CString strDrvrLsnExprDate=FormGetField("HSE_VCLACDNTENT","VCLACDNTENT_DRVRLSNEXPDAT");
	if(strDrvrLsnExprDate.IsEmpty())
	{
		return false;
	}
	CString sDateNow="";
	COleDateTime dateDrvrLsnExprDate, dateNow ;
	dateDrvrLsnExprDate.ParseDateTime(strDrvrLsnExprDate,VAR_DATEVALUEONLY);
	CTime now = CTime::GetCurrentTime() ;
	sDateNow .Format("%02d/%02d/%04d",now.GetDay(),now.GetMonth(),now.GetYear());
	dateNow.ParseDateTime(sDateNow , VAR_DATEVALUEONLY );
	if( dateNow<dateDrvrLsnExprDate )			
		return true;
	else 
	    return false;
}


HRESULT VehicleAccident::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	if(strBtnName=="SAVE")
	{	
		//polydb_task nRecordStatus = 0
		//if(pToolBarButtonClicked->iComplete==1 && strFormTag == "HSE_TGVCLACDNTENT" && pToolBarButtonClicked->nRecordStatus == 0) {
		if(pToolBarButtonClicked->iComplete==1 && strFormTag == "HSE_TGVCLACDNTENT") {
			//407-510-4
			CString strEmpNo = FormGetField("HSE_VCLACDNTENT","VCLACDNTENT_DRVRNAM");
			int nAccdnt = calculateDriverTtlAccdnt(strEmpNo);
			CString strNumOfAccdnt = itoa(nAccdnt);
			FormSetField("HSE_VCLACDNTENT","VCLACDNTENT_NOACDNTDURSRVC",strNumOfAccdnt);
			//DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		}		
			//000_2
			//Action : Add new record
			//Condition : If new reject reason for that record exist 
			//Response : Message box inform user that reject reason already exist and cancel add operation 
			//Else Add new record
		if(pToolBarButtonClicked->iComplete==0 && (strSubFormTag=="HSE_TGVCLACDNTFLWUPRJCRSN"||strSubFormTag=="HSE_TGVCLACDNTCNFRMTNRJCRSN")) {																									
			SetRjctRsnFlag(pToolBarButtonClicked,"VCLACDNTENT_VCLACDNTNO");							
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


//Enable Confirmation comments in confirmation screens 
HRESULT VehicleAccident::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(strFormTag=="HSE_TGVCLACDNTCNFRMTN"&&pRecordRepostion->iComplete==1)
	{
		//SetFieldVisibleAttrib("HSE_TGVCLACDNTCNFRMTN","VCLACDNTENT_CNFRMCMNT","NULL");
		//SetFieldVisibleAttrib("HSE_TGVCLACDNTCNFRMTN","VCLACDNTENT_FLW_UPCMNT","NULL");
		SetFieldVisibleAttrib("HSE_TGVCLACDNTCNFRMTN","VCLACDNTENT_CNFRMTNCMNT","NULL");
	}	
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT VehicleAccident::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID :	408_510~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="VCLACDNTENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");	
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT VehicleAccident::DisplayBrowsingSelectedItem(BROWSINGSELECTEDITEM  *pBrowsingSelectedItem)
{
	CString strFullRecord = pBrowsingSelectedItem-> Full_Record;
	CString strFieldName = pBrowsingSelectedItem-> BrowsingField;		
		//to fix driven from department field ,employee screen it should be driven in appMgr but it doesn't work
		if(strFieldName == "VCLACDNTENT_DRVRNAM") {
			CString strEmpDept = GetFieldValue(strFullRecord,"EMP_DPRT");	
			CString strDprtName = getDeptNameByCod(strEmpDept);
			FormSetField("HSE_VCLACDNTENT","VCLACDNTENT_DEPRT",strDprtName);
		}			
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}

void VehicleAccident::rejectRecord(CString strForm_Tag)
{
	setSrcKyFldNm("VCLACDNTNO");
	setOwnerScreenTag(m_strScreenTag);
	CString strVclAcdntNo=FormGetField("HSE_VCLACDNTENT","VCLACDNTENT_VCLACDNTNO");
	if(strForm_Tag=="HSE_TGVCLACDNTCNFRMTN")
	{
		Reject("VCLACDNTENT-L1",strVclAcdntNo);
	}
	else if(strForm_Tag=="HSE_TGVCLACDNTFLWUP")
	{
		Reject("VCLACDNTENT-L2",strVclAcdntNo);
	}
}

/**
 *	Get total number of vechile accident(s) by driver
 *	@param strEmpNo employee number
 *	@return total number of accident by employee
 */
int VehicleAccident::calculateDriverTtlAccdnt(CString strEmpNo) {
	CString strSQL;
	strSQL.Format("SELECT COUNT(0) AS nAccdnt FROM HSE_VCLACDNTENT WHERE VCLACDNTENT_DRVRNAM = '%s'",strEmpNo);
	CString nAccdnt=GetRecordSetData(strSQL,1);		
	return atoi(nAccdnt);
}

/**
 *	Get Employee department name from department code
 */
CString VehicleAccident::getDeptNameByCod(CString strDeptCod)
{
	CString strSQL;
	strSQL.Format("SELECT DPRT_DESC FROM HSE_DPRT WHERE DPRT_COD = '%s'",strDeptCod);
	CString strDprtName = GetRecordSetData(strSQL,1);
	return strDprtName;
}