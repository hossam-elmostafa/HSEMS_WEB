// AuditCheckListResultCategory.h: interface for the AuditCheckListResultCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITCHECKLISTRESULTCATEGORY_H__C5BD1D51_628D_4711_BEF9_A549849ABD03__INCLUDED_)
#define AFX_AUDITCHECKLISTRESULTCATEGORY_H__C5BD1D51_628D_4711_BEF9_A549849ABD03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class AuditCheckListResultCategory : public CHSEMSCommonCategory  
{
public:
	AuditCheckListResultCategory(CCategory *pCategory);
	virtual ~AuditCheckListResultCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);

	long ShowAuditCheckListResult(CString strCriteria);
};

#endif // !defined(AFX_AUDITCHECKLISTRESULTCATEGORY_H__C5BD1D51_628D_4711_BEF9_A549849ABD03__INCLUDED_)
