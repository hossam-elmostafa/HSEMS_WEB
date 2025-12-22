// AccidentFollowUpCategory.h: interface for the AccidentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTFOLLOWUPCATEGORY_H__FAE4EEB6_05E4_4497_9A0D_FBC297CD411F__INCLUDED_)
#define AFX_ACCIDENTFOLLOWUPCATEGORY_H__FAE4EEB6_05E4_4497_9A0D_FBC297CD411F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AccidentCategory.h"

class AccidentFollowUpCategory : public AccidentCategory  
{
public:
	AccidentFollowUpCategory(CCategory *pCategory);
	virtual ~AccidentFollowUpCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void CloseAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAccidentFollowup(CString strCriteria);
	void updtAccidentTypeRotCusLib(CString strAccidentTypeKey);
};

#endif // !defined(AFX_ACCIDENTFOLLOWUPCATEGORY_H__FAE4EEB6_05E4_4497_9A0D_FBC297CD411F__INCLUDED_)
