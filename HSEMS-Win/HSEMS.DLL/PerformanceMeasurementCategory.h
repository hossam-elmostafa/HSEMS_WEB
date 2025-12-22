// PerformanceMeasurementCategory.h: interface for the PerformanceMeasurementCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFORMANCEMEASUREMENTCATEGORY_H__FC1A45CE_6D4D_4675_86DE_DD42CB98E5E3__INCLUDED_)
#define AFX_PERFORMANCEMEASUREMENTCATEGORY_H__FC1A45CE_6D4D_4675_86DE_DD42CB98E5E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class PerformanceMeasurementCategory : public CHSEMSCommonCategory  
{
public:
	PerformanceMeasurementCategory(CCategory *pCategory);
	virtual ~PerformanceMeasurementCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void rejectRecord(CString strForm_Tag);
	void ShowDetails(tag_CustomButtonClicked* pCustomButtonClicked);

};

#endif // !defined(AFX_PERFORMANCEMEASUREMENTCATEGORY_H__FC1A45CE_6D4D_4675_86DE_DD42CB98E5E3__INCLUDED_)
