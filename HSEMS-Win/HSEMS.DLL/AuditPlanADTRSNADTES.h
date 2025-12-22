// AuditPlanADTRSNADTES.h: interface for the AuditPlanADTRSNADTES class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITPLANADTRSNADTES_H__A07D9C06_6473_4FF0_911A_717F7A5AE653__INCLUDED_)
#define AFX_AUDITPLANADTRSNADTES_H__A07D9C06_6473_4FF0_911A_717F7A5AE653__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AuditCategory.h"
#include "IEmailSupport.h"
#include "HSEobj.h"

class AuditPlanADTRSNADTES  : public AuditCategory
{
public:
	AuditPlanADTRSNADTES(CCategory *pCategory);
	virtual ~AuditPlanADTRSNADTES();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowAuditPlanAuditorsandAuditees(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
protected:
	//void OpenAuditorsScreen();
	//void OpenAuditeesScreen();
	void CompleteAuditWthAdtrsNAdtees(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void openAuditCheckList();
	bool CompleteActivity(CString strAuditKeyFieldValue,CString strActivtyKeyValue);
	

};

#endif // !defined(AFX_AUDITPLANADTRSNADTES_H__A07D9C06_6473_4FF0_911A_717F7A5AE653__INCLUDED_)
