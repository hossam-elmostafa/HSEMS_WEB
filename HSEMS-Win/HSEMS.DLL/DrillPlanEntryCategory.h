#ifndef DrillPlanEntryCategory_HEADER
#define DrillPlanEntryCategory_HEADER

#include "DrillEntryCategory.h"

class DrillPlanEntryCategory : public DrillEntryCategory  
{
public:
	DrillPlanEntryCategory(CCategory *pCategory);
	virtual ~DrillPlanEntryCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);		
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowDrillPlanEntry(CString strCriteria);
	CString allowDrillEntry();
	CString getPolicyDrillYr();
	void CompleteDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked);
	void reloadScenario(tag_CustomButtonClicked* pCustomButtonClicked);
};

#endif

