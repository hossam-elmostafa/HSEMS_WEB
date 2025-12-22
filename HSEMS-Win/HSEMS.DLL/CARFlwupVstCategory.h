// arFlwupVstCategory.h: interface for the CarFlwupVstCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARFLWUPVSTCATEGORY_H__ED804E3E_AF91_4354_BD37_334294783075__INCLUDED_)
#define AFX_ARFLWUPVSTCATEGORY_H__ED804E3E_AF91_4354_BD37_334294783075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CARCategory.h"

class CarFlwupVstCategory  : public CarCategory
{
public:
	CarFlwupVstCategory(CCategory *pCategory);
	virtual ~CarFlwupVstCategory();
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	bool CloseCarWithFLwUp(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);	
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	long ShowCorrectiveActionFollowupvisit(CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
};

#endif // !defined(AFX_ARFLWUPVSTCATEGORY_H__ED804E3E_AF91_4354_BD37_334294783075__INCLUDED_)
