// PrformnceMsurmntChcklistCategory.h: interface for the PrformnceMsurmntChcklistCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRFORMNCEMSURMNTCHCKLISTCATEGORY_H__9A054604_4A5B_429D_A1D3_FA1281A85FD6__INCLUDED_)
#define AFX_PRFORMNCEMSURMNTCHCKLISTCATEGORY_H__9A054604_4A5B_429D_A1D3_FA1281A85FD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class PrformnceMsurmntChcklistCategory : public CHSEMSCommonCategory  
{
public:
	PrformnceMsurmntChcklistCategory(CCategory *pCategory);
	virtual ~PrformnceMsurmntChcklistCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowPerformanceMeasurementCheckList(CString strCriteria);

};

#endif // !defined(AFX_PRFORMNCEMSURMNTCHCKLISTCATEGORY_H__9A054604_4A5B_429D_A1D3_FA1281A85FD6__INCLUDED_)
