// PerformanceMEasurementInquiry.h: interface for the PerformanceMEasurementInquiry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFORMANCEMEASUREMENTINQUIRY_H__C34479F6_D745_45C2_9071_95A38C2868C1__INCLUDED_)
#define AFX_PERFORMANCEMEASUREMENTINQUIRY_H__C34479F6_D745_45C2_9071_95A38C2868C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerformanceMeasurementCategory.h"

class PerformanceMEasurementInquiry  : public PerformanceMeasurementCategory 
{
public:
	PerformanceMEasurementInquiry(CCategory *pCategory);
	virtual ~PerformanceMEasurementInquiry();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPerformanceInquiry(CString strCriteria);

};

#endif // !defined(AFX_PERFORMANCEMEASUREMENTINQUIRY_H__C34479F6_D745_45C2_9071_95A38C2868C1__INCLUDED_)
