// PotentialHazardCategory.h: interface for the PotentialHazardCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POTENTIALHAZARDCATEGORY_H__1EC96929_BB43_456E_B669_475A9158250F__INCLUDED_)
#define AFX_POTENTIALHAZARDCATEGORY_H__1EC96929_BB43_456E_B669_475A9158250F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class PotentialHazardCategory : public CHSEMSCommonCategory  
{
public:
	PotentialHazardCategory(CCategory *pCategory);
	virtual ~PotentialHazardCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void rejectRecord(CString strForm_Tag);	
};

#endif // !defined(AFX_POTENTIALHAZARDCATEGORY_H__1EC96929_BB43_456E_B669_475A9158250F__INCLUDED_)
