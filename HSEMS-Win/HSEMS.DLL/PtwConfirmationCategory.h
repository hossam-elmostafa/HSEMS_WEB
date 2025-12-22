// PtwConfirmationCategory.h: interface for the PtwConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTWCONFIRMATIONCATEGORY_H__D0DDEEEE_58F1_42AE_97D4_F5DEC895AFEE__INCLUDED_)
#define AFX_PTWCONFIRMATIONCATEGORY_H__D0DDEEEE_58F1_42AE_97D4_F5DEC895AFEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtwCategory.h"

class PtwConfirmationCategory : public PtwCategory  
{
public:
	PtwConfirmationCategory(CCategory *pCategory);
	virtual ~PtwConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

	void ConfirmPTW(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowPTWRegisterConfirmation(CString strCriteria);

};

#endif // !defined(AFX_PTWCONFIRMATIONCATEGORY_H__D0DDEEEE_58F1_42AE_97D4_F5DEC895AFEE__INCLUDED_)
