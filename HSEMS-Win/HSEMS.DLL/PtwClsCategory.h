// PtwClsCategory.h: interface for the PtwClsCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTWCLSCATEGORY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_)
#define AFX_PTWCLSCATEGORY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtwCategory.h"

class PtwClsCategory  : public PtwCategory
{
public:
	PtwClsCategory(CCategory *pCategory);
	virtual ~PtwClsCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPTWsRgstrCls(CString strCriteria);
	void closePTW(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_PTWCLSCATEGORY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_)
