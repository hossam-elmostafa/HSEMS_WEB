#include "stdafx.h"
#include "HSEMS.h"
#include "JobPtntlHzrdsLib.h"

JobPtntlHzrdsLib::JobPtntlHzrdsLib(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

JobPtntlHzrdsLib::~JobPtntlHzrdsLib()
{

}

long JobPtntlHzrdsLib::ShowJobPtntlHzrdsLib(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_JobPtntlHzrdsLib;
	return ParentManage("HSE_TgPtntlHzrdsLib",TRUE,TRUE,TRUE,"HSE_PtntlHzrdsLib",strCriteria);
}


HRESULT JobPtntlHzrdsLib::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;		
	if(strFieldName=="PTNTLHZRDSLIB_HZRDCAT")
	{
		CString strHzrdGrp=FormGetField("HSE_PTNTLHZRDSLIB","PTNTLHZRDSLIB_HZRDGRP");
		if(strHzrdGrp.IsEmpty())
			strHzrdGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK='%s')",strHzrdGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	}
	else if(strFieldName=="PTNTLHZRDSLIB_HRMTYP")
	{
		CString strTskGrp=FormGetField("HSE_PTNTLHZRDSLIB","PTNTLHZRDSLIB_HZRDCTG");
		if(strTskGrp.IsEmpty())
			strTskGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strTskGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}