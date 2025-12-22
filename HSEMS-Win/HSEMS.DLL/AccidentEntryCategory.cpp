// AccidentEntryCategory2.cpp: implementation of the AccidentEntryCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMS.h"
#include "AccidentEntryCategory.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*AccidentEntryCategory::AccidentEntryCategory()
{

}*/

AccidentEntryCategory::~AccidentEntryCategory()
{
}
//404_400_1
/*************************************************************************************************************/
HRESULT AccidentEntryCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//Press (Entry Completed) button
	//"	If (No. of injured persons [33]) <> number or records 
	//inserted in (injured persons [@405] tab) then display an error message and stop
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	if(strBtnName=="ACDNTENT_ENTCMPLT") {
		int Count = 0;
		if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
		if( Count == 0) {
			AfxMessageBox("You must save the Record first");
			return false;
		}
		CString strKey = FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM");
		if(isValidForEntryComplete(strKey) == 'N') {		
			///////////////////////////////////////////////////////////////////////////////////////////////
			//By Hossam 23/10/2021
			//Bugs-10-2021.55
			//The message "Some causes don't have recommended actions, proceed" is not correct
			//It should be "Some recommended actions don't Linked Cause , proceed" since it depends on the field 'Linked Causes' @Recommeneded actions tab
			int nRet = MessageBox(NULL,"Some Recommended Actions don't have Linked Cause , proceed ?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
			///////////////////////////////////////////////////////////////////////////////////////////////
			if(nRet == IDYES) {
				CompleteAccident(pCustomButtonClicked);
			}			
		} else {
			CompleteAccident(pCustomButtonClicked);
		}
	}
	//404_405_1
	//Action : Press (injured body parts [13] buttom
	//A pop-up screen [@070~7] should be displayed, filled with all records from "body parts" [@800]
	//Let the user mark/check the required rows	
	//else if(pCustomButtonClicked-> nRecordStatus != 0 && strBtnName=="ACDNTENTINJRDPRSN_INJRDBODYPART")
	else if(strBtnName=="ACDNTENTINJRDPRSN_INJRDBODYPART") {	
		CString strMainKeyFeild = FormGetField("HSE_ACDNTENT" ,"ACDNTENT_ACDNTNUM");	
		int nNumberOfRecords = getNumberOfRecord("HSE_ACDNTENTINJRDPRSN" , strMainKeyFeild,"ACDNTENTINJRDPRSN_LNK");	
		//404_405_3
		//injured body parts without any record in the Injured person screen shouldn't be opened
		if(nNumberOfRecords != 0) {
			OpenBodyParts("HSE_TGACDNTENT");
		}		
	}
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	return AccidentCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*************************************************************************************************************/
HRESULT AccidentEntryCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strFieldName = pFieldChange ->Field_Name;
	//Field deleted
	/*if(strFieldName=="ACDNTENT_CHKOTHRLOC")
	{
 		CString strOtherLoc=FormGetField("HSE_ACDNTENT","ACDNTENT_CHKOTHRLOC");
		if(strOtherLoc=="1") {		
			FormSetField("HSE_ACDNTENT" , "ACDNTENT_SIT" ," ");
			FormSetField("HSE_ACDNTENT" , "ACDNTENT_AREA" ," ");
			FormSetField("HSE_ACDNTENT" , "ACDNTENT_LOCTYP" ," ");			
		} 
		//Field deleted "ACDNTENT_OTHRLOC"
		//else {
		//	FormSetField("HSE_ACDNTENT" , "ACDNTENT_OTHRLOC" ," ");;
		//}
	}*/
	return AccidentCategory::DisplayFieldChange (pFieldChange);
}
/*************************************************************************************************************/
HRESULT AccidentEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{	
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strBtnName(pToolBarButtonClicked->Button_Name);	
	
	/*if(strBtnName=="NEW")
	{
		//Check if incident flash report requires		
		CString strSiteCode = FormGetField("HSE_ACDNTENT","ACDNTENT_SIT");
		CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
		CString strFlashRprtRqrd = isFlashRprtRqrd(strCmpnyNam);
		if(strFlashRprtRqrd == "1") {
			SetFieldVisibleAttrib(strFormTag,"ACDNTENT_TEMPFLSHNO","MUST");
			SetFieldVisibleAttrib(strFormTag,"ACDNTENT_Sit","ALWAYSLOCKED");
			SetFieldVisibleAttrib(strFormTag,"ACDNTENT_YR","ALWAYSLOCKED");			
		}
	}*/

	/*if(strFormTag=="HSE_TGACDNTENT" && strBtnName=="SAVE" )
	{
		CString strFullRecord = pToolBarButtonClicked->Full_Record;			
		CString strMainLink   = GetFieldValue(strFullRecord,"MAINLINK");		
		CString strClauseName  = GetFieldValue(strFullRecord,"ADTPLNENTACTVT_CLSNAM");		
		
		CString strSql;
		strSql.Format("INSERT INTO [dbo].[HSE_ADTCHCKLSTPLN] ([ADTCHCKLSTPLN_TSKNUM],[ADTCHCKLSTPLN_PLN],[ADTCHCKLSTPLN_ADTPNT],[ADTCHCKLSTPLN_DSCRPTN] \
           ,[ADTCHCKLSTPLN_REFNUM],[ADTCHCKLSTPLN_ADTLOC],[ADTCHCKLSTPLN_PLNDAT],[ADTCHCKLSTPLN_PLNFRM],[ADTCHCKLSTPLN_PLNTO] \
           ,[ADTCHCKLSTPLN_RMRK],[ADTCHCKLSTPLN_LNK],[ADTCHCKLSTPLN_ACTDAT],[ADTCHCKLSTPLN_FRMTIM],[ADTCHCKLSTPLN_TOTIM] \
           ,[ADTCHCKLSTPLN_ADTRSLT],[ADTCHCKLSTPLN_FNDNG],[ADTCHCKLSTPLN_CUSROT],[ADTCHCKLSTPLN_CMNTS],[ADTCHCKLSTPLN_TSKSTS] \
           ,[ADTCHCKLSTPLN_NCNFRMTRNK],[AdtchckLstPln_Ref]) \
		SELECT LINENUM,1,NULL,DESCRPTN,NULL,NULL,NULL,NULL,NULL,NULL,%s,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL \
		FROM HSE_ADTCHKLST	WHERE ADTCHKLST_LNK = %s",strMainLink,strClauseName);
		ExecuteSQL(strSql);

	}*/
	return AccidentCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
/*************************************************************************************************************/
HRESULT AccidentEntryCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	/*
	// Bugs-10-2021.19
	// tHE FUNCTION SHOULD BE CARRIED OUT BY POLYDB FROM FIRSTSYS
	if(strFieldName=="ACDNTENTACDNTRSN_DTLCOD")
	{
		CString strLocTyp=FormGetField("HSE_ACDNTENTACDNTRSN","ACDNTENTACDNTRSN_GRP");								
		if(strLocTyp.IsEmpty())
			strLocTyp="		";
		CString strSQL;
		strSQL.Format("WHERE (ACDNTRSNDTL_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	if(strFieldName=="ACDNTENTACDNTRSN_SUBDTL")
	{
		CString strSubDtl=FormGetField("HSE_ACDNTENTACDNTRSN","ACDNTENTACDNTRSN_DTLCOD");								
		if(strSubDtl.IsEmpty())
			strSubDtl=" ";
		CString strSQL;
		strSQL.Format("WHERE (LINK ='%s')",strSubDtl);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}
	*/
     //404_400_2
	//Browse on second level acoording to the selection in first level
	/*Browse (Location type) then Filter all existing record acoording to 
	  the selection in first level(SITE) */
	if(strFieldName=="ACDNTENT_LOCTYP") {
		CString strLocTyp=FormGetField("HSE_ACDNTENT","ACDNTENT_SIT");								
		if(strLocTyp.IsEmpty())
			strLocTyp=" ";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	//404_400_3
	//Browse on Third level acoording to the selection in second level
	/* Browse (Work location) then Filter all existing record acoording to 
	   the selection in second level(Location type)*/
	else if(strFieldName=="ACDNTENT_WRKLOC") {
		CString strLocTyp=FormGetField("HSE_ACDNTENT","ACDNTENT_LOCTYP");								
		if(strLocTyp.IsEmpty())
			strLocTyp="";
		CString strSQL;
		strSQL.Format("WHERE (WRKLOCWRKLOC_LNK ='%s')",strLocTyp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
	return AccidentCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}


HRESULT AccidentEntryCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strFieldName  = pBrowsingSelectedItem->BrowsingField;	
	strFieldName.MakeUpper();
	if(strFieldName == "ACDNTENT_TEMPFLSHNO"){
		CString strFullRecord = pBrowsingSelectedItem->Full_Record;			
		CString strMainLink   = GetFieldValue(strFullRecord,"MAINLINK");		
		//TODO : this code must be removed after solving Polydb problem
		/*CString strSite = GetFieldValue(strFullRecord,"INCDNTFLSHRPRT_SIT_UKF");
		FormSetField("HSE_ACDNTENT","ACDNTENT_ACDNTDAT",GetFieldValue(strFullRecord,"INCDNTFLSHRPRT_INCDNTDATTM"));		
		FormSetField("HSE_ACDNTENT","ACDNTENT_RPORTDDATTIM",GetFieldValue(strFullRecord,"INCDNTFLSHRPRT_FLSHDATTIM"));
		FormSetField("HSE_ACDNTENT","ACDNTENT_LOCTYP",GetFieldValue(strFullRecord,"INCDNTFLSHRPRT_LOC"));
		FormSetField("HSE_ACDNTENT","ACDNTENT_SIT",strSite);
		FormSetField("HSE_ACDNTENT","ACDNTENT_WRKLOC",GetFieldValue(strFullRecord,"INCDNTFLSHRPRT_AREANAM"));*/
		FormSetField("HSE_ACDNTENT","ACDNTENT_FLSHNO",strMainLink);
	}
	return AccidentCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}



/*************************************************************************************************************/

/*************************************************************************************************************
Helper function
*************************************************************************************************************/
void AccidentEntryCategory::CompleteAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	/*Commented_Code Moved to DB to check original code,Please check source safe earlier versions*/
	CString strSQL;
	CString strUserName = GetUserLogin();
	CString strSourceScreenName = GetScrCptnByTag(66,pCustomButtonClicked->Form_Tag,"");	
	CString KeyFieldValue = GetKeyField(pCustomButtonClicked->Form_Tag,pCustomButtonClicked->pMultiSelectedRows);
	strSQL.Format("EXECUTE completeAccidentTXN '%s','%s','%s'",KeyFieldValue,strSourceScreenName,strUserName);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

long AccidentEntryCategory::ShowAccidentEntry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTENT;
	return ParentManage("HSE_TGACDNTENT",TRUE,TRUE,TRUE,"HSE_ACDNTENT",strCriteria);
}

CString AccidentEntryCategory::isFlashRprtRqrd(CString strCmpnyName) {
	CString strSQL;	
	strSQL.Format("SELECT ISNULL(HSEPlc_IsFlshRprtMst,'0') FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyName);
	CString strFlashRprtRqrd = GetRecordSetData(strSQL).GetBuffer(0);	
	return strFlashRprtRqrd;
}

CString AccidentEntryCategory::isValidForEntryComplete(CString strKey) {
	CString strSQL;	
	strSQL.Format("SELECT dbo.ValidateBeforeIncdntEntryComplt("+strKey+")");
	CString isValid = GetRecordSetData(strSQL).GetBuffer(0);	
	return isValid;
}