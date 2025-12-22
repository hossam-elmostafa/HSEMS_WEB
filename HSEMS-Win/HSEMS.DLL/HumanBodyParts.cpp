// HumanBodyParts.cpp: implementation of the CHumanBodyParts class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "HumanBodyParts.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHumanBodyParts::CHumanBodyParts(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


CHumanBodyParts::~CHumanBodyParts()
{

}

HRESULT CHumanBodyParts::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick) {

	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	
	//a.ali 3-11-2013
	//In (Human Body Parts) popup, The (body part) combo list, should exclude already selected records for same "injured person"
	if(strFieldName == "HMNBODYPART_COD") {
		CString strLink = FormGetField("HSE_HMNBODYPART","HMNBODYPART_LNK");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (HUMNBDYPRT_COD NOT IN (SELECT HMNBODYPART_COD FROM HSE_HMNBODYPART WHERE HMNBODYPART_LNK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}		
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);	
}

long CHumanBodyParts::ShowHumanBodyParts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HMNBODYPART;
	return ParentManage("HSE_TGHMNBODYPART",TRUE,TRUE,TRUE,"HSE_HMNBODYPART",strCriteria);
}