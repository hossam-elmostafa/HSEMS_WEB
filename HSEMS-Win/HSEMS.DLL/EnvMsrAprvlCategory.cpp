#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMsrAprvlCategory.h"
#include "HSEobj.h"

EnvMsrAprvlCategory::EnvMsrAprvlCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ENVMSRENTRY";
	m_ScreenTgName="HSE_TGENVMSRAPRVL";
}

EnvMsrAprvlCategory::~EnvMsrAprvlCategory(){}


long EnvMsrAprvlCategory::ShowEnvMsrAprvl(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EnvMsrAprvl;
	return ParentManage("HSE_TGENVMSRAPRVL",FALSE,FALSE,FALSE,"HSE_ENVMSRENTRY",strCriteria);	
}

HRESULT EnvMsrAprvlCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
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
	if(strButtonName=="ENVMSRENTRY_REJECT")
	{
		rejectEnvMsr(strPrmKy);		
	} else if(strButtonName=="ENVMSRENTRY_APPROVE") {
		approveEnvMsr(strPrmKy);	
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void EnvMsrAprvlCategory::rejectEnvMsr(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE rejectEnvMsr '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EnvMsrAprvlCategory::approveEnvMsr(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE approveEnvMsr '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}