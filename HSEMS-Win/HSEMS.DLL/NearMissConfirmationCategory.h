// NearMissConfirmationCategory.h: interface for the NearMissConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSCONFIRMATIONCATEGORY_H__A19444DC_6D7A_4D16_BEF5_7AD958790C55__INCLUDED_)
#define AFX_NEARMISSCONFIRMATIONCATEGORY_H__A19444DC_6D7A_4D16_BEF5_7AD958790C55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NearMissCategory.h"

class NearMissConfirmationCategory : public NearMissCategory  
{
public:
	NearMissConfirmationCategory(CCategory *pCategory);
	virtual ~NearMissConfirmationCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);

	void AcceptNearMiss(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowNearMissConfirmation(CString strCriteria);

};

#endif // !defined(AFX_NEARMISSCONFIRMATIONCATEGORY_H__A19444DC_6D7A_4D16_BEF5_7AD958790C55__INCLUDED_)
