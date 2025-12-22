// IncdntFlshRprtCategory.cpp: implementation of the IncdntFlshRprtCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "IncdntFlshRprtCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IncdntFlshRprtCategory::IncdntFlshRprtCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

IncdntFlshRprtCategory::~IncdntFlshRprtCategory(){}

HRESULT IncdntFlshRprtCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{	
	CString strFormTag = CString(pScreenReady->Form_Tag);
	bool bMultiCompany = isMultiCompany();
	if(!bMultiCompany) {				
		UpdateValidation("66",strFormTag,"","INCDNTFLSHRPRT_CMPNYNAM","HIDDEN","NewValidationAttrib");
	} else {
		UpdateValidation("66",strFormTag,"","INCDNTFLSHRPRT_CMPNYNAM","NULL","NewValidationAttrib");
	}
	if(strFormTag == "HSE_TGINCDNTFLSHRPRTRVEW") {		
		if(!isEdtFlshRvwAlwd()) {
			LockRecord("","",true);
		} else {
			LockRecord("","",false);
		}
		//Disable New Button
		EnableToolbar("",TOOLBAR_NEW,FALSE);
	}
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT IncdntFlshRprtCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName=="INCDNTFLSHRPRT_LOC")
	{
		CString strSite=FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_SIT");	
		CString strSQL;
		if(strSite.IsEmpty())
			strSite=" ";
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSite);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	else if(strFieldName=="INCDNTFLSHRPRT_AREANAM")
	{
		CString strLocTyp=FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LOC");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	else if(strFieldName=="INCDNTFLSHRPRT_SIT")
	{
		CString strCmpnyNam=FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_CMPNYNAM");								
		if(strCmpnyNam.IsEmpty())
			strCmpnyNam=" ";
		CString strSQL;
		strSQL.Format("WHERE (STE_CMPNYNAME ='%s')",strCmpnyNam);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT IncdntFlshRprtCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	if(strFieldName == "INCDNTFLSHRPRT_CMPNYNAM") {
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_SIT","");
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LOC","");
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_AREANAM","");
	} else if (strFieldName == "INCDNTFLSHRPRT_SIT") {
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LOC","");
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_AREANAM","");
	} else if (strFieldName == "INCDNTFLSHRPRT_LOC") {
		FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_AREANAM","");
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT IncdntFlshRprtCategory::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	CString strLinkedYr = FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LNKDYR");	
	if(strLinkedYr != "0") {	
		LockRecord("HSE_TGINCDNTFLSHRPRT","",true);
	} else {
		LockRecord("HSE_TGINCDNTFLSHRPRT","",false);
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT IncdntFlshRprtCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	LONG iComplete = pToolBarButtonClicked->iComplete;
	if(strBtnName == "DELETE"){
		if(strFormTag == "HSE_TGINCDNTFLSHRPRT"){
			if(iComplete == 0) {
				int strFlashSts = atoi(FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_FLSHSTS"));
				if(strFlashSts >= 5) {
					AfxMessageBox("Cannot delete this flash record");
					*pToolBarButtonClicked->iCancle = 1;
				}
				int strLinkedIncidentNo = atoi(FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LNKDINCDNTNO"));
				if(strLinkedIncidentNo>0){
					AfxMessageBox("Cannot delete this flash record");
					*pToolBarButtonClicked->iCancle = 1;
				}
			}
		}		
	} else if(strBtnName == "NEW") {
		if(strFormTag == "HSE_TGINCDNTFLSHRPRT"){
			if(iComplete == 1) {
				//TODO : this code should be replaced by setFieldDefaultValue 
				bool bMultiCompany = isMultiCompany();
				if(!bMultiCompany) {	
					FormSetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_CMPNYNAM",GetSingleCompanyName());
					//hide company field
					SetFieldVisibleAttrib(strFormTag,"INCDNTFLSHRPRT_CMPNYNAM","HIDDEN");
				}
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT IncdntFlshRprtCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strPrmryKey = FormGetField("HSE_INCDNTFLSHRPRT","MAINLINK");
	if(strBtnName=="NCDNTFLSHRPRT_ENTRYCMPLT") {		
		CompleteFlashEntry(strPrmryKey);
	} else if (strBtnName == "INCDNTFLSHRPRTRVEW_RJCT") {
		rejectFlashFromReview(strPrmryKey);
	} else if (strBtnName == "INCDNTFLSHRPRTRVEW_ACCPT") {
		accptFlashFromReview(strPrmryKey);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long IncdntFlshRprtCategory::ShowIncdntFlshRprt(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntFlshRprt;
	return ParentManage("HSE_TGINCDNTFLSHRPRT",TRUE,TRUE,TRUE,"HSE_IncdntFlshRprt",strCriteria);
}

long IncdntFlshRprtCategory::ShowIncdntFlshRprtRvw(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntFlshRprt;
	return ParentManage("HSE_TGINCDNTFLSHRPRTRVEW",TRUE,TRUE,TRUE,"HSE_IncdntFlshRprt",strCriteria);
}

long IncdntFlshRprtCategory::ShowIncdntFlshRprtInq(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntFlshRprt;
	return ParentManage("HSE_TGINCDNTFLSHRPRTINQ",FALSE,FALSE,FALSE,"HSE_IncdntFlshRprt",strCriteria);
}

void IncdntFlshRprtCategory::CompleteFlashEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE CompleteFlashEntry '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

bool IncdntFlshRprtCategory::isEdtFlshRvwAlwd(){
	bool allowEdtFlshRvw = false;
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_EDTFLSHRVWSCR,'') FROM HSE_HSEPLC");
	CString strEdtFlshRvwScr = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strEdtFlshRvwScr == 'Y')
		allowEdtFlshRvw = true;
	return allowEdtFlshRvw;
}

//Check policy if HSE application serving multi-company or deployed for one company
bool IncdntFlshRprtCategory::isMultiCompany(){
	bool bMultiCompany = false;
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPlc_SrvMltCmpny,'') FROM HSE_HSEPLC");
	CString strMultiCompany = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strMultiCompany == 'Y')
		bMultiCompany = true;
	return bMultiCompany;
}

//Get Company Name 
CString IncdntFlshRprtCategory::GetCompanyName() {
	CString strSQL;
	CString strCompanyName;
	strSQL.Format("SELECT TOP 1 (COMPPRF_FULNAM) FROM HSE_COMPPRF");	
	strCompanyName = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCompanyName;
}

void IncdntFlshRprtCategory::rejectFlashFromReview(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE RejectFlashFromReview '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void IncdntFlshRprtCategory::accptFlashFromReview(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE AccptFlashFromReview '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}




