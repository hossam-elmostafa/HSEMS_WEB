// NecessaryControlMeasures.cpp: implementation of the NecessaryControlMeasures class.
//
//	Handle pop-up screen opened from Risk Assessment Entry(Screen) -> Steps(tab)
//  When user click on Control Measures custom button
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "NecessaryControlMeasures.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NecessaryControlMeasures::NecessaryControlMeasures(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

NecessaryControlMeasures::~NecessaryControlMeasures(){}

HRESULT NecessaryControlMeasures::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//Filter control measure browsing field on Group field
	CString strSQL;
	CString strFormTag     = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag  = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName   = pBeforBrowsingButtonClick->Field_Name;
	CString strFullRecored = pBeforBrowsingButtonClick->Full_Record;
	strFieldName.MakeUpper();
	/*
	 * Field deleted from screen
	if (strFieldName=="NCSSRYCTRLMSRMNT_CTRLMSRMNT")
	{
		CString strGroup=FormGetField("HSE_NCSSRYCTRLMSRMNT","NCSSRYCTRLMSRMNT_GRP");
		CString strSql;
		strSql.Format("WHERE (CTRLMSRMNTCTRLMSRMNT_LNK ='%s')",strGroup);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSql.AllocSysString(); 		
	}*/	
	if(strFieldName == "NCSSRYCTRLMSRMNT_DTLNAM") {
		CString strGrpCod = FormGetField("HSE_NCSSRYCTRLMSRMNT","NCSSRYCTRLMSRMNT_CMCODE");
		if(strGrpCod.IsEmpty()) {
			strGrpCod = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (HRCTRLDTL_LNK = '%s')",strGrpCod);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
HRESULT NecessaryControlMeasures::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString FieldName = pFieldChange->Field_Name;
	/*Removed Code,Please Check Source Safe*/

	/*
	 * Field removed from Screen
	if(FieldName == "NCSSRYCTRLMSRMNT_GRP")
	{
		FormSetField("HSE_NCSSRYCTRLMSRMNT","NCSSRYCTRLMSRMNT_CTRLMSRMNT","");
		FormSetField("HSE_NCSSRYCTRLMSRMNT","ABC[NCSSRYCTRLMSRMNT_CTRLMSRMNT=HSE_CTRLMSRMNTCTRLMSRMNT.MAINLINK][CTRLMSRMNTCTRLMSRMNT_DESC]","");
	}*/
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

/*Removed Code,Please Check Source Safe*/

long NecessaryControlMeasures::ShowNecessaryControlMeasures(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_NCSSRYCTRLMSRMNT;
	return ParentManage("HSE_TGNCSSRYCTRLMSRMNT",TRUE,TRUE,TRUE,"HSE_NCSSRYCTRLMSRMNT",strCriteria);
}