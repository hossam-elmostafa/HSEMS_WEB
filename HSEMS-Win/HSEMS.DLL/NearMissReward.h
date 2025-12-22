// NearMissReward.h: interface for the NearMissReward class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSREWARD_H__9E3E49CE_B227_4782_A713_7DF8B2088956__INCLUDED_)
#define AFX_NEARMISSREWARD_H__9E3E49CE_B227_4782_A713_7DF8B2088956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class NearMissReward : public CHSEMSCommonCategory  
{
public:
	NearMissReward(CCategory *pCategory);
	virtual ~NearMissReward();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void completeNearMissReward(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	long ShowNearMissReward(CString strCriteria);
};

#endif // !defined(AFX_NEARMISSREWARD_H__9E3E49CE_B227_4782_A713_7DF8B2088956__INCLUDED_)
