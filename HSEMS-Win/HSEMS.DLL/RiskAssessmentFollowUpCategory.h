// RiskAssessmentFollowUpCategory.h: interface for the RiskAssessmentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RISKASSESSMENTFOLLOWUPCATEGORY_H__6E25C046_5AE3_4C8B_9C2D_EB95EBEF447B__INCLUDED_)
#define AFX_RISKASSESSMENTFOLLOWUPCATEGORY_H__6E25C046_5AE3_4C8B_9C2D_EB95EBEF447B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RiskAssessmentCategory.h"

class RiskAssessmentFollowUpCategory : public RiskAssessmentCategory  
{
public:
	RiskAssessmentFollowUpCategory(CCategory *pCategory);
	virtual ~RiskAssessmentFollowUpCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	void CloseRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowRiskAssessmentFollowUp(CString strCriteria);
};

#endif // !defined(AFX_RISKASSESSMENTFOLLOWUPCATEGORY_H__6E25C046_5AE3_4C8B_9C2D_EB95EBEF447B__INCLUDED_)
