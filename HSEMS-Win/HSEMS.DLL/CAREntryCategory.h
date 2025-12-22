// arEntryCategory.h: interface for the CarEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENTRYCATEGORY_H__97CA853F_EEB8_44BF_BC9F_586425BFC5A1__INCLUDED_)
#define AFX_ARENTRYCATEGORY_H__97CA853F_EEB8_44BF_BC9F_586425BFC5A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CARCategory.h"

class CarEntryCategory : public CarCategory  
{
public:
	CarEntryCategory(CCategory *pCategory);
	virtual ~CarEntryCategory();
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	//CString GetAuditCheckListsFilteredOnAuditActivity();
	CString GetAuditActivityFilteredOnAdtNoAndAdtYr();
	CString GetAuditNumbersFilteredOnAuditYear();
	void FillCarDescAndLoc(CString strCarBasDesc,CString strCarBasDept,CString strCarBasLoc,CString strCarBasSubLoc,CString strCarBasSit,CString strCarBasArea,CString strTableName,CString strKeyFldName,CString strKeyFldVal);
	void CompleteCorrectiveActionEntry(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	//void setAlwaysLockedFieldsAttr(CString strTagName,CString strAttrib);
	long ShowCorrectiveActionEntry(CString strCriteria);
	void emptyRelatedTXNFlds();
	void CarEntryCategory::CancelCorrectiveActionEntry(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_ARENTRYCATEGORY_H__97CA853F_EEB8_44BF_BC9F_586425BFC5A1__INCLUDED_)
