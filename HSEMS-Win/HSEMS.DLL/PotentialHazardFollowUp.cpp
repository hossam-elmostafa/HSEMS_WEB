// PotentialHazardFollowUp.cpp: implementation of the PotentialHazardFollowUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PotentialHazardFollowUp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PotentialHazardFollowUp::PotentialHazardFollowUp(CCategory *pCategory):PotentialHazardCategory(pCategory)
{
	m_pCategory = pCategory;
}

PotentialHazardFollowUp::~PotentialHazardFollowUp()
{

}
/*************************************************************************************************************************************************/
HRESULT PotentialHazardFollowUp::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)	
{
	//212_320~2_2
	CString strButtonName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	strButtonName.MakeUpper();
	//Framework Event : Pressing Button(Close)[35]
	DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
	if(strButtonName=="PTNLHZRDENT_CLS")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
			ClosePotential(pCustomButtonClicked);
	}		
	return PotentialHazardCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************************************************/
HRESULT PotentialHazardFollowUp::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_NEW,FALSE);
	return PotentialHazardCategory::DisplayScreenReady(pScreenReady);
}

/*************************************************************************************************************************************************
Helper function
*************************************************************************************************************************************************/
void PotentialHazardFollowUp::ClosePotential(tag_CustomButtonClicked* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	//Update (Record Status[41@320~2]) of selected record into (6)
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,6,"PTNLHZRDENT","PTNLHZRDENT_PTNLHZRDNO");
	//"	Insert record into (tracing [@32020~2@66]) tab
	InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",CLOSED);
}

long PotentialHazardFollowUp::ShowPotentialhazardFollowUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTNLHZRDFOLLOWUP;
	return ParentManage("HSE_TGPTNLHZRDFOLLOWUP",TRUE,TRUE,TRUE,"HSE_PTNLHZRDENT",strCriteria);
}