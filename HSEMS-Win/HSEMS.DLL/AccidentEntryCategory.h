// AccidentEntryCategory.h: interface for the AccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTENTRYCATEGORY2_H__1A12E0E3_C6AF_4FED_847B_815548B721B3__INCLUDED_)
#define AFX_ACCIDENTENTRYCATEGORY2_H__1A12E0E3_C6AF_4FED_847B_815548B721B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccidentCategory.h"

class AccidentEntryCategory : public AccidentCategory  
{
public:
	//AccidentEntryCategory();
	AccidentEntryCategory(CCategory *pCategory):AccidentCategory(pCategory)
	{
		m_pCategory = pCategory;
	};
	virtual ~AccidentEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);

	void CompleteAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAccidentEntry(CString strCriteria);
	CString isFlashRprtRqrd(CString strCmpnyName);
	CString isValidForEntryComplete(CString strCmpnyName);
};

#endif // !defined(AFX_ACCIDENTENTRYCATEGORY2_H__1A12E0E3_C6AF_4FED_847B_815548B721B3__INCLUDED_)
