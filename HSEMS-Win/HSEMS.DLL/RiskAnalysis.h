// RiskAnalysis.h: interface for the RiskAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RISKANALYSIS_H__F7EB35B5_EF6C_4A7E_BE27_4FBCABE54D68__INCLUDED_)
#define AFX_RISKANALYSIS_H__F7EB35B5_EF6C_4A7E_BE27_4FBCABE54D68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class RiskAnalysis : public CHSEMSCommonCategory  
{
public:
	RiskAnalysis(CCategory *pCategory);
	virtual ~RiskAnalysis();
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void filterSvrtDscrOnRskCls(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void chkRnkAftrPrfrmncMsrs(FIELDCHANGE *pFieldChange);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	void calcRnkBfrPrfrmncMsre(CString strFullRecord);
	void calcRnkAfrPrfrmncMsre(CString strFullRecord);
	void RetriveRiskRankDes();	
	CString OpenNcsryCntrlMsure(CString strFormTag);
	void fltrCntrlMsrOnGrp(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void fltrGrpOnHrchyCntrl(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void emptySubLevels(CString strFieldName);	
	long ShowRiskAnalysis(CString strCriteria);
};

#endif // !defined(AFX_RISKANALYSIS_H__F7EB35B5_EF6C_4A7E_BE27_4FBCABE54D68__INCLUDED_)
