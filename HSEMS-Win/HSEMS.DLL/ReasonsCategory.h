// ReasonsCategory.h: interface for the CReasonsCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REASONSCATEGORY_H__B693E926_B3F2_48A4_89B1_8BF7EF6A9610__INCLUDED_)
#define AFX_REASONSCATEGORY_H__B693E926_B3F2_48A4_89B1_8BF7EF6A9610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"
#include "IReasonCaller.h" 

class CReasonsCategory : public CHSEMSCommonCategory  
{
public:
	CReasonsCategory(CCategory *pCategory,IReasonCaller *pCaller);
	virtual ~CReasonsCategory();
	
	
	UINT ShowScreen(CString strScreenTag,CString strCriteria);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void ScreenReady(LPCTSTR FormTag);
	virtual void ButtonClicked(LPCTSTR Caption, LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Master_Full_Recoed,LPCTSTR Details_Full_Recoe,LPCTSTR Field_Caption);
	void BeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record);
	virtual void OnScreenExit();


	void SetParameters(CString strOwnerScreenTag,CString CallerIdInitial,CString CallerIdFinal);

protected:
	CString m_strOwnerScreenTag,
			m_CallerIdInitial,
			m_CallerIdFinal,
			m_strPrmKey,
			m_strReasonsFilter,
			m_strCaption,
			m_strScreenKey;

	IReasonCaller* m_pCaller;
	
	bool	m_bEnableResults,
			m_bList;
	

};

#endif // !defined(AFX_REASONSCATEGORY_H__B693E926_B3F2_48A4_89B1_8BF7EF6A9610__INCLUDED_)
