// RiskAnalysis.cpp: implementation of the RiskAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RiskAnalysis.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RiskAnalysis::RiskAnalysis(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


RiskAnalysis::~RiskAnalysis()
{

}


HRESULT RiskAnalysis::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strField_Name =	pFieldChange->Field_Name;
	LONG nComplete = pFieldChange->iComplete;

	if(nComplete == 1) {
		//717-102-2
		if( strField_Name == "RSKANLYS_RNK2") {		 		
			chkRnkAftrPrfrmncMsrs(pFieldChange);
			RetriveRiskRankDes();
		}			
		else if(strField_Name == "RSKANLYS_RNK1") {
			RetriveRiskRankDes();
		}	
		else if(strField_Name =="CNTRLMSRS_HRCHYCTRL") {
			emptySubLevels(strField_Name);
		}
		else if(strField_Name == "CNTRLMSRS_GRP") {
			emptySubLevels(strField_Name);
		}				
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}


HRESULT RiskAnalysis::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName = pBeforBrowsingButtonClick->Field_Name;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;

	if(strSubFormTag == "") {
		//Rule ID : 717-102-1
		if(strFieldName == "RSKANLYS_SVRTYDSCRPTN" || strFieldName == "RSKANLYS_SVRTYDSCRPTN2") {		
			filterSvrtDscrOnRskCls(pBeforBrowsingButtonClick);	
		}
	} else {
		//Rule ID :717-1021-1
		if(strFieldName == "CNTRLMSRS_GRP") {
			fltrGrpOnHrchyCntrl(pBeforBrowsingButtonClick);
		}
		//Rule ID :717-1021-2
		else if(strFieldName == "CNTRLMSRS_CTRLMSRMNT") {
			fltrCntrlMsrOnGrp(pBeforBrowsingButtonClick);
		}	
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT RiskAnalysis::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strBrowsingField = pBrowsingSelectedItem->BrowsingField;
	CString strFullRecord    = pBrowsingSelectedItem->Full_Record;
	CString strReturnFld	 = pBrowsingSelectedItem->Return_Value;
	
	if(strBrowsingField == "RSKANLYS_SVRTYDSCRPTN") {		
		calcRnkBfrPrfrmncMsre("");
		FormSetField("HSE_RSKANLYS","RSKANLYS_SVRTYDSCRPTN2",strReturnFld);
		calcRnkAfrPrfrmncMsre("");
	}	
	else if(strBrowsingField == "RSKANLYS_LKLHODDSC") {
		calcRnkBfrPrfrmncMsre(strFullRecord);
		FormSetField("HSE_RSKANLYS","RSKANLYS_LKLHODDSC2",strReturnFld);
		calcRnkAfrPrfrmncMsre(strFullRecord);
	}
	else if(strBrowsingField == "RSKANLYS_SVRTYDSCRPTN2") {
		calcRnkAfrPrfrmncMsre("");
	}
	else if(strBrowsingField == "RSKANLYS_LKLHODDSC2") {
		calcRnkAfrPrfrmncMsre(strFullRecord);
	}
	/*
	else if(strBrowsingField =="CNTRLMSRS_HRCHYCTRL") {
		emptySubLevels(strBrowsingField);
	}
	else if(strBrowsingField == "CNTRLMSRS_GRP") {
		emptySubLevels(strBrowsingField);
	}
	*/
	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}

void RiskAnalysis::calcRnkBfrPrfrmncMsre(CString strFullRecord)
{
	int nLikelyHoodCode = 0;
	if(strFullRecord == "") {
		nLikelyHoodCode = atoi(FormGetField("HSE_RSKANLYS","ABC[RSKANLYS_LKLHODDSC=HSE_RSKLKLYHODRAT.RSKLKLYHODRAT_RAT][RSKLKLYHODRAT_RAT]"));
	} else {
		nLikelyHoodCode = atoi(GetFieldValue(strFullRecord,"RSKLKLYHODRAT_RAT"));
	}
	int nSeverityCode = atoi(FormGetField("HSE_RSKANLYS","ABC[RSKANLYS_SVRTYDSCRPTN=HSE_RSKSVRTYRAT.MAINLINK][RSKSVRTYRAT_RAT]"));
	int nRiskRankBeforeMeasure = nSeverityCode * nLikelyHoodCode;
	FormSetField("HSE_RSKANLYS","RSKANLYS_RNK1",itoa(nRiskRankBeforeMeasure));
}

void RiskAnalysis::calcRnkAfrPrfrmncMsre(CString strFullRecord)
{
	int nLikelyHoodCode = 0;
	if(strFullRecord == "") {
		nLikelyHoodCode = atoi(FormGetField("HSE_RSKANLYS","ABC[RSKANLYS_LKLHODDSC2=HSE_RSKLKLYHODRAT.RSKLKLYHODRAT_RAT][RSKLKLYHODRAT_RAT]"));
	} else {
		nLikelyHoodCode = atoi(GetFieldValue(strFullRecord,"RSKLKLYHODRAT_RAT"));
	}
	int nSeverityCode = atoi(FormGetField("HSE_RSKANLYS","ABC[RSKANLYS_SVRTYDSCRPTN2=HSE_RSKSVRTYRAT.MAINLINK][RSKSVRTYRAT_RAT]"));
	int nRiskRankAfterMeasure = nSeverityCode * nLikelyHoodCode;
	FormSetField("HSE_RSKANLYS","RSKANLYS_RNK2",itoa(nRiskRankAfterMeasure));
}


void RiskAnalysis::filterSvrtDscrOnRskCls(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)	
{
	CString strRskClss=FormGetField("HSE_RSKANLYS","RSKANLYS_RSKCLS");	
	CString strSQL;
	if(strRskClss.IsEmpty()) {
		strRskClss=" ";
	}
	strSQL.Format("WHERE (Link ='%s')",strRskClss);
	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
}

void RiskAnalysis::chkRnkAftrPrfrmncMsrs(FIELDCHANGE *pFieldChange)
{
	CString strRnkAftrPrfrmncMsur = FormGetField("HSE_RSKANLYS","RSKANLYS_RNK2"); 
	CString strRnkBfrPrfrmncMsur  = FormGetField("HSE_RSKANLYS","RSKANLYS_RNK1");

	if(atoi(strRnkAftrPrfrmncMsur) > atoi(strRnkBfrPrfrmncMsur)) {
			*pFieldChange->iCancle = 1;
			ShowMessageEx(IDS_Rnk,"",IV_NOICON,"Error");
			FormSetField("HSE_RSKANLYS","RSKANLYS_SVRTYDSCRPTN2","");
			FormSetField("HSE_RSKANLYS","ABC[RSKANLYS_SVRTYDSCRPTN2=HSE_RSKSVRTYRAT.MAINLINK][RSKSVRTYRAT_RAT]","");
			FormSetField("HSE_RSKANLYS","RSKANLYS_LKLHODDSC2","");
			FormSetField("HSE_RSKANLYS","ABC[RSKANLYS_LKLHODDSC2=HSE_RSKLKLYHODRAT.RSKLKLYHODRAT_RAT][RSKLKLYHODRAT_RAT]","");
			FormSetField("HSE_RSKANLYS","RSKANLYS_RNK2","");			
	}	

}

void RiskAnalysis::RetriveRiskRankDes()
{
	CString strRiskRank1=FormGetField("HSE_RSKANLYS","RSKANLYS_RNK1");
	CString strRiskRank=FormGetField("HSE_RSKANLYS","RSKANLYS_RNK2");
	if(strRiskRank1 != "")
	{
		CString strSQL1="Select RSKRNKDESC_DESC From HSE_RSKRNKDESC Where RSKRNKDESC_RNK="+strRiskRank1;
		CString strRiskRankDesc1=GetRecordSetData(strSQL1,1);
		FormSetField("HSE_RSKANLYS","RSKANLYS_RNKDSC1",strRiskRankDesc1);
	}
	if(strRiskRank != "")
	{
		CString strSQL="Select RSKRNKDESC_DESC From HSE_RSKRNKDESC Where RSKRNKDESC_RNK="+strRiskRank;
		CString strRiskRankDesc=GetRecordSetData(strSQL,1);
		FormSetField("HSE_RSKANLYS","RSKANLYS_RNKDSC2",strRiskRankDesc);
	}
}

CString RiskAnalysis:: OpenNcsryCntrlMsure(CString strFormTag)
{
	CString strDefValues;
	bool bLocked = true;
	CString strKeyField = FormGetField("HSE_RSKANLYS" ,"MAINLINK");		
	CString strSQL ;	
	if(strKeyField == "")
		return "";
	/*InitDefaultValue(1); // Screen Count
	InitScreenDefaultValue(0,"HSE_TGNCSSRYCTRLMSRMNT",1); // Fields Count in first Screen
	AddFieldDefaultValue(0,0,"NCSSRYCTRLMSRMNT_LNK",strKeyField);*/
	strDefValues.Format("NCSSRYCTRLMSRMNT_LNK|%s",strKeyField);
	strSQL.Format("SELECT * FROM HSE_NCSSRYCTRLMSRMNT WHERE (NCSSRYCTRLMSRMNT_LNK = %s)",strKeyField);
	m_strScreenTag = "HSE_TGNCSSRYCTRLMSRMNT";
	//OpenScreen("Necessary Control Measures",strSQL,ID_HSE_NCSSRYCTRLMSRMNT,0,1);
	if(strFormTag =="HSE_TGRSKANLYS") {
		bLocked = false;
	}		
	ShowScreen(0,"HSE_TGNCSSRYCTRLMSRMNT","Necessary Control Measures",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
	/*if(strFormTag !="HSE_TGRSKANLYS")
		ShowListScreen("HSE_TGNCSSRYCTRLMSRMNT","Necessary Control Measures",strSQL,"","",TRUE,NULL,FALSE,"LOCKED");
	else
		ShowListScreen("HSE_TGNCSSRYCTRLMSRMNT","Necessary Control Measures",strSQL,"","",TRUE,NULL,TRUE);
	*/	
	return strKeyField;
}


//Filter group on Hierarchy control
void RiskAnalysis::fltrGrpOnHrchyCntrl(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strHrchyCtrl = FormGetField("HSE_CNTRLMSRS","CNTRLMSRS_HRCHYCTRL");
	if(strHrchyCtrl.IsEmpty()) {
		strHrchyCtrl=" ";
	}
	CString strSQL;
	strSQL.Format("WHERE (CTRLMSRMNT_HRCHYCTRL='%s')",strHrchyCtrl);
	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 
}

//Filter control measure field according to group
void RiskAnalysis::fltrCntrlMsrOnGrp(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strCtrlMsrmnt = FormGetField("HSE_CNTRLMSRS","CNTRLMSRS_GRP");
	if(strCtrlMsrmnt.IsEmpty()) {
		strCtrlMsrmnt=" ";
	}
	CString strSQL;
	strSQL.Format("WHERE (CTRLMSRMNTCTRLMSRMNT_LNK ='%s')",strCtrlMsrmnt);
	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
}


//Empty sub Fields if main field change
void RiskAnalysis::emptySubLevels(CString strFieldName)
{
	if(strFieldName == "CNTRLMSRS_HRCHYCTRL") {
		FormSetField("HSE_CNTRLMSRS","CNTRLMSRS_GRP","");
	}
	FormSetField("HSE_CNTRLMSRS","CNTRLMSRS_CTRLMSRMNT","");		
}

long RiskAnalysis::ShowRiskAnalysis(CString strCriteria)
{
	m_nCurrentCard=ID_RISKANALYSIS;
	return ParentManage("HSE_TGRSKANLYS",TRUE,TRUE,TRUE,"HSE_RSKANLYS",strCriteria);
}