#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnExctnCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AwrnsPlnExctnCategory::AwrnsPlnExctnCategory(CCategory *pCategory):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
}

AwrnsPlnExctnCategory::~AwrnsPlnExctnCategory() {}

long AwrnsPlnExctnCategory::ShowAwrnsPlnExctn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AwrnsPlnExctn;
	return ParentManage("HSE_TgAwrnsPlnExctn",TRUE,TRUE,TRUE,"HSE_AwrnsPln",strCriteria);
}


HRESULT AwrnsPlnExctnCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if (strButtonName=="AWRNSPLNEXCTN_CRS_CRTUNPLNDCRS") {		
		createUnPlndCrs();
	} else if (strButtonName=="AWRNSPLNEXCTN_PLNCLSD") {
		//////////////////////////////////////////////////////////////////////////////////////////////
		//Bugs-10-2021.11
		//CString strAwrnsPlnYr = FormGetField("HSE_AWRNSPLN","AWRNSPLN_YR");
		CString strAwrnsSrl = FormGetField("HSE_AWRNSPLN","SerialPK");
		closeAwrnsPln(strAwrnsSrl);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}
	return AwrnsPlnCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT AwrnsPlnExctnCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT AwrnsPlnExctnCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	//Enable delete only on unplanned courses
	if(strSubFormTag == "HSE_TGAWRNSPLNEXCTN_CRS" && pRecordRepostion->iComplete==0)
	{		 
		CString strPlndCrs = FormGetField("HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_PLNDCRS");
		if(strPlndCrs == "Yes")
		{			
			EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);					
		} else {
			EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

/**
 * Display message "Are you sure to create un-planned course (Yes/No)" 
 * If user clicks (No) then cancel the action
 * Allow user to insert new record into (courses) tab, in a similar way like (awareness plan entry) & (awareness plan execution)
 * All frames are "enabled"
 **/
void AwrnsPlnExctnCategory::createUnPlndCrs() 
{
	int nRet=0;
	nRet = MessageBox(NULL,"Are you sure to create un-planned course ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
	if(nRet == IDYES) {
		DoToolBarAction(TOOLBAR_NEW,"HSE_TGAWRNSPLNEXCTN","HSE_TGAWRNSPLNEXCTN_CRS");		
	}
}

void AwrnsPlnExctnCategory::closeAwrnsPln(CString strAwrnsPlnYr) 
{
	int nRet=0;
	nRet = MessageBox(NULL,"Are you sure to close the plan ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
	if(nRet == IDYES) {
		CString strSQL;	
		strSQL.Format("EXECUTE AwrnsPlnClosed %s",strAwrnsPlnYr);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	}
}