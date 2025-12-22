// HSEMS.cpp : Implementation of DLL Exports.

            
// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f HSEMSps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include "initguid.h"
#include "HSEMS.h"

#include "HSEMS_i.c"
#include "HSEObj.h"
#include "HSEMSapp.h"


#ifdef	_DEBUG
#define	new	DEBUG_NEW
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_HSEObj, CHSEObj)
END_OBJECT_MAP()


CHSEMSApp theApp;

BOOL CHSEMSApp::InitInstance()
{
	AfxEnableControlContainer();
	_Module.Init(ObjectMap, m_hInstance);
	SetHSEObj(NULL);
	return CWinApp::InitInstance();
}

int CHSEMSApp::ExitInstance()
{
	_Module.Term();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	_Module.UnregisterServer();
	return S_OK;
}


