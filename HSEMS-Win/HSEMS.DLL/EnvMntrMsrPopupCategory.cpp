#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMntrMsrPopupCategory.h"
#include "HSEobj.h"

EnvMntrMsrPopupCategory::EnvMntrMsrPopupCategory(CCategory *pCategory):EnvMntrCategory(pCategory) {
	m_pCategory = pCategory;
}

EnvMntrMsrPopupCategory::~EnvMntrMsrPopupCategory(){}

// Pollutants
// Updated to Measure
long EnvMntrMsrPopupCategory::ShowEnvMntrActvtsPltnt(CString strCriteria,CString strComeFrom)
{
	m_nCurrentCard=ID_HSE_EnvMntrActvtsPltnt;
	strComeFrom = getOwnerScreenTag();
	BOOL bAllowUpdate = TRUE;
	BOOL bAllowAddNew = FALSE;
	BOOL bAllowDelete = FALSE;
	if(strComeFrom == "HSE_TGENVMNTRPLAN") {
		bAllowUpdate = FALSE;
		bAllowAddNew = FALSE;
		bAllowDelete = FALSE;
		EnableToolbar("HSE_TGENVMNTRACTVTSPLTNT",TOOLBAR_SHOW_DETAILS,FALSE);		
	}	
	return ParentManage("HSE_TGENVMNTRACTVTSPLTNT",bAllowUpdate,bAllowAddNew,bAllowDelete,"HSE_ENVMNTRACTVTSPLTNT",strCriteria);
}

HRESULT EnvMntrMsrPopupCategory::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);
	if(strSubFormTag != ""){
		CString strActivityStatusHolder = FormGetField("HSE_ENVMNTRACTVTSPLTNT","ActivityStatusHolder");
		int nActivityStatusHolder = atoi(strActivityStatusHolder);
		if(nActivityStatusHolder >= 5) {
			EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);		
			EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
			EnableToolbar(strSubFormTag,TOOLBAR_SAVE,FALSE);
			LockRecord("",strSubFormTag,true);
		} else {
			EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);		
			EnableToolbar(strSubFormTag,TOOLBAR_NEW,TRUE);
			EnableToolbar(strSubFormTag,TOOLBAR_SAVE,TRUE);
			LockRecord("",strSubFormTag,false);
		}
	}
	return EnvMntrCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT EnvMntrMsrPopupCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strBtnName(pCustomButtonClicked->Button_Name);		
	CString strTableName=CString(pCustomButtonClicked->Table_Name);
	
	if(strBtnName == "ENVMNTRACTVTSPLTNTMSRAREA_RQRDACTN"){
		CString strRqrNCR = FormGetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_RQRNCR");
		if(strRqrNCR == "Y") {
			CString strSQL ;	
			bool bLocked = false;
			CString strPrmKyFeild = FormGetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA" ,"PRMKY");
			CString strDefValues;
			strDefValues.Format("ENVMNTRRQRDACTN_LNK|%s",strPrmKyFeild);	
			strSQL.Format("SELECT * FROM HSE_ENVMNTRRQRDACTN WHERE (ENVMNTRRQRDACTN_LNK = %s)",strPrmKyFeild);
			ShowScreen(0,"HSE_TGENVMNTRRQRDACTN","Required Actions",NORMAL_MODE,false,strSQL,"",strDefValues,"",bLocked);
		} else {
			ShowMessageEx(IDS_MSR_RQR_NCR,"",IV_NOICON,"Complete Activity");
		}
	}

	return EnvMntrCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}