// NearMissCategory.h: interface for the NearMissCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEARMISSCATEGORY_H__78D178AD_5D2D_4A87_BB42_8EAAE825433D__INCLUDED_)
#define AFX_NEARMISSCATEGORY_H__78D178AD_5D2D_4A87_BB42_8EAAE825433D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class NearMissCategory : public CHSEMSCommonCategory  
{
public:
	NearMissCategory(CCategory *pCategory);
	virtual ~NearMissCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	void rejectRecord(CString strFormTag);
	void SetRecordStatus(CString strTableName,CString strMainStatus);
	BOOL DoSaveDeleteValid(CString strTableName);
	void EnableRequireedActionFields(CString strSubFormTag);
	void lockExpctdHzrdFlds(CString strSubFormTag);
	void unLockExpctdHzrdFlds(CString strSubFormTag);
	void getNearMissRootCauses();
	void getNearMissHazards();
	CString getTargetMonthSTD(CString strDep,CString strOccuranceDT);

};

#endif // !defined(AFX_NEARMISSCATEGORY_H__78D178AD_5D2D_4A87_BB42_8EAAE825433D__INCLUDED_)
