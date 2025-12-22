#ifndef SitSrvyCategory_HEADER
#define SitSrvyCategory_HEADER


#include "HSEMSCommonCategory.h"

class SitSrvyCategory : public CHSEMSCommonCategory  
{
public:
	SitSrvyCategory(CCategory *pCategory);
	virtual ~SitSrvyCategory();
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void rejectRecord(CString strForm_Tag);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
};

#endif

