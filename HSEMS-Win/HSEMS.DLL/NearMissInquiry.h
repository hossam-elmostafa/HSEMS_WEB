// NearMissInquiry.h: interface for the NearMissInquiry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSINQUIRY_H__BE718090_BCAF_4B4E_B36D_94AE3550AA7A__INCLUDED_)
#define AFX_NEARMISSINQUIRY_H__BE718090_BCAF_4B4E_B36D_94AE3550AA7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NearMissCategory.h"
class NearMissInquiry : public NearMissCategory   
{
public:
	NearMissInquiry(CCategory *pCategory);
	virtual ~NearMissInquiry();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowNearMissInquiry(CString strCriteria);
};

#endif // !defined(AFX_NEARMISSINQUIRY_H__BE718090_BCAF_4B4E_B36D_94AE3550AA7A__INCLUDED_)
