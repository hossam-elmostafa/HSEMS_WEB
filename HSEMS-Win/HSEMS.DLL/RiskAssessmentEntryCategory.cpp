// RiskAssessmentEntryCategory.cpp: implementation of the RiskAssessmentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "RiskAssessmentEntryCategory.h"
#include <string>
#include "HSEobj.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RiskAssessmentEntryCategory::RiskAssessmentEntryCategory(CCategory *pCategory):RiskAssessmentCategory(pCategory)
{
	m_pCategory = pCategory;
}

RiskAssessmentEntryCategory::~RiskAssessmentEntryCategory(){}

/*************************************************************************************************************************************/
//FrameWork Events
/*************************************************************************************************************************************/
HRESULT RiskAssessmentEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//101_300_1
	CString strSQL	=	"";
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strButtonName(pCustomButtonClicked->Button_Name);
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="RSKASSMNTENT_ENTCMPLT")
	{
		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompleteRiskAssessment(pCustomButtonClicked);
	}
	/*Potential hazard tab removed
	 *Done by:- Doaa Desouky
	else if(strButtonName=="RSKASSMNTENTPTNLHZRD_NCSSRYCTRLMSRMNT")
	{
		//
		CString strKeyField = FormGetField("HSE_RSKASSMNTENTPTNLHZRD","MAINLINK");
		OpnRskAnlysScr(strKeyField,false);
		//Removed Code,Please Check Source Safe
	}	
	else if(strButtonName=="RSKASSMNTENTPTNLHZRD_GETJOBHZRDS") {		
		getRskAsmntPtntlHzrd();
		RefreshScreen("HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE",REFRESH_SCREEN);	
	}*/

	///////////////////////////////////////////////////////////////////////////////////////////
	//RQ-2-2021(Batch2).47
	if(strButtonName=="RSKASSMNTENT_SHOWMATRIX")
	{
		CString strWorkingFolder=m_pCategory->GetHSEObj()->GetWorkingFolder();
		CString strRiskAssementMatrixPath=strWorkingFolder+"\\RiskAssessment.jpg";		
#define	FILE_EXISTS	32
		if (GetFileAttributes(strRiskAssementMatrixPath) != FILE_EXISTS)
			AfxMessageBox("Matrix picture is not found at:" + strRiskAssementMatrixPath);
		else
			ShellExecute(0, 0, strRiskAssementMatrixPath, 0, 0 , SW_SHOW );
	}
	return RiskAssessmentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}	

HRESULT RiskAssessmentEntryCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;
	CString strFullRecord = pBrowsingSelectedItem->Full_Record;	
	if(strBrowsingField == "RSKASSMNTENT_SIT") {
		CString strSiteCode = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_SIT");
		CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
		CString strMaxAcptdRskRnk = getMaxAcptdRskRnk(strCmpnyNam);
		FormSetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ACPTDRSKRNK",strMaxAcptdRskRnk);
	}
	return RiskAssessmentCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}
	
/**************************************************************************************************************/
HRESULT RiskAssessmentEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);	
	/*CString strSvrty = FormGetField("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTYRAT");
	if(strBtnName=="CANCEL"&& CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGRSKASSMNTENTPTNLHZRD" && pToolBarButtonClicked->iComplete == 0)
	{
		//polydb_task
		CurrentRiskAnalysisFrame("ALWAYSLOCKED");		
		if(strSvrty== "" || strSvrty.Find("0") != -1)
			RskAnlsAfterNcsryCntrlMsur("ALWAYSLOCKED");
		else
			RskAnlsAfterNcsryCntrlMsur("MUST");	
		
	}
	if(strBtnName=="NEW"&& CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGRSKASSMNTENTPTNLHZRD" && pToolBarButtonClicked->iComplete == 1)
	{
		//polydb_task
		CurrentRiskAnalysisFrame("ALWAYSLOCKED");	
		if(strSvrty == ""|| strSvrty.Find("0") != -1)
			RskAnlsAfterNcsryCntrlMsur("ALWAYSLOCKED");
		else
			RskAnlsAfterNcsryCntrlMsur("MUST");		
	}*/
	if(strBtnName=="SAVE")
	{
		if(CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGRSKASSMNTENTPTNLHZRD") {
			if(pToolBarButtonClicked->iComplete == 1) {
				RefreshScreen("HSE_TGRSKASSMNTENTPTNLHZRD",REFRESH_SELECTED);
			}
		}
		if(pToolBarButtonClicked->iComplete == 1) {
			bool bNewRec = m_pCategory->GetHSEObj()->getbNewMode(strFormTag);
			if(bNewRec) {
				CString strRskAssmntKey = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
				getRskAsmntPtntlHzrd();
				getRskAssmntJobSteps(strRskAssmntKey);
			}
		}
	} /*else if(strBtnName == "DELETE"){
		CString strKeyField = FormGetField("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENT_ASSMNTNO");
		HandleDelClass *handleDelClass = new HandleDelClass(m_pCategory);
		bool bAllowDel = handleDelClass->deleteRec("HSE_TGRSKASSMNTENT",strKeyField,GetUserLogin());
		if(!bAllowDel)
			*pToolBarButtonClicked->iCancle = 1;
	}*/	

	if(strBtnName=="SAVETAB")
	{
		CString strTabName= (*pToolBarButtonClicked).SubForm_Tag;
		if(strTabName=="HSE_TGRSKASSMNTENTSTEPS")
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/*
				RQ-9-2021.1
				If (risk rank1) > (risk rank2) then display warning message “The risk is increases, please check”.
				By Hossam 20/9/21
			*/
			int nRiskRank1 = atoi(FormGetField("HSE_TGRSKASSMNTENTSTEPS","RSKASSMNTENTSteps_Rsk2"))*atoi(FormGetField("HSE_TGRSKASSMNTENTSTEPS","RSKASSMNTENTSteps_Lkhod2"));
			int nRiskRank2 = atoi(FormGetField("HSE_TGRSKASSMNTENTSTEPS","RSKASSMNTENTSteps_Rsk1"))*atoi(FormGetField("HSE_TGRSKASSMNTENTSTEPS","RSKASSMNTENTSteps_Lkhod1"));
			if(nRiskRank1>nRiskRank2)
			{
				AfxMessageBox("The risk is increasing, please check");
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	return RiskAssessmentCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/**************************************************************************************************************/
HRESULT RiskAssessmentEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//filter detail browsing field according to main browsing field 
	//note : not in specs 
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	//101_300_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	
	/*
	 * Moved to FirstSysDB
	 if(strFieldName=="RSKASSMNTENT_LOCTYP")
	{

		CString strLocTyp=FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_SIT");	
		CString strSQL;
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	*/

	//101_300_3	
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	
	/*
	 * Moved to FirstSysDB
	 if(strFieldName=="RSKASSMNTENT_WRKLOC")
	{
		CString strLocTyp=FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_LOCTYP");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}*/

	
	// Update : tab removed from system
	// Rule Id :CR03-08 
	// Done by : Doaa Desouky 
	// Date : 9/06/2010
	// Requirement:-  Filter severity rating [18] according to first level Severity Rating Class [70]
	//else if(strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTYRAT")
	//{
	//	OpenSeverityRatingTabFilteredonSelectedClass(pBeforBrowsingButtonClick,"RSKASSMNTENTPTNLHZRD_SVRTRATCLS");
	//}
	//else if(strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTYRAT1")
	//{
	//	OpenSeverityRatingTabFilteredonSelectedClass(pBeforBrowsingButtonClick,"RSKASSMNTENTPTNLHZRD_SVRTRATCLS1");
	//}
	//else if(strFieldName=="RSKASSMNTENTPTNLHZRD_HZRDCTG")
	//{
	//	CString strHzrdGrp=FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_HZRDGRP");
	//	if(strHzrdGrp.IsEmpty())
	//		strHzrdGrp=" ";
	//	CString strSQL;
	//	strSQL.Format("WHERE (HZRDHZRDCTG_LNK='%s')",strHzrdGrp);
	//	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
	//}
	//else if(strFieldName=="RSKASSMNTENTPTNLHZRD_HRMTYP")
	//{
	//	CString strTskGrp=FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_HZRDCTG");
	//	if(strTskGrp.IsEmpty())
	//		strTskGrp=" ";
	//	CString strSQL;
	//	strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strTskGrp);
	//	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	//}

	/*
	 * 18-8-2020 Moved to firstSysDB
	 else if(strFieldName=="RSKASSMNTENT_JOBNAM")
	{
		CString strTskGrp=FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_TSKCOD");
		if(strTskGrp.IsEmpty())
			strTskGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (TSKDTLS_LNK ='%s')",strTskGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}*/
	
	if (strFieldName=="RSKASSMNTENTSTEPS_HZRDCATGRY") {
		CString strHzrdGrp=FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_HZRDGRP");
		if(strHzrdGrp.IsEmpty())
			strHzrdGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (HZRDHZRDCTG_LNK='%s')",strHzrdGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}

	else if (strFieldName=="RSKASSMNTENTSTEPS_HRMTYP") {
		CString strHzrdCat=FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_HZRDCATGRY");
		if(strHzrdCat.IsEmpty())
			strHzrdCat=" ";
		CString strSQL;
		strSQL.Format("WHERE (HRMTYP_LNK ='%s')",strHzrdCat);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}

	/*
	 * Create new Consequence screen which is not related to hirearchy (job -> details -> steps -> Hazard -> Consequences )
	 * user select from this new lookup screen and system build library automatically
	else if (strFieldName=="RSKASSMNTENTSTEPS_CNSQNCS") {
		CString strHzrdGrp = FormGetField("HSE_RSKASSMNTENTSTEPS","RSKASSMNTENTSTEPS_HZRDGRP");
		CString strHzrdCat = FormGetField("HSE_RSKASSMNTENTSTEPS","RSKASSMNTENTSTEPS_HZRDCATGRY");
		CString strStep = FormGetField("HSE_RSKASSMNTENTSTEPS","RSKASSMNTENTSTEPS_STEP");
		//CString strHrmTyp = FormGetField("HSE_RSKASSMNTENTSTEPS","RSKASSMNTENTSTEPS_HRMTYP");
		if(strHzrdGrp.IsEmpty())
			strHzrdGrp = " ";
		if(strHzrdCat.IsEmpty())
			strHzrdCat = " ";		
		if(strStep.IsEmpty())
			strStep = " ";
		CString strSQL;
		//strSQL.Format("WHERE (CNSQNCS_LNKWTHDTL ='%s')",strHrmTyp);
		strSQL.Format("WHERE Cnsqncs_LNKWTHDTL IN (SELECT MAINLINK FROM HSE_JOBSTPHZRD WHERE JobStpHzrd_hzrdGrp = '%s' AND JobStpHzrd_hzrdCtg = %s AND DETAILLINK = %s)",strHzrdGrp,strHzrdCat,strStep);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	*/

	return RiskAssessmentCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

void RiskAssessmentEntryCategory::OpenSeverityRatingTab(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick,CString SeverityClassName)
{
	CString strSevrityClass=FormGetField("HSE_RSKASSMNTENTPTNLHZRD",SeverityClassName);
	if(strSevrityClass.IsEmpty())
	{
		CString strSQL;
		strSQL="";
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 	
	}		
	else
	{
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strSevrityClass);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
	}
}
/**************************************************************************************************************/
HRESULT RiskAssessmentEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{		
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	
	//Tab Removed "Potential Hazard tab"
	// 101_301_1 , 101_301_5
	// Calculate OnLine Resk Rank according to CR-09 by Nehad
	/*if(strSubFormTag=="HSE_TGRSKASSMNTENTPTNLHZRD" && (strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTYRAT1"||strFieldName=="RSKASSMNTENTPTNLHZRD_LKLYHODRAT1") && (*pFieldChange).iComplete == 1)
	{
		if(!CalcCurrRiskRank())
		{
			*pFieldChange->iCancle = 1;
			return S_FALSE;
		}
	}*/

	// 101_301_1 , 101_301_5
	// Calculate OnLine Resk Rank according to CR-09 by Nehad
	/*
	if(strSubFormTag=="HSE_TGRSKASSMNTENTPTNLHZRD" && (strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTYRAT"||strFieldName=="RSKASSMNTENTPTNLHZRD_LKLYHODRAT") && (*pFieldChange).iComplete == 1)
	{
		if(!CalcRiskRank())
		{
			*pFieldChange->iCancle = 1;
			return S_FALSE;
		}
	}
	
	// Calculate OnLine Resk Rank description according to CR-09 by Nehad
	if(strSubFormTag=="HSE_TGRSKASSMNTENTPTNLHZRD" && (strFieldName=="RSKASSMNTENTPTNLHZRD_RSKRNK"||strFieldName=="RSKASSMNTENTPTNLHZRD_RSKRNK1") && (*pFieldChange).iComplete == 1)
	{
		RetriveRiskRankDes();
	}*/		
	
	// Tab Removed
	// Rule Id : CR03-13
	//Done by : Doaa Desouky
	//Date : 08/06/2010
	/* Requirement is : Enter Data in this tab in following:-
                        identify hazard then current risk analysis then 
						add record in screen necessary control measure 
						then risk analysis after control measure) */
	/*else if(strSubFormTag=="HSE_TGRSKASSMNTENTPTNLHZRD"&&(strFieldName=="RSKASSMNTENTPTNLHZRD_IDNTFDHZRD"||
			strFieldName=="RSKASSMNTENTPTNLHZRD_HZRDGRP" || strFieldName=="RSKASSMNTENTPTNLHZRD_HZRDCTG" ||
			strFieldName=="RSKASSMNTENTPTNLHZRD_HRMTYP" || strFieldName=="RSKASSMNTENTPTNLHZRD_HZRDDESC" ||
			strFieldName=="RSKASSMNTENTPTNLHZRD_EFCTDESC"))
	{
		
		bool bIdentifiedHazard = 0;

		bIdentifiedHazard = CheckIdentifiedHazardFrame();
		if(bIdentifiedHazard)
		{
			CurrentRiskAnalysisFrame("MUST");
		}
	}*/
	
	//tab removed
	//else if(strSubFormTag=="HSE_TGRSKASSMNTENTPTNLHZRD"&&(strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTYRAT"||
	//		strFieldName=="RSKASSMNTENTPTNLHZRD_LKLYHODRAT1" || strFieldName=="RSKASSMNTENTPTNLHZRD_SVRTRATCLS1" ||
	//		strFieldName=="RSKASSMNTENTPTNLHZRD_CMNT1"))
	//{
		//bool CurrentAnalysis;
		//CurrentAnalysis = CheckCurrentAnalysisFrame();
		/*if(CurrentAnalysis)
		{
			EnableNecessaryControlMeasure();
		} */
	/*
	 * Moved to FirstSysDB
		if (strFieldName=="RSKASSMNTENT_SIT") {
		FormSetField("HSE_RSKASSMNTENT","rskassmntEnt_LocTyp","");
	}*/	
	return RiskAssessmentCategory::DisplayFieldChange (pFieldChange);
}

/*************************************************************************************************************************************/
 // HELPER FUNCTIONS
/*************************************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
bool RiskAssessmentEntryCategory::CheckIdentifiedHazardFrame()
{
	CString HZRDGRP = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_HZRDGRP");
	CString HZRDCTG = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_HZRDCTG");
	//harm type become optional field according to CR1
	//CString HRMTYP = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_HRMTYP");
	CString HZRDDESC = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_HZRDDESC");
	CString EFCTDESC = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_EFCTDESC");

	if(	HZRDGRP !="" && HZRDCTG !="" && HZRDDESC !="" && EFCTDESC !="")
		return true;
	
	else
		return false;
}
/*************************************************************************************************************
 * !! Not Used !!
 * Helper function
 *************************************************************************************************************/
void RiskAssessmentEntryCategory::CurrentRiskAnalysisFrame(CString Attribute)
{
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTYRAT1",Attribute);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_LKLYHODRAT1",Attribute);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTRATCLS1",Attribute);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_CMNT1",Attribute);
}
/*************************************************************************************************************
 * !! Not Used !!
 * Helper function
 *************************************************************************************************************/
bool RiskAssessmentEntryCategory::CheckCurrentAnalysisFrame()
{
	CString SVRTYRAT = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_SVRTYRAT1");
	CString LKLYHODRAT = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_LKLYHODRAT1");
	CString SVRTRATCLS = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_SVRTRATCLS1");
	CString CMNT = FormGetField("HSE_RSKASSMNTENTPTNLHZRD" ,"RSKASSMNTENTPTNLHZRD_CMNT1");
	
	if(	(SVRTYRAT!=""||SVRTYRAT.Find("0") == -1) && LKLYHODRAT!="" && SVRTRATCLS!=""&& CMNT!="")
		return true;
	
	else
		return false;
}

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void RiskAssessmentEntryCategory::EnableNecessaryControlMeasure()
{

}
/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void RiskAssessmentEntryCategory::RskAnlsAfterNcsryCntrlMsur(CString Attrib)
{
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTYRAT",Attrib);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_LKLYHODRAT",Attrib);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTRATCLS",Attrib);
	SetFieldVisibleAttrib("HSE_TGRSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_CMNT",Attrib);
}

bool RiskAssessmentEntryCategory::CheckRecordsInNecessaryControlMeasure(CString strKeyField)
{
	int nNumberOfRecords = getNumberOfRecord("HSE_NCSSRYCTRLMSRMNT" ,strKeyField,"NCSSRYCTRLMSRMNT_LNK");

	if(nNumberOfRecords != 0)
	{

		return true;
	}
	else
		return false;
}

/*Removed Code,Please Check Source Safe*/

void RiskAssessmentEntryCategory::EnableRiskAnalysisAfterControlMeasure(CString strKeyField)
{
				
	bool NCsryCntrlMsur;		
	NCsryCntrlMsur = CheckRecordsInNecessaryControlMeasure(strKeyField);
			
	if(NCsryCntrlMsur)
	{
		RskAnlsAfterNcsryCntrlMsur("MUST");
		CString strCrntRskClass = FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTRATCLS1");
		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTRATCLS",strCrntRskClass);
		 
	}
}

bool RiskAssessmentEntryCategory::ReturnMesssageToFillAllData()
{
	CString strMessage="You must enter data in identified hazard frame and current risk analysis frame";
	AfxMessageBox(strMessage);
	return false;
}

void RiskAssessmentEntryCategory::OpenSeverityRatingTabFilteredonSelectedClass(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick,CString SeverityClassName)
{
	CString strSevrityClass=FormGetField("HSE_RSKASSMNTENTPTNLHZRD",SeverityClassName);
	if(strSevrityClass.IsEmpty())
		strSevrityClass=" ";
	CString strSQL ="";
	strSQL.Format("WHERE (LINK ='%s')",strSevrityClass);
	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 		
}
/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void RiskAssessmentEntryCategory::ChangeTabDefaultValue()
{
	COleDateTime dt = COleDateTime::GetCurrentTime();
	CString strDateVal = dt.Format("%d %b %Y %H:%M:%S");
	strDateVal.Format("%f",dt.m_dt);
	FormSetField("HSE_RSKASSMNTENTPTNLHZRD","MAINLINK",dt.m_dt);
}

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void RiskAssessmentEntryCategory::CompleteRiskAssessment(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	/*Deleted Code,Please Check Source Safe*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE completeRskAssmntTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}


long RiskAssessmentEntryCategory::ShowRiskAssessmentEntry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_RSKASSMNTENT;
	return ParentManage("HSE_TGRSKASSMNTENT",TRUE,TRUE,TRUE,"HSE_RSKASSMNTENT",strCriteria);	
}


void RiskAssessmentEntryCategory::getRskAsmntPtntlHzrd() {
	CString strRiskAssmentKey = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
	CString strJobGroup	      = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_TSKCOD");	 
	CString strJobName		  = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_JOBNAM");
	if(strRiskAssmentKey == "")
		return;
	CString strSQL = "EXECUTE getExpectedRiskAssmentHazards "+strRiskAssmentKey+",'"+strJobGroup+"','"+strJobName+"'";
	ExecuteSQL(strSQL);
}


