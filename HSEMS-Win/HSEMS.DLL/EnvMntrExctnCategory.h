#ifndef EnvMntrExctnCategory_HEADER
#define EnvMntrExctnCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvMntrExctnCategory : public CHSEMSCommonCategory  
{
public:
	EnvMntrExctnCategory(CCategory *pCategory);
	virtual ~EnvMntrExctnCategory();
	long ShowEnvMntrExctn(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	void closeEnvMntrPlan(CString strPrmryKey);
	enum Activity_Status {Pending=1,Cancelled=2,Completed=5};
	void changeEnvMsrActvtStat(int ActivityNewStatus,CString ActivityPrmKy);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
};

#endif

