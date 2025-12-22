#include "HSEMSCommonCategory.h"

class JobPtntlHzrdsLib : public CHSEMSCommonCategory  
{
public:
	JobPtntlHzrdsLib(CCategory *pCategory);
	virtual ~JobPtntlHzrdsLib();		
	long ShowJobPtntlHzrdsLib(CString strCriteria);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
};

