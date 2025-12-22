// AccidentConfirmationCategory.h: interface for the AccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTCONFIRMATIONCATEGORY_H__CC496798_A559_4A7C_AAAF_67403AB3C147__INCLUDED_)
#define AFX_ACCIDENTCONFIRMATIONCATEGORY_H__CC496798_A559_4A7C_AAAF_67403AB3C147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccidentCategory.h"

class AccidentConfirmationCategory : public AccidentCategory  
{
public:
	AccidentConfirmationCategory(CCategory *pCategory);
	virtual ~AccidentConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	
	void ConfirmAccident(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowAccidentConfirmation(CString strCriteria);
};

#endif // !defined(AFX_ACCIDENTCONFIRMATIONCATEGORY_H__CC496798_A559_4A7C_AAAF_67403AB3C147__INCLUDED_)
