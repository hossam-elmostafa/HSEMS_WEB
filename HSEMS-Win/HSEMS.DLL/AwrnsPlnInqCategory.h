#include "AwrnsPlnCategory.h"

class AwrnsPlnInqCategory : public AwrnsPlnCategory  
{
public:
	AwrnsPlnInqCategory(CCategory *pCategory);
	virtual ~AwrnsPlnInqCategory();
	long ShowAwrnsPlnInq(CString strCriteria);		
};


