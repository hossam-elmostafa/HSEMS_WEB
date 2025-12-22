// arCategory.h: interface for the CarCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCATEGORY_H__9153DBD1_881B_438A_BBDD_A26C4431A7FB__INCLUDED_)
#define AFX_ARCATEGORY_H__9153DBD1_881B_438A_BBDD_A26C4431A7FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CarCategory : public CHSEMSCommonCategory  
{
public:
	CarCategory(CCategory *pCategory);
	virtual ~CarCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT CarCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);

	void rejectRecord(CString strFormTag);
	//void EnableConfirmationComments(CString strFormTag);
	//void HideAuditDescFileds(CString strFormTag);
	//void EnableAuditDescFields(CString strFormTag);
	void OpenTXNInquiryScreen();	
	bool setCARNotAcptd(CString KeyFieldValue,CString strFormTag);
	void setCARAcptd(CString KeyFieldValue,CString strFormTag);

	void RejectCorrectiveAction(tag_CustomButtonClicked* pCustomButtonClicked);
	void CancelCorrectiveAction(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif // !defined(AFX_ARCATEGORY_H__9153DBD1_881B_438A_BBDD_A26C4431A7FB__INCLUDED_)
