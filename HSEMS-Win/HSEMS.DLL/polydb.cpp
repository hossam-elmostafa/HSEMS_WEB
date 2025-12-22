// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
//#include "polydb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _DB properties

/////////////////////////////////////////////////////////////////////////////
// _DB operations
_DB::_DB()
{
	CreateDispatch ("PolyDB.DB");
}

_DB::~_DB()
{
	ReleaseDispatch();
}

long _DB::BuildTree(LPCTSTR Table_Name, LPCTSTR Tree_Title, LPCTSTR Index, LPCTSTR Displayed_Field, LPCTSTR Return_Field, LPCTSTR Link_Table, LPCTSTR Link_Field, BOOL finish)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Table_Name, Tree_Title, Index, Displayed_Field, Return_Field, Link_Table, Link_Field, finish);
	return result;
}

long _DB::BuildTreeSingleTable(LPCTSTR Table_Name, LPCTSTR Form_Title, LPCTSTR Item_Key, LPCTSTR Parent_Key, LPCTSTR Displayed_Field, LPCTSTR Return_Field, short Style, LPCTSTR SelectedItemKey)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Table_Name, Form_Title, Item_Key, Parent_Key, Displayed_Field, Return_Field, Style, SelectedItemKey);
	return result;
}

void _DB::HandleAction(short Form_Type, BOOL Cancel, short Selected_Records, LPCTSTR Selection_Formula)
{
	static BYTE parms[] =
		VTS_I2 VTS_BOOL VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Type, Cancel, Selected_Records, Selection_Formula);
}

long _DB::RecGetAbsolute()
{
	long result;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

short _DB::FormSetField(LPCTSTR Table_Name, LPCTSTR Field_Name, LPCTSTR Field_Value, short Form_Type)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Table_Name, Field_Name, Field_Value, Form_Type);
	return result;
}

short _DB::RecSetBOF()
{
	short result;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL _DB::RecSetEOF()
{
	BOOL result;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

short _DB::RecUpdate()
{
	short result;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecClose()
{
	short result;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::PositionMaskSingle(short Relative_Row, short Relative_Column)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Relative_Row, Relative_Column);
	return result;
}

long _DB::ReportPrint(LPCTSTR Report_File, LPCTSTR Report_Title, LPCTSTR Selection_Formula, LPCTSTR Formulas, long Window_State, BOOL bExport, long Parent_Handle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Report_File, Report_Title, Selection_Formula, Formulas, Window_State, bExport, Parent_Handle);
	return result;
}

long _DB::ListShow(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, long Parent_Handle, LPCTSTR strAttribTableName, long lHelpID)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_I4 VTS_BSTR VTS_I4;
	InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		List_Tag, List_Title, SQL_Statement, Return_Field, Old_Value, MultiSelection, Parent_Handle, strAttribTableName, lHelpID);
	return result;
}

BOOL _DB::GridSetFocus(LPCTSTR Field_Name, BOOL Cancel_Update)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Field_Name, Cancel_Update);
	return result;
}

LPDISPATCH _DB::DBCloneSet()
{
	LPDISPATCH result;
	InvokeHelper(0x60030010, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short _DB::ReportEnable(short Form_Type, LPCTSTR Report_File)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030011, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Form_Type, Report_File);
	return result;
}

CString _DB::FormGetField(LPCTSTR Table_Name, LPCTSTR Field_Name, short Form_Type)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x60030012, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Table_Name, Field_Name, Form_Type);
	return result;
}

short _DB::FormUpdateGrid(LPCTSTR Table_Name, short Form_Type)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0x60030013, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Table_Name, Form_Type);
	return result;
}

CString _DB::FormCancelUpdate(LPCTSTR Table_Name, short Form_Type)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0x60030014, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Table_Name, Form_Type);
	return result;
}

short _DB::LinkFieldsAttribSingle(LPCTSTR Link_Name, LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, 
		LPCTSTR Field_F_SQL, LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x60030015, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Link_Name, Field_Name, Field_Caption, Max_Length, Field_Help, Field_Attrib_Rules, Field_Mask, Field_Default, Field_Style, Field_F_Table, Field_F_SQL, Field_F_Field, Field_Ret_Field, strBrowsingArray, ArabicHelpContextID, 
		EnglishHelpContextID, strBrowsingHelpFields);
	return result;
}

short _DB::LinkFieldsAttrib(LPCTSTR Table_Name)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030016, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Table_Name);
	return result;
}

short _DB::LinkSetAttribSingle(LPCTSTR LinkName, LPCTSTR LinkCaption, LPCTSTR LinkLocalField, LPCTSTR LinkOtherTable, LPCTSTR LinkOtherField, LPCTSTR LinkSQL, short LinkMaxRecs, LPCTSTR UniqueFields, long ArabicHelpContextID, 
		long EnglishHelpContextID, LPCTSTR strTabAttribute)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x60030017, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		LinkName, LinkCaption, LinkLocalField, LinkOtherTable, LinkOtherField, LinkSQL, LinkMaxRecs, UniqueFields, ArabicHelpContextID, EnglishHelpContextID, strTabAttribute);
	return result;
}

short _DB::LinkSetAttrib(LPCTSTR Table_Name)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030018, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Table_Name);
	return result;
}

short _DB::FieldSetAttribSingle(LPCTSTR Field_Name, LPCTSTR Field_Caption, short Max_Length, LPCTSTR Field_Help, LPCTSTR Field_Attrib_Rules, LPCTSTR Field_Mask, LPCTSTR Field_Default, short Field_Style, LPCTSTR Field_F_Table, LPCTSTR Field_F_SQL, 
		LPCTSTR Field_F_Field, LPCTSTR Field_Ret_Field, LPCTSTR strBrowsingArray, long ArabicHelpContextID, long EnglishHelpContextID, LPCTSTR strBrowsingHelpFields)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x60030019, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Field_Name, Field_Caption, Max_Length, Field_Help, Field_Attrib_Rules, Field_Mask, Field_Default, Field_Style, Field_F_Table, Field_F_SQL, Field_F_Field, Field_Ret_Field, strBrowsingArray, ArabicHelpContextID, EnglishHelpContextID, 
		strBrowsingHelpFields);
	return result;
}

short _DB::FieldSetAttrib(LPCTSTR Table_Name)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Table_Name);
	return result;
}

LPDISPATCH _DB::GridShow(LPCTSTR Title, BOOL AllowUpdate, BOOL AllowAdd, BOOL AllowDelete, LPCTSTR Form_Tag, long Parent_Handle, short Window_State, long StartingFromPosition, long HelpID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL VTS_BOOL VTS_BSTR VTS_I4 VTS_I2 VTS_I4 VTS_I4;
	InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Title, AllowUpdate, AllowAdd, AllowDelete, Form_Tag, Parent_Handle, Window_State, StartingFromPosition, HelpID);
	return result;
}

short _DB::RecAddNew()
{
	short result;
	InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecFieldPut(LPCTSTR Field_Name, LPCTSTR Value)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Field_Name, Value);
	return result;
}

short _DB::RecEdit()
{
	short result;
	InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecDelete()
{
	short result;
	InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long _DB::DBExecSql(LPCTSTR SQL_Statement, short nFirstsysdbFlag, BOOL bIgnoreBeginCommit)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_BOOL;
	InvokeHelper(0x60030020, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		SQL_Statement, nFirstsysdbFlag, bIgnoreBeginCommit);
	return result;
}

CString _DB::RecFieldGet(LPCTSTR FieldName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030021, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		FieldName);
	return result;
}

long _DB::RecRefresh()
{
	long result;
	InvokeHelper(0x60030022, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

short _DB::RecFindLast(LPCTSTR Where_Statement)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030023, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Where_Statement);
	return result;
}

short _DB::RecFindFirst(LPCTSTR Where_Statement)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030024, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Where_Statement);
	return result;
}

short _DB::RecMoveAbsolute(short Old_Position, long New_Position)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x60030025, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Old_Position, New_Position);
	return result;
}

long _DB::DBBuildSet(short Form_Style, LPCTSTR SQL_Statement, short Max_Records, LPCTSTR UniqueFields, LPDISPATCH ActiveRecordset, LPCTSTR strFormTag)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_I2 VTS_BSTR VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0x60030026, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Form_Style, SQL_Statement, Max_Records, UniqueFields, ActiveRecordset, strFormTag);
	return result;
}

long _DB::DBInit(short DB_Type, LPCTSTR strConnectionString, LPCTSTR DB_SystemName, short ApplicationCode, short UserIDNumber, LPCTSTR HelpFileName, LPCTSTR ODBCName, LPCTSTR strProfileName)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BSTR VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030027, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DB_Type, strConnectionString, DB_SystemName, ApplicationCode, UserIDNumber, HelpFileName, ODBCName, strProfileName);
	return result;
}

short _DB::RecMovePrevious()
{
	short result;
	InvokeHelper(0x60030028, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecMoveNext()
{
	short result;
	InvokeHelper(0x60030029, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecMoveLast()
{
	short result;
	InvokeHelper(0x6003002a, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::RecMoveFirst()
{
	short result;
	InvokeHelper(0x6003002b, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short _DB::FormDoAction(short Form_Type, short Button_Index)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x6003005a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Form_Type, Button_Index);
	return result;
}

short _DB::FormMakeRefresh(short Form_Type)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003005c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Form_Type);
	return result;
}

short _DB::TabEnable(short TabIndex, BOOL State)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x6003005d, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		TabIndex, State);
	return result;
}

BOOL _DB::GetCurrentLanguage()
{
	BOOL result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _DB::SetCurrentLanguage(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short _DB::FormEnableButton(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, BOOL State, BOOL bForCurrTabOnly)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x6003005e, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Form_Type, ToolbarID, ButtonKey, State, bForCurrTabOnly);
	return result;
}

VARIANT _DB::ShowCustomizedInputBox(LPCTSTR MainTitle, LPCTSTR SubTitle, const VARIANT& ArrayOfProperties)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x6003005f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		MainTitle, SubTitle, &ArrayOfProperties);
	return result;
}

long _DB::RecCount()
{
	long result;
	InvokeHelper(0x60030060, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DB::GetBrowsingSQL(LPCTSTR TableName, LPCTSTR WhereClause)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030061, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		TableName, WhereClause);
	return result;
}

CString _DB::GetFieldCaption(LPCTSTR TableName, LPCTSTR FieldName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030062, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		TableName, FieldName);
	return result;
}

VARIANT _DB::GetBrowsingArray(LPCTSTR Table_Name, LPCTSTR Field_Name)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030063, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Table_Name, Field_Name);
	return result;
}

VARIANT _DB::GetFieldAttributes(LPCTSTR Table_Name, LPCTSTR Field_Name)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030064, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Table_Name, Field_Name);
	return result;
}

VARIANT _DB::GetTableAttributes(LPCTSTR Table_Name)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030065, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Table_Name);
	return result;
}

BOOL _DB::CreateQuery(LPCTSTR Query_Name, LPCTSTR SQLStatement)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030066, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Query_Name, SQLStatement);
	return result;
}

LPDISPATCH _DB::GetListRecordset()
{
	LPDISPATCH result;
	InvokeHelper(0x60030067, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _DB::FormFindFirst(short Form_Type, LPCTSTR WhereClause)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030068, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Form_Type, WhereClause);
	return result;
}

short _DB::SubMakeRefresh()
{
	short result;
	InvokeHelper(0x60030069, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

CString _DB::RecGetFullRecord()
{
	CString result;
	InvokeHelper(0x6003006a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _DB::FormChangeCaption(short Form_Type, LPCTSTR strCaption)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6003006b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Form_Type, strCaption);
	return result;
}

void _DB::CloseConnection()
{
	InvokeHelper(0x6003006c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT _DB::CancelAction(short Form_Type, LPCTSTR strRecords)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6003006d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		Form_Type, strRecords);
	return result;
}

VARIANT _DB::CreateListFilter(LPCTSTR strFilterCaption, LPCTSTR strFilterCriteria, LPCTSTR strIcon)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003006e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strFilterCaption, strFilterCriteria, strIcon);
	return result;
}

VARIANT _DB::SetCalendarType(BOOL bHijriState)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6003006f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		bHijriState);
	return result;
}

CString _DB::GetGregDate(LPCTSTR strHijriDate)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030070, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strHijriDate);
	return result;
}

short _DB::RecFieldAppendChunk(LPCTSTR Field_Name, const VARIANT& Value)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x60030071, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Field_Name, &Value);
	return result;
}

CString _DB::ShowFilterScreen(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BSTR;
	InvokeHelper(0x60030072, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strTableName, strCaption, strTrueTableName, bSaveFilter, strBaseCriteria);
	return result;
}

void _DB::LockScreen(short Form_Type, BOOL bLockMain, BOOL bLockSubs)
{
	static BYTE parms[] =
		VTS_I2 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030073, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Type, bLockMain, bLockSubs);
}

CString _DB::GetServerDate()
{
	CString result;
	InvokeHelper(0x60030074, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _DB::ChangeFormSQL(short Form_Type, LPCTSTR strFullSQL)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60030075, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Form_Type, strFullSQL);
	return result;
}

CString _DB::GetFormSQL(short Form_Type)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030076, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Form_Type);
	return result;
}

void _DB::RefreshMemoScreen(short Form_Type, LPCTSTR strMemoTag)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x6003007a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Type, strMemoTag);
}

short _DB::ChangeCustomButtonProperty(short Form_Type, short ToolbarID, LPCTSTR ButtonKey, LPCTSTR strPropertyName, LPCTSTR strPropertyValue)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003007b, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Form_Type, ToolbarID, ButtonKey, strPropertyName, strPropertyValue);
	return result;
}

long _DB::BringFormToTop(short Form_Type)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003007c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Form_Type);
	return result;
}

short _DB::GetFormType()
{
	short result;
	InvokeHelper(0x6003007d, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL _DB::CheckTabsIntegrity(LPCTSTR strFormTag, LPCTSTR strKeyValue, BOOL bShowMessage)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x6003007e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFormTag, strKeyValue, bShowMessage);
	return result;
}

long _DB::DBInitEx(LPCTSTR strConnectionString)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003007f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strConnectionString);
	return result;
}

CString _DB::ShowFilterScreenEx(LPCTSTR strTableName, LPCTSTR strCaption, LPCTSTR strTrueTableName, BOOL bSaveFilter, LPCTSTR strBaseCriteria, BSTR* strReadableCriteria)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BSTR VTS_PBSTR;
	InvokeHelper(0x60030080, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strTableName, strCaption, strTrueTableName, bSaveFilter, strBaseCriteria, strReadableCriteria);
	return result;
}

short _DB::TabEnableEx(LPCTSTR strSubTag, BOOL State)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030082, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strSubTag, State);
	return result;
}

long _DB::ShowLightReport(LPCTSTR Report_File, LPCTSTR Report_Title, LPCTSTR Selection_Formula, LPCTSTR Formulas, long Window_State, BOOL bExport, long Parent_Handle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BOOL VTS_I4;
	InvokeHelper(0x60030083, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Report_File, Report_Title, Selection_Formula, Formulas, Window_State, bExport, Parent_Handle);
	return result;
}

long _DB::GetScreenHWnd(short Form_Type)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030084, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Form_Type);
	return result;
}

VARIANT _DB::ShowMailSetting()
{
	VARIANT result;
	InvokeHelper(0x60030085, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _DB::SendMail()
{
	VARIANT result;
	InvokeHelper(0x60030086, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}


CString _DB::ListShowEx(LPCTSTR List_Tag, LPCTSTR List_Title, LPCTSTR SQL_Statement, LPCTSTR Return_Field, LPCTSTR Old_Value, BOOL MultiSelection, LPCTSTR strAttribTableName, long lHelpID)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BSTR VTS_I4;
	InvokeHelper(0x60030087, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		List_Tag, List_Title, SQL_Statement, Return_Field, Old_Value, MultiSelection, strAttribTableName, lHelpID);
	return result;
}

void _DB::HandleActionEx(short Form_Type, short Cancel, short Selected_Records)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x60030088, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Type, Cancel, Selected_Records);
}

BOOL _DB::ValidateEmptyFields(LPCTSTR strFormTag, LPCTSTR strKeyField, LPCTSTR strKeyValue, LPCTSTR strMessage, BOOL bShowMessage)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030089, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFormTag, strKeyField, strKeyValue, strMessage, bShowMessage);
	return result;
}

void _DB::SetProfileName(LPCTSTR strProfileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003008a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strProfileName);
}

void _DB::LockRecord(short nFormType, BOOL bMainArea, BOOL bLock)
{
	static BYTE parms[] =
		VTS_I2 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x6003008c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nFormType, bMainArea, bLock);
}

long _DB::SetWindowState(short Form_Type, long iWindowState)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x6003008e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Form_Type, iWindowState);
	return result;
}

BOOL _DB::IsErrorFlagRaised()
{
	BOOL result;
	InvokeHelper(0x6003008f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _DB::ShowAutoMailSetupScreen(long lParentHandle)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030090, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lParentHandle);
	return result;
}

long _DB::SendMailByAction(LPCTSTR strFormTag, LPCTSTR strButtonID, LPCTSTR strCriteria, LPCTSTR strTableName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030091, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFormTag, strButtonID, strCriteria, strTableName);
	return result;
}

VARIANT _DB::CreateFilter(LPCTSTR strFormTag, LPCTSTR strFilterCaption, LPCTSTR strFilterCriteria, BOOL bAsDefault)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030093, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strFormTag, strFilterCaption, strFilterCriteria, bAsDefault);
	return result;
}

void _DB::ShowMailWizard()
{
	InvokeHelper(0x60030096, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _DB::ShowMailServerSetting()
{
	InvokeHelper(0x60030097, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _DB::SelectFieldToExport(LPCTSTR strFormTag, LPCTSTR strFormCaption, LPCTSTR strTableName, LPCTSTR strCriteria, LPCTSTR strSelectedFields, LPCTSTR strRect)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60030098, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strFormTag, strFormCaption, strTableName, strCriteria, strSelectedFields, strRect);
	return result;
}

CString _DB::ExportToExcel(LPCTSTR strFormTag, LPCTSTR strFormCaption, LPCTSTR strTableName, LPCTSTR strCriteria, LPCTSTR strSelectedFields, LPCTSTR strRect, BOOL bSilent, LPCTSTR strExportedFile)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BSTR;
	InvokeHelper(0x60030099, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strFormTag, strFormCaption, strTableName, strCriteria, strSelectedFields, strRect, bSilent, strExportedFile);
	return result;
}

long _DB::ShowUserDefinedChecks()
{
	long result;
	InvokeHelper(0x6003009a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString _DB::BuildSQLFromReadableCriteria(LPCTSTR strTableName, LPCTSTR strBaseCriteria, LPCTSTR strReadableCriteria, LPCTSTR strTrueTableName)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003009b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strTableName, strBaseCriteria, strReadableCriteria, strTrueTableName);
	return result;
}

long _DB::ShowTransactionTypes()
{
	long result;
	InvokeHelper(0x6003009d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long _DB::ShowSegmentsSystem(long lParentHandle)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6003009e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lParentHandle);
	return result;
}

LPDISPATCH _DB::GetAppConnection()
{
	LPDISPATCH result;
	InvokeHelper(0x6003009f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

short _DB::FormSetFieldEx(LPCTSTR FormTag, LPCTSTR Field_Name, LPCTSTR Field_Value, short Form_Type)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x600300a0, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		FormTag, Field_Name, Field_Value, Form_Type);
	return result;
}

CString _DB::FormGetFieldEx(LPCTSTR FormTag, LPCTSTR Field_Name, short Form_Type)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x600300a1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		FormTag, Field_Name, Form_Type);
	return result;
}

void _DB::MoveForm(short Form_Type, short nLeft, short nTop, short nWidth, short nHeight)
{
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x600300df, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Type, nLeft, nTop, nWidth, nHeight);
}


/////////////////////////////////////////////////////////////////////////////
// __DB properties

/////////////////////////////////////////////////////////////////////////////
// __DB operations

void __DB::MFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Main_Position, long Details_Position, short Grid_Edit_Mode, short EditMode_Area)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Field_Name, Old_Value, New_Value, Full_Record, Main_Position, Details_Position, Grid_Edit_Mode, EditMode_Area);
}

void __DB::SFieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Record_Position, short Grid_Edit_Mode)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I2;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Field_Name, Old_Value, New_Value, Full_Record, Record_Position, Grid_Edit_Mode);
}

void __DB::SGFieldChanged(LPCTSTR Form_Tag, LPCTSTR Panel_Tag, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Record_Position)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Panel_Tag, Field_Name, Old_Value, New_Value, Full_Record, Record_Position);
}

void __DB::BrowsingAddNewItem(LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Criteria, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_DISPATCH;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Table_Name, Criteria, ListRecordset);
}

void __DB::BrowsingSelectedItem(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Return_Value, LPCTSTR Browsing_Field, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Full_Record, Table, Return_Value, Browsing_Field, Criteria, AbsolutePosition, ListRecordset);
}

void __DB::BrowsingEditItemSelected(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Full_Record, Table, Criteria, AbsolutePosition, ListRecordset);
}

void __DB::BrowsingChangePosition(LPCTSTR Form_Tag, LPCTSTR Full_Record, LPCTSTR Table, LPCTSTR Return_Value, LPCTSTR Criteria)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Full_Record, Table, Return_Value, Criteria);
}

void __DB::TreeNodeSelected(LPCTSTR Table_Name, LPCTSTR Field_Value)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Table_Name, Field_Value);
}

void __DB::UniqueConflict(LPCTSTR Form_Tag, LPCTSTR LinkName, LPCTSTR Field_Name, LPCTSTR Field_Value, short Grid_Edit_Mode)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, LinkName, Field_Name, Field_Value, Grid_Edit_Mode);
}

void __DB::CustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR Field_Caption, LPCTSTR Master_Full_Record, const VARIANT& Details_Full_Record, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_VARIANT VTS_BSTR VTS_BSTR VTS_I4 VTS_DISPATCH;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Field_Name, Field_Caption, Master_Full_Record, &Details_Full_Record, Table_Name, Criteria, AbsolutePosition, ListRecordset);
}

void __DB::ToolBarButtonClicked(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID, LPCTSTR strSelectedTabTag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2 VTS_VARIANT VTS_I2 VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button_Name, Form_Tag, Seleted_Tab, &Full_Record, Toolbar_ID, strSelectedTabTag);
}

void __DB::ToolBarButtonClickedDone(LPCTSTR Button_Name, LPCTSTR Form_Tag, short Seleted_Tab, const VARIANT& Full_Record, short Toolbar_ID, LPCTSTR strSelectedTabTag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I2 VTS_VARIANT VTS_I2 VTS_BSTR;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Button_Name, Form_Tag, Seleted_Tab, &Full_Record, Toolbar_ID, strSelectedTabTag);
}

void __DB::MainSubReposition(LPCTSTR Form_Tag, long Main_Position, short Seleted_Tab, LPCTSTR strSelectedTabTag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I2 VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Main_Position, Seleted_Tab, strSelectedTabTag);
}

void __DB::GridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record, LPCTSTR strSelectedTabTag)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I2 VTS_I4 VTS_BSTR VTS_BSTR;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Selected_Tab, AbsolutePosition, Full_Record, strSelectedTabTag);
}

void __DB::BrowsableCustomButtonClicked(LPCTSTR Form_Tag, LPCTSTR Table, const VARIANT& Record, short CustomButtonID, LPCTSTR Criteria, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT VTS_I2 VTS_BSTR VTS_DISPATCH;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Table, &Record, CustomButtonID, Criteria, ListRecordset);
}

void __DB::BeforeBrowseButtonClick(LPCTSTR Form_Tag, LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTag, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, Link_Tag, FieldName, CellContent, BrowsingTag, BrowsingSQL, BrowsingField, BrowsingRetField, Full_Record);
}

void __DB::BrowsingToolBarButtonClicked(LPCTSTR List_Tag, LPCTSTR Button_Key, LPCTSTR Table_Name, LPCTSTR Criteria, long AbsolutePosition, const VARIANT& Record, LPDISPATCH ListRecordset, BOOL bCustomButton)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_VARIANT VTS_DISPATCH VTS_BOOL;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 List_Tag, Button_Key, Table_Name, Criteria, AbsolutePosition, &Record, ListRecordset, bCustomButton);
}

void __DB::KeyCheck(BOOL Dongle_Exist)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Dongle_Exist);
}

void __DB::ScreenReady(LPCTSTR Form_Tag)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag);
}

void __DB::ListCommonDone(LPCTSTR List_Tag, const VARIANT& vAffectedRecords)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 List_Tag, &vAffectedRecords);
}

void __DB::BeforeBrowsing(LPCTSTR strFormTag, LPCTSTR strFieldName, BSTR* strBrowsingField, BSTR* strBrowsingRetField, BSTR* strBrowsingSQL)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strFormTag, strFieldName, strBrowsingField, strBrowsingRetField, strBrowsingSQL);
}

void __DB::BrowseDuplicatedValue(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strBrowsingTable, LPCTSTR strBrowsingRetField, LPCTSTR strBrowsingValue, BOOL* bShowList)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_PBOOL;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strFormTag, strFieldName, strBrowsingTable, strBrowsingRetField, strBrowsingValue, bShowList);
}

void __DB::FieldChangeDone(LPCTSTR Form_Tag, LPCTSTR TableName, LPCTSTR Field_Name, LPCTSTR Old_Value, LPCTSTR New_Value, LPCTSTR Full_Record, long Main_Position, long Details_Position, short Grid_Edit_Mode, short EditMode_Area)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, TableName, Field_Name, Old_Value, New_Value, Full_Record, Main_Position, Details_Position, Grid_Edit_Mode, EditMode_Area);
}

void __DB::BeforeSendMail(LPCTSTR strFormTag, LPCTSTR strButtonID, const VARIANT& vRecordsArray, LPCTSTR strTableName, LPCTSTR strKeyField, BSTR* strOtherInfo)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_VARIANT VTS_BSTR VTS_BSTR VTS_PBSTR;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strFormTag, strButtonID, &vRecordsArray, strTableName, strKeyField, strOtherInfo);
}

void __DB::SendReport(BSTR* strTo, BSTR* strCc, BSTR* strSubject, BSTR* strBody)
{
	static BYTE parms[] =
		VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strTo, strCc, strSubject, strBody);
}

void __DB::ReletedRecordSelected(LPCTSTR strFormTag, LPCTSTR strTableName, LPCTSTR strCriteria)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strFormTag, strTableName, strCriteria);
}

long _DB::FindRecord(LPCTSTR strCriteria, short nFormType)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0x600300e2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strCriteria, nFormType);
	return result;
}
long _DB::GetTXNDependencyInfo(LPCTSTR strFormTag, LPCTSTR strFieldName, LPCTSTR strFieldValue)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6003012c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strFormTag, strFieldName, strFieldValue);
	return result;
}

long _DB::ShowAutomaticEmails(long lParentHandle)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030177, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lParentHandle);
	return result;
}


CString _DB::GetScreenCaption(LPCTSTR strScreenTag)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x600300e9, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strScreenTag);
	return result;
}

short _DB::SetActiveTab(LPCTSTR strTabTag)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003012f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strTabTag);
	return result;
}

VARIANT _DB::CancelUploadImage(LPCTSTR strFieldName)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6003017d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strFieldName);
	return result;
}

void __DB::BrowsingAddNewItemEx(LPCTSTR Form_Tag, LPCTSTR BrowseForm_Tag, LPCTSTR Table_Name, LPCTSTR Criteria, LPDISPATCH ListRecordset)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_DISPATCH;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Form_Tag, BrowseForm_Tag, Table_Name, Criteria, ListRecordset);
}

void __DB::CreateLikeInvoked(LPCTSTR strFormTag, LPCTSTR strTableName, LPCTSTR strKeyFieldValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strFormTag, strTableName, strKeyFieldValue);
}

CString _DB::GetVersion()
{
	CString result;
	InvokeHelper(0x60030180, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _DB::ValidateTabEmptyFields(LPCTSTR strFormTag, LPCTSTR strKeyField, LPCTSTR strKeyValue, LPCTSTR strTabFormTag, LPCTSTR strTabKeyField, LPCTSTR strTabKeyValue, LPCTSTR strMessage, BOOL bShowMessage)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030181, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFormTag, strKeyField, strKeyValue, strTabFormTag, strTabKeyField, strTabKeyValue, strMessage, bShowMessage);
	return result;
}

