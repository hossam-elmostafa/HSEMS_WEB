
#include "stdafx.h"
#include "HSEMS.h"
#include "LogsEntDtlCntCategory.h"
#include "HSEobj.h"


LogsEntDtlCntCategory::LogsEntDtlCntCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
	m_MultiRecAttr = false;
}

LogsEntDtlCntCategory::~LogsEntDtlCntCategory()
{

}

long LogsEntDtlCntCategory::ShowLogsEntDtlCnt(CString strCriteria)
{
	m_MultiRecAttr = m_pCategory->GetHSEObj()->getbLogMultiRec();
	m_nCurrentCard=ID_HSE_LOGS_ENT_DTL_CNT;
	return ParentManage("HSE_TgLogsEntDtlCnt",TRUE,TRUE,TRUE,"HSE_LogsEntDtlCnt",strCriteria);
}

void LogsEntDtlCntCategory::OnBeforeListReady(LPCTSTR FormTag)
{
	if(m_MultiRecAttr == true) {		
		EnableToolbar("",TOOLBAR_DELETE,TRUE);
		FormSetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_VAL","");
		SetFieldVisibleAttrib("HSE_TGLOGSENTDTLCNT","LOGSENTDTLCNT_VAL","ALWAYSLOCKED");
	} else {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		SetFieldVisibleAttrib("HSE_TGLOGSENTDTLCNT","LOGSENTDTLCNT_VAL","MUST");
	}
	CHSEMSCommonCategory::OnBeforeListReady(FormTag);
}

HRESULT LogsEntDtlCntCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	if(m_MultiRecAttr == true) {
		EnableToolbar("",TOOLBAR_DELETE,TRUE);
		FormSetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_VAL","");
		SetFieldVisibleAttrib("HSE_TGLOGSENTDTLCNT","LOGSENTDTLCNT_VAL","ALWAYSLOCKED");
	} else {
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		SetFieldVisibleAttrib("HSE_TGLOGSENTDTLCNT","LOGSENTDTLCNT_VAL","MUST");
	}
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}


HRESULT LogsEntDtlCntCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	/*(strBtnName=="NEW") {
		CString str	= FormGetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_FLDNAMKEYVAL");
		if(str == "") {		
			FormSetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_VAL","");
			SetFieldVisibleAttrib("HSE_TGLOGSENTDTLCNT","LOGSENTDTLCNT_VAL","NULL");
		}
	}*/
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT LogsEntDtlCntCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	if(pRecordRepostion->iComplete==1) {
		if(strSubFormTag != "") {		
				CString strBrwsTbl = FormGetField("HSE_LOGSENTDTLCNTDTL","LOGSENTDTLCNTDTL_BRWSTBL");
				CString strBrwsFld = FormGetField("HSE_LOGSENTDTLCNTDTL","LOGSENTDTLCNTDTL_BRWSFLD");
				if(strBrwsTbl != "" && strBrwsFld != "") {
					LinkFieldsAttribSingle(strSubFormTag,"LOGSENTDTLCNTDTL_VAL","",-1,"","","","",1,strBrwsTbl," ",strBrwsFld,strBrwsFld,"",0,0,"");	
				} else {
					LinkFieldsAttribSingle(strSubFormTag,"LOGSENTDTLCNTDTL_VAL","",-1,"","","","",0," ",NULL," "," ","",0,0,"");
				}		
		} else {
			CString strBrwsTbl = FormGetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_BRWSTBL");
			CString strBrwsFld = FormGetField("HSE_LOGSENTDTLCNT","LOGSENTDTLCNT_BRWSFLD");
			if(strBrwsTbl != "" && strBrwsFld != "") {	
				FieldSetAttribSingle("LOGSENTDTLCNT_VAL","", -1,"","","","",1,strBrwsTbl,"",strBrwsFld,strBrwsFld,"",0, 0,"");		
			} else {
				FieldSetAttribSingle("LOGSENTDTLCNT_VAL","", -1,"","","","",0," "," "," "," ","",0, 0,"");		
			}
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

