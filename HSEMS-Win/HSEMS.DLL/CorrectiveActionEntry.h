// NewCarEntryCategory.h: interface for the CCorrectiveActionEntry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CORRECTIVEACTIONENTRY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)
#define AFX_CORRECTIVEACTIONENTRY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditModuleCategory.h"

class CCorrectiveActionEntry : public CAuditModuleCategory  
{
public:
	CCorrectiveActionEntry(CCategory *pCategory,CMapStringToOb* pmpListOfParameters=NULL,CString strWhereCriteria="");
	virtual ~CCorrectiveActionEntry();

	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);

private:
	void InsertIntoTracingTab(CString strActionDescription,CString strSrcScreen,CString strPrmKy);
	void acceptRecord(CString strForm_Tag,bool bLocked=false);
	void Reject(CString strSourceModule,CString strKeyField);

	void AddNewSerial(CString strFormTag);

	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);

//Member variables
	CString m_strParentKey;
};

#endif // !defined(AFX_CORRECTIVEACTIONENTRY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)
