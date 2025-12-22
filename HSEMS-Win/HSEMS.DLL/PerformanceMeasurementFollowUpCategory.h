// PerformanceMeasurementFollowUpCategory.h: interface for the PerformanceMeasurementFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFORMANCEMEASUREMENTFOLLOWUPCATEGORY_H__4D8C706D_D82C_4679_A9F5_721C73C0E53E__INCLUDED_)
#define AFX_PERFORMANCEMEASUREMENTFOLLOWUPCATEGORY_H__4D8C706D_D82C_4679_A9F5_721C73C0E53E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerformanceMeasurementCategory.h"

class PerformanceMeasurementFollowUpCategory : public PerformanceMeasurementCategory  
{
public:
	PerformanceMeasurementFollowUpCategory(CCategory *pCategory);
	virtual ~PerformanceMeasurementFollowUpCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	void ClosePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowPerformanceMeasurementFollowUp(CString strCriteria);
};

#endif // !defined(AFX_PERFORMANCEMEASUREMENTFOLLOWUPCATEGORY_H__4D8C706D_D82C_4679_A9F5_721C73C0E53E__INCLUDED_)
