// AuditPlanChckLstFdbck.cpp: implementation of the AuditPlanChckLstFdbck class.
//
//	Imp Note : this class will handle audit Plan follow-up screen
//	Screen ID 460~7
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuditPlanChckLstFdbck.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanChckLstFdbck::AuditPlanChckLstFdbck(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanChckLstFdbck::~AuditPlanChckLstFdbck(){}

bool ChkLstRsul = FALSE;

HRESULT AuditPlanChckLstFdbck::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);	
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strBtnName.MakeUpper();		
	CString strAdtyr=FormGetField("HSE_ADTPLNENT","MAINLINK");
	
	if(strBtnName=="ADTPLNENTACTVT_ADTCHKLSTFDBCK")
	{
		ChkLstRsul = openAuditChecklistResult(strFormTag);
	}

	//HSE_CR09_17 open actual auditors screen
	else if(strBtnName == "AUDITORS"){
		OpenAuditorsScreen(strFormTag,2);
	}
	
	//HSE_CR09_17 open actual audities screen
	else if(strBtnName == "AUDITIES"){
		OpenAuditeesScreen(strFormTag,2);
	}
	
	//HSE_CR09_17 close activity
	else if(strBtnName == "ACTVITYCLSD"){
		closeActivity(pCustomButtonClicked);
	}

	/*else if(strBtnName=="CLSSNDCYCLE")
	{
		if(getRecordStatus() == "8")
		{
			CString KeyFieldValue = GetKeyField(strFormTag);
			if(KeyFieldValue != "")
			{
				CloseAuditWithAdtChkLstFdbck(pCustomButtonClicked);
				RefreshScreen("" ,REFRESH_SELECTED);
			}
		}
	}*/
	/***********************************************************
	 *	Ref : HSE_CR11_17
	 *	Commented by a.ali
	 *	Date: 29-05-2013
	 *	Ref : HSE_CR12v2_99
	 *	
	 *	else if(strBtnName == "ADTPLNENTACTVT_RQRDACTNBTN"){
	 *		CString KeyFieldValue = GetKeyField(strFormTag);
	 *		if(KeyFieldValue != "")
	 *			{
	 *			OpenRequiredActionScreen(strFormTag,0);
	 *		}
	 *	}
	 ************************************************************/

	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/*Deleted Code ,Please Check Source Safe*/

HRESULT AuditPlanChckLstFdbck::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strForm_Tag(pRecordRepostion->Form_Tag);
	CString strSubForm_Tag(pRecordRepostion->SubForm_Tag);
	CString ActlToDate = FormGetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTLFRMDATES");
	/***************************************************************************************************
	 * Commented by a.ali
	 * 19-06-2013
	 * No ref to this code in specs
	if(ActlToDate =="")
	{
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLFRMDATES","ALWAYSLOCKED");
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLTODATE","ALWAYSLOCKED");
	}
	else
	{
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLFRMDATES","MUST");
		SetFieldVisibleAttrib("HSE_TGADTPLNCHKLSTFDBCKACTV","ADTPLNENTACTVT_ACTLTODATE","MUST");
	}
	****************************************************************************************************/
	if(strSubForm_Tag == "HSE_TGADTPLNCHKLSTFDBCKACTV") {
		EnableToolbar(strSubForm_Tag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubForm_Tag,TOOLBAR_DELETE,FALSE);
		EnableToolbar(strSubForm_Tag,TOOLBAR_COPY,FALSE);
	}
	return AuditCategory::DisplayRecordRepostion(pRecordRepostion);
}

void AuditPlanChckLstFdbck::closeActivity(tag_CustomButtonClicked* pCustomButtonClicked){
	CString strKeyFieldArray;
	CString strLinkedField;
	bool bPendingRqrdActnsExist = false;
	for(int nSel = 0 ;nSel < pCustomButtonClicked->pMultiSelectedRows->lCount;nSel++)
	{
		CString strRec= pCustomButtonClicked->pMultiSelectedRows->pRow[nSel].strFullRecord;   
		CString strTemp = strRec;
		//below code to solve problem of sending main record and detail record with the same field name MAINLINK 
		strTemp = strRec.Mid(strRec.Find("MAINLINK")+CString("MAINLINK").GetLength());
		CString strKeyField	= GetFieldValue(strTemp ,"MAINLINK");
		strLinkedField = GetFieldValue(strRec , "ADTPLNENTACTVT_LNK");
		strKeyFieldArray += strKeyField;
		strKeyFieldArray += ",";
	}
	strKeyFieldArray.TrimRight(",");
	strKeyFieldArray.TrimLeft(",");
	if(strKeyFieldArray.IsEmpty())	
		return;	
	//TODO : Check if more than one resource is closed
	CString strAuditKey = FormGetField("HSE_ADTPLNENT","MAINLINK");
	bPendingRqrdActnsExist = hasPendingRequiredActions("HSE_ADTACTVTRQRDACTN",strAuditKey);
	if(!bPendingRqrdActnsExist) {
		CString strSQL = "EXECUTE sp_close_activity '"+strKeyFieldArray+"','"+strLinkedField+"'";
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}

HRESULT AuditPlanChckLstFdbck::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return AuditCategory::DisplayScreenReady(pScreenReady);
}

long AuditPlanChckLstFdbck::ShowAuditPlanAuditchecklistfeedback(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNCHKLSTFDBCK;
	return ParentManage("HSE_TGADTPLNCHKLSTFDBCK",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}


