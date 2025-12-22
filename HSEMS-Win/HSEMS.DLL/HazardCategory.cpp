// HazardCategory.cpp: implementation of the HazardCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "HazardCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HazardCategory::HazardCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

HazardCategory::~HazardCategory()
{

}

HRESULT HazardCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	//polydb_task nRecordStatus = 0
	//if(pCustomButtonClicked-> nRecordStatus != 0)
	//{
	CString strBtnName=(pCustomButtonClicked->Button_Name);
	m_strScreenTag = pCustomButtonClicked->Form_Tag;	
	CString strMainKeyFeild = FormGetField("HSE_HZRD" ,"HZRD_HZRDGRP");	
	int nNumberOfRecords = getNumberOfRecord("HSE_HZRDHZRDCTG" , strMainKeyFeild,"HZRDHZRDCTG_LNK");
	CString strSQL ;
	if(nNumberOfRecords != 0)
	{

		//001_201~3_1
		//Action : Press Custom button
		//Condition : Custom button= Harm types[5]	
		if(strBtnName=="HZRDHZRDCTG_HRMTYP")
		{
			CString strKeyField = FormGetField("HSE_HZRDHZRDCTG","MAINLINK");
			//Response :Open Harm type Pop up screen[@070] filter on selected record 
			CString strDefValues;				
			strDefValues.Format("HRMTYP_LNK|%s",strKeyField);						
			strSQL.Format("SELECT * FROM HSE_HRMTYP WHERE ( HRMTYP_LNK = %s) ",strKeyField);			
			ShowScreen(0,"HSE_TGHRMTYP","Harm type",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);			
		} else if(strBtnName=="HZRDHZRDCTG_HSERGLTN") {
			CString strKeyField = FormGetField("HSE_HZRDHZRDCTG","MAINLINK");
			CString strDefValues;
			//Response : Open HSE Regulations Pop up screen[@280] filter on selected record
			strDefValues.Format("HSERGLR_LNK|%s",strKeyField);					
			strSQL.Format("SELECT * FROM HSE_HSERGLR WHERE ( HSERGLR_LNK = '%s') ",strKeyField);			
			ShowScreen(0,"HSE_TGHSERGLR","HSE Regulation",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);			
		}
	}	
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long HazardCategory::ShowHazard(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HZRD;
	return ParentManage("HSE_TGHZRD",TRUE,TRUE,TRUE,"HSE_HZRD",strCriteria);
}