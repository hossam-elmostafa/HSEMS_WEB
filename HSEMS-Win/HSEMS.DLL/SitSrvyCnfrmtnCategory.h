#include "SitSrvyCategory.h"

class SitSrvyCnfrmtnCategory : public SitSrvyCategory  
{
public:
	SitSrvyCnfrmtnCategory(CCategory *pCategory);
	virtual ~SitSrvyCnfrmtnCategory();
	long ShowSitSrvyCnfrmtn(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void confirmSiteSrvy(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
};


