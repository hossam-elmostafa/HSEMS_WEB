// SitSrvyRqrdActn.h: interface for the SitSrvyRqrdActn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITSRVYRQRDACTN_H__0F3E496E_2EAA_410D_93FE_99866591A77C__INCLUDED_)
#define AFX_SITSRVYRQRDACTN_H__0F3E496E_2EAA_410D_93FE_99866591A77C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SitSrvyCategory.h"

class SitSrvyRqrdActn : public SitSrvyCategory  
{
public:
	SitSrvyRqrdActn(CCategory *pCategory);
	virtual ~SitSrvyRqrdActn();
	long ShowSitSrvyRqrdActn(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
};

#endif // !defined(AFX_SITSRVYRQRDACTN_H__0F3E496E_2EAA_410D_93FE_99866591A77C__INCLUDED_)
