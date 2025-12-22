#include "AwrnsPlnCategory.h"

class AwrnsPlnEntryCategory : public AwrnsPlnCategory  
{
public:
	AwrnsPlnEntryCategory(CCategory *pCategory);
	virtual ~AwrnsPlnEntryCategory();
	long ShowAwrnsPlnEntry(CString strCriteria);		
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	void completeEntry(CString strAwrnsPlnYr);
};


