// HumanBodyParts.h: interface for the CHumanBodyParts class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUMANBODYPARTS_H__2D114B5F_A929_47D9_AF52_422383C780B3__INCLUDED_)
#define AFX_HUMANBODYPARTS_H__2D114B5F_A929_47D9_AF52_422383C780B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CHumanBodyParts : public CHSEMSCommonCategory  
{
public:
	CHumanBodyParts(CCategory *pCategory);
	virtual ~CHumanBodyParts();
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowHumanBodyParts(CString strCriteria);
};

#endif // !defined(AFX_HUMANBODYPARTS_H__2D114B5F_A929_47D9_AF52_422383C780B3__INCLUDED_)
