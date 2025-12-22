// LossAccidentConfirmationCategory.h: interface for the LossAccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOSSACCIDENTCONFIRMATIONCATEGORY_H__488F6CD7_6C4E_46EE_ACFC_45B7D2D5C851__INCLUDED_)
#define AFX_LOSSACCIDENTCONFIRMATIONCATEGORY_H__488F6CD7_6C4E_46EE_ACFC_45B7D2D5C851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LossAccidentCategory.h"

class LossAccidentConfirmationCategory : public LossAccidentCategory  
{
public:
	LossAccidentConfirmationCategory(CCategory *pCategory);
	virtual ~LossAccidentConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

	void ConfirmLossAccident(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowLossAccidentConfirmation(CString strCriteria);

};

#endif // !defined(AFX_LOSSACCIDENTCONFIRMATIONCATEGORY_H__488F6CD7_6C4E_46EE_ACFC_45B7D2D5C851__INCLUDED_)
