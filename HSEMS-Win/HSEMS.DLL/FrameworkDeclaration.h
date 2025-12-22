#define   RC_FILE			0
#define   RUNTIME_FILE		1
#define   BINARY_FILE		2
#define   DATA_FILE			3

#define   REFRESH_SCREEN	0
#define   REFRESH_SELECTED	1
#define   REFRESH_CLEAR		2

#define TOOLBAR_COUNT			22
#define TOOLBAR_NEW				1
#define TOOLBAR_SAVE			2
#define TOOLBAR_CANCEL			3	
#define TOOLBAR_DELETE			4
#define TOOLBAR_PRINT			5
#define TOOLBAR_MOVEFIRST		6
#define TOOLBAR_MOVEPREV		7
#define TOOLBAR_MOVENEXT		8
#define TOOLBAR_MOVELAST		9
#define TOOLBAR_FIND			10
#define TOOLBAR_UNFILTER		11
#define TOOLBAR_FILTER			23
#define TOOLBAR_COMMON			12
#define TOOLBAR_STOP			13
#define TOOLBAR_CHANGECOLOR		14
#define TOOLBAR_EXIT			15
#define TOOLBAR_SELECT			16
#define TOOLBAR_SHOW_DETAILS	17
#define TOOLBAR_FILTERCURRENT	18	
#define TOOLBAR_SORT			20
#define TOOLBAR_COPY			21
#define TOOLBAR_PASTE			22
///////////////////////////////////////////////


typedef struct  tag_OpenFile
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR Field_Name;
    BSTR Full_Record;
    BSTR bstrFilter;
    LONG ButtonClicked;
    LONG AddNew_Mode;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    BSTR Table_Name;
    long lOwnerApp;
    long lOwnerTransAction;
    }	OPENFILE;

typedef struct  tag_FieldChange
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR Field_Name;
    BSTR Old_Value;
    BSTR New_Value;
    BSTR Full_Record;
    LONG AbsolutePosition;
    LONG AddNew_Mode;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    BSTR Table_Name;
    long lOwnerApp;
    long lOwnerTransAction;
    }	FIELDCHANGE;

typedef struct  tag_SelectedRecArray
    {
    BSTR Full_Record;
    BOOL bSelectedRec;
    }	SELECTEDRECARRAY;

typedef struct  tag_SelectedRec
    {
    LONG nRecordsCount;
    SELECTEDRECARRAY __RPC_FAR *pSelectedRecArray;
    }	SELECTEDREC;

typedef struct  tagSelectedRow
    {
    LONG iCancel;
    BSTR strFullRecord;
    }	SelectedRow;

typedef struct  tagMultiSelectedRows
    {
    long lCount;
    SelectedRow __RPC_FAR *pRow;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	MultiSelectedRows;

typedef struct  tag_BrowsingSelectedItem
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR BrowsingTable;
    BSTR BrowsingField;
    BSTR Return_Value;
    BSTR Full_Record;
    MultiSelectedRows __RPC_FAR *pMultiSelectedRows;
    BSTR Criteria;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	BROWSINGSELECTEDITEM;

typedef struct  tag_CustomButtonClicked
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
	//a.ali
	BSTR Table_Name;
    BSTR Button_Name;
    BSTR Full_Record;
    long lRecordCount;
    int nRecordStatus;
    MultiSelectedRows __RPC_FAR *pMultiSelectedRows;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
	long AbsolutePosition;
    }	CUSTOMBUTTONCLICKED;

typedef struct  tag_ToolBarButtonClicked
    {
    BSTR Button_Name;
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR Table_Name;
    BSTR Full_Record;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    MultiSelectedRows __RPC_FAR *pMultiSelectedRows;
    long lOwnerApp;
    long lOwnerTransAction;
    int nRecordStatus;
    }	TOOLBARBUTTONCLICKED;

typedef struct  tag_RecordRepostion
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR Full_Record;
    LONG Position;
    LONG iActiveView;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	RECORDREPOSTION;

typedef struct  tag_BROWSINGINFO
    {
    BSTR BrowsingTable;
    BSTR MainCriteria;
    BSTR BrowsingSQL;
    BSTR FilterSQL;
    BSTR BrowsingField;
    BSTR BrowsingRetField;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	BROWSINGINFO;

typedef struct  tag_BeforBrowsingButtonClick
    {
    BSTR Browse_Form_Tag;
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    BSTR Field_Name;
    BSTR Current_Value;
    BSTR Full_Record;
    LONG iNavigate;
    BROWSINGINFO __RPC_FAR *pBrowsingInfo;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    int iScreenMode;
    LONG __RPC_FAR *iCancle;
    }	BEFORBROWSINGBUTTONCLICK;

typedef struct  tag_ScreenReady
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    int nScreenType;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    int nBrowseMode;
    int iScreenMode;
    }	SCREENREADY;

typedef struct  tag_TabSelected
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    LONG iActiveView;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    LONG lPrevTabNumber;
    LONG lTabNumber;
    long lOwnerApp;
    long lOwnerTransAction;
    }	TABSELECTED;

typedef struct  tag_ListEditNavigation
    {
    BSTR Form_Tag;
    BSTR SubForm_Tag;
    LONG iActiveView;
    LONG iComplete;
    LONG __RPC_FAR *iCancle;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	LISTEDITNAVIGATION;

typedef struct  tag_KeyPress
    {
    INT message;
    INT wParam;
    LONG lParam;
    LONG lCategoryID;
    long lOwnerApp;
    long lOwnerTransAction;
    }	KEYPRESS;

typedef struct tagDisplayFieldAttribInfo
{
	int  ID;
	int  iLength;
	int  AppCode;
	BSTR strArCaption;
	BSTR strEnCaption;
	BSTR BrowsingTag;
	int  MaxLength;
	int  FieldIndex;
	int  FieldStyle;
	int  DataType;
	int  Attribute[8];
	int  DataMask;
	long HelpId;
	BSTR TableName;
	BSTR FieldName;
	BSTR ValidationRule;
	BSTR HelpString;
	BSTR BrowsingTable;
	BSTR BrowsingField;
	BSTR BrowsingRetField;
	BSTR BrowsingSQL;
	BSTR strAliasName;
	BSTR strServerFolder;
	BSTR strClass;
	BSTR strClassValues;
	BSTR strTagName;
	BSTR strDefaultValue;
	BSTR strUniqueGroup;
	BOOL bDisableCopy;
	BSTR strMaskFormat;
}DISPLAYFIELDATTRIBINFO;