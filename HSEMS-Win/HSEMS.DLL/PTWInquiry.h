// PTWInquiry.h: interface for the PTWInquiry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTWINQUIRY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_)
#define AFX_PTWINQUIRY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtwCategory.h"

class PTWInquiry  : public PtwCategory
{
public:
	PTWInquiry(CCategory *pCategory);
	virtual ~PTWInquiry();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPTW(CString strCriteria);
};

#endif // !defined(AFX_PTWINQUIRY_H__ADAB3FFA_59CA_4E5D_920F_6C2552182B02__INCLUDED_)
