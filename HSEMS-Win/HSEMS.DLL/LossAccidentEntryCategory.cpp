// LossAccidentEntryCategory.cpp: implementation of the LossAccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "LossAccidentEntryCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LossAccidentEntryCategory::LossAccidentEntryCategory(CCategory *pCategory):LossAccidentCategory(pCategory)
{
	m_pCategory = pCategory;
}

LossAccidentEntryCategory::~LossAccidentEntryCategory()
{

}
/*************************************************************************************************************************/
HRESULT LossAccidentEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	//Commented by Hossam "Variables not used"
	//CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	if(strButtonName=="LOSSACDNTENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		//203_330_2	
		CompleteLossAccident(pCustomButtonClicked);
	}
	return LossAccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************************/
HRESULT LossAccidentEntryCategory::DisplayFieldChange(FIELDCHANGE *pFieldChange)
{
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	/*if(strSubFormTag=="" && strFieldName=="LOSSACDNTENT_HNDLPTNLREC")
	//203_330_1
	//"	If (Has Potential Record checkbox [1]) = checked, 
	//then enable the (Potential Hazard No [3]) field, else disable the (Potential Hazard No [3]) field
	{
 		CString strhasPtntlHzrdRcrd=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_HNDLPTNLREC");
		if(strhasPtntlHzrdRcrd=="0")
		{		
			SetFieldVisibleAttrib("HSE_TGLOSSACDNTENT","LOSSACDNTENT_PTNLHZRDNO","ALWAYSLOCKED");
		}
		else 
		{
			//SetFieldVisibleAttrib("HSE_TGLOSSACDNTENT","LOSSACDNTENT_PTNLHZRDNO","NULL");
		}
	}*/

	//Added by a.ali
	//date 04-09-2012
	//CR09 : The (site type) data should be driven & displayed online after selecting the site, but not at record saving
	if(strFieldName == "LOSSACDNTENT_SIT"){
		retrievSiteType();
	}

	if(strSubFormTag=="HSE_TGLOSSACDNTEMRGNCY" && strFieldName=="LOSSACDNTEMRGNCY_COMPEMP")
	{
		
		selectEmployeeType(pFieldChange,"HSE_TGLOSSACDNTEMRGNCY");

	}

	if(strSubFormTag=="HSE_TGLOSSACDNTEMRGNCY" && strFieldName=="LOSSACDNTEMRGNCY_COMPEMP")
	{
		//101_302_1
		//Change Field Value
		//Allow user either to feed the company employee information [4], [5], [6], [7] 
		//or non-company employee information [9], [10], [11], [12] but not bot
		EmptyEmployeeFields();
	}
	return LossAccidentCategory::DisplayFieldChange(pFieldChange);
}
/*************************************************************************************************************************/
//203_330_1
//same requirement but check in new mode 
HRESULT LossAccidentEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{

	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);

	//a.ali code inside condition is commented no need for condition
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="NEW")
	//{	
		/*CString strhasPtntlHzrdRcrd=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_HNDLPTNLREC");
		if(strhasPtntlHzrdRcrd=="0")
		{		
			SetFieldVisibleAttrib("HSE_TGLOSSACDNTENT","LOSSACDNTENT_PTNLHZRDNO","ALWAYSLOCKED");
		}
		else 
		{
			SetFieldVisibleAttrib("HSE_TGLOSSACDNTENT","LOSSACDNTENT_PTNLHZRDNO","NULL");
	
		}*/
	//}

	//if(strBtnName=="CANCLE"&& CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGLOSSACDNTEMRGNCY" && pToolBarButtonClicked->iComplete == 0)
	if(strBtnName=="CANCEL"&& CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGLOSSACDNTEMRGNCY" && pToolBarButtonClicked->iComplete == 0)
	{
		SetFieldVisibleAttrib("HSE_TGLOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_EMPNUM","NULL");
	}

	//a.ali code inside condition is commented no need for condition
	//if(strBtnName=="SAVE"&& CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGLOSSACDNTEMRGNCY" && pToolBarButtonClicked->iComplete == 1)
	//{
		//RefreshScreen("HSE_TGLOSSACDNTEMRGNCY",REFRESH_SELECTED);
	//}

	return LossAccidentCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/*************************************************************************************************************************/
HRESULT LossAccidentEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();


	// 203_330_3

	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="LOSSACDNTENT_LOCTYP")
	{

		CString strLocTyp=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	
	// 203_330_4

	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="LOSSACDNTENT_WRKLOC")
	{

		CString strLocTyp=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_LOCTYP");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}

	else if(strFieldName=="LOSSACDNTENT_HZRDCTG")
	{
		CString strHZRDGrp=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_HZRDGRP");
		if(strHZRDGrp.IsEmpty())
			strHZRDGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK ='%s')",strHZRDGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}
	else if(strFieldName=="LOSSACDNTENT_HRMTYP")
	{
		CString strHZRDCTG = FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_HZRDCTG");
		if(strHZRDCTG.IsEmpty())
			strHZRDCTG=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strHZRDCTG);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}
	return LossAccidentCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);

}
/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void LossAccidentEntryCategory::EmptyEmployeeFields()
{
	//203_303205_1
	//Change Field Value
	//Allow user either to feed the company employee information [4], [5], [6], [7] 
	//or non-company employee information [9], [10], [11], [12] but not both

	CString strCompEmp=FormGetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_COMPEMP");
	CString strEmp=FormGetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_EMPNUM");
	if(strCompEmp=="0")
	{		
		FormSetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_EMPNUM"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","{5}"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","{6}"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","{7}"," ");
	}

	if(strCompEmp=="1")
	{		
		FormSetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_NAM"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_COMP"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_DPRT"," ");
		FormSetField("HSE_LOSSACDNTEMRGNCY","LOSSACDNTEMRGNCY_JOBTTL"," ");
	}
}
/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void LossAccidentEntryCategory::CompleteLossAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//If (HSE policy, Loss Accident Confirmation checkbox [33]) = "un-checked"
	bool bIslossAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_ACTHZRDCNFRMTNRQRD");
	if(bIslossAccdntCnfrmtnRqrd==false)
	{
		//Set (Record Status [41]) = 5
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
		InsertActionRecordIntoTracingTab("LOSSACDNTENT","ACTHZRDNO",CONFIRM);
	}
	else
	{	
		//"Set (Record Status [41]) = 4
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
		InsertActionRecordIntoTracingTab("LOSSACDNTENT","ACTHZRDNO",COMPLETE);
	}
}

void LossAccidentEntryCategory::retrievSiteType()
{
	CString strSiteCode=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_SIT");
	if(strSiteCode != "")
	{
		CString strSQL;
		strSQL.Format("select SITETYPE_DES from HSE_SITETYPE where SITETYPE_CODE in (select STE_SITETYPE FROM HSE_STE WHERE MAINLINK = '%s')",strSiteCode);
		CString strSiteType=GetRecordSetData(strSQL,1);
		FormSetField("HSE_LOSSACDNTENT","{95@330@66}",strSiteType);
	}
		
}

long LossAccidentEntryCategory::ShowLossAccidentEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LOSSACDNTENT;
	return ParentManage("HSE_TGLOSSACDNTENT",TRUE,TRUE,TRUE,"HSE_LOSSACDNTENT",strCriteria);
}