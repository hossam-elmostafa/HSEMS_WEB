/**
 *	Handle Log definition screen
 *
 */
#include "HSEMSCommonCategory.h"

class LogsDefCategory : public CHSEMSCommonCategory  
{
public:
	LogsDefCategory(CCategory *pCategory);
	virtual ~LogsDefCategory();
	long ShowHSELogsDef(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
};

