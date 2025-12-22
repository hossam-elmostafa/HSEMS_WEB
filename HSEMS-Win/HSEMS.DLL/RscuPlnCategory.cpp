#include "stdafx.h"
#include "HSEMS.h"
#include "RscuPlnCategory.h"
#include "HSEobj.h"

RscuPlnCategory::RscuPlnCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
}

RscuPlnCategory::~RscuPlnCategory(){}


long RscuPlnCategory::ShowRscuPlnEntry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_RscuPlnEntry;
	return ParentManage("HSE_TGRSCUPLNENTRY",TRUE,TRUE,TRUE,"HSE_RSCUPLN",strCriteria);	
}

//Rescue Inquery screen
long RscuPlnCategory::ShowRscuPlnInqury(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_RscuPlnInqury;
	return ParentManage("HSE_TGRSCUPLNINQURY",TRUE,FALSE,FALSE,"HSE_RSCUPLN",strCriteria);	
}

HRESULT RscuPlnCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1)
	{	
		if(strSubFormTag == "") {
			if(strBtnName=="NEW") {
				CString strNewSrl = getNxtSrl("RSCUPLN_PLNN","HSE_RSCUPLN","RSCUPLN_PLNYR",FormGetField("HSE_RSCUPLN","RSCUPLN_PLNYR"));
				FormSetField("HSE_RSCUPLN","RSCUPLN_PLNN",strNewSrl);
			}
			else if (strBtnName=="SAVE") {		
				
			}
		} else if (strSubFormTag == "HSE_TGRSCUPLNENTRY_PTNTLHZRDS") {
			if(strBtnName=="NEW") {
				CString strNewSrl = getNxtSrl("RSCUPLN_PTNTLHZRDS_SRLN","HSE_RSCUPLN_PTNTLHZRDS","RSCUPLN_PTNTLHZRDS_LNK",FormGetField("HSE_RSCUPLN","RSCUPLN_PRMRYKY"));
				FormSetField("HSE_RSCUPLN_PTNTLHZRDS","RSCUPLN_PTNTLHZRDS_SRLN",strNewSrl);
			}
		} else if (strSubFormTag == "HSE_TGRSCUPLNENTRY_ATTCHMNTS") {
			if(strBtnName=="NEW") {
				CString strNewSrl = getNxtSrl("RSCUPLN_ATTCHMNTS_LNN","HSE_RSCUPLN_ATTCHMNTS","RSCUPLN_ATTCHMNTS_LNK",FormGetField("HSE_RSCUPLN","RSCUPLN_PRMRYKY"));
				FormSetField("HSE_RSCUPLN_ATTCHMNTS","RSCUPLN_ATTCHMNTS_LNN",strNewSrl);
			}
		} else if (strSubFormTag == "HSE_TGRSCUPLNENTRY_RQURDTLS") {
			if(strBtnName=="NEW") {
				CString strNewSrl = getNxtSrl("RSCUPLN_RQURDTLS_SRLN","HSE_RSCUPLN_RQURDTLS","RSCUPLN_RQURDTLS_LNK",FormGetField("HSE_RSCUPLN","RSCUPLN_PRMRYKY"));
				FormSetField("HSE_RSCUPLN_RQURDTLS","RSCUPLN_RQURDTLS_SRLN",strNewSrl);
			}
		} else if (strSubFormTag == "HSE_TGRSCUPLNENTRY_CMMUNCTNMTHDS") {
			if(strBtnName=="NEW") {
				CString strNewSrl = getNxtSrl("RSCUPLN_CMMUNCTNMTHDS_SRLN","HSE_RSCUPLN_CMMUNCTNMTHDS","RSCUPLN_CMMUNCTNMTHDS_LNK",FormGetField("HSE_RSCUPLN","RSCUPLN_PRMRYKY"));
				FormSetField("HSE_RSCUPLN_CMMUNCTNMTHDS","RSCUPLN_CMMUNCTNMTHDS_SRLN",strNewSrl);
			}
		}
	}	
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT RscuPlnCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	m_FSATableName = strFormTag;
	int Count = 0;
	CString strRscuPlanKey = FormGetField("HSE_RSCUPLN","RSCUPLN_PRMRYKY");
	if(strButtonName == "RSCUPLN_PLNCNCLD") {	//Plan Cancelled
		cancelRscuPlan(strRscuPlanKey);	
	} else if(strButtonName=="RSCUPLN_PLNCLSD") {	//Plan Closed	
		closeRscuPlan(strRscuPlanKey);		
	} else if (strButtonName=="RSCUPLNINQURY_RTRNPLNTOENTRY") {
		rtrnRscuPlnToEntry(strRscuPlanKey);
	} 	
	else if (strButtonName=="RSCUPLN_TRCNG_VWCNCLRSN") {		
		CString strKeyField=FormGetField("HSE_RSCUPLN_TRCNG","RSCUPLN_TRCNG_REFCLOSRSN");
		if(strKeyField != "") {
			m_strScreenTag = "CMS_RSNS";	
			CString strSQL="";
			CString strDefValues;
			bool bLocked = true;
			strSQL.Format("SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '%s')",strKeyField);	
			strDefValues.Format("RSNS_RFREC|%s",strKeyField);
			ShowScreen(0,"CMS_RSNS","Cancel Reasons",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

// Enable (cancel reason)
// Enter (cancel reason) must, text, size=500
// Set (plan status = '5')  >>> plan cancelled
// Set (@T) = 'Plan is cancelled' & insert record into (tracing) tab
// Refresh main screen
void RscuPlnCategory::cancelRscuPlan(CString strRscuPlanKey)
{			
	if(OpenReasonsScr("Cancel Reasons","2",true))
	{
		CString strCallerIdFinal = m_strFinal;
		CString strSQL;	
		strSQL.Format("EXECUTE cancelRscuPlan '%s','%s'",strRscuPlanKey,strCallerIdFinal);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

// Set (@N) = count (rescue team) TAB
// If (@N) <> (main\No. rescue team members) then display error message "Sorry (rescue team members) doesn’t match" & cancel the action
// Set (plan status = '9')  >>> plan closed
// Set (@T) = 'Plan is closed' & insert record into (tracing) tab
// Refresh main screen
void RscuPlnCategory::closeRscuPlan(CString strRscuPlanKey)
{		
	CString strSQL;	
	strSQL.Format("EXECUTE closeRscuPlan '%s'",strRscuPlanKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);	
}



//This button is enabled only if (plan status) >= "5"
//Display "Are you sure to return (rescue plan) to entry ? (Yes/No)"
//If user reply = "No" then cancel the action
//Set (plan status = "2")  >>> returned from inquiry
//Set (@T) = "Plan is returned from inquiry" & insert record into (tracing) tab
//Refresh main screen
void RscuPlnCategory::rtrnRscuPlnToEntry(CString strRscuPlanKey)
{		
	int nRscuPlnStts = atoi(FormGetField("HSE_RSCUPLN","RSCUPLN_PLNSTTUS"));
	if(nRscuPlnStts >= 5) {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to return (rescue plan) to entry ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			CString strSQL;	
			strSQL.Format("EXECUTE rtrnRscuPlnToEntry '%s'",strRscuPlanKey);
			ExecuteSQL(strSQL);
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
}
