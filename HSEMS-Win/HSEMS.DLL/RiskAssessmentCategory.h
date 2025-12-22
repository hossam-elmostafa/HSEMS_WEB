// RiskAssessmentCategory.h: interface for the RiskAssessmentCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RISKASSESSMENTCATEGORY_H__22D2B65E_64C1_4C6A_8217_2AF70D369060__INCLUDED_)
#define AFX_RISKASSESSMENTCATEGORY_H__22D2B65E_64C1_4C6A_8217_2AF70D369060__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class RiskAssessmentCategory : public CHSEMSCommonCategory
{
public:
	RiskAssessmentCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
	{
		m_pCategory = pCategory;
	};	
	virtual ~RiskAssessmentCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	void RetriveRiskRankDes();
	BOOL CalcCurrRiskRank();
	//BOOL CalcRiskRank();
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	void rejectRecord(CString strForm_Tag);
	CString OpenCntrlMsure(CString strFormTag);
	void OpnEffectsScr(CString strForm_Tag);
	void OpnRskAnlysScr(CString strKeyField,bool bLocked);
	void getRskAssmntJobSteps(CString strRskAssmntKey);
	void checkRiskAssessment(CString strRskAssmntKey);
};

#endif // !defined(AFX_RISKASSESSMENTCATEGORY_H__22D2B65E_64C1_4C6A_8217_2AF70D369060__INCLUDED_)
