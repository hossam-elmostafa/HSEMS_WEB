// AuditPlanEntryCategory.h: interface for the AuditPlanEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANENTRYCATEGORY_H__4952523A_4005_4EF1_A758_2E50F534B49E__INCLUDED_)
#define AFX_AUDITPLANENTRYCATEGORY_H__4952523A_4005_4EF1_A758_2E50F534B49E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditCategory.h"

class AuditPlanEntryCategory : public AuditCategory  
{
public:
	AuditPlanEntryCategory(CCategory *pCategory);
	virtual ~AuditPlanEntryCategory();
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);	
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);

	void DuplicateActvtTab();
	CString OpenWorkLocationFilteredOnSite();
	CString OpenWotkLocationsTabFilteredOnLocationType();
	bool CompleteAudit(CUSTOMBUTTONCLICKED* pCustomButtonClicked);	
	void AddUnplanedActivity();
	void OpenAuditCheckListScreen();
	//void OpenAuditorsScreen();
	void OpenAuditeesScreen();
	void RturnActvtyTabToDfultBhv();
	long ShowAuditPlanEntry(CString strCriteria);


};

#endif // !defined(AFX_AUDITPLANENTRYCATEGORY_H__4952523A_4005_4EF1_A758_2E50F534B49E__INCLUDED_)
