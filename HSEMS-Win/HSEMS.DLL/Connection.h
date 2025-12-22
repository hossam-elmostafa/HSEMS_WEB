#if !defined(_CONNECTION_H_)
#define _CONNECTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Connection.h : header file
//

#include"eventimp.h"
class _SetUp;
class _Security;
class _Priv;

/////////////////////////////////////////////////////////////////////////////
// CConnection command target

class CConnection : public CCmdTarget
{

// Attributes
public:
	CConnection();           // protected constructor used by dynamic creation
	~CConnection();
	void SetApplicationCode(int nAppCode);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnection)
	//}}AFX_VIRTUAL

BEGIN_INTERFACE_PART_NEW(EventSink)
	afx_msg void OnEventUserNumber(long User_Num);
	afx_msg void OnEventCloseScreen();
DECLARE_EVENTSINK_MAP()

// Implementation

public:
	_SetUp		*m_pSetupConnection;
	_Security	*m_pSecurityConnection;
	_Priv		*m_pPrivConnection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_CONNECTION_H_)
