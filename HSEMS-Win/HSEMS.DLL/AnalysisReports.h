////advanced.h
#ifndef _REPORTS_H
#define _REPORTS_H


class CAnalysisReports:public CCommonCategory
{
public:
	CAnalysisReports(CCategory *pCategory):CCommonCategory(pCategory)
	{
		m_pCategory = pCategory;
		m_pReportsObj = NULL;
	};

	~CAnalysisReports();
	UINT ReportTypes();
	void ReportsList(BOOL bArabic);

protected:
	IReportsObjPtr m_pReportsObj;

};

#endif

