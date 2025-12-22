#ifndef _TOOLSCATEGORY_H
#define _TOOLSCATEGORY_H

class CToolsCategory
{
public:
	virtual void OnAddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)=0;
	virtual void OnDeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)=0;
	virtual void OnChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID)=0;
	virtual void OnUnloadPlan(BOOL bActionCompleted)=0;
	virtual void OnPlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID)=0;
	virtual void OnNodeSelected(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData)=0;
	virtual void OnNodeExpanded(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData)=0;
	virtual void OnNodeMenuPulled( LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID,LPCTSTR strData )=0;
	virtual void OnNodeMenuItemClicked(LPCTSTR MenuID,LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData)=0;
	virtual void OnRangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2)=0;
	virtual void OnColDblClicked(LPCTSTR ColValue)=0;
	//virtual void OnNodesCheckList (_variant_t & vtNodes)=0;
	virtual void OnNodeDeselect ()=0;
	virtual void OnTreeTerminated ()=0;
};

#endif
