#ifndef AwrnsPlnCrsTrgtDprtCategory_HEADER
#define AwrnsPlnCrsTrgtDprtCategory_HEADER


#include "AwrnsPlnCategory.h"

class CAwrnsPlnCrsTrgtDprtCategory : public AwrnsPlnCategory  
{
public:
	CAwrnsPlnCrsTrgtDprtCategory(CCategory *pCategory,CString strCriteria);
	virtual ~CAwrnsPlnCrsTrgtDprtCategory();

	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);

};

#endif

