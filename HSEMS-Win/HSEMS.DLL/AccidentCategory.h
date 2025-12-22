// AccidentCategory.h: interface for the AccidentCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTCATEGORY_H__D7D29625_F4EA_40D0_BB8D_559C04BD1F33__INCLUDED_)
#define AFX_ACCIDENTCATEGORY_H__D7D29625_F4EA_40D0_BB8D_559C04BD1F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class AccidentCategory : public CHSEMSCommonCategory  
{
public:
	AccidentCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
	{
		m_pCategory = pCategory;
	};
	virtual ~AccidentCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void emptyTXNRefFlds();
	void rejectRecord(CString strForm_Tag);
	//void OpenBodyParts(CString strFormTag = "");		
	//void openInjuryType(CString strFormTag);	
	void openPrsnlPrtctvEqpmntScr(CString strFormTag);
	void getAccidentRootCauses();
	int getRelatedMtrlRlsCount(CString strAcdntNum);
	//void openInjueryRsltScr(CString strFormTag);
};

#endif // !defined(AFX_ACCIDENTCATEGORY_H__D7D29625_F4EA_40D0_BB8D_559C04BD1F33__INCLUDED_)
