// ToolsListener.h: interface for the CToolsListener class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_TOOLSLISTENER_H_)
#define _TOOLSLISTENER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolsListener :public CCmdTarget
{
public:
	CToolsListener();
	virtual ~CToolsListener();

	void SetMyCCategory(CToolsCategory *pToolsCategory){m_pToolsCategory=pToolsCategory;};
	_Tools*		m_pToolsObj;

	BEGIN_INTERFACE_PART_NEW(EventSink)
	afx_msg void OnAddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID);
	afx_msg void OnDeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID);
	afx_msg void OnChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID);
	afx_msg void OnUnloadPlan(BOOL bActionCompleted);
	afx_msg void OnPlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID);
	afx_msg void OnNodeSelected(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData);
	afx_msg void OnRangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2);
	afx_msg void OnNodeMenuPulled( LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID ,LPCTSTR strData );
	afx_msg void OnNodeMenuItemClicked(LPCTSTR MenuID,LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData );
	afx_msg void OnColDblClicked(LPCTSTR ColValue);
	afx_msg void OnNodeExpanded(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData);
//	afx_msg  void OnNodesCheckList (_variant_t & vtNodes);
	afx_msg  void OnTreeTerminated ();
	afx_msg  void OnNodeDeselect ();

	DECLARE_EVENTSINK_MAP()
	

protected:	
	CToolsCategory	*m_pToolsCategory;

};

#endif // !defined(_TOOLSLISTENER_H_)
