#ifndef DrillPlanAprvCategory_HEADER
#define DrillPlanAprvCategory_HEADER

#include "DrillEntryCategory.h"

class DrillPlanAprvCategory : public DrillEntryCategory  
{
public:
	DrillPlanAprvCategory(CCategory *pCategory);
	virtual ~DrillPlanAprvCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);		
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	long ShowDrillPlanAprv(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void ApproveDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked);
	void RejectDrillPlan(tag_CustomButtonClicked* pCustomButtonClicked);
};

#endif

