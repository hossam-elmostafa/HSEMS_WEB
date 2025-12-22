#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnInqCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AwrnsPlnInqCategory::AwrnsPlnInqCategory(CCategory *pCategory):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
}

AwrnsPlnInqCategory::~AwrnsPlnInqCategory() {}

long AwrnsPlnInqCategory::ShowAwrnsPlnInq(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AwrnsPlnInq;
	return ParentManage("HSE_TgAwrnsPlnInq",FALSE,FALSE,FALSE,"HSE_AwrnsPln",strCriteria);
}



