#ifndef TaskResponsibilityPopCategory_HEADER
#define TaskResponsibilityPopCategory_HEADER


#include "RiskAssessmentCategory.h"

class CTaskResponsibilityPopCategory : public RiskAssessmentCategory  
{
public:
	CTaskResponsibilityPopCategory(CCategory *pCategory,CString strCriteria);
	virtual ~CTaskResponsibilityPopCategory();

	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);

};

#endif

