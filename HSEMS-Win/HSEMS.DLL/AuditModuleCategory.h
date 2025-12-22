// AuditModuleCategory.h: interface for the CAuditModuleCategory class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_AuditModuleCategory_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)
#define AFX_AuditModuleCategory_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CAuditModuleCategory : public CHSEMSCommonCategory  
{
public:
	CAuditModuleCategory(CCategory *pCategory,CMapStringToOb* pmpListOfParameters=NULL);
	virtual ~CAuditModuleCategory();	
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);

protected:
	void CalculateNewSerial(CString strFormTag,CString strSubFormTag);
	void CheckEmail(CString strFormTag,CString strField,long *iCancle,bool bCheckOnly=false);
	bool AuditConfirmation_AddRecordToCAREntry(int nParentPrmKy);
	virtual void ConfirmToolBarButtonClickedDone(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubForm_Tag);


	CString m_strSerialFieldName;
	CString m_strFormTable;
	CString m_strMasterKeyField;
	CString m_strMainSerialDependentKey;
	CMapStringToOb *m_pmpDetailsSerials;
	CMapStringToOb *m_mpDetailsKeyFields;
	CMapStringToOb *m_mpDetailsTableName;
	void	*m_pMsg;
	bool	m_bFirstTime;
	
};


#endif // !defined(AFX_AuditModuleCategory_H__95031D12_CE37_493B_BD56_098888A5314F__INCLUDED_)