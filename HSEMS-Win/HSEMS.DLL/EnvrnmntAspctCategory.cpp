#include "stdafx.h" 
#include "HSEMS.h"
#include "EnvrnmntAspctCategory.h"
#include "HSEobj.h"
#include "EMailMsgHandler.h"

EnvrnmntAspctCategory::EnvrnmntAspctCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_Aspcts_VIEW";
	m_ScreenTgName="HSE_AspctsEntryAtEntry";
}

EnvrnmntAspctCategory::~EnvrnmntAspctCategory(){}

HRESULT EnvrnmntAspctCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	int nScreenType = pScreenReady->nScreenType;
	int iScreenMode = pScreenReady->iScreenMode;
	if(allowAspctEntry() == "N"){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		if(iScreenMode == 1) {
			EnableToolbar("",TOOLBAR_SAVE,FALSE);			
		}
		EnableTab("HSE_ASPCTS_ASPCTS",false);
		EnableTab("HSE_ASPCTS_TRCNG",false);
	} 	
	//TODO : below code will be set as a privilege
	/*bool bAdminUser = m_pCategory->GetHSEObj()->IsAdminUser(m_pCategory->GetUserID());
	if(bAdminUser) {
		ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE (ASPCTS_ASPCTSTTUS < 5)");
	} else {
		CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin());
		ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE (ASPCTS_ASPCTSTTUS < 5 AND ASPCTS_DPRTMNT = '"+strEmpDep+"')");
	}*/
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT EnvrnmntAspctCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1 && strBtnName=="NEW" && strSubFormTag == "")
	{					
		CString strAspctYr = getAspctYr();
		CString strAspctMonth = getAspctMonth();
		if(strAspctYr!= "" || strAspctMonth != "") {
			FormSetField("HSE_ASPCTS_VIEW","ASPCTS_YR",strAspctYr);
			FormSetField("HSE_ASPCTS_VIEW","ASPCTS_MNTH",getAspctMonth());
			bool bAdminUser = m_pCategory->GetHSEObj()->IsAdminUser(m_pCategory->GetUserID());
			if(bAdminUser) {
				SetFieldVisibleAttrib("HSE_ASPCTSENTRYATENTRY","ASPCTS_DPRTMNT","MUST");
			} else {
				CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);								
				FormSetField("HSE_ASPCTS_VIEW","ASPCTS_DPRTMNT",strEmpDep);
				SetFieldVisibleAttrib("HSE_ASPCTSENTRYATENTRY","ASPCTS_DPRTMNT","ALWAYSLOCKED");
			}
		} else {
			*pToolBarButtonClicked->iCancle = 1;			
			return S_FALSE;
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


HRESULT EnvrnmntAspctCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strAspctStatus = FormGetField("HSE_ASPCTS_VIEW","ASPCTS_ASPCTSTTUS");	
	int nRecordStatus = atoi(strAspctStatus); 
	if(nRecordStatus >= 5) {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
	} if (nRecordStatus == 10) {
		LockRecord("HSE_ASPCTSENTRYATENTRY","",true);
		LockRecord("HSE_ASPCTSENTRYATENTRY","HSE_ASPCTS_ASPCTS",true);
	} else {
		LockRecord("HSE_ASPCTSENTRYATENTRY","",false);
		LockRecord("HSE_ASPCTSENTRYATENTRY","HSE_ASPCTS_ASPCTS",false);
	}
	return S_OK;
}


HRESULT EnvrnmntAspctCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName = pFieldChange->Field_Name;
	CString strFieldValue= pFieldChange->New_Value;
	CString strTableName = pFieldChange->Table_Name;
	LONG strComplete = pFieldChange->iComplete;
	if(strComplete == 1) {		
		if(strFieldName == "ASPCTS_ASPCTS_CNSQUNC" || strFieldName == "ASPCTS_ASPCTS_LKLHD" || strFieldName == "ASPCTS_ASPCTS_CNSQUNC_1" || strFieldName == "ASPCTS_ASPCTS_LKLHD_1"){																		
			int nRskRnk1 = atoi(FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK"));
			int nRskRnk2 = atoi(FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK_1"));
			if(nRskRnk1 < nRskRnk2)
			{							
				ShowMessageEx(IDS_ASPCT_INVALID_RISK);
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK_1","");
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_CNSQUNC_1","");
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_LKLHD_1","");	
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKACCPTD","N");	
			} 
			else {				
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKACCPTD","Y");
			}
			if(strFieldName == "ASPCTS_ASPCTS_CNSQUNC_1" || strFieldName == "ASPCTS_ASPCTS_LKLHD_1") {
				//get f-Risk Level 
				CString strRskRnk2 = FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK_1");
				CString strfRskLvl = getfRskLvl(CString(strRskRnk2));
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKLVL_1",strfRskLvl);
			}
			if(strFieldName == "ASPCTS_ASPCTS_CNSQUNC" || strFieldName == "ASPCTS_ASPCTS_LKLHD") {
				//get f-Risk Level 
				CString strRskRnk1 = FormGetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKRNK");
				CString strfRskLvl = getfRskLvl(CString(strRskRnk1));
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_RSKLVL",strfRskLvl);
			}
		}
	}
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

HRESULT EnvrnmntAspctCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="VIEW_HISTORY")
	{
		bool bAdminUser = m_pCategory->GetHSEObj()->IsAdminUser(m_pCategory->GetUserID());
		if(bAdminUser) {
			ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE ASPCTS_DPRTMNT LIKE '%'");
		} else {
			CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),false);
			ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE (ASPCTS_DPRTMNT IN ("+strEmpDep+"))");
		}
	} else if(strButtonName=="VIEW_CURRENT_MONTH") {
		bool bAdminUser = m_pCategory->GetHSEObj()->IsAdminUser(m_pCategory->GetUserID());
		if(bAdminUser) {
			CString strYear   = FormGetField("HSE_ASPCTS_VIEW","ASPCTS_YR"); 
			CString strMonth  = FormGetField("HSE_ASPCTS_VIEW","ASPCTS_MNTH");
			ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE (ASPCTS_YR = '"+strYear+"' AND ASPCTS_MNTH = '"+strMonth+"')");
		} else {
			CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),false);
			CString strYear   = FormGetField("HSE_ASPCTS_VIEW","ASPCTS_YR"); 
			CString strMonth  = FormGetField("HSE_ASPCTS_VIEW","ASPCTS_MNTH");
			ChangeCriteria("HSE_ASPCTSENTRYATENTRY","","WHERE (ASPCTS_DPRTMNT IN ("+strEmpDep+") AND ASPCTS_YR = '"+strYear+"' AND ASPCTS_MNTH = '"+strMonth+"')");
		}
	} else if(strButtonName=="ENTRY_COMPLETED") {
		CString strPrmryKy = FormGetField("HSE_ASPCTS_VIEW","PrmryKy");
		completeEntry(strPrmryKy);
		
				
		//CString strSenderName  = "HSEMS System";	
		//CString strMailSubject = "Received CAR , Auto E-mail From HSEMS";						
		//EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
		//CString strMailBody = eMailMsgHandler->FormatMailBody(nMsgBody,strFormTag);
		//m_pCategory->GetHSEObj()->sendMail("a.ali","mail Subject","ahmed.ali@first-egyptian.com","Mail Body","");		
		//pDaoRecset->MoveNext();	
		
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long EnvrnmntAspctCategory::ShowEnvronmntlAspctEntry(CString strCriteria)
{	
	m_nCurrentCard=HSE_ASPECTS_ENTRY_ENTRY;
	return ParentManage("HSE_ASPCTSENTRYATENTRY",TRUE,TRUE,TRUE,"HSE_ASPCTS_VIEW","WHERE (ASPCTS_ASPCTSTTUS < 5)");	
}

CString EnvrnmntAspctCategory::getAspctYr(){
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_ASPCTYR,'') FROM HSE_HSEPLC");
	CString strAspctYr = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctYr;
}

CString EnvrnmntAspctCategory::getAspctMonth(){
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_ASPCTMNTH,'') FROM HSE_HSEPLC");
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctMonth;
}


CString EnvrnmntAspctCategory::allowAspctEntry(){
	CString strSQL;
	strSQL.Format("SELECT TOP 1 ISNULL(HSEPLC_ALWASPCTENTRY,'') FROM HSE_HSEPLC");
	CString strAspctMonth = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAspctMonth;
}

//Moved to HSEMSCommonCategory
/*CString EnvrnmntAspctCategory::getEmpDep(CString strUserID) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(EMPLOYEE_WSHOP,'') FROM CMN_EMPLOYEE WHERE EMPLOYEE_LOGINNAME = '"+strUserID+"'");
	CString strEmpDep = GetRecordSetData(strSQL).GetBuffer(0);	
	return strEmpDep;
}*/

void EnvrnmntAspctCategory::completeEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE EnvAspctEntryComplete '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}


CString EnvrnmntAspctCategory::getfRskLvl(CString fstrRskRnk) 
{
	if(fstrRskRnk != ""){
		CString strSQL;	
		strSQL.Format("SELECT RSKLVL_DSC FROM HSE_RSKLVL WHERE RSKLVL_COD = (SELECT RSKRNKDESC_RSKLVL FROM HSE_RSKRNKDESC WHERE RSKRNKDESC_RNK = %s)",fstrRskRnk);
		CString strfRskLvl = GetRecordSetData(strSQL).GetBuffer(0);	
		return strfRskLvl;
	}
	else 
		return "";
}