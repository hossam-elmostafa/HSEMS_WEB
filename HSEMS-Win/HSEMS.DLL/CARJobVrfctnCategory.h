// CARJobVrfctnCategory.h: interface for the CARJobVrfctnCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "CARCategory.h"

#if !defined(AFX_CARJOBVRFCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
#define AFX_CARJOBVRFCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARJobVrfctnCategory  : public CarCategory
{
public:
	CARJobVrfctnCategory(CCategory *pCategory);
	virtual ~CARJobVrfctnCategory();	
	long ShowCrctveAccJobVrfctn(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CARJobAcptd(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	//void CARJobRjctd(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void CARJobCncld(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_CARJOBVRFCTNCATEGORY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
