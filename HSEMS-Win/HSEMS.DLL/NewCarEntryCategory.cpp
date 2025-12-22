// NewCarEntryCategory.cpp: implementation of the CNewCarEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewCarEntryCategory.h"

#include "HSEMS.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewCarEntryCategory::CNewCarEntryCategory(CCategory *pCategory,CMapStringToOb* pmpListOfParameters):CAuditModuleCategory(pCategory,pmpListOfParameters)
{
}

CNewCarEntryCategory::~CNewCarEntryCategory()
{

}

HRESULT CNewCarEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFormTag(pFieldChange->Form_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	CString strNewVal(pFieldChange->New_Value);

	CString strNewSrl="" ;

	if(strFieldName=="CRENTRY_CRDT")
	{
		CString strDate=FormGetField(strFormTag,"CRENTRY_CRDT");
		COleDateTime OleDate;
		OleDate.ParseDateTime(strDate);
		int nYear=OleDate.GetYear();
		CString strYear="";
		strYear.Format("%d",nYear);
		CString strCurrentYear=FormGetField("HSE_CRENTRY","CRENTRY_CRYR");
		if(strCurrentYear!=strYear)
		{
			////////////////////////////////////////////////////////////
			//RQ-SF-9-1-23_2
			UpdateYearAndSerial(strFormTag,strYear);
			//DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
			////////////////////////////////////////////////////////////
		}
	}

	if(strFormTag=="HSE_TGCRRVW" && strFieldName=="CRENTRY_CRSRC")
	{
		/*RQ-SF-9-1-23_3_1_1 (NC Source) frame
		SET (CAR Serial No.) AL = auto serial No. per (CAR year) & (CAR source) 
		*/
		CString strCARSrc=FormGetField("HSE_CRENTRY","CRENTRY_CRSRC");
		if(strCARSrc!="")
		{
			CString strCARYear=FormGetField("HSE_CRENTRY","CRENTRY_CRYR");		
			CString strWhereCluase;
			strWhereCluase.Format("CRENTRY_CRYR='%s' and CRENTRY_CRSRC='%s'",strCARYear,strCARSrc);
			strNewSrl = getNxtSrl("CRENTRY_CRSRLN",m_strFormTable,"","",strWhereCluase);
			FormSetField("HSE_CRENTRY","CRENTRY_CRSRLN",strNewSrl);
		}
		else
			FormSetField("HSE_CRENTRY","CRENTRY_CRSRLN","");
	}

	if(strFormTag=="HSE_TGACTNSRVIW")
	{
		//////////////////////////////////////////////////////////////////
		/*
			RQ-SF-9-1-23_6_2
		*/
		if(strFieldName=="CRRCTVACTNS_LNKCSS_ACC")
		{
			if(strNewVal=="Y")
			{
				//If (Accepted = Yes) then LOCK the (Reject Reason) field
				SetFieldVisibleAttrib("HSE_ACTNSENTRY_CRR","CRRCTVACTNS_LnkCss_RjcRsn","ALWAYSLOCKED");
				
				//RQ_SF_23_3_23_1 
				//if (Accepted = Yes) then SET (Action Status = “Accepted”)
				FormSetField("HSE_ACTNSENTRY_CRR","ACTNSENTRY_CRR_ACTSTT","A");
			}
			else
			{
				//If (Accepted = No) then the (Reject Reason) should be Enabled & MUST
				SetFieldVisibleAttrib("HSE_ACTNSENTRY_CRR","CRRCTVACTNS_LnkCss_RjcRsn","MUST");

				//RQ_SF_23_3_23_1 
				//If (Accepted = No) then SET (Action Status = “Rejected”)
				FormSetField("HSE_ACTNSENTRY_CRR","ACTNSENTRY_CRR_ACTSTT","R");
			}

		}
		//////////////////////////////////////////////////////////////////
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT CNewCarEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strButtonName=(*pToolBarButtonClicked).Button_Name;
	CString strFormTag=(*pToolBarButtonClicked).Form_Tag;
	CString strSubFormTag=(*pToolBarButtonClicked).SubForm_Tag;
	if(strButtonName=="NEW" && (*pToolBarButtonClicked).iComplete==1)
	{
		//////////////////////////////////////////////////////
		//RQ-SF-9-1-23_2_4
		if(strFormTag=="HSE_TGCRENTRY")
		{
			CString strSQL;
			strSQL.Format("exec GetPolicyValue 'HSE_HSEPLC','HSEPLC_DFLST'");
			CString strDefaultSite= GetRecordSetData(strSQL).GetBuffer(0);
			FormSetField("HSE_CRENTRY","CRENTRY_ST",strDefaultSite);
			
			//RQ_SF_15_3_23_1
			CString strUserDepartment=getEmpDep(m_pCategory->GetUserLogin(),true);
			FormSetField("HSE_CRENTRY","CRENTRY_DPR",strUserDepartment);
			/////////////////////////////////////////////////////

		}
		//////////////////////////////////////////////////////
		if(strSubFormTag=="HSE_ACTNSENTRY_CRR")
		{
			//////////////////////////////////////////////////////
			//RQ-SF-9-1-23_5_2 (Corrections) Tab
			//(Serial) auto serial No., AL, incremented when click NEW record
			CString strPrmKy=FormGetField(strFormTag,"PrmKy");
			CString strNewSrl = getNxtSrl("ACTNSENTRY_Crr_Srl","HSE_ACTNSENTRY_Crr","PrmKy",strPrmKy);
			FormSetField("HSE_ACTNSENTRY_Crr","ACTNSENTRY_Crr_Srl",strNewSrl);
			//////////////////////////////////////////////////////
		}
		if(strSubFormTag=="HSE_ACTNSENTRY_RTCSS")
		{
			//////////////////////////////////////////////////////
			//RQ-SF-9-1-23_5_3_1 (Cause Analysis) frame
			//(Serial) auto serial No., AL, incremented when click NEW record
			CString strPrmKy=FormGetField(strFormTag,"PrmKy");
			CString strNewSrl = getNxtSrl("ACTNSENTRY_RtCss_Srl","HSE_ACTNSENTRY_RtCss","PrmKy",strPrmKy);
			FormSetField("HSE_ACTNSENTRY_RtCss","ACTNSENTRY_RtCss_Srl",strNewSrl);
			//////////////////////////////////////////////////////
		}
	}
	if(strButtonName=="SAVE")
	{
		if((*pToolBarButtonClicked).iComplete==1)
		{
			if(strFormTag=="HSE_TGCRENTRY")
			{
			//////////////////////////////////////////////////////
			/*RQ-SF-9-1-23_2_7 Standard (SAVE) button
			When “save” the CAR record, for only 1st time, then :
			Insert record into (Tracing) Tab
			SET (datetime) = current date & time
			SET (Source Screen) = “CAR Entry”
			SET (action description) = “Creating new CAR”
			SET (entry user) = @user name
			SET (CAR Status = 01)
			*/
				//CString strStatus=FormGetField(strFormTag,"CRENTRY_CRSTT");
				FormSetField(strFormTag,"CRENTRY_CRSTT","01");
				/*GetRecordSetData("select * from #TEMP_TABLE_AUTOTRC");
				GetRecordSetData("select * from #TEMP_HSE_TABLE");
				CString strSQL;
				//strSQL.Format("INSERT INTO #TEMP_HSE_TABLE (KEYrec,VALUErec) VALUES('CAR Entry','')",strPrmKy);
				
				
				CString strPrmKy=FormGetField(strFormTag,"PrmKy");
				CString strUpdateSQL;
				strUpdateSQL.Format("UPDATE HSE_CRENTRY_TRC SET CRENTRY_TRC_LNK=CRENTRY_TRC_LNK WHERE CRENTRY_TRC_LNK=%s",strPrmKy);
				long lRetFieldsCount = ExecuteSQL(strUpdateSQL);
				if (lRetFieldsCount == 0) //First time to save
				{
					CString strActionDescription="CAR Entry";
					CString strSrcScreen="Creating new CAR";
					InsertIntoTracingTab(strActionDescription,strSrcScreen,strPrmKy);
				}*/

				
			}
		}
		else
		{
			if(strFormTag=="HSE_TGCRENTRY")
			{
				CString str;//=GetRecordSetData("select * from ##TEMP_HSE_TABLE");
				//str.Format("begin try DELETE from ##TEMP_HSE_TABLE end try begin catch end catch");
				str.Format("IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))");
				long lRecords=ExecuteSQL(str);
				str.Format("delete from ##TEMP_HSE_TABLE");
				lRecords=ExecuteSQL(str);
				str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('ActionDescription','Creating new CAR')");
				lRecords=ExecuteSQL(str);
				str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('SrcScreen','CAR Entry')");
				lRecords=ExecuteSQL(str);
				//str=GetRecordSetData("select * from #TEMP_HSE_TABLE");

				CString strCurrentYear=FormGetField("HSE_CRENTRY","CRENTRY_CRYR");
				if(strCurrentYear=="")
				{
					CString strDate=FormGetField(strFormTag,"CRENTRY_CRDT");
					COleDateTime OleDate;
					OleDate.ParseDateTime(strDate);
					int nYear=OleDate.GetYear();
					CString strYear;
					strYear.Format("%d",nYear);
					UpdateYearAndSerial(strFormTag,strYear);
				}

			}
		}

	}
	if(strButtonName=="NEXT")
	{
		if(strFormTag=="HSE_TGACTNSRVIW")
		{
			/*
				RQ_SF_6_3_23_1
				(Main) screen 
				- The default value of (Review Date) should = today.   (The date of opening the review for the first time)
			*/
			CString strReviewData=FormGetField(strFormTag,"CRENTRY_RVWDT"); 
			if(strReviewData=="")
			{
				FormSetField(strFormTag,"CRENTRY_RVWDT",m_pHSEMSPolyDb->GetServerDate()); 
			}
			//////////////////////////////////////////////

		}
	}

	if(strButtonName=="DELETE")
	{
		if((*pToolBarButtonClicked).iComplete==0)
		{
			if(strFormTag=="HSE_TGCRENTRY")
			{
				//////////////////////////////////////////////////////
				//RQ-1-2024.10
				CString strIsAutomaticallyGenerated=FormGetField(strFormTag,"CRENTRY_ATGNR"); 
				if(strIsAutomaticallyGenerated=="Y")
				{
					AfxMessageBox("Record can not be deleted because it is automatically generated");
					*(pToolBarButtonClicked->iCancle)=true;
					return S_FALSE;
				}
				/////////////////////////////
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

extern CString gstrCncrnDprt;
extern CString gstrProject;

HRESULT CNewCarEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName=(*pCustomButtonClicked).Button_Name;
	CString strFormTag=(*pCustomButtonClicked).Form_Tag;
	CString strSubFormTag=(*pCustomButtonClicked).SubForm_Tag;
	CString strPrmKy=FormGetField(strFormTag,"PrmKy");
	
	if(strButtonName=="VIEW_SOURCE_TXN")// && (*pCustomButtonClicked).iComplete==1)
	{
		if(strFormTag=="HSE_TGCRENTRY" || strFormTag=="HSE_TGCRRVW" || strFormTag=="HSE_TGACTNSENTRY" || strFormTag=="HSE_TGACTNSRVIW" || strFormTag=="HSE_TGCRAPRVL")
		{
			//RQ-SF-9-1-23_2_1_8 (View Source TXN) Button	
			//RQ-SF-9-1-23_3_1_2 (View Source TXN) Button
			//RQ-SF-9-1-23_5_1_1 (View Source TXN) Button
			//(RQ_SF_23_5_23_1.5.1) (View Source TXN) Button
			//(RQ_SF_23_5_23_1.3.3) Add (View Source TXN) Button
			//Enabled only if (auto generated = Yes)
			CString strAutoGenerated=FormGetField(strFormTag,"CRENTRY_ATGNR");
			strAutoGenerated.MakeUpper();
			if(strAutoGenerated=="Y")
			{
				CString strCARSrc=FormGetField(strFormTag,"CRENTRY_CRSRC");
				strCARSrc.MakeUpper();
				/*
				Used to open the relative/source TXN screen, on a specific record, as follows :				
				If (main > CAR source) = “observation” then we need to open the (observation inquiry) screen, where (observation > observation year) = (main > TXN year) and (observation > observation No.) = (main > TXN No.) 
				Etc.
				*/
				if (strCARSrc=="INTERNAL AUDIT")
				{
					/*If (main > CAR source) = “internal audit” then we need to open the (audit plan inquiry) screen, 
					where (audit plan > audit year) = (main > TXN year) and (audit plan > plan No.) = (main > TXN No.) 
					and (audit plan > audits TAB > audit No.) = main > detail No.)
					*/

				}
			}
			else
				AfxMessageBox("Auto generated is set to 'No'");
			///////////////////////////////////////////////////////////
		}
	
	}

	if(strButtonName=="ENTRY_COMPLETED")
	{
		//CString strPrmKy=FormGetField(strFormTag,"PrmKy");
		if(strFormTag=="HSE_TGACTNSENTRY")
		{
/*
			RQ-SF-9-1-23_5_5 (Entry Completed) Button
			
*/
			/////////////////////////////////////////////////////////////////////
			//Check, if count (Root Causes) TAB = zero, then display error message (Must enter root causes) & cancel the user action
			CString strCheckExistance;
			strCheckExistance.Format("update HSE_ACTNSENTRY_RtCss set prmky=prmky where prmky=%s",strPrmKy);
			long lRecords=0;
			try
			{
				lRecords=ExecuteSQL(strCheckExistance);
				if(lRecords==0)
				{
					AfxMessageBox("Must enter root causes");
				}
			}
			catch(...)
			{
				AfxMessageBox("ERROR EXCECUTING STATEMENT 1453");
			}
			if(lRecords!=0)
			{
				/////////////////////////////////////////////////////////////////////
				//Check, each record in (Root Causes) TAB, must have at least (1) record in (Corrective Actions) popup screen.  
				strCheckExistance.Format("update HSE_ACTNSENTRY_RtCss set prmky=prmky where AutoFieldNum not in (select isnull(CrrctvActns_Lnkwthmn,0) from HSE_CrrctvActns) and prmky=%s",strPrmKy); 
				lRecords=0;
				try
				{
					lRecords=ExecuteSQL(strCheckExistance);
					if(lRecords!=0)
					{
						//If not, then display error message (Each root cause must at least have 1 action) & cancel the user action
						AfxMessageBox("Each root cause must at least have 1 action");
					}
				}
				catch(...)
				{
					AfxMessageBox("ERROR EXCECUTING STATEMENT 1453");
				}
				if(lRecords==0)
				{
				/*
				RQ_HM_17_8_23.01.04.02
				In (Corrections) Tab    
				- User MUST enter at least (1) record in this tab    
				*/
					strCheckExistance.Format("update HSE_ACTNSENTRY_CRR set prmky=prmky where prmky=%s",strPrmKy); 
					lRecords=0;
					try
					{
						lRecords=ExecuteSQL(strCheckExistance);
						if(lRecords==0)
						{
							//If not, then display error message (User MUST enter at least (1) record in Corrections tab ) & cancel the user action
							AfxMessageBox("User MUST enter at least (1) record in Corrections tab");
						}
					}
					catch(...)
					{
						AfxMessageBox("ERROR EXCECUTING STATEMENT 1454");
					}
					if(lRecords!=0)
					{

					/*
					Insert record into (Tracing) Tab
					SET (datetime) = current date & time
					SET (Source Screen) = “Actions Entry”
					SET (action description) = “Entry Completed”
					SET (entry user) = @user name
					*/			
						CString strActionDescription="Entry Completed";
						CString strSrcScreen="Actions Entry";
						InsertIntoTracingTab(strActionDescription,strSrcScreen,strPrmKy);

						//SET (CAR Status = 16)
						FormSetField(strFormTag,"CRENTRY_CRSTT","16");

						//BUG_HM_5_3_23_3
						//RQ_SF_23_3_23_1
						CString strSQL="";
						strSQL.Format("update HSE_ACTNSENTRY_CRR  set CRRCTVACTNS_LnkCss_Acc='Y',ACTNSENTRY_Crr_ActStt='A' where PrmKy=%s",strPrmKy);
						ExecuteSQL(strSQL);					
						
						//Bugs-3-2023.51
 						strSQL.Format("update HSE_CRRCTVACTNS  set CRRCTVACTNS_Acc='Y' , CRRCTVACTNS_ACTSTT=3 where CrrctvActns_LnkWthMn in (select AutoFieldNum from HSE_ACTNSENTRY_RTCSS where  PrmKy=%s)",strPrmKy);
						ExecuteSQL(strSQL);
						///////////////////////////////////////

						//Refresh screen
						DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
					}
				}
				
				/////////////////////////////////////////////////////////////////////
				
			}
		}		
		if(strFormTag=="HSE_TGCRENTRY")
		{
		////////////////////////////////////////////
		//RQ-SF-9-1-23_2_1_9 (Entry Completed) Button
		/*
			Insert record into (Tracing) Tab
			SET (datetime) = current date & time
			SET (Source Screen) = “CAR Entry”
			SET (action description) = “Entry Completed”
			SET (entry user) = @user name
		 */
			CString strActionDescription="Entry Completed";
			CString strSrcScreen="CAR Entry";
			InsertIntoTracingTab(strActionDescription,strSrcScreen,strPrmKy);

			//SET (CAR Status = 05)
			FormSetField(strFormTag,"CRENTRY_CRSTT","05");

			//Refresh screen
			DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		}
		////////////////////////////////////////////
	}

	if(strButtonName=="REJECT_CAR")
	{
		if(strFormTag=="HSE_TGCRRVW" ||strFormTag=="HSE_TGACTNSENTRY" || strFormTag=="HSE_TGACTNSRVIW" || strFormTag=="HSE_TGCRFLOUP")
		{
			RejectCARProcess(strFormTag,pCustomButtonClicked);
		}		
	}

	if(strButtonName=="CRENTRY_VWRJCTRSNS")
	{
		if(strFormTag=="HSE_TGCRENTRY")
		{
			viewRejectReason(pCustomButtonClicked);
		}
	}

	if(strButtonName=="ACCEPT_CAR")
	{
		if(strFormTag=="HSE_TGCRRVW")
		{
			AcceptCARProcess(strFormTag,pCustomButtonClicked);			
		}
		if(strFormTag=="HSE_TGACTNSRVIW")
		{
			//////////////////////////////////////////////////
			//RQ-SF-9-1-23_6_1_4 (Accept CAR) Button
			/*
				Count (Corrections) TAB, where (Accepted = No).  If count > zero, 
				then display error message (Please check the corrections TAB) & cancel the user action
			*/
			CString strCheckExistance;
			strCheckExistance.Format("update HSE_ACTNSENTRY_CRR set prmky=prmky where prmky=%s AND isnull(CRRCTVACTNS_LNKCSS_ACC,'') <> 'Y'",strPrmKy);
			long lRecords=0;
			try
			{
				lRecords=ExecuteSQL(strCheckExistance);
				if(lRecords>0)
				{
					AfxMessageBox("Please check the corrections TAB");
				}
				else
				{
						//Count (Root Causes) TAB > (Corrective Actions) popup, where (Accepted = No).   
					//If count > zero, then display error message (Please check the Root Causes TAB & related corrective actions) & cancel the user action
					CString strCheckExistance;
					strCheckExistance.Format("update HSE_CRRCTVACTNS set CRRCTVACTNS_lnkwthmn=CRRCTVACTNS_lnkwthmn where CRRCTVACTNS_acc='N' and CRRCTVACTNS_lnkwthmn in (select autoFieldnum from HSE_ACTNSENTRY_RTCSS where prmky=%s)",strPrmKy);
					long lRecords=0;
					try
					{
						lRecords=ExecuteSQL(strCheckExistance);
						if(lRecords>0)
						{
							AfxMessageBox("Please check the Root Causes TAB & related corrective actions");
						}
					}
					catch(...)
					{
						AfxMessageBox("ERROR EXCECUTING STATEMENT 1453");
					}
					if(lRecords==0)
					{
						/*
						Insert record into (Tracing) Tab
						SET (datetime) = current date & time
						SET (Source Screen) = “Actions Review”
						SET (action description) = “CAR Accepted”
						SET (entry user) = @user name

						SET (CAR Status = 19)
						Update (main > CAR Target Date) 
						Check all records in (Root Causes) TAB > (Corrective Actions) popup, if any record has (Revised Target Date) > (main > CAR Target Date) then SET (main > CAR Target Date) = (Revised Target Date).
						Note, the (main > CAR Target Date) should always hold the biggest (Revised Target Date) value.
						*/
						CString strStatus="CAR Accepted";
						InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,"",strFormTag);
						//SET (CAR Status = 19)
						FormSetField(strFormTag,"CRENTRY_CRSTT","19");
						/*
							Update (main > CAR Target Date) 
								Check all records in (Root Causes) TAB > (Corrective Actions) popup, if any record has 
								(Revised Target Date) > (main > CAR Target Date) then SET (main > CAR Target Date) = (Revised Target Date).
								Note, the (main > CAR Target Date) should always hold the biggest (Revised Target Date) value.
						*/
						CString strRootCausePK=strPrmKy;
						CString strUpdateSql;
						strUpdateSql.Format("with ROOT_CAUSES_TRGTDT as (select max(CRRCTVACTNS_trgDt) RootCauseTrgtdt from HSE_CRRCTVACTNS where CRRCTVACTNS_lnkWthMn in (select Autofieldnum from HSE_ACTNSENTRY_RTCSS where prmky=%s)),MAIN_TRGTDT as (select CRENTRY_CRTRGDT MainTrgtdt from hse_CRENTRY where prmky=%s),JOIN_THE_TWO as(select * from ROOT_CAUSES_TRGTDT,MAIN_TRGTDT) update JOIN_THE_TWO set MainTrgtdt=RootCauseTrgtdt where isnull(MainTrgtdt,0)<RootCauseTrgtdt",strRootCausePK,strRootCausePK);
						ExecuteSQL(strUpdateSql);

						//Refresh screen
						DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
						RefreshScreen("",REFRESH_SELECTED);

					}
				}
			}
			catch(...)
			{
				AfxMessageBox("ERROR EXCECUTING STATEMENT 1453");
			}

		}
	}

	///////////////////////////////////////////
	//RQ_SF_18_3_23_1
	if(strButtonName=="CAR_REVIEW_INFO")
	{
		if(strFormTag=="HSE_TGCRRVW")
		{
			////////////////////////////
			//RQ-2-2024.8
			///////////////////////////////
			//Bugs-2-2024.4
			CString strEnable_CAR_Review_Info="N";
			CString strCARSource= FormGetField("HSE_TGCRRVW","CRENTRY_CRSRC");
			CString strCARBasis=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS");
			if(strCARSource==strCARBasis)
				strEnable_CAR_Review_Info=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENBCRRVWINF");
			/////////////////////
			if (strEnable_CAR_Review_Info=="Y")			
				acceptRecord(strFormTag);
			else 
				AfxMessageBox("According to current System Policy. This function is not allowed");
		}
	}
	///////////////////////////////////////////

	///////////////////////////////////////////
	//RQ_SF_18_3_23_2
	if(strButtonName=="CAR_APPROVAL_INFO")
	{
		if(strFormTag=="HSE_TGCRAPRVL")
		{
			////////////////////////////
			//RQ-2-2024.8
			///////////////////////////////
			//Bugs-2-2024.5
			CString strEnable_CAR_Approval_Info="N";
			CString strCARSource= FormGetField("HSE_TGCRAPRVL","CRENTRY_CRSRC");
			CString strCARBasis=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS");
			if(strCARSource==strCARBasis)
				strEnable_CAR_Approval_Info=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENBCRAPPINF");
			/////////////////////
			if (strEnable_CAR_Approval_Info=="Y")			
				approveCARRecord(strFormTag);
			else 
				AfxMessageBox("According to current System Policy. This function is not allowed");
		}
	}
	///////////////////////////////////////////
	if(strButtonName=="VIEW_CAR_REVIEW_INFO")
	{
		if(strFormTag=="HSE_TGCRAPRVL" || strFormTag=="HSE_TGACTNSENTRY" || strFormTag=="HSE_TGCREDTNG")
		{
			//RQ-SF-9-1-23_4_1_2 (View CAR Review Info) Button
			//RQ-SF-9-1-23_5_1_2 (View CAR Review Info) button
			//RQ_SF_23_5_23_1.9.1
			//Open (CAR Review Info) popup, as specified in ((CAR Review) screen > (Accept CAR)) button & LOCKED
			acceptRecord(strFormTag,true);
		}
	}

	if(strButtonName=="REJECT_CAR")
	{
		if(strFormTag=="HSE_TGCRAPRVL")
		{
			//RQ-SF-9-1-23_4_1_4 (Reject CAR) Button
			//Open (reject reasons) popup screen & allow user to enter at least (1) reason.
			rejectRecord(strFormTag);
			bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
			if(bUpdateRejectedRecord==true)
			{
/*
				Insert record into (Tracing) Tab
				SET (datetime) = current date & time
				SET (Source Screen) = “CAR Approval”
				SET (action description) = “Entry Rejected”
				SET (entry user) = @user name
				Link the tracing record with entered (reject reasons)
*/				
				CString strSourceScreenName = "CAR Approval";
				CString strStatus="Entry Rejected";
				InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
				
				//SET (CAR Status = 06)
				FormSetField(strFormTag,"CRENTRY_CRSTT","06");
				
				//Refresh screen
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
				RefreshScreen("",REFRESH_SELECTED);
			}
			/////////////////////////////////////////////////////
		}
	}

	if(strButtonName=="CANCEL_CAR")
	{
		if(strFormTag=="HSE_TGCRAPRVL")
		{
			//RQ-SF-9-1-23_4_1_3 (Cancel CAR) Button
			//Open (reject reasons) popup screen & allow user to enter at least (1) reason.
			rejectRecord(strFormTag);
			bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
			if(bUpdateRejectedRecord==true)
			{
/*
				Insert record into (Tracing) Tab
				SET (datetime) = current date & time
				SET (Source Screen) = “CAR Approval”
				SET (action description) = “CAR Cancelled”
				SET (entry user) = @user name
				Link the tracing record with entered (reject reasons)
*/				
				CString strSourceScreenName = "CAR Approval";
				CString strStatus="CAR Cancelled";
				InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
				
				//SET (CAR Status = 20)
				FormSetField(strFormTag,"CRENTRY_CRSTT","20");
				
				//Refresh screen
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
				RefreshScreen("",REFRESH_SELECTED);
			}
			/////////////////////////////////////////////////////
		}
	}

	if(strButtonName=="APPROVE_CAR" && strFormTag=="HSE_TGCRAPRVL")
	{
		ApproveCARProcess2(strFormTag, pCustomButtonClicked);
	}
	if(strButtonName=="VIEW_CAR_APPROVE_INFO" && (strFormTag=="HSE_TGACTNSENTRY" || strFormTag=="HSE_TGCREDTNG"))
	{
		ApproveCARProcess(strFormTag, pCustomButtonClicked,true);
	}

	if(strButtonName=="CORRECTIVE_ACTIONS")
	{
		//RQ-SF-9-1-23_5_3_2 (Corrective Actions) Button
		//RQ-SF-9-1-23_6_3 (Root Causes) Tab
		//RQ-SF-9-1-23_9_3 (Root Causes) Tab
		//Open (Corrective Actions) 1-M popup screen :
		CString strKeyField=FormGetField(strSubFormTag,"AutoFieldNum");

		//RQ_SF_23_5_23_1.9.2
		CString strMainCARDate="";
		if(strFormTag=="HSE_TGCREDTNG")
		{
			strMainCARDate=FormGetField(strFormTag,"CrEntry_CrDt");
		}

		////////////////////////////////////////////

		CString strDefValues;
		CString strCriteria;
		strCriteria.Format("SELECT * FROM HSE_CRRCTVACTNS WHERE (CRRCTVACTNS_LNKWTHMN= '%s') ",strKeyField);	
		strDefValues.Format("CRRCTVACTNS_LNKWTHMN|%s",strKeyField);
		m_pCategory->GetHSEObj()->setstrKeyValForPopUpScr("HSE_CRRCTVACTNS",strKeyField);
		if(strFormTag=="HSE_TGCRAPRVL" ||strFormTag=="HSE_TGCREDTNG")
			ShowScreen(0,"HSE_TGCRRCTVACTNS","Corrective Actions",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false,strMainCARDate.GetBuffer(NULL));
		else if(strFormTag=="HSE_TGACTNSRVIW")
			ShowScreen(0,"HSE_TgCrrctvActns_Rvw","Corrective Actions",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);
		else if(strFormTag=="HSE_TGCRFLOUP")
			ShowScreen(0,"HSE_TgCrrctvActnsFlwUp","Corrective Actions",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);	
		///////////////////////////////////////////////
		//BUG_HM_2_8_23.02
		else if(strFormTag=="HSE_TGACTNSENTRY")
			ShowScreen(0,"HSE_TgCrrctvActns_ActEnt","Corrective Actions",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);	
		///////////////////////////////////////////////
		
		///////////////////////////////////////////////
		//RQ_SF_23_5_23_1.9.2
		else if (strFormTag=="HSE_TGACTNSENTRY")
			ShowScreen(0,"HSE_TgCrrctvActns_ActEnt","Corrective Actions",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);	
		///////////////////////////////////////////////
	}

	if(strButtonName=="CLOSE_CAR")
	{
		/*
			RQ-SF-9-1-23_9_1_5 (Close CAR) button
			Data validations
		*/
		CString strCheckExistance;
		strCheckExistance.Format("update HSE_ACTNSENTRY_CRR set prmky=prmky where prmky=%s AND isnull(ACTNSENTRY_CRR_ACTACC,'') <> 'Y'",strPrmKy);
		long lRecords=0;
		try
		{
			//If count ((Corrections) TAB > (Action Accepted = No)) > zero, 
			//then display error message (some “actions” are not accepted, please check the Corrections TAB) & cancel the user action
			lRecords=ExecuteSQL(strCheckExistance);
			if(lRecords>0)
			{
				AfxMessageBox("Some “actions” are not accepted, please check the Corrections TAB");
			}
			else
			{
				strCheckExistance.Format("update HSE_CRRCTVACTNS set CrrctvActns_LnkWthMn=CrrctvActns_LnkWthMn where CrrctvActns_LnkWthMn in (select AutoFieldNum from HSE_ACTNSENTRY_RTCSS where PrmKy=%s ) and isnull(CRRCTVACTNS_ACTACC,'') <>'y'",strPrmKy);
				lRecords=ExecuteSQL(strCheckExistance);
				if(lRecords>0)
				{
					//If count ((Root Causes) TAB > (Corrective Actions) popup > (Action Accepted = No)) > zero, 
					// then display error message (some “actions” are not accepted, please check the Corrective Actions) & cancel the user action
					AfxMessageBox("Some “actions” are not accepted, please check the Corrective Actions");
				}
				else
				{
					CString strCARCloseDate=FormGetField(strFormTag,"CRENTRY_CRCLSDT");
					if(strCARCloseDate=="")
					{
						//If (CAR Closed Date = Null) 
						//then display error message (please enter CAR closed date) & cancel user action
						AfxMessageBox("please enter CAR closed date");
					}
					else
					{
						//SET (CAR Status = 25)
						FormSetField(strFormTag,"CRENTRY_CRSTT","25");
						//Refresh screen
						DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
						RefreshScreen("",REFRESH_SELECTED);
					}
				}
			}
		}
		catch(...)
		{
			AfxMessageBox("ERROR EXCECUTING STATEMENT 1453");
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


void CNewCarEntryCategory::InsertIntoTracingTab(CString strActionDescription,CString strSrcScreen,CString strPrmKy)
{
	CString strUserName = GetUserLogin();
	CString strInsertIntoTracing;
	//The structure is presdefined for all tracing tabs
	strInsertIntoTracing.Format("insert into HSE_CRENTRY_TRC(CRENTRY_TRC_LNK,CRENTRY_TRC_DATTIM,CRENTRY_TRC_ACCDESC,SRCSCRN,CrEntry_Trc_EntUsr) values(%s,GETDATE(),'%s','%s','%s')",strPrmKy,strActionDescription,strSrcScreen,strUserName);
	long lRetFieldsCount = ExecuteSQL(strInsertIntoTracing);
}

void CNewCarEntryCategory::rejectRecord(CString strForm_Tag)
{
	CString strPrimKey=FormGetField(strForm_Tag,"PRMKY");
	setSrcKyFldNm("PRMKY");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGCRRVW")
	{
		Reject("CRCTVEACCENT-RV",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGCRAPRVL")
	{
		Reject("CRCTVEACCENT-AP",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGACTNSENTRY")
	{
		Reject("CRCTVEACCENT-AE",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGACTNSRVIW")
	{
		Reject("ACTION-RV",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGCRFLOUP")
	{
		Reject("CARFLOWUP-RV",strPrimKey);
	}

}
extern bool gEnableScreen=true;
void CNewCarEntryCategory::acceptRecord(CString strForm_Tag,bool bLocked)
{
	CString strPrimKey=FormGetField(strForm_Tag,"PRMKY");
	setSrcKyFldNm("PRMKY");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGCRRVW")
	{
		gEnableScreen=!bLocked;
		Accept("CRCTVEACCENT-RV",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGCRAPRVL")
	{
		gEnableScreen=!bLocked;
		Accept("CRCTVEACCENT-RV",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGACTNSENTRY" || strForm_Tag=="HSE_TGCREDTNG")
	{
		//RQ_SF_23_5_23_1.9.1
		gEnableScreen=!bLocked;
		Accept("",strPrimKey);
	}
	gEnableScreen=true;
}

void CNewCarEntryCategory::approveCARRecord(CString strForm_Tag,bool bLocked)
{
	CString strPrimKey=FormGetField(strForm_Tag,"PRMKY");
	setSrcKyFldNm("PRMKY");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGCRAPRVL" || strForm_Tag=="HSE_TGACTNSENTRY" || strForm_Tag=="HSE_TGCREDTNG")
	{
		//RQ_SF_23_5_23_1.9.1
		gEnableScreen=!bLocked;
		Approve("CRCTVEACCENT-AP",strPrimKey);
	}
	gEnableScreen=true;
}


void CNewCarEntryCategory::Reject(CString strSourceModule,CString strKeyField)
{		
	CString strDefValues;
	CString strCriteria;
	//open reject screen locked
	setgbOpenRejectScreenLocked(false);		
	strCriteria.Format("SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN= '%s') AND (RJCTRSN_MODULETYPE= '%s') AND (RJCTRSN_TRACINGLNK = 0)",strKeyField,strSourceModule);	
	strDefValues.Format("RJCTRSN_LINKWITHMAIN|%s|RJCTRSN_MODULETYPE|%s",strKeyField,strSourceModule);
	m_pCategory->GetHSEObj()->setstrSrcForRjctRsn(strSourceModule);
	m_pCategory->GetHSEObj()->setstrKeyValForRjctScr(strKeyField);
	//Bugs-3-2021.63 screen should open in modal mode
	ShowScreen(0,"HSE_TGRJCTRSN","Reject Reason",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);
}

HRESULT CNewCarEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strFormTag(pScreenReady->Form_Tag);

	if(strFormTag=="HSE_TGACTNSRVIW")
	{
		/*
			RQ_SF_6_3_23_1
			(Main) screen 
			- The default value of (Review Date) should = today.   (The date of opening the review for the first time)
		*/
		CString strReviewData=FormGetField(strFormTag,"CRENTRY_RVWDT"); 
		if(strReviewData=="")
		{
			FormSetField(strFormTag,"CRENTRY_RVWDT",m_pHSEMSPolyDb->GetServerDate()); 
		}
	}

	if(strFormTag=="HSE_TGCRFLOUP")
	{
		/*RQ-3-2023.6
				• (Corrections) Tab
						 o  Disable standard buttons (NEW) & (DELETE)
				• (Root Causes) Tab
						 o  Disable standard buttons (NEW) & (DELETE)
		*/
		EnableToolbar("HSE_ACTNSENTRY_Crr",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_ACTNSENTRY_Crr",TOOLBAR_DELETE,FALSE);
		EnableToolbar("HSE_ACTNSENTRY_RtCss",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_ACTNSENTRY_RtCss",TOOLBAR_DELETE,FALSE);
		////////////////////////////////////////////////////////////////
	}

	return CAuditModuleCategory::DisplayScreenReady(pScreenReady);
}

HRESULT CNewCarEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag=(*pRecordRepostion).Form_Tag;
	CString strSubFormTag=(*pRecordRepostion).SubForm_Tag;

	if(strFormTag=="HSE_TGCRFLOUP")
	{
		/////////////////////////////////////////////////
		//RQ_SF_19_3_23_1
		if (strSubFormTag=="HSE_ACTNSENTRY_CRR")
		{
			CString strActualFollowUpDate=FormGetField(strSubFormTag,"ACTNSENTRY_CRR_ACTFLLDT"); 
			if(strActualFollowUpDate=="")
			{
				FormSetField(strSubFormTag,"ACTNSENTRY_CRR_ACTFLLDT",m_pHSEMSPolyDb->GetServerDate()); 
			}
			
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

void CNewCarEntryCategory::ApproveCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked)
{
	//RQ-SF-9-1-23_5_1_3 (View CAR Approve Info) button
	//opens (CAR Approve Info) popup, as specified above in (CAR Approval) screen > (Approve CAR) button & LOCKED

	//RQ-SF-9-1-23_4_1_5 (Approve CAR) Button
	/*
		Open (CAR Approval Info) 1-1 popup screen (allows to insert only 1 record) :
		Comment : to complete the process, the user MUST enter the (CAR Approval Info) record

	*/
//	Open (CAR Review Info) 1-1 popup screen (allows to insert only 1 record) :

	approveCARRecord(strFormTag,bLocked);
	bool bUpdateApprovedRecord=getgbUpdateApprovedRecord();

	if(bUpdateApprovedRecord==true)
	{
/*
		Insert record into (Tracing) Tab
		SET (datetime) = current date & time
		SET (Source Screen) = “CAR Approval”
		SET (action description) = “CAR Approved”
		SET (entry user) = @user name
		Refresh screen
*/				
		CString strSourceScreenName = "CAR Approval";
		CString strStatus="CAR Approved";
		InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
		
		//SET (CAR Status = 15)
		FormSetField(strFormTag,"CRENTRY_CRSTT","15");
/*
		SET (main) screen > (Concerned Department) = (CAR Approval Info) popup > (Concerned Department)
		SET (main) screen > (Project) = (CAR Approval Info) popup > (Related Project)
*/				
		FormSetField(strFormTag,"CRENTRY_CNCDPR",gstrCncrnDprt);
		FormSetField(strFormTag,"CRENTRY_PRJ",gstrProject);
		//Refresh screen
		DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		RefreshScreen("",REFRESH_SELECTED);
	}

}

void CNewCarEntryCategory::ApproveCARProcess2(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked)
{
	//RQ-SF-9-1-23_5_1_3 (View CAR Approve Info) button
	//opens (CAR Approve Info) popup, as specified above in (CAR Approval) screen > (Approve CAR) button & LOCKED

	//RQ-SF-9-1-23_4_1_5 (Approve CAR) Button
	/*
		Open (CAR Approval Info) 1-1 popup screen (allows to insert only 1 record) :
		Comment : to complete the process, the user MUST enter the (CAR Approval Info) record

	*/
//	Open (CAR Review Info) 1-1 popup screen (allows to insert only 1 record) :

	//RQ_SF_18_3_23_2: Commented the following 2 lines 
	//approveCARRecord(strFormTag,bLocked);
	//bool bUpdateApprovedRecord=getgbUpdateApprovedRecord();

	bool bUpdateApprovedRecord=true;
	//RQ-2-2024.8
	///////////////////////////////
	//Bugs-2-2024.5
	CString strEnable_CAR_Approval_Info="N";
	CString strCARSource= FormGetField("HSE_TGCRAPRVL","CRENTRY_CRSRC");
	CString strCARBasis=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS");
	if(strCARSource==strCARBasis)
		strEnable_CAR_Approval_Info=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENBCRAPPINF");
	/////////////////////
	if (strEnable_CAR_Approval_Info=="Y")			
	{
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		CString strSql;
		strSql.Format("update HSE_CRAPPRVLINF set CRAPPRVLINF_LnkWthMn=CRAPPRVLINF_LnkWthMn where CRAPPRVLINF_LnkWthMn=%s",KeyFieldValue);
		long lRetFieldsCount = ExecuteSQL(strSql);
		bUpdateApprovedRecord=(lRetFieldsCount ==1);
	}
	
	if(bUpdateApprovedRecord==true)
	{
/*
		Insert record into (Tracing) Tab
		SET (datetime) = current date & time
		SET (Source Screen) = “CAR Approval”
		SET (action description) = “CAR Approved”
		SET (entry user) = @user name
		Refresh screen
*/				
		CString strSourceScreenName = "CAR Approval";
		CString strStatus="CAR Approved";
		InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
		
		//SET (CAR Status = 15)
		FormSetField(strFormTag,"CRENTRY_CRSTT","15");
/*
		SET (main) screen > (Concerned Department) = (CAR Approval Info) popup > (Concerned Department)
		SET (main) screen > (Project) = (CAR Approval Info) popup > (Related Project)
*/				
		FormSetField(strFormTag,"CRENTRY_CNCDPR",gstrCncrnDprt);
		FormSetField(strFormTag,"CRENTRY_PRJ",gstrProject);
		//Refresh screen
		DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		RefreshScreen("",REFRESH_SELECTED);
	}
	else
	{
		//RQ_SF_18_3_23_2
		//the (Accept CAR) button should not act if the record is not inserted in (CAR Review Info) popup
		AfxMessageBox("Please insert a CAR Approval Information");
	}

}

void CNewCarEntryCategory::RejectCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked)
{
	/////////////////////////////////////////////////////
	//RQ-SF-9-1-23_3_1_3 (Reject CAR) Button
	//RQ-SF-9-1-23_5_1_4 (Reject CAR) button
	//RQ-SF-9-1-23_6_1_3 (Reject CAR) button
	//RQ-SF-9-1-23_9_1_4 (Reject CAR) button
	//Open (reject reasons) 1-1 popup screen & allow user to enter (1) reason at least.
	rejectRecord(strFormTag);
	bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
	if(bUpdateRejectedRecord==true)
	{
/*
		Insert record into (Tracing) Tab
		Set (datetime) = current date & time
		Set (Source Screen) = “CAR Review” or " “Actions Review”" according to screen
		Set (action description) = “Entry Rejected” or "CAR Rejected" according to screen
		Set (entry user) = @user name
		Link the tracing record with entered (reject reasons)
*/				
		CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");
		CString strStatus="Entry Rejected";
		if(strFormTag=="HSE_TGACTNSRVIW" || strFormTag=="HSE_TGCRFLOUP")
			strStatus="CAR Rejected";
		InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);

		/*CString strSQL;
		CString strUserName = GetUserLogin();
		CString strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		//'HSE_CRENTRY_' will be changed to 'HSE_CRENTRY_TRC' in the procedure
		CString strAcionDesc="Entry Rejected";
		if(strFormTag=="HSE_TGACTNSRVIW" || strFormTag=="HSE_TGCRFLOUP")
			strAcionDesc="CAR Rejected";
		strSQL.Format("EXECUTE ChaneEntityStatus '%s','%s','%s','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','02','%s'",KeyFieldValue,strSourceScreenName,strUserName,strAcionDesc);
		ExecuteSQL(strSQL);		
		*/
		
		if(strFormTag=="HSE_TGCRRVW")
			//SET (CAR Status = 02)
			FormSetField(strFormTag,"CRENTRY_CRSTT","02");
		else if(strFormTag=="HSE_TGACTNSENTRY") //SET (CAR Status = 11)
			FormSetField(strFormTag,"CRENTRY_CRSTT","11");
		else if(strFormTag=="HSE_TGACTNSRVIW") //SET (CAR Status = 12)
			FormSetField(strFormTag,"CRENTRY_CRSTT","12");
		else if(strFormTag=="HSE_TGCRFLOUP") //SET (CAR Status = 15) RQ-SF-9-1-23_9_1_4 (Reject CAR) button
			FormSetField(strFormTag,"CRENTRY_CRSTT","15");
		
		//Refresh screen
		DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		RefreshScreen("",REFRESH_SELECTED);
	}
	/////////////////////////////////////////////////////
}

void CNewCarEntryCategory::AcceptCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked)
{
	
	//RQ-SF-9-1-23_3_1_4 (Accept CAR) Button
	/*
		Comment : to complete the process, the user MUST enter the (CAR Review Info) record

	*/
//				Open (CAR Review Info) 1-1 popup screen (allows to insert only 1 record) :

	//RQ_SF_18_3_23_1: Commented the following 2 lines 
	//acceptRecord(strFormTag);
	//bool bUpdateAcceptedRecord=getgbUpdateAcceptedRecord();

	bool bUpdateAcceptedRecord=true;
	//RQ-2-2024.8
	///////////////////////////////
	//Bugs-2-2024.4
	CString strEnable_CAR_Review_Info="N";
	CString strCARSource= FormGetField("HSE_TGCRRVW","CRENTRY_CRSRC");
	CString strCARBasis=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS");
	if(strCARSource==strCARBasis)
		strEnable_CAR_Review_Info=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENBCRRVWINF");
	/////////////////////
	if (strEnable_CAR_Review_Info=="Y")			
	{
		//RQ_SF_18_3_23_1
		//the (Accept CAR) button should not act if the record is not inserted in (CAR Review Info) popup
		CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
		CString strSql;
		strSql.Format("update HSE_CRRVWINF set CrRvwInf_LnkWthMn=CrRvwInf_LnkWthMn where CrRvwInf_LnkWthMn=%s",KeyFieldValue);
		long lRetFieldsCount = ExecuteSQL(strSql);
		bUpdateAcceptedRecord=(lRetFieldsCount ==1);
	}
	if(bUpdateAcceptedRecord==true)
	{
/*
		Insert record into (Tracing) Tab
		SET (datetime) = current date & time
		SET (Source Screen) = “CAR Review”
		SET (action description) = “CAR Accepted”
		SET (entry user) = @user name
*/				
		CString strSourceScreenName = "CAR Review";
		CString strStatus="CAR Accepted";
		InsertIntoTracingTabProcess(pCustomButtonClicked,strStatus,strSourceScreenName);
		
		//SET (CAR Status = 10)
		FormSetField(strFormTag,"CRENTRY_CRSTT","10");
		
		//Refresh screen
		DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
		RefreshScreen("",REFRESH_SELECTED);
	}
	else
	{
		//RQ_SF_18_3_23_1
		//the (Accept CAR) button should not act if the record is not inserted in (CAR Review Info) popup
		AfxMessageBox("Please insert a CAR Review Information");
	}

}

void CNewCarEntryCategory::InsertIntoTracingTabProcess(tag_CustomButtonClicked* pCustomButtonClicked,CString strStatus,CString strSourceScreenName,CString strFormTag)
{
	CString strSQL;
	CString strUserName = GetUserLogin();
	if(strSourceScreenName=="")
		strSourceScreenName = GetScrCptnByTag(66,strFormTag,"");
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	//'HSE_CRENTRY_' will be changed to 'HSE_CRENTRY_TRC' in the procedure
	strSQL.Format("EXECUTE ChangeEntityStatus '%s','%s','%s','HSE_CRENTRY_','CRENTRY_CRSTT','PRMKEY','02','%s'",KeyFieldValue,strSourceScreenName,strUserName,strStatus);
	ExecuteSQL(strSQL);
}

void CNewCarEntryCategory::UpdateYearAndSerial(CString strFormTag,CString strYear)
{
	CString strNewSrl;
	FormSetField("HSE_CRENTRY","CRENTRY_CRYR",strYear);
	if(m_strMainSerialDependentKey=="")
		strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,"","");
	else
	{
		CString strKeyFieldValue=FormGetField(strFormTag,m_strMainSerialDependentKey); 
		strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,m_strMainSerialDependentKey,strKeyFieldValue);
	}
	FormSetField(strFormTag,m_strSerialFieldName,strNewSrl);
}