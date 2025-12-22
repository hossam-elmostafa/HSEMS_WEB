/////////////////////////////////////////////////////////////////////////////
// _DeclarationsObj wrapper class

class _DeclarationsObj : public COleDispatchDriver
{
public:
	_DeclarationsObj() {}		// Calls COleDispatchDriver default constructor
	_DeclarationsObj(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_DeclarationsObj(const _DeclarationsObj& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'SetStuct' not emitted because of invalid return type or parameter type
	// method 'GetStuct' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// _Tools wrapper class

class _Tools : public COleDispatchDriver
{
public:
	_Tools() ;//{}		// Calls COleDispatchDriver default constructor
	~_Tools();
	_Tools(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Tools(const _Tools& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SetPlanScale(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, LPCTSTR StartTime, LPCTSTR EndTime, BOOL bShowTaskDetails);
	short ShowPlan(LPCTSTR strTitle, long Parent_Handle, long Window_State);
	short CreateBar(short nRow, LPCTSTR BarDescription, LPCTSTR X1, LPCTSTR X2, long DescriptionColor, long BarColor);
	short DeleteBar(short nRow, LPCTSTR X1);
	short MoveBar(short nRow, LPCTSTR X1, LPCTSTR NewX1, LPCTSTR NewY1);
	short CopyBar(short nRow, LPCTSTR X1, LPCTSTR X2);
	// method 'GetBarData' not emitted because of invalid return type or parameter type
	short GetBarsCount(short nRow);
	void CancelAction();
	BOOL GetCurrentLanguage();
	void SetCurrentLanguage(BOOL bNewValue);
	CString GetCalendarDate(LPCTSTR DefaultDate, LPCTSTR CalendarTitle, BOOL bHijriState);
	void ShowFixedLines(BOOL bSetting);
	void ShowBandLines(BOOL bSetting);
	short AVIPlayNow(LPCTSTR AVI_File, LPCTSTR AVI_Title, long Parent_Handle);
	short WAVPlayNow(LPCTSTR WAV_File);
	short PictureShow(LPCTSTR Picture_File, LPCTSTR Picture_Title, long Parent_Handle);
	BOOL RangeEnter(LPCTSTR Form_Tag, LPCTSTR RangeTitle, LPCTSTR Line1, LPCTSTR Line2, LPCTSTR Mask1, LPCTSTR Mask2, BSTR* strRetVal1, BSTR* strRetVal2, LPCTSTR Default1, LPCTSTR Default2);
	VARIANT SetArrayDimentions(short xRow, short yCol);
	VARIANT FillChartArray(short xRow, short yCol, LPCTSTR Value);
	VARIANT ShowChart(LPCTSTR ChartTitle, long ChartType, long ChartStyle, long Window_State, long Parent_Handle, BOOL Language);
	VARIANT SetPolyDB(LPDISPATCH PolyDBObj);
	long DBInit(short ApplicationCode, short UserIDNumber, short DB_Type, LPCTSTR strConnectionString, LPCTSTR strFirstsysDB, LPCTSTR strProfile="");
	long FillTree(LPCTSTR SQL_Statement, LPCTSTR Displayed_Field, LPCTSTR Return_Field, LPCTSTR RecordID, LPCTSTR Node_Image, LPCTSTR NodeCaption, BOOL bSeparated_Nodes, BOOL bShowFilterScreen, BSTR* strRetCriteria, BOOL bSortedItems=FALSE,BOOL bExpanded=FALSE);
	VARIANT ShowTree(LPCTSTR TreeTitle, LPCTSTR TreeIcon, long Window_State, long Parent_Handle, BOOL bCheckMode, BOOL bRecodingMode, LPCTSTR strListItems);
	VARIANT SetTreeMenu(LPCTSTR strMenu);
	BOOL ShowFixedColsItem(long Item, BOOL bShowStatus);
	void EnabledToolBarButton(long FormType, long ButtonID, BOOL bStatus);
	void FormDoAction(long FormType, long ButtonID);
	BOOL IsEmptyCell(short nRow, LPCTSTR X1);
	BOOL ColorBar(short nRow, LPCTSTR X1, long nColor);
	VARIANT CloseTree();
	short SetAppCode(short AppCode);
	CString GetServerDate(LPCTSTR strFormat, BOOL bHijriState);
	VARIANT ShowUserDefinedChecks();
	VARIANT ShowSegmentsSystem(long lParentHandle);
	BOOL IsCodingAllowed(short iAppCode, LPCTSTR strFormTag);
	CString GetSegmentedCode(short iAppCode, LPCTSTR strFormTag, LPCTSTR strFullRecord, BOOL bIncrementSerial);
	void IncreaseSegment(short iAppCode, LPCTSTR strFormTag, LPCTSTR strFullRecord);
	long RebuildSegments(short iAppCode, LPCTSTR strFormTag, LPCTSTR strAppSQL);
	void MoveTree(short nLeft, short nTop, short nWidth, short nHeight);
};
/////////////////////////////////////////////////////////////////////////////
// __Tools wrapper class

class __Tools : public COleDispatchDriver
{
public:
	__Tools() {}		// Calls COleDispatchDriver default constructor
	__Tools(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	__Tools(const __Tools& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void AddNewBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID);
	void DeleteBar(LPDISPATCH Bar, BOOL bActionCompleted, long UserID);
	void ChangeBar(LPDISPATCH OldBar, LPDISPATCH NewBar, BOOL bActionCompleted, long UserID);
	void UnloadPlan(BOOL bActionCompleted);
	void PlanChange(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, BOOL bActionCompleted, long UserID);
	void NodeSelected(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID, LPCTSTR strData);
	void RangeNotify(LPCTSTR Form_Tag, LPCTSTR Mask1, LPCTSTR Mask2);
	void NodeMenuPulled(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID, LPCTSTR strData);
	void NodeMenuItemClicked(LPCTSTR MenuID, LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID, LPCTSTR strData);
	void ColDblClicked(LPCTSTR ColValue);
	void NodeExpanded(LPCTSTR TableName, LPCTSTR ReturnValue, LPCTSTR strVisablePath, LPCTSTR strHiddenPath, short nNodeLevel, long UserID, LPCTSTR strData);
	void NodesCheckListRecoding(const VARIANT& vtNodes);
	void TreeTerminated();
	void NodeDeselect();
	void NodesCheckList(const VARIANT& vtNodes, const VARIANT& vtTableName, const VARIANT& ReturnValue, const VARIANT& strVisablePath, const VARIANT& strHiddenPath, const VARIANT& nNodeLevel, long UserID, const VARIANT& vtData);
};
