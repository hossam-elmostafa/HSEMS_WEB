// PtwEntryCategory.h: interface for the PtwEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTWENTRYCATEGORY_H__DF56FB02_D85D_4E07_BD3E_7C5B2A700C92__INCLUDED_)
#define AFX_PTWENTRYCATEGORY_H__DF56FB02_D85D_4E07_BD3E_7C5B2A700C92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtwCategory.h"

class PtwEntryCategory : public PtwCategory  
{
public:
	PtwEntryCategory(CCategory *pCategory);
	virtual ~PtwEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);

	void CompletePTW(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPTWRegisterEntry(CString strCriteria,CString strScreenDefaults);
};

#endif // !defined(AFX_PTWENTRYCATEGORY_H__DF56FB02_D85D_4E07_BD3E_7C5B2A700C92__INCLUDED_)
