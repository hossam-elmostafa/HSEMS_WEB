// NearMissFollowUpCategory.cpp: implementation of the NearMissFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissFollowUpCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NearMissFollowUpCategory::NearMissFollowUpCategory(CCategory *pCategory):NearMissCategory(pCategory)
{
	m_pCategory = pCategory;
}

NearMissFollowUpCategory::~NearMissFollowUpCategory()
{
}
/**************************************************************************************************************/
HRESULT NearMissFollowUpCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{
	//403_370~2_1
	//This screen is permitted only if (HSE policy, Near Miss Follow-up  checkbox [11@600])= checked
	/*bool bIsNrMssFlwUpRqrd=CheckPolicy("HSEPLC_NRSTMISCFLW_UPRQRD");
	if(bIsNrMssFlwUpRqrd==true)
	{
		return NearMissCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	}
	//Else message box displayed to user with this message 
	//"Near Miss Follow-up checkbox must be checked to open this screen"
	else
	{
		ShowMessageEx(IDS_NMFR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}*/
	return NearMissCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
}
/**************************************************************************************************************/
HRESULT NearMissFollowUpCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//403_307~2_2	
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);

	DoToolBarAction(TOOLBAR_SAVE,pCustomButtonClicked->Form_Tag,"");
	if(strBtnName=="NRSTMISCENT_CLS")
	{
		bool bPendingRqrdActnsExist = false;
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "") {			
			//RQ-9-2023.31
			CString strEmployeeName=GetEmployeeCodeForLoginUser();
			if(strEmployeeName!="")
			///////////////////////////////
			{
				/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/					
				CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");
				CString strUserName = GetUserLogin();
				CString strSQL;
				strSQL.Format("EXECUTE closeNearMissTXN '%s','%s','%s'",KeyFieldValue,strUserName,strSourceScreenName);
				ExecuteSQL(strSQL);
				//////////////////////////
				//RQ-9-2023.31,RQ-9-2023.32
				
				//•  If ((Main > Require CAR) = Yes) then INSERT record into (CAR Entry) screen
				CString strRequireCAR = FormGetFieldEx(m_strFormTag,"NRSTMISCENT_RQRCR");	
				if(strRequireCAR=="Y")
				{				
					//o  SET (CAR Date) = current date 
					CString strFormat="DAYMONTHYEAR";
					CString strCARDateInitial=GetCurrentDate(strFormat);
					CString strCARDate=GeneralizeDate(strCARDateInitial);

					//o  SET (CAR Year) = current year 
					CString strCARYear=GetCurrentYear();

					//o  SET (CAR No.) = next available (CAR No.) per current year 
					CString strCARNo=GetNextCarNumForYear(strCARYear);

					//o  SET (CAR Status) = (HSE Policy > Observation > Generated CAR Status) 
					CString CARStatus=GetPolicyTabField("HSE_HSEOBSRVTN","HSEOBSRVTN_GnrCrStt");
					//////////////////////////////
					//Bugs-1-2024.6
					if(strlen(CARStatus) <2)
					{
						
						CString strTemp;
						strTemp.Format("0%s",CARStatus);
						CARStatus=strTemp;
					}
					//////////////////////////////

					//o  SET (CAR Target Date) = null 

					//o  SET (Department) = (HSE Policy > Owner Department) 
					CString strOwnerDept = getOwnerDept("");

					//o  SET (Employee) = (login user) > (Employee) > (Employee No.)
					//m_strLoginUserName
					//Bugs-12-2023
					CString strEmployee=strEmployeeName;
					//////////////

					//o  SET (Job Title) = driven from (Employee) record 
					
					//o  SET (NC Description) = (observation > main > description) 
					CString strNCDescription=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_NRSTMISCDESC");	
						   
					//o  SET (Evidence) = (observation > main > Evidence)
					CString strEvidence=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_SFTYOBSRV");	
						   
					//o  SET (Notes) = null 

					//o  SET (CAR Priority) = “Medium” 

					//o  SET (Site) = (observation) > (Site) 
					CString strSite=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_SIT");	

					//o   SET (Location) = (observation) > (Location) 
					CString strLocation=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_LOCTYP");	

					//o   SET (Area) = (observation) > (Area) 
					CString strArea=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_WRKLOC");	

					//o   SET (Exact Area) = null 

					//o   SET (Concerned Department) = (observation) > (Concerned Department) 
					CString strConcernedDepartment=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_CNCRNDDEP");	

					//o   SET (Project) = (observation) > (Project) 
					CString strProject=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_PRJCT");				
					
					//o   SET (Auto Generated) = Yes
					
					//o   SET (CAR Source) = (HSE Policy > observation > CAR Basis) 
					CString strCARSource=GetPolicyTabField("HSE_HSEOBSRVTN","HSEOBSRVTN_CrBss");
						   
					//o   SET (CAR Serial No.) = auto serial No. based on (CAR Year & CAR Source) 
					CString strQuery;
					strQuery.Format("select max(CRENTRY_CRSRLN)+1 from  hse_CRENTRY where CRENTRY_CRYR=%s and CRENTRY_CRSRC='%s'",strCARYear,strCARSource); 					
					CString strCARSerialNo=GetRecordSetData(strQuery);
						   
					//o   SET (TXN Year) = (observation > observation Year) 
					CString strTXNYear=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_YR");	
						   
					//o   SET (TXN No.) = (observation > observation No.) 
					CString strTXNNo=FormGetFieldEx(m_strFormTag,"NRSTMISCENT_NO");	
						  
					//o   SET (Detail No.) = null/

					CString strSql;
					if (strProject=="")
					{
						strSql.Format("insert into HSE_CRENTRY (CRENTRY_CRDT,CRENTRY_CRYR,CRENTRY_CRN,CRENTRY_CRSTT,CRENTRY_DPR, \
							CRENTRY_NM,CRENTRY_NCDSC,CRENTRY_EVD,CRENTRY_CRPRR,CRENTRY_ST,CRENTRY_LCT,CRENTRY_AR,CRENTRY_CNCDPR, \
							CRENTRY_ATGNR, CRENTRY_CRSRC,CRENTRY_CRSRLN,CRENTRY_TXNYR,CRENTRY_TXNN) \
							Values ('%s','%s','%s','%s','%s','%s','%s','%s','m','%s','%s','%s','%s','Y','%s','%s','%s','%s')", \
							strCARDate,strCARYear,strCARNo,CARStatus,strOwnerDept,strEmployee,strNCDescription,strEvidence,strSite, \
							strLocation,strArea,strConcernedDepartment,strCARSource,strCARSerialNo,strTXNYear,strTXNNo);
					}
					else
					{
						strSql.Format("insert into HSE_CRENTRY (CRENTRY_CRDT,CRENTRY_CRYR,CRENTRY_CRN,CRENTRY_CRSTT,CRENTRY_DPR, \
							CRENTRY_NM,CRENTRY_NCDSC,CRENTRY_EVD,CRENTRY_CRPRR,CRENTRY_ST,CRENTRY_LCT,CRENTRY_AR,CRENTRY_CNCDPR, \
							CRENTRY_ATGNR, CRENTRY_CRSRC,CRENTRY_CRSRLN,CRENTRY_TXNYR,CRENTRY_TXNN,CRENTRY_PRJ) \
							Values ('%s','%s','%s','%s','%s','%s','%s','%s','m','%s','%s','%s','%s','Y','%s','%s','%s','%s','%s')", \
							strCARDate,strCARYear,strCARNo,CARStatus,strOwnerDept,strEmployee,strNCDescription,strEvidence,strSite, \
							strLocation,strArea,strConcernedDepartment,strCARSource,strCARSerialNo,strTXNYear,strTXNNo,strProject);
					}
					CString str;
					str.Format("IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))");
					long lRecords=ExecuteSQL(str);
					str.Format("delete from ##TEMP_HSE_TABLE");
					lRecords=ExecuteSQL(str);
					str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('ActionDescription','Creating new CAR')");
					lRecords=ExecuteSQL(str);
					str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('SrcScreen','Observation Approval')");
					lRecords=ExecuteSQL(str);		
					CString strUserID=m_pCategory->GetUserLogin();
					str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('UserID','%s')",strUserID);
					lRecords=ExecuteSQL(str);
					ExecuteSQL(strSql);

					CString strPicture=FormGetField(m_strFormTag,"NRSTMISCENT_NRSTMISCNUM");	
					CString strSQL;
					strSQL.Format("select PrmKy from HSE_CRENTRY where CRENTRY_CRDT='%s' and CRENTRY_CRYR= '%s' and CRENTRY_CRN='%s'",strCARDate,strCARYear,strCARNo);
					CString strPrmKy=GetRecordSetData(strSQL,1);

					strSql.Format ("EXEC CopyImages 'HSEMS','HSE_VWNRSTMISCENT','NRSTMISCENT_NRSTMISCNUM','%s','NRSTMISCENT_OBSRVTNIMG','HSE_CRENTRY','PrmKy','%s','CRENTRY_NCPHT'",strPicture,strPrmKy);
					lRecords=ExecuteSQL(strSql);

					///////////////////////////////
					//RQ-12-2023.2
					//strSql.Format ("select max(CrEntry_CrN)MaxCarNum from HSE_CRENTRY where CrEntry_CrYr=%s",strCARYear);
					//CString strMaxCarNum=GetRecordSetData(strSql,1);
					CString strMessage;
					strMessage.Format("CAR is generated with CAR No. = %s-%s",strCARYear,strCARNo);
					AfxMessageBox(strMessage);
					///////////////////////////////
				//////////////////////////*/
				}
				RefreshScreen("",REFRESH_SELECTED);
			}
		}
	}
	return NearMissCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/**************************************************************************************************************/
HRESULT NearMissFollowUpCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return NearMissCategory::DisplayScreenReady(pScreenReady);
}

HRESULT NearMissFollowUpCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	CString strtag = m_strScreenTag;
	/*Commented_Code Deleted,Please check source safe earlier versions*/
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);

}
/**************************************************************************************************************/

/**************************************************************************************************************
Helper function
**************************************************************************************************************/
//Not Used 
void NearMissFollowUpCategory::CloseNearMiss(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Update (Record Status[40@370]) of selected record into (6)
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,6,"NRSTMISCENT","NRSTMISCENT_NRSTMISCNUM");
	//Insert record into (tracing [@37020~2]) tab
	InsertActionRecordIntoTracingTab("NRSTMISCENT","NRSTMISCNUM",CLOSED);
}

long NearMissFollowUpCategory::ShowNearMissFollowUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NRSTMISCFLWUP;
	//RQ-2-2024.5
	return ParentManage("HSE_TGNRSTMISCFLWUP",TRUE,TRUE,FALSE,"HSE_vwNRSTMISCENT",strCriteria);
}

//Not Used 
void NearMissFollowUpCategory::updtNrMisTypRotCusLib(CString strNearMissKey) {
	CString strSQL = "EXECUTE updateRootCauseLib "+strNearMissKey;
	ExecuteSQL(strSQL);	
}

//Not Used
void NearMissFollowUpCategory::updtNrMisTypExpctdHzrdsLib(CString strNearMissKey) {
	CString strSQL = "EXECUTE updateExpectedHazardLib "+strNearMissKey;
	ExecuteSQL(strSQL);	
}


CString NearMissFollowUpCategory::GetNextCarNumForYear(CString strCARYear)
{
	CString strSql;
	strSql.Format("select max(CRENTRY_CRN)+1 NewCARNo from hse_CRENTRY where CRENTRY_CRYR=%s",strCARYear);
	CString strMaxCarNum=GetRecordSetData(strSql,1);
	if(strMaxCarNum=="")//First time at this year
		strMaxCarNum="1";
	return strMaxCarNum;
}