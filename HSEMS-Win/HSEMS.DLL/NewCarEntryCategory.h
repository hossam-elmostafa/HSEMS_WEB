// NewCarEntryCategory.h: interface for the CNewCarEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWCARENTRYCATEGORY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)
#define AFX_NEWCARENTRYCATEGORY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuditModuleCategory.h"

class CNewCarEntryCategory : public CAuditModuleCategory  
{
public:
	CNewCarEntryCategory(CCategory *pCategory,CMapStringToOb* pmpListOfParameters=NULL);
	virtual ~CNewCarEntryCategory();

	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);

private:
	void InsertIntoTracingTab(CString strActionDescription,CString strSrcScreen,CString strPrmKy);
	void rejectRecord(CString strForm_Tag);
	void acceptRecord(CString strForm_Tag,bool bLocked=false);
	void approveCARRecord(CString strForm_Tag,bool bLocked=false);
	void Reject(CString strSourceModule,CString strKeyField);

	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void ApproveCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked=false);
	void ApproveCARProcess2(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked=false);
	void RejectCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked=false);
	void AcceptCARProcess(CString strFormTag,tag_CustomButtonClicked* pCustomButtonClicked,bool bLocked=false);
	void InsertIntoTracingTabProcess(tag_CustomButtonClicked* pCustomButtonClicked,CString strStatus,CString strSourceScreenName="",CString strFormTag="");
	void UpdateYearAndSerial(CString strFormTag,CString strYear);

//Member variables
	CString m_strOwnerDepartment;
	CString m_strUserDepartment;
};

#endif // !defined(AFX_NEWCARENTRYCATEGORY_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)
