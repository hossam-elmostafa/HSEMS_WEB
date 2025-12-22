#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnAprvlCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AwrnsPlnAprvlCategory::AwrnsPlnAprvlCategory(CCategory *pCategory):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGAWRNSPLNAPRVL";
	m_strScreen			= "Awareness Plan Approval";
}

AwrnsPlnAprvlCategory::~AwrnsPlnAprvlCategory() {}

long AwrnsPlnAprvlCategory::ShowAwrnsPlnAprvl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AwrnsPlnAprvl;
	return ParentManage("HSE_TgAwrnsPlnAprvl",TRUE,TRUE,TRUE,"HSE_AwrnsPln",strCriteria);
}

HRESULT AwrnsPlnAprvlCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strButtonName=="AWRNSPLNAPRVL_PLNENTRYAPRVD") {
		//////////////////////////////////////////////////////////////////////////////////////////////
		//Bugs-10-2021.11
		//CString strAwrnsPlnYr = FormGetField("HSE_AWRNSPLN","AWRNSPLN_YR");
		CString strAwrnsSrl = FormGetField("HSE_AWRNSPLN","SerialPK");
		approveAwrnsPln(strAwrnsSrl);					
		//////////////////////////////////////////////////////////////////////////////////////////////
	} else if (strButtonName=="AWRNSPLNAPRVL_PLNENTRYRJCTD") {
		if(OpenReasonsScr("Reject Reasons","3",true))
		{
			//////////////////////////////////////////////////////////////////////////////////////////////
			//Bugs-10-2021.11
			//CString strAwrnsPlnYr = FormGetField("HSE_AWRNSPLN","AWRNSPLN_YR");
			CString strAwrnsSrl = FormGetField("HSE_AWRNSPLN","SerialPK");
			CString strCallerIdFinal = m_strFinal;
			rjctAwrnsPln(strAwrnsSrl,strCallerIdFinal);
		}
	}
	return AwrnsPlnCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AwrnsPlnAprvlCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

void AwrnsPlnAprvlCategory::approveAwrnsPln(CString strAwrnsPlnYr) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE AwrnsPlnApproved %s",strAwrnsPlnYr);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void AwrnsPlnAprvlCategory::rjctAwrnsPln(CString strAwrnsPlnYr,CString strRjctRsn) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE AwrnsPlnRejected %s,'%s'",strAwrnsPlnYr,strRjctRsn);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}