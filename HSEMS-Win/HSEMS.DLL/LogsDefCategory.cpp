#include "stdafx.h"
#include "LogsDefCategory.h"


LogsDefCategory::LogsDefCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

LogsDefCategory::~LogsDefCategory()
{

}

long LogsDefCategory::ShowHSELogsDef(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOGS_DEF;
	return ParentManage("HSE_TgLogsDfntn",TRUE,TRUE,TRUE,"HSE_LogsDfntn",strCriteria);
}

HRESULT LogsDefCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strBtnName  = pCustomButtonClicked->Button_Name;
	m_strScreenTag      = pCustomButtonClicked->Form_Tag;
			
	if(strBtnName=="DTL_ATTR")
	{
		CString strIsGrp   = FormGetField("HSE_LOGSDFNTNATRBTS","LOGSDFNTNATRBTS_GRP");
		CString strKeyField = FormGetField("HSE_LOGSDFNTNATRBTS","MAINLINK");
		if(strIsGrp == "1") {
			CString strDefValues,strSQL;				
			strDefValues.Format("LogsDfntnAtrbtsDtl_Lnk|%s",strKeyField);						
			strSQL.Format("SELECT * FROM HSE_LogsDfntnAtrbtsDtl WHERE ( LogsDfntnAtrbtsDtl_Lnk = %s) ",strKeyField);			
			ShowScreen(0,"HSE_TGLOGSDFNTNATRBTSDTL","Detail Attributes",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);			
		} else {
			AfxMessageBox("Only group attributes have detail attributes.");
			return S_FALSE;
		}		
	}
		
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}