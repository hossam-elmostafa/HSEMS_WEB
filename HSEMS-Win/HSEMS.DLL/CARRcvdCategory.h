// CARRcvdCategory.h: interface for the CARRcvdCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "CARCategory.h"

#if !defined(AFX_CARRCVDCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
#define AFX_CARRCVDCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARRcvdCategory  : public CarCategory
{
public:
	CARRcvdCategory(CCategory *pCategory);
	virtual ~CARRcvdCategory();	
	long ShowCrctveAccRcvd(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void AcceptCARExe(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void RejectCARExe(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_CARRCVDCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
