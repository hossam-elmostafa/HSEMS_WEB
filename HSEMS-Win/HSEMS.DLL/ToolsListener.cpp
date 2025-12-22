// ToolsListener.cpp: implementation of the CToolsListener class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ToolsListener.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CToolsListener, CCmdTarget)
    //{{AFX_EVENTSINK_MAP(CAboutDlg)
	ON_EVENT(CToolsListener, -1, 1  ,	OnAddNewBar,			VTS_DISPATCH VTS_BOOL VTS_I4)
	ON_EVENT(CToolsListener, -1, 2  ,	OnDeleteBar,			VTS_DISPATCH VTS_BOOL VTS_I4)
	ON_EVENT(CToolsListener, -1, 3  ,	OnChangeBar,			VTS_DISPATCH VTS_DISPATCH VTS_BOOL VTS_I4)
	ON_EVENT(CToolsListener, -1, 4  ,	OnUnloadPlan,			VTS_BOOL)
	ON_EVENT(CToolsListener, -1, 5  ,	OnPlanChange,			VTS_I2 VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL VTS_I4)
	ON_EVENT(CToolsListener, -1, 6  ,	OnNodeSelected,			VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR)
	ON_EVENT(CToolsListener, -1, 7  ,	OnRangeNotify,			VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CToolsListener, -1, 8  ,	OnNodeMenuPulled,		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR)
	ON_EVENT(CToolsListener, -1, 9  ,	OnNodeMenuItemClicked,	VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR)
	ON_EVENT(CToolsListener, -1, 10 ,	OnColDblClicked,		VTS_BSTR )
	ON_EVENT(CToolsListener, -1, 11 ,	OnNodeExpanded,			VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR)
	ON_EVENT(CToolsListener, -1, 13  ,	OnTreeTerminated,		VTS_NONE)
	ON_EVENT(CToolsListener, -1, 14  ,	OnNodeDeselect,			VTS_NONE)
		
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



INTERFACE_IMPL(CToolsListener,EventSink)

CToolsListener::CToolsListener()
{
	m_pToolsObj	=	new _Tools();
	m_pObject	= m_pToolsObj->m_lpDispatch;
	EstablishConnectionWithEventSrc();	
	m_pToolsCategory = NULL;
}

CToolsListener::~CToolsListener()
{
	EndConnectionWithEventSrc();
	if(m_pToolsObj)
	{
		delete m_pToolsObj;
		m_pToolsObj =NULL;
	}
}


///////////////////
//Events handling
//////////////////

void CToolsListener::OnAddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnAddNewBar( Bar, bActionCompleted, UserID);
}

void CToolsListener::OnDeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnDeleteBar(Bar, bActionCompleted, UserID);
}

void CToolsListener::OnChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnChangeBar(OldBar, NewBar, bActionCompleted, UserID);
}

void CToolsListener::OnUnloadPlan(BOOL bActionCompleted)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnUnloadPlan(bActionCompleted);
}

void CToolsListener::OnPlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnPlanChange(nRows, StartDate, EndDate, PlanStyle, bActionCompleted, UserID);
}

void CToolsListener::OnNodeSelected(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath,LPCTSTR strHiddenPath,short nNodeLevel,long UserID,LPCTSTR strData){}

void CToolsListener::OnRangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnRangeNotify(Form_Tag, Mask1, Mask2);
}

void CToolsListener::OnNodeMenuPulled( LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID ,LPCTSTR strData )
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnNodeMenuPulled ( TableName, ReturnValue, strVisablePath, strHiddenPath, nNodeLevel, UserID ,strData );

}

void CToolsListener::OnNodeMenuItemClicked(LPCTSTR MenuID,LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath , LPCTSTR strHiddenPath, short nNodeLevel,long UserID,LPCTSTR strData )
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnNodeMenuItemClicked(MenuID,TableName, ReturnValue, strVisablePath , strHiddenPath, nNodeLevel,UserID,strData );
}

void CToolsListener::OnColDblClicked(LPCTSTR ColValue)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnColDblClicked(ColValue);
}
void CToolsListener::OnNodeExpanded(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath,LPCTSTR strHiddenPath,short nNodeLevel,long UserID,LPCTSTR strData)
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnNodeExpanded(TableName, ReturnValue, strVisablePath , strHiddenPath , nNodeLevel, UserID,strData);
}

void CToolsListener::OnTreeTerminated()
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnTreeTerminated();
}
void CToolsListener::OnNodeDeselect()
{
	if(m_pToolsCategory)
		m_pToolsCategory->OnNodeDeselect();
}
