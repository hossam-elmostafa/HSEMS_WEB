// InjueryType.cpp: implementation of the CInjueryType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "InjueryType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInjueryType::CInjueryType(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

CInjueryType::~CInjueryType()
{

}


HRESULT CInjueryType::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick) {

	//Replaced by Unique group in FirsSysDB
	/*CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	if(strFieldName == "INJRYTP_INJRYTYP") {
		CString strLink = FormGetField("HSE_INJRYTP","INJRYTP_LNK");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (INJRYTYP_COD NOT IN (SELECT INJRYTP_INJRYTYP FROM HSE_INJRYTP WHERE INJRYTP_LNK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}*/
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);	
}

long CInjueryType::ShowInjuryTypepopUpscreen(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_INJUERYTYPE_POP_UP;
	return ParentManage("HSE_TGINJRYTP",TRUE,TRUE,TRUE,"HSE_INJRYTP",strCriteria);
}