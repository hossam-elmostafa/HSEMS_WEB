// AreaHzrdsCategory.h: interface for the AreaHzrdsCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "RiskAssessmentCategory.h"
#if !defined(AFX_AREAHZRDSCATEGORY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_)
#define AFX_AREAHZRDSCATEGORY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AreaHzrdsCategory  : public RiskAssessmentCategory  
{
public:
	AreaHzrdsCategory(CCategory *pCategory);
	virtual ~AreaHzrdsCategory();	
	long ShowAreaHzrds(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowAHzrdsPtntlHzrdCtrlMsr(CString strCriteria);
	long ShowAreaHzrdsPtntlHzrdEffct(CString strCriteria);
	void OpenAreaHzrdsCntrlMsure(CString strFormTag);
	void OpnAreaHzrdsEffectsScr(CString strFormTag);
};

#endif // !defined(AFX_AREAHZRDSCATEGORY_H__2C52E67B_D441_4562_AEBB_42E51486E8D0__INCLUDED_)
