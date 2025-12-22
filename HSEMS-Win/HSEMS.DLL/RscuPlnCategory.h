#ifndef RscuPlnCategory_HEADER
#define RscuPlnCategory_HEADER

#include "HSEMSCommonCategory.h"

class RscuPlnCategory : public CHSEMSCommonCategory  
{
public:
	RscuPlnCategory(CCategory *pCategory);
	virtual ~RscuPlnCategory();
	long ShowRscuPlnEntry(CString strCriteria);
	long ShowRscuPlnInqury(CString strCriteria);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);		
	void cancelRscuPlan(CString strRscuPlanKey);
	void closeRscuPlan(CString strRscuPlanKey);
	void rtrnRscuPlnToEntry(CString strRscuPlanKey);
};

#endif