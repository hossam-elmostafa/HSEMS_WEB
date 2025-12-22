// arEntryCategory.cpp: implementation of the CarEntryCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "CAREntryCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CarEntryCategory::CarEntryCategory(CCategory *pCategory):CarCategory(pCategory)
{
	m_pCategory = pCategory;
	m_FSATableName = "HSE_TGCRCTVEACCENT";
	m_strScreen	   = "Corrective Action Entry";
}

CarEntryCategory::~CarEntryCategory()
{

}
/*******************************************************************************************************************/
HRESULT CarEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//701_430_1
	//Framework Event : Press custom button
	//Condition : button Pressed is (Entry Completed)[24] 
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="CRCTVEACCENT_ENTCMPLT")
	{
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;

		if( Count == 0)
		{
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CompleteCorrectiveActionEntry(pCustomButtonClicked);
		RefreshScreen("",REFRESH_SELECTED);
	}
	//HSEMS New requirements_27_10_2020  3
	else if(strBtnName=="CRCTVEACCENT_CNCL")
	{
		if(OpenReasonsScr("Cancel Reasons","3",true)){
			CancelCorrectiveAction(pCustomButtonClicked);
			RefreshScreen("",REFRESH_SELECTED);
		}
	}
	return CarCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*******************************************************************************************************************/

// Requirement 1.n.i 
// this function check if the user enter Audit year , it will make audit number enabled
HRESULT CarEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFormTag(pFieldChange->Form_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	CString strCarBasDesc ,strCarBasDept,strCarBasLoc,strCarBasSubLoc,
			strCarBasSit,strCarBasArea , strTableName,strKeyFldName,strKeyFldVal;
	CString CarBasValue = pFieldChange->New_Value;	
	//Done by Doaa Desouky
	//According to change request 06
	//"	Fill Field Case descreption[18@430],
	//Location frame: Departement [7@430] , location [8@430] , sub location [9@430], Site [10@430] , Area [11@430]with the location of selected car Basis 
	//and if car basis not have department don't fill with value
	//Review Rule id :car entry-> 701_403_6			
	if(strFieldName == "CRCTVEACCENT_NRMSNUM") {
		strCarBasDesc   = "NRSTMISCENT_NRSTMISCDESC";
		strCarBasDept   = "NRSTMISCENT_CNCDPRT";
		strCarBasLoc    = "NRSTMISCENT_LOCTYP";
		strCarBasSubLoc = "NRSTMISCENT_WRKLOC";
		strCarBasSit    = "NRSTMISCENT_SIT";
		strCarBasArea   = "NRSTMISCENT_AREA";
		strTableName    = "HSE_NRSTMISCENT";
		strKeyFldName   = "NRSTMISCENT_NRSTMISCNUM";
		strKeyFldVal    = CarBasValue;
		FillCarDescAndLoc(strCarBasDesc,strCarBasDept,strCarBasLoc,strCarBasSubLoc,strCarBasSit,strCarBasArea,strTableName,strKeyFldName ,CarBasValue);
	} else if(strFieldName == "CRCTVEACCENT_ACCDNTNUM") {
		strCarBasDesc   = "ACDNTENT_ACDNTDESC";
		strCarBasDept   = "";
		strCarBasLoc    = "ACDNTENT_LOCTYP";
		strCarBasSubLoc = "ACDNTENT_WRKLOC";
		strCarBasSit    = "ACDNTENT_SIT";
		strCarBasArea   = "ACDNTENT_AREA";
		strTableName    = "HSE_ACDNTENT";
		strKeyFldName   = "ACDNTENT_ACDNTNUM";
		strKeyFldVal    = CarBasValue;
		FillCarDescAndLoc(strCarBasDesc,strCarBasDept,strCarBasLoc,strCarBasSubLoc,strCarBasSit,strCarBasArea,strTableName,strKeyFldName ,CarBasValue);
	} else if(strFieldName == "CRCTVEACCENT_PTNHZRDNUM") {
		strCarBasDesc   = "PTNLHZRDENT_HZRDDESC";
		strCarBasDept   = "PTNLHZRDENT_CNCDPRT";
		strCarBasLoc    = "PTNLHZRDENT_LOCTYP";
		strCarBasSubLoc = "PTNLHZRDENT_WRKLOC";
		strCarBasSit    = "PTNLHZRDENT_SIT";
		strCarBasArea   = "PTNLHZRDENT_AREA";
		strTableName    = "HSE_PTNLHZRDENT";
		strKeyFldName   = "PTNLHZRDENT_PTNLHZRDNO";
		strKeyFldVal    = CarBasValue;
		FillCarDescAndLoc(strCarBasDesc,strCarBasDept,strCarBasLoc,strCarBasSubLoc,strCarBasSit,strCarBasArea,strTableName,strKeyFldName ,CarBasValue);
	} 	
	else if(strFieldName == "CRCTVEACCENT_CARMODELBAS") {		
		CString strOldValue(pFieldChange->Old_Value);
		int iComplete = pFieldChange->iComplete;
		if(strOldValue != "" && iComplete == 0) {
			emptyRelatedTXNFlds();					
		}
	}  			
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT CarEntryCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	return CHSEMSCommonCategory::DisplayTabSelected (pTabSelected);
}

HRESULT CarEntryCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	return CarCategory::DisplayScreenReady(pScreenReady);
}
/*******************************************************************************************************************/
HRESULT CarEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;			
	//Ref 701_431_1
	//Browse on sub detail acoording to the selection in detail
	if(strFieldName=="CRCTVEACCENTROOTCUSE_DTLCOD")
	{
		CString strGroup=FormGetField("HSE_CRCTVEACCENTROOTCUSE","CRCTVEACCENTROOTCUSE_GRP");								
		if(strGroup.IsEmpty())
			strGroup=" ";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strGroup);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}	
	//Ref 701_431_2
	//Browse on Detail Code acoording to the selection in Group
	if(strFieldName=="CRCTVEACCENTROOTCUSE_SUBDTL")
	{
		CString strDtl=FormGetField("HSE_CRCTVEACCENTROOTCUSE","CRCTVEACCENTROOTCUSE_DTLCOD");								
		if(strDtl.IsEmpty())
			strDtl=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strDtl);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//701_430_2
    //Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	else if(strFieldName=="CRCTVEACCENT_LOCTYP")
	{
		CString strLocTyp=FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	//701_430_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="CRCTVEACCENT_WRKLOC")
	{
		CString strGrp=FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_LOCTYP");
		if(strGrp.IsEmpty())
			strGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	return CarCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}
/*******************************************************************************************************************/
HRESULT CarEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/******************************************************************************************************************/
HRESULT CarEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{	
	if(pRecordRepostion->iComplete==0) {
		//HSEMS New requirements_27_10_2020 , -The system should prevent deletion of record with status other than incomplete 
		CString strState = FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_RECSTS");
		if(strState != "1") {
			EnableToolbar("",TOOLBAR_DELETE,FALSE);
		}
	}	
	return CarCategory::DisplayRecordRepostion(pRecordRepostion);
}

/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
CString CarEntryCategory::GetAuditNumbersFilteredOnAuditYear()
{
		
	CString strAuditYR=FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_ADTYR");
	CString strCriteria;
	CString strSQL;
	if(strAuditYR.IsEmpty())
		strAuditYR=" ";
	strSQL.Format("WHERE ((ADTPLNENT_ADTPLNSTS=9 or ADTPLNENT_ADTPLNSTS = 8 )And ADTPLNENT_ADTYR ='%s')",strAuditYR);
	return strSQL;
}

/*******************************************************************************************************************
Helper function
*******************************************************************************************************************/
void CarEntryCategory::CompleteCorrectiveActionEntry(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE completeCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}

void CarEntryCategory::CancelCorrectiveActionEntry(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	setOwnerScreenTag(m_strScreenTag);	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);	
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	strSQL.Format("EXECUTE CancelCARTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);	
}

void CarEntryCategory::FillCarDescAndLoc(CString strCarBasDesc,CString strCarBasDept,CString strCarBasLoc,CString strCarBasSubLoc,CString strCarBasSit,CString strCarBasArea,CString strTableName,CString strKeyFldName,CString strKeyFldVal)
{
	CString strSQL;
	CString selectedColoumns;
	int i = 0;
	char** arrCarBasisInfo = new char*[5];	
	if(strCarBasDesc != "")
		selectedColoumns = strCarBasDesc+" AS CARDESC ,";
	else
		arrCarBasisInfo[0] = " ";
	if(strCarBasDept !="")
		selectedColoumns +=strCarBasDept+" AS CARDEPT , ";
	else
		arrCarBasisInfo[1] = " ";
	if(strCarBasLoc !="")
		selectedColoumns +=strCarBasLoc+",";
	else
		arrCarBasisInfo[2] = " ";
	if(strCarBasSubLoc !="")
		selectedColoumns +=strCarBasSubLoc+",";
	else
		arrCarBasisInfo[3] = " ";
	if(strCarBasSit !="")
		selectedColoumns +=strCarBasSit+",";
	else 
		arrCarBasisInfo[4] = " ";
	if(strCarBasArea !="")
		selectedColoumns +=strCarBasArea;
	else
		arrCarBasisInfo[5] = " ";
	strSQL="SELECT "+selectedColoumns+" FROM "+strTableName+" WHERE "+strKeyFldName+" = '"+strKeyFldVal+"'" ;
	CString SlctdCarBasInfo=GetRecordSetData(strSQL,1);
	char* RtrndInfo = strtok(SlctdCarBasInfo.GetBuffer (NULL),"\1");
	for(int j =0 ; j<6 ; j++)
	{
		if(arrCarBasisInfo[i] ==" ")
		{
			i++;
			arrCarBasisInfo[i] = RtrndInfo;
			RtrndInfo = strtok(NULL,"\1");
		} else {
			arrCarBasisInfo[i] = RtrndInfo;
			RtrndInfo = strtok(NULL,"\1");
		}	
		i++;		
	}
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_CASESDESC" , arrCarBasisInfo[0]);
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_DPRT"	   , arrCarBasisInfo[1]);
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_SIT"       , arrCarBasisInfo[4]);
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_AREA"      , arrCarBasisInfo[5]);
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_LOCTYP"    , arrCarBasisInfo[2]);
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_WRKLOC"    , arrCarBasisInfo[3]);
}

long CarEntryCategory::ShowCorrectiveActionEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CRCTVEACCENT;
	return ParentManage("HSE_TGCRCTVEACCENT",TRUE,TRUE,TRUE,"HSE_CRCTVEACCENT",strCriteria);
}

void CarEntryCategory::emptyRelatedTXNFlds() {
	//CAR Basis[Audit(1)|Near Miss(2)|Accident (3)|Potential(4)|General(5)]	
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_CASESDESC" , "");		
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_DPRT" , "");
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_LOCTYP" , "");
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_WRKLOC" ,"");
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_SIT" , "");
	FormSetField("HSE_CRCTVEACCENT" , "CRCTVEACCENT_AREA" , "");
}