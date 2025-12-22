// RejectReason.h: interface for the CRejectReason class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REJECTREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
#define AFX_REJECTREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CRejectReason : public CHSEMSCommonCategory  
{
public:
	CRejectReason(CCategory *pCategory);
	virtual ~CRejectReason();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayScreenReady(SCREENREADY *pSreenReady);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowRejectreason(CString strCriteria);
	
	
	void SetScreenAttrib();
	void RejectReasonOk();
};

#endif // !defined(AFX_REJECTREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
