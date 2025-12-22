// PtwCategory.h: interface for the PtwCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTWCATEGORY_H__771E94EF_FD66_4EFB_8E61_55E03FFA8BE1__INCLUDED_)
#define AFX_PTWCATEGORY_H__771E94EF_FD66_4EFB_8E61_55E03FFA8BE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class PtwCategory : public CHSEMSCommonCategory  
{
public:
	PtwCategory(CCategory *pCategory);
	virtual ~PtwCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);	
	HRESULT DisplayFieldChange(FIELDCHANGE *pFieldChange);
	void rejectRecord(CString strForm_Tag);
	bool isRenewAccepted(CString strCompanyNam,int ptwNO);
	bool isGMSIntegrationEnabled();
};

#endif // !defined(AFX_PTWCATEGORY_H__771E94EF_FD66_4EFB_8E61_55E03FFA8BE1__INCLUDED_)
