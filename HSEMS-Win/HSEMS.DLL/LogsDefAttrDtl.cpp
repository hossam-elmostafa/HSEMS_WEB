#include "stdafx.h"
#include "LogsDefAttrDtl.h"


LogsDefAttrDtl::LogsDefAttrDtl(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

LogsDefAttrDtl::~LogsDefAttrDtl()
{

}

long LogsDefAttrDtl::ShowLogsDefAttrDtl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOGS_DEF_ATTR_DTL;
	return ParentManage("HSE_TgLogsDfntnAtrbtsDtl",TRUE,TRUE,TRUE,"HSE_LogsDfntnAtrbtsDtl",strCriteria);
}
