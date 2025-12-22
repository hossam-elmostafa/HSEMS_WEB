// arConfirmationCategory.h: interface for the CarConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCONFIRMATIONCATEGORY_H__53373EF9_D610_4C5C_850D_362E0EDE15B3__INCLUDED_)
#define AFX_ARCONFIRMATIONCATEGORY_H__53373EF9_D610_4C5C_850D_362E0EDE15B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CARCategory.h"

class CarConfirmationCategory : public CarCategory  
{
public:
	CarConfirmationCategory(CCategory *pCategory);
	virtual ~CarConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void ConfirmCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowCorrectiveActionConfirmation(CString strCriteria);
	//void RejectCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked);
	//void CancelCorrectiveActionConfirmation(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_ARCONFIRMATIONCATEGORY_H__53373EF9_D610_4C5C_850D_362E0EDE15B3__INCLUDED_)
