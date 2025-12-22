// ApproveCAR.h: interface for the CApproveCAR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ApproveCAR_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
#define AFX_ApproveCAR_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CApproveCAR : public CHSEMSCommonCategory  
{
public:
	CApproveCAR(CCategory *pCategory);
	virtual ~CApproveCAR();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayScreenReady(SCREENREADY *pSreenReady);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowApproveCAR(CString strCriteria);
	
	
	void SetScreenAttrib();
	void ApproveCAROk();

	virtual void OnScreenExit();

private:
	bool CheckForEmptyRecord(CString strFullRecord);
};

#endif // !defined(AFX_ApproveCAR_H__88DC9D00_B815_4CCC_B9A9_952A24F00EA2__INCLUDED_)
