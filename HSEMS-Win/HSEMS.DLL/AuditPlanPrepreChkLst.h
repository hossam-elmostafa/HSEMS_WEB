// AuditPlanPrepreChkLst.h: interface for the AuditPlanPrepreChkLst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANPREPRECHKLST_H__44E6C032_2B09_4521_9276_DAF60AE775E6__INCLUDED_)
#define AFX_AUDITPLANPREPRECHKLST_H__44E6C032_2B09_4521_9276_DAF60AE775E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AuditCategory.h"

class AuditPlanPrepreChkLst  : public AuditCategory
{
public:
	AuditPlanPrepreChkLst(CCategory *pCategory);
	virtual ~AuditPlanPrepreChkLst();

	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	//void OpenAuditCheckListScreen();
	void CompleteAuditWithAuditChkLst(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAuditPlanPreparechecklist(CString strCriteria);

};

#endif // !defined(AFX_AUDITPLANPREPRECHKLST_H__44E6C032_2B09_4521_9276_DAF60AE775E6__INCLUDED_)
