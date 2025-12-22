// PotentialHazardEntryCategory.cpp: implementation of the PotentialHazardEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PotentialHazardEntryCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PotentialHazardEntryCategory::PotentialHazardEntryCategory(CCategory *pCategory):PotentialHazardCategory(pCategory)
{
	m_pCategory = pCategory;
}

PotentialHazardEntryCategory::~PotentialHazardEntryCategory()
{

}
//****************************************************************************
// FrameWork EVENTS 
//**************************************************************************
//201_320_1
HRESULT PotentialHazardEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	
	CString strSQL	=	"";
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strButtonName(pCustomButtonClicked->Button_Name);
	strButtonName.MakeUpper();
	//If (Potential Hazard Confirmation checkbox [8@600]) = "un-checked", then
	if(strButtonName=="PTNLHZRDENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompletePotential(pCustomButtonClicked);
	}
	return PotentialHazardCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

//***************************************************************************************
BOOL PotentialHazardEntryCategory::CalcRiskRank()
{
	CString strSVRTYRAT=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_SVRTYRAT");
	CString strLKLYHODRAT=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_LKLYHODRAT");
	if(strLKLYHODRAT != "" && strSVRTYRAT != "")
	{
		CString strSQL="Select RSKSVRTYRAT_RAT From HSE_RSKSVRTYRAT Where MAINLINK="+strSVRTYRAT;
		CString strSVRTYRATVal=GetRecordSetData(strSQL,1);

		int nReskRank = atoi(strLKLYHODRAT)*atoi(strSVRTYRATVal);
		CString strReskRank;
		strReskRank.Format("%d",nReskRank);
		FormSetField("HSE_PTNLHZRDENT","PTNLHZRDENT_RSKRNK",strReskRank);
	}
	else
	{
		FormSetField("HSE_PTNLHZRDENT","PTNLHZRDENT_RSKRNK","");
		FormSetField("HSE_PTNLHZRDENT","PTNLHZRDENT_RSKRNKDESC","");
		
	}
	return TRUE;
}
HRESULT PotentialHazardEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName(pFieldChange->Field_Name);
	//New Requirement --N--
	//by Mr Sameer 8-11-2009
	//Retrive risk rank description from risk rank screen 
	if((strFieldName=="PTNLHZRDENT_SVRTYRAT" || strFieldName=="PTNLHZRDENT_LKLYHODRAT") && pFieldChange->iComplete == 1)
	{
		if(!CalcRiskRank())
		{
			*pFieldChange->iCancle = 1;
			return S_FALSE;
		}

	}
	if(strFieldName=="PTNLHZRDENT_RSKRNK")
	{
		retrievRiskRankDesc(pFieldChange);
	}
	if(strFieldName=="PTNLHZRDENT_SIT")
	{
		retrievSiteType();
		
	}
	return PotentialHazardCategory::DisplayFieldChange (pFieldChange);
}
//**************************************************************************************  P

HRESULT PotentialHazardEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)

{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	//201_320_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="PTNLHZRDENT_LOCTYP")
	{
		CString strLocTyp=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//201_320_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="PTNLHZRDENT_WRKLOC")
	{
		CString strLocTyp=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_LOCTYP");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	else if(strFieldName=="PTNLHZRDENT_HZRDCTG")
	{
		CString strHZRDGrp=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_HZRDGRP");
		if(strHZRDGrp.IsEmpty())
			strHZRDGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK ='%s')",strHZRDGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	else if(strFieldName=="PTNLHZRDENT_HRMTYP")
	{
		CString strHzrdCTG=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_HZRDCTG");
		if(strHzrdCTG.IsEmpty())
			strHzrdCTG=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strHzrdCTG);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}

	else if(strFieldName=="PTNLHZRDENT_SVRTYRAT")
	{
		CString strSvrtRatSQL = GetSevirityRatingCriteria().AllocSysString();
		if(strSvrtRatSQL.IsEmpty()) {
			strSvrtRatSQL = " ";
		}
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL = strSvrtRatSQL.AllocSysString(); 		
	}
	return PotentialHazardCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
//*********************************************************************************************************

HRESULT PotentialHazardEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);	
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	/*According to the Change request (HSE_CR12v2_11),
	 *For all HSE transactions except Performance Measurement & AUDIT transactions.
	 *RuleID: 000_7 General Specs
	 *Calculate TXN Number and Year
	 *Done By:Mai Talaat*/
	 if(strSubFormTag == "") {
		 if(strBtnName == "NEW" && pToolBarButtonClicked->iComplete == 1) {
			GenerateTXNNo(strFormTag);
		 }
	 }
	//a.ali if this part of condition "pToolBarButtonClicked->iComplete == 1" exist 
	//function in parent class "DisplayToolBarButtonClicked on PotentialHazardCategory" will not reached
	//if(strBtnName=="SAVE" && pToolBarButtonClicked->iComplete == 1)
	 if(strBtnName=="SAVE") {
		return PotentialHazardCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
	 }
}
//***************************************************************************************************
// Helper Functions
//***************************************************************************************************
CString PotentialHazardEntryCategory::GetSevirityRatingCriteria()
{
	CString strSQL;
	CString strSevrityClass=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_SVRTRATCLS");
	if(strSevrityClass.IsEmpty())
		strSevrityClass =" ";
	strSQL.Format("WHERE (LINK ='%s')",strSevrityClass);
	return strSQL;
}
//***************************************************************************************************
// Helper Functions
//***************************************************************************************************
void PotentialHazardEntryCategory::CompletePotential(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	bool bIsHzrdCnfrmnRqrd=CheckPolicy("HSEPLC_PTNLHZRDCNFRMTNRQRD");
	ACTION RecStatus=APPROVE;
	if(bIsHzrdCnfrmnRqrd==false)
	{
		//Set (Record Status [41]) = 5 
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,5,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
		//Insert record into (tracing [@32020]) tab
		//RecStatus=APPROVE;
		InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",RecStatus);
	}	
	else
	{
		//Set (Record Status [41]) = 4
		UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,4,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
		//Insert record into (tracing [@32020]) tab
		RecStatus=COMPLETE;		
		InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",RecStatus);
	}
}
//***************************************************************************************************
// Helper Functions
//***************************************************************************************************
void PotentialHazardEntryCategory::retrievRiskRankDesc(FIELDCHANGE *pFieldChange)
{
	CString strRskRnk=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_RSKRNK");
	if(strRskRnk != "")
	{
		CString strSQL="Select RSKRNKDESC_DESC From HSE_RSKRNKDESC Where RSKRNKDESC_RNK="+strRskRnk;
		CString strRiskRankDesc=GetRecordSetData(strSQL,1);
		FormSetField("HSE_PTNLHZRDENT","PTNLHZRDENT_RSKRNKDESC",strRiskRankDesc);
	}
}
void PotentialHazardEntryCategory::retrievSiteType()
{
	CString strSiteCode=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_SIT");
	if(strSiteCode != "")
	{
		CString strSQL;
		strSQL.Format("select SITETYPE_DES from HSE_SITETYPE where SITETYPE_CODE in (select STE_SITETYPE FROM HSE_STE WHERE MAINLINK = '%s')",strSiteCode);
		CString strSiteType=GetRecordSetData(strSQL,1);
		//polydb_task 
		//FormSetField("HSE_PTNLHZRDENT","{95@320@66}",strSiteType);//95@320@66
		FormSetField("HSE_PTNLHZRDENT","STE_SiteType611",strSiteType);//95@320@66
	}
		
}


long PotentialHazardEntryCategory::ShowPotentialHazardEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTNLHZRDENT;
	return ParentManage("HSE_TGPTNLHZRDENT",TRUE,TRUE,TRUE,"HSE_PTNLHZRDENT",strCriteria);
}