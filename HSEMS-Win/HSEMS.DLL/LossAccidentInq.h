// LossAccidentInq.h: interface for the LossAccidentInq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOSSACCIDENTINQ_H__31449CF8_9F8F_40E3_B972_A3144DF96CD1__INCLUDED_)
#define AFX_LOSSACCIDENTINQ_H__31449CF8_9F8F_40E3_B972_A3144DF96CD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LossAccidentCategory.h"
class LossAccidentInq  : public LossAccidentCategory
{
public:
	LossAccidentInq(CCategory *pCategory);
	virtual ~LossAccidentInq();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);	
	long ShowLossAccidentInquiry(CString strCriteria);

};

#endif // !defined(AFX_LOSSACCIDENTINQ_H__31449CF8_9F8F_40E3_B972_A3144DF96CD1__INCLUDED_)
