// RiskAssessmentCategory.cpp: implementation of the RiskAssessmentCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "RiskAssessmentCategory.h"
/*
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"
*/
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*RiskAssessmentCategory::RiskAssessmentCategory()
{

}*/

RiskAssessmentCategory::~RiskAssessmentCategory()
{

}
/***************************************************************************************************************/
HRESULT RiskAssessmentCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{

	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	//polydb_task nRecordStatus = 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{				
		if(strSubFormTag=="HSE_TGRSKASSMNTFOLLWUPDT"||strSubFormTag=="HSE_TGRSKASSMNTAPRRJCRSN")
		{																						
				SetRjctRsnFlag(pToolBarButtonClicked,"RSKASSMNTENT_ASSMNTNO");					
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/***************************************************************************************************************/
HRESULT RiskAssessmentCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGRSKASSMNTAPR")
		{
			SetFieldVisibleAttrib("HSE_TGRSKASSMNTAPR","RSKASSMNTENT_APRCMNT","NULL");		
			//EnableToolbar("",TOOLBAR_SAVE,TRUE);
		}	
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}
/***************************************************************************************************************/
HRESULT RiskAssessmentCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 102_300~1_3 CR03-03
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strButton_Name=="RSKASSMNTENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			//UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");		
			//RefreshScreen("",REFRESH_SCREEN);
			CString strSQL;
			CString strUserName = GetUserLogin();
			CString strSourceScreenName = GetScrCptnByTag(66,strForm_Tag,"");
			CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
			strSQL.Format("EXECUTE rejectRiskAssessment '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
			ExecuteSQL(strSQL);		
			RefreshScreen("",REFRESH_SELECTED);
		}
	}else if(strButton_Name=="RSKASSMNTENTSTEPS_GETSTPS"){
		CString strRskAssmntKey = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
		getRskAssmntJobSteps(strRskAssmntKey);
		RefreshScreen("HSE_TGRSKASSMNTENT.HSE_TGRSKASSMNTENTSTEPS",REFRESH_SCREEN);	
	}
	else if(strButton_Name=="RSKASSMNTENTSTEPS_CNTRLMSR") {
		OpenCntrlMsure(strForm_Tag);
	}
	else if(strButton_Name=="RSKASSMNTENT_CHCKRSK") {
		CString strRskAssmntKey = FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
		checkRiskAssessment(strRskAssmntKey);
		RefreshScreen("",REFRESH_SELECTED);	
	} else if(strButton_Name=="RSKASSMNTENTSTEPS_STPEFFCT"){		
		OpnEffectsScr(strForm_Tag);
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	else if (strButton_Name=="POTENTIAL_HAZARDS")
	{
		//RQ-9-2021.1
		//Open (Potential Hazards) popup
		CString strSQL;
		extern CString gstrKeyField;
		gstrKeyField  =FormGetField(strSubFormTag,"MainLink");		//"HSE_TGAWRNSPLNENTRY_CRS"
		strSQL.Format("SELECT * FROM HSE_PtnlHzrd WHERE (PtnlHzrd_PrntLnk = %s) ",gstrKeyField);
		m_strScreenTag = "HSE_TGPTNLHZRD";
		bool bLocked=true;
		if(strForm_Tag=="HSE_TGRSKASSMNTENT")
			bLocked=false;
		///////////////////////////////////////////////////////
		//RQ-1-2022.33
		OpenPopUpScreen(strSubFormTag,strSubFormTag,"MainLink","HSE_PTNLHZRD","HSE_TGPTNLHZRD","Potential Hazards","","PtnlHzrd_PrntLnk",-1);
		//ShowScreen(0,m_strScreenTag,"Potential Hazards",NORMAL_MODE,true,strSQL,"","","",bLocked);		
		///////////////////////////////////////////////////////
	}

	else if (strButton_Name=="TASK_RESPONSIBILITY")
	{
		//RQ-9-2021.1
		//Open (Task Responsibility) popup
		CString strSQL;
		extern CString gstrKeyField;
		gstrKeyField  =FormGetField(strSubFormTag,"MAINLINK");
		strSQL.Format("SELECT * FROM HSE_TskRsp WHERE (TskRsp_PrntLnk = %s) ",gstrKeyField);
		m_strScreenTag = "HSE_TGTskRsp";
		bool bLocked=false;
		if(strForm_Tag=="HSE_TGRSKASSMNTINQ")
			bLocked=true;
		ShowScreen(0,m_strScreenTag,"Task Responsibility",NORMAL_MODE,true,strSQL,"","","",bLocked);		
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT RiskAssessmentCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName  = pFieldChange->Field_Name;	
	strFieldName.MakeUpper();
	if(strFieldName == "RSKASSMNTENTSTEPS_RSK1") {
		CString strNewVal = FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_RSK1");
		FormSetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_RSK2",strNewVal);
	} else if (strFieldName == "RSKASSMNTENTSTEPS_LKHOD1") {
		CString strNewVal = FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_LKHOD1");
		FormSetField("HSE_RSKASSMNTENTSTEPS_VIEW","RSKASSMNTENTSTEPS_LKHOD2",strNewVal);
	}
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

/***************************************************************************************************************
Helper Function
***************************************************************************************************************/			
void RiskAssessmentCategory::rejectRecord(CString strForm_Tag)
{
	CString strAssmntNo=FormGetField("HSE_RSKASSMNTENT","RSKASSMNTENT_ASSMNTNO");
	//if no record selected
	if(strAssmntNo=="")
		return;
	setSrcKyFldNm("ASSMNTNO");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGRSKASSMNTAPR")
	{			
		Reject("RSKASSMNTENT-L1",strAssmntNo);			
	}
	else if(strForm_Tag=="HSE_TGRSKASSMNTFOLLWUPDT")
	{
	
		Reject("RSKASSMNTENT-L2",strAssmntNo);		
	}
}


/***************************************************************************************************************
 * !!Not Used!!
 * Helper function
 * This function calc current risk rank 
 * Added By Nehad according to CR-09
 **************************************************************************************************************/
BOOL RiskAssessmentCategory::CalcCurrRiskRank()
{
	CString strSVRTYRAT1=FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_SVRTYRAT1");
	CString strLKLYHODRAT1=FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_LKLYHODRAT1");
	if(strLKLYHODRAT1 != "" && strSVRTYRAT1 != "")
	{
		CString strSQL="Select RSKSVRTYRAT_RAT From HSE_RSKSVRTYRAT Where MAINLINK="+strSVRTYRAT1;
		CString strSVRTYRAT1Val=GetRecordSetData(strSQL,1);

		int nReskRank = atoi(strLKLYHODRAT1)*atoi(strSVRTYRAT1Val);
		CString strReskRank;
		strReskRank.Format("%d",nReskRank);
		CString strAfterRsk =  FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNK");
		if(nReskRank <= atoi(strAfterRsk))
		{
			AfxMessageBox("[Risk Rank After Control Measures] must be less than [Current Risk Rank]");
			return FALSE;
		}

		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNK1",strReskRank);
	}
	else
	{
		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNK1","");
		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNKDESC1","");
	}
	return TRUE;
}

/**Deleted Code : CalcRiskRank function please check SourceSafe*/

/***************************************************************************************************************
	Helper function
	--This function take risk rank from risk assessment entry screen and retrive risk rank description
	  from risk rank entry screen and set it in risk assessment entry screen
****************************************************************************************************************/
void RiskAssessmentCategory::RetriveRiskRankDes()
{
	CString strRiskRank1 = FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNK1");
	CString strRiskRank  = FormGetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNK");
	if(strRiskRank1 != "")
	{
		CString strSQL1="SELECT RSKRNKDESC_DESC FROM HSE_RSKRNKDESC WHERE RSKRNKDESC_RNK="+strRiskRank1;
		CString strRiskRankDesc1=GetRecordSetData(strSQL1,1);
		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNKDESC1",strRiskRankDesc1);
	}
	if(strRiskRank != "")
	{
		CString strSQL="SELECT RSKRNKDESC_DESC FROM HSE_RSKRNKDESC WHERE RSKRNKDESC_RNK="+strRiskRank;
		CString strRiskRankDesc=GetRecordSetData(strSQL,1);
		FormSetField("HSE_RSKASSMNTENTPTNLHZRD","RSKASSMNTENTPTNLHZRD_RSKRNKDESC",strRiskRankDesc);
	}
}		

CString RiskAssessmentCategory::OpenCntrlMsure(CString strFormTag) {		
	CString strSQL,
			strDefValues,
			strKeyField;			
	bool bLocked = true;	
	CString strMainKeyFeild = FormGetField("HSE_RSKASSMNTENT" ,"RSKASSMNTENT_ASSMNTNO");			
	strKeyField		= FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","MAINLINK");
	if(strKeyField == "")
		return "";	
	strDefValues.Format("NCSSRYCTRLMSRMNT_LNK|%s",strKeyField);	
	strSQL.Format("SELECT * FROM HSE_NCSSRYCTRLMSRMNT WHERE (NCSSRYCTRLMSRMNT_LNK = %s)",strKeyField);
	m_strScreenTag = "HSE_TGNCSSRYCTRLMSRMNT";		
	if(strFormTag =="HSE_TGRSKASSMNTENT") {
		bLocked = false;
	}				
	ShowScreen(0,"HSE_TGNCSSRYCTRLMSRMNT","Control Measures",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);		
	return strKeyField;
}

void RiskAssessmentCategory::OpnEffectsScr(CString strFormTag) {
	CString strSQL,
		    strDefValues,
			strMainlink;					
	strMainlink = FormGetField("HSE_RSKASSMNTENTSTEPS_VIEW","MAINLINK");		
	strDefValues.Format("STPEFFCT_LNK|%s",strMainlink);	
	strSQL.Format("SELECT * FROM HSE_STPEFFCT WHERE (STPEFFCT_LNK = '%s')",strMainlink);							
	////////////////////////////////////////////////////////////////////////////////////////
	//Bugs-3-2021.142
	/*
		Risk Assessment Inquiry	"Under ""Job Steps"" tab, 
		The ""Effects"" popup screen, delete buttn is enabled and delete historical data
	*/
	bool bBlocked=false;
	if(strFormTag=="HSE_TGRSKASSMNTINQ")
		bBlocked=true;
	////////////////////////////////////////////////////////////////////////////////////////
	ShowScreen(0,"HSE_TGSTPEFFCT","Effects",NORMAL_MODE,true,strSQL,"",strDefValues,"",bBlocked);
}

/*************************************************************************************************************
*@deprecated 'Potential Hazard tab removed'
*Helper function
*Open Risk Analysis screen
*************************************************************************************************************/
void RiskAssessmentCategory::OpnRskAnlysScr(CString strKeyField,bool bLocked)
{
	//check key field before open screen
	if(strKeyField == "")
		return;
	CString strSQL;
	CString strDefValues;	
	strDefValues.Format("RSKANLYS_LNK|%s",strKeyField);	
	strSQL.Format("SELECT * FROM HSE_RSKANLYS WHERE (RSKANLYS_LNK = '%s')",strKeyField);				
	ShowScreen(0,"HSE_TGRSKANLYS","Risk Analysis",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}


void RiskAssessmentCategory::getRskAssmntJobSteps(CString strRskAssmntKey) {
	if(strRskAssmntKey != "") {
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		//RQ-9-2021.1 Starts HSM 29/9/21
		CString strSQL;
		//strSQL.Format("EXECUTE getJopStepsCnsqns %s",strRskAssmntKey);
		strSQL.Format("declare @x int;exec @x=Rsk_Asmnt_entry_Save %s;select @x as x into #Temp",strRskAssmntKey);
		ExecuteSQL(strSQL);
		strSQL.Format("Select  x from #Temp");

		CPolyDbRecordset DaoDBRecSet;
		DaoDBRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
		CString strRetVal;
		if(DaoDBRecSet.IsEOF())
		{
			strRetVal = "1";
		}
		else
			DaoDBRecSet.GetFieldValue("x",strRetVal);
		DaoDBRecSet.Close();
		ExecuteSQL(CString("drop table #Temp"));
		int nRetVal=atoi(strRetVal);
		switch(nRetVal)
		{
		case 0:
			AfxMessageBox("Main record does not exist or it is not at entry ststus");
			break;
		case -1:
			AfxMessageBox("Record has previous details");
			break;
		case -2:
			AfxMessageBox("There was a prevoius record");
			break;
		case -3:
			AfxMessageBox("Record has previous details and they are now deleted and copied from prevoius record ");
			break;
		case -4:
			AfxMessageBox("There was no prevoius record to copy from");
			break;
		case -5:
			AfxMessageBox("Record has previous details & There was no prevoius record to copy from or previous record has no details so previous details were not deleted");
			break;
		}
		//RQ-9-2021.1 Ends HSM 29/9/21
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	}
}


void RiskAssessmentCategory::checkRiskAssessment(CString strRskAssmntKey) {
	if(strRskAssmntKey != "") {
		CString strSQL;
		strSQL.Format("EXECUTE checkRskAssmntTXN %s",strRskAssmntKey);
		ExecuteSQL(strSQL);
	}
}