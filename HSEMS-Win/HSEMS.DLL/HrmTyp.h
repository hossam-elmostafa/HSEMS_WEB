// HrmTyp.h: interface for the HrmTyp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRMTYP_H__AFB05AF9_D275_45BB_8C62_C556771C261A__INCLUDED_)
#define AFX_HRMTYP_H__AFB05AF9_D275_45BB_8C62_C556771C261A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HSEMSCommonCategory.h"

class HrmTyp  : public CHSEMSCommonCategory
{
public:
	HrmTyp(CCategory *pCategory);
	virtual ~HrmTyp();

	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	long ShowHarmTypes(CString strCriteria);

};

#endif // !defined(AFX_HRMTYP_H__AFB05AF9_D275_45BB_8C62_C556771C261A__INCLUDED_)
