// NewCarEntryCategory.cpp: implementation of the CCorrectiveActionEntry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CorrectiveActionEntry.h"

#include "HSEMS.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//RQ-SF-9-1-23_5_3_2_1 (Corrective Actions) popup


CCorrectiveActionEntry::CCorrectiveActionEntry(CCategory *pCategory,CMapStringToOb* pmpListOfParameters,CString strWhereCriteria):CAuditModuleCategory(pCategory,pmpListOfParameters)
{
	m_strParentKey="";
	if(strWhereCriteria!="")
	{
		//{"SELECT * FROM HSE_CRRCTVACTNS WHERE (CRRCTVACTNS_LNKWTHMN= '23') "}
		m_strMainSerialDependentKey.MakeUpper();
		char *str=strstr(strWhereCriteria,m_strMainSerialDependentKey);
		if(str!=NULL)
		{
			m_strParentKey=(str+m_strMainSerialDependentKey.GetLength()+1);
			m_strParentKey.Replace(")","");
			m_strParentKey.Replace("'","");
			m_strParentKey.Replace(" ","");
		}
	}
}

CCorrectiveActionEntry::~CCorrectiveActionEntry()
{

}

HRESULT CCorrectiveActionEntry::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFormTag(pFieldChange->Form_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	CString strNewVal(pFieldChange->New_Value);

	if(strFormTag=="HSE_TGCRRCTVACTNS_RVW" && strFieldName=="CRRCTVACTNS_ACC")
	{
		////////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_6_3 (Root Causes) Tab
		//If (Accepted = Yes) then SET (Action Details) > (Action Status = 3) i.e “accepted by review”
		//If (Accepted = No) then SET (Action Details) > (Action Status = 2) i.e “rejected by review”
		if(strNewVal=="Y")
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","3");
		else
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","2");
	}
	
	if(strFormTag=="HSE_TGCRRCTVACTNSFLWUP" && strFieldName=="CRRCTVACTNS_ACTACC")
	{
		////////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_9_3 (Root Causes) Tab
		//If (Action Accepted = No) then SET (Action Status = 7) 
		//If (Action Accepted = Yes) then SET (Action Status = 9) 
		if(strNewVal=="Y")
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","9");
		else
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","7");
	}

	/////////////////////////////////////////////////////////
	//RQ_SF_23_5_23_1.9.2
	if(strFormTag=="HSE_TGCRRCTVACTNS" && strFieldName=="CRRCTVACTNS_RVSTRGDT" && strNewVal!="")
	{
		COleDateTime OleDate;
		OleDate.ParseDateTime(strNewVal);
		
		COleDateTime OleCarDate;
		extern CString gstrCARDate;
		OleCarDate.ParseDateTime(gstrCARDate);

		if (OleDate<OleCarDate)
		{
			CString strMsg;
			strMsg.Format("Entered date must be >= CAR Date :[%s]",gstrCARDate);
			AfxMessageBox(strMsg);
			FormSetField(strFormTag,strFieldName,"");
			return FALSE;
		}	
	}
	/////////////////////////////////////////////////////////

	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT CCorrectiveActionEntry::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strButtonName=(*pToolBarButtonClicked).Button_Name;
	CString strFormTag=(*pToolBarButtonClicked).Form_Tag;
	CString strSubFormTag=(*pToolBarButtonClicked).SubForm_Tag;
	if(strButtonName=="NEW" && (*pToolBarButtonClicked).iComplete==1)
	{
		if(strFormTag=="HSE_TGCRRCTVACTNS" ||  strFormTag=="HSE_TGCRRCTVACTNS_ACTENT") //BUG_HM_2_8_23.02
		{
			AddNewSerial(strFormTag);
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

extern CString gstrCncrnDprt;
extern CString gstrProject;

HRESULT CCorrectiveActionEntry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName=(*pCustomButtonClicked).Button_Name;
	CString strFormTag=(*pCustomButtonClicked).Form_Tag;
	if(pCustomButtonClicked->nRecordStatus==1)
	{
		
		if(strButtonName=="NEW")
		{
			if(strFormTag=="HSE_TGCRRCTVACTNS")
			{
				CString strKeyFieldValue=FormGetField(strFormTag,m_strMainSerialDependentKey); 
				CString strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,m_strMainSerialDependentKey,m_strParentKey);
				FormSetField(strFormTag,m_strMainSerialDependentKey,strNewSrl);
			}
		}
	}
	else
	{
		if(strButtonName=="VIEW_SOURCE_CAR")
		{
			//RQ-SF-9-1-23_7_3 (View Source CAR) button
			//RQ-SF-9-1-23_8_1_3  (View Source CAR) button
			//Open (Actions Review) screen, fully LOCKED, with reference to related (CAR Year) & (CAR No.)
			CString strCarYear=FormGetField(strFormTag,"CrEntry_CrYr");
			CString strCarNo=FormGetField(strFormTag,"CRENTRY_CRN");
			m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);
			m_strScreenCaption="";
			ShowMenuScreenV2("",33170,"",NULL);
			///////////////////////////////////////////////////////
		}
		else if(strButtonName=="REJECT_ACTION")
		{
			////////////////////////////////////////////////
			//RQ_HM_17_8_23.01.07.02.01
			CString strRejectReason=FormGetField(strFormTag,"CRRCTVACTNS_RJCRSN");
			if(strRejectReason=="")
			{
				AfxMessageBox("Please enter reject reason");
			}
			else
			////////////////////////////////////////////////
			{

				//RQ-SF-9-1-23_7_4 (Reject Action) button
				/*
					ENABLE (Reject Reason) text, size=500, MUST
					SET (Action Status = 4)
					Refresh screen
				*/
				FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","4");
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");			
			}
		}
		else if (strButtonName=="ACCEPT_ACTION")
		{
			//RQ-SF-9-1-23_7_5 (Accept Action) button
			/*
				SET (Action Status = 5)
				SET (Accepted Date) = today
				Refresh screen
			*/
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","5");
			FormSetField(strFormTag,"CRRCTVACTNS_ACCDT",GetFullDateTime("%d/%m/%Y"));

			DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");			
		}
		else if (strButtonName=="KEEP_ACTION_PENDING")
		{
			/*
			RQ-SF-9-1-23_8_1_4 (Keep Action Pending) button
			(Keep Action Pending) button
			If (Pending Reason = Null) then display error message (Please enter pending reason) & cancel user action
			SET (Action Status = 6)
			*/
			CString strPendingReason=FormGetField(strFormTag,"CRRCTVACTNS_PndRsn");
			if(strPendingReason=="")
			{
				AfxMessageBox("Please enter pending reason");
			}
			else
			{
				FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","6");
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");			
			}
		}
		else if (strButtonName=="ACTION_COMPLETED")
		{
			/*
				RQ-SF-9-1-23_8_1_5 (Action Completed) button
				(Action Completed) button
				Validations
				If (Completion Date = Null) then display message (enter completion date) & cancel user action
				If (Executed By = Null) then display message (enter executed by) & cancel user action
				SET (Action Status = 8)
				Refresh screen
			*/
			CString strCompletionDate=FormGetField(strFormTag,"CRRCTVACTNS_CMPDT");
			if(strCompletionDate=="")
			{
				AfxMessageBox("Please enter Completion Date");
			}
			else
			{
				CString strExcBy=FormGetField(strFormTag,"CRRCTVACTNS_EXCBY");
				if(strExcBy=="")
				{
					AfxMessageBox("Please enter Executed By");
				}
				else
				{
					FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","8");
					DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");			
				}
			}

		}
		/////////////////////////////////////////////////////////////////////////
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


void CCorrectiveActionEntry::InsertIntoTracingTab(CString strActionDescription,CString strSrcScreen,CString strPrmKy)
{
	CString strUserName = GetUserLogin();
	CString strInsertIntoTracing;
	//The structure is presdefined for all tracing tabs
	//strInsertIntoTracing.Format("insert into HSE_CRENTRY_TRC(CRENTRY_TRC_LNK,CRENTRY_TRC_DATTIM,CRENTRY_TRC_ACCDESC,SRCSCRN,CrEntry_Trc_EntUsr) values(%s,GETDATE(),'%s','%s','%s')",strPrmKy,strActionDescription,strSrcScreen,strUserName);
	//long lRetFieldsCount = ExecuteSQL(strInsertIntoTracing);
}

//extern bool gEnableScreen=true;
void CCorrectiveActionEntry::acceptRecord(CString strForm_Tag,bool bLocked)
{
	CString strPrimKey=FormGetField(strForm_Tag,"PRMKY");
	setSrcKyFldNm("PRMKY");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGCRRVW")
	{
		//gEnableScreen=!bLocked;
		Accept("CRCTVEACCENT-RV",strPrimKey);
	}
	if(strForm_Tag=="HSE_TGCRAPRVL")
	{
		//gEnableScreen=!bLocked;
		Accept("CRCTVEACCENT-AP",strPrimKey);
	}
	//gEnableScreen=true;
}

void CCorrectiveActionEntry::Reject(CString strSourceModule,CString strKeyField)
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

HRESULT CCorrectiveActionEntry::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strFormTag(pScreenReady->Form_Tag);
	if(strFormTag=="HSE_TGCRRCTVACTNS")
	{
		CString strSrl=FormGetField(strFormTag,m_strSerialFieldName); 
		if(strSrl=="")
		{
			AddNewSerial(strFormTag);
		}
	}

	if(strFormTag=="HSE_TGCRRCTVACTNS_RVW")
	{
		////////////////////////////////////////////////////////////
		//BUG_HM_16_3_23_1 -- it was working only with the field change
		CString strVal=FormGetField(strFormTag,"CRRCTVACTNS_ACC");
		if(strVal=="Y")
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","3");
		else
			FormSetField(strFormTag,"CRRCTVACTNS_ACTSTT","2");
		////////////////////////////////////////////////////////////
	}
	if(strFormTag=="HSE_TGCRRCTVACTNSFLWUP")
	{
		/////////////////////////////////////////////////
		//RQ_SF_19_3_23_2
		//BUG_SF_20_3_23_3
		CString strActualFollowUpDate=FormGetField(strFormTag,"CRRCTVACTNS_ACTFLLDT"); 
		if(strActualFollowUpDate=="")
		{
			FormSetField(strFormTag,"CRRCTVACTNS_ACTFLLDT",m_pHSEMSPolyDb->GetServerDate()); 
		}
		
		////////////////
	}

	return CAuditModuleCategory::DisplayScreenReady(pScreenReady);
}

HRESULT CCorrectiveActionEntry::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag=(*pRecordRepostion).Form_Tag;
	CString strSubFormTag=(*pRecordRepostion).SubForm_Tag;

	if(strFormTag=="HSE_TGCRRCTVACTNSFLWUP")
	{
		/////////////////////////////////////////////////
		//RQ_SF_19_3_23_2
		//BUG_SF_20_3_23_3
		CString strActualFollowUpDate=FormGetField(strFormTag,"CRRCTVACTNS_ACTFLLDT"); 
		if(strActualFollowUpDate=="")
		{
			FormSetField(strFormTag,"CRRCTVACTNS_ACTFLLDT",m_pHSEMSPolyDb->GetServerDate()); 
		}
		
		////////////////
	}

	return CAuditModuleCategory::DisplayRecordRepostion(pRecordRepostion);
}

void CCorrectiveActionEntry::AddNewSerial(CString strFormTag)
{
	////////////////////////////////////////////////////////////////////////
	//RQ-SF-9-1-23_5_3_2_1
	//(Serial) auto serial No., LOCKED, incremented when click NEW record	
	CString strKeyFieldValue=FormGetField(strFormTag,m_strMainSerialDependentKey); 
	CString strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,m_strMainSerialDependentKey,m_strParentKey);
	FormSetField(strFormTag,m_strSerialFieldName,strNewSrl);
	FormSetField(strFormTag,m_strMainSerialDependentKey,m_strParentKey);
	////////////////////////////////////////////////////////////////////////
}