// AuditPlanApprovalCategory.h: interface for the AuditPlanApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANAPPROVALCATEGORY_H__18225FE2_DE03_44B6_B26B_ABD7F750C51C__INCLUDED_)
#define AFX_AUDITPLANAPPROVALCATEGORY_H__18225FE2_DE03_44B6_B26B_ABD7F750C51C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditCategory.h"

class AuditPlanApprovalCategory : public AuditCategory  
{
public:
	AuditPlanApprovalCategory(CCategory *pCategory);
	virtual ~AuditPlanApprovalCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void openAuditChecklistResult();
	void ApproveAudit(CString strAdtyr,tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowAuditPlanApproval(CString strCriteria);
};

#endif // !defined(AFX_AUDITPLANAPPROVALCATEGORY_H__18225FE2_DE03_44B6_B26B_ABD7F750C51C__INCLUDED_)
