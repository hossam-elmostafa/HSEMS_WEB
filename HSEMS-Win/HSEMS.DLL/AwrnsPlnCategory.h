#ifndef AwrnsPlnCategory_HEADER
#define AwrnsPlnCategory_HEADER


#include "HSEMSCommonCategory.h"

class AwrnsPlnCategory : public CHSEMSCommonCategory  
{
public:
	AwrnsPlnCategory(CCategory *pCategory);
	virtual ~AwrnsPlnCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange(FIELDCHANGE *pFieldChange);
	void enterAttendees(CString strAwrnsPlnYr,CString strSubFormTag) ;
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

};

#endif

