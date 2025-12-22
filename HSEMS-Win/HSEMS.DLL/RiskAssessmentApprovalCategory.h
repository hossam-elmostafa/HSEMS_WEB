// RiskAssessmentApprovalCategory.h: interface for the RiskAssessmentApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RISKASSESSMENTAPPROVALCATEGORY_H__266F3AF7_2166_402A_B4ED_0278B23E195D__INCLUDED_)
#define AFX_RISKASSESSMENTAPPROVALCATEGORY_H__266F3AF7_2166_402A_B4ED_0278B23E195D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RiskAssessmentCategory.h"

class RiskAssessmentApprovalCategory : public RiskAssessmentCategory  
{
public:
	RiskAssessmentApprovalCategory(CCategory *pCategory);
	virtual ~RiskAssessmentApprovalCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	
	void ApproveRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowRiskAssessmentApproval(CString strCriteria);
};

#endif // !defined(AFX_RISKASSESSMENTAPPROVALCATEGORY_H__266F3AF7_2166_402A_B4ED_0278B23E195D__INCLUDED_)
