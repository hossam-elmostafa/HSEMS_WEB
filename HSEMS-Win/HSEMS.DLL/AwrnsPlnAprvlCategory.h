#include "AwrnsPlnCategory.h"

class AwrnsPlnAprvlCategory : public AwrnsPlnCategory  
{
public:
	AwrnsPlnAprvlCategory(CCategory *pCategory);
	virtual ~AwrnsPlnAprvlCategory();
	long ShowAwrnsPlnAprvl(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void approveAwrnsPln(CString strAwrnsPlnYr); 
	void rjctAwrnsPln(CString strAwrnsPlnYr,CString strRjctRsn);
};


