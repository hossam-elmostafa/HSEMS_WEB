// Auditess.h: interface for the Auditess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITESS_H__F1A46647_AFD3_423A_98E4_EF2C7BA72CB4__INCLUDED_)
#define AFX_AUDITESS_H__F1A46647_AFD3_423A_98E4_EF2C7BA72CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class Auditess  : public CHSEMSCommonCategory
{
public:
	Auditess(CCategory *pCategory);
	virtual ~Auditess();
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);

	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowAudites(CString strCriteria);
};

#endif // !defined(AFX_AUDITESS_H__F1A46647_AFD3_423A_98E4_EF2C7BA72CB4__INCLUDED_)
