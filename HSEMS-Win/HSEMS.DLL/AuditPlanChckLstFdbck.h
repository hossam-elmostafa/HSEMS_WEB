// AuditPlanChckLstFdbck.h: interface for the AuditPlanChckLstFdbck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANCHCKLSTFDBCK_H__A5B04B69_52C0_4D83_AD2A_89A955AA0900__INCLUDED_)
#define AFX_AUDITPLANCHCKLSTFDBCK_H__A5B04B69_52C0_4D83_AD2A_89A955AA0900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AuditCategory.h"

class AuditPlanChckLstFdbck  : public AuditCategory
{
public:
	AuditPlanChckLstFdbck(CCategory *pCategory);
	virtual ~AuditPlanChckLstFdbck();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	//void openAuditChecklistResult();
	void CloseAuditWithAdtChkLstFdbck(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void closeActivity(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowAuditPlanAuditchecklistfeedback(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
};

#endif // !defined(AFX_AUDITPLANCHCKLSTFDBCK_H__A5B04B69_52C0_4D83_AD2A_89A955AA0900__INCLUDED_)
