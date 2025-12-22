#include "stdafx.h"
#include "HSEMS.h"
#include "NearMissTypesCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NearMissTypesCategory::NearMissTypesCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

NearMissTypesCategory::~NearMissTypesCategory()
{

}

HRESULT NearMissTypesCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName=="NRMISTYPESEXPCTDHZRDS_HZRDCAT")
	{
		CString strHzrdGrp=FormGetField("HSE_NRMISTYPESEXPCTDHZRDS","NRMISTYPESEXPCTDHZRDS_HZRDGRP");
		if(strHzrdGrp.IsEmpty())
			strHzrdGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK='%s')",strHzrdGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}
	else if(strFieldName=="NRMISTYPESEXPCTDHZRDS_HRMTYP")
	{
		CString strTskGrp=FormGetField("HSE_NRMISTYPESEXPCTDHZRDS","NRMISTYPESEXPCTDHZRDS_HZRDCAT");
		if(strTskGrp.IsEmpty())
			strTskGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strTskGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	else if(strFieldName=="NRMISTYPESROTCAUS_ROTCAUSCOD")
	{
		CString strLocTyp=FormGetField("HSE_NRMISTYPESROTCAUS","NRMISTYPESROTCAUS_ROTCAUSGRP");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	else if(strFieldName=="NRMISTYPESROTCAUS_ROTCAUSSUBDTL")
	{
		CString strSubDtl=FormGetField("HSE_NRMISTYPESROTCAUS","NRMISTYPESROTCAUS_ROTCAUSCOD");								
		if(strSubDtl.IsEmpty())
			strSubDtl=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strSubDtl);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

long NearMissTypesCategory::ShowNearMissTypes(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NearMissTypes;
	return ParentManage("HSE_TGNrMisTypes",TRUE,TRUE,TRUE,"HSE_NrMisTypes",strCriteria);
}