// HazardCategory.h: interface for the HazardCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HAZARDCATEGORY_H__99070C69_1BD6_4FBF_B770_52F5D88ECA9B__INCLUDED_)
#define AFX_HAZARDCATEGORY_H__99070C69_1BD6_4FBF_B770_52F5D88ECA9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class HazardCategory : public CHSEMSCommonCategory  
{
public:
	HazardCategory(CCategory *pCategory);
	virtual ~HazardCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowHazard(CString strCriteria);
};

#endif // !defined(AFX_HAZARDCATEGORY_H__99070C69_1BD6_4FBF_B770_52F5D88ECA9B__INCLUDED_)
