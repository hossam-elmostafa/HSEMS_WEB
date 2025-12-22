// InjueryType.h: interface for the CInjueryType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INJUERYTYPE_H__EF9D153B_FBA6_46E5_9614_764DA5D4B6CB__INCLUDED_)
#define AFX_INJUERYTYPE_H__EF9D153B_FBA6_46E5_9614_764DA5D4B6CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CInjueryType : public CHSEMSCommonCategory  
{
public:
	CInjueryType(CCategory *pCategory);
	virtual ~CInjueryType();
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowInjuryTypepopUpscreen(CString strCriteria);
};

#endif // !defined(AFX_INJUERYTYPE_H__EF9D153B_FBA6_46E5_9614_764DA5D4B6CB__INCLUDED_)
