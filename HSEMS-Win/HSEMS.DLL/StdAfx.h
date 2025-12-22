// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(_STDAFX_H_)
#define _STDAFX_H_
#pragma warning(disable:4996)
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define STRICT

#include <afxwin.h>
#include <afxdisp.h>

//#define _WIN32_WINNT 0x0400
#define _ATL_APARTMENT_THREADED

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#define	APP_ODBC		"HSEMSREPORTS"
#define	APP_NAME		"HSEMS"
#define	APP_CODE		66
#define	APPCODE		66				//used in old HSE
#define	STR_APP_CODE	"66"

#include <atlcom.h>
#include <afxdtctl.h>
#include "polydb.h"
#include "TOOLSCATEGORY.H"
#include "polytools.h"
#include "AfxDao.h"		// For DAO work
#include "eventimp.h"
#include "Category.h"
#include "CommonCategory.h"
#include "CommonCategoryWrapper.h"
#include "HSEMSCommonCategory.h"
#include "firstdatabaseprivilegetool.h"
#include "Connection.h"
//#include "WaitWnd.h"
#include <afxctl.h >
#include <afxext.h>         // MFC extensions
#include "..\\HSEMS.EXE\\Resource.h"
#include "resource.h"
#include "Definitions.h"


//Old import before using H drive
/*
#import "FDesktop.dll" no_namespace 
#import "DnglClnt.dll" no_namespace
#import "RepModule.dll" no_namespace
#import <RegistryHandle.dll> no_namespace
#import "VBALib.dll" no_namespace
#import "PSSendEmail.dll" no_namespace
#import "EGPCReports.dll" no_namespace
*/


#import "H:\Input\HSEMS\Shared\FDesktop.dll" no_namespace 
#import "H:\Input\HSEMS\Shared\DnglClnt.dll" no_namespace
#import "H:\Input\HSEMS\Shared\RepModule.dll" no_namespace
#import "H:\Input\HSEMS\Shared\RegistryHandle.dll" no_namespace
#import "H:\Input\HSEMS\Shared\VBALib.dll" no_namespace
#import "H:\Input\HSEMS\Shared\PSSendEmail.dll" no_namespace
#import "H:\Input\HSEMS\Shared\EGPCReports.dll" no_namespace
#import "H:\Input\HSEMS\Shared\MonthlyReport.dll" no_namespace



#endif // !defined(AFX_STDAFX_H__51FA7D18_82F3_11D2_A9BC_444553540000__INCLUDED)
