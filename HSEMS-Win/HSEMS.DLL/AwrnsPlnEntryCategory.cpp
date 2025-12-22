#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnEntryCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AwrnsPlnEntryCategory::AwrnsPlnEntryCategory(CCategory *pCategory):AwrnsPlnCategory(pCategory) {
	m_pCategory = pCategory;
}

AwrnsPlnEntryCategory::~AwrnsPlnEntryCategory() {}

long AwrnsPlnEntryCategory::ShowAwrnsPlnEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AwrnsPlnEntry;
	return ParentManage("HSE_TgAwrnsPlnEntry",TRUE,TRUE,TRUE,"HSE_AwrnsPln",strCriteria);
}

HRESULT AwrnsPlnEntryCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strSubFormTag = pBrowsingSelectedItem->SubForm_Tag;
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;
	CString strReturnValue = pBrowsingSelectedItem->Return_Value;
	if(strBrowsingField=="AWRNSPLN_CRS_PLNDLOC")
	{		
		FormSetField("HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_ACTLLOC",strReturnValue);
	}
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}

HRESULT AwrnsPlnEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strButtonName=="AWRNSPLNENTRY_PLNENTRYCMPLTD") {
		//////////////////////////////////////////////////////////////////////////////////////////////
		//Bugs-10-2021.11
		//CString strAwrnsPlnYr = FormGetField("HSE_AWRNSPLN","AWRNSPLN_YR");
		CString strAwrnsSrl = FormGetField("HSE_AWRNSPLN","SerialPK");
		completeEntry(strAwrnsSrl);			
		//////////////////////////////////////////////////////////////////////////////////////////////
		
	}
	return AwrnsPlnCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void AwrnsPlnEntryCategory::completeEntry(CString strAwrnsPlnYr) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE AwrnsPlnEntryCompleted %s",strAwrnsPlnYr);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}
