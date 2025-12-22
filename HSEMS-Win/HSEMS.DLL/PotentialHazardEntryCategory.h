// PotentialHazardEntryCategory.h: interface for the PotentialHazardEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTENTIALHAZARDENTRYCATEGORY_H__3E4F84A4_B40D_44F1_A337_ABA114B43C1C__INCLUDED_)
#define AFX_POTENTIALHAZARDENTRYCATEGORY_H__3E4F84A4_B40D_44F1_A337_ABA114B43C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PotentialHazardCategory.h"

class PotentialHazardEntryCategory : public PotentialHazardCategory  
{
public:
	PotentialHazardEntryCategory(CCategory *pCategory);
	virtual ~PotentialHazardEntryCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);	
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowPotentialHazardEntry(CString strCriteria);

	CString GetSevirityRatingCriteria();
	void CompletePotential(tag_CustomButtonClicked* pCustomButtonClicked);
	void retrievRiskRankDesc(FIELDCHANGE *pFieldChange);
	BOOL CalcRiskRank();
	void retrievSiteType();
};

#endif // !defined(AFX_POTENTIALHAZARDENTRYCATEGORY_H__3E4F84A4_B40D_44F1_A337_ABA114B43C1C__INCLUDED_)
