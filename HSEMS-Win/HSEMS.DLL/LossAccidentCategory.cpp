// LossAccidentCategory.cpp: implementation of the LossAccidentCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "LossAccidentCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LossAccidentCategory::LossAccidentCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

LossAccidentCategory::~LossAccidentCategory()
{

}

HRESULT LossAccidentCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
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
		if(strSubFormTag=="HSE_TGLOSSACDNTENTRJCRSN"||strSubFormTag=="HSE_TGLOSSRJCRSN")
		{																						
				SetRjctRsnFlag(pToolBarButtonClicked,"LOSSACDNTENT_ACTHZRDNO");					
		}
	}
	else if(pToolBarButtonClicked->iComplete==1 && strBtnName=="SAVE")
	{
		RefreshScreen("",REFRESH_SELECTED);
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

//Enable Confirmation comments in confirmation screens 
//note : not in specs 
HRESULT LossAccidentCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	if(pRecordRepostion->iComplete==1)
	{
		if(strFormTag=="HSE_TGLOSSACDNTCNFRMTN")
		{
			SetFieldVisibleAttrib("HSE_TGLOSSACDNTCNFRMTN","LOSSACDNTENT_CNFRMTNCMNT","NULL");		
		}	
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}


HRESULT LossAccidentCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//Rule ID : 204_330~1_3
	//Rule Description : Reject the record
	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	if(strButton_Name=="LOSSACDNTENT_RJCTD")
	{
		rejectRecord(strForm_Tag);
		bool bUpdateRejectedRecord=getgbUpdateRejectedRecord();
		if(bUpdateRejectedRecord==true)
		{
			UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,3,"LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");		
		}
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/************************************************************************************************
Helper function ->Rule ID : 102_300~1_3
	--Reject record
************************************************************************************************/
void LossAccidentCategory::rejectRecord(CString strForm_Tag)
{
	CString strActHzrdNo=FormGetField("HSE_LOSSACDNTENT","LOSSACDNTENT_ACTHZRDNO");
	//if no record selected
	if(strActHzrdNo=="")
		return;
	setSrcKyFldNm("ACTHZRDNO");
	setOwnerScreenTag(m_strScreenTag);
	if(strForm_Tag=="HSE_TGLOSSACDNTCNFRMTN")
	{			
		Reject("LOSSACDNTENT-L1",strActHzrdNo);			
	}
}


