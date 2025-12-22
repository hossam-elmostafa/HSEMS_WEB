// NecessaryControlMeasures.h: interface for the NecessaryControlMeasures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NECESSARYCONTROLMEASURES_H__9745E2B9_233D_4919_82FF_B01C0AC168AA__INCLUDED_)
#define AFX_NECESSARYCONTROLMEASURES_H__9745E2B9_233D_4919_82FF_B01C0AC168AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class NecessaryControlMeasures : public CHSEMSCommonCategory  
{
public:
	NecessaryControlMeasures(CCategory *pCategory);
	virtual ~NecessaryControlMeasures();
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	//Changed according to CR03-12 by Ahmed Ali
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	//HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	//HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	long ShowNecessaryControlMeasures(CString strCriteria);
};

#endif // !defined(AFX_NECESSARYCONTROLMEASURES_H__9745E2B9_233D_4919_82FF_B01C0AC168AA__INCLUDED_)
