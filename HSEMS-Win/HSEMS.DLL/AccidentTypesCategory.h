#include "HSEMSCommonCategory.h"

class AccidentTypesCategory : public CHSEMSCommonCategory  
{
public:
	AccidentTypesCategory(CCategory *pCategory);
	virtual ~AccidentTypesCategory();	
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowAccidentTypes(CString strCriteria);
};


