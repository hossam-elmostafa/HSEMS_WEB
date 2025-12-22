#ifndef AwrnsPlnCrsInstrctrCategory_HEADER
#define AwrnsPlnCrsInstrctrCategory_HEADER


#include "AwrnsPlnCategory.h"

class CAwrnsPlnCrsInstrctrCategory : public AwrnsPlnCategory  
{
public:
	CAwrnsPlnCrsInstrctrCategory(CCategory *pCategory,CString strCriteria);
	virtual ~CAwrnsPlnCrsInstrctrCategory();

	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);

};

#endif

