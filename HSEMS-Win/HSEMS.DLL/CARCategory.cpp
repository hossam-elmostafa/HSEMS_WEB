// arCategory.cpp: implementation of the CarCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CARCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CarCategory::CarCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

CarCategory::~CarCategory(){}
/*******************************************************************************************************/
HRESULT CarCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{

	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	//polydb_task , nRecordStatus always sent 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{						
		//000_2
		//Action : Add new record
		//Condition : If new reject reason for that record exist 
		//Response : Message box inform user that reject reason already exist and cancel add operation 
		//Else Add new record
		if(strSubFormTag=="HSE_TGCRCTVEACCFLWUPREJRSN"||strSubFormTag=="HSE_TGCRCTVEACCCNFRMREJRES")																									
		SetRjctRsnFlag(pToolBarButtonClicked,"CRCTVEACCENT_CARMODELNUM");								
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/*******************************************************************************************************/
HRESULT CarCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}
/*******************************************************************************************************/
HRESULT CarCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{	
	CString strButton_Name = pCustomButtonClicked->Button_Name;
	CString strFormTag    = pCustomButtonClicked->Form_Tag;	
	if(strButton_Name == "CRCTVEACCENT_VWTXNDTLS") {		
		OpenTXNInquiryScreen();			
	} else if (strButton_Name=="SHOWCARREASON") {		
			CString strKeyField=FormGetField("HSE_CRCTVEACCENTTRC","CRCTVEACCENTTRC_TRCNG_REFRJCTRSN");
			if(strKeyField != "") {
				m_strScreenTag = "CMS_RSNS";	
				CString strSQL="";
				CString strDefValues;
				bool bLocked = true;
				strSQL.Format("SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '%s')",strKeyField);	
				strDefValues.Format("RSNS_RFREC|%s",strKeyField);
				ShowScreen(0,"CMS_RSNS","Reject Reasons",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
			}
	} else {
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
		if(strButton_Name == "CRCTVEACCENT_RJC") {			//Rule ID : 702_430~1_3			
			rejectRecord(strFormTag);
			bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
			if(bUpdateRejectedRecord==true) {												
				CString strSQL;
				CString strUserName = GetUserLogin();
				CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
				strSQL.Format("EXECUTE rejectCARTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
				ExecuteSQL(strSQL);	
				RefreshScreen("",REFRESH_SELECTED);
			}
		} else if(strButton_Name == "CRCTVEACCENT_CARNOTACPTD"){			
			bool bCARNotAcptd = setCARNotAcptd(KeyFieldValue,strFormTag);
			if(bCARNotAcptd) {
				CString strCARSerial = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_NO");
				CString strDep       = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_DPRT");
				CString strDepMail	 = getDepMail(strDep);
				sendCARNotAccptdNotification(strDepMail,strCARSerial); 
				RefreshScreen("",REFRESH_SELECTED);
			}
		} else if (strButton_Name == "CRCTVEACCENT_CARACPTD") {
			setCARAcptd(KeyFieldValue,strFormTag);
			RefreshScreen("",REFRESH_SELECTED);
		}						
	}		 	
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT CarCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}


HRESULT CarCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;

	if(strFieldName=="HSEVRFDBY" || strFieldName=="CRCTVEACCENTRQDACTN_HSEVRFDBY" || strFieldName=="CARFLLWUPVST_HSEVRFDBY")
	{		
		CString strSiteCode = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_SIT");
		CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
		CString strOwnerDept = getOwnerDept(strCmpnyNam);
		CString strSQL;
		//strSQL.Format("WHERE (EMP_DPRT ='%s')",strOwnerDept);		
		strSQL.Format("WHERE (EMPLOYEE_WSHOP ='%s')",strOwnerDept);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 

	}	
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

/*******************************************************************************************************
Helper function
*******************************************************************************************************/
void CarCategory::rejectRecord(CString strForm_Tag)
{
	CString strCarmodelNum=FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELNUM");
	setSrcKyFldNm("CARMODELNUM");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGCRCTVEACCCNFRMTN")
	{
		Reject("CRCTVEACCENT-L1",strCarmodelNum);
	}
	else if(strForm_Tag=="HSE_TGCRCTVEACCFLWUP")
	{
		Reject("CRCTVEACCENT-L2",strCarmodelNum);
	}
}

/*******************************************************************************************************
Helper function
*******************************************************************************************************/

/*********************************
 * Open related TXN Inquiry screen
 *
 *	Audit Follow up pop up screen 13
 *	Site Survey screen			  11
 *	Env Drill					  12
 *  Env Measure					  13
 *  
	[Incident|6]
	[Observation|3]	
	[Drill|12]	
	[ENV-Measure|13]
	[Audit|2]
	[MGMT-Tour|9]
	[Performance Measurement|1]	
	[Site Survey|11]
	[Risk Assessment|8]
	[Potential Hazard|4]
	[Vehicle Accident|5]
	[Actual Loss|7]
	[Others|10]
 *********************************/
void CarCategory::OpenTXNInquiryScreen()
{
	CString strAutoGenerated = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_AUTOGNRTD");
	if (strAutoGenerated == "N") {
		AfxMessageBox("Only Automatic generated CARs have related TXN");
		return;
	}
	CString strCARBasis = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELBAS");
	CString strTXNNum   = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_ACCDNTNUM");
	CString strTXNTag;
	CString strTXNKeyField;
	CString strWhereCond;
	CString strTableName;
	int nCARBasis = atoi(strCARBasis);
	switch(nCARBasis)				
	{
		case 1:											//Performance Measurement
			strTXNTag      = "HSE_TgperfMsrmntInq";
			strTXNKeyField = "PERFMSRMNTENT_VSTNO";
			strTableName   = "HSE_perfMsrmntEnt";
			break;	
		case 2:											//Audit
			strTXNTag      = "HSE_TgAdtPlnInq";
			strTXNKeyField = "MAINLINK";
			strTableName   = "HSE_AdtPlnEnt";
			break;
		case 3:											//Observation
			strTXNTag = "HSE_TgNrstMiscInq";
			strTXNKeyField = "NRSTMISCENT_NRSTMISCNUM";
			strTableName   = "HSE_vwNrstMiscEnt";
			break; 
		//TXN deleted
		case 4:											//Potential Hazard
			strTXNTag = "HSE_TgptnlhzrdInq";
			strTXNKeyField = "PTNLHZRDENT_PTNLHZRDNO";
			strTableName   = "HSE_ptnlhzrdEnt";
			break; 	
 		case 5:											//Vehicle Accident
		//TXN deleted
			strTXNTag = "HSE_TgVclAcdntInqtn";
			strTXNKeyField = "VCLACDNTENT_VCLACDNTNO";
			strTableName   = "HSE_VclAcdntEnt";			
			break; 
		case 6:											//Incident 
			strTXNTag = "HSE_TgAcdntInq";
			strTXNKeyField = "ACDNTENT_ACDNTNUM";
			strTableName   = "HSE_ACDNTENT";
			break; 
		//TXN deleted
		case 7:											//Loss Accident 
			strTXNTag = "HSE_TgLossAcdntInq";
			strTXNKeyField = "LOSSACDNTENT_ACTHZRDNO";
			strTableName   = "HSE_LossAcdntEnt";
			break; 
		case 8:											//Risk Assessment
			strTXNTag ="HSE_TgrskassmntInq";
			strTXNKeyField = "RSKASSMNTENT_ASSMNTNO";
			strTableName   = "HSE_rskassmntEnt";
			break;			
		case 11:											//Site Survey
			strTXNTag ="HSE_TGSITSRVYINQ";
			strTXNKeyField = "SITSRVYENT_SITSRVYNO";
			strTableName   = "HSE_SITSRVYENT";
			break;	
		
		case 12:											//Drill
			strTXNTag ="HSE_DRLLPLNINQ";
			strTXNKeyField = "DRLLPLN_PRMRYKY";
			strTableName   = "HSE_DRLLPLN_VW";
			break;

		case 13:											//ENV-Measure
			strTXNTag ="HSE_TGENVMNTREXCTN";
			strTXNKeyField = "ENVMNTRPLAN_YR";
			strTableName   = "HSE_ENVMNTRPLAN";
			break;
		default:	//0-record created from Corrective Action Screen,9-Management Review,10-Other
			return;
			break;
	};
	if(strTableName!="" && strTXNKeyField!="" && strTXNNum != "") {
		strWhereCond="SELECT * FROM "+strTableName+" WHERE ("+strTXNKeyField+" = "+strTXNNum+")";	
		ShowScreen(0,strTXNTag,"TXN Inquiry",NORMAL_MODE,false,strWhereCond,"","","",true);
	}
}

/**
 * reject CAR : Change CAR Status to "CAR Not Accepted" , update related record in required action 
 * under initiator TXN
 * @KeyFieldValue : selected record(s) key seprated by ","
 * @strFormTag
 */
bool CarCategory::setCARNotAcptd(CString KeyFieldValue,CString strFormTag) {
	bool bCARNotAcptd = false;
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
	strSQL.Format("EXECUTE setCARNotAcptd '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	long lRetVal = ExecuteSQL(strSQL);
	if(lRetVal != 0)
		bCARNotAcptd = true;
	return bCARNotAcptd;
}

/**
 * Accept CAR : Change CAR Status to "CAR Accepted" , update related record in required action 
 * under initiator TXN
 * @KeyFieldValue : selected record(s) key seprated by ","
 * @strFormTag
 */
void CarCategory::setCARAcptd(CString KeyFieldValue,CString strFormTag) {	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
	strSQL.Format("EXECUTE setCARAcptd '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}



// Reject CAR from Confirmation screen
// Change record status to Rejected-Review 2,Insert record into tracing tab
void CarCategory::RejectCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strCallerIdFinal = m_strFinal;
	CString strSQL,strNewStatus,strTracingAction;
	CString strUserName = GetUserLogin();
	CString strSrcScrTag = m_FSATableName;
	CString strSourceScreenName = GetScrCptnByTag(66,strSrcScrTag,"");
	CString KeyFieldValue = GetKeyField(strSrcScrTag,pCustomButtonClicked->pMultiSelectedRows);	
	/*if (strSrcScrTag == "HSE_TGCRCTVEACCCNFRMTN") {
		strNewStatus = "2";
		strTracingAction = "Rejected By Review";	
	} else if (strSrcScrTag == "HSE_TGCARFLWUPVSTS") {
		strNewStatus = "3";
		strTracingAction = "Rejected By Approval";
	} else if (strSrcScrTag == "HSE_TGCRCTVEACCRCVD") {
		strNewStatus = "17";
		strTracingAction = "Rejected By Received";
	} else if (strSrcScrTag == "HSE_TGCRCTVEACCUNDREXCTN") {
		strNewStatus = "16";
		strTracingAction = "Rejected By Execution";
	} else if (strSrcScrTag == "HSE_TGCRCTVEACCJOBVRFCTN") {
		strNewStatus = "21";
		strTracingAction = "Rejected By HSE";
	}*/
	strSQL.Format("EXECUTE rejectCARTXN '%s','%s','%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName,strSrcScrTag,strCallerIdFinal);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}	

void CarCategory::CancelCorrectiveAction(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strFormTag = pCustomButtonClicked->Form_Tag;
	setOwnerScreenTag(strFormTag);	
	CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");	
	strSQL.Format("EXECUTE CancelCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}