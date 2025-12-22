// NearMissEntryCategory.h: interface for the NearMissEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSENTRYCATEGORY_H__1659D094_0D5A_4C94_9BDA_D1273AD56331__INCLUDED_)
#define AFX_NEARMISSENTRYCATEGORY_H__1659D094_0D5A_4C94_9BDA_D1273AD56331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IEmailSupport.h"
#include "NearMissCategory.h"


class NearMissEntryCategory : public NearMissCategory,public IEmailSupport
{
public:
	NearMissEntryCategory(CCategory *pCategory);
	virtual ~NearMissEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	bool CompleteNearMiss(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowNearMissEntry(CString strCriteria);
	CString getScreenFldVal(CString strFldName);
	bool isObservationTabsEnabled();
};

#endif // !defined(AFX_NEARMISSENTRYCATEGORY_H__1659D094_0D5A_4C94_9BDA_D1273AD56331__INCLUDED_)
