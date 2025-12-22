#include "SitSrvyCategory.h"

class SitSrvyFlwupCategory : public SitSrvyCategory  
{
public:
	SitSrvyFlwupCategory(CCategory *pCategory);
	virtual ~SitSrvyFlwupCategory();
	long ShowSitSrvyFlwup(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CloseSiteSurveyTXN(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};


