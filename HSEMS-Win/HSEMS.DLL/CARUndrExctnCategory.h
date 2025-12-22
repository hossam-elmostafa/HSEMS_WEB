// CARUndrExctnCategory.h: interface for the CARUndrExctnCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "CARCategory.h"

#if !defined(AFX_CARUNDREXCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
#define AFX_CARUNDREXCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARUndrExctnCategory  : public CarCategory
{
public:
	CARUndrExctnCategory(CCategory *pCategory);
	virtual ~CARUndrExctnCategory();	
	long ShowCrctveAccUndrExctn(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CARJobCmpltd(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	//void CARJobRjctd(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CARJobPndng(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_CARUNDREXCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
