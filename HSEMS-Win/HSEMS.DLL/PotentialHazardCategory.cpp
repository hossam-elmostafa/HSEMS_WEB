// PotentialHazardCategory.cpp: implementation of the PotentialHazardCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PotentialHazardCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PotentialHazardCategory::PotentialHazardCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

PotentialHazardCategory::~PotentialHazardCategory()
{

}
/************************************************************************************************************/
HRESULT PotentialHazardCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	//polydb_task nRecordStatus = 0
	//if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE"&&pToolBarButtonClicked->nRecordStatus==0)
	if(pToolBarButtonClicked->iComplete==0 && strBtnName=="SAVE")
	{			
		//000_2
		//Action : Add new record
		//Condition : If new reject reason for that record exist 
		//Response : Message box inform user that reject reason already exist and cancel add operation 
		//Else Add new record
		if(strSubFormTag=="HSE_TGPTNLHZRDFLWUPRJCRSN"||strSubFormTag=="HSE_TGPTNLHZRDCNFRMTNRJCRSN")
		{																						
				SetRjctRsnFlag(pToolBarButtonClicked,"PTNLHZRDENT_PTNLHZRDNO");					
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);	
}
/************************************************************************************************************/
//Enable Confirmation comments in confirmation screens 
//note : not in specs 
HRESULT PotentialHazardCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGPTNLHZRDCNFRMTN")
		{
			SetFieldVisibleAttrib("HSE_TGPTNLHZRDCNFRMTN","PTNLHZRDENT_CNFRMTNCMNT","NULL");		
			//EnableToolbar("",TOOLBAR_SAVE,TRUE);
		}	
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}
/************************************************************************************************************/
HRESULT PotentialHazardCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 102_300~1_3
	//Rule Description : Reject the record		
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="PTNLHZRDENT_RJC")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");		
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);;
}
/************************************************************************************************
Helper function ->Rule ID : 102_300~1_3
	--Reject record
************************************************************************************************/
void PotentialHazardCategory::rejectRecord(CString strForm_Tag)
{
	CString strPtnlHzrdNo=FormGetField("HSE_PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
	//if no record selected
	if(strPtnlHzrdNo=="")
		return;
	setSrcKyFldNm("PTNLHZRDNO");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGPTNLHZRDCNFRMTN")
	{			
		Reject("PTNLHZRDENT-L1",strPtnlHzrdNo);			
	}
	else if(strForm_Tag=="HSE_TGPTNLHZRDFOLLOWUP")
	{	
		Reject("PTNLHZRDENT-L2",strPtnlHzrdNo);		
	}
}

