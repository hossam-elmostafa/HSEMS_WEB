// RiskAssessmentEntryCategory.h: interface for the RiskAssessmentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "RiskAssessmentCategory.h"
#if !defined(AFX_RISKASSESSMENTENTRYCATEGORY_H__857EAF96_60FD_44F4_B73E_1E5E59D277D0__INCLUDED_)
#define AFX_RISKASSESSMENTENTRYCATEGORY_H__857EAF96_60FD_44F4_B73E_1E5E59D277D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RiskAssessmentEntryCategory : public RiskAssessmentCategory  
{
public:
	RiskAssessmentEntryCategory(CCategory *pCategory);
	virtual ~RiskAssessmentEntryCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);

	void CompleteRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked);
	void OpenSeverityRatingTab(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick,CString SeverityClassName);
	long ShowRiskAssessmentEntry(CString strCriteria);	

private:
	bool CheckIdentifiedHazardFrame();
	void CurrentRiskAnalysisFrame(CString Attribute);
	bool CheckCurrentAnalysisFrame();
	void EnableNecessaryControlMeasure();
	void RskAnlsAfterNcsryCntrlMsur(CString Attrib);
	bool CheckRecordsInNecessaryControlMeasure(CString strKeyField);
	CString GetKeyFieldOfTab();
	void DeleteTabbContents();
	bool ReturnMesssageToFillAllData();	
	void EnableRiskAnalysisAfterControlMeasure(CString strKeyField);
	void ChangeTabDefaultValue();
	void OpenSeverityRatingTabFilteredonSelectedClass(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick,CString SeverityClassName);
	void SetScreenFormTag(CString strScreenTag) {m_strScreenTag = strScreenTag;};
	CString GetScreenFormTag() {return m_strScreenTag;};
	void getRskAsmntPtntlHzrd();		
	
};

#endif // !defined(AFX_RISKASSESSMENTENTRYCATEGORY_H__857EAF96_60FD_44F4_B73E_1E5E59D277D0__INCLUDED_)
