#include "stdafx.h"
#include "HSEMS.h"
#include "SitSrvyInqCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SitSrvyInqCategory::SitSrvyInqCategory(CCategory *pCategory):SitSrvyCategory(pCategory) {
	m_pCategory = pCategory;
}

SitSrvyInqCategory::~SitSrvyInqCategory() {}

long SitSrvyInqCategory::ShowSitSrvyEntInq(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SITSRVYINQ;
	return ParentManage("HSE_TgSitSrvyInq",FALSE,FALSE,FALSE,"HSE_SitSrvyEnt",strCriteria);
}

HRESULT SitSrvyInqCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return SitSrvyCategory::DisplayScreenReady(pScreenReady);
}
