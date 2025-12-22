#include "SitSrvyCategory.h"

class SitSrvyInqCategory : public SitSrvyCategory  
{
public:
	SitSrvyInqCategory(CCategory *pCategory);
	virtual ~SitSrvyInqCategory();
	long ShowSitSrvyEntInq(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
};


