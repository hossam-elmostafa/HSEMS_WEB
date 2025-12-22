// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
//#include "firstdatabaseprivilegetool.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


_Priv::_Priv()
{
	CreateDispatch("FirstPrivilegeTool.Priv");
}

_Priv::~_Priv()
{
	ReleaseDispatch();
}

_Security::_Security()
{
	CreateDispatch("FirstPrivilegeTool.Security");
}

_Security::~_Security()
{
	ReleaseDispatch();
}

_SetUp::_SetUp()
{
	CreateDispatch("FirstPrivilegeTool.Setup");
}

_SetUp::~_SetUp()
{
	ReleaseDispatch();
}

BOOL _Security::IsAdminUser(long lUserID)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		lUserID);
	return result;
}

short _Security::ShowLogScreen(LPCTSTR AppCode, BOOL ShowMessageFlag, BOOL LanguageFlag,LPCTSTR strVersionNo)
{	
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_BSTR ;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		AppCode, ShowMessageFlag, LanguageFlag,strVersionNo);
	return result;
}

void _Security::ShowUsersScreen(LPCTSTR ApplicationCode, BOOL ShowMessageFlag, BOOL LanguageFlag, long lUserID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ApplicationCode, ShowMessageFlag, LanguageFlag, lUserID);
}

long _Security::ShowUsersLoginHistory(short ApplicationCode, BOOL Lang)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ApplicationCode, Lang);
	return result;
}

VARIANT _Security::CheckUser(LPCTSTR ApplicationCode, LPCTSTR strUserLogin, LPCTSTR strUserPassword)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		ApplicationCode, strUserLogin, strUserPassword);
	return result;
}

void _Security::SetApplicationInfo(short nAppCode, LPCTSTR strProfileName)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nAppCode, strProfileName);
}

short _Security::ShowLogScreenEx(LPCTSTR AppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, BOOL bUseDefaultProfile,LPCTSTR VersionNo)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BSTR ;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		AppCode, ShowMessageFlag, LanguageFlag, bUseDefaultProfile,VersionNo);
	return result;
}

void _Security::ShowSplashScreen(short lAppCode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 lAppCode);
}

/////////////////////////////////////////////////////////////////////////////
// _Priv properties

/////////////////////////////////////////////////////////////////////////////
// _Priv operations

void _Priv::UpdateMenu(LPCTSTR AppCode, long MenuHandle, long lUserID, BOOL bLanguage, BOOL bReportPriv)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 AppCode, MenuHandle, lUserID, bLanguage, bReportPriv);
}

void _Priv::MenuBuild(LPCTSTR AppCode, long MenuHandle, long lUserID, BOOL bLanguage, BOOL bReportPriv)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 AppCode, MenuHandle, lUserID, bLanguage, bReportPriv);
}

void _Priv::ShowMenusItem(LPCTSTR MyAppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, BOOL bReportsMenu, long lUserID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MyAppCode, ShowMessageFlag, LanguageFlag, bReportsMenu, lUserID);
}

void _Priv::ShowMenusDetails(LPCTSTR MyAppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, long lUserID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 MyAppCode, ShowMessageFlag, LanguageFlag, lUserID);
}

BOOL _Priv::MenuVerify(LPCTSTR AppID, short UserNumber, long MenuID)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I4;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		AppID, UserNumber, MenuID);
	return result;
}

BOOL _Priv::ReportAllowVerify(LPCTSTR AppID, short UserNumber, long MenuID)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I4;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		AppID, UserNumber, MenuID);
	return result;
}

VARIANT _Priv::GetAllFieldAttrib(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, BOOL Lang)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		AppID, UserNumber, TableName, Lang);
	return result;
}

short _Priv::FieldVerify(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, LPCTSTR FieldName, BSTR* FieldCaption, BOOL Lang, LPCTSTR LinkTableName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_PBSTR VTS_BOOL VTS_BSTR;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		AppID, UserNumber, TableName, FieldName, FieldCaption, Lang, LinkTableName);
	return result;
}

VARIANT _Priv::ButtonsVerify(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, LPCTSTR LinkTableName)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		AppID, UserNumber, TableName, LinkTableName);
	return result;
}

CString _Priv::GetUserLoginName(long UserID)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		UserID);
	return result;
}

CString _Priv::GetUserFullName(long UserID)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		UserID);
	return result;
}

VARIANT _Priv::ShowMenuReportsPriv(LPCTSTR strAppCode, BOOL bShowMessageFlag, BOOL bLanguage, long lUserID)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_I4;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strAppCode, bShowMessageFlag, bLanguage, lUserID);
	return result;
}

long _Priv::GetUserIDForApp(short nMainAppCode, LPCTSTR nMainUserLogin, short nAppCode)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_I2;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMainAppCode, nMainUserLogin, nAppCode);
	return result;
}

void _Priv::SetApplicationInfo(short nAppCode, LPCTSTR strProfileName)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030017, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nAppCode, strProfileName);
}


/////////////////////////////////////////////////////////////////////////////
// _SetUp properties

/////////////////////////////////////////////////////////////////////////////
// _SetUp operations

CString _SetUp::GetSystemDBName()
{
	CString result;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

VARIANT _SetUp::SetSystemDBName(LPCTSTR strFirstsysDBPath)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strFirstsysDBPath);
	return result;
}

short _SetUp::GetApplicationDBType()
{
	short result;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long _SetUp::ShowSetup(LPCTSTR dbName, LPCTSTR Login, LPCTSTR Password, BOOL ShowMessageFlag, BOOL Lang, short nAppCode, long lUserID)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_I2 VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		dbName, Login, Password, ShowMessageFlag, Lang, nAppCode, lUserID);
	return result;
}

CString _SetUp::GetApplicationDataBaseName()
{
	CString result;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetApplicationReports()
{
	CString result;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSystemDBLoginName()
{
	CString result;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetApplicationDBLoginName()
{
	CString result;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetApplicationDBPassword()
{
	CString result;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSystemDBPassword()
{
	CString result;
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetApplicationHelpPath()
{
	CString result;
	InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetOraclePassword()
{
	CString result;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _SetUp::IsParametersOK()
{
	BOOL result;
	InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetDatabaseDateStyle()
{
	CString result;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSQLServerUserID()
{
	CString result;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSQLServerCatalog()
{
	CString result;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSQLServerMachineName()
{
	CString result;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetSQLServerPassword()
{
	CString result;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetOracleUserID()
{
	CString result;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetOracleDataSource()
{
	CString result;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetServerName()
{
	CString result;
	InvokeHelper(0x60030017, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short _SetUp::GetProtocolType()
{
	short result;
	InvokeHelper(0x60030018, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _SetUp::GetOracleVersion()
{
	short result;
	InvokeHelper(0x60030019, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

VARIANT _SetUp::ChangeDatabasePath(BOOL bLanguage, short nAppCode)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BOOL VTS_I2;
	InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		bLanguage, nAppCode);
	return result;
}

BOOL _SetUp::GetHijriState()
{
	BOOL result;
	InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _SetUp::SetApplicationCode(short nAppCode)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nAppCode);
}

CString _SetUp::GetWorkingFolder()
{
	CString result;
	InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short _SetUp::IsReplicationEnabled()
{
	short result;
	InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetReplicateFrom()
{
	CString result;
	InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetReplicationLocationID()
{
	CString result;
	InvokeHelper(0x60030020, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _SetUp::GetConnectionString(short nAppCode)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030021, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nAppCode);
	return result;
}

CString _SetUp::GetCurrentProfile()
{
	CString result;
	InvokeHelper(0x60030025, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _SetUp::SetProfileName(LPCTSTR strProfileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030026, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strProfileName);
}

void _SetUp::SetApplicationInfo(short nAppCode, LPCTSTR strProfileName)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030027, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nAppCode, strProfileName);
}
