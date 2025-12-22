// AuditPlanResultEntryCategory.h: interface for the AuditPlanResultEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANRESULTENTRYCATEGORY_H__433E7CFE_97F3_4DF8_842A_AC7752D1CC03__INCLUDED_)
#define AFX_AUDITPLANRESULTENTRYCATEGORY_H__433E7CFE_97F3_4DF8_842A_AC7752D1CC03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditCategory.h"

class AuditPlanResultEntryCategory : public AuditCategory  
{
public:
	AuditPlanResultEntryCategory(CCategory *pCategory);
	virtual ~AuditPlanResultEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

};

#endif // !defined(AFX_AUDITPLANRESULTENTRYCATEGORY_H__433E7CFE_97F3_4DF8_842A_AC7752D1CC03__INCLUDED_)
