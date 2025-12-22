// NearMissReward.cpp: implementation of the NearMissReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NearMissReward.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NearMissReward::NearMissReward(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


NearMissReward::~NearMissReward()
{

}


HRESULT NearMissReward::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	//RQ-2-2021(Batch2).5
	// It was a bug since the strBtnName variable was not initialized
	CString strBtnName=pCustomButtonClicked->Button_Name;
	///////////////////////////////////////////////////////
	CString strSubFormTag = pCustomButtonClicked->SubForm_Tag;
	if(strBtnName=="NRSTMISCENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;

		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		completeNearMissReward(pCustomButtonClicked);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void NearMissReward::completeNearMissReward(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	FormSetField("HSE_vwNRSTMISCENT","NRSTMISCENT_RWRDFLG","Y");
	RefreshScreen("",REFRESH_SELECTED);
}

HRESULT NearMissReward::DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion)
{
	//To disable NEW button "Framework bug disable function doesn't work in tabs"
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	EnableToolbar("",TOOLBAR_DELETE,FALSE);
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

HRESULT NearMissReward::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);	
	EnableToolbar("",TOOLBAR_DELETE,FALSE);
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

long NearMissReward::ShowNearMissReward(CString strCriteria)
{
	m_nCurrentCard=ID_NEARMISS_NEARMISSREWARD;
	return ParentManage("HSE_TgNrMisRward",TRUE,TRUE,TRUE,"HSE_vwNrstMiscEnt",strCriteria);
}