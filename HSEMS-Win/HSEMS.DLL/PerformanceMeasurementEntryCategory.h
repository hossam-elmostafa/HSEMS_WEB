// PerformanceMeasurementEntryCategory.h: interface for the PerformanceMeasurementEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFORMANCEMEASUREMENTENTRYCATEGORY_H__13717A8C_A4A9_4C42_98D0_CEA138BF50D4__INCLUDED_)
#define AFX_PERFORMANCEMEASUREMENTENTRYCATEGORY_H__13717A8C_A4A9_4C42_98D0_CEA138BF50D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerformanceMeasurementCategory.h"

class PerformanceMeasurementEntryCategory : public PerformanceMeasurementCategory  
{
public:
	PerformanceMeasurementEntryCategory(CCategory *pCategory);
	virtual ~PerformanceMeasurementEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	bool CompletePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	//void retrievSiteType();	replaced by driven field
	long ShowPerformanceMeasurementEntry(CString strCriteria);

private:
	void SelectSectionFilteredSelectedGroup(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	
};

#endif // !defined(AFX_PERFORMANCEMEASUREMENTENTRYCATEGORY_H__13717A8C_A4A9_4C42_98D0_CEA138BF50D4__INCLUDED_)
