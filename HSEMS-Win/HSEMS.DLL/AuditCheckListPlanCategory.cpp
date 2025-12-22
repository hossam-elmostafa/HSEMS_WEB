// AuditCheckListPlanCategory.cpp: implementation of the AuditCheckListPlanCategory class.
// Handle checklist popup screen opened from Audit Plan Follow-up
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "AuditCheckListPlanCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditCheckListPlanCategory::AuditCheckListPlanCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


AuditCheckListPlanCategory::~AuditCheckListPlanCategory()
{

}

long AuditCheckListPlanCategory::ShowAuditCheckListPlan(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AUDITCHECKLISTRSLT;
	//return ParentManage("HSE_TGADTCHCKLSTRSLT",TRUE,TRUE,TRUE,"HSE_ADTCHCKLSTRSLT",strCriteria);
	return ParentManage("HSE_TGADTCHCKLSTRSLT",TRUE,TRUE,TRUE,"HSE_ADTCHCKLSTPLN",strCriteria);
}