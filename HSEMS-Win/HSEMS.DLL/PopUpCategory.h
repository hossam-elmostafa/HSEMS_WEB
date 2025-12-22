#ifndef RiskAssessmentHzrdPopCategory_HEADER
#define RiskAssessmentHzrdPopCategory_HEADER


#include "RiskAssesmentInquiry.h"

class CRiskAssessmentHzrdPopCategory : public RiskAssessmentCategory  
{
public:
	CRiskAssessmentHzrdPopCategory(CCategory *pCategory,CString strCriteria);
	virtual ~CRiskAssessmentHzrdPopCategory();

	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);

};

#endif

