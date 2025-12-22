 // Connection.cpp : implementation file
//

#include "stdafx.h"
//#include "Connection.h"

//#include "firstprivilegetool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnection

CConnection::CConnection()
{
	CoInitialize(NULL);
	m_pSetupConnection		=new _SetUp;
//	m_pSetupConnection->SetSettingMode(1);
	m_pSecurityConnection	=new _Security;
	m_pPrivConnection		=new _Priv;
	m_pObject				=m_pPrivConnection->m_lpDispatch;
	EstablishConnectionWithEventSrc();
}

void CConnection::SetApplicationCode(int nAppCode)
{
	//m_pSetupConnection->SetApplicationCode(nAppCode);//mahmoud
	m_pSetupConnection->SetProfileName(m_pSetupConnection->GetCurrentProfile());//mahmoud
}

CConnection::~CConnection()
{
	EndConnectionWithEventSrc();
	delete m_pSetupConnection;
	delete m_pSecurityConnection;
	delete m_pPrivConnection;
	CoUninitialize(); 
}


BEGIN_EVENTSINK_MAP(CConnection, CCmdTarget)
	ON_EVENT(CConnection, -1, 1  , OnEventUserNumber, VTS_I4)
	ON_EVENT(CConnection, -1, 2  , OnEventCloseScreen,NULL)

END_EVENTSINK_MAP()

INTERFACE_IMPL(CConnection,EventSink)

/////////////////////////////////////////////////////////////////////////////
// CConnection message handlers


void CConnection::OnEventUserNumber(long User_Num)
{
}

void CConnection::OnEventCloseScreen()
{
}

