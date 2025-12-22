#include"stdafx.h"
#include "DongleListen.h"
#include "CMNObjBase.h"
BEGIN_EVENTSINK_MAP(CDongleListen, CCmdTarget)
    //{{AFX_EVENTSINK_MAP(CAboutDlg)
	//ON_EVENT(CDongleListen, -1, 1  ,OnEventSignOn, VTS_I4)
	ON_EVENT(CDongleListen, -1, 1  ,OnEventSignOn, VTS_I2 VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

INTERFACE_IMPL(CDongleListen,EventSink)
int nCheckCounter = 0;
CDongleListen::CDongleListen(CCMNObjBase *pHSEObj,CString strServerName,int iProtocol,int iAppCode,CString strUserName)
{
	m_bApplyDongle=FALSE;//means disable dongle
	m_pHSEObj =pHSEObj;
	try
	{
		HRESULT hr =m_DnglPrxy.CreateInstance(__uuidof(DnglPrxy));
		if(hr!=0)
		{
			FatalAppExit(0,"Dongle object not Installed");
		}
		m_pHSEObj =pHSEObj;
		m_pObject =m_DnglPrxy;
		EstablishConnectionWithEventSrc(__uuidof(_IDnglPrxyEvents));
		hr = m_DnglPrxy->SetConnectionInfo(strServerName.AllocSysString(),0);
		if(hr!=0)
		{
			hr = m_DnglPrxy->SetConnectionInfo(strServerName.AllocSysString(),1);
		}
		m_DnglPrxy->SignOn(APP_CODE,strUserName.AllocSysString());
	}
	catch(...)
	{
	}

}
CDongleListen::~CDongleListen()
{
	try
	{
		EndConnectionWithEventSrc();
		m_DnglPrxy.Release();
	}
	catch(...)
	{
	}

}
void CDongleListen::OnEventSignOn(int iFlag,LPCTSTR strMessage)
{
	CString strLocalMessage = strMessage;
	if(!m_bApplyDongle)
		return;
	if(iFlag <= 0)
	{
		if(iFlag==0 || iFlag ==-1 || iFlag ==-2)
		{
			m_pHSEObj->FireCloseAll();
			FatalAppExit(0,strLocalMessage);
		}
		else if(iFlag ==-4)
		{
			nCheckCounter++;
			m_DnglPrxy->EnableFire(false);
			if(nCheckCounter>3)
			{
				m_pHSEObj->FireCloseAll();
				FatalAppExit(0,"Cannot Contact Server");
			}
			else
				AfxMessageBox("Check Your Network connection");
			m_DnglPrxy->EnableFire(true);
		}

		else if(iFlag ==-5)
		{
			MessageBox(NULL,"Cannot find the server\nPlease change it and reconnect again","Prompt",MB_OK| MB_RIGHT);
			CString strFormat;
			strFormat.Format("Please reconnect again.",strMessage);
			m_pHSEObj->FireCloseAll();
			FatalAppExit(0,strFormat);
		}
		else if(iFlag ==-6)
		{
			m_pHSEObj->FireCloseAll();
			FatalAppExit(0,strMessage);
		}

	}
	else
		nCheckCounter=0;

}

void CDongleListen::SignOut(int iFlag)
{
	if(!m_bApplyDongle)
		return;
	try
	{
		m_DnglPrxy->SignOut();
	}
	catch(...)
	{
	}
}
