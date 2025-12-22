// PotentialHazardFollowUp.h: interface for the PotentialHazardFollowUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTENTIALHAZARDFOLLOWUP_H__41A15DED_8921_4CE5_A520_18FD9AC3AFD9__INCLUDED_)
#define AFX_POTENTIALHAZARDFOLLOWUP_H__41A15DED_8921_4CE5_A520_18FD9AC3AFD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PotentialHazardCategory.h"

class PotentialHazardFollowUp : public PotentialHazardCategory  
{
public:
	PotentialHazardFollowUp(CCategory *pCategory);
	virtual ~PotentialHazardFollowUp();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);	
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	long ShowPotentialhazardFollowUp(CString strCriteria);

	void ClosePotential(tag_CustomButtonClicked* pCustomButtonClicked);

};

#endif // !defined(AFX_POTENTIALHAZARDFOLLOWUP_H__41A15DED_8921_4CE5_A520_18FD9AC3AFD9__INCLUDED_)
