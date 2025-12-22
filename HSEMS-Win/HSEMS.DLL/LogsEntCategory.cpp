#include "stdafx.h"
#include "HSEMS.h"
#include "LogsEntCategory.h"
#include "HSEobj.h"


LogsEntCategory::LogsEntCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

LogsEntCategory::~LogsEntCategory()
{

}

long LogsEntCategory::ShowHSELogsEnt(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOGS_ENT;
	return ParentManage("HSE_TgLogsEnt",TRUE,TRUE,TRUE,"HSE_LogsEnt",strCriteria);
}

HRESULT LogsEntCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName  = pCustomButtonClicked->Button_Name;
	m_strScreenTag      = pCustomButtonClicked->Form_Tag;			
	if(strBtnName=="LOGSENTDTL_OPNCNTDTL")
	{		
		CString strIsGrpAttr = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_GRP"); 
		CString strMRec		 = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_MREC"); 
		CString strAttKeyFld = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_CNTENTKEY");

		if(strIsGrpAttr == "1") {
			CString strDefValues,strSQL;
			CString strKeyField = FormGetField("HSE_LOGSENTDTL","MAINLINK");
			if(strMRec == "1") {
				strDefValues.Format("LOGSENTDTLCNT_LNK|%s|LOGSENTDTLCNT_FLDNAMKEYVAL|%s",strKeyField,strAttKeyFld);						
			} else {
				strDefValues.Format("LOGSENTDTLCNT_LNK|%s",strKeyField);						
			}
			strSQL.Format("SELECT * FROM HSE_LOGSENTDTLCNT WHERE (LOGSENTDTLCNT_LNK = %s)",strKeyField);	
			
			CString strLogMultiRec = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_MREC");
			if(strLogMultiRec == "1") {
				m_pCategory->GetHSEObj()->setbLogMultiRec(true);	
			} else {
				m_pCategory->GetHSEObj()->setbLogMultiRec(false);
			}
			ShowScreen(0,"HSE_TGLOGSENTDTLCNT","Content Details",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);			
		} else {
			AfxMessageBox("Only group attributes have content details.");
			return S_FALSE;	
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


HRESULT LogsEntCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);	
	if(pRecordRepostion->iComplete==1) {		
		CString strBrwsTbl = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_BRWSTBL");
		CString strBrwsFld = FormGetField("HSE_LOGSENTDTL","LOGSENTDTL_BRWSFLD");
		if(strBrwsTbl != "" && strBrwsFld != "") {
			LinkFieldsAttribSingle(strSubFormTag,"LOGSENTDTL_VAL","",-1,"","","","",1,strBrwsTbl,NULL,strBrwsFld,strBrwsFld,"",0,0,"");	
		} else {
			LinkFieldsAttribSingle(strSubFormTag,"LOGSENTDTL_VAL","",-1,"","","","",0," ",NULL," "," ","",0,0,"");
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}
