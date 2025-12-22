#ifndef DrillPlanExecutionCategory_HEADER
#define DrillPlanExecutionCategory_HEADER

#include "DrillEntryCategory.h"

class DrillPlanExecutionCategory : public DrillEntryCategory  
{
public:
	DrillPlanExecutionCategory(CCategory *pCategory);
	virtual ~DrillPlanExecutionCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);		
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	//HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowDrillPlanExecution(CString strCriteria);
	void CloseDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked);
	void CancelDrill(tag_CustomButtonClicked* pCustomButtonClicked);
	void reloadScenario(tag_CustomButtonClicked* pCustomButtonClicked);
};

#endif

