// AuditResultApprovalCategory.h: interface for the AuditResultApprovalCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITRESULTAPPROVALCATEGORY_H__0C77BB4C_6F7C_499D_AE20_FAD1F7CBECC0__INCLUDED_)
#define AFX_AUDITRESULTAPPROVALCATEGORY_H__0C77BB4C_6F7C_499D_AE20_FAD1F7CBECC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditCategory.h"

class AuditResultApprovalCategory : public AuditCategory  
{
public:
	AuditResultApprovalCategory(CCategory *pCategory);
	virtual ~AuditResultApprovalCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

};

#endif // !defined(AFX_AUDITRESULTAPPROVALCATEGORY_H__0C77BB4C_6F7C_499D_AE20_FAD1F7CBECC0__INCLUDED_)
