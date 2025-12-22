/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *							This class handle Control Measures Screen
 *
 * Because of there is no separate class for the HierarchyControl Screen,
 * I have created a class for it, for handling the new added custom button,
 * This is done according to the Change Request(HSE_CR12v2_8)
 * Done By:Mai Talaat on 21/3/2013*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "HierarchyControl.h"

CHierarchyControl::CHierarchyControl(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

CHierarchyControl::~CHierarchyControl(){}

HRESULT CHierarchyControl::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	/**
	 * Button removed 'by a.ali according to Mr Sameer HSE Comments1 2017-03-09.xlsx'
	 *
	 *Rule ID :66_30_1
	 *Rule Description : Open the pop-up screen related to the (Control Measures) custom button
	 *According to Change Request(HSE_CR12v2_8)
	 *Done By:Mai Talaat
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strScreenTag(pCustomButtonClicked->Form_Tag);	
	CString strSQL ;
	//if the clicked custom button is [3@46105](Detail Control Measure)	
	if(strScreenTag == "HSE_TGHRCTRL" && strBtnName =="HRCTRL_DTLCTRLMSR") 
	{
		CString strDefValues;
		CString strMainKeyField=FormGetField("HSE_HRCTRL","HRCTRL_COD");//[1@46105](Hierarchy Control Code)
		//Initiate the default value for the (2)pop-up screen [30~1](Detail Control Measures)
		strDefValues.Format("CTRLMSRMNT_HRCHYCTRL|%s",strMainKeyField);		
		strSQL.Format("SELECT * FROM HSE_CTRLMSRMNT WHERE (CTRLMSRMNT_HRCHYCTRL='%s')", strMainKeyField);			
		ShowScreen(0,"HSE_TGCTRLMSRMNT","Detail Control Measures",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
	}*/
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

long CHierarchyControl::ShowControlMeasures(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CTRLMSRMNT;
	return ParentManage("HSE_TGHRCTRL",TRUE,TRUE,TRUE,"HSE_HRCTRL",strCriteria);
}