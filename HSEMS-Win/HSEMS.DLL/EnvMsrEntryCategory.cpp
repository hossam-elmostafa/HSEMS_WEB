#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMsrEntryCategory.h"
#include "HSEobj.h"

EnvMsrEntryCategory::EnvMsrEntryCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ENVMSRENTRY";
	m_ScreenTgName="HSE_TGENVMSRENTRY";
}

EnvMsrEntryCategory::~EnvMsrEntryCategory(){}


long EnvMsrEntryCategory::ShowEnvMsrEntry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EnvMsrEntry;
	return ParentManage("HSE_TGENVMSRENTRY",TRUE,TRUE,TRUE,"HSE_ENVMSRENTRY",strCriteria);	
}

HRESULT EnvMsrEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1)
	{	
		if(strBtnName=="NEW") {
			CString strCurrentYear = FormGetField("HSE_ENVMSRENTRY","ENVMSRENTRY_YR");
			FormSetField("HSE_ENVMSRENTRY","ENVMSRENTRY_MSRNO",getCurrentMsrNo(strCurrentYear));
		}
		else if(strBtnName=="SAVETAB" && strSubFormTag=="HSE_TGENVMSRENTRYPOLTANT") {
			FormSetField("HSE_ENVMSRENTRY","ENVMSRENTRY_LSTMDFYDT",GetFullDateTime("%d/%m/%Y"));
			FormSetField("HSE_ENVMSRENTRY","ENVMSRENTRY_LSTMDFYBY",GetUserLogin());
			DoToolBarAction(TOOLBAR_SAVE,"HSE_TGENVMSRENTRY","");
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT EnvMsrEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strForm		  = pRecordRepostion->Form_Tag;
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;	
	CString strEntryStts = FormGetField("HSE_ENVMSRENTRY","ENVMSRENTRY_ENTRYSTTS");	
	int nEntryStts = atoi(strEntryStts);
	if(nEntryStts >= 5) {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);		
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		EnableToolbar("",TOOLBAR_SAVE,FALSE);
		LockRecord("","",true);
	} else {
		EnableToolbar("",TOOLBAR_DELETE,TRUE);		
		EnableToolbar("",TOOLBAR_NEW,TRUE);
		EnableToolbar("",TOOLBAR_SAVE,TRUE);
		LockRecord("","",false);
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);	
}

HRESULT EnvMsrEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "") {
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	}
	CString strPrmKy = FormGetField("HSE_ENVMSRENTRY","ENVMSRENTRY_PRMKY");
	if(strButtonName=="ENVMSRENTRY_ENTRYCMPLTD")
	{
		completeEnvMsrEntry(strPrmKy);		
	} else if(strButtonName=="ENVMSRENTRYPOLTANT_PRPRCARS") {
		CString strSQL ;	
		bool bLocked = false;
		CString strMainLinkFeild = FormGetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_PRMKY");	
		CString strDefValues;
		strDefValues.Format("EnvMsrEntryPoltantPrpCAR_Lnk|%s",strMainLinkFeild);	
		strSQL.Format("SELECT * FROM HSE_EnvMsrEntryPoltantPrpCAR WHERE EnvMsrEntryPoltantPrpCAR_Lnk = %s",strMainLinkFeild);
		ShowScreen(0,"HSE_TgEnvMsrEntryPoltantPrpCAR","Recommended CARs",NORMAL_MODE,false,strSQL,"",strDefValues,"",bLocked);	
	} else if(strButtonName=="ENVMSRENTRYPOLTANT_RELDPOLTNTS") {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to re-load pollutant records ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			reldPoltnts(strPrmKy);
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT EnvMsrEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName = pFieldChange->Field_Name;
	CString strFieldValue= pFieldChange->New_Value;
	CString strTableName = pFieldChange->Table_Name;
	LONG strComplete = pFieldChange->iComplete;
	if(strComplete == 1) {		
		if(strFieldName == "ENVMSRENTRYPOLTANT_MSRDVAL" ){
			int nMinVal = atoi(FormGetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_MINVAL"));
			int nMaxVal = atoi(FormGetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_MAXVAL"));
			int nMsrdVal = atoi(FormGetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_MSRDVAL"));
			if(nMsrdVal < nMinVal)
			{							
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_DVTN",itoa(nMsrdVal-nMinVal));
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_RSLTACPTD","N");		//moved from FirstSys customAttribute		
			} else if(nMsrdVal > nMaxVal){
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_DVTN",itoa(nMsrdVal-nMaxVal));
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_RSLTACPTD","N");		//moved from FirstSys customAttribute		
			} else {
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_DVTN","0");
				FormSetField("HSE_ENVMSRENTRYPOLTANT","ENVMSRENTRYPOLTANT_RSLTACPTD","Y");		//moved from FirstSys customAttribute		
			}
		}
	}
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

CString EnvMsrEntryCategory::getCurrentMsrNo(CString strCurrentYear) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(ENVMSRENTRY_MSRNO)+1,1) FROM HSE_ENVMSRENTRY WHERE ENVMSRENTRY_YR = '%s'",strCurrentYear);
	CString strCurrentMsrNo = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCurrentMsrNo;	
}

void EnvMsrEntryCategory::completeEnvMsrEntry(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE completeEnvMsrEntry '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvMsrEntryCategory::reldPoltnts(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE reldPoltnts '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}