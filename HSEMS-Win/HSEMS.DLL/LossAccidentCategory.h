// LossAccidentCategory.h: interface for the LossAccidentCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOSSACCIDENTCATEGORY_H__145F2EFD_9477_4418_A176_BCD3EFDFE0F0__INCLUDED_)
#define AFX_LOSSACCIDENTCATEGORY_H__145F2EFD_9477_4418_A176_BCD3EFDFE0F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class LossAccidentCategory : public CHSEMSCommonCategory  
{
public:
	LossAccidentCategory(CCategory *pCategory);
	virtual ~LossAccidentCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);


	void rejectRecord(CString strForm_Tag);


};

#endif // !defined(AFX_LOSSACCIDENTCATEGORY_H__145F2EFD_9477_4418_A176_BCD3EFDFE0F0__INCLUDED_)
