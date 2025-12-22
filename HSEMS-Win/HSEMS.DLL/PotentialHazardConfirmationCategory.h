// PotentialHazardConfirmationCategory.h: interface for the PotentialHazardConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTENTIALHAZARDCONFIRMATIONCATEGORY_H__E449362D_2798_4F28_98E0_2ED0382787F6__INCLUDED_)
#define AFX_POTENTIALHAZARDCONFIRMATIONCATEGORY_H__E449362D_2798_4F28_98E0_2ED0382787F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PotentialHazardCategory.h"

class PotentialHazardConfirmationCategory : public PotentialHazardCategory  
{
public:
	PotentialHazardConfirmationCategory(CCategory *pCategory);
	virtual ~PotentialHazardConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);	
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	long ShowPotentialHazardConfirmation(CString strCriteria);
	void ConfirmPotential(tag_CustomButtonClicked* pCustomButtonClicked);
};

#endif // !defined(AFX_POTENTIALHAZARDCONFIRMATIONCATEGORY_H__E449362D_2798_4F28_98E0_2ED0382787F6__INCLUDED_)
