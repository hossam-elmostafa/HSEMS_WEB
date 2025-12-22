 #include "stdafx.h"
#include "Listlisten.h"

void CListlistener::SetData(LPCTSTR lpszRetVal,LPCTSTR lpszAllData)
{
	m_pDataSinkWnd->SetWindowText(lpszRetVal);
}

void CListlistener::SetDataSink(CWnd *pSinkControl)
{
	m_pDataSinkWnd=pSinkControl;
}
