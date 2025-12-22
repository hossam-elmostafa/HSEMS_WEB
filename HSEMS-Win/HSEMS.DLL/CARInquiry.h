// ARInquiry.h: interface for the CARInquiry class.
//
//////////////////////////////////////////////////////////////////////

#include "CARCategory.h"

#if !defined(AFX_ARINQUIRY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
#define AFX_ARINQUIRY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CARInquiry  : public CarCategory
{
public:
	CARInquiry(CCategory *pCategory);
	virtual ~CARInquiry();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowCAR(CString strCriteria);
};

#endif // !defined(AFX_ARINQUIRY_H__204B9D98_DFB5_4543_9F9B_CA183B9B964D__INCLUDED_)
