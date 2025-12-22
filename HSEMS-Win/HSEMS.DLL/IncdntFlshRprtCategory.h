// IncdntFlshRprtCategory.h: interface for the IncdntFlshRprtCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INCDNTFLSHRPRTCATEGORY)
#define AFX_INCDNTFLSHRPRTCATEGORY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class IncdntFlshRprtCategory : public CHSEMSCommonCategory  
{
public:
	IncdntFlshRprtCategory(CCategory *pCategory);
	virtual ~IncdntFlshRprtCategory();	
	long ShowIncdntFlshRprt(CString strCriteria);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange(FIELDCHANGE *pFieldChange);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	long ShowIncdntFlshRprtInq(CString strCriteria);
	long ShowIncdntFlshRprtRvw(CString strCriteria);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	void CompleteFlashEntry(CString strPrmryKey);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	bool isEdtFlshRvwAlwd();
	void rejectFlashFromReview(CString strPrmryKey); 
	void accptFlashFromReview(CString strPrmryKey); 
	bool isMultiCompany();
	CString GetCompanyName();
};

#endif // !defined(AFX_HAZARDCATEGORY_H__99070C69_1BD6_4FBF_B770_52F5D88ECA9B__INCLUDED_)
