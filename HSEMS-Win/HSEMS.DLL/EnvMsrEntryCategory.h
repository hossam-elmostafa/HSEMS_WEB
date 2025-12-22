#ifndef EnvMsrEntryCategory_HEADER
#define EnvMsrEntryCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvMsrEntryCategory : public CHSEMSCommonCategory  
{
public:
	EnvMsrEntryCategory(CCategory *pCategory);
	virtual ~EnvMsrEntryCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	long ShowEnvMsrEntry(CString strCriteria);
	CString getCurrentMsrNo(CString strCurrentYear);
	void completeEnvMsrEntry(CString strPrmKy);
	void reldPoltnts(CString strPrmKy);
};

#endif

