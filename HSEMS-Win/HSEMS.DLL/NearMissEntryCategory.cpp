// NearMissEntryCategory.cpp: implementation of the NearMissEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissEntryCategory.h"
#include "EMailMsgHandler.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NearMissEntryCategory::NearMissEntryCategory(CCategory *pCategory):NearMissCategory(pCategory)
{
	m_pCategory = pCategory;
}
NearMissEntryCategory::~NearMissEntryCategory()
{
}
/*************************************************************************************************************/
//401_370_1
HRESULT NearMissEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//If (Near Miss Confirmation) checkbox [11@600] = "un-checked", then	
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="NRSTMISCENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}		
		
		//////////////////////////////
		//RQ-2-2024.14
		CString strDescription = FormGetField("HSE_TgNrstMiscEnt","NRSTMISCENT_NRSTMISCDESC");	
		CString strID=FormGetField("HSE_TgNrstMiscEnt","NRSTMISCENT_NRSTMISCNUM");	
		CString strSql;
		strSql.Format("update HSE_VWNRSTMISCENT set NRSTMISCENT_RPRDSC='%s' where NRSTMISCENT_NRSTMISCNUM=%s",strDescription,strID);
		long lRet = ExecuteSQL(strSql);
		//////////////////////////////

		CString strSenderName  = "HSEMS System";	
		CString strMailSubject = "Received Observation Auto Email From HSEMS";			
		CString strSiteCode = FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_SIT");
		CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
		CString strHSEDepMail  = getHSEDepMail(strCmpnyNam);
		bool bNearMissComplete = CompleteNearMiss(pCustomButtonClicked);
		if(bNearMissComplete) {			
			//EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
			//CString strMailBody = eMailMsgHandler->FormatMailBody(ID_MAIL_Observation_Complete,"");
			//CString strMsg = m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strHSEDepMail,strMailBody,"");
			//AfxMessageBox(strMsg);
			//RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return NearMissCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT NearMissEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	//Bugs-10-2021.53
	//Move the following lines to the parent NearMissCategory to be used with all Near Miss Screens and not the entry only
	/*
	if(strFieldName=="NRSTMISCENTPSSBLNRSTMISCCUSE_DTLCOD")
	{
		CString strGrp=FormGetField("HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_GRP");
		if(strGrp.IsEmpty())
			strGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	*/
	////////////////////////////////////////////////////////////////////////////////////////////
	//401_370_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="NRSTMISCENT_LOCTYP") {
		CString strLocTyp=FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//401_370_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="NRSTMISCENT_WRKLOC") {
		CString strLocTyp=FormGetField("HSE_vwNRSTMISCENT","NRSTMISCENT_LOCTYP");
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-10-2021.53
	//Move the following lines to the parent NearMissCategory to be used with all Near Miss Screens and not the entry only
	/*
	else if(strFieldName=="NRSTMISCENTPSSBLNRSTMISCCUSE_SUBDTL") {
		CString strDtlCode =FormGetField("HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_DTLCOD");								
		if(strDtlCode.IsEmpty())
			strDtlCode=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strDtlCode);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return NearMissCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
/*************************************************************************************************************/
HRESULT NearMissEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);	
	if(pRecordRepostion->iComplete==1)
	{		
		bool bObservationTabsEnabled = isObservationTabsEnabled();
		if(bObservationTabsEnabled) {
			EnableTab("HSE_TgNrstMiscEntpssblNrstMisccuse",true);
			EnableTab("HSE_TgNrstExpctdHrz",true);
			EnableTab("HSE_TgNrstMiscEntRqrdAcc",true);
			EnableTab("HSE_TgFRMSIGNTR",true);			
			EnableTab("HSE_TgNrstMiscEntinvstgtntems",true);
			EnableTab("HSE_TGNERMSENTEnvrnmntCndtns",true);
			EnableTab("HSE_TgNrstMiscEntRwrd",true);
			EnableTab("HSE_TgNrstMiscEntAtch",true);
			EnableTab("HSE_TgNrstMiscEntTrc",true);
			EnableTab("HSE_TGNERMSENTCMNTS",true);
		} else {
			EnableTab("HSE_TgNrstMiscEntpssblNrstMisccuse",false);
			EnableTab("HSE_TgNrstExpctdHrz",false);
			EnableTab("HSE_TgNrstMiscEntRqrdAcc",false);
			EnableTab("HSE_TgFRMSIGNTR",false);
			EnableTab("HSE_TgNrstMiscEntinvstgtntems",false);
			EnableTab("HSE_TGNERMSENTEnvrnmntCndtns",false);
			EnableTab("HSE_TgNrstMiscEntRwrd",false);
			EnableTab("HSE_TgNrstMiscEntAtch",false);
			EnableTab("HSE_TgNrstMiscEntTrc",false);
			EnableTab("HSE_TGNERMSENTCMNTS",false);
		}
	}
	return NearMissCategory::DisplayRecordRepostion(pRecordRepostion);
}
/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
bool NearMissEntryCategory::CompleteNearMiss(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	bool bNearMissComplete = false;
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE completeNearMissTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	long lRet = ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
	if(lRet != 0)
		bNearMissComplete  = true;	
	return bNearMissComplete;
}

long NearMissEntryCategory::ShowNearMissEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NRSTMISCENT;
	return ParentManage("HSE_TGNRSTMISCENT",TRUE,TRUE,TRUE,"HSE_vwNRSTMISCENT",strCriteria);
}

CString NearMissEntryCategory::getScreenFldVal(CString strFldName) {
	return "From Near Miss Entry";
}


bool NearMissEntryCategory::isObservationTabsEnabled()
{
	bool bObservationTabsEnabled = false;
	CString strObservationTabsLocked,strSQL;	
	strSQL.Format("SELECT TOP 1 ISNULL(HSEOBSRVTN_LCKENTRYTABS,'') FROM HSE_HSEOBSRVTN");
	strObservationTabsLocked = GetRecordSetData(strSQL).GetBuffer(0);	
	strObservationTabsLocked.MakeUpper();
	if(strObservationTabsLocked != "YES") {
		bObservationTabsEnabled = true;
	}	
	return bObservationTabsEnabled;
}



