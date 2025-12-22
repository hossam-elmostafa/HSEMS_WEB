// AuditCategory.h: interface for the AuditCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITCATEGORY_H__9941D416_5C59_48DF_8E01_1CCB1C080D58__INCLUDED_)
#define AFX_AUDITCATEGORY_H__9941D416_5C59_48DF_8E01_1CCB1C080D58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class AuditCategory : public CHSEMSCommonCategory  
{
public:
	AuditCategory(CCategory *pCategory);
	virtual ~AuditCategory();
	void LockOnStatus();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	
	void rejectRecord(CString strForm_Tag);	
	void OpenAuditCheckListScreen(CString ScreenTag);
	BOOL openAuditChecklistResult(CString ScreenTag);
	void OpenAuditorsScreen(CString ScreenTag,int ScreenMode);
	void OpenAuditeesScreen(CString ScreenTag,int ScreenMode);
	void OpenCncrndDept(CString ScreenTag);
	void LockActivityTab();
	void UnLockActivityTab();
	CString getRecordStatus();
	void OpenRequiredActionScreen(CString ScreenTag,int ScreenMode);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
};

#endif // !defined(AFX_AUDITCATEGORY_H__9941D416_5C59_48DF_8E01_1CCB1C080D58__INCLUDED_)
