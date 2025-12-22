#include "SitSrvyCategory.h"

class SitSrvyEntCategory : public SitSrvyCategory  
{
public:
	SitSrvyEntCategory(CCategory *pCategory);
	virtual ~SitSrvyEntCategory();
	long ShowSitSrvyEnt(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CompleteSitSrvy(CUSTOMBUTTONCLICKED* pCustomButtonClicked);	
};


