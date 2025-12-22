// PerformanceMeasurementCategory.cpp: implementation of the PerformanceMeasurementCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PerformanceMeasurementCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PerformanceMeasurementCategory::PerformanceMeasurementCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}
PerformanceMeasurementCategory::~PerformanceMeasurementCategory()
{
}

HRESULT PerformanceMeasurementCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{						
		//When user press save button Check if there is reject reason before setting new reject reason
		if(strSubFormTag=="HSE_TGPERFMSRMNTENTRJCRSN"||strSubFormTag=="HSE_TGPERFMSRMNTEVALREJRES")																									
		SetRjctRsnFlag(pToolBarButtonClicked,"PERFMSRMNTENT_VSTNO");								
	} else if(pToolBarButtonClicked->iComplete==1 && strBtnName=="SAVE") {
		RefreshScreen("",REFRESH_SELECTED);
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

//Enable Confirmation comments in confirmation screens 
//note : not in specs 
HRESULT PerformanceMeasurementCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGPERFMSRMNTFLWUP")
		{
			SetFieldVisibleAttrib("HSE_TGPERFMSRMNTFLWUP","PERFMSRMNTENT_FLW_UPCMNT","NULL");		
		}	
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}


HRESULT PerformanceMeasurementCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 
	//Rule Description : Reject the record	
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="PERFMSRMNTENT_RJCT")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			//UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"PERFMSRMNTENT","PERFMSRMNTENT_VSTNO");	
			//RefreshScreen("",REFRESH_SELECTED);
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectPerformaceTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);	
		}
	} else if(strButton_Name=="SHW_DTL") {
		ShowDetails(pCustomButtonClicked);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void PerformanceMeasurementCategory::rejectRecord(CString strForm_Tag)
{
	CString strVstNo=FormGetField("HSE_PERFMSRMNTENT","PERFMSRMNTENT_VSTNO");	
	if(strVstNo=="")	//if no record selected
		return;
	setSrcKyFldNm("VSTNO");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGPERFMSRMNTEVAL") {			
		Reject("PERFMSRMNTENT-L1",strVstNo);			
	}
	else if(strForm_Tag=="HSE_TGPERFMSRMNTFLWUP") {	
		Reject("PERFMSRMNTENT-L2",strVstNo);		
	}
}

void PerformanceMeasurementCategory::ShowDetails(tag_CustomButtonClicked* pCustomButtonClicked)
{	
	CString strSlctdGrp = FormGetField("HSE_PERFMSRMNTENTVSTCHCKLST" ,"PERFMSRMNTENTVSTCHCKLST_GRP");		// Get Selected group	
	CString strSlctdSec = FormGetField("HSE_PERFMSRMNTENTVSTCHCKLST" ,"PERFMSRMNTENTVSTCHCKLST_SECND");		// Get Selected Section
	// Open details screen
	CString strWhereCond="WHERE (DTLFORCHKLIST_LNK = '"+strSlctdSec+"')";
	m_strScreenTag = "HSE_TGCPYDTL";	
	CString strFormTag = "";
	strFormTag = (pCustomButtonClicked->Form_Tag);
	CString strKeyField = FormGetField("HSE_PERFMSRMNTENTVSTCHCKLST","MAINLINK");		
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL="EXEC FillDetailForPerformanceTab '"+strKeyField+"' , '"+strSlctdSec+"'";					
	long lRetVal = ExecuteSQL(strSQL);
	if(strFormTag =="HSE_TGPERFMSRMNTENT") {			
		strWhereCond="SELECT * FROM HSE_DTLFORCHKLIST WHERE (DTLFORCHKLIST_LNK = '"+strSlctdSec+"' AND (DTLFORCHKLIST_LNKWZCHKLSTTAB = '"+strKeyField+"'))";			
		tagMultiSelectedRows strFieldsValues;										
		ShowScreen(0,"HSE_TGCPYDTL","Details",NORMAL_MODE,true,strWhereCond,"","","",false);
	} else  {
		strWhereCond="SELECT * FROM HSE_DTLFORCHKLIST WHERE (DTLFORCHKLIST_LNK = '"+strSlctdSec+"' AND (DTLFORCHKLIST_LNKWZCHKLSTTAB = '"+strKeyField+"') AND DTLFORCHKLIST_USEINEVAL = 1)";
		if (strFormTag =="HSE_TGPERFMSRMNTEVAL") {				
			ShowScreen(0,"HSE_TGANSWRDTL","Details",NORMAL_MODE,true,strWhereCond,"","","",false);	
		} else if(strFormTag =="HSE_TGPERFMSRMNTFLWUP" || strFormTag =="HSE_TGPERFMSRMNTINQ" ) {
			tagMultiSelectedRows strFieldsValues;
			strFieldsValues.lCount = 0;
			ShowScreen(0,"HSE_TGANSWRDTL","Details",NORMAL_MODE,true,strWhereCond,"","","",false);					
		}	
	}
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
}


