// AuditPlanPrepreChkLst.cpp: implementation of the AuditPlanPrepreChkLst class.
//
//	!!!!!!! Important !!!!!!!!!
//  ****************************
//  
//	This Screen is no longer used 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuditPlanPrepreChkLst.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuditPlanPrepreChkLst::AuditPlanPrepreChkLst(CCategory *pCategory):AuditCategory(pCategory)
{
	m_pCategory = pCategory;
}

AuditPlanPrepreChkLst::~AuditPlanPrepreChkLst()
{
	
}

HRESULT AuditPlanPrepreChkLst::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{

	CString strBtnName=(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSQL;
	m_strScreenTag = pCustomButtonClicked->Form_Tag;


	if(strBtnName=="ADTPLNENTACTVT_ADTCHCKLST")
	{
		OpenAuditCheckListScreen(strFormTag);	
	}

	if(strBtnName=="ENTRYCMPLTTHRDCYCL")
	{
		CString KeyFieldValue = GetKeyField(strFormTag,pCustomButtonClicked->pMultiSelectedRows);
		if(KeyFieldValue != "")
		{
			CompleteAuditWithAuditChkLst(pCustomButtonClicked);	
			RefreshScreen("" ,REFRESH_SELECTED);
		}
	}
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AuditCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/*void AuditPlanPrepreChkLst::OpenAuditCheckListScreen()
{
	CString strMainKeyFeild = FormGetField("HSE_ADTPLNENT" ,"MAINLINK");
	int nNumberOfRecords = getNumberOfRecord("HSE_ADTPLNENTACTVT" , strMainKeyFeild,"ADTPLNENTACTVT_LNK");
	CString strSQL ;

	//Audit Checklist without any record in the Activities screen shouldn't be opened
	if(nNumberOfRecords != 0)
	{
		//Response : "	Display the (Audit Checklist [@481]) pop-up screen"	Allow user to edit all records			
		CString strKeyField = FormGetField("HSE_ADTPLNENTACTVT","MAINLINK");	
		InitDefaultValue(1);
		InitScreenDefaultValue(0,"HSE_TGADTCHCKLSTPLN",1); // Fields Count in first Screen
		AddFieldDefaultValue(0,0,"ADTCHCKLSTPLN_LNK",strKeyField);
		strSQL.Format(" WHERE (ADTCHCKLSTPLN_LNK = %s)",strKeyField);
		ShowListScreen("HSE_TGADTCHCKLSTPLN","Audit Check List",strSQL,"","",TRUE,NULL,TRUE);		
	}
}*/

//Not Used
void AuditPlanPrepreChkLst::CompleteAuditWithAuditChkLst(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);
	CString strKey_FiledValB = FormGetField("HSE_ADTPLNENT","ADTPLNENT_ADTPLNSTS");
	UpdateStatus(pCustomButtonClicked->pMultiSelectedRows,8,"ADTPLNENT","MAINLINK");
	CString Mainlink = FormGetField("HSE_ADTPLNENT","MAINLINK");
	CString strSQL="Select ADTPLNENT_ADTPLNSTS From HSE_ADTPLNENT where MAINLINK = "+Mainlink+" ";
	CString strKey_FiledValAF=GetRecordSetData(strSQL,1);
	if(strKey_FiledValB != strKey_FiledValAF)
		InsertActionRecordIntoTracingTab("ADTPLNENT","MAINLINK",CHKLSTENTRY);
}

long AuditPlanPrepreChkLst::ShowAuditPlanPreparechecklist(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTPLNPRPERCHKLST;
	return ParentManage("HSE_TGADTPLNPRPRCHKLST",TRUE,TRUE,TRUE,"HSE_ADTPLNENT",strCriteria);
}