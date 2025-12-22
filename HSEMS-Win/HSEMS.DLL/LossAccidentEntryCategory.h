// LossAccidentEntryCategory.h: interface for the LossAccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOSSACCIDENTENTRYCATEGORY_H__CE5F1369_283C_4C90_A9A7_77D413FBD9C3__INCLUDED_)
#define AFX_LOSSACCIDENTENTRYCATEGORY_H__CE5F1369_283C_4C90_A9A7_77D413FBD9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LossAccidentCategory.h"

class LossAccidentEntryCategory : public LossAccidentCategory  
{
public:
	LossAccidentEntryCategory(CCategory *pCategory);
	virtual ~LossAccidentEntryCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void EmptyEmployeeFields();
	void CompleteLossAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void retrievSiteType();
	long ShowLossAccidentEntry(CString strCriteria);
};

#endif // !defined(AFX_LOSSACCIDENTENTRYCATEGORY_H__CE5F1369_283C_4C90_A9A7_77D413FBD9C3__INCLUDED_)
