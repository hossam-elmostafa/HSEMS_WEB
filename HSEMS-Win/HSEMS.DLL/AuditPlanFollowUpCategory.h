// AuditPlanFollowUpCategory.h: interface for the AuditPlanFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANFOLLOWUPCATEGORY_H__761D370B_E7B5_4ECB_BDDB_AC59B207734B__INCLUDED_)
#define AFX_AUDITPLANFOLLOWUPCATEGORY_H__761D370B_E7B5_4ECB_BDDB_AC59B207734B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AuditCategory.h"

class AuditPlanFollowUpCategory : public AuditCategory   
{
public:
	AuditPlanFollowUpCategory(CCategory *pCategory);
	virtual ~AuditPlanFollowUpCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	
	void CloseAudit(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAuditPlanFlwUp(CString strCriteria);
};

#endif // !defined(AFX_AUDITPLANFOLLOWUPCATEGORY_H__761D370B_E7B5_4ECB_BDDB_AC59B207734B__INCLUDED_)
