#include "stdafx.h"
#include "HSEMS.h"
#include "EnvrnmntAspctReviewCategory.h"
#include "HSEobj.h"

EnvrnmntAspctReviewCategory::EnvrnmntAspctReviewCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ASPCTS_VIEW";
	m_ScreenTgName="HSE_ASPCTSRVWATRVW";
}

EnvrnmntAspctReviewCategory::~EnvrnmntAspctReviewCategory(){}

HRESULT EnvrnmntAspctReviewCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	if(allowAspctReview() == "N"){
		EnableToolbar("",TOOLBAR_SAVE,FALSE);
		EnableToolbar("HSE_ASPCTS_ASPCTS",TOOLBAR_SAVE,FALSE);		
		EnableToolbar("HSE_ASPCTS_ASPCTS",TOOLBAR_DELETE,FALSE);
		EnableToolbar("HSE_ASPCTS_ASPCTS",TOOLBAR_NEW,FALSE);
	}
	/*int nScreenType = pScreenReady->nScreenType;
	int iScreenMode = pScreenReady->iScreenMode;
	if(allowAspctEntry() == "N"){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		if(iScreenMode == 1) {
			EnableToolbar("",TOOLBAR_SAVE,FALSE);			
		}
		EnableTab("HSE_ASPCTS_ASPCTS",false);
		EnableTab("HSE_ASPCTS_TRCNG",false);
	}*/ 	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT EnvrnmntAspctReviewCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	/*CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1 && strBtnName=="NEW")
	{						
		FormSetField("HSE_ASPCTS","ASPCTS_YR",getAspctYr());
		FormSetField("HSE_ASPCTS","ASPCTS_MNTH",getAspctMonth());
	}*/
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


HRESULT EnvrnmntAspctReviewCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	/*CString strAspctStatus = FormGetField("HSE_ASPCTS","ASPCTS_ASPCTSTTUS");	
	int nRecordStatus = atoi(strAspctStatus);
	if(nRecordStatus >= 5) {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
	}*/
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}


HRESULT EnvrnmntAspctReviewCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName = pFieldChange->Field_Name;
	CString strFieldValue= pFieldChange->New_Value;
	CString strTableName = pFieldChange->Table_Name;
	LONG strComplete = pFieldChange->iComplete;
	if(strComplete == 1) {
		//TODO : move this code to main category
		if(strFieldName == "ASPCTS_ASPCTS_RSKRNK_1"){
			int nRskRnk1 = atoi(FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK"));
			int nRskRnk2 = atoi(FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK_1"));
			if(nRskRnk1 < nRskRnk2)
			{							
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK_1","");
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_CNSQUNC_1","");
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_LKLHD_1","");								
			} else {
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKACCPTD","Y");
			}
		}
	}
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}


HRESULT EnvrnmntAspctReviewCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="ACCEPTED")
	{
		CString strPrmryKy = FormGetField("HSE_ASPCTS_VIEW","PrmryKy");
		acceptEnvAspect(strPrmryKy);
	} else if(strButtonName=="REJECTED") {
		CString strPrmryKy = FormGetField("HSE_ASPCTS_VIEW","PrmryKy");
		rejectEnvAspect(strPrmryKy);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long EnvrnmntAspctReviewCategory::ShowEnvronmntlAspctReview(CString strCriteria)
{	
	m_nCurrentCard=HSE_ASPECTS_ENTRY_ENTRY;
	return ParentManage("HSE_ASPCTSRVWATRVW",TRUE,TRUE,TRUE,"HSE_ASPCTS_VIEW",strCriteria);	
}

CString EnvrnmntAspctReviewCategory::allowAspctReview(){
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_EDTASPCTRVWSCR,'') FROM HSE_HSEPLC");
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctMonth;
}

void EnvrnmntAspctReviewCategory::acceptEnvAspect(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE EnvAspctRvwAccpt '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvrnmntAspctReviewCategory::rejectEnvAspect(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE EnvAspctRjct '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}