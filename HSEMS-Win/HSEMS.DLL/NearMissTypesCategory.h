#include "HSEMSCommonCategory.h"

class NearMissTypesCategory : public CHSEMSCommonCategory  
{
public:
	NearMissTypesCategory(CCategory *pCategory);
	virtual ~NearMissTypesCategory();	
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowNearMissTypes(CString strCriteria);
};


