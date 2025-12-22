#include "HSEMSCommonCategory.h"

class JobCategory : public CHSEMSCommonCategory  
{
public:
	JobCategory(CCategory *pCategory);
	virtual ~JobCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);	
	long ShowJob(CString strCriteria);
};

