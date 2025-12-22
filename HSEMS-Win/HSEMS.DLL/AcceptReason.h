// AcceptReason.h: interface for the CAcceptReason class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AcceptREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
#define AFX_AcceptREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CAcceptReason : public CHSEMSCommonCategory  
{
public:
	CAcceptReason(CCategory *pCategory);
	virtual ~CAcceptReason();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayScreenReady(SCREENREADY *pSreenReady);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowAcceptreason(CString strCriteria);
	long ShowApprovReason(CString strCriteria);
	
	
	void SetScreenAttrib();
	void AcceptReasonOk();

	virtual void OnScreenExit();

private:
	bool CheckForEmptyRecord(CString strFullRecord);
};

#endif // !defined(AFX_AcceptREASON_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
