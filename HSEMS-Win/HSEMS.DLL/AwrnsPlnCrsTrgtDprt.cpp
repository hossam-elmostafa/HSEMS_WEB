#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnCrsTrgtDprt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString gstrKeyField;
//////////////////////////////////////////////////////////////////////////
//RQ-2-2021(Batch2).38

CAwrnsPlnCrsTrgtDprtCategory::CAwrnsPlnCrsTrgtDprtCategory(CCategory *pCategory,CString strCriteria):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGAWRNSPLN_CRS_TRGTDPRT";
	m_strScreen			= "Target Departments";

}

CAwrnsPlnCrsTrgtDprtCategory::~CAwrnsPlnCrsTrgtDprtCategory() 
{
}

/*HRESULT CAwrnsPlnCrsTrgtDprtCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	FormSetField("HSE_TgAwrnsPln_Crs_TrgtDprt","TrgtDprt_PrntCrs",gstrKeyField);	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}*/

HRESULT CAwrnsPlnCrsTrgtDprtCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);			
	if(strBtnName=="NEW") {			
		FormSetField("HSE_TgAwrnsPln_Crs_TrgtDprt","TrgtDprt_PrntCrs",gstrKeyField);	
	}	
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


////////////////////////////////////////////////////////////////////////////