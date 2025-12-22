#include "stdafx.h"
#include "HSEMS.h"
#include "AccidentTypesCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
AccidentTypesCategory::AccidentTypesCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

AccidentTypesCategory::~AccidentTypesCategory()
{

}

HRESULT AccidentTypesCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName=="ACDNTTYPROTCAUS_ROTCAUSCOD")
	{
		CString strLocTyp=FormGetField("HSE_ACDNTTYPROTCAUS","ACDNTTYPROTCAUS_ROTCAUSGRP");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	else if(strFieldName=="ACDNTTYPROTCAUS_ROTCAUSSUBDTL")
	{
		CString strSubDtl=FormGetField("HSE_ACDNTTYPROTCAUS","ACDNTTYPROTCAUS_ROTCAUSCOD");								
		if(strSubDtl.IsEmpty())
			strSubDtl=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strSubDtl);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

long AccidentTypesCategory::ShowAccidentTypes(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NearMissTypes;
	return ParentManage("HSE_TGACDNTTYP",TRUE,TRUE,TRUE,"HSE_ACDNTTYP",strCriteria);
}