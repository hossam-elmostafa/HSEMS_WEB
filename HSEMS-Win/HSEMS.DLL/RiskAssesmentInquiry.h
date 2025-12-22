// RiskAssesmentInquiry.h: interface for the RiskAssesmentInquiry class.
//
//////////////////////////////////////////////////////////////////////
#include "RiskAssessmentCategory.h"
#if !defined(AFX_RISKASSESMENTINQUIRY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_)
#define AFX_RISKASSESMENTINQUIRY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RiskAssesmentInquiry  : public RiskAssessmentCategory  
{
public:
	RiskAssesmentInquiry(CCategory *pCategory);
	virtual ~RiskAssesmentInquiry();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowRiskInquiry(CString strCriteria);

};

#endif // !defined(AFX_RISKASSESMENTINQUIRY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_)
