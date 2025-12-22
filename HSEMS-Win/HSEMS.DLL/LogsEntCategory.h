/**
 *	Handle Log Entry screen
 *
 */
#include "HSEMSCommonCategory.h"

class LogsEntCategory : public CHSEMSCommonCategory  
{
public:
	LogsEntCategory(CCategory *pCategory);
	virtual ~LogsEntCategory();
	long ShowHSELogsEnt(CString strCriteria);	
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);	
};

