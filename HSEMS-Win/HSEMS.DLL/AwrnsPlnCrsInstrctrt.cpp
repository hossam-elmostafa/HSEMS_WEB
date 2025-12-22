#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnCrsInstrctr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString gstrKeyField;
extern CString gstrCourseName; 

//////////////////////////////////////////////////////////////////////////
//RQ-2-2021(Batch2).39

CAwrnsPlnCrsInstrctrCategory::CAwrnsPlnCrsInstrctrCategory(CCategory *pCategory,CString strCriteria):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGAWRNSPLN_CRS_INSTRCTR";
	m_strScreen			= "Instructors";

}

CAwrnsPlnCrsInstrctrCategory::~CAwrnsPlnCrsInstrctrCategory() 
{
}

/*HRESULT CAwrnsPlnCrsInstrctrCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	FormSetField(m_FSATableName,"Instrctr_PrntCrs",gstrKeyField);	
	FormSetField(m_FSATableName,"Instrctr_CourseId",gstrCourseName);	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}*/

HRESULT CAwrnsPlnCrsInstrctrCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	if(strBtnName=="NEW") {			
		FormSetField(m_FSATableName,"Instrctr_PrntCrs",gstrKeyField);	
		FormSetField(m_FSATableName,"Instrctr_CourseId",gstrCourseName);	
	}	
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

////////////////////////////////////////////////////////////////////////////