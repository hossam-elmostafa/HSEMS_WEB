// CommonCategoryWrapper.h: interface for the CCommonCategoryWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONCATEGORYWRAPPER_H__B60B3A15_75E8_42D5_827F_390AC622F3D3__INCLUDED_)
#define AFX_COMMONCATEGORYWRAPPER_H__B60B3A15_75E8_42D5_827F_390AC622F3D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FrameworkDeclaration.h"

class CCommonCategoryWrapper : public CCommonCategory  
{
public:
	CCommonCategoryWrapper(CCategory *pCategory):CCommonCategory(pCategory)
	{
		m_pCategory     = pCategory;
		m_strSlctdTab	= "";
		m_strDefaultValueScreenTag = "";
		m_bEnableMainScreen=true;
	};

	~CCommonCategoryWrapper(){};

	///// framework events
public:
	virtual HRESULT DisplayScreenReady(tag_ScreenReady *pScreenReady){return S_OK;};
	virtual HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked){return S_OK;};
	virtual HRESULT DisplayFieldChange (tag_FieldChange *pFieldChange){return S_OK;};
	virtual HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick){return S_OK;};
	virtual HRESULT DisplayToolBarButtonClicked(tag_ToolBarButtonClicked *pToolBarButtonClicked){return S_OK;};
	virtual HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion){return S_OK;};
	virtual HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem  *pBrowsingSelectedItem){return S_OK;};
	//a.ali
	virtual HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected){return S_OK;};

	void SetEnableMainScreen(BOOL bAllowUpdate,BOOL bAllowAddNew=FALSE,BOOL bAllowDelete=FALSE){m_bAllowUpdate=bAllowUpdate;m_bAllowAddNew=bAllowAddNew;m_bAllowDelete=bAllowDelete;}

	/// polydb events
protected:
	void OnBeforeListReady(LPCTSTR FormTag);
	void OnScreenReady(LPCTSTR FormTag);
	void UpdateAfterBrowse(LPCTSTR Form_Tag, LPCTSTR Full_Recoed, LPCTSTR Table, LPCTSTR Return_Value,LPCTSTR Browsing_Field, LPCTSTR Criteria);
	void ConfirmToolBarButtonClicked(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubFormTag);
	virtual void ConfirmToolBarButtonClickedDone(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubForm_Tag);
	void FieldChanged(LPCTSTR Form_Tag, LPCTSTR Field_Name, LPCTSTR old_value, LPCTSTR New_Value, LPCTSTR full_record, long Main_Position, long Details_Position, short grid_edit_mode);
	void ButtonClicked(LPCTSTR Caption, LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Master_Full_Recoed,LPCTSTR Details_Full_Recoe,LPCTSTR Field_Caption,long AbsolutePosition);
    void MainSubReposition(CString strFormTag, long Main_Position, short Seleted_Tab);
	void GridRowReposition(LPCTSTR Form_Tag, short Selected_Tab, long AbsolutePosition, LPCTSTR Full_Record,LPCTSTR subForm_Tag);
	void OnBrowsableCustomButtonClicked(LPCTSTR Table, const VARIANT& Record, short CustomButtonID);
	void BeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record);

	// helper function
public:
	CString			GetCurrentYear();
	CString			GetConectionString();
	CString			GetFieldValue(CString strFullRecord, CString strFieldName);
	CString			FormatDate(CString strDate);
	CString			FormatDate(COleDateTime dtDate);
	CString			GetRecordSetData(CString strSQL,long lRecordsCount=1);
	CString			GetFullDateTime(CString strFormat);
	CString			GetServerDate_STRING(CString strFormat);
	CString			GetServerTime();
	CString			GetSetting( CString strKey, CString strSubKey, CString strValueName,CString strDefaultData="",CString strProfile = "HSEMS");
	CString			itoa(long iValue);
	CString			GetUserLogin();
	COleDateTime	GetServerDate_DATE();
	bool			ParseDateTime(COleDateTime *pdtDate,CString strDate);
	bool			DoToolBarAction(long lButtonKey,CString strScreenTag="",CString strSubFormTag="");
	void			SetFieldAttrib(DISPLAYFIELDATTRIBINFO *pFieldAttrib);
	void			RefreshScreen(CString strSubFormTag,int nRefreshMode=0);
	void			LockRecord(CString strMainFormTag,CString strScreenTag,bool bLocked);
	void			EnableTab(CString strTabTag,BOOL bStatus);
	void			EnableToolbar(CString strScreenTag,CString strSubFormTag,long lButtonID,bool bEnabled);
	void			EnableToolbar(CString strSubFormTag,long lButtonID,bool bEnabled);
	void			SetScreenCaption(CString strTag,CString strCaption);
	void			ShowList(UINT unMenuId,CString strSQL="",long lParam=0,CString strDefValues="");
	int				ShowScreen(int nCurrentCard,CString strTableName,CString strTitle,int nStatus,BOOL bModal,CString strCriteria="",CString strComesFrom="",CString strScreenDefauls="",CString strReturnBrowseValue="",bool blocked=false,void* Caller=NULL,int nScreenID=-1,CCommonCategoryWrapper **pCommonCategoryWrapper=NULL);
	
	void			SaveSetting(CString strKey, CString strSubKey, CString strValueName, CString strData, CString strProfile = "HSEMS");
	void			ChangeCriteria(CString strTag,CString strSubTag,CString strCriteria);
	HRESULT			ShowListScreen(CString strList_Tag,CString strList_Title,CString strSQL_Statement,CString strKeyField,CString strRecKeyValue,BOOL bMultiSelection,tagMultiSelectedRows* pstrFieldsValues,CString strScreenAttribute="");
	HRESULT			SetValidationRule(CString strFormTag,CString strFieldName,CString ValidationRule);
	long			ShowMessage(CString strMessage	, CString strButtons = "OK", int IconVal = 3, CString strTitle = "Health, Safety and Environment Management System (HSEMS®)");
	long			ShowMessageEx(UINT nMessageID, CString strButtons = "OK", int IconVal = 3, CString strTitle = "Health, Safety and Environment Management System (HSEMS®)");
	long			GetCharCount(CString strString,CString strSubString);
	double			GetServerDate_DOUBLE();
	CString			GetTabTag(CString strForm_Tag,int nTabIndex);
	CString			GetTabTag(CString strForm_Tag,CString strTabTableName);
	CString			GetButtonName(long lButtonID);
	long			GetButtonID(long lButtonID);
	CString			GetTrueTableName(int nAppCode,CString strFormTag,CString strSubFormTag);
	CString			GetScrCptnByTag(int nAppCode,CString strFormTag,CString strSubFormTag);
	bool			IsFieldExist(int nAppCode,CString strFormTag,CString strSubFormTag,CString strFieldName);

	void			EnableTab(CString strScreenTag,CString strSubFormTag,bool bEnabled);


	void			InitDefaultValue(int nScreenCount);
	void			InitScreenDefaultValue(int nScreenIndex,CString strScreenTag,int nFieldsCount);
	void			AddFieldDefaultValue(int nScreenIndex,int nFieldIndex,CString strFieldName,CString strDefaultValue);
	void			SetDefaultValue(CString strFormTag,CString strFieldName,CString strFieldValue);
	void			SetFieldVisibleAttrib(CString strScreenTag,CString strFieldName,CString strAttribute);
	void			UpdateValidation(CString strApp,CString strTag,CString strLinkName,CString strField,CString strValidation,CString strUpdatedFieldName);
	void			UpdateTabAttrib(CString strApp,CString strTag,CString strLinkName,CString strField,CString strUpdatedFieldVal);
	void			UpdateEnableMainScreen(bool bEnableMainScreen) {m_bEnableMainScreen=bEnableMainScreen;};
protected:
	CString			m_strScreenTag;
	CString			m_strSlctdTab;
	CString			m_strDefaultValueScreenTag;
	bool			m_bEnableMainScreen;
};

#endif // !defined(AFX_COMMONCATEGORYWRAPPER_H__B60B3A15_75E8_42D5_827F_390AC622F3D3__INCLUDED_)
