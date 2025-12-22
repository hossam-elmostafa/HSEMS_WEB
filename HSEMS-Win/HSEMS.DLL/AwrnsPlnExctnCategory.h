#include "AwrnsPlnCategory.h"

class AwrnsPlnExctnCategory : public AwrnsPlnCategory  
{
public:
	AwrnsPlnExctnCategory(CCategory *pCategory);
	virtual ~AwrnsPlnExctnCategory();
	long ShowAwrnsPlnExctn(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	void createUnPlndCrs();
	void closeAwrnsPln(CString strAwrnsPlnYr);
};


