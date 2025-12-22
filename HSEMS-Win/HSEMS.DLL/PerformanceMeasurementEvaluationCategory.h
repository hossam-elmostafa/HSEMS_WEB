// PerformanceMeasurementEvaluationCategory.h: interface for the PerformanceMeasurementEvaluationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFORMANCEMEASUREMENTEVALUATIONCATEGORY_H__BC48552A_95FF_443E_B73C_D88B1CF8ACC4__INCLUDED_)
#define AFX_PERFORMANCEMEASUREMENTEVALUATIONCATEGORY_H__BC48552A_95FF_443E_B73C_D88B1CF8ACC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerformanceMeasurementCategory.h"

class PerformanceMeasurementEvaluationCategory : public PerformanceMeasurementCategory  
{
public:
	PerformanceMeasurementEvaluationCategory(CCategory *pCategory);
	virtual ~PerformanceMeasurementEvaluationCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);

	void CompletePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPerformanceMeasurementEvaluation(CString strCriteria);
};

#endif // !defined(AFX_PERFORMANCEMEASUREMENTEVALUATIONCATEGORY_H__BC48552A_95FF_443E_B73C_D88B1CF8ACC4__INCLUDED_)
