// PotentialHazardInquiry.h: interface for the PotentialHazardInquiry class.
//
//////////////////////////////////////////////////////////////////////
#include "PotentialHazardCategory.h"
#if !defined(AFX_POTENTIALHAZARDINQUIRY_H__C6CF9125_14E3_4329_899D_FDEFE1C94354__INCLUDED_)
#define AFX_POTENTIALHAZARDINQUIRY_H__C6CF9125_14E3_4329_899D_FDEFE1C94354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PotentialHazardInquiry  : public PotentialHazardCategory
{
public:
	PotentialHazardInquiry(CCategory *pCategory);
	virtual ~PotentialHazardInquiry();
	HRESULT PotentialHazardInquiry::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowPotentialhazardinquiry(CString strCriteria);

};

#endif // !defined(AFX_POTENTIALHAZARDINQUIRY_H__C6CF9125_14E3_4329_899D_FDEFE1C94354__INCLUDED_)
