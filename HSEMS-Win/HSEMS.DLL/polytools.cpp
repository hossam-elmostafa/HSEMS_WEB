// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
//#include "polytools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// _DeclarationsObj properties

/////////////////////////////////////////////////////////////////////////////
// _DeclarationsObj operations


/////////////////////////////////////////////////////////////////////////////
// _Tools properties

/////////////////////////////////////////////////////////////////////////////
// _Tools operations
_Tools::_Tools()
{
	CreateDispatch ("PolyTools.Tools");
}

_Tools::~_Tools()
{
	ReleaseDispatch();
}

/////////////////////////////////////////////////////////////////////////////
// _Tools properties

/////////////////////////////////////////////////////////////////////////////
// _Tools operations

void _Tools::SetPlanScale(short nRows, LPCTSTR StartDate, LPCTSTR EndDate, long PlanStyle, LPCTSTR StartTime, LPCTSTR EndTime, BOOL bShowTaskDetails)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nRows, StartDate, EndDate, PlanStyle, StartTime, EndTime, bShowTaskDetails);
}

short _Tools::ShowPlan(LPCTSTR strTitle, long Parent_Handle, long Window_State)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strTitle, Parent_Handle, Window_State);
	return result;
}

short _Tools::CreateBar(short nRow, LPCTSTR BarDescription, LPCTSTR X1, LPCTSTR X2, long DescriptionColor, long BarColor)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nRow, BarDescription, X1, X2, DescriptionColor, BarColor);
	return result;
}

short _Tools::DeleteBar(short nRow, LPCTSTR X1)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nRow, X1);
	return result;
}

short _Tools::MoveBar(short nRow, LPCTSTR X1, LPCTSTR NewX1, LPCTSTR NewY1)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nRow, X1, NewX1, NewY1);
	return result;
}

short _Tools::CopyBar(short nRow, LPCTSTR X1, LPCTSTR X2)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nRow, X1, X2);
	return result;
}

short _Tools::GetBarsCount(short nRow)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		nRow);
	return result;
}

void _Tools::CancelAction()
{
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Tools::GetCurrentLanguage()
{
	BOOL result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Tools::SetCurrentLanguage(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString _Tools::GetCalendarDate(LPCTSTR DefaultDate, LPCTSTR CalendarTitle, BOOL bHijriState)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		DefaultDate, CalendarTitle, bHijriState);
	return result;
}

void _Tools::ShowFixedLines(BOOL bSetting)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030019, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bSetting);
}

void _Tools::ShowBandLines(BOOL bSetting)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 bSetting);
}

short _Tools::AVIPlayNow(LPCTSTR AVI_File, LPCTSTR AVI_Title, long Parent_Handle)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		AVI_File, AVI_Title, Parent_Handle);
	return result;
}

short _Tools::WAVPlayNow(LPCTSTR WAV_File)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		WAV_File);
	return result;
}

short _Tools::PictureShow(LPCTSTR Picture_File, LPCTSTR Picture_Title, long Parent_Handle)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Picture_File, Picture_Title, Parent_Handle);
	return result;
}

BOOL _Tools::RangeEnter(LPCTSTR Form_Tag, LPCTSTR RangeTitle, LPCTSTR Line1, LPCTSTR Line2, LPCTSTR Mask1, LPCTSTR Mask2, BSTR* strRetVal1, BSTR* strRetVal2, LPCTSTR Default1, LPCTSTR Default2)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_PBSTR VTS_PBSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Form_Tag, RangeTitle, Line1, Line2, Mask1, Mask2, strRetVal1, strRetVal2, Default1, Default2);
	return result;
}

VARIANT _Tools::SetArrayDimentions(short xRow, short yCol)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		xRow, yCol);
	return result;
}

VARIANT _Tools::FillChartArray(short xRow, short yCol, LPCTSTR Value)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030020, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		xRow, yCol, Value);
	return result;
}

VARIANT _Tools::ShowChart(LPCTSTR ChartTitle, long ChartType, long ChartStyle, long Window_State, long Parent_Handle, BOOL Language)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL;
	InvokeHelper(0x60030021, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		ChartTitle, ChartType, ChartStyle, Window_State, Parent_Handle, Language);
	return result;
}

VARIANT _Tools::SetPolyDB(LPDISPATCH PolyDBObj)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60030022, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		PolyDBObj);
	return result;
}

long _Tools::DBInit(short ApplicationCode, short UserIDNumber, short DB_Type, LPCTSTR strConnectionString, LPCTSTR strFirstsysDB, LPCTSTR strProfile)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030023, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		ApplicationCode, UserIDNumber, DB_Type, strConnectionString, strFirstsysDB, strProfile);
	return result;
}

long _Tools::FillTree(LPCTSTR SQL_Statement, LPCTSTR Displayed_Field, LPCTSTR Return_Field, LPCTSTR RecordID, LPCTSTR Node_Image, LPCTSTR NodeCaption, BOOL bSeparated_Nodes, BOOL bShowFilterScreen, BSTR* strRetCriteria, BOOL bSortedItems, BOOL bExpanded)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL VTS_PBSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030024, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		SQL_Statement, Displayed_Field, Return_Field, RecordID, Node_Image, NodeCaption, bSeparated_Nodes, bShowFilterScreen, strRetCriteria, bSortedItems,bExpanded);
	return result;
}

VARIANT _Tools::ShowTree(LPCTSTR TreeTitle, LPCTSTR TreeIcon, long Window_State, long Parent_Handle, BOOL bCheckMode, BOOL bRecodingMode, LPCTSTR strListItems)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL VTS_BOOL VTS_BSTR;
	InvokeHelper(0x60030025, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		TreeTitle, TreeIcon, Window_State, Parent_Handle, bCheckMode, bRecodingMode, strListItems);
	return result;
}

VARIANT _Tools::SetTreeMenu(LPCTSTR strMenu)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030026, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strMenu);
	return result;
}

BOOL _Tools::ShowFixedColsItem(long Item, BOOL bShowStatus)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x6003002e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Item, bShowStatus);
	return result;
}

void _Tools::EnabledToolBarButton(long FormType, long ButtonID, BOOL bStatus)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BOOL;
	InvokeHelper(0x6003002f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FormType, ButtonID, bStatus);
}

void _Tools::FormDoAction(long FormType, long ButtonID)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60030030, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FormType, ButtonID);
}

BOOL _Tools::IsEmptyCell(short nRow, LPCTSTR X1)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030031, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nRow, X1);
	return result;
}

BOOL _Tools::ColorBar(short nRow, LPCTSTR X1, long nColor)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_I4;
	InvokeHelper(0x60030032, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		nRow, X1, nColor);
	return result;
}

VARIANT _Tools::CloseTree()
{
	VARIANT result;
	InvokeHelper(0x60030034, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

short _Tools::SetAppCode(short AppCode)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030035, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		AppCode);
	return result;
}

CString _Tools::GetServerDate(LPCTSTR strFormat, BOOL bHijriState)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030036, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strFormat, bHijriState);
	return result;
}

VARIANT _Tools::ShowUserDefinedChecks()
{
	VARIANT result;
	InvokeHelper(0x60030037, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _Tools::ShowSegmentsSystem(long lParentHandle)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003003e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		lParentHandle);
	return result;
}

BOOL _Tools::IsCodingAllowed(short iAppCode, LPCTSTR strFormTag)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6003003f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		iAppCode, strFormTag);
	return result;
}

CString _Tools::GetSegmentedCode(short iAppCode, LPCTSTR strFormTag, LPCTSTR strFullRecord, BOOL bIncrementSerial)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030040, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		iAppCode, strFormTag, strFullRecord, bIncrementSerial);
	return result;
}

void _Tools::IncreaseSegment(short iAppCode, LPCTSTR strFormTag, LPCTSTR strFullRecord)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030042, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iAppCode, strFormTag, strFullRecord);
}

long _Tools::RebuildSegments(short iAppCode, LPCTSTR strFormTag, LPCTSTR strAppSQL)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030044, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAppCode, strFormTag, strAppSQL);
	return result;
}

void _Tools::MoveTree(short nLeft, short nTop, short nWidth, short nHeight)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x60030047, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nLeft, nTop, nWidth, nHeight);
}