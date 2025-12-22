// AccidentCategory.cpp: implementation of the AccidentCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AccidentCategory.h"
#include "HSEobj.h"
#include <ctime>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AccidentCategory::~AccidentCategory()
{
}

/*Commented_Function: IsAllInjrdPrsnRcrdEntrd()*/

HRESULT AccidentCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strSubFormTag = pFieldChange->SubForm_Tag;
	CString strFldName    = pFieldChange->Field_Name;	
	if(strFldName == "ACDNTENTRFRNCS_RFRNCTYP") {		
		emptyTXNRefFlds();				//Empty TXN details fields(Year,Subject,Issue Date)
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

void AccidentCategory::emptyTXNRefFlds() {
	FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_YR","0");
	FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_SBJCT","");
	FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_ISUDT","");
}

HRESULT AccidentCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	if(strBtnName=="SAVE")
	{	
		if(pToolBarButtonClicked->iComplete==1 ) {
			bool bNewRec = m_pCategory->GetHSEObj()->getbNewMode(strFormTag);
			if(bNewRec) {
				getAccidentRootCauses();
			}
		} else {			
			/*Commented_Code Deleted,Please check source safe earlier versions*/
			//404_405_2
			//Action :Press (save) button
			//if(strSubFormTag=="HSE_TGACDNTENTINJRDPRSN") {
				/*Commented_Code Deleted,Please check source safe earlier versions*/
			//}
		}
	}
	if(strBtnName == "NEW") {
		if(strSubFormTag == "HSE_TGACDNTENTLOSS") {			
			CString strSiteCode = FormGetField("HSE_ACDNTENT","ACDNTENT_SIT");
			CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
			CString strDefCurncy = getDefCurrency(strCmpnyNam);
			FormSetField("HSE_ACDNTENTLOSS","CRNCY",strDefCurncy);		
		} else if (strSubFormTag == "HSE_TGACDNTENTMTRLRLS") {	
			int nComplete = pToolBarButtonClicked->iComplete;
			if( nComplete == 0) {
				CString strAcdntNum = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
				int nMtrlRlsCount = getRelatedMtrlRlsCount(strAcdntNum);
				if(nMtrlRlsCount > 0) {					
					CString strSiteCode = FormGetField("HSE_ACDNTENT","ACDNTENT_SIT");
					CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
					CString strMultiMtrlRlsAllowed = IsMultiMtrlRlsAllowed(strCmpnyNam);
					if(strMultiMtrlRlsAllowed == "0") {
						*pToolBarButtonClicked->iCancle = 1;
						AfxMessageBox("According to policy,only one record allowed in Material Release tab.");
						return S_FALSE;
					}
				}
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

//Enable Confirmation comments in confirmation screens 
//note : not in specs 
HRESULT AccidentCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);	
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGACDNTCNFRMTN")
		{			
			SetFieldVisibleAttrib("HSE_TGACDNTCNFRMTN","ACDNTENT_CNFRMTNCMNT","NULL");		
		}
		//Enable Our Vehicles,Other Vehicles only if  Vehicles Involved = "Yes"
		CString strVhclsInvld = FormGetField("HSE_ACDNTENT","ACDNTENT_VHCLSINVLD");
		if(strVhclsInvld == "1") {
			EnableTab("HSE_TgAcdntEntOurVhcls",true);
			EnableTab("HSE_TgAcdntEntOthrVhcls",true);
			EnableTab("HSE_TGACDNTENTQSTNS",true);
			EnableTab("HSE_TGACDNTENTPOLCRPRT",true);			
		} else {
			EnableTab("HSE_TgAcdntEntOurVhcls",false);
			EnableTab("HSE_TgAcdntEntOthrVhcls",false);
			EnableTab("HSE_TGACDNTENTQSTNS",false);
			EnableTab("HSE_TGACDNTENTPOLCRPRT",false);
		}
		//Enable Causers Tab if caused by = company
		CString strCauser = FormGetField("HSE_ACDNTENT","ACDNTENT_CSDBY");
		if(strCauser == "C") {			
			EnableTab("HSE_TgAcdntEntCsr",true);
		} else {
			EnableTab("HSE_TgAcdntEntCsr",false);
		}
		//Enable tab material Release if Material Release Invloved = Yes
		CString strMtrlRlsInvlvd = FormGetField("HSE_ACDNTENT","ACDNTENT_MTRLRLSINVLVD");
		if(strMtrlRlsInvlvd == "1") {
			EnableTab("HSE_TGACDNTENTMTRLRLS",true);
		} else {
			EnableTab("HSE_TGACDNTENTMTRLRLS",false);
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

/***************************************************************************************************************/
HRESULT AccidentCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strSubFormTag = pBrowsingSelectedItem->SubForm_Tag;
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;
	CString strReturnValue = pBrowsingSelectedItem->Return_Value;
	CString strFullRecord = pBrowsingSelectedItem->Full_Record;	
	CString strYearFldNam,strSubjectFldNam,strIssueDateFldNam,strYearVal,strSubjectVal,strIssueDateVal;									
	if(strBrowsingField.CompareNoCase("ACDNTENTRFRNCS_NRMSNO") == 0 || strBrowsingField.CompareNoCase("ACDNTENTRFRNCS_ASMNTNO") == 0 
		|| strBrowsingField.CompareNoCase("ACDNTENTRFRNCS_CAR") == 0 || strBrowsingField.CompareNoCase("ACDNTENTRFRNCS_PTWNO") == 0 ) {									
		CString strTXNTyp = FormGetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_RFRNCTYP");
		if(strTXNTyp == "1") {
			strYearFldNam = "NRSTMISCENT_YR";
			strSubjectFldNam = "NRSTMISCENT_SBJCT" ;
			strIssueDateFldNam = "NRSTMISCENT_ISUDAT";
		} else if (strTXNTyp == "2") {
			strYearFldNam = "RSKASSMNTENT_YR";
			strSubjectFldNam = "RSKASSMNTENT_SBJCT";
			strIssueDateFldNam = "RSKASSMNTENT_ISUDAT";
		}
		else if (strTXNTyp == "3") {
			strYearFldNam = "CRCTVEACCENT_YR";
			strSubjectFldNam = "CRCTVEACCENT_SBJCT";
			strIssueDateFldNam = "CRCTVEACCENT_ISUDAT";
		}
		else if (strTXNTyp == "4") {
			strYearFldNam = "PTWSRGSTRENT_YR";
			strSubjectFldNam = "";
			strIssueDateFldNam = "PTWSRGSTRENT_ISUDAT";
		}
		strYearVal = GetFieldValue(strFullRecord,strYearFldNam);
		if(strSubjectFldNam != "")
			strSubjectVal= GetFieldValue(strFullRecord,strSubjectFldNam);
		else 
			strSubjectVal= "";
		strIssueDateVal = GetFieldValue(strFullRecord,strIssueDateFldNam); 					
		FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_YR",strYearVal);
		FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_SBJCT",strSubjectVal);
		FormSetField("HSE_ACDNTENTRFRNCS","ACDNTENTRFRNCS_ISUDT",strIssueDateVal);
	}
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}

HRESULT AccidentCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID :	405_400~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="ACDNTENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			//UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"ACDNTENT","ACDNTENT_ACDNTNUM");	
			//RefreshScreen("",REFRESH_SELECTED);
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectIncident '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);	
		}
	}
	//404_405_5
	else if (strButton_Name== "INJRYTYP") {	
		openInjuryType(strForm_Tag);
	//} else if(strButton_Name == "ACDNTENT_PRSNLPRTCTVEQPMNTS") {	//TODO : Check button name , how this name change
	} else if(strButton_Name == "PRSNL_PRTCTVE_EQUIP") {
		openPrsnlPrtctvEqpmntScr(strForm_Tag);
	} else if (strButton_Name=="ACDNTENTACDNTRSN_GETROTCUS") {
		getAccidentRootCauses();
	} else if (strButton_Name=="INJURY_ANALYSIS") {
		openInjueryRsltScr(strForm_Tag);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AccidentCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();	
	if(strFieldName == "ACDNTENTCSR_EMPL") {
		CString strSQL;
		CString strDprt = FormGetField("HSE_ACDNTENTCSR","ACDNTENTCSR_DPRT");							
		if(strDprt.IsEmpty())
			strDprt = " ";
		strSQL.Format("WHERE (EMPLOYEE_WSHOP ='%s')",strDprt);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

void AccidentCategory::rejectRecord(CString strForm_Tag)
{
	setSrcKyFldNm("ACDNTNUM");
	setOwnerScreenTag(m_strScreenTag);
	CString strAcdntNum=FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
	if(strForm_Tag=="HSE_TGACDNTCNFRMTN")
	{
		Reject("ACDNTENT-L1",strAcdntNum);
	}
	else if(strForm_Tag=="HSE_TGACDNTFOLLWUPDT")
	{
		Reject("ACDNTENT-L2",strAcdntNum);
	}
}

/*void AccidentCategory::OpenBodyParts(CString strFormTag)
{
	CString strKeyField=FormGetField("HSE_ACDNTENTINJRDPRSN","ACDNTENTINJRDPRSN_LNRNUM");
	m_strScreenTag = "HSE_TGHMNBODYPART";	
	CString strSQL="";
	CString strDefValues;
	bool bLocked = true;
	strSQL.Format("SELECT * FROM HSE_HMNBODYPART WHERE (HMNBODYPART_LNK = '%s')",strKeyField);
	m_strScreenTag = "HSE_TGHMNBODYPART";
	if(strFormTag == "HSE_TGACDNTENT") {
		bLocked = false;
	} 
	strDefValues.Format("HMNBODYPART_LNK|%s",strKeyField);
	ShowScreen(0,"HSE_TGHMNBODYPART","Human body parts",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}*/

/**
 *  Moved to HSECommonCategory to handle Incident Medical Report
 *	Open Injury Type pop-up screen
 *	@param strFormTag if screen opened from personnel accident entry screen , open it for edit else open it locked
 */
/*void AccidentCategory::openInjuryType(CString strFormTag)
{
	bool bLocked = true;
	m_strScreenTag = "HSE_TGINJRYTP";	
	CString strSQL="";
	CString strDefValues;
	CString strKeyField=FormGetField("HSE_ACDNTENTINJRDPRSN","ACDNTENTINJRDPRSN_LNRNUM");					
	strSQL.Format("SELECT * FROM HSE_INJRYTP WHERE (INJRYTP_LNK = '%s')",strKeyField);
	m_strScreenTag = "HSE_TGINJRYTP";
	if(strFormTag =="HSE_TGACDNTENT") {
		bLocked = false;
	}
	strDefValues.Format("INJRYTP_LNK|%s",strKeyField);
	ShowScreen(0,"HSE_TGINJRYTP","Injury Type",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
}*/

void AccidentCategory::openPrsnlPrtctvEqpmntScr(CString strFormTag)
{
	CString strKeyField=FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
	m_strScreenTag = "HSE_TGPRSNLPRTCTVEQPMNT";	
	CString strSQL="";
	CString strDefValues;
	bool bLocked = true;
	strSQL.Format("SELECT * FROM HSE_PRSNLPRTCTVEQPMNT WHERE (PRSNLPRTCTVEQPMNT_LINK = '%s')",strKeyField);
	m_strScreenTag = "HSE_TGPRSNLPRTCTVEQPMNT";				
	if(strFormTag =="HSE_TGACDNTENT") {
		bLocked = false;
	}	
	strDefValues.Format("PRSNLPRTCTVEQPMNT_LINK|%s",strKeyField);
	ShowScreen(0,"HSE_TGPRSNLPRTCTVEQPMNT","Personal Protective Equipments",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}

void AccidentCategory::openInjueryRsltScr(CString strFormTag)
{
	CString strKeyField=FormGetField("HSE_ACDNTENTINJRDPRSN","ACDNTENTINJRDPRSN_LNRNUM");
	m_strScreenTag = "HSE_TgInjryAnlysis";	
	CString strSQL="";
	CString strDefValues;
	bool bLocked = true;
	strSQL.Format("SELECT * FROM HSE_InjryAnlysis WHERE (InjryAnlysis_LINK = '%s')",strKeyField);	
	if(strFormTag =="HSE_TGACDNTENT") {
		bLocked = false;
	}	
	strDefValues.Format("InjryAnlysis_LINK|%s",strKeyField);
	ShowScreen(0,"HSE_TgInjryAnlysis","Injury Analysis",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}

void AccidentCategory::getAccidentRootCauses(){
	CString userName = GetUserLogin();
	CString strAccidentKey = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
	CString strAccidentType = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTTYP");	
	if(userName != "" && strAccidentKey != "" && strAccidentType != "") {
		CString strSQL = "EXECUTE getAccidentRootCauses '"+userName+"',"+strAccidentKey+",'"+strAccidentType+"'";
		ExecuteSQL(strSQL);
		RefreshScreen("HSE_TGACDNTENTACDNTRSN",REFRESH_SCREEN);	
	}
}


int AccidentCategory::getRelatedMtrlRlsCount(CString strAcdntNum) {
	CString strSQL;
	strSQL.Format("SELECT COUNT(0) AS MtrlRlsCount FROM HSE_AcdntEntMtrlRls WHERE AcdntEntMtrlRls_Lnk = %s",strAcdntNum);
	CString strMtrlRlsCount= GetRecordSetData(strSQL).GetBuffer(0);	
	int nMtrlRlsCount = atoi(strMtrlRlsCount);
	return nMtrlRlsCount;
}