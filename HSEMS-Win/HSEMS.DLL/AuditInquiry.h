// AuditInquiry.h: interface for the AuditInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "AuditCategory.h"
#if !defined(AFX_AUDITINQUIRY_H__820FE8A4_B7B3_4288_B623_80B15F1DE7F5__INCLUDED_)
#define AFX_AUDITINQUIRY_H__820FE8A4_B7B3_4288_B623_80B15F1DE7F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AuditInquiry  : public AuditCategory  
{
public:
	AuditInquiry(CCategory *pCategory);
	virtual ~AuditInquiry();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAudit(CString strCriteria);

};

#endif // !defined(AFX_AUDITINQUIRY_H__820FE8A4_B7B3_4288_B623_80B15F1DE7F5__INCLUDED_)
