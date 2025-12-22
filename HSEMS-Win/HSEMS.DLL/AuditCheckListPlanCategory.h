// AuditCheckListPlanCategory.h: interface for the AuditCheckListPlanCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITCHECKLISTPLANCATEGORY_H__B54E6901_587A_45B8_A10C_12371458BB14__INCLUDED_)
#define AFX_AUDITCHECKLISTPLANCATEGORY_H__B54E6901_587A_45B8_A10C_12371458BB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HSEMSCommonCategory.h"
//sdfgwssdfsgs/lknmsg
//sdffgdfg

class AuditCheckListPlanCategory : public CHSEMSCommonCategory  
{
public:
	AuditCheckListPlanCategory(CCategory *pCategory);
	virtual ~AuditCheckListPlanCategory();
	long ShowAuditCheckListPlan(CString strCriteria);

};

#endif // !defined(AFX_AUDITCHECKLISTPLANCATEGORY_H__B54E6901_587A_45B8_A10C_12371458BB14__INCLUDED_)
