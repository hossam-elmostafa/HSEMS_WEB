// PerformanceMeasurementEntryCategory.cpp: implementation of the PerformanceMeasurementEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PerformanceMeasurementEntryCategory.h"
//polydb_task , why this file include resource file in HSE
//#include ".\\HSE_RC_EN\\resource.h"  

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
PerformanceMeasurementEntryCategory::PerformanceMeasurementEntryCategory(CCategory *pCategory):PerformanceMeasurementCategory(pCategory)
{
	m_pCategory = pCategory;
}
PerformanceMeasurementEntryCategory::~PerformanceMeasurementEntryCategory(){}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
 	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	if(strFieldName=="PERFMSRMNTENTVSTTM_COMPEMP")
	{
		//404_30211_1
		//Change Field Value
		//Allow user either to feed the company employee information [4], [5], [6], [7] 
		//or non-company employee information [9], [10], [11], [12] but not both		
 		//CString strCompEmp=FormGetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_COMPEMP");
		CString strCompEmp(pFieldChange->New_Value);
		CString strEmp=FormGetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_EMPNUM");
		if(strCompEmp=="0")
		{		
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_EMPNUM"," ");					
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_Dprt34"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_JobTtl44"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","Emp_Nam24"," ");
		}
		if(strCompEmp=="1")
		{		
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_NAM"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_COMP"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_DPRT"," ");
			FormSetField("HSE_PERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_JOBTTL"," ");
		}		
	}	
	if(strFieldName =="PERFMSRMNTENTVSTCHCKLST_GRP") {
		FormSetField("HSE_PERFMSRMNTENTVSTCHCKLST","PERFMSRMNTENTVSTCHCKLST_SECND","");
	}
	return PerformanceMeasurementCategory::DisplayFieldChange (pFieldChange);
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{			
	CString strBtnName(pCustomButtonClicked->Button_Name);
	if(strBtnName=="PERFMSRMNTENT_ENTCMPLT")			//301_340_1:Press entry complete button
	{
		int Count = 0;		
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}
		bool bprfmncMsrmntCmplt = CompletePerformanceMeasure(pCustomButtonClicked);
		if(bprfmncMsrmntCmplt)
			RefreshScreen("",REFRESH_SELECTED);
	} 
	return PerformanceMeasurementCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

/***********************************************************************************************************/
HRESULT PerformanceMeasurementEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(strBtnName=="SAVE" && pToolBarButtonClicked->iComplete == 0) {
		GenerateTXNNo(pToolBarButtonClicked->Form_Tag);	
	}
	if(CString(pToolBarButtonClicked->SubForm_Tag) == "HSE_TGPERFMSRMNTENTVSTTM") {
		if(strBtnName=="CANCEL" && pToolBarButtonClicked->iComplete == 0)
		{
			SetFieldVisibleAttrib("HSE_TGPERFMSRMNTENTVSTTM","PERFMSRMNTENTVSTTM_EMPNUM","NULL");
		}	
		else if(strBtnName=="SAVE" && pToolBarButtonClicked->iComplete == 1)
		{
			RefreshScreen("HSE_TGPERFMSRMNTENTVSTTM",REFRESH_SELECTED);
		}
	}
	else if(strBtnName=="SAVE" && pToolBarButtonClicked->iComplete == 1 )
	{
		RefreshScreen("",REFRESH_SELECTED);		
	}
	return PerformanceMeasurementCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/***********************************************************************************************************/
HRESULT PerformanceMeasurementEntryCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);
	if(strSubFormTag == "HSE_TGPERFMSRMNTENTSECNDEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTENTSECNDEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTENTSECNDEVAL",TOOLBAR_DELETE,FALSE);
	}
	if(strSubFormTag =="HSE_TGPERFMSRMNTENTGRPEVAL")
	{
		EnableToolbar("HSE_TGPERFMSRMNTENTGRPEVAL",TOOLBAR_NEW,FALSE);
		EnableToolbar("HSE_TGPERFMSRMNTENTGRPEVAL",TOOLBAR_DELETE,FALSE);
	} 	
	if(strSubFormTag == "HSE_TGPERFMSRMNTENTSECNDEVAL")
	{
		RefreshScreen("HSE_TGPERFMSRMNTENTSECNDEVAL",0);		
	}
	if(strSubFormTag=="HSE_TGPERFMSRMNTENTGRPEVAL")
	{		
		RefreshScreen("HSE_TGPERFMSRMNTENTGRPEVAL",0);
	}
	return PerformanceMeasurementCategory::DisplayTabSelected(pTabSelected);
}

/***********************************************************************************************************/
HRESULT PerformanceMeasurementEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	//301_340_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="PERFMSRMNTENT_LOCTYP")
	{
		CString strLocTyp=FormGetField("HSE_PERFMSRMNTENT","PERFMSRMNTENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//301_340_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="PERFMSRMNTENT_WRKLOC")
	{
		CString strGrp=FormGetField("HSE_PERFMSRMNTENT","PERFMSRMNTENT_LOCTYP");
		if(strGrp.IsEmpty())
			strGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	//TODO : this field is not exist check it in sql server appmgr
	else if(strFieldName=="PERFMSRMNTENTATCH_SUBCLS")
	{
		CString strCls=FormGetField("HSE_PERFMSRMNTENTATCH","PERFMSRMNTENTATCH_CLS");
		if(strCls.IsEmpty())
			strCls=" ";
		CString strSQL;
		strSQL.Format("WHERE (ATCHCLSFCDTL_LNK ='%s')",strCls);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	// Rule id : 301-341-1
	// done by : doaa desouky
	// date : 07/06/2010
	// tab component : Performance Measurement Entry - Visit Checklist (Tab) - component No. (341)
	// Req. description : Browse on Section filter on the selection on group field 
	else if(strFieldName=="PERFMSRMNTENTVSTCHCKLST_SECND")
	{		
		SelectSectionFilteredSelectedGroup(pBeforBrowsingButtonClick);
	}
	return PerformanceMeasurementCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
/***********************************************************************************************************
Helper function
***********************************************************************************************************/
void PerformanceMeasurementEntryCategory::SelectSectionFilteredSelectedGroup(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSelectedGroup=FormGetField("HSE_PERFMSRMNTENTVSTCHCKLST","PERFMSRMNTENTVSTCHCKLST_GRP");
	if(strSelectedGroup.IsEmpty())
		strSelectedGroup=" ";
	CString strSQL;
	strSQL.Format("WHERE (perfMsrmntChkListSecnd_LNK ='%s')",strSelectedGroup);
	pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
}

bool PerformanceMeasurementEntryCategory::CompletePerformanceMeasure(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	bool bprfmncMsrmntCmplt = false;
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE completePrfrmncMsrmntTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	long lRetVal = ExecuteSQL(strSQL);
	if(lRetVal != 0)
		bprfmncMsrmntCmplt = true;
	return bprfmncMsrmntCmplt;
}

/*Commented_Function: retrievSiteType()*/

long PerformanceMeasurementEntryCategory::ShowPerformanceMeasurementEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTENT;
	return ParentManage("HSE_TGPERFMSRMNTENT",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTENT",strCriteria);
}