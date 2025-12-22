// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _Security wrapper class

class _Security : public COleDispatchDriver
{
public:
	_Security();
	~_Security();

	_Security(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Security(const _Security& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL IsAdminUser(long lUserID);
	short ShowLogScreen(LPCTSTR AppCode, BOOL ShowMessageFlag, BOOL LanguageFlag,LPCTSTR VersionNo);
	void ShowUsersScreen(LPCTSTR ApplicationCode, BOOL ShowMessageFlag, BOOL LanguageFlag, long lUserID);
	long ShowUsersLoginHistory(short ApplicationCode, BOOL Lang);
	VARIANT CheckUser(LPCTSTR ApplicationCode, LPCTSTR strUserLogin, LPCTSTR strUserPassword);
	void SetApplicationInfo(short nAppCode, LPCTSTR strProfileName);
	short ShowLogScreenEx(LPCTSTR AppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, BOOL bUseDefaultProfile,LPCTSTR VersionNo);
	void ShowSplashScreen(short lAppCode);
};
/////////////////////////////////////////////////////////////////////////////
// _Priv wrapper class

class _Priv : public COleDispatchDriver
{
public:
	_Priv();
	~_Priv();
	_Priv(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Priv(const _Priv& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void UpdateMenu(LPCTSTR AppCode, long MenuHandle, long lUserID, BOOL bLanguage, BOOL bReportPriv);
	void MenuBuild(LPCTSTR AppCode, long MenuHandle, long lUserID, BOOL bLanguage, BOOL bReportPriv);
	void ShowMenusItem(LPCTSTR MyAppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, BOOL bReportsMenu, long lUserID);
	void ShowMenusDetails(LPCTSTR MyAppCode, BOOL ShowMessageFlag, BOOL LanguageFlag, long lUserID);
	BOOL MenuVerify(LPCTSTR AppID, short UserNumber, long MenuID);
	BOOL ReportAllowVerify(LPCTSTR AppID, short UserNumber, long MenuID);
	VARIANT GetAllFieldAttrib(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, BOOL Lang);
	short FieldVerify(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, LPCTSTR FieldName, BSTR* FieldCaption, BOOL Lang, LPCTSTR LinkTableName);
	VARIANT ButtonsVerify(LPCTSTR AppID, short UserNumber, LPCTSTR TableName, LPCTSTR LinkTableName);
	CString GetUserLoginName(long UserID);
	CString GetUserFullName(long UserID);
	VARIANT ShowMenuReportsPriv(LPCTSTR strAppCode, BOOL bShowMessageFlag, BOOL bLanguage, long lUserID);
	long GetUserIDForApp(short nMainAppCode, LPCTSTR nMainUserLogin, short nAppCode);
	void SetApplicationInfo(short nAppCode, LPCTSTR strProfileName);
};
/////////////////////////////////////////////////////////////////////////////
// _SetUp wrapper class

class _SetUp : public COleDispatchDriver
{
public:
	_SetUp();
	~_SetUp();
	_SetUp(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_SetUp(const _SetUp& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetSystemDBName();
	VARIANT SetSystemDBName(LPCTSTR strFirstsysDBPath);
	short GetApplicationDBType();
	long ShowSetup(LPCTSTR dbName, LPCTSTR Login, LPCTSTR Password, BOOL ShowMessageFlag, BOOL Lang, short nAppCode, long lUserID);
	CString GetApplicationDataBaseName();
	CString GetApplicationReports();
	CString GetSystemDBLoginName();
	CString GetApplicationDBLoginName();
	CString GetApplicationDBPassword();
	CString GetSystemDBPassword();
	CString GetApplicationHelpPath();
	CString GetOraclePassword();
	BOOL IsParametersOK();
	CString GetDatabaseDateStyle();
	CString GetSQLServerUserID();
	CString GetSQLServerCatalog();
	CString GetSQLServerMachineName();
	CString GetSQLServerPassword();
	CString GetOracleUserID();
	CString GetOracleDataSource();
	CString GetServerName();
	short GetProtocolType();
	short GetOracleVersion();
	VARIANT ChangeDatabasePath(BOOL bLanguage, short nAppCode);
	BOOL GetHijriState();
	void SetApplicationCode(short nAppCode);
	CString GetWorkingFolder();
	short IsReplicationEnabled();
	CString GetReplicateFrom();
	CString GetReplicationLocationID();
	CString GetConnectionString(short nAppCode);
	CString GetCurrentProfile();
	void SetProfileName(LPCTSTR strProfileName);
	void SetApplicationInfo(short nAppCode, LPCTSTR strProfileName);
};
