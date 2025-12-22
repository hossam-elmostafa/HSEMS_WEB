// NearMissFollowUpCategory.h: interface for the NearMissFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSFOLLOWUPCATEGORY_H__CBE189C3_186B_46BB_9835_E9FF62992E83__INCLUDED_)
#define AFX_NEARMISSFOLLOWUPCATEGORY_H__CBE189C3_186B_46BB_9835_E9FF62992E83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NearMissCategory.h"

class NearMissFollowUpCategory : public NearMissCategory  
{
public:
	NearMissFollowUpCategory(CCategory *pCategory);
	virtual ~NearMissFollowUpCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	void CloseNearMiss(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowNearMissFollowUp(CString strCriteria);
	void updtNrMisTypRotCusLib(CString strNearMissKey);
	void updtNrMisTypExpctdHzrdsLib(CString strNearMissKey);
protected:
	CString GetNextCarNumForYear(CString strCARYear);
};

#endif // !defined(AFX_NEARMISSFOLLOWUPCATEGORY_H__CBE189C3_186B_46BB_9835_E9FF62992E83__INCLUDED_)
