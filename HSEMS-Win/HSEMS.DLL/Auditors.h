// Auditors.h: interface for the Auditors class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITORS_H__8B912763_AF00_4C35_919F_452D288B4BE4__INCLUDED_)
#define AFX_AUDITORS_H__8B912763_AF00_4C35_919F_452D288B4BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "HSEMSCommonCategory.h"
class Auditors   : public CHSEMSCommonCategory
{
public:
	Auditors(CCategory *pCategory);
	virtual ~Auditors();
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);	
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowAuditors(CString strCriteria);
	long ShowActualAuditors(CString strCriteria);
};

#endif // !defined(AFX_AUDITORS_H__8B912763_AF00_4C35_919F_452D288B4BE4__INCLUDED_)
