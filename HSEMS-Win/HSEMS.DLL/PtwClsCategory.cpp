// PtwClsCategory.cpp: implementation of the PtwClsCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PtwClsCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PtwClsCategory::PtwClsCategory(CCategory *pCategory):PtwCategory(pCategory)
{
	m_pCategory = pCategory;
}

PtwClsCategory::~PtwClsCategory()
{

}


HRESULT PtwClsCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strSubScrnTag(pCustomButtonClicked->SubForm_Tag);
	//RQ-5-2021_1
	if(strButtonName=="PTWSRGSTRENT_CLSPTW") {
		closePTW(pCustomButtonClicked);
	} 
	return PtwCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long PtwClsCategory::ShowPTWsRgstrCls(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTWSRGSTRENT;	
	return ParentManage("HSE_TGPTWSRGSTRCLS",TRUE,FALSE,FALSE,"HSE_PTWSRGSTRENT",strCriteria);
}

//RQ-5-2021_1
void PtwClsCategory::closePTW(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE closePTW '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}