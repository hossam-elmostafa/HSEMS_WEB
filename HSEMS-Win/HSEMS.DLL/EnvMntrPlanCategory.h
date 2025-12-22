#ifndef EnvMntrPlanCategory_HEADER
#define EnvMntrPlanCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvMntrPlanCategory : public CHSEMSCommonCategory  
{
public:
	EnvMntrPlanCategory(CCategory *pCategory);
	virtual ~EnvMntrPlanCategory();
	long ShowEnvMntrPlan(CString strCriteria);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);	
	void approveEnvMntrPlan(CString strPrmryKey);
	void reOpnEnvMntrPlan(CString strPrmryKey);
	void reloadEnvMntrPlnActvty(CString strPrmryKey);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
};

#endif

