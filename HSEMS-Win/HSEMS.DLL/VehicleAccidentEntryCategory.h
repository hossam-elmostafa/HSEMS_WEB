// VehicleAccidentEntryCategory.h: interface for the VehicleAccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEHICLEACCIDENTENTRYCATEGORY_H__9FB86702_9BE5_4930_AE25_A14AD90D1727__INCLUDED_)
#define AFX_VEHICLEACCIDENTENTRYCATEGORY_H__9FB86702_9BE5_4930_AE25_A14AD90D1727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VehicleAccident.h"

class VehicleAccidentEntryCategory : public VehicleAccident  
{
public:
	VehicleAccidentEntryCategory(CCategory *pCategory);
	virtual ~VehicleAccidentEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	void CompleteVehicleAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowVehicleAccidentEntry(CString strCriteria);

};

#endif // !defined(AFX_VEHICLEACCIDENTENTRYCATEGORY_H__9FB86702_9BE5_4930_AE25_A14AD90D1727__INCLUDED_)
