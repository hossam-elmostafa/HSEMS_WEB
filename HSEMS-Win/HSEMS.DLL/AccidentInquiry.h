// AccidentInquiry.h: interface for the AccidentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTINQUIRY_H__9A043B1B_213B_4C0D_9BBC_91D84934CB3F__INCLUDED_)
#define AFX_ACCIDENTINQUIRY_H__9A043B1B_213B_4C0D_9BBC_91D84934CB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccidentCategory.h"
class AccidentInquiry  : public AccidentCategory  
{
public:
	AccidentInquiry(CCategory *pCategory):AccidentCategory(pCategory)
	{
		m_pCategory = pCategory;
	};
	//virtual ~AccidentInquiry();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAccident(CString strCriteria);

};

#endif // !defined(AFX_ACCIDENTINQUIRY_H__9A043B1B_213B_4C0D_9BBC_91D84934CB3F__INCLUDED_)
