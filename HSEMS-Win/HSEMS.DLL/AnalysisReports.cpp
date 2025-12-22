#include "stdafx.h"
#include "AnalysisReports.h"

UINT CAnalysisReports::ReportTypes()
{	
	m_nCurrentCard=ID_ANALYSIS_REPORTTYPES;
	CString strWhere="";
	strWhere.Format("WHERE (APPLICATIONCODE = %ld)",APP_CODE);
	return ParentManage("REPORTTYPES",TRUE,TRUE,TRUE,"",strWhere);
}

CAnalysisReports::~CAnalysisReports()
{
	if(m_pReportsObj)
		m_pReportsObj.Release();
}

void CAnalysisReports::ReportsList(BOOL bArabic)
{
	if (m_pReportsObj == NULL)
		m_pReportsObj.CreateInstance(__uuidof(ReportsObj));
	
	_bstr_t bstr;
	bstr.Assign(m_pCategory->GetConnectionString().AllocSysString());

	try
	{
		if (m_pReportsObj)
		{
			CRect rcRect=m_pCategory->GetFormRect();
			m_pReportsObj->SetFormRect(rcRect.left, rcRect.top, rcRect.right, rcRect.bottom);
			m_pReportsObj->SetPolyDb(m_pCategory->m_nEngine,bstr,STR_APP_CODE,APP_ODBC,m_pCategory->GetUserID(),bArabic,TRUE);
		}
		else
			AfxMessageBox("Can not create instance from Report Module. \r\nPlease make sure module is installed correctly");
	}
	catch(_com_error &e)
	{
		_bstr_t s=e.Description();
	}
}
