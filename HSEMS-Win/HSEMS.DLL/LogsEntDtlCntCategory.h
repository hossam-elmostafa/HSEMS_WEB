/**
 *	Handle Log definition Attribute Detail screen
 *
 */
#include "HSEMSCommonCategory.h"

class LogsEntDtlCntCategory : public CHSEMSCommonCategory  
{
public:
	LogsEntDtlCntCategory(CCategory *pCategory);
	virtual ~LogsEntDtlCntCategory();
	long ShowLogsEntDtlCnt(CString strCriteria);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);	
	void LogsEntDtlCntCategory::OnBeforeListReady(LPCTSTR FormTag);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

private:
	bool m_MultiRecAttr;
};

