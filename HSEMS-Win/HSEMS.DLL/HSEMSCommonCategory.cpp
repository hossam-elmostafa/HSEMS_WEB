// HSEMSCommonCategory.cpp: implementation of the CHSECommonCategory class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "HSEMSCommonCategory.h"
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"
#include "HandleDelClass.h"
#include "EMailMsgHandler.h"
#include <math.h>
#include "ChemicalRegisterCategory.h"
#include "DrillEntryCategory.h"
#include "EnvrnmntAspctCategory.h"
#include "AwrnsPlnCrsTrgtDprt.h"
#include "AwrnsPlnCrsInstrctr.h"
#include "ReasonsCategory.h"
#include "RiskAssessmentHzrdPopCategory.h"
#include "TaskResponsibilityPopCategory.h"

#include "AuditModuleCategory.h"
#include "NewCarEntryCategory.h"
#include "CorrectiveActionEntry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//polydb_task
  extern CString gstrSrcKyFldNm="";
  extern CString gstrScreen_tag="";
//extern CMapStringToString m_ScreenTagForTracing;
//extern bool gbOpenRejectScreenLocked;
  extern bool gbUpdateRejectedRecord=FALSE;


#define THIRD_LEVEL_SCREENS	"HSE_TGADTCHCKLSTRSLT,HSE_TGHRMTYP,HSE_TGSUBDTL"



/*
CHSEMSCommonCategory::CHSEMSCommonCategory():m_pszThirdLevelScreens(THIRD_LEVEL_SCREENS)
{
	m_bDisableSerial = FALSE;
	gbUpdateRejectedRecord=false;	
	
}
*/
CHSEMSCommonCategory::CHSEMSCommonCategory(CCategory *pCategory):CCommonCategoryWrapper(pCategory)
{
	//m_bDisableSerial = FALSE;
	//gbUpdateRejectedRecord=false;			
	m_bEnableMainScreen=true;// Must be true as default for all system screens and should be overriden if needed
	
	
	m_strStatusFieldName="";
	m_FSATableName="";
	m_strPrefix="";
	m_strAppPrefix="HSE";
	m_strMainLink="";
	m_strScreen="";
	m_bDisableSerial=TRUE;
	m_pszThirdLevelScreens=NULL;
	m_bEnableResults=true;

	
}

CHSEMSCommonCategory::~CHSEMSCommonCategory()
{

}

//Overrides
/* 
Get current year from company profile screen 700@66 and set current year in every screen
contain year field
*/

//*********************************************************************************************
// FrameWork Events
//********************************************************************************************
HRESULT CHSEMSCommonCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strScreenTag(pScreenReady->Form_Tag);
	/**
	 * Commented by ahmed ali
	 * 28-05-2013
	 * HSE_CR12v2_74-1
	 *
		if (strScreenTag == "HSE_TGPERFMSRMNTENT")
		{
			CString strMainTablePart=strScreenTag;
			CString strSQL="Select COMPPRF_CURYR From HSE_COMPPRF";
			CString currentYear=GetRecordSetData(strSQL,1);
			strMainTablePart.Replace("HSE_TG","");
			CString strFieldName=strMainTablePart+"_YR";
			SetDefaultValue(strScreenTag,strFieldName,currentYear);
		}
	 */

	if(pScreenReady->nBrowseMode)
	{
		//polydb_task
		//if(strstr(m_pszThirdLevelScreens,strScreenTag.GetBuffer (0)))
		//{
		//	EnableToolbar("",TOOLBAR_NEW,FALSE);
		//	EnableToolbar("",TOOLBAR_DELETE,FALSE);
		//}
	}
	//Solve bug of unique group with hidden field (Code + Link) is unique group and link is hidden
	if(strScreenTag == "HSE_TGSUBDTL") {
		SetFieldVisibleAttrib(strScreenTag,"LINK","HIDDEN");
	}	

	else if(strScreenTag == "HSE_TGDRLFNDNGS") {
		CString strComeFrom = getOwnerScreenTag();
		if(strComeFrom == "HSE_DRLLPLNINQ_ACTULSCNR") {
			EnableToolbar("HSE_DRLLPLNINQ_ACTULSCNR",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_DRLLPLNINQ_ACTULSCNR",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_DRLLPLNINQ_ACTULSCNR",TOOLBAR_SAVE,FALSE);
		}
	}
	else if(strScreenTag == "HSE_TGDRLFNDNGSUNPLNDSCNROS") {
		CString strComeFrom = getOwnerScreenTag();
		if(strComeFrom == "HSE_DRLLPLNINQ_UNPLNDSCNROS") {
			EnableToolbar("HSE_TGDRLFNDNGSUNPLNDSRCMNDACTN",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_TGDRLFNDNGSUNPLNDSRCMNDACTN",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_TGDRLFNDNGSUNPLNDSRCMNDACTN",TOOLBAR_SAVE,FALSE);
		}
	}

	return S_OK;
}

HRESULT CHSEMSCommonCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{	
	CString strFormTag	  = pRecordRepostion->Form_Tag;
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;
	CString strFullRecord = pRecordRepostion->Full_Record;		
	if(strFormTag == "HSE_TGADTPLNENT" && strSubFormTag == "HSE_TGADTPLNENTCNCDPRT")
	{
		m_bDisableSerial = FALSE;
	}
	else if(strFormTag.Find("APR")!=-1||strFormTag.Find("CNFRMTN")!=-1||strFormTag.Find("FOLLWUPDT")!=-1||strFormTag.Find("FOLLOWUP")!=-1||strFormTag.Find("FLWUP")!=-1 ||strFormTag.Find("FOLLWUP")!=-1 && pRecordRepostion->iComplete==1)
	{
		if(strSubFormTag.Find("REJRSN")!=-1||strSubFormTag.Find("RJCRSN")!=-1||strSubFormTag.Find("REJRES")!=-1)
		{
			CString strTableMainPart=strFullRecord.Mid(0,strFullRecord.Find("_"));			
			CString strFlg=strTableMainPart+CString("_FLG");
			CString strTableName=CString("HSE_")+CString(strTableMainPart);
			strTableName.Replace("HSE_TG","HSE_");			
			CString strFlgValue=FormGetField(strTableName,strFlg);
			//Condition : If Flag Field = 0
			//Response : Enable delete button
			//Else Disable Delete button
			if(strFlgValue.Compare("0")!=0) {				
				EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,FALSE);
			} else {
				EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,TRUE);
			}
		}
	} if(strSubFormTag.Find("CMNT") != -1 && pRecordRepostion->iComplete == 1){	//Comment tab
		//get flag value
		//CString strFlag = FormGetField("HSE_PTNHZRDCMNTS" , "FLAG");
		//manageCommentsTabToolBar(pRecordRepostion);
		manageCommentsTabToolBar(strFormTag,strSubFormTag);
	}
	return S_OK;
}
HRESULT CHSEMSCommonCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	int nRecordStatus(pToolBarButtonClicked->nRecordStatus);
	CString strFullRecord = pToolBarButtonClicked->Full_Record;
	CString strTableName  = pToolBarButtonClicked->Table_Name;
	strTableName.MakeUpper();
	CString strSubFormTag = pToolBarButtonClicked->SubForm_Tag;
	strSubFormTag.MakeUpper();
	CString strFormTag    = pToolBarButtonClicked->Form_Tag;
	strFormTag.MakeUpper();
	if(strBtnName=="SAVE") {				
		if(pToolBarButtonClicked->iComplete == 1) {
			if(strSubFormTag =="") {
				bool bNewRec = m_pCategory->GetHSEObj()->getbNewMode(strFormTag);
				if(bNewRec) {
					if(strFormTag == "HSE_TGRSKASSMNTENT"|| strFormTag == "HSE_TGPERFMSRMNTENT"|| strFormTag == "HSE_TGACDNTENT" 
					|| strFormTag == "HSE_TGNRSTMISCENT"   || strFormTag == "HSE_TGPTWSRGSTRENT" || strFormTag == "HSE_TGCRCTVEACCENT"
					|| strFormTag == "HSE_TGSITSRVYENT" || strFormTag == "HSE_TGINCDNTFLSHRPRT") {
						GenerateTXNNo(strFormTag);		
					}
				} 
				if(strFormTag == "HSE_TGADTRSCHDL") {								
					CString strRecKey= FormGetField("HSE_ADTRSCHDL","MAINLINK");					
					CString strSQL;
					strSQL.Format("Execute updateAdtSchdlDat %s",strRecKey);
					ExecuteSQL(strSQL);													
				}
				if(strFormTag == "HSE_CHMCLITMSDFNTN") {								
					FormSetField("HSE_CHMCLITMSDFNTN","CHMCLITMSDFNTN_LSTMDFYDT",GetFullDateTime("%d/%m/%Y"));
					FormSetField("HSE_CHMCLITMSDFNTN","CHMCLITMSDFNTN_LSTMDFYBY",GetUserLogin());
				}
			}		
			if(strSubFormTag == "") {
				//Risk Analysis screen deleted 
				/*if(strFormTag == "HSE_TGRSKANLYS") {
					getRskAnalysisCntrlMsr();
				}*/

				setChangedRecInfo(pToolBarButtonClicked->Table_Name);
				
				//Below code written to solve Framework Bug
				/* this if statements to solve bug if the screen doesn't contain data and allow only update not allow
				and the user exit the screen the refresh statements makes the system hang
				CString KeyFieldValue = GetKeyField (strTableName,pToolBarButtonClicked->pMultiSelectedRows);
				if(KeyFieldValue != "") {
					RefreshScreen("",REFRESH_SELECTED);
				}*/

				//Insert action created in tracing tab
				bool bIsNewRecord = m_pCategory->GetHSEObj()->getbNewMode(strFormTag);
				if(bIsNewRecord) {
					insertIntoTracing(strFormTag,CREATED); 
					m_pCategory->GetHSEObj()->setbNewMode(strFormTag,false);			
				}
			} else if (strSubFormTag == "HSE_DRVRLCNS") {
				RefreshScreen("",REFRESH_SELECTED);			
			}
		}
	}
	if(strBtnName=="NEW" && pToolBarButtonClicked->iComplete == 1 && strSubFormTag =="") {
		//polydb doesn't support nRecordStatus
		m_pCategory->GetHSEObj()->setbNewMode(strFormTag,true);
	}
	//set screen source in all commnets tab screen source field in new mode
	if(strBtnName=="NEW" && pToolBarButtonClicked->iComplete == 0 && strSubFormTag.Find("CMNT") != -1 ){
		//setCmntScreenSource(strFormTag,strSubFormTag,strTableName);
		//enableCmntTabFields(strSubFormTag);
		//enableCmntTabToolBar(strFormTag,strSubFormTag);
		manageCommentsTabToolBar(strFormTag,strSubFormTag);
	}	
	if(strBtnName=="CANCLE"&& pToolBarButtonClicked->iComplete == 0 && strSubFormTag !="")
	{
		CString strTablePart  = CString(strFormTag);
		strTablePart.Replace("HSE_TG","");				
		SetFieldVisibleAttrib(strSubFormTag,CString(strTablePart)+"_EMPNUM","NULL");
	}
	if(strBtnName=="NEW") {
		if (strFormTag == "HSE_TGSTE") {
			CString strCompanyName;
			strCompanyName = GetSingleCompanyName();
			if(strCompanyName != "") {
				FormSetField("HSE_STE","STE_CmpnyName",strCompanyName);
				SetFieldVisibleAttrib("HSE_TgSTE","STE_CmpnyName","ALWAYSLOCKED");
			}
		} else if (strFormTag == "CMN_EMPLOYEE") {
			/*
			CString strCmpnyNam = FormGetField("CMN_EMPLOYEE","EMPLOYEE_CMPNYCOD");
			CString strCurrencyCode = getCompanyCurreny(strCmpnyNam);
			FormSetField("CMN_EMPLOYEE","EMPLOYEE_CURRENCYS",strCurrencyCode);
			*///RQ-9-2023.5 EMPLOYEE_CURRENCYS is not used
		} else if (strFormTag == "HSE_TGENVMNTRACTVTSPLTNT") {
			if(pToolBarButtonClicked->iComplete == 1) {				
				FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_LNNO",getMsrArLineNo(FormGetField("HSE_ENVMNTRACTVTSPLTNT","EnvMntrActvtsPltnt_prmKy")));
			}
		} else if (strSubFormTag == "HSE_TGAWRNSCRSCRSMTRL") {
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("AWRNSCRSCRSMTRL_LNNO","HSE_AWRNSCRSCRSMTRL","AWRNSCRSCRSMTRL_LNK",FormGetField("HSE_AWRNSCRS","AWRNSCRS_CRSCOD"));
				FormSetField("HSE_AWRNSCRSCRSMTRL","AWRNSCRSCRSMTRL_LNNO",strNewSrl);
			}
		} else if (strSubFormTag == "HSE_TGAWRNSCRSPINSTRCTRS") {
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("AWRNSCRSPINSTRCTRS_SRLNO","HSE_AWRNSCRSPINSTRCTRS","AWRNSCRSPINSTRCTRS_LNK",FormGetField("HSE_AWRNSCRS","AWRNSCRS_CRSCOD"));
				FormSetField("HSE_AWRNSCRSPINSTRCTRS","AWRNSCRSPINSTRCTRS_SRLNO",strNewSrl);
			}
		} else if (strFormTag == "HSE_TGAWRNSPLN_CRS_ATNDS") {
			CString strNewSrl = getNxtSrl("AWRNSPLN_CRS_ATNDS_SRLNO","HSE_AWRNSPLN_CRS_ATNDS","AWRNSPLN_CRS_Atnds_LNK",FormGetField("HSE_AWRNSPLN_CRS_ATNDS","AWRNSPLN_CRS_ATNDS_LNK"));
			FormSetField("HSE_AWRNSPLN_CRS_ATNDS","AWRNSPLN_CRS_ATNDS_SRLNO",strNewSrl);
		}		
		//HSE BGC-EZZ Requirements_2892020
		else if (strSubFormTag == "HSE_TGACDNTENTACDNTRSN") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ACDNTENTACDNTRSN_LNRNUM","HSE_ACDNTENTACDNTRSN","AcdntEntAcdntRsn_Lnk",FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM"));			
				FormSetField("HSE_ACDNTENTACDNTRSN","ACDNTENTACDNTRSN_LNRNUM",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGACDNTENTTKNRMDLACTNS") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ACDNTENTTKNRMDLACTNS_LNRNUM","HSE_ACDNTENTTKNRMDLACTNS","ACDNTENTTKNRMDLACTNS_LNK",FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM"));			
				FormSetField("HSE_ACDNTENTTKNRMDLACTNS","ACDNTENTTKNRMDLACTNS_LNRNUM",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGACDNTENTRECMNDACC") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ACDNTENTRECMNDACC_ACTNNO","HSE_ACDNTENTRECMNDACC","ACDNTENTRECMNDACC_LNK",FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM"));			
				FormSetField("HSE_ACDNTENTRECMNDACC","ACDNTENTRECMNDACC_ACTNNO",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGACDNTENTATCH") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ACDNTENTATCH_SRLNO","HSE_ACDNTENTATCH","ACDNTENTATCH_LNK",FormGetField("HSE_ACDNTENT","ACDNTENT_ACDNTNUM"));			
				FormSetField("HSE_ACDNTENTATCH","ACDNTENTATCH_SRLNO",strNewSrl);
			}
		}						   
		else if (strSubFormTag == "HSE_TGNRSTMISCENTPSSBLNRSTMISCCUSE") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM","HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_LNK",FormGetField("HSE_VWNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM"));			
				FormSetField("HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM",strNewSrl);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////
		/*
			Bugs-10-2021.10
		*/
		else if (strSubFormTag == "HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM","HSE_NRSTMISCENTPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_LNK",FormGetField("HSE_VWNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM"));			
				FormSetField("HSE_TGNRSTMISCFLWUPPSSBLNRSTMISCCUSE","NRSTMISCENTPSSBLNRSTMISCCUSE_LNRNUM",strNewSrl);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////								   
		else if (strSubFormTag == "HSE_TGNRSTMISCENTRQRDACC" || strSubFormTag == "HSE_TGNRSTMISCFLWUPRQRDACC" || strSubFormTag == "HSE_TGNRSTMISCCNFRMTNRQRDACC") {//Bugs-10-2021.10			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("NRSTMISCENTRQDACTN_ACTNNO","HSE_NRSTMISCENTRQDACTN","NRSTMISCENTRQDACTN_LNK",FormGetField("HSE_VWNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM"));			
				FormSetField("HSE_NRSTMISCENTRQDACTN","NRSTMISCENTRQDACTN_ACTNNO",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGNRSTMISCENTATCH") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("NRSTMISCENTATCH_SRLNO","HSE_NRSTMISCENTATCH","NRSTMISCENTATCH_LNK",FormGetField("HSE_VWNRSTMISCENT","NRSTMISCENT_NRSTMISCNUM"));			
				FormSetField("HSE_NRSTMISCENTATCH","NRSTMISCENTATCH_SRLNO",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGADTPLNENTACTVT") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ADTPLNENTACTVT_ACTVNUM","HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_LNK",FormGetField("HSE_ADTPLNENT","MAINLINK"));			
				FormSetField("HSE_ADTPLNENTACTVT","ADTPLNENTACTVT_ACTVNUM",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGDRLFNDNGSRCMNDACTN") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLFNDNGSRCMNDACTN_ACTNNO","HSE_DRLFNDNGSRCMNDACTN","DrlFndngsRcmndActn_LNK",FormGetField("HSE_DRLFNDNGS","PrmryKy"));			
				FormSetField("HSE_DRLFNDNGSRCMNDACTN","DRLFNDNGSRCMNDACTN_ACTNNO",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGDRLFNDNGSUNPLNDSRCMNDACTN") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLFNDNGSUNPLNDSRCMNDACTN_ACTNNO","HSE_DRLFNDNGSUNPLNDSRCMNDACTN","DRLFNDNGSUNPLNDSRCMNDACTN_LNK",FormGetField("HSE_DRLFNDNGSUNPLNDSCNROS","PrmryKy"));			
				FormSetField("HSE_DRLFNDNGSUNPLNDSRCMNDACTN","DRLFNDNGSUNPLNDSRCMNDACTN_ACTNNO",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_TGCRCTVEACCENTTAKRMDALACC") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("CRCTVEACCENTTAKRMDALACC_LNRNUM","HSE_CRCTVEACCENTTAKRMDALACC","crctveAccEntTakrmdalAcc_LNK",FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELNUM"));			
				FormSetField("HSE_CRCTVEACCENTTAKRMDALACC","CRCTVEACCENTTAKRMDALACC_LNRNUM",strNewSrl);
			}
		}			
		else if (strSubFormTag == "HSE_TGCRCTVEACCENTROOTCUSE") {			
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("CRCTVEACCENTROOTCUSE_LNRNUM","HSE_CRCTVEACCENTROOTCUSE","CRCTVEACCENTROOTCUSE_LNK",FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELNUM"));			
				FormSetField("HSE_CRCTVEACCENTROOTCUSE","CRCTVEACCENTROOTCUSE_LNRNUM",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_DRLLPLN_UNPLNDSCNROS") {			
			if(pToolBarButtonClicked->iComplete == 1) {						 
				CString strNewSrl = getNxtSrl("DRLLPLN_UNPLNDSCNROS_SCNRCD","HSE_DRLLPLN_UNPLNDSCNROS","MainLink",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_UNPLNDSCNROS","DRLLPLN_UNPLNDSCNROS_SCNRCD",strNewSrl);
			}
		}		
		else if (strSubFormTag == "HSE_TGADTCHKLST") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("LINENUM","HSE_ADTCHKLST","ADTCHKLST_LNK",FormGetField("HSE_HSEPRCDRLIST","HSEPRCDRLIST_PRCDRNO"));			
				FormSetField("HSE_ADTCHKLST","LINENUM",strNewSrl);
			}
		}		
		else if (strSubFormTag == "HSE_DRLLPLN_EMT") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLLPLN_EMT_SRLN","HSE_DRLLPLN_EMT","MAINLINK",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_EMT","DRLLPLN_EMT_SRLN",strNewSrl);
			}
		}		
		else if (strSubFormTag == "HSE_DRLLPLN_ERT") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLLPLN_ERT_SRLN","HSE_DRLLPLN_ERT","MAINLINK",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_ERT","DRLLPLN_ERT_SRLN",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_DRLLPLN_INTRNLRSURCS") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLLPLN_INTRNLRSURCS_SRLN","HSE_DRLLPLN_INTRNLRSURCS","MAINLINK",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_INTRNLRSURCS","DRLLPLN_INTRNLRSURCS_SRLN",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_DRLLPLN_EXTRNLSUPPRT") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLLPLN_EXTRNLSUPPRT_SRLN","HSE_DRLLPLN_EXTRNLSUPPRT","MAINLINK",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_EXTRNLSUPPRT","DRLLPLN_EXTRNLSUPPRT_SRLN",strNewSrl);
			}
		}
		else if (strSubFormTag == "HSE_DRLLPLN_EXCUTNCMMNTS") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("DRLLPLN_EXCUTNCMMNTS_SRLN","HSE_DRLLPLN_EXCUTNCMMNTS","MAINLINK",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_PRMRYKY"));			
				FormSetField("HSE_DRLLPLN_EXCUTNCMMNTS","DRLLPLN_EXCUTNCMMNTS_SRLN",strNewSrl);
			}
		}
		else if (strFormTag == "HSE_TGDRLFNDNGS") {				
			if(pToolBarButtonClicked->iComplete == 1) {					
				CString strNewSrl = getNxtSrl("DRLFNDNGS_SRLNO","HSE_DRLFNDNGS","DRLFNDNGS_LNK",FormGetField("HSE_DRLFNDNGS","DRLFNDNGS_LNK"));		
				FormSetField("HSE_DRLFNDNGS","DRLFNDNGS_SRLNO",strNewSrl);
			}
		}
		else if (strFormTag == "HSE_TGDRLFNDNGSUNPLNDSCNROS") {				
			if(pToolBarButtonClicked->iComplete == 1) {						
				CString strNewSrl = getNxtSrl("DRLFNDNGSUNPLNDSCNROS_SRLNO","HSE_DRLFNDNGSUNPLNDSCNROS","DRLFNDNGSUNPLNDSCNROS_LNK",FormGetField("HSE_DRLFNDNGSUNPLNDSCNROS","DRLFNDNGSUNPLNDSCNROS_LNK"));		
				FormSetField("HSE_DRLFNDNGSUNPLNDSCNROS","DRLFNDNGSUNPLNDSCNROS_SRLNO",strNewSrl);
			}
		}
		else if (strFormTag == "HSE_DRLLPLNENTRYATENTRY") {				
			if(pToolBarButtonClicked->iComplete == 1) {					
				CString strNewSrl = getNxtSrl("DRLLPLN_DRLLN","HSE_DRLLPLN_VW","DRLLPLN_DRLLYR",FormGetField("HSE_DRLLPLN_VW","DRLLPLN_DRLLYR"));		
				FormSetField("HSE_DRLLPLN_VW","DRLLPLN_DRLLN",strNewSrl);
			}
		}			
		else if (strSubFormTag == "HSE_ASPCTS_ASPCTS") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("ASPCTS_ASPCTS_SRLNO","HSE_ASPCTS_ASPCTS","MAINLINK",FormGetField("HSE_ASPCTS_VIEW","PrmryKy"));			
				FormSetField("HSE_ASPCTS_ASPCTS","ASPCTS_ASPCTS_SRLNO",strNewSrl);
			}
		}
		else if (strFormTag == "HSE_TGENVMNTRRQRDACTN") {				
			if(pToolBarButtonClicked->iComplete == 1) {						
				CString strNewSrl = getNxtSrl("ENVMNTRRQRDACTN_ACTNNO","HSE_ENVMNTRRQRDACTN","ENVMNTRRQRDACTN_LNK",FormGetField("HSE_ENVMNTRRQRDACTN","ENVMNTRRQRDACTN_LNK"));		
				FormSetField("HSE_ENVMNTRRQRDACTN","ENVMNTRRQRDACTN_ACTNNO",strNewSrl);
			}
		}
		//Required Actions >> Audit Checklist Result >> Audits >> Audit Plan Follow up
		else if (strFormTag == "HSE_TGADTACTVTRQRDACTN") {
			CString strNewSrl = getNxtSrl("ADTACTVTRQRDACTN_ACTNNO","HSE_ADTACTVTRQRDACTN","ADTACTVTRQRDACTN_LNK",FormGetField("HSE_ADTACTVTRQRDACTN","ADTACTVTRQRDACTN_LNK"));
			FormSetField("HSE_ADTACTVTRQRDACTN","ADTACTVTRQRDACTN_ACTNNO",strNewSrl);
		}		
		//Corrective Actions Under Execution >> CAR Process
		else if (strSubFormTag == "HSE_TGCRCTVEACCUNDREXCTNCRPRCSS") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("CRCTVEACCENTRQDACTN_LNRNO","HSE_CRCTVEACCENTRQDACTN","crctveAccEntRqdActn_Lnk",FormGetField("HSE_CRCTVEACCENT","CRCTVEACCENT_CARMODELNUM"));			
				FormSetField("HSE_CRCTVEACCENTRQDACTN","CRCTVEACCENTRQDACTN_LNRNO",strNewSrl);
			}
		}
		//Bugs-2-2021.32
		//Auditee  - Audit Check List Plan
		else if (strFormTag == "HSE_TGADTCHCKLSTPLN") {
			CString strNewSrl = getNxtSrl("ADTCHCKLSTPLN_TSKNUM","HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_LNK",FormGetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_LNK"));
			FormSetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_TSKNUM",strNewSrl);
		}
		//RQ-6-2021.3
		else if (strSubFormTag == "HSE_TGPTWSRGSTRENTSFTYINSTRCTNS") {				
			if(pToolBarButtonClicked->iComplete == 1) {	
				CString strNewSrl = getNxtSrl("PTWSRGSTRSFTYINSTRCTNS_SRLNO","HSE_PTWSRGSTRSFTYINSTRCTNS","PTWSRGSTRSFTYINSTRCTNS_Lnk",FormGetField("HSE_PTWSRGSTRENT","PTWSRGSTRENT_PTWSNUM"));			
				FormSetField("HSE_PTWSRGSTRSFTYINSTRCTNS","PTWSRGSTRSFTYINSTRCTNS_SRLNO",strNewSrl);
			}
		}
	}		
	/*Deleted code to handle phsical/logical delete , Check Source Safe*/	
	if(strBtnName=="DELETE" && pToolBarButtonClicked->iComplete == 0)
	{				
		if(strSubFormTag == "")
		{
			CString strKeyFldNam;			
			CString strKeyFldVal;	
			CString strTableNam;
			strKeyFldVal = getTXNKeyFldVal(strFormTag);			
			if(strKeyFldVal != "") {				
				HandleDelClass *handleDelClass = new HandleDelClass(m_pCategory);				
				CString strSiteName = CString(strTableName);
				strSiteName.Replace("HSE_","");
				strSiteName = strSiteName+"_SIT";
				CString strSiteCode = FormGetField(strTableName,strSiteName);
				CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);		
				
				CString strPhysicaldeleteAllowed = IsPhysicaldeleteAllowed(strCmpnyNam);	
				//if physical delete allowed then perform physical delete
				if(strPhysicaldeleteAllowed != "True" && strPhysicaldeleteAllowed != "1") {	
					CString strSrcScr = GetScrCptnByTag(66,strFormTag,"");
					CString strDate = GetFullDateTime("%m/%d/%Y");	//TODO : Check this 
					bool bRecLogclDel = handleDelClass->performLogclDel(strFormTag,strKeyFldVal,GetUserLogin(),strSrcScr,strDate);
					if(bRecLogclDel) {
						*pToolBarButtonClicked->iCancle = 1;
						RefreshScreen("",REFRESH_SELECTED);
					}
				}
			}
		}
		else if (strSubFormTag == "HSE_TGWSTZNSALWDITMS") {
			int QOH = atoi(FormGetField("HSE_WSTZNSALWDITMS","WSTZNSALWDITMS_QOH"));
			if(QOH > 0) {
				*pToolBarButtonClicked->iCancle = 1;
				RefreshScreen("",REFRESH_SELECTED);
			}
		}
	}
	if(strBtnName=="EXIT" && pToolBarButtonClicked->iComplete == 0)
	{
		if(strFormTag=="HSE_TGHSEPLC")
		{
			/////////////////////////////////////////////
			//RQ-9-2023
			CString strSQL;
			strSQL.Format("UPDATE HSE_HSEOBSRVTN set HSEOBSRVTN_GnrCrStt=HSEOBSRVTN_GnrCrStt");
			long lRet=ExecuteSQL(strSQL);
			if(lRet==0)
			{
				AfxMessageBox("The policy screen observation tab must be filled");
				*(pToolBarButtonClicked->iCancle)=1;
				return S_FALSE;
			}
			/////////////////////////////////////////////
		}
	}
	return S_OK;
}

HRESULT CHSEMSCommonCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{	
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strBtnName(pCustomButtonClicked->Button_Name);		
	CString strTableName=CString(pCustomButtonClicked->Table_Name);
	
	CString strFieldMainPartName=CString(strTableName);
	strFieldMainPartName.Replace("HSE_","");		

	if((strBtnName.Find("ADNLNSCRVRSN") != -1)&&strTableName != "HSE_ADTPLNENTACTVT")
	{
		addNewVersion(strTableName,strFormTag,strSubFormTag,strFieldMainPartName);
	} 
	else if(strBtnName.Find("_VWRJCTRSNS")!=-1) 
	{						
		viewRejectReason(pCustomButtonClicked);								//According to change request CR03-2:user press view reject reason 
	} 
	else if (strBtnName=="OBSRVTNAPR_GENERATECAR")
	{
		//////////////////////////////////////////
		//RQ_SF_OA_11_06_23.01
		//Request to remove:RQ-HM-11-10-23.01.07.12

		//////////////////////////////////////////

	}
	else if(strBtnName.Find("_GENERATECAR") != -1) 
	{
		//08/11/2020
		//TODO : below condition to handle drill Inq case,we shouldn't generate CAR in drill but I should hide button instead
		CString strComeFrom = getOwnerScreenTag();
		if(strComeFrom != "HSE_DRLLPLNINQ_UNPLNDSCNROS" && strComeFrom != "HSE_DRLLPLNINQ_ACTULSCNR") {
			generateCAR(strFormTag,strSubFormTag);
		}
	} 
	else if(strBtnName.Find("_UPDTCAR") != -1) 
	{	
		updateCAR(strFormTag,strSubFormTag);
	} 
	else if(strBtnName.Find("_CNLCAR") != -1)
	{
		cancelCAR(strFormTag,strSubFormTag);
	} 
	else if(strBtnName.Find("_VIEWRELATEDCAR") != -1)					//CR_11 view related CAR,Ref : 701_4_403_01							
	{		
		CString strFieldName = "CAR_KEY";
		CString strCARNum = FormGetField(strTableName,strFieldName);
		if(strCARNum!="")
			openRelatedCARs(strCARNum);		
	} 
	else if(strBtnName == "ADTCHCKLSTPLN_RQRDACTNBTN") 
	{
		CString strNCnfrmtyTsk = FormGetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_TSKSTS");			
		//716-48102-2
		if(strFormTag == "HSE_TGADTCHCKLSTPLN") {
			OpenRequiredActionScreen("HSE_TGADTCHCKLSTPLN",0);
		}
		//48102_2
		else if(strFormTag == "HSE_TGADTCHCKLSTRSLT") {
			OpenRequiredActionScreen("HSE_TGADTCHCKLSTRSLT",0);
		}			
	} 
	/* Not Used
	 else if(strBtnName == "CNTRLMSRS_GETCNTRLMSR") {
		getRskAnalysisCntrlMsr();
	}*/ 
	else if(strBtnName == "JOBSTPHZRD_CNSQNCS") 
	{
		opnJobHzrdCnsqncs();
	} 
	else if(strBtnName == "ADTGRPADTE_ADTCHCKLST") 
	{
		opnAdtGrpAdtChkLst();
	} 
	else if (strBtnName.Find("_WRKACPTD") != -1) 
	{
		accptWrk(strTableName,strFormTag,strSubFormTag);
	} 	
	else if (strBtnName.Find("_WRKNOTACPTD") != -1) 
	{
		rjctWrk(strTableName,strFormTag,strSubFormTag);
	}
	else if(strBtnName == "OPEN_FOLDER") 
	{
		//ShellExecute(NULL, "open", "C:\"", NULL, NULL, SW_SHOWDEFAULT);
		//system("explorer \\its\Internal Releases\Baseline 01-12-2017\Release-01");
		//CString strWorkingFolder =  m_pCategory->GetHSEObj()->GetWorkingFolder() + "\\Documentation";	
		CString strWorkingFolder = FormGetField("HSE_HSEPLC","HSEPLC_QMSMNULPATH");
		ShellExecute(NULL, "open", "explorer.exe", strWorkingFolder, NULL, SW_SHOWNORMAL);
	}
	else if(strBtnName == "ACTIVATE") 
	{
		CString strRecStsFldNam = strTableName;
		strRecStsFldNam.Replace("HSE_","");
		strRecStsFldNam = strRecStsFldNam + "_RECSTS";
		CString strFldVal = FormGetField(strTableName,strRecStsFldNam);
		CString strKeyFldNam = GetKeyFieldName(strTableName);
		CString strKeyFldVal = FormGetField(strTableName,strKeyFldNam);
		if(strFldVal == "2" || strFldVal == "10"){
			CString strSQL;
			strSQL.Format("EXECUTE updateTXNSts '%s','%s','%s','%s','%s'",strKeyFldVal,"1",strTableName,strRecStsFldNam,strKeyFldNam);
			ExecuteSQL(strSQL);		
			CString strMainTablePart = strTableName;
			strMainTablePart.Replace("HSE_","");
			CString strLnkField = strMainTablePart;
			strLnkField = strMainTablePart + "_LNK";
			insertIntoTracing(strFormTag,ACTIVATED);
			RefreshScreen("",REFRESH_SCREEN);
		}
	} else if(strBtnName == "VIEW_IN-ACTIVE_ITEMS") {
		ChangeCriteria("HSE_CHMCLITMSDFNTN","","WHERE (CHMCLITMSDFNTN_ACTV = 'N')");	
	} else if(strBtnName == "VIEW_ACTIVE_ITEMS") {
		ChangeCriteria("HSE_CHMCLITMSDFNTN","","WHERE (CHMCLITMSDFNTN_ACTV = 'Y')");	
	} else if (strBtnName == "RE-LOAD_HAZARDS") {
		CString strItemCode = FormGetField("HSE_CHMCLITMSDFNTN","CHMCLITMSDFNTN_ITMCD");
		CString strSQL;	
		strSQL.Format("RELOAD_HAZARD '%s'",strItemCode);
		ExecuteSQL(strSQL);
		RefreshScreen("",REFRESH_SELECTED);
	} else if (strBtnName == "ACDNTENTOURVHCLS_MSNRSTLOG"){
		OpenMsonRstLog(strFormTag,1);
	} else if(strBtnName == "INCDNTMDCLRPRTINJRDPRSNS_INJRDBODYPART") {
		OpenBodyParts("HSE_TGINCDNTMDCLRPRT");
	} else if (strBtnName == "INCDNTMDCLRPRTINJRDPRSNS_INJRYTYP") {
		openInjuryType("HSE_TGINCDNTMDCLRPRT");
	} else if (strBtnName == "INCDNTMDCLRPRT_ENTRYCMPLT") {
		CString strKeyVal = FormGetField("HSE_INCDNTMDCLRPRT","IncdntMdclRprt_FlshNo");
		completeIncdntMdclRprt(strKeyVal);
	} else if (strBtnName == "INCDNTMDCLRPRT_CLS") {
		CString strKeyVal = FormGetField("HSE_INCDNTMDCLRPRT","IncdntMdclRprt_FlshNo");
		closeIncdntMdclRprt(strKeyVal);
	} else if (strBtnName == "TOPEVNTSTHRTS_BRS") {
		CString strSQL ;		
		CString strKeyField = FormGetField("HSE_TOPEVNTSTHRTS","MAINLINK");				
		CString strDefValues;		
		strSQL.Format("SELECT * FROM HSE_TOPEVNTSTHRTSBRS WHERE (LINK = %s)","'"+strKeyField+"'");
		strDefValues.Format("LINK|%s",strKeyField);
		ShowScreen(0,"HSE_TGTOPEVNTSTHRTSBRS","Barriers",NORMAL_MODE,false,strSQL,"",strDefValues,"",false);
	} else if (strBtnName == "INCDNTMDCLRPRTINJRDPRSNS_INJRYCTGRY") {
		openInjueryRsltScr(strFormTag);
	}
	/* Moved to EnvMntrMsrPopupCategory Class
	else if(strBtnName == "ENVMNTRACTVTSPLTNTMSRAREA_RQRDACTN"){
		CString strSQL ;	
		bool bLocked = false;
		CString strPrmKyFeild = FormGetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA" ,"PRMKY");
		CString strDefValues;
		strDefValues.Format("ENVMNTRRQRDACTN_LNK|%s",strPrmKyFeild);	
		strSQL.Format("SELECT * FROM HSE_ENVMNTRRQRDACTN WHERE (ENVMNTRRQRDACTN_LNK = %s)",strPrmKyFeild);
		ShowScreen(0,"HSE_TGENVMNTRRQRDACTN","Required Actions",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
	}*/
	return S_OK;
}

//******************************************************

//Done by  : Doaa Desouky
// Date : 23/6/2010
// Purpose : Solve bug 6763 " Empty second selection in abrowsing field according to first selection "
//******************************************************

HRESULT CHSEMSCommonCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strSQL;
	CString strFormTag    = pBrowsingSelectedItem->Form_Tag;
	CString strSubFormTag = pBrowsingSelectedItem->SubForm_Tag;
	CString strFieldName  = pBrowsingSelectedItem->BrowsingField;	
	strFieldName.MakeUpper();
	CString strTablePart;
	CString strTableName;
	if (strFieldName == "ADTRSCHDL_ACTVNUMTEMP") {
		CString strPlnNum   = GetFieldValue(pBrowsingSelectedItem->Full_Record,"ADTRSCHDLBRWS_ADTNUM");
		CString strCmpnyNam = GetFieldValue(pBrowsingSelectedItem->Full_Record,"ADTRSCHDLBRWS_CMPNYNAM");
		CString strAdtYr    = GetFieldValue(pBrowsingSelectedItem->Full_Record,"ADTRSCHDLBRWS_ADTYR");
		CString strMainLink = GetFieldValue(pBrowsingSelectedItem->Full_Record,"MAINLINK");

		FormSetField("HSE_ADTRSCHDL","ADTRSCHDL_ADTNUM",strPlnNum);				
		FormSetField("HSE_ADTRSCHDL","ADTRSCHDL_CMPNYNAM",strCmpnyNam);
		FormSetField("HSE_ADTRSCHDL","ADTRSCHDL_ADTYR",strAdtYr);
		FormSetField("HSE_ADTRSCHDL","ADTRSCHDL_ACTVNUM",strMainLink);
	}
	//	HSE BGC-EZZ Requirements_2892020
	//	Incident Medical Report >> Injured Persons >> If (company employee) = "Yes", then count & view (No. Previous Injuries) of the same EE
	//  Employee can be inserted from flash report or medical report screen
	else if (strFieldName == "INCDNTMDCLRPRTINJRDPRSNS_EMPNUM" || strFieldName == "INCDNTFLSHRPRTINJURDPRSNS_EMPNUM") {		
		CString strEmpNum,
				strSit = "",
				strLoc = "";
		if(strFormTag == "HSE_TGINCDNTFLSHRPRT" || strFormTag == "HSE_TGINCDNTFLSHRPRTRVEW"){			
			strTableName = "HSE_INCDNTFLSHRPRTINJURDPRSNS";
			strSit = FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_SIT");
			strLoc = FormGetField("HSE_INCDNTFLSHRPRT","INCDNTFLSHRPRT_LOC");

		} else {
			strTableName = "HSE_INCDNTMDCLRPRTINJRDPRSNS";			
		}				
		strEmpNum    =  FormGetField(strTableName,strFieldName);		
		CString nPrevInjry = getPrevInjry(strTableName,strFieldName,strEmpNum,strSit,strLoc);		
		FormSetField(strTableName,"NOPRVSINJRS",nPrevInjry);
	}
	//HSE BGC-EZZ Requirements_2892020
	else if (strFieldName == "WSTZNSALWDITMS_ITMCOD" ) {
		CString strAlwSnglUOM = FormGetField("HSE_WSTZNS","WSTZNS_ALWSNGLUOM");
		if(strAlwSnglUOM == "Yes") {
			CString strItemUOM = FormGetField("HSE_WSTZNSALWDITMS","UOM");
			CString strZoneUOM = FormGetField("HSE_WSTZNS","WSTZNS_ALWSNGLUOMD_UKF");
			if(strItemUOM != strZoneUOM) {
				//Empty Item Code
				AfxMessageBox("All items in zone must have same UOM!");
				FormSetField("HSE_WSTZNSALWDITMS","WSTZNSALWDITMS_ITMCOD","");
			}
		}
		CString strItemKey = FormGetField("HSE_WSTZNSALWDITMS","WSTZNSALWDITMS_ITMCOD");
		if(!isItemRqrChkQOH(strItemKey)) {
			FormSetField("HSE_WSTZNSALWDITMS","WSTZNSALWDITMS_MXALWDQOH","0");
		}
	}
	return S_OK;
}

HRESULT CHSEMSCommonCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strSQL;
	CString strFormTag    = pFieldChange->Form_Tag;
	CString strSubFormTag = pFieldChange->SubForm_Tag;
	CString strFieldName  = pFieldChange->Field_Name;	
	CString strFieldValue = pFieldChange->New_Value;
	CString strTableName  = pFieldChange->Table_Name;
	strFieldName.MakeUpper();
	CString strTablePart;
	
	//This code instead of equation "SUBDTL_COD+'_'+LINK"
	//Polydb doesn't support equation in one to one screen
	if(strFieldName == "SUBDTL_COD" && strFormTag =="HSE_TGSUBDTL") {
		CString strDtlCode = FormGetField(strTableName,strFieldName);
		CString strLink	   = FormGetField(strTableName,"LINK");
		FormSetField(strTableName,"MAINLINK",strDtlCode+"_"+strLink);
	}

	/* According to Change Request(CR12v21-102)
	 *Validate Transaction date based on Policy.Max TXN Late Days for all transactions
	 *ValidationRule:101-300-4, 201_320_7, 503_33_6, 301_340_5, 401_370_4, 404_400_4, 407_510_3, 501_440_4, 701_430_12 
	 *Done By:Mai Talaat*/
	if (strFieldName == "NRSTMISCENT_ISUDAT"	|| 
		strFieldName == "RSKASSMNTENT_ISUDAT"	||
		strFieldName == "PTNLHZRDENT_ISUDAT"	||
		strFieldName == "ACDNTENT_ACDNTDAT"     ||
		strFieldName == "VCLACDNTENT_ACDNTDATTIM"||
		strFieldName == "LOSSACDNTENT_ACTDAT"	||
		strFieldName == "PERFMSRMNTENT_PLNVSTDAT"
		//||strFieldName == "PTWSRGSTRENT_ISUDAT" according to Mr Sameer requirement this validation is not required in this TXN.
		)
	{
		CString strSiteNam = CString(strTableName);
		strSiteNam.Replace("HSE_","");
		strSiteNam = strSiteNam+"_SIT";
		if(strSiteNam == "vwNRSTMISCENT_SIT")
			strSiteNam = "NRSTMISCENT_SIT";
		CString strSiteCode = FormGetField(strTableName,strSiteNam);
		CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);					
		if (!IsIssueDateValid(strFieldValue,strCmpnyNam))
		{
			*pFieldChange->iCancle = 1;
			return S_FALSE;;
		}
		//Commented by a.ali 16-11-2016
		//GenerateTXNNo function moved from new to save , so no need to generate TXN No on field change
		//else
		//{
			/*According to the Change request (HSE_CR12v2_11),
			 *For all HSE transactions except Performance Measurement & AUDIT transactions.
			 *RuleID: 000_7 General Specs
			 *Calculate TXN Number and Year
			 *Done By:Mai Talaat*/
		   	//GenerateTXNNo(strFormTag);
		//}
	}


/*
	Web Migration Review WMR
	if(strSubFormTag == "")
	{
		strTablePart = GetTablePartOfFieldName(strTableName);
		//strTableName = GetTableName(strFormTag);
	}
	else
	{
		strTablePart = GetTablePartOfFieldName(strTableName);
		//strTableName = GetTableName(strSubFormTag);
	}
*/
	strTablePart = GetTablePartOfFieldName(strTableName);
	
	//if(strFieldName.Find("_SIT") != -1|| strFieldName.Find("_LOCTYP")!= -1)
	if((strFieldName.Find("_SIT") != -1|| strFieldName.Find("_LOCTYP")!= -1) && strFormTag != "HSE_TGACDNTENT")
		EmptySiteHirarchyInChange(strTableName,strFieldName,strTablePart);
	else if(strFieldName.Find("SVRTYRAT")!= -1|| strFieldName.Find("LKLYHODRAT")!= -1)
		EmptySeverityRankInChangeSeverityRating(strTableName,strFieldName,strTablePart);
	else if(strFieldName.Find("SVRTRATCLS")!= -1)
		EmptySeverityRate(strTableName,strFieldName,strTablePart);
	else if(strFieldName.Find("HZRDCTG")!= -1|| strFieldName.Find("HZRDGRP")!= -1)
		EmptyHazardHirarchyInChange(strTableName , strFieldName,strTablePart);
	else if(strFieldName.Find("CLS")!= -1)
		EmptySubClassInChangeClass(strTableName , strFieldName,strTablePart);
	//else if(strFieldName.Find("_GRP")!= -1)
	//	EmptyDetlCodeInChangGrp(strTableName , strFieldName,strTablePart);
	else if(strFieldName.Find("_FILNAM") != -1 || strFieldName.Find("_TXNIMG") != -1 || strFieldName.Find("_ATTCHDFLNM") != -1) {
		CString strFieldNameTemp = CString(strFieldName);
		strFieldNameTemp.Replace("_FILNAM","");	
		CString strTableName = CString("HSE_")+CString(strFieldNameTemp);		
		CString strExtensionFieldName = CString(strFieldName);
		strExtensionFieldName.Replace("_FILNAM","_FILETYPE");
		CString strFileSizeFieldName = CString(strFieldName);
		strFileSizeFieldName.Replace("_FILNAM","_FILESIZ");
		if(!strFieldValue.IsEmpty()) {
			CString strSize = getFileSize(strFieldValue);			
			CString strFileExt	= getFileExtension(strFieldValue);			
			
			CString strSiteNam = CString(strFormTag);
			CString strMainTableNam = CString(strFormTag);
			//strMainTableNam.Replace("HSE_TG","HSE_"); //RQ-HM-11-10-23.01.06.09 it must not be changed likewise. The screentag could have TG in it
			strSiteNam.Replace("HSE_TG","");
			strSiteNam = strSiteNam+"_SIT";		
			CString strSiteCode = FormGetField(strMainTableNam,strSiteNam);
			CString strCmpnyNam = getCmpnyNamFrmSit(strSiteCode);
			if(!ValidFileSize(strFileExt,atof(strSize),strCmpnyNam))	
			{
				if(strFieldName.Find("_TXNIMG") != -1){	//handle multi-image 
					m_pHSEMSPolyDb->CancelUploadImage(strFieldName);
				} else {
					*pFieldChange->iCancle = 1;				
					FormSetField(strTableName, strFieldName, "");//(RQ-HM-11-10-23.01.06.09) the field name must be cleared
					return S_FALSE;
				}	
			}								
			FormSetField(strTableName, strExtensionFieldName, strFileExt);
			FormSetField(strTableName, strFileSizeFieldName, strSize);
		}
	}
	else if (strFieldName == "AWRNSCRSCRSMTRL_FILENAM") {
		FormSetField("HSE_AWRNSCRSCRSMTRL", "AWRNSCRSCRSMTRL_FILETYP", getFileExtension(strFieldValue));
		FormSetField("HSE_AWRNSCRSCRSMTRL", "AWRNSCRSCRSMTRL_FILESZKB", getFileSize(strFieldValue));
	}
	else if (strFieldName == "RSCUPLN_ATTCHMNTS_FLNM") {
		FormSetField("HSE_RSCUPLN_ATTCHMNTS", "RSCUPLN_ATTCHMNTS_FLTYP", getFileExtension(strFieldValue));
		FormSetField("HSE_RSCUPLN_ATTCHMNTS", "RSCUPLN_ATTCHMNTS_FLSZKB", getFileSize(strFieldValue));
	}

	if(strFieldName.Find("COMPEMP") != -1) {
		EmptyEmployeeInfoWhenChangeCompanyEmplyee(pFieldChange,strTableName , strFieldName,strTablePart);
	}
	else if(strFieldName == "HSEPLCEQINSPCTNS_ALWEDTRSLTSCR") {
		CString strAppCode = "66";
		CString strTag = "HSE_EQINSPCTNRSULT";
		CString strUpdatedFieldName = "NewValidationAttrib";
		if(strFieldValue == "Y") {
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQTYP","MUST",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQOWNR","MUST",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQSRLN","MUST",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQPLTN","NULL",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_DRVRNM","NULL",strUpdatedFieldName);			
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_ST","MUST",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_LCTN","MUST,",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_AR","MUST",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EXCTAR","NULL",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_INSDPLNT","MUST",strUpdatedFieldName);				
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_CNCRNDDPRTMNT","MUST",strUpdatedFieldName);	
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_PRJCT","NULL",strUpdatedFieldName);																						
		} else {
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQTYP","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQOWNR","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQSRLN","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EQPLTN","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_DRVRNM","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_ST","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_LCTN","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_AR","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_EXCTAR","ALWAYSLOCKED",strUpdatedFieldName);
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_INSDPLNT","ALWAYSLOCKED",strUpdatedFieldName);				
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_CNCRNDDPRTMNT","ALWAYSLOCKED",strUpdatedFieldName);	
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_PRJCT","ALWAYSLOCKED",strUpdatedFieldName);	
		}
	}
	else if(strFieldName == "HSEPLCEQINSPCTNS_VALDEQCRTFCTTLEXT") {
		CString strAppCode = "66";
		CString strTag = "HSE_EQINSPCTNRSULT";
		CString strUpdatedFieldName = "NewValidationRule";
		if(strFieldValue == "Y") {
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_CRTFCTEXPRYDT","[DATETIME][EQINSPCTN_CRTFCTEXPRYDT>=EQINSPCTN_EXPCTDEXTDT]",strUpdatedFieldName);
		} else {
			UpdateValidation(strAppCode,strTag,"","EQINSPCTN_CRTFCTEXPRYDT","NULL",strUpdatedFieldName);
		}
	} 
	/*
	 * TXN deleted
	else if(strFieldName == "ENVMNTRACTVTSPLTNT_MSRDVAL") {
		float nMinVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_MINVAL"));
		float nMaxVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_MAXVAL"));
		float nMsrdVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_MSRDVAL"));
		if(nMsrdVal < nMinVal)
		{										
			FormSetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_DVTN","Y");		//moved from FirstSys customAttribute		
		} else if(nMsrdVal > nMaxVal){			
			FormSetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_DVTN","Y");		//moved from FirstSys customAttribute		
		} else {			
			FormSetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_DVTN","N");		//moved from FirstSys customAttribute		
		}
	}
	*/ 
	else if (strFieldName == "ENVMNTRACTVTSPLTNTMSRAREA_MSRDVAL") {
		float nMinVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_MINVAL"));
		float nMaxVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNT","ENVMNTRACTVTSPLTNT_MAXVAL"));
		float nMsrdVal = atof(FormGetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_MSRDVAL"));
		if(nMsrdVal < nMinVal)
		{										
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_DVTN","Y");		
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_RQRNCR","Y");
		} else if(nMsrdVal > nMaxVal){			
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_DVTN","Y");
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_RQRNCR","Y");
		} else {			
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_DVTN","N");	
			FormSetField("HSE_ENVMNTRACTVTSPLTNTMSRAREA","ENVMNTRACTVTSPLTNTMSRAREA_RQRNCR","N");
		}		
	}
	//Bugs-6-2021.7
	//Integration with GMS requirement
	else if (strFieldName == "HSEPLC_GMSINTGRTNDB" && strFieldValue != "") {	
		CString strSQL,strGMSDB,dbExist;	
		CString strGMSDBs = strFieldValue;			
		bool bErrorExist = false;
		CPolyDbRecordset DaoDBRecSet;
		strGMSDB=strtok(strGMSDBs.GetBuffer(0),",");				
		while (strGMSDB != "")
		{	
			//Imp Note : I can't use below code as strtok function exist in GetRecordSetData
			//and when calling it from while loop 'strtok' we get an error
			/*strSQL.Format("SELECT DB_ID('%s')",strGMSDB);						
			strGMSDB=strtok(NULL,",");		
			dbExist=GetRecordSetData(strSQL).GetBuffer(0);;
			if(dbExist != "1") {	
				bErrorExist = true;
			}*/
			strSQL.Format("SELECT DB_ID('%s') AS DB_EXIST",strGMSDB);				
			DaoDBRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
			if(DaoDBRecSet.IsEOF())
			{
				dbExist = "";
			}
			DaoDBRecSet.GetFieldValue("DB_EXIST",dbExist);				
			DaoDBRecSet.Close();
			if(dbExist == "") {	
				bErrorExist = true;
			}
			strGMSDB=strtok(NULL,",");	
		}	
		if(bErrorExist) {			
			FormSetField("HSE_HSEPLC","HSEPLC_GMSINTGRTNDB","");
			AfxMessageBox("Please make sure that GMS Database(s) exists before integrate with GMS.");
		} else { //Create integration view

		
		}
	}
	
	else if(strFieldName == "CARMODELBAS_SYSPHS")
	{
		///////////////////////////////////////////////
		//RQ-SF-9-1-23_5_1.1
		CString strSQL;
		strSQL.Format("UPDATE %s set %s='%s' where %s='%s'",strTableName,strFieldName,strFieldValue,strFieldName,strFieldValue);
		long lRet=ExecuteSQL(strSQL);
		if(lRet!=0)
		{
			AfxMessageBox("The selected value has been choosen before. Cann't be used again");
			FormSetField(strTableName,strFieldName,"");				
			return S_FALSE;
		}
		///////////////////////////////////////////////

	}
	return S_OK;
}
	

HRESULT CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strSQL;
	CString strFormTag    = pBeforBrowsingButtonClick->Form_Tag;
	CString strSubFormTag = pBeforBrowsingButtonClick->SubForm_Tag;
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	CString strTableName = strSubFormTag;
	strFieldName.MakeUpper();
	char *pstrFound;

	CString DeptWhereCondition  = "";
	CString strCriteria = "";
	
	pstrFound=strstr(strSubFormTag,"ATCH");
	if(pstrFound!=NULL)
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL = FilterSubClassOnSelectedClass(strSubFormTag,strFieldName).AllocSysString();


	//HSE_CR12v2_93-1
	if(strFormTag == "HSE_TGADTCHCKLSTPLN") {		
		filterAdtChkLst(pBeforBrowsingButtonClick);
	}


	if(strFieldName.Find("EMPNUM") != -1)
	{	
		
		/*
		 * Commented,was written to handle unique problem in tabs
		// Get table name which browse on employee screen
		CString strTableName = GetTableNameBrwsOnEmp(strFieldName);

		// Get Link Field Value 
		CString LnkFieldName = GetLnkFldNameForBrowseTable(strTableName);
		CString LnkFieldValue = FormGetField(strTableName , LnkFieldName);
		
		//Update department change to Auditee 
		// in case of Auditees filter also on auditee depratement in activity tab
		//if(strTableName == "HSE_ADTPLNENTAUDITEES")
		//	DeptWhereCondition = FilterAditeesEmpOnAudteeDpt(LnkFieldValue);

		// get all selected employee for browse table
		CString strSelectedEmployees = GetSlctdEmp(strFieldName,strTableName,LnkFieldName,LnkFieldValue);

		if(strSelectedEmployees.IsEmpty()!=true && strSelectedEmployees != "','")
			strCriteria = FilterEmployees(DeptWhereCondition , strSelectedEmployees);
		else
			strCriteria = GetAllEmployees(DeptWhereCondition);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strCriteria.AllocSysString();		
		*/

	}

	/*
	Update : field _HSEVRFDBY deleted from system 
	According : HSE_CR12v2_19 
	Rules ID : (101_30010_1,103_30010~2_1,104_30010~3_1,201_324_1,202_324~10 _1,203_32310 _1,204_32310~1 _1,212_324~11 _1,
				301_34010 _1,302_34010~1_1,303_34010~2_1,402_32410_3,402_32410~1_3,402_32410~2_3,404_32440_1,
				405_32440~1_1,406_32440~2_1,407_51040_1,408_51040~1_1,409_51040~2_1)

	By Mohamed Elghobashi	
	if(strFieldName == "RSKASSMNTENTRQRDACTN_HSEVRFDBY" 
		|| strFieldName =="LOSSACDNTENTRQRDACC_HSEVRFDBY"	
		|| strFieldName =="PTNLHZRDENTRQRDACC_HSEVRFDBY"	
		|| strFieldName =="NRSTMISCENTRQDACTN_HSEVRFDBY"	
		|| strFieldName =="ACDNTENTRECMNDACC_HSEVRFDBY"		
		|| strFieldName =="PERFMSRMNTENTRQRDACTN_HSEVRFDBY" 
		|| strFieldName =="VCLACDNTRQDACTN_HSEVRFDBY"
		|| strFieldName =="ADTACTVTRQRDACTN_HSEVRFDBY")
	{
			// Filter the HSE Verified By browsing list of EE’s where (department = policy, owner department) 
			CString strOwnerDept = getOwnerDept();
			CString strSQL;			
			strSQL.Format("WHERE (EMPLOYEE_WSHOP ='%s')",strOwnerDept);
			pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
		
	}
	*/
	
	//a.ali 3-11-2013
	//In the (Protective Equipment) combo list, exclude selected records for same "accident No."
	//TODO this code should move to new class which handle personal protective equipment events
	//Moved to PrsonlProtctveEquip.cpp file
	/*if(strFieldName == "PRSNLPRTCTVEQPMNT_PRTCTVEQPMNT") {
		CString strLink = FormGetField("HSE_PrsnlPrtctvEqpmnt","PRSNLPRTCTVEQPMNT_LINK");
		if(strLink.IsEmpty()) {
			strLink = " ";
		}
		CString strSQL;
		strSQL.Format("WHERE (PRSNLPRTCTVEQ_COD NOT IN (SELECT PRSNLPRTCTVEQPMNT_PRTCTVEQPMNT FROM HSE_PRSNLPRTCTVEQPMNT WHERE PRSNLPRTCTVEQPMNT_LINK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}*/

	//a.ali 3-11-2013
	//In (Human Body Parts) popup, The (body part) combo list, should exclude already selected records for same "injured person"
	//TODO this code should be moved to special class which handle human body parts screen
	//Moved to HumanBodyParts.cpp file
	/*if(strFieldName == "HMNBODYPART_COD") {
		CString strLink = FormGetField("HSE_HMNBODYPART","HMNBODYPART_LNK");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (HUMNBDYPRT_COD NOT IN (SELECT HMNBODYPART_COD FROM HSE_HMNBODYPART WHERE HMNBODYPART_LNK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}*/

	//Moved to InjueryType.cpp
	/*if(strFieldName == "INJRYTP_INJRYTYP") {
		CString strLink = FormGetField("HSE_INJRYTP","INJRYTP_LNK");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (INJRYTYP_COD NOT IN (SELECT INJRYTP_INJRYTYP FROM HSE_INJRYTP WHERE INJRYTP_LNK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}*/
	
	//a.ali 3-11-2013
	//(Item Name) combo, browsing list of (item name) from "HSE Tools & Equipments, items" belonging to selected "Group Name"
	//Moved to CHSEToolEquipDistrib
	/*if(strFieldName == "TOOLSEQUIPDSTRBTNITEMS_ITMNAM") {
		CString strLink = FormGetField("HSE_TOOLSEQUIPDSTRBTNITEMS","TOOLSEQUIPDSTRBTNITEMS_LNK");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE TOOLEQUIPITEMS_ITMCOD IN (SELECT TOOLEQUIPITEMS_ITMCOD FROM HSE_TOOLEQUIPITEMS WHERE TOOLEQUIPITEMS_LNK = '%s')",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}*/
	//return CCommonCategoryWrapper::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);

	if(strFieldName == "EMPLOYEE_SIT") {
		CString strCompanyCode = FormGetField("CMN_EMPLOYEE","EMPLOYEE_CMPNYCOD");
		if(strCompanyCode.IsEmpty()) {
			strCompanyCode = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (STE_CMPNYNAME = '%s')",strCompanyCode);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}
	if(strFieldName == "COMPPRFSUBSDRS_CMPNAM") {
		CString strCompanyCode = FormGetField("HSE_COMPPRF","COMPPRF_COD");
		if(strCompanyCode.IsEmpty()) {
			strCompanyCode = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (COMPPRF_COD != '%s' AND COMPPRF_COD NOT IN (SELECT COMPPRFSUBSDRS_CMPNAM FROM HSE_COMPPRFSUBSDRS WHERE COMPPRFSUBSDRS_LNK = '%s' AND COMPPRFSUBSDRS_CMPNAM IS NOT NULL))",strCompanyCode,strCompanyCode);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}
	//replaced by unique value in custom attribute
	/*if(strFieldName == "ACDNTENTINJRDPRSN_EMPNUM") {
		CString strLinkFld = FormGetField("HSE_ACDNTENTINJRDPRSN","ACDNTENTINJRDPRSN_LNK");
		if(strLinkFld.IsEmpty()) {
			strLinkFld = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE ACDNTENTINJRDPRSN_EMPNUM NOT IN (SELECT ACDNTENTINJRDPRSN_EMPNUM FROM HSE_ACDNTENTINJRDPRSN WHERE ACDNTENTINJRDPRSN_LNK = %s AND ACDNTENTINJRDPRSN_EMPNUM IS NOT NULL)",strLinkFld);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}*/
		
	if(strFieldName == "CNSQNCSEXSTCNTRLMSR_DTLCOD") {
		CString strMsrCntrlGrp = FormGetField("HSE_CNSQNCSEXSTCNTRLMSR","CNSQNCSEXSTCNTRLMSR_HRCHYCTRL");
		if(strMsrCntrlGrp.IsEmpty()) {
			strMsrCntrlGrp = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (HRCTRLDTL_LNK = '%s')",strMsrCntrlGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}
	/*if(strFieldName == "ACDNTENT_RMKRSNDPRCTNRYACTN") {		
		CString strSQL;
		strSQL.Format("WHERE (EMPLOYEE_WSHOP = (SELECT TOP 1 HSEPLC_OWNRDPRT FROM HSE_HSEPLC))");
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();	
	}*/
	return S_OK;
}

CString CHSEMSCommonCategory::GetTableNameBrwsOnEmp(CString FieldName)
{
	CString strTablepart = "";
	CString DeptWhereCondition = "";
	CString LnkFldName;
	strTablepart  = FieldName;
	strTablepart.Replace("_EMPNUM","");
	CString strTableNamePart = CString (strTablepart);
	CString strTableName = CString("HSE_")+ CString(strTableNamePart);
	return strTableName;
}

CString CHSEMSCommonCategory::GetLnkFldNameForBrowseTable(CString strTableName)
{
	CString LnkFldName = "";
	CString strTablepart = "";
	strTablepart  = strTableName;
	strTablepart.Replace("HSE_","");

	if(strTableName == "HSE_ADTPLNENTAUDITORS")
		LnkFldName = CString("LINK");
	else if(strTableName == "HSE_ADTPLNENTAUDITEES")
		LnkFldName = CString("LINK");
	else
		LnkFldName = CString(strTablepart)+CString("_LNK");
	return LnkFldName;
}

CString CHSEMSCommonCategory::FilterAditeesEmpOnAudteeDpt(CString ActivityKeyField)
{
	CString DeptWhereCondition = "";
	CString strSQL=("Select ADTPLNENTACTVT_ADITEDPRT From HSE_ADTPLNENTACTVT WHERE MAINLINK = '"+ActivityKeyField+"'");
	CString AuditeeDept=GetRecordSetData(strSQL,1);
	DeptWhereCondition.Format("(EMPLOYEE_WSHOP ='%s')",AuditeeDept);
	return DeptWhereCondition;
}

CString CHSEMSCommonCategory::GetSlctdEmp(CString strFieldName,CString strTableName,CString LnkFieldName,CString LnkFieldValue)
{
	CString strSQL;
	CString strCriteria;
	strSQL.Format("SELECT "+strFieldName+" FROM "+strTableName+" WHERE "+LnkFieldName +" = '"+LnkFieldValue+"'");
	CString strSelectedEmployees=GetRecordSetData(strSQL,-1);
	strSelectedEmployees.Replace("\2","','");
	return strSelectedEmployees;
}

CString CHSEMSCommonCategory::FilterEmployees(CString DeptWhereCondition,CString strSelectedEmployees)
{
	CString strCriteria = "";		
	if(DeptWhereCondition == "")		
		strCriteria.Format("WHERE (EMPLOYEE_CODE NOT IN ('%s'))",strSelectedEmployees);
	else		
		strCriteria.Format("WHERE (EMPLOYEE_CODE NOT IN ('%s') and %s )",strSelectedEmployees,DeptWhereCondition);
	return strCriteria;

}

CString CHSEMSCommonCategory::GetAllEmployees(CString DeptWhereCondition)
{
	CString strCriteria;
	if(DeptWhereCondition == "")
		strCriteria="";
	else
		strCriteria.Format("WHERE  %s",DeptWhereCondition);
	return strCriteria;

}

CString CHSEMSCommonCategory::FilterSubClassOnSelectedClass(CString strSubFormTag , CString strFieldName)
{
	CString strSQL;
	CString strClass=CString(strSubFormTag);
	strClass.Replace("HSE_TG","");
	strClass=CString(strClass)+"_CLS";
	CString strTableName=CString(strSubFormTag);
	strTableName.Replace("HSE_TG","HSE_");
	//m_strScreenTag=strSubFormTag;
	CString strClassValue=FormGetField(strTableName,strClass);
	CString strSubClass=CString(strSubFormTag);
	strSubClass.Replace("HSE_TG","");
	strSubClass=CString(strSubClass)+"_SUBCLS";
	if(strcmp(strSubClass,strFieldName)==0)
	{		
		if(strClass.IsEmpty())
			strSQL="";
		else
			strSQL.Format("WHERE (ATCHCLSFCDTL_LNK ='%s')",strClassValue);
	}
	return strSQL;
}

/*Commented Function : GetMaxTracingSerial,check it in Source safe*/

HRESULT CHSEMSCommonCategory::DisplayTabSelected(tag_TabSelected *pTabSelected)
{
	CString strSubFormTag(pTabSelected->SubForm_Tag);		   	
	/*
	 * Moved to FirstSysDB
	 * if(strSubFormTag.Find("PTWSRGSTRCNFRM") == -1 && strSubFormTag != "HSE_TGCNTRCTR_CNTRCTS")		// Second part of condition "strSubFormTag.Find("HSE_TGPTWSRGSTRCNFRMTN")" to handle problem 
																										// of existing TRC in tag TODO : Should find cleaner way.
																										//&& strSubFormTag.Find("TRC") != -1 moved to FirstSysDB
	{
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
	}*/
	if(strSubFormTag == "HSE_TGVCLACCDNTQSTNSENTRY")
	{
		EnableToolbar(strSubFormTag,TOOLBAR_NEW,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
		EnableToolbar(strSubFormTag,TOOLBAR_COPY,FALSE);
	}

	return S_OK;
}

/*Commented Function : InsertActionRecordIntoTracingTab,check it in Source safe*/

/*
This function check if there is exist new reject reason in reject reason tab according to 
flag in reject reason tab
This will be changed according to CR03 by Ahmed Ali 21-6-2010
*/
bool CHSEMSCommonCategory::IsNewRjcRsnExist(CString strModuleName,CString KeyField)
	{				
		CString strSQL;
		strSQL.Format("SELECT COUNT(0) FROM HSE_RJCTRSN WHERE RJCTRSN_MODULETYPE = '%s' AND RJCTRSN_LINKWITHMAIN = '%s' AND RJCTRSN_TRACINGLNK = 0 ",strModuleName,KeyField);
		//commented by a.ali,polydb_task
		//int nRejectReasonCount=atoi((char*)m_pSilentRS->GetRecordSetData(strSQL.AllocSysString(),-1));
		int nRejectReasonCount=atoi(GetRecordSetData(strSQL,-1));
		if(nRejectReasonCount > 0)
			return true;
		else 
		return false;
	}


/*Commented Function : UpdateRjctRsnSrl,check it in Source safe*/

bool CHSEMSCommonCategory::CheckPolicy(CString fieldName)
{
	CString strSQL="Select " +fieldName+CString( " From HSE_HSEPLC ");
	CString IsRskAssmntRqrd=GetRecordSetData(strSQL,1);
	if(IsRskAssmntRqrd=="1" || IsRskAssmntRqrd=="True")
		return true;
	else
		return false;
}

CString CHSEMSCommonCategory::GetPolicyTabField(CString strTableName,CString fieldName)
{
	CString strSQL="Select " +fieldName+CString( " From ") + strTableName;
	CString strValue=GetRecordSetData(strSQL,1);
	return strValue;
}

/*Commented Function : UpdateStatusForAudit,check it in Source safe*/

/*Commented Function : UpdateStatus,check it in Source safe*/

bool CHSEMSCommonCategory::EnableField(TOOLBARBUTTONCLICKED *pToolBarButtonClicked,CString FieldToCheck,CString FieldToEnable)
{
	CString tableName(pToolBarButtonClicked->Table_Name);
	CString subForm_Tag(pToolBarButtonClicked->Form_Tag);
	CString strEnableField=FormGetField(tableName,FieldToCheck);//"LOSSACDNTENT_HNDLPTNLREC"
	if(strEnableField=="1")
	{
		SetFieldVisibleAttrib(subForm_Tag,FieldToEnable,"ALWAYSLOCKED");		
	}
	return true;
}

/**
 *	Return number of record in table 
 *	@param tableName
 */
int CHSEMSCommonCategory::getNumberOfRecord(CString tableName,CString strKeyField,CString ColumnName)
{
	CString strSQL="Select Count("+ColumnName+") From "+tableName+" WHERE "+ColumnName+" = '"+strKeyField+"'";
	CString strRecordCount=GetRecordSetData(strSQL,1);
	int nRecordCount=atoi(strRecordCount);
	return nRecordCount;
}


/**
 *	Return number of Previous Injuries for the employee 
 *	@param tableName
 */
CString CHSEMSCommonCategory::getPrevInjry(CString strTableNam,CString strFldNam,CString strEmpNum,CString strSit,CString strLoc)
{
	CString strSQL;
	//strSQL.Format("SELECT COUNT(0) AS EMP_PREV_INJUIRES FROM %s WHERE %s = %s",strTableNam,strFldNam,strEmpNum);
	//strSQL.Format("SELECT (SELECT COUNT(0) AS EMP_PREV_INJUIRES FROM HSE_INCDNTFLSHRPRTINJURDPRSNS WHERE INCDNTFLSHRPRTINJURDPRSNS_EMPNUM = %s AND IncdntFlshRprtInjurdPrsns_Lnk NOT IN (SELECT IncdntMdclRprtInjrdPrsns_LNK FROM HSE_INCDNTMDCLRPRTINJRDPRSNS)) + (SELECT COUNT(0) AS EMP_PREV_INJUIRES FROM HSE_INCDNTMDCLRPRTINJRDPRSNS WHERE IncdntMdclRprtInjrdPrsns_EMPNUM = %s)",strEmpNum,strEmpNum);		
	strSQL.Format("SELECT "
	"(SELECT COUNT(0) AS EMP_PREV_INJUIRES FROM HSE_INCDNTFLSHRPRTINJURDPRSNS INNER JOIN HSE_INCDNTFLSHRPRT "
	"ON HSE_INCDNTFLSHRPRTINJURDPRSNS.IncdntFlshRprtInjurdPrsns_Lnk = HSE_INCDNTFLSHRPRT.MAINLINK "
	"WHERE INCDNTFLSHRPRTINJURDPRSNS_EMPNUM = '%s' "
	"AND IncdntFlshRprtInjurdPrsns_Lnk NOT IN (SELECT IncdntMdclRprtInjrdPrsns_LNK FROM HSE_INCDNTMDCLRPRTINJRDPRSNS) "
	"AND HSE_INCDNTFLSHRPRT.INCDNTFLSHRPRT_SIT = '%s' "
	"AND HSE_INCDNTFLSHRPRT.INCDNTFLSHRPRT_LOC = '%s' )" 
	"+ (SELECT COUNT(0) AS EMP_PREV_INJUIRES FROM HSE_INCDNTMDCLRPRTINJRDPRSNS WHERE IncdntMdclRprtInjrdPrsns_EMPNUM = '%s')",strEmpNum,strSit,strLoc,strEmpNum);
	//AfxMessageBox(strSQL);
	CString strRecordCount=GetRecordSetData(strSQL,1);	
	return strRecordCount;
}


/**
 * This function set 0 as a flag to indecate that a new reject reason is inserted in reject reason tab
 */
bool CHSEMSCommonCategory::SetRjctRsnFlag(TOOLBARBUTTONCLICKED *pToolBarButtonClicked,CString KeyField)
{
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString tableName(pToolBarButtonClicked->Table_Name);
	strSubFormTag.Replace("HSE_TG","");
	//CString fieldName=tableName.Replace("HSE_","")+"_SRL";
	CString fieldName=strSubFormTag+"_FLG"; //no use 
	CString strBtnName=(pToolBarButtonClicked->Button_Name); //no use 
	if(pToolBarButtonClicked->iComplete==0)
	{
		if((strSubFormTag.Find("RJCRSN")!=-1||strSubFormTag.Find("REJRSN")!=-1)||strSubFormTag.Find("REJRES")!=-1&& strBtnName=="SAVE")
		{
			bool bIsNewRjcRsnExist=IsNewRjcRsnExist(tableName,KeyField);
			if(bIsNewRjcRsnExist==true)
			{
				*pToolBarButtonClicked->iCancle=1;
				ShowMessageEx(IDS_RJRE,"",IV_NOICON,"Error");
				return false;
			}
			else
			{
				FormSetField(tableName,fieldName,"0");
				return true;
			}
		}
		else
			return true;
	}
}

/*
	This function display message to user with two buttons ok|Cancel 
	if user select ok it delete last reject reason and approve record 
	@param reject reason table name
	******************************************************************
	Cahnged on 21-6-2010 by Ahmed Ali CR03
	@param ModuleName and keyField
	******************************************************************
*/
bool CHSEMSCommonCategory::showApproveMessageBox(CString strModuleName,CString strKeyField)
{
	bool retVal = false;
	if(AfxMessageBox(IDS_RJCT_RSN_EXST,MB_OKCANCEL|MB_ICONEXCLAMATION) == IDOK)
	{
		CString strSQL;
		strSQL.Format("EXECUTE deleteRjctRsn '%s','%s'",strModuleName,strKeyField);
		ExecuteSQL(strSQL);		
		retVal = true;
	}
	return retVal;
}

/*
	Select employee type : company employee or non company employee
*/

void CHSEMSCommonCategory::selectEmployeeType(FIELDCHANGE *pFieldChange,CString strTableTagName)
{
	CString strEmpCompValue(pFieldChange->New_Value);
	CString mainTablePart=CString(strTableTagName);
	mainTablePart.Replace("HSE_TG","");
	CString strEmpNum=CString(mainTablePart)+CString("_EMPNUM");
	CString strNam=CString(mainTablePart)+CString("_NAM");
	CString strComp=CString(mainTablePart)+CString("_COMP");
	CString strDprt=CString(mainTablePart)+CString("_DPRT");
	CString strJobTtl=CString(mainTablePart)+CString("_JOBTTL");
	int nEmpCompValue=atoi(strEmpCompValue);
	if(nEmpCompValue==1)
	{	
		SetFieldVisibleAttrib(strTableTagName,strEmpNum,"MUST");
		SetFieldVisibleAttrib(strTableTagName,strNam,"ALWAYSLOCKED");
		SetFieldVisibleAttrib(strTableTagName,strComp,"ALWAYSLOCKED");
		SetFieldVisibleAttrib(strTableTagName,strDprt,"ALWAYSLOCKED");
		SetFieldVisibleAttrib(strTableTagName,strJobTtl,"ALWAYSLOCKED");
	} else if(nEmpCompValue==0) {			
		SetFieldVisibleAttrib(strTableTagName,strEmpNum,"ALWAYSLOCKED");
		SetFieldVisibleAttrib(strTableTagName,strNam,"MUST");
		SetFieldVisibleAttrib(strTableTagName,strComp,"NULL");
		SetFieldVisibleAttrib(strTableTagName,strDprt,"NULL");
		SetFieldVisibleAttrib(strTableTagName,strJobTtl,"NULL");
	}
}

CString CHSEMSCommonCategory:: GetTableName(CString strTag)
{
	CString strTableName;
	strTableName=CString(strTag);
	strTableName.Replace("HSE_TG","HSE_");
	return strTableName;
}

CString CHSEMSCommonCategory::GetTablePartOfFieldName(CString strTag)
{			
	CString strTablePart;
	strTablePart  = CString(strTag);
	//strTablePart.Replace("HSE_TG","");
	strTablePart.Replace("HSE_","");
	return strTablePart;
}

void CHSEMSCommonCategory::EmptyEmployeeInfoWhenChangeCompanyEmplyee(FIELDCHANGE *pFieldChange,CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strCompEmpChkBx = CString(strTablePart)+"_COMPEMP";
	CString strEmpNum = CString(strTablePart)+"_EMPNUM";
	CString strSubFormTag    = pFieldChange->SubForm_Tag;	
	if (strcmp(strCompEmpChkBx,strFieldName)==0)
	{
		CString strCompEmpChkBxVal=FormGetField(strTableName,strCompEmpChkBx);
		
		if(strCompEmpChkBxVal=="0")
		{		
			FormSetField(strTableName,strEmpNum," ");
			FormSetField(strTableName,"Emp_Dprt34"," ");
			FormSetField(strTableName,"Emp_JobTtl44"," ");
			FormSetField(strTableName,"Emp_Nam24"," ");
		} else {
			FormSetField(strTableName,CString(strTablePart)+"_NAM"," ");
			FormSetField(strTableName,CString(strTablePart)+"_COMP"," ");
			FormSetField(strTableName,CString(strTablePart)+"_DPRT"," ");
			FormSetField(strTableName,CString(strTablePart)+"_JOBTTL"," ");
		}
		selectEmployeeType(pFieldChange,strSubFormTag);
	}
}

void CHSEMSCommonCategory::EmptySeverityRate(CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strSevrityRatClass =CString(strTablePart)+"_SVRTRATCLS";
	CString strSeverityRsting = CString(strTablePart)+"_SVRTYRAT";
	CString strLklhoodRate = CString(strTablePart)+"_LKLYHODRAT";
	CString strRskRnk = CString(strTablePart)+"_RSKRNK";
	CString strRskRnkDesc = CString(strTablePart)+"_RSKRNKDESC";

	CString strSevrityRatClass1 =CString(strTablePart)+"_SVRTRATCLS1";
	CString strSeverityRsting1 = CString(strTablePart)+"_SVRTYRAT1";
	CString strLklhoodRate1 = CString(strTablePart)+"_LKLYHODRAT1";
	CString strRskRnk1 = CString(strTablePart)+"_RSKRNK1";
	CString strRskRnkDesc1 = CString(strTablePart)+"_RSKRNKDESC1";

	if(strcmp(strSevrityRatClass,strFieldName)==0)
	{
	
		FormSetField(strTableName,strSeverityRsting,"");
		FormSetField(strTableName,strRskRnk,"");
		FormSetField(strTableName,strRskRnkDesc,"");
	}

	if(strcmp(strSevrityRatClass1,strFieldName)==0)
	{
		FormSetField(strTableName,strSeverityRsting1,"");
		FormSetField(strTableName,strRskRnk1,"");
		FormSetField(strTableName,strRskRnkDesc1,"");
	}
}


void CHSEMSCommonCategory::EmptySeverityRankInChangeSeverityRating(CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strSeverityRsting = CString(strTablePart)+"_SVRTYRAT";
	CString strLklhoodRate = CString(strTablePart)+"_LKLYHODRAT";
	CString strRskRnk = CString(strTablePart)+"_RSKRNK";
	CString strRskRnkDesc = CString(strTablePart)+"_RSKRNKDESC";

	CString strSeverityRsting1 = CString(strTablePart)+"_SVRTYRAT1";
	CString strLklhoodRate1 = CString(strTablePart)+"_LKLYHODRAT1";
	CString strRskRnk1 = CString(strTablePart)+"_RSKRNK1";
	CString strRskRnkDesc1 = CString(strTablePart)+"_RSKRNKDESC1";

	if(	   strRskRnk      == "RSKASSMNTENTPTNLHZRD_RSKRNK" 
		|| strRskRnk1     == "RSKASSMNTENTPTNLHZRD_RSKRNK1"
		|| strRskRnkDesc  == "RSKASSMNTENTPTNLHZRD_RSKRNKDESC"
		|| strRskRnkDesc1 == "RSKASSMNTENTPTNLHZRD_RSKRNKDESC1"
		|| strRskRnk	  == "PTNLHZRDENT_RSKRNK"
		|| strRskRnkDesc  == "PTNLHZRDENT_RSKRNKDESC")
		return;
	if(strcmp(strSeverityRsting,strFieldName)==0 || strcmp(strLklhoodRate,strFieldName)==0)
	{
		
		FormSetField(strTableName,strRskRnk,"");
		FormSetField(strTableName,strRskRnkDesc,"");
	}

	if(strcmp(strSeverityRsting1,strFieldName)==0 || strcmp(strLklhoodRate1,strFieldName)==0)
	{
		FormSetField(strTableName,strRskRnk1,"");
		FormSetField(strTableName,strRskRnkDesc1,"");
	}
}

void CHSEMSCommonCategory::EmptySiteHirarchyInChange(CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strSite = CString(strTablePart)+"_SIT";
	CString strLocationType = CString(strTablePart)+"_LOCTYP";
	CString strWorkLocation = CString(strTablePart)+"_WRKLOC";

	if(strcmp(strSite,strFieldName)==0)
	{
		FormSetField(strTableName,strLocationType,"");
		FormSetField(strTableName,strWorkLocation,"");
	}
	else if(strcmp(strLocationType,strFieldName)==0)
			FormSetField(strTableName,strWorkLocation,"");
}

void CHSEMSCommonCategory::EmptyHazardHirarchyInChange(CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strHazardCategory = CString(strTablePart)+ "_HZRDCTG";
	CString strHazardGroup = CString(strTablePart)+ "_HZRDGRP";
	CString strHarmType = CString(strTablePart)+ "_HRMTYP";
	
	if(strcmp(strHazardGroup,strFieldName)==0)
	{
		FormSetField(strTableName,strHazardCategory,"");
		FormSetField(strTableName,strHarmType,"");
	}

	if(strcmp(strHazardCategory,strFieldName)==0)
		FormSetField(strTableName,strHarmType,"");
}

void CHSEMSCommonCategory::EmptyDetlCodeInChangGrp(CString strTableName ,CString strFieldName,CString strTablePart)
{
	CString strGropu      = CString(strTablePart)+"_GRP";
	CString strDetails    = CString(strTablePart)+"_DTLCOD";
	CString strSUBDetails = CString(strTablePart)+"_SUBDTL";	
	FormSetField(strTableName,strDetails,"");
	FormSetField(strTableName,strSUBDetails,"");
}

void CHSEMSCommonCategory::EmptySubClassInChangeClass(CString strTableName ,CString strFieldName,CString strTablePart)
{
	//Bugs-10-2021.51
	if (strTablePart=="INJRYTYP")
		return;
	//////////////////////////////
	CString strClass = CString(strTablePart)+"_CLS";
	CString strSubClass = CString(strTablePart)+"_SUBCLS";

	if (strcmp(strClass,strFieldName)==0)
		FormSetField(strTableName,strSubClass,"");
}

//Helper function related to CR03-3
void CHSEMSCommonCategory::Reject(CString strSourceModule,CString strKeyField)
{		
	CString strDefValues;
	CString strCriteria;
	//open reject screen locked
	setgbOpenRejectScreenLocked(false);		
	strCriteria.Format("SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN= '%s') AND (RJCTRSN_MODULETYPE= '%s') AND (RJCTRSN_TRACINGLNK = 0)",strKeyField,strSourceModule);	
	strDefValues.Format("RJCTRSN_LINKWITHMAIN|%s|RJCTRSN_MODULETYPE|%s",strKeyField,strSourceModule);
	m_pCategory->GetHSEObj()->setstrSrcForRjctRsn(strSourceModule);
	m_pCategory->GetHSEObj()->setstrKeyValForRjctScr(strKeyField);
	//Bugs-3-2021.63 screen should open in modal mode
	ShowScreen(0,"HSE_TGRJCTRSN","Reject Reason",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);
}

//Helper function related to RQ-SF-9-1-23_3_1_4 (Accept CAR) Button
void CHSEMSCommonCategory::Accept(CString strSourceModule,CString strKeyField,bool bLocked)
{		
	CString strDefValues;
	CString strCriteria;
	//open reject screen locked
	setgbOpenRejectScreenLocked(false);		
	if (strSourceModule!="")
		strCriteria.Format("SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= %s) AND (CrRvwInf_MdlTyp= '%s')",strKeyField,strSourceModule);	
	else
		strCriteria.Format("SELECT * FROM HSE_CrRvwInf WHERE (CrRvwInf_LnkWthMn= %s)",strKeyField);	
	strDefValues.Format("CrRvwInf_LnkWthMn|%s|CrRvwInf_MdlTyp|%s",strKeyField,strSourceModule);
	m_pCategory->GetHSEObj()->setstrSrcForAccptRsn(strSourceModule);
	m_pCategory->GetHSEObj()->setstrKeyValForAccptScr(strKeyField);
	//Bugs-3-2021.63 screen should open in modal mode
	ShowScreen(0,"HSE_CrRvwInf","CAR Review Info",NORMAL_MODE,true,strCriteria,"",strDefValues,"",bLocked);
}

//Helper function related to RQ-SF-9-1-23_4_1_5 (Approve CAR) Button
void CHSEMSCommonCategory::Approve(CString strSourceModule,CString strKeyField,bool bLocked)
{		
	CString strDefValues;
	CString strCriteria;
	//open reject screen locked
	setgbOpenRejectScreenLocked(false);		
	strCriteria.Format("SELECT * FROM HSE_CrApprvlInf WHERE (CrApprvlInf_LnkWthMn= %s) AND (CrApprvlInf_MdlTyp= '%s')",strKeyField,strSourceModule);	
	strDefValues.Format("CrApprvlInf_LnkWthMn|%s|CrApprvlInf_MdlTyp|%s",strKeyField,strSourceModule);
	m_pCategory->GetHSEObj()->setstrSrcForApprvlRsn(strSourceModule);
	m_pCategory->GetHSEObj()->setstrKeyValForApprvlScr(strKeyField);
	//Bugs-3-2021.63 screen should open in modal mode
	ShowScreen(0,"HSE_CrApprvlInf","CAR Approve Info",NORMAL_MODE,true,strCriteria,"",strDefValues,"",bLocked);
}

//Set key field part as a member variable in class HSECommonCategory
//helper function to set rejected record in tracing tab
void CHSEMSCommonCategory::setSrcKyFldNm(CString strKeyField)
{
	//polydb_task
	m_pCategory->GetHSEObj()->gstrSrcKyFldNm=strKeyField;
}

//get key field part as a member variable in class HSECommonCategory
//helper function to set rejected record in tracing tab
CString CHSEMSCommonCategory::getSrcKyFldNm()
{
	//polydb_task
	return m_pCategory->GetHSEObj()->gstrSrcKyFldNm;
	//return "";
}

//Set screen tag in class HSECommonCategory
//helper function to set rejected record in tracing tab
void CHSEMSCommonCategory::setOwnerScreenTag(CString strScreenTag)
{	
	m_pCategory->GetHSEObj()->gstrScreen_tag=strScreenTag;
}

//get screen tag in class HSECommonCategory
//helper function to set rejected record in tracing tab
CString CHSEMSCommonCategory::getOwnerScreenTag()
{	
	return m_pCategory->GetHSEObj()->gstrScreen_tag;
}


//Helper function related to CR03-2
void CHSEMSCommonCategory::viewRejectReason(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//open reject screen locked
	setgbOpenRejectScreenLocked(true);
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strSubFormTag=pCustomButtonClicked->SubForm_Tag;
	CString strFormTag=pCustomButtonClicked->Form_Tag;
	CString strModulefrmSource="";			
	m_pCategory->GetHSEObj()->m_ScreenTagForTracing.Lookup(strFormTag,strModulefrmSource);
	if(strModulefrmSource=="")
	{
		if(strFormTag.Find("CAR")!= -1)
			strModulefrmSource = "CRCTVEACCENT";
		else if(strFormTag.Find("ADT")!= -1)
			strModulefrmSource = "ADTPLNENT";
		else if(strFormTag.Find("PERFM")!= -1)
			strModulefrmSource = "PERFMSRMNTENT";
		else if(strFormTag.Find("TGCR")!= -1) //New CAR module RQ-SF-9-1-23_2_4 (Tracing) Tab
			strModulefrmSource = "CRCTVEACCENT";
		else if(strFormTag.Find("TGACT")!= -1) //New CAR module RQ-SF-9-1-23_6_1_3 
			strModulefrmSource = "CRCTVEACCENT";
	}
	CString strCriteria;
	CString strTableName=CString(strBtnName);
	strTableName.Replace("_VWRJCTRSNS","");
	strTableName=CString("HSE_")+strTableName;
	CString strActionType=CString(strBtnName);
	strActionType.Replace("_VWRJCTRSNS","");
	strActionType+=CString("_ACCDESC");
	CString strActionTypeValue=FormGetField(strTableName,strActionType);
	if(strActionTypeValue=="Rejected from Confirmation" || strActionTypeValue=="Rejected from follow-up" || strActionTypeValue == "REJECT"|| strActionTypeValue == "Rejected" || strActionTypeValue=="Entry Rejected" || strActionTypeValue=="CAR Rejected") //RQ-SF-9-1-23_3_1_3 (Reject CAR) Button:added "Entry Rejected" , RQ-SF-9-1-23_6_1_3 "CAR Rejected"
	{
		CString strFieldName=CString(strBtnName);
		strFieldName.Replace("_VWRJCTRSNS","");
		strFieldName+=CString("_MAINLNK");
		CString strMainLink=FormGetField(strTableName,strFieldName);
		CString strLinkField=CString(strBtnName);
		strLinkField.Replace("_VWRJCTRSNS","");
		strLinkField+=CString("_LNK");
		CString strLinkValue=FormGetField(strTableName,strLinkField);			
		
		//strCriteria="WHERE (RJCTRSN_LINKWITHMAIN = '"+strLinkValue+"') AND (RJCTRSN_TRACINGLNK = '"+strMainLink+"') AND ((RJCTRSN_MODULETYPE ='"+strModulefrmSource+"') OR (RJCTRSN_MODULETYPE ='"+strModulefrmSource2nd+"'))";		
		//strCriteria="SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN = '"+strLinkValue+"') AND (RJCTRSN_TRACINGLNK = 0) AND ((RJCTRSN_MODULETYPE ='"+strModulefrmSource+"') OR (RJCTRSN_MODULETYPE ='"+strModulefrmSource2nd+"'))";		
		
		//HSM: L1 and L2 are for 'Level 1' and 'Level 2' as I understood from A. Ali, so it is prefered to let the expression be 'Like'
		//strModulefrmSource+=CString("-L1");
		//CString strModulefrmSource2nd=CString(strModulefrmSource);
		//strModulefrmSource2nd.Replace("-L1","-L2");
		//strCriteria="SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN = '"+strLinkValue+"') AND ((RJCTRSN_MODULETYPE ='"+strModulefrmSource+"') OR (RJCTRSN_MODULETYPE ='"+strModulefrmSource2nd+"'))";		
		strCriteria="SELECT * FROM HSE_RJCTRSN WHERE (RJCTRSN_LINKWITHMAIN = '"+strLinkValue+"') AND (RJCTRSN_MODULETYPE like '"+strModulefrmSource+"%')";		
		
		//strCriteria="WHERE (RJCTRSN_LINKWITHMAIN = '"+strLinkValue+"') AND (RJCTRSN_TRACINGLNK = '"+strMainLink+"') AND (RJCTRSN_MODULETYPE ='"+strModulefrmSource+"')";		
		//ShowListScreen("HSE_TGRJCTRSN","",strCriteria,"RJCTRSN_RJCTRSN","",TRUE,NULL);
		tagMultiSelectedRows strFieldsValues;
		strFieldsValues.lCount = 0;
		ShowScreen(0,"HSE_TGRJCTRSN","Reject Reason",NORMAL_MODE,true,strCriteria,"","","",true);			
	}
}
//set global variale gbOpenRejectScreenLocked
void CHSEMSCommonCategory::setgbOpenRejectScreenLocked(bool bOpenRejectScreenLocked)
{
	//polydb_task
	m_pCategory->GetHSEObj()->gbOpenRejectScreenLocked=bOpenRejectScreenLocked;
}

//get global variale gbOpenRejectScreenLocked
bool CHSEMSCommonCategory::getgbOpenRejectScreenLocked()
{	
	return m_pCategory->GetHSEObj()->gbOpenRejectScreenLocked;
}

bool CHSEMSCommonCategory::getgbOpenAcceptScreenLocked()
{	
	return m_pCategory->GetHSEObj()->gbOpenAcceptScreenLocked;
}



//set global variable gbUpdateRejectedRecord;
void CHSEMSCommonCategory::setgbUpdateRejectedRecord(bool bRejectCurrentRecord)
{
	//polydb_task
	m_pCategory->GetHSEObj()->gbUpdateRejectedRecord=bRejectCurrentRecord;
}

void CHSEMSCommonCategory::setgbUpdateAcceptedRecord(bool bAcceptCurrentRecord)
{
	//polydb_task
	m_pCategory->GetHSEObj()->gbUpdateAcceptedRecord=bAcceptCurrentRecord;
}

void CHSEMSCommonCategory::setgbUpdateApprovedRecord(bool bApproveCurrentRecord)
{
	//polydb_task
	m_pCategory->GetHSEObj()->gbUpdateApprovedRecord=bApproveCurrentRecord;
}

//get global variable gbUpdateRejectedRecord;
bool CHSEMSCommonCategory::getgbUpdateRejectedRecord()
{	
	return m_pCategory->GetHSEObj()->gbUpdateRejectedRecord;
}

bool CHSEMSCommonCategory::getgbUpdateAcceptedRecord()
{	
	return m_pCategory->GetHSEObj()->gbUpdateAcceptedRecord;
}

bool CHSEMSCommonCategory::getgbUpdateApprovedRecord()
{	
	return m_pCategory->GetHSEObj()->gbUpdateApprovedRecord;
}
/**
 * get array of selected record(s) key field values
 * return [keyFldVal_1],[keyFldVal_2],...
 */
CString CHSEMSCommonCategory::GetKeyField(CString strTableName,MultiSelectedRows *pMultiSelectedRows)
{
	CString keyFldNam		 = "";
	CString KeyFieldValue	 = "";
	CString strKeyFieldArray = "";

	keyFldNam = GetKeyFieldName(strTableName);

	int RcrdCount = 0;
	RcrdCount = pMultiSelectedRows->lCount;	
	for(int nSel = 0 ;nSel < RcrdCount;nSel++)
	{
	  CString strRec= pMultiSelectedRows->pRow[nSel].strFullRecord;    
	  CString strKeyField	= GetFieldValue(strRec,keyFldNam);		
	  strKeyFieldArray +=strKeyField;
	  strKeyFieldArray +=",";
	}
	strKeyFieldArray.TrimRight(",");
	strKeyFieldArray.TrimLeft(",");
	return strKeyFieldArray;
}

/**
*	@Author Ahmed Ali
*	Requirement ID : HSE_CR10_6-11
*	Date : 30-10-2012
*		
*	Check flag in comment tab if flag == 1 "Record status has been changed"
*		disable delete and save button
*	else
*		Check for user if current user == user who entered record 
*			enable save and delete buttons
*		else 
*			disable save and delete buttons
*	@param pRecordRepostion
*   Update : instead of using pRecordRepostion as parameter change it to strFormTag,strSubFormTag to be used in
*   AddNew record cause this method is not used when we add new record "reposition is not fired"
*/
//void CHSEMSCommonCategory::manageCommentsTabToolBar(RECORDREPOSTION *pRecordRepostion)
void CHSEMSCommonCategory::manageCommentsTabToolBar(CString strFormTag,CString strSubFormTag)
{
	CString strTableName = GetTrueTableName (66,strFormTag.AllocSysString(),strSubFormTag.AllocSysString());		
	CString strFlag = FormGetField(strTableName , "FLAG");	
	if(strFlag == "1"){
		disableCmntTabFields(strSubFormTag);
		disableCmntTabToolBar(strFormTag,strSubFormTag);
	} else if(strFlag == "0"){ 
		CString strCommentedBy = FormGetField(strTableName,"CMNTDNEBY");		
		CString userName = GetUserLogin();		
		if(strCommentedBy.Compare(userName) == 0){
 			enableCmntTabFields(strSubFormTag);
			enableCmntTabToolBar(strFormTag,strSubFormTag);
		} else {
			disableCmntTabToolBar(strFormTag,strSubFormTag);
			disableCmntTabFields(strSubFormTag);
		}
	}
	//to handle case of first record
	else if(strFlag == ""){
		enableCmntTabFields(strSubFormTag);
		enableCmntTabToolBar(strFormTag,strSubFormTag);
		setCmntScreenSource(strFormTag,strSubFormTag,strTableName);
	}
}

/**
*	Disable Comment tab buttons(delete,save)
*/
void CHSEMSCommonCategory::disableCmntTabToolBar(CString strFormTag,CString strSubFormTag)
{
	EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,FALSE);
	EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_SAVE,FALSE);	
}

/**
*	Enable Comment tab buttons(delete,save)
*/
void CHSEMSCommonCategory::enableCmntTabToolBar(CString strFormTag,CString strSubFormTag)
{
	EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,TRUE);
	//polydb_task , enable save in tab cause an error
	EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_SAVE,TRUE);	
}

/**
*	Disable comment tab fields
*/
void CHSEMSCommonCategory::disableCmntTabFields(CString strSubFormTag)
{
	//commented by a.ali,polydb_task
	//get link_name as first parameter for LinkFieldsAttribSingle
	SetFieldVisibleAttrib(strSubFormTag,"CMNT","ALWAYSLOCKED");		
	//LinkFieldsAttribSingle(LPCTSTR Link_Name,"CMNT","",-1,"","ALWAYSLOCKED","","",0,"", "","","","",0,0,"");
}

/**
*	Enable comment tab fields
*/
void CHSEMSCommonCategory::enableCmntTabFields(CString strSubFormTag)
{
	//commented by a.ali,polydb_task
	//get link_name as first parameter for LinkFieldsAttribSingle
	SetFieldVisibleAttrib(strSubFormTag,"CMNT","MUST");	
	//LinkFieldsAttribSingle(LPCTSTR Link_Name,"CMNT","",-1,"","","","",0,"", "","","","",0,0,"");
}

/**
*	Set main screen caption in comment tab source screen Field
*/
void CHSEMSCommonCategory::setCmntScreenSource(CString strFormTag,CString strSubFormTag,CString strTableName)
{
	//commented by a.ali,polydb_task
	//CString strSourceScreenName = (char*)(m_BinaryOperationManager->GetScreenCaption(strFormTag.AllocSysString()));
	CString strSourceScreenName = GetScreenCaption();
//	FormSetField(strTableName,"SRCSCRN",strSourceScreenName);	
	//enableCmntTabFields(strSubFormTag);	
	//enableCmntTabToolBar(strFormTag,strSubFormTag);
}

/**
 *	Open CAR screen
 */
void CHSEMSCommonCategory::openRelatedCARs(CString strCARNo)
{				
	CString strCriteria;		
	CString strDefValues;	
	strCriteria.Format("SELECT * FROM HSE_CRCTVEACCENT WHERE ( CRCTVEACCENT_CARMODELNUM = %s)",strCARNo);				
	strDefValues.Format("CRCTVEACCENT_CARMODELNUM|%s",strCARNo);		
	ShowScreen(0,"HSE_TGCRCTVEACCINQ","Corrective Action Inquery",NORMAL_MODE,true,strCriteria,"",strDefValues,"",false);
}


/**
 *	Generate CAR for Required actions under selected TXN
 */
void CHSEMSCommonCategory::generateCARSP(CString strTXN,CString strSubFormtag)
{
	CString strSQL = "EXECUTE sp_Generate_CARs "+strTXN+",'"+strSubFormtag+"'";
	ExecuteSQL(strSQL);		
}

/**
 * update CAR
 */
bool CHSEMSCommonCategory::updateCARSP(CString strTXN,CString strSubFormtag,CString strCARNum)
{
	bool bUpdateCARSuccessful = false;
	CString strSQL = "EXECUTE sp_Update_CARs "+strTXN+",'"+strSubFormtag+"',"+strCARNum;
	long lRetVal = ExecuteSQL(strSQL);	
	if(lRetVal != 0)
		bUpdateCARSuccessful = true;
	return bUpdateCARSuccessful;
}


/**
 * Cancel CAR
 */
void CHSEMSCommonCategory::cancelCARSP(CString strTXN,CString strSubFormtag,CString strCARNum) 
{
	CString strSQL = "EXECUTE cancelCAR "+strTXN+",'"+strSubFormtag+"',"+strCARNum;
	ExecuteSQL(strSQL);	
}

/*According to Change Request(CR12v21-102)
*Validate Transaction date based on Policy.Max TXN Late Days
*Validation Rule:
Any Transaction date must not exceed today'date and may be equivalent or exceed the difference between 
(TodayDate,Max TXN Late Days)
*Implemented By:Mai Talaat On 17/3/2013*/
bool CHSEMSCommonCategory::IsIssueDateValid(CString strIssueDate,CString strCmpnyNam)
{
	m_nMaxLateDays = GetMaxLateDays(strCmpnyNam);
	double dtTodayDate = GetServerDate_DOUBLE();//Get Today's Date
	COleDateTime dtIssue;
	ParseDateTime(&dtIssue,strIssueDate);
	dtTodayDate = dtTodayDate - m_nMaxLateDays;
	double daysOnly;	//int part of current date
	modf(dtTodayDate , &daysOnly);
	if(dtIssue.m_dt < daysOnly)
	{
		CString strMsg="TXN Date must be greater than Today and Maximum TXN Late Days";
		AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
		return false;
	}
	return true;
}

bool CHSEMSCommonCategory::IsAuditValidToAssignRes(CString strAdtTodate,CString strCmnyNam) {
	bool bAllowedToAsgnRes = false;
	if(strAdtTodate == "")
		return bAllowedToAsgnRes;
	int nAllowdAdtAdvDays = getAdtAdvDays(strCmnyNam);
	int nAdtMinNotificaionDays = getAdtMinNotificationDays(strCmnyNam);
	if(nAllowdAdtAdvDays == 0) {
		bAllowedToAsgnRes = true;
	}
	COleDateTime dtTodayDate = GetServerDate_DATE();	//Get Today's Date
	COleDateTime dtAdtToDate;
	ParseDateTime(&dtAdtToDate,strAdtTodate);
	double nDayDiff = 0;	  
	COleDateTimeSpan  diff = dtAdtToDate-dtTodayDate;
	//nDayDiff = diff.GetDays();
	modf(dtAdtToDate-dtTodayDate , &nDayDiff);
	dtTodayDate += COleDateTimeSpan( nAllowdAdtAdvDays+1, 0, 0, 0 );//dtTodayDate + nAllowdAdtAdvDays;

	if (dtTodayDate >= dtAdtToDate && nDayDiff > nAdtMinNotificaionDays) {
		bAllowedToAsgnRes = true;
	}
	return bAllowedToAsgnRes;
}

int CHSEMSCommonCategory::GetMaxLateDays(CString strCmpnyNam)//Define MaxLateDays as a member variable
{
	CString strSQL;
	strSQL.Format("SELECT ISNULL(HSEPLC_MXTXNLTDY,0) FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);		
	return atoi(GetRecordSetData(strSQL,1)); //Get Max TXN Late Days
}

int CHSEMSCommonCategory::getAdtAdvDays(CString strCmpnyNam) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(HSEPLC_ADVNCDTOASGNADTRES,0) FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);	
	return atoi(GetRecordSetData(strSQL,1)); 
}

int CHSEMSCommonCategory::getAdtMinNotificationDays(CString strCompnyNam) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(HSEPLC_ADTMNNTFCTNDAYS,0) FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCompnyNam);	
	return atoi(GetRecordSetData(strSQL,1)); 
}

/*changed function : GenerateTXNNo,to check original implementation check Source Safe*/

// Change Request : HSE_CR12v2_52
// By Mohamed Elghobshi
// Get Owner Department from policy.

CString CHSEMSCommonCategory::getOwnerDept(CString strCmpnyNam)
{
	CString strSQL;
	if(strCmpnyNam=="")
		strSQL.Format("SELECT HSEPLC_OWNRDPRT FROM dbo.HSE_HSEPLC");//By Hossam RQ-8-2021.4 16/9/2021
	else
		strSQL.Format("SELECT HSEPLC_OWNRDPRT FROM dbo.HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strOwnerDept=GetRecordSetData(strSQL,1);
	return strOwnerDept;
}

/**
 *	Get Company home currency
 */
CString CHSEMSCommonCategory::getDefCurrency(CString strCmpnyNam)
{
	CString strSQL;
	strSQL.Format("SELECT HMCRNY FROM dbo.HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strDefCurncy=GetRecordSetData(strSQL,1);
	return strDefCurncy;
}


/**
 * Filter Audit Checklist on Procedure Num  
 */
void CHSEMSCommonCategory::filterAdtChkLst(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick) 
{
	CString strLink = FormGetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_LNK");
	CString strSQL;	
	strSQL.Format("SELECT ISNULL(ADTPLNENTACTVT_REFNUM,0) AS ProNum FROM HSE_ADTPLNENTACTVT WHERE MAINLINK = '%s'",strLink);
	CString strProcedureNum = GetRecordSetData(strSQL,1);
	if(strProcedureNum != "0") {
		CString strSQL;
		//strSQL.Format("WHERE (ADTCHKLST_LNK ='%s')",strProcedureNum);
		strSQL.Format("WHERE (ADTCHCKLST_LNKWTHDTL ='%s')",strProcedureNum);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();
	}
}

/**
 *	Open required action pop up screen
 */
void CHSEMSCommonCategory::OpenRequiredActionScreen(CString ScreenTag,int ScreenMode)
{
	CString strSQL ;	
	bool bLocked = false;
	CString strMainLinkFeild = FormGetField("HSE_ADTCHCKLSTPLN" ,"MAINLINK");
	CString strFinding = FormGetField("HSE_ADTCHCKLSTPLN","ADTCHCKLSTPLN_FNDNG");	
	//Mail : 13/12/2020 18:32
	//Yes, if the (finding) field is empty, the (required actions) screen should display an error message, and don't open
	if(strFinding == "") {
		AfxMessageBox("Please insert finding first.");
		return;
	}
	CString strQurey;
	strQurey.Format("SELECT ADTPLNENTACTVT_CLSNAM FROM HSE_ADTPLNENTACTVT WHERE MAINLINK = (SELECT ADTCHCKLSTPLN_LNK FROM HSE_ADTCHCKLSTPLN WHERE MAINLINK = %s)",strMainLinkFeild);
	CString strProcedureCode = GetRecordSetData(strQurey,1);
	CString strDefValues;
	strDefValues.Format("ADTACTVTRQRDACTN_LNK|%s|ADTACTVTRQRDACTN_ACCDESC|%s|ADTACTVTRQRDACTN_VLTDPRCDR|%s",strMainLinkFeild,strFinding,strProcedureCode);	
	strSQL.Format("SELECT * FROM HSE_ADTACTVTRQRDACTN WHERE (ADTACTVTRQRDACTN_LNK = %s)",strMainLinkFeild);
	ShowScreen(0,"HSE_TGADTACTVTRQRDACTN","Required Actions",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}

/**
 * Set Changed record data (changed by - changed date time)
 * Action : user update existing record
 * note : this code not in specs document
 */
void CHSEMSCommonCategory::setChangedRecInfo(CString strTableName){				
	CString userName = GetUserLogin();		
	//CString strSrvrDate = GetFullDateTime("dd/mm/yyyy");				
	CString strSrvrDate = GetFullDateTime("%d/%m/%Y");					
	CString strChngdBy= CString(strTableName);
	strChngdBy.Replace("HSE_","");
	strChngdBy += "_ChngdBy";
	CString strChngdDatTim=CString(strTableName);
	strChngdDatTim.Replace("HSE_","");
	strChngdDatTim+="_ChngdDatTim";

	CString strTagName = CString(strTableName);
	strTagName.Replace("HSE_","HSE_TG");
	bool bChangedByFldExist = IsFieldExist(66,strTagName,"",strChngdBy);
	bool bChangedDateTimeFldExist   = IsFieldExist(66,strTagName,"",strChngdDatTim);
	if(bChangedByFldExist && bChangedDateTimeFldExist) {
		FormSetField(strTableName,strChngdBy,userName);		
		FormSetField(strTableName,strChngdDatTim,strSrvrDate);	
	}
}


long CHSEMSCommonCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode) {
	//AfxMessageBox("Open Screen function not yet implemented");
	return 0;
}

/*CHSEMSCommonCategory::OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode)
{	
	bool bIsAccdntCnfrmtnRqrd=CheckPolicy("HSEPLC_ACDNTCNFRMTNRQRD");
	if(bIsAccdntCnfrmtnRqrd==true) {
		//return AccidentCategory::OpenScreen(strCaption,strSQL,lMenuID,lParam,bModal,bFilterMode);
	} else {
		//Else message box displayed to user with this message 
		//"Accident Confirmation  checkbox must be checked to open this screen"
		ShowMessageEx(IDS_ACR,"",IV_NOICON,"Policy change required");
		return S_FALSE;
	}
}*/


HRESULT CHSEMSCommonCategory::ShowListScreen(CString strList_Tag,
											 CString strList_Title,
										     CString strSQL_Statement,	
											 CString strKeyField,
											 CString strRecKeyValue,
											 BOOL bMultiSelection,
											 tagMultiSelectedRows* pstrFieldsValues,
											 BOOL bIsModal,
											 CString strAttrib)	{
	SetModalScreen(bIsModal);
	tagMultiSelectedRows strFieldsValues;
	if(pstrFieldsValues == NULL) {		
		strFieldsValues.lCount = 0;
	}
	//return CCommonCategoryWrapper::ShowListScreen(strList_Tag,strList_Title,strSQL_Statement,strKeyField,strRecKeyValue,bMultiSelection,&strFieldsValues,strAttrib);			
	return S_OK;
}		   


void CHSEMSCommonCategory::LockTab(CString strMainScreenTag,CString strTabTag,BOOL Locked) {
	EnableTab(strTabTag,!Locked);
}

long CHSEMSCommonCategory::ShowHSEStpEffct(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_StpEffct;
	return ParentManage("HSE_TGSTPEFFCT",TRUE,TRUE,TRUE,"HSE_STPEFFCT",strCriteria);
}

long CHSEMSCommonCategory::ShowSite(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_STE;
	return ParentManage("HSE_TGSTE",TRUE,TRUE,TRUE,"HSE_STE",strCriteria);
}

long CHSEMSCommonCategory::ShowMsnRsLog(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MSNRSLOG;
	return ParentManage("HSE_TGMSNRSLOG",TRUE,TRUE,TRUE,"HSE_MSNRSLOG",strCriteria);
}

long CHSEMSCommonCategory::ShowSitSrvyTyps(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_Srvy_Typs;
	return ParentManage("HSE_TGSITSRVYTYPS",TRUE,TRUE,TRUE,"HSE_SITSRVYTYPS",strCriteria);
}

long CHSEMSCommonCategory::ShowDrlFndngs(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_DrlFndngs;
	CString strComeFrom = getOwnerScreenTag();
	BOOL bAllowUpdate = TRUE;
	BOOL bAllowAddNew = TRUE;
	BOOL bAllowDelete = TRUE;
	if(strComeFrom == "HSE_DRLLPLNINQ_ACTULSCNR") {
		bAllowUpdate = FALSE;
		bAllowAddNew = FALSE;
		bAllowDelete = FALSE;		
	}	
	return ParentManage("HSE_TGDRLFNDNGS",bAllowUpdate,bAllowAddNew,bAllowDelete,"HSE_DRLFNDNGS",strCriteria);	
}

long CHSEMSCommonCategory::ShowDrlFndngsUnPlndScnros(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_DrlFndngs;
	CString strComeFrom = getOwnerScreenTag();
	BOOL bAllowUpdate = TRUE;
	BOOL bAllowAddNew = TRUE;
	BOOL bAllowDelete = TRUE;
	if(strComeFrom == "HSE_DRLLPLNINQ_UNPLNDSCNROS") {
		bAllowUpdate = FALSE;
		bAllowAddNew = FALSE;
		bAllowDelete = FALSE;		
	}
	return ParentManage("HSE_TGDRLFNDNGSUNPLNDSCNROS",bAllowUpdate,bAllowAddNew,bAllowDelete,"HSE_DRLFNDNGSUNPLNDSCNROS",strCriteria);	
}


long CHSEMSCommonCategory::ShowSiteType(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SITE_TYPE;
	return ParentManage("HSE_TGSITETYPE",TRUE,TRUE,TRUE,"HSE_SITETYPE",strCriteria);	
}

long CHSEMSCommonCategory::ShowCountry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CNTRY;
	//return ParentManage("HSE_TGCNTRY",TRUE,TRUE,TRUE,"HSE_CNTRY",strCriteria);
	return ParentManage("CMN_COUNTRIES",TRUE,TRUE,TRUE,"CMN_COUNTRIES",strCriteria);
}

long CHSEMSCommonCategory::ShowLanguage(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LANG;
	return ParentManage("HSE_TGLANG",TRUE,TRUE,TRUE,"HSE_LANG",strCriteria);
}

long CHSEMSCommonCategory::ShowDepartment(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_DPRT;
	//return ParentManage("HSE_TGDPRT",TRUE,TRUE,TRUE,"HSE_DPRT",strCriteria);
	return ParentManage("CMN_DEPART",TRUE,TRUE,TRUE,"CMN_DEPART",strCriteria);	
}

long CHSEMSCommonCategory::ShowWorkLocation(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_WRKLOC;
	return ParentManage("HSE_TGWRKLOC",TRUE,TRUE,TRUE,"HSE_WRKLOC",strCriteria);
}

long CHSEMSCommonCategory::ShowTask(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TSK;
	return ParentManage("HSE_TGTSK",TRUE,TRUE,TRUE,"HSE_TSK",strCriteria);
}

long CHSEMSCommonCategory::ShowDetails(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_DTL;
	return ParentManage("HSE_TGDTL",TRUE,TRUE,TRUE,"HSE_DTL",strCriteria);
}

long CHSEMSCommonCategory::ShowCARBasis(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CARMODELBAS;
	return ParentManage("HSE_TGCARMODELBAS",TRUE,TRUE,TRUE,"HSE_CARMODELBAS",strCriteria);
}

void CHSEMSCommonCategory::LockScreen(BOOL bLock)
{
	BOOL bLockSubs = false;
	CCommonCategory::LockScreen(UNKNOWN,bLock,bLockSubs);
}

long CHSEMSCommonCategory::ShowRole(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ROL;
	return ParentManage("HSE_TGROL",TRUE,TRUE,TRUE,"HSE_ROL",strCriteria);
}

long CHSEMSCommonCategory::ShowVehicleAccidentQuestions(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCLQSTNS;
	return ParentManage("HSE_TGVCLQSTNS",TRUE,TRUE,TRUE,"HSE_VCLQSTNS",strCriteria);
}

long CHSEMSCommonCategory::ShowBasicReference(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_BASICREFERENCE;
	return ParentManage("HSE_TGBSCREF",TRUE,TRUE,TRUE,"HSE_BSCREF",strCriteria);
}

long CHSEMSCommonCategory::ShowEnvironmentCondition(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_ENVIRONMENTCONDITION;
	return ParentManage("HSE_TGENVRNMNTCND",TRUE,TRUE,TRUE,"HSE_ENVRNMNTCND",strCriteria);
}

long CHSEMSCommonCategory::ShowCurrency(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_CURRENCY;
	//return ParentManage("HSE_TGCRRNCY",TRUE,TRUE,TRUE,"HSE_CRRNCY",strCriteria);
	return ParentManage("CMN_CURENCY",TRUE,TRUE,TRUE,"CMN_CURENCY",strCriteria);
}

long CHSEMSCommonCategory::ShowSignatures(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_SIGNATURES;
	return ParentManage("HSE_TGSGNTUR",TRUE,TRUE,TRUE,"HSE_SGNTUR",strCriteria);
}

long CHSEMSCommonCategory::ShowInjuryType(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_INJURYTYPE;
	return ParentManage("HSE_TGINJRYTYP",TRUE,TRUE,TRUE,"HSE_INJRYTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowRewardType(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_REWARDTYPE;
	return ParentManage("HSE_TGRWRDTYP",TRUE,TRUE,TRUE,"HSE_RWRDTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowPotentialSeverity(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_POTENTIALSEVERITY;
	return ParentManage("HSE_TGPTNTLSVRTY",TRUE,TRUE,TRUE,"HSE_PTNTLSVRTY",strCriteria);
}

long CHSEMSCommonCategory::ShowPerformanceMeasureScore(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTSCOR;
	return ParentManage("HSE_TGPERFMSRMNTSCOR",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTSCOR",strCriteria);
}

//Moved to new class AccidentTypesCategory
//long CHSEMSCommonCategory::ShowAccidentType(CString strCriteria)
//{
//	m_nCurrentCard=ID_HSE_ACDNTTYP;
//	return ParentManage("HSE_TGACDNTTYP",TRUE,TRUE,TRUE,"HSE_ACDNTTYP",strCriteria);
//}

long CHSEMSCommonCategory::ShowAccidentMedium(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ACDNTMDM;
	return ParentManage("HSE_TGACDNTMDM",TRUE,TRUE,TRUE,"HSE_ACDNTMDM",strCriteria);
}

long CHSEMSCommonCategory::ShowTypeofLoss(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TYPLOSS;
	return ParentManage("HSE_TGTYPLOSS",TRUE,TRUE,TRUE,"HSE_TYPLOSS",strCriteria);
}

long CHSEMSCommonCategory::ShowTaskAuditResult(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TSKADTRSLT;
	return ParentManage("HSE_TGTSKADTRSLT",TRUE,TRUE,TRUE,"HSE_TSKADTRSLT",strCriteria);
}

long CHSEMSCommonCategory::ShowPTWType(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTWSTYP;
	return ParentManage("HSE_TGPTWSTYP",TRUE,TRUE,TRUE,"HSE_PTWSTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowRiskSeverityRatingClass(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKSVRTYRATCLS;
	return ParentManage("HSE_TGRSKSVRTRATCLS",TRUE,TRUE,TRUE,"HSE_RSKSVRTRATCLS",strCriteria);
}

long CHSEMSCommonCategory::ShowRiskLikelihoodRating(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKLKLYHODRAT;
	return ParentManage("HSE_TGRSKLKLYHODRAT",TRUE,TRUE,TRUE,"HSE_RSKLKLYHODRAT",strCriteria);
}


long CHSEMSCommonCategory::ShowEmployee(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EMP;
	//return ParentManage("HSE_TGEMP",TRUE,TRUE,TRUE,"HSE_EMP",strCriteria);
	return ParentManage("CMN_EMPLOYEE",TRUE,TRUE,TRUE,"CMN_EMPLOYEE",strCriteria);
}

long CHSEMSCommonCategory::ShowRiskRankDescription(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKRNKDESC;
	return ParentManage("HSE_TGRSKRNKDESC",TRUE,TRUE,TRUE,"HSE_RSKRNKDESC",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEProceduresList(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HSEPRCDRLIST;
	return ParentManage("HSE_TGHSEPRCDRLIST",TRUE,TRUE,TRUE,"HSE_HSEPRCDRLIST",strCriteria);
}

long CHSEMSCommonCategory::ShowPTWResponsibility(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PTWSRSPNSBLTY;
	return ParentManage("HSE_TGPTWSRSPNSBLTY",TRUE,TRUE,TRUE,"HSE_PTWSRSPNSBLTY",strCriteria);
}

long CHSEMSCommonCategory::ShowEntryRejectReasons(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ENTRJCRSN;
	return ParentManage("HSE_TGENTRJCRSN",TRUE,TRUE,TRUE,"HSE_ENTRJCRSN",strCriteria);
}

long CHSEMSCommonCategory::ShowHSERegulations(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HSERGLR;
	return ParentManage("HSE_TGHSERGLR",TRUE,TRUE,TRUE,"HSE_HSERGLR",strCriteria);
}

long CHSEMSCommonCategory::ShowAuditees(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AUDITEES;
	return ParentManage("HSE_TGAUDITEES",TRUE,TRUE,TRUE,"HSE_AUDITEES",strCriteria);
}

long CHSEMSCommonCategory::ShowAttachmentclassification(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ATCHCLSFC;
	return ParentManage("HSE_TGATCHCLSFC",TRUE,TRUE,TRUE,"HSE_ATCHCLSFC",strCriteria);
}
long CHSEMSCommonCategory::ShowHSEPolicy(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HSEPLC;
	/////////////////////////////////////////////////////////////////////////////////
	//RQ-11-2021.17
	m_bEditMode=true;
	int retVal=ParentManage("HSE_TGHSEPLC",TRUE,TRUE,TRUE,"HSE_HSEPLC",strCriteria);
	m_bEditMode=false;
	/////////////////////////////////////////////////////////////////////////////////
	return retVal;
}

long CHSEMSCommonCategory::ShowCompanyProfile(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_COMPPRF;
	/////////////////////////////////////////////////////////////////////////////////
	//RQ-11-2021.21
	m_bEditMode=true;
	int retVal=ParentManage("HSE_TGCOMPPRF",TRUE,TRUE,TRUE,"HSE_COMPPRF",strCriteria);
	m_bEditMode=false;
	/////////////////////////////////////////////////////////////////////////////////
	return retVal;
}

long CHSEMSCommonCategory::ShowBodyParts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HUMNBDYPRT;
	return ParentManage("HSE_TGHUMNBDYPRT",TRUE,TRUE,TRUE,"HSE_HUMNBDYPRT",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEProceduresListInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HSEPROCDRSLSTINQ;
	return ParentManage("HSE_TGHSEPRCDRLISTINQ",FALSE,FALSE,FALSE,"HSE_HSEPrcdrList",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEToolsEquipments(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TOOLEQUIP;
	return ParentManage("HSE_TGTOOLEQUIP",TRUE,TRUE,TRUE,"HSE_TOOLEQUIP",strCriteria);
}

long CHSEMSCommonCategory::ShowSubDetails(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SUBDETAILS;
	return ParentManage("HSE_TGSUBDTL",TRUE,TRUE,TRUE,"HSE_SUBDTL",strCriteria);
}

long CHSEMSCommonCategory::ShowDtlForChkLst(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_DTLFORCHKLIST;
	return ParentManage("HSE_TGANSWRDTL",TRUE,TRUE,TRUE,"HSE_DTLFORCHKLIST",strCriteria);
}


long CHSEMSCommonCategory::ShowPrecautions(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PRECAUTIONS;
	return ParentManage("HSE_TGPrctns",TRUE,TRUE,TRUE,"HSE_Prctns",strCriteria);
}

long CHSEMSCommonCategory::ShowContactMethods(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CNTCT_MTHDS;
	return ParentManage("CMN_CntctMthds",TRUE,TRUE,TRUE,"CMN_CntctMthds",strCriteria);
}

long CHSEMSCommonCategory::ShowAtchmntFrmts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ATCHMNTFRMTS;
	return ParentManage("CMN_AtchmntFrmt",TRUE,TRUE,TRUE,"CMN_AtchmntFrmt",strCriteria);
}


long CHSEMSCommonCategory::ShowTrades(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TRADES;
	return ParentManage("CMN_TRADES",TRUE,TRUE,TRUE,"CMN_TRADES",strCriteria);
}

long CHSEMSCommonCategory::ShowFirstInfo(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_FIRSTINFO;
	return ParentManage("HSE_TGFirstInfo",TRUE,TRUE,TRUE,"HSE_FirstInfo",strCriteria);
}

long CHSEMSCommonCategory::ShowDocType(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_DOCTYP;
	return ParentManage("CMN_DOCTYP",TRUE,TRUE,TRUE,"CMN_DOCTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowAdtActvtRqrdActn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ADTACTVTRQRDACTN;
	return ParentManage("HSE_TGADTACTVTRQRDACTN",TRUE,TRUE,TRUE,"HSE_ADTACTVTRQRDACTN",strCriteria);
}

long CHSEMSCommonCategory::ShowRskSvrtyCntrlMsr(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RSKSVRTYCNTRLMSR;
	return ParentManage("HSE_TgRskSvrtyCntrlMsr",TRUE,TRUE,TRUE,"HSE_RSKSVRTYCNTRLMSR",strCriteria);
}

long CHSEMSCommonCategory::ShowAdtChkLstPopUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtChkLstPopUp;
	return ParentManage("HSE_TgAdtChkLstPopUp",TRUE,TRUE,TRUE,"HSE_AdtChkLst",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEPrjcts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_Prjcts;
	return ParentManage("HSE_TgPrjcts",TRUE,TRUE,TRUE,"HSE_Prjcts",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEAdtTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtTyp;
	return ParentManage("HSE_TgAdtTyp",TRUE,TRUE,TRUE,"HSE_AdtTyp",strCriteria);
}

long CHSEMSCommonCategory::ShowHSERskRat(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtTyp;
	return ParentManage("HSE_TgRskRat",TRUE,TRUE,TRUE,"HSE_RskRat",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEJobStp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_JobStp;
	return ParentManage("HSE_TgJobStp",TRUE,TRUE,TRUE,"HSE_JobStp",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEIncdntPrd(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntPrd;
	return ParentManage("HSE_TgIncdntPrd",TRUE,TRUE,TRUE,"HSE_IncdntPrd",strCriteria);
}

long CHSEMSCommonCategory::ShowHSECnsqncs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntPrd;
	return ParentManage("HSE_TgCnsqncs",TRUE,TRUE,TRUE,"HSE_Cnsqncs",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEAdtGrp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtGrp;
	return ParentManage("HSE_TgAdtGrp",TRUE,TRUE,TRUE,"HSE_AdtGrp",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEAdtChcklst(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtChcklst;
	return ParentManage("HSE_TgAdtChcklst",TRUE,TRUE,TRUE,"HSE_AdtChcklst",strCriteria);
}

long CHSEMSCommonCategory::ShowAdtRschdl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtRschdl;
	return ParentManage("HSE_TgAdtRschdl",TRUE,TRUE,TRUE,"HSE_AdtRschdl",strCriteria);
}

long CHSEMSCommonCategory::ShowAdtRschdlBrws(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtRschdlBrws;
	return ParentManage("HSE_TgAdtRschdlBrws",TRUE,TRUE,TRUE,"HSE_AdtRschdlBrws",strCriteria);
}

long CHSEMSCommonCategory::ShowAdtCncl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AdtCncl;
	return ParentManage("HSE_TgAdtCncl",TRUE,TRUE,TRUE,"HSE_AdtCncl",strCriteria);
}


long CHSEMSCommonCategory::ShowHSERltdOprtns(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RltdOprtns;
	return ParentManage("HSE_TgRltdOprtns",TRUE,TRUE,TRUE,"HSE_RltdOprtns",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEMtrlRlsTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MtrlRlsTyp;
	return ParentManage("HSE_TgMtrlRlsTyp",TRUE,TRUE,TRUE,"HSE_MtrlRlsTyp",strCriteria);
}

long CHSEMSCommonCategory::ShowHSECntnmntTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CntnmntTyp;
	return ParentManage("HSE_TgCntnmntTyp",TRUE,TRUE,TRUE,"HSE_CntnmntTyp",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEMtrlRlsCat(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MtrlRlsCat;
	return ParentManage("HSE_TgMtrlRlsCat",TRUE,TRUE,TRUE,"HSE_MtrlRlsCat",strCriteria);
}

long CHSEMSCommonCategory::ShowHSERlsMtrl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RlsMtrl;
	return ParentManage("HSE_TgRlsMtrl",TRUE,TRUE,TRUE,"HSE_RlsMtrl",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEUOM(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_UOM;
	return ParentManage("HSE_TgUOM",TRUE,TRUE,TRUE,"HSE_UOM",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEMtrlRlsTo(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MtrlRlsTo;
	return ParentManage("HSE_TgMtrlRlsTo",TRUE,TRUE,TRUE,"HSE_MtrlRlsTo",strCriteria);
}

long CHSEMSCommonCategory::ShowHSESplClsfctn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_SplClsfctn;
	return ParentManage("HSE_TgSplClsfctn",TRUE,TRUE,TRUE,"HSE_SplClsfctn",strCriteria);
}


long CHSEMSCommonCategory::ShowHSEAstTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AstTyp;
	return ParentManage("HSE_TgAstTyp",TRUE,TRUE,TRUE,"HSE_AstTyp",strCriteria);
}

long CHSEMSCommonCategory::ShowInjryAnlysisPopup(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_INJRYANLYSIS;
	return ParentManage("HSE_TgInjryAnlysis",TRUE,TRUE,TRUE,"HSE_InjryAnlysis",strCriteria);
}

long CHSEMSCommonCategory::ShowSftyInstrctn(CString strCriteria)
{
	m_nCurrentCard=ID_SAFETY_INSTRUCTIONS;
	return ParentManage("HSE_TgSftyInstrctn",TRUE,TRUE,TRUE,"HSE_SftyInstrctn",strCriteria);
}

long CHSEMSCommonCategory::ShowHRCTRLBrws(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HRCTRLBDTLRWS;
	return ParentManage("HSE_TgHRCTRLDtlBrws",TRUE,TRUE,TRUE,"HSE_HRCTRLDTL",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEJobStpDfntn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_JOB_STP_DFNTN;
	return ParentManage("HSE_TgJobStpDfntn",TRUE,TRUE,TRUE,"HSE_JobStpDfntn",strCriteria);
}

long CHSEMSCommonCategory::ShowHSECnsqncsDfntn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CNSQ_DFNTN;
	return ParentManage("HSE_TGCnsqncsDfntn",TRUE,TRUE,TRUE,"HSE_CnsqncsDfntn",strCriteria);
}

long CHSEMSCommonCategory::ShowVclTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_VCL_TYPE;
	return ParentManage("HSE_TGVCLTYP",TRUE,TRUE,TRUE,"HSE_VCLTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowHSECntrctr(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CNTRCTR;
	return ParentManage("HSE_TGCNTRCTR",TRUE,TRUE,TRUE,"HSE_CNTRCTR",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEMnDrlTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MnDrlTyp;
	return ParentManage("HSE_TGMNDRLTYP",TRUE,TRUE,TRUE,"HSE_MNDRLTYP",strCriteria);
}

long CHSEMSCommonCategory::ShowHSEIncdntMdclRprt(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntMdclRprt;
	return ParentManage("HSE_TGINCDNTMDCLRPRT",TRUE,TRUE,TRUE,"HSE_INCDNTMDCLRPRT",strCriteria);
}

long CHSEMSCommonCategory::ShowInvlvdPrts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_INVLVDPRTS;
	return ParentManage("HSE_TGINVLVDPRTS",TRUE,TRUE,TRUE,"HSE_INVLVDPRTS",strCriteria);
}

long CHSEMSCommonCategory::ShowPckngMthds(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PckngMthds;
	return ParentManage("HSE_TGPCKNGMTHDS",TRUE,TRUE,TRUE,"HSE_PCKNGMTHDS",strCriteria);
}

//waste zones
long CHSEMSCommonCategory::ShowWstZns(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_WstZns;
	return ParentManage("HSE_TGWSTZNS",TRUE,TRUE,TRUE,"HSE_WSTZNS",strCriteria);
}

//Waste Items
long CHSEMSCommonCategory::ShowWstItms(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_WstItms;
	return ParentManage("HSE_TGWSTITMS",TRUE,TRUE,TRUE,"HSE_WSTITMS",strCriteria);
}

//Waste Transporters
long CHSEMSCommonCategory::ShowWstTrnsprtrs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_WstTrnsprtrs;
	return ParentManage("HSE_TGWSTTRNSPRTRS",TRUE,TRUE,TRUE,"HSE_WSTTRNSPRTRS",strCriteria);
}

//Waste Treatment Plants
long CHSEMSCommonCategory::ShowWstTrtmntPlnts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_WstTrtmntPlnts;
	return ParentManage("HSE_TGWSTTRTMNTPLNTS",TRUE,TRUE,TRUE,"HSE_WSTTRTMNTPLNTS",strCriteria);
}

//Environmental Monitoring Activities
long CHSEMSCommonCategory::ShowEnvMntrActvts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EnvMntrActvts;
	return ParentManage("HSE_TGENVMNTRACTVTS",TRUE,TRUE,TRUE,"HSE_ENVMNTRACTVTS",strCriteria);
}

//Environmental Measure Types	
long CHSEMSCommonCategory::ShowEnvMsrTyps(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EnvMsrTyps;
	return ParentManage("HSE_TGENVMSRTYPS",TRUE,TRUE,TRUE,"HSE_ENVMSRTYPS",strCriteria);
}

//Observation Targets
long CHSEMSCommonCategory::ShowObsrvtnTrgts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ObsrvtnTrgts;
	return ParentManage("HSE_TGOBSRVTNTRGTS",TRUE,TRUE,TRUE,"HSE_OBSRVTNTRGTS",strCriteria);
}

long CHSEMSCommonCategory::ShowEnvMsrEntryPoltantPrpCAR(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EnvMsrEntryPoltantPrpCAR;
	return ParentManage("HSE_TGENVMSRENTRYPOLTANTPRPCAR",TRUE,TRUE,TRUE,"HSE_ENVMSRENTRYPOLTANTPRPCAR",strCriteria);
}

//EQ Types
long CHSEMSCommonCategory::ShowEQTyps(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EQTyps;
	return ParentManage("HSE_EQTYPS",TRUE,TRUE,TRUE,"HSE_EQTYPS",strCriteria);
}

//ERT List
long CHSEMSCommonCategory::ShowHSEERTList(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_ERTList;
	return ParentManage("HSE_TGERTLST",TRUE,TRUE,TRUE,"HSE_ERTLST",strCriteria);
}

//Risk Level
long CHSEMSCommonCategory::ShowHSERskLvl(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RskLvl;
	return ParentManage("HSE_TGRSKLVL",TRUE,TRUE,TRUE,"HSE_RSKLVL",strCriteria);
}

//Top Events
long CHSEMSCommonCategory::ShowHSETopEvnts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TopEvnts;
	return ParentManage("HSE_TGTOPEVNTS",TRUE,TRUE,TRUE,"HSE_TOPEVNTS",strCriteria);
}

//Barriers
long CHSEMSCommonCategory::ShowHSETopEvntsThrtsBrs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TopEvntsThrtsBrs;
	return ParentManage("HSE_TGTOPEVNTSTHRTSBRS",TRUE,TRUE,TRUE,"HSE_TOPEVNTSTHRTSBRS",strCriteria);
}


//Life Saving Rules
long CHSEMSCommonCategory::ShowLfSavngRls(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_LfSavngRls;
	return ParentManage("HSE_TGLFSAVNGRLS",TRUE,TRUE,TRUE,"HSE_LFSAVNGRLS",strCriteria);
}

//Awareness Courses
long CHSEMSCommonCategory::ShowAwrnsCrs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AwrnsCrs;
	return ParentManage("HSE_TGAWRNSCRS",TRUE,TRUE,TRUE,"HSE_AwrnsCrs",strCriteria);
}

//Course Attendees
long CHSEMSCommonCategory::ShowAwrnsPlnCrsAtnds(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_AWRNSPLN_CRS_Atnds;
	return ParentManage("HSE_TGAWRNSPLN_CRS_ATNDS",TRUE,TRUE,TRUE,"HSE_AWRNSPLN_CRS_Atnds",strCriteria);
}

//Reason
long CHSEMSCommonCategory::ShowCMSRsns(CString strCriteria)
{
	m_nCurrentCard=ID_CMS_Rsns;
	return ParentManage("CMS_RSNS",TRUE,TRUE,TRUE,"CMS_RSNS",strCriteria);
}

//Action Reason
long CHSEMSCommonCategory::ShowCMSActnRsn(CString strCriteria)
{
	m_nCurrentCard=ID_CMS_ActnRsn;
	return ParentManage("CMS_ACTNRSN",TRUE,TRUE,TRUE,"CMS_ACTNRSN",strCriteria);
}

//Rescue Entities
long CHSEMSCommonCategory::ShowRscuEntts(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RscuEntts;
	return ParentManage("HSE_TGRSCUENTTS",TRUE,TRUE,TRUE,"HSE_RSCUENTTS",strCriteria);
}

//Confined Space Types
// Update "HSEMS New requirements_27_10_2020" : Screen name changed to : Rescue Types
long CHSEMSCommonCategory::ShowCnfndSpcTyps(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_CnfndSpcTyps;
	return ParentManage("HSE_TGCNFNDSPCTYPS",TRUE,TRUE,TRUE,"HSE_CNFNDSPCTYPS",strCriteria);
}

//Rescue Hazards
long CHSEMSCommonCategory::ShowRscuHzrds(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RscuHzrds;
	return ParentManage("HSE_TGRSCUHZRDS",TRUE,TRUE,TRUE,"HSE_RscuHzrds",strCriteria);
}

//Rescue Tools
long CHSEMSCommonCategory::ShowRscuTls(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RscuTls;
	return ParentManage("HSE_TGRSCUTLS",TRUE,TRUE,TRUE,"HSE_RSCUTLS",strCriteria);
}

//Rescue Communication Methods
long CHSEMSCommonCategory::ShowRscuCmmunctnMthds(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RscuCmmunctnMthds;
	return ParentManage("HSE_TGRSCUCMMUNCTNMTHDS",TRUE,TRUE,TRUE,"HSE_RSCUCMMUNCTNMTHDS",strCriteria);
}

//Hazard Process
long CHSEMSCommonCategory::ShowHzrdPrcs(CString strCriteria)
{
	m_nCurrentCard=ID_Hzrd_Procs;
	return ParentManage("HSE_TGHZRDPRCS",TRUE,TRUE,TRUE,"HSE_HZRDPRCS",strCriteria);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//HSE BGC-EZZ Requirements_2892020

//Risk Type
long CHSEMSCommonCategory::ShowRskTyp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RskTyp;
	return ParentManage("HSE_TGRSKTYP",TRUE,TRUE,TRUE,"HSE_RSKTYP",strCriteria);
}

//Medical Authority
long CHSEMSCommonCategory::ShowMdclAuthrty(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MdclAuthrty;
	return ParentManage("HSE_TGMDCLAUTHRTY",TRUE,TRUE,TRUE,"HSE_MDCLAUTHRTY",strCriteria);
}

//Medical Decision
long CHSEMSCommonCategory::ShowMdclDcsn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_MdclDcsn;
	return ParentManage("HSE_TGMDCLDCSN",TRUE,TRUE,TRUE,"HSE_MDCLDCSN",strCriteria);
}

//Auditors
long CHSEMSCommonCategory::ShowAudtrs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_Audtrs;
	return ParentManage("HSE_TGAUDTRS",TRUE,TRUE,TRUE,"HSE_AUDTRS",strCriteria);
}

//Environmental Measures
long CHSEMSCommonCategory::ShowEnvrnmntlMsrs(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EnvrnmntlMsrs;
	return ParentManage("HSE_TGENVRNMNTLMSRS",TRUE,TRUE,TRUE,"HSE_ENVRNMNTLMSRS",strCriteria);
}

//Incident Medical Report Follow-Up
long CHSEMSCommonCategory::ShowIncdntMdclRprtFlwUp(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_IncdntMdclRprtFlwUp;
	return ParentManage("HSE_TGINCDNTMDCLRPRTFLWUP",TRUE,TRUE,TRUE,"HSE_INCDNTMDCLRPRT",strCriteria);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Pollutants
// Updated to Measure
long CHSEMSCommonCategory::ShowEnvMntrActvtsPltnt(CString strCriteria,CString strComeFrom)
{
	m_nCurrentCard=ID_HSE_EnvMntrActvtsPltnt;
	strComeFrom = getOwnerScreenTag();
	BOOL bAllowUpdate = TRUE;
	BOOL bAllowAddNew = FALSE;
	BOOL bAllowDelete = TRUE;
	if(strComeFrom == "HSE_TGENVMNTRPLAN") {
		bAllowUpdate = FALSE;
		bAllowAddNew = FALSE;
		bAllowDelete = FALSE;
		EnableToolbar("HSE_TGENVMNTRACTVTSPLTNT",TOOLBAR_SHOW_DETAILS,FALSE);		
	}	
	return ParentManage("HSE_TGENVMNTRACTVTSPLTNT",bAllowUpdate,bAllowAddNew,bAllowDelete,"HSE_ENVMNTRACTVTSPLTNT",strCriteria);
}

//Measure Required Actions Pop-up screen
long CHSEMSCommonCategory::ShowHSEEnvmntrRqrdActn(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_EnvmntrRqrdActn;
	return ParentManage("HSE_TGENVMNTRRQRDACTN",TRUE,TRUE,TRUE,"HSE_ENVMNTRRQRDACTN",strCriteria);
}

long CHSEMSCommonCategory::ShowPopupEntry(CString strFormTag, CString strCriteria)
{
	m_nCurrentCard=0;
	CString strTableName;
	if (strFormTag == "HSE_TGANSWRDTL")
		strTableName = "HSE_DTLFORCHKLIST";
	else if("HSE_TGCNCRNDPT") 
		strTableName = "HSE_CNCRNDPT";
	return ParentManage(strFormTag,TRUE,TRUE,TRUE,strTableName,strCriteria);
}

UINT CHSEMSCommonCategory::ShowMenuScreenV2(CString strScreenTableName,CString strCriteria)
{
	return ParentManage(m_FSATableName,m_bAllowUpdate,m_bAllowAddNew,m_bAllowDelete,strScreenTableName,strCriteria);
}

//TODO
void* p=NULL;
extern CString gstrCARDate=""; //RQ_SF_23_5_23_1.9.2
long CHSEMSCommonCategory::ShowMenuScreenV2(CString strTableName,int nScreenId,CString strCriteria,void* msg)
{	
	CString strInitialTableName=strTableName;
	int ninitialScreenId=nScreenId;
	m_pCategory->GetHSEObj()->SetCurrentMenuItemID(nScreenId);
	m_pCategory->SetMenuID(nScreenId);
	m_nScreenId=nScreenId;
	CString strScreenTag="";
	CString strScreenTableName="";
	CString strTablePart;
	CString strFSATableName="";//"HSE_AUDTACTNSCNFRMTN";
	CString strFormTable="";//"HSE_AUDTACTNSCNFRMTN";
	CString strSerialFieldName="";//"AUDTACTNSCNFRMTN_PLNN";
	CString strEmpDep;
	CString strOwnerDept; 

	CMapStringToOb* pmpListOfParameters=NULL;
	CAuditModuleCategory* pAuditModuleCategory=NULL;
	CNewCarEntryCategory* pNewCarEntryCategory=NULL;
	CCorrectiveActionEntry* pCorrectiveActionEntry=NULL;

	switch (m_nScreenId)
	{	
	case HSE_ASPECT_TYPE:
		strScreenTag="HSE_AspctTyp";
		break;
	case HSE_ASPECT_ACTIVITY:
		strScreenTag="HSE_AspctActvty";
		break;
	case HSE_ASPECT_FREQUENCY:
		strScreenTag="HSE_AspctFrquncy";
		break;
	case HSE_ASPECT_UOM:
		strScreenTag="HSE_AspctUOM";
		break;
	case HSE_ASPECT_STATE:
		strScreenTag="HSE_AspctStt";
		break;
	case HSE_CHEMICAL_ITEM_FREQUENCY:
		strScreenTag="HSE_ChmclItmFrquncy";
		break;
	case HSE_CHEMICAL_HAZARD_HEALTH:
		strScreenTag="HSE_ChmclHzrdHlth";
		break;
	case HSE_CHEMICAL_HAZARD_FLAMMABILITY:
		strScreenTag="HSE_ChmclHzrdFlmmblty";
		break;
	case HSE_CHEMICAL_HAZARD_RE_ACTIVITY:
		strScreenTag="HSE_ChmclHzrdR_Actvty";
		break;
	case HSE_CHEMICAL_HAZARD_SPECIAL:
		strScreenTag="HSE_ChmclHzrdSpcl";
		break;
	case HSE_DRILL_TYPE:
		strScreenTag="HSE_DRLLTYP_VW";
		break;
	case HSE_EXTERNAL_SUPPORT:
		strScreenTag="HSE_EXTRNLSUPPRT";
		break;
	case HSE_EMT_LIST:
		strScreenTag="HSE_EMTLST";
		break;
	case HSE_DRILL_ROLE:
		strScreenTag="HSE_DRLLRL";
		break;
	case HSE_INTERNAL_RESOURCE:
		strScreenTag="HSE_INTRNLRSURC";
		break;
	case HSE_ASPECTS_ENTRY_ENTRY:
		{			
			EnvrnmntAspctCategory* pEnvrnmntAspctCategory=new EnvrnmntAspctCategory(m_pCategory);
			pEnvrnmntAspctCategory->ShowMenuScreenV2("",strCriteria);
		}		
		break;
	case HSE_ASPECTS_REVIEW_REVIEW:
		strScreenTag="HSE_ASPCTSRVWATRVW";
		break;
	case HSE_CHEMICAL_HAZARD:
		strScreenTag="HSE_CHMCLHZRD";
		break;
	case HSE_CHEMICAL_ITEMS_DEFINITION:
		strScreenTag="HSE_CHMCLITMSDFNTN";
		break;
	
	case HSE_CHEMICAL_REGISTER:
		{
			ChemicalRegisterCategory* pChemicalRegisterCategory=new ChemicalRegisterCategory(m_pCategory);
			pChemicalRegisterCategory->ShowMenuScreenV2("",strCriteria);		
		}
		break;
	
	case HSE_DRILL_PLAN_ENTRY_ENTRY:
		{			
			DrillEntryCategory* pDrillEntryCategory=new DrillEntryCategory(m_pCategory);
			pDrillEntryCategory->ShowMenuScreenV2("",strCriteria);		
		}		
		break;
	case HSE_DRILL_PLAN_APPROVAL_APPROVAL:
		strScreenTag="HSE_DrllPlnApprvlAtApprvl";
		break;
	case HSE_DRILL_EXECUTION_EXECUTION:
		strScreenTag="HSE_DrllExcutnAtExcutn";
		break;	
	case ID_HSE_WRKLOC:
		strScreenTag="HSE_TGWRKLOC";
		break;
	case ID_HSE_POP_UP_CRCTVACTN:
		strScreenTag="HSE_TGCRRCTVACTNS";
		break;
	case ID_CMS_Rsns:
		{
			if(msg)
				p=msg;
			else
				msg=(IReasonCaller *)p;
			CReasonsCategory* pReasonsCategory;
			pReasonsCategory=new CReasonsCategory(m_pCategory,(IReasonCaller *)msg);
			pReasonsCategory->ShowScreen("CMS_Rsns",strCriteria);
		}
		break;
	case ID_CMS_ActnRsn:
		strScreenTag="CMS_ActnRsn";
		break;

	//RQ-2-2021(Batch2).15
	/*(New) Audit Standards	•        Simple (code, description, active) screen, used to identify the (ISO Standards) to be used with Auditors and Audit plans for better classifications	"o	01 = ISO 9001
	o	02 = ISO 14005
	o	03 = ISO 14001
	o	04 = QMS
	o	05 = EMS-OH&S
	o	Etc.
	"
	*/
	case ID_SETUP_AUDITS_QUALITYSTANDARDS:
		strTablePart="QltyStndrd";//"TestTable";
		strScreenTag="HSE_Tg"+strTablePart;
		strScreenTableName="HSE_"+strTablePart;


		break;

	case ID_INQUIRYSCREENS_INCIDENTMEDICALINQUIRY:
		return ParentManage("HSE_TGINCDNTMDCLRPRTINQ",FALSE,FALSE,FALSE,"HSE_INCDNTMDCLRPRT",strCriteria);
		break;

	case ID_SETUP_AWARENESS_ATTENDEEGRADES:
		strScreenTag="HSE_TGATNDGRD";
		strScreenTableName="HSE_ATNDGRD";
		break;

	//RQ-9-2021.5
	//HSM Sep 29 2021  6:35PM
	case ID_SETUP_CAR_ACTIONCLASSIFICATION:
		strScreenTag="HSE_tgActnClssfctn";
		strScreenTableName="HSE_ActnClssfctn";
		break;
		////////////////////////////////////////////////////////////////////////


	//HSE Audit Module 25/11/2021
	case ID_AUDITSTANDARDS:
		strScreenTag="HSE_AUDTSTNDRDS";
		strScreenTableName="HSE_AUDTSTNDRDS";
		break;

	case ID_AUDITCLAUSES:
		strScreenTag="HSE_AUDTCLUSS";
		strScreenTableName="HSE_AUDTCLUSS";
		break;

	case ID_AUDITEE:
		strScreenTag="HSE_AUDT";
		strScreenTableName="HSE_AUDT";
		break;

	case ID_CERTIFICATETYPE:
		strScreenTag="HSE_CrtfctTyp";
		strScreenTableName="HSE_CrtfctTyp";
		break;

	case ID_AUDITROLE:
		strScreenTag="HSE_AUDTRL";
		strScreenTableName="HSE_AUDTRL";
		break;

	case ID_CERTIFICATEPROVIDER:
		strScreenTag="HSE_CrtfctPrvdr";
		strScreenTableName="HSE_CrtfctPrvdr";
		break;

	case ID_UNPLANNEDAUDITJUSTIFICATION:
		strScreenTag="HSE_Un_PlnndAudtJustfctn";
		strScreenTableName="HSE_Un_PlnndAudtJustfctn";
		break;

	case ID_SCHEDULECHANGEREASON:
		strScreenTag="HSE_SchdulChngRsn";
		strScreenTableName="HSE_SchdulChngRsn";
		break;

	case ID_DEFAULTDISTRIBUTION:
		strScreenTag="HSE_DfultDstrbutnLst";
		strScreenTableName="HSE_DfultDstrbutnLst";
		break;

	case ID_CERTIFIEDAUDITORS:
		//strScreenTag="HSE_CRTFDAUDTRS";
		//strScreenTableName="HSE_CRTFDAUDTRS";
		strScreenTag="HSE_VW_CRTFDAUDTRS";
		strScreenTableName="HSE_VW_CRTFDAUDTRS";
		break;

	case ID_AUDITPLANENTRY:
		strScreenTag="HSE_AUDTPLNENTRY";
		strScreenTableName="HSE_AUDTPLNENTRY";
		strCriteria="WHERE (AUDTPLNENTRY_PLNSTTUS = 'O')";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_AUDITPLANAPPROVAL:
		strScreenTag="HSE_AUDTPLNAPPRVL";
		strScreenTableName="HSE_AUDTPLNENTRY";
		strCriteria="WHERE (AUDTPLNENTRY_PLNSTTUS = 'C')";
		strTableName=strScreenTag;//strScreenTableName;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_AUDITNOTIFICATION:
		strScreenTag="HSE_AUDTNTFCTN";
		strScreenTableName="HSE_AUDTPLNENTRY";
		//////////////////////////////////////////////////////////////////////////////////
		//RQ-1-2022.20
		strCriteria="WHERE (AUDTPLNENTRY_PLNSTTUS = 'F') and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (AUDTPLNENTRY_AUDTS_AUDTSTTUS='1')) ";
		//////////////////////////////////////////////////////////////////////////////////
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_AUDITEXECUTIONRESULT:
		strScreenTag="HSE_AUDTEXCUTNRSULT";
		strScreenTableName="HSE_AUDTPLNENTRY";
		/////////////////////////////////////////////////////////////////////////
		//RQ-12-2021.34
		strCriteria="where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where AudtPlnEntry_Audts_AudtSttus='5'))";
		/////////////////////////////////////////////////////////////////////////
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_AUDITACTIONSENTRY:
		strScreenTag="HSE_AUDTACTNSENTRY";
		strScreenTableName="HSE_AUDTPLNENTRY";
		/////////////////////////////////////////////////////////////////////////
		//RQ-12-2021.61,62
		//Bugs-1-2022.18 
		strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);//Bugs-4-2022.23
		if(strEmpDep=="")
		{
			AfxMessageBox("User department is not known, Can not open screen");
			return -1;
		}
		//strCriteria="where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')!='Y' and AudtPlnEntry_Audts_AudtUnt in(select PrmKy from HSE_Audt_AudtUnt where Audt_AudtUnt_Dprtmnt='07'))))";
		//RQ-4-2022.43 added 'and AudtPlnEntry_Audts_AudtUnt in(select PrmKy from HSE_Audt_AudtUnt where Audt_AudtUnt_Dprtmnt='%s')'
		strOwnerDept = getOwnerDept("");
		if(strOwnerDept!=strEmpDep) //RQ-4-2022.44
			strCriteria.Format("where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')!='Y' and AudtPlnEntry_Audts_AudtUnt in(select PrmKy from HSE_Audt_AudtUnt where Audt_AudtUnt_Dprtmnt='%s'))))",strEmpDep);
		else
			strCriteria="where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')!='Y')))";
		/////////////////////////////////////////////////////////////////////////
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_AUDITACTIONSCONFIRMATION:
		
		/*strScreenTag="HSE_AUDTACTNSCNFRMTN";
		strScreenTableName="HSE_AUDTACTNSCNFRMTN";*/
		
		strScreenTag="HSE_AUDTACTNCNFRM";
		strScreenTableName="HSE_AUDTACTNCNFRM";
		////////////////////////////////////////////////////////////////////////////////////////
		//RQ-4-2022.42 	add the part and AUDTPLNENTRY_AUDTS_CR_GNR='N' 
		//strCriteria="where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (								   AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')='Y' AND isnull(AUDTPLNENTRY_AUDTS_CRSGNR,'N')='N')) and MasterKey in(select MainLink from HSE_AUDTPLNENTRY_AUDTS where AUDTPLNENTRY_AUDTS_CR_GNR='N'))";		
		strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),false);
		strCriteria="where (AUDTPLNENTRY_PLNSTTUS = 'F' and MasterKey in (select mainlink from HSE_AudtPlnEntry_Audts where (AUDTPLNENTRY_AUDTS_CR_GNR='N' and AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')='Y' AND isnull(AUDTPLNENTRY_AUDTS_CRSGNR,'N')='N')))";		

		////////////////////////////////////////////////////////////////////////////////////////
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	//POP UPS
	case ID_POPUPS_LNKDAUDTCLUSSPPUP:
		strScreenTag="HSE_LNKDAUDTCLUSSPPUP";
		strScreenTableName="HSE_LNKDAUDTCLUSSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_LNKDAUDTLCTNPPUP:
		strScreenTag="HSE_LNKDAUDTLCTNPPUP";
		strScreenTableName="HSE_LNKDAUDTLCTNPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

		
	case ID_POPUPS_AUDITCHECKLISTPOPUP://0
		strScreenTag="HSE_AUDTCHCKLST";
		strScreenTableName="HSE_AUDTCHCKLST";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;


	case ID_POPUPS_ACTUALAUDITEEPERSONSPOPUP://1
		strScreenTag="HSE_ACTULAUDTPRSNSPPUP";
		strScreenTableName="HSE_ACTULAUDTPRSNSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_AUDITCLAUSESPOPUP://2
		/////////////////////////////////////////////////////
		//Bugs-12-2021.64
		strScreenTag="HSE_AUDTCLUSSPPUP";
		strScreenTableName="HSE_AUDTCLUSSPPUP";
		/////////////////////////////////////////////////////
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		/*		
		//////////////////////////////////////////////////////////////////////////
		pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTCLUSSPPUP","HSE_IDNTFYAUDTCLUSSPPUP","IDNTFYAUDTCLUSSPPUP_SRL","PrmKy","MainLink");
		pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
		pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
		return 0;
		//////////////////////////////////////////////////////////////////////////
		*/
		break;

	case ID_POPUPS_AUDITCONCLUSIONPOPUP://3
		strScreenTag="HSE_AUDTCNCLUSNPPUP";
		strScreenTableName="HSE_AUDTCNCLUSNPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_AUDITSTRENGTHPOINTSPOPUP://4
		strScreenTag="HSE_AUDTSTRNGTHPNTSPPUP";
		strScreenTableName="HSE_AUDTSTRNGTHPNTSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_AUDITWEAKPOINTSPOPUP://5
		strScreenTag="HSE_AUDTWKPNTSPPUP";
		strScreenTableName="HSE_AUDTWKPNTSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_CLAUSELEVELRESULTPOPUP://6
		strScreenTag="HSE_CLUS_LVLRSULTPPUP";
		strScreenTableName="HSE_CLUS_LVLRSULTPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDENTIFYAUDITCLAUSESPOPUP://7
		strScreenTag="HSE_IDNTFYAUDTCLUSSPPUP";
		strScreenTableName="HSE_IDNTFYAUDTCLUSSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDENTIFYAUDITEELOCATIONSPOPUP://8
		strScreenTag="HSE_IDNTFYAUDTLCTNSPPUP";
		strScreenTableName="HSE_IDNTFYAUDTLCTNSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDENTIFYAUDITORSTEAMPOPUP://9
		strScreenTag="HSE_IDNTFYAUDTRSTMPPUP";
		strScreenTableName="HSE_IDNTFYAUDTRSTMPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDENTIFYNEEDEDTOOLSPOPUP://10
		strScreenTag="HSE_IDNTFYNDDTLSPPUP";
		strScreenTableName="HSE_IDNTFYNDDTLSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDENTIFYSHIFTGROUPSPOPUP://11
		strScreenTag="HSE_IDNTFYSHFTGRUPSPPUP";
		strScreenTableName="HSE_IDNTFYSHFTGRUPSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IMPROVEMENTOPORTUNITYPOPUP://12
		strScreenTag="HSE_IMPRVMNTOPRTUNTYPPUP";
		strScreenTableName="HSE_IMPRVMNTOPRTUNTYPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_MOREPERSONSTONOTIFYPOPUP://13
		strScreenTag="HSE_MRPRSNSTNTFYPPUP";
		strScreenTableName="HSE_MRPRSNSTNTFYPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_RECOMMENDEDACTIONSPOPUP://14
		strScreenTag="HSE_RCMMNDDACTNSPPUP";
		strScreenTableName="HSE_RCMMNDDACTNSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_VIEWRECOMMENDEDACTIONSPOPUP://15
		strScreenTag="HSE_VWRCMMNDDACTNSPPUP";
		strScreenTableName="HSE_VWRCMMNDDACTNSPPUP";
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_AUDITCHECKLISTPOPUP_LNK://#POPUPSCREEN1 #1
		strScreenTag="HSE_AUDTCHCKLST_LNK";
		strScreenTableName="HSE_AUDTCHCKLST";
		m_bEnableMainScreen=FALSE;
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_AUDITCLAUSESPOPUP_LNK: //#2
		strScreenTag="HSE_IDNTFYAUDTCLUSSPPUP_LNK";
		strScreenTableName="HSE_IDNTFYAUDTCLUSSPPUP_LNK";
		m_bEnableMainScreen=FALSE;
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDNTFYAUDTLCTNSPPUP_LNK://#3
		strScreenTag="HSE_IDNTFYAUDTLCTNSPPUP_LNK";
		strScreenTableName="HSE_IDNTFYAUDTLCTNSPPUP_LNK";
		m_bEnableMainScreen=FALSE;
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDNTFYNDDTLSPPUP_LNK: //#4
		strScreenTag="HSE_IDNTFYNDDTLSPPUP_LNK";
		strScreenTableName="HSE_IDNTFYNDDTLSPPUP_LNK";
		m_bEnableMainScreen=FALSE;
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	case ID_POPUPS_IDNTFYAUDTRSTMPPUP_LNK://#5
		strScreenTag="HSE_IDNTFYAUDTRSTMPPUP_LNK";
		strScreenTableName="HSE_IDNTFYAUDTRSTMPPUP_LNK";
		m_bEnableMainScreen=FALSE;
		strTableName=strScreenTag;
		m_nScreenId=-1;
		strScreenTag="ANYTHING";
		break;

	/////////////////////////////////////////
	//RQ-2-2022.11
	case ID_SETUP_GENERAL_DEPARTMENTGROUP:
		strScreenTag="HSE_TgDprtmntGrp";
		strScreenTableName="HSE_DprtmntGrp";
		break;

	/////////////////////////////////////////
	//RQ-2-2022.13
	case ID_SETUP_AUDITS_CLAUSEGROUP:
		strScreenTag="HSE_TgClausGrp";
		strScreenTableName="HSE_ClausGrp";
		break;
	/////////////////////////////
	//Process_Add_New_PopUp #1
		//////////////////////////////////////////////////////////
	case ID_NEWCARMODULE_CARENTRY:		
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_2
		strScreenTag="HSE_TgCrEntry";
		strScreenTableName="HSE_CrEntry";
		//////////////////
		//RQ-SF-9-1-23_2_1
		{
			CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);
			CString strOwnerDep= getOwnerDept("");
			if(strEmpDep!="" && strEmpDep!=strOwnerDep)
				strCriteria=" WHERE CRENTRY_DPR='" + CString(strEmpDep) +CString("'");//This merges with the default criteria in FirstSys
		}
		///////////////////////////////////////////////////////
		break;
		////////////////////////////////////////////////////////
	case ID_NEWCARMODULE_CARREVIEW:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_3_1
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgCrRvw";
		strScreenTableName="HSE_CrEntry";
		break;
		////////////////////////////////////////////////////////
	case ID_NEWCARMODULE_CARAPPROVAL:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_4_1
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgCrAprvl";
		strScreenTableName="HSE_CrEntry";
		////////////////////////////////////////////////////////
		break;
	case ID_NEWCARMODULE_ACTIONSENTRY:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_5_1d
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgActnsEntry";
		strScreenTableName="HSE_CrEntry";
		{
		//RQ-SF-9-1-23_5_1 (Main) screen
		/*
			If (@user department) = (@owner department) then 
			View list of all CAR records, where (CAR Status = 12 & 15)
			ELSE view list of all CAR records, where :
			(CAR Status = 12 & 15) and
			(main > department) = (@user department)
		*/
		/*RQ_SF_23_3_23_2 
			New (Actions Entry) screen
			At screen open,
			If (@user department) = (@owner department) then 
			View list of all CAR records, where (CAR Status = 12 & 15)

			ELSE view list of all CAR records, where :

			(CAR Status = 12 & 15) and (main > concerned department) = (@user department)

			All the above spec's is correct, except only in the last line, we need to use (main > concerned department) instead of the requester (department)
		*/
			CString strOwnerDepartment=getOwnerDept("");
			CString strUserDepartment=getEmpDep(m_pCategory->GetUserLogin(),true);
			if(strOwnerDepartment!=strUserDepartment)
				//strCriteria.Format("WHERE CRENTRY_DPR='%s'",strUserDepartment);RQ_SF_23_3_23_2
				strCriteria.Format("WHERE isnull(CRENTRY_CNCDPR,'')='%s'",strUserDepartment);
			///////////////////////////////////////////////////////////////
		}
		////////////////////////////////////////////////////////
		break;
	case ID_NEWCARMODULE_ACTIONSREVIEW:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_6_1
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgActnsRviw";
		strScreenTableName="HSE_CrEntry";
		////////////////////////////////////////////////////////
		break;
	case ID_NEWCARMODULE_ACTIONSRECEIVED:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_7 (Actions Received) Screen
		/*
			If (HSE Policy) screen > (CAR) TAB > (Skip Execution Department = Yes) 
			then display error message (Sorry, this screen is disabled) & don’t open this screen
		*/
		if(GetPolicyTabField("HSE_HSEPlcCAR","HSEPLCCAR_SkpExcDpr")=="Y")
		{
			ShowMessage("Sorry, this screen is disabled	");
		}
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgActnsRcvd";
		strScreenTableName="HSE_vwActnsRcvd";
		/*
			RQ-SF-9-1-23_7_1 and Bugs-3-2023.22 since the RQ was not implemented
			Open (Actions Received) screen
			If (@user department) = (@owner department) then 
			View list of detail records from (Root Causes) TAB > (Corrective Actions) popup, where :
			(main > CAR Status = 19) and 
			(Action Status = 3)
			If (@user department) <> (@owner department) then 
			View list of detail records from (Root Causes) TAB > (Corrective Actions) popup, where :
			(main > CAR Status = 19) and 
			(Action Status = 3) and
			(@user department) = (Execution Department)
			Disable standard buttons (NEW) & (DELETE)	 
		*/
		{
			CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);
			CString strOwnerDep= getOwnerDept("");
			if(strEmpDep==strOwnerDep)
				strCriteria=" WHERE CRENTRY_CRSTT=19 and CRRCTVACTNS_ACTSTT in (3)";
			else
				strCriteria=" WHERE CRENTRY_CRSTT=19 and CRRCTVACTNS_ACTSTT in (3) and isnull(CRRCTVACTNS_EXCDPR,'')='" + strEmpDep +CString("'");
		}			
		break;
		////////////////////////////////////////////////////////
	case ID_NEWCARMODULE_ACTIONSUNDEREXECUTION:
		//RQ-SF-9-1-23_8 (Actions Under Execution) Screen
		/*
			If (HSE Policy) screen > (CAR) TAB > (Skip Execution Department = Yes) 
			then display error message (Sorry, this screen is disabled) & don’t open this screen
		*/
		if(GetPolicyTabField("HSE_HSEPlcCAR","HSEPLCCAR_SkpExcDpr")=="Y")
		{
			ShowMessage("Sorry, this screen is disabled	");
		}
		strScreenTag="HSE_TgActnsUndrExec";
		strScreenTableName="HSE_vwActnsRcvd";
		//RQ-SF-9-1-23_8_1 (Main) screen
		/*
		If (@user department) = (@owner department) then 
				View list of detail records from (Root Causes) TAB > (Corrective Actions) popup, where :
		(main > CAR Status = 19) and 
		(Action Status = 5 & 6 & 7)
		If (@user department) <> (@owner department) then 
				  View list of detail records from (Root Causes) TAB > (Corrective Actions) popup, where :
		(main > CAR Status = 19) and 
		(Action Status = 5 & 6 & 7) and
		(@user department) = (Execution Department)
		*/
		{
			CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);
			CString strOwnerDep= getOwnerDept("");
			if(strEmpDep==strOwnerDep)
				strCriteria=" WHERE CRENTRY_CRSTT=19 and CRRCTVACTNS_ACTSTT in (5,6,7)";
			else
			{
				if(strEmpDep!="")
					strCriteria=" WHERE CRENTRY_CRSTT=19 and CRRCTVACTNS_ACTSTT in (5,6,7) and CRRCTVACTNS_EXCDPR='" + strEmpDep +CString("'");
				else
					strCriteria=" WHERE CRENTRY_CRSTT=19 and CRRCTVACTNS_ACTSTT in (5,6,7) and CRRCTVACTNS_EXCDPR is NULL";
			}
		}
		//Disable standard buttons (NEW) & (DELETE)
		m_bEnableMainScreen=FALSE;
		break;
	case ID_NEWCARMODULE_CARFOLLOWUP:
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_9_1
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgCrFloUp";
		strScreenTableName="HSE_CrEntry";
		////////////////////////////////////////////////////////
		break;
		//////////////////////////////////////////////////////////
	case ID_PERFORMANCE_ACTIONTRACKING_CAREDITING:
		////////////////////////////////////////////////////////
		//RQ_SF_23_5_23_1.9.1
		m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_TgCrEdtng";
		strScreenTableName="HSE_CrEntry";
		////////////////////////////////////////////////////////
		break;
		//////////////////////////////////////////////////////////
	case ID_PERFORMANCE_AUDITING_AUDITRESULTCONFIRMATION:
		////////////////////////////////////////////////////////
		//RQ_HM_17_8_23.01.23.01
		//m_bEnableMainScreen=FALSE;
		strScreenTag="HSE_ADTRSLTCNFRMTN";
		strScreenTableName="HSE_AudtPlnEntry";
		///////////////////////////////////////
		//RQ-9-2023.50
		//In (main) screen, DISABLE (NEW) & (DELETE) buttons
		m_bEnableMainScreen=FALSE;
		////////////////////////////////////////////////////////
		break;

	case ID_POPUPS_SUMMARY_NC_DESCRIPTION:
		//RQ_HM_17_8_23.01.17.02
		strScreenTag="HSE_TGSMMRYNCDSCRPTN";
		strScreenTableName="HSE_SMMRYNCDSCRPTN";
		////////////////////////////////////////////////////////
		break;
	}
	if(strTableName!="" && strScreenTag!="")
	{
		strTableName.MakeUpper();
		strScreenTag.MakeUpper();
		if(strTableName!=strScreenTag)
		{
			strScreenTag="";
			m_nScreenId=-1;
		}
	}
	strTableName.MakeUpper();
	if (strTableName!="" && strScreenTag=="" && m_nScreenId<=0) 
	{
		if (strTableName!=strScreenTag) 
			strScreenTag=strTableName;				
		if (strTableName=="HSE_CHMCLHZRD")
			m_nScreenId=HSE_CHEMICAL_HAZARD;
		else if(strTableName=="HSE_CHMCLITMSDFNTN")
			m_nScreenId=HSE_CHEMICAL_ITEMS_DEFINITION;
		else if(strTableName=="HSE_CHMCLRGSTR")
			m_nScreenId=HSE_CHEMICAL_REGISTER;
		else if(strTableName=="HSE_CHMCLITMFRQUNCY")
			m_nScreenId= HSE_CHEMICAL_ITEM_FREQUENCY;
		else if(strTableName=="HSE_ASPCTTYP")
			m_nScreenId=HSE_ASPECT_TYPE;
		else if(strTableName=="HSE_ASPCTACTVTY")
			m_nScreenId=HSE_ASPECT_ACTIVITY;
		else if(strTableName=="HSE_ASPCTFRQUNCY")
			m_nScreenId=HSE_ASPECT_FREQUENCY;
		else if(strTableName=="HSE_ASPCTUOM")
			m_nScreenId=HSE_ASPECT_UOM;
		else if(strTableName=="HSE_ASPCTSTT")
			m_nScreenId=HSE_ASPECT_STATE;
		else if(strTableName=="HSE_ASPCTSENTRYATENTRY")
			m_nScreenId=HSE_ASPECTS_ENTRY_ENTRY;
		else if(strTableName=="HSE_DRLLTYP_VW" || strTableName=="HSE_DRLLTYP")
			m_nScreenId=HSE_DRILL_TYPE;	
		else if(strTableName=="HSE_EXTRNLSUPPRT")
			m_nScreenId=HSE_EXTERNAL_SUPPORT;		
		else if(strTableName=="HSE_EMTLST")
			m_nScreenId=HSE_EMT_LIST;	
		else if(strTableName=="HSE_DRLLRL")
			m_nScreenId=HSE_DRILL_ROLE;	
		else if(strTableName=="HSE_INTRNLRSURC")
			m_nScreenId=HSE_INTERNAL_RESOURCE;	
		
		else if (strTableName=="CMS_RSNS")
			m_nScreenId=ID_CMS_Rsns;
		else if (strTableName=="CMS_ACTNRSN")
			m_nScreenId=ID_CMS_ActnRsn;
		else if (strTableName=="HSE_WRKLOC")
			m_nScreenId=ID_HSE_WRKLOC;
//		else if (strTableName=="HSE_TGCRRCTVACTNS")
//			m_nScreenId=ID_HSE_POP_UP_CRCTVACTN;
		else if (strTableName=="HSE_TGQLTYSTNDRD")
			m_nScreenId=ID_SETUP_AUDITS_QUALITYSTANDARDS;
		else if (strTableName=="HSE_TGAWRNSPLN_CRS_TRGTDPRT")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2021(Batch2).38,41
			CAwrnsPlnCrsTrgtDprtCategory* pCAwrnsPlnCrsTrgtDprtCategory=new CAwrnsPlnCrsTrgtDprtCategory(m_pCategory,strCriteria);
			pCAwrnsPlnCrsTrgtDprtCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGAWRNSPLN_CRS_INSTRCTR")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2021(Batch2).39,42
			CAwrnsPlnCrsInstrctrCategory* pAwrnsPlnCrsInstrctrCategory=new CAwrnsPlnCrsInstrctrCategory(m_pCategory,strCriteria);
			pAwrnsPlnCrsInstrctrCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGAWRNSPLN_CRS_TRGTDPRT")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2021(Batch2).39,42
			CAwrnsPlnCrsInstrctrCategory* pAwrnsPlnCrsInstrctrCategory=new CAwrnsPlnCrsInstrctrCategory(m_pCategory,strCriteria);
			pAwrnsPlnCrsInstrctrCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}

		//RQ-2-2021(Batch2).28
		else if (strTableName=="HSE_TGINCDNTMDCLRPRTINQ")
			m_nScreenId=ID_INQUIRYSCREENS_INCIDENTMEDICALINQUIRY;

		else if (strTableName=="HSE_TGATNDGRD")
			m_nScreenId=ID_SETUP_AWARENESS_ATTENDEEGRADES;

		else if (strTableName=="HSE_TGPTNLHZRD")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-9-2021.1	
			CRiskAssessmentHzrdPopCategory* pCRiskAssessmentHzrdPopCategory=new CRiskAssessmentHzrdPopCategory(m_pCategory,strCriteria);
			//RQ-1-2022.33
			pCRiskAssessmentHzrdPopCategory->ShowMenuScreenV2("HSE_PTNLHZRD",strCriteria);	
			////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGTSKRSP")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-9-2021.1
			CTaskResponsibilityPopCategory* pTaskResponsibilityPopCategory=new CTaskResponsibilityPopCategory(m_pCategory,strCriteria);
			pTaskResponsibilityPopCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		//RQ-9-2021.5
		//HSM Sep 29 2021  6:35PM
		else if (strTableName=="HSE_TGACTNCLSSFCTN")
			m_nScreenId=ID_SETUP_CAR_ACTIONCLASSIFICATION;
		////////////////////////////////////////////////////////////////////////


	//HSE Audit Module 25/11/2021
		else if (strTableName=="HSE_AUDTSTNDRDS")// || strTableName=="HSE_AUDTCLUSS" || strTableName=="HSE_AUDT" || strTableName=="HSE_CRTFCTTYP" || strTableName=="HSE_AUDTRL" || strTableName=="HSE_CRTFCTPRVDR" || strTableName=="HSE_UN_PLNNDAUDTJUSTFCTN" || strTableName=="HSE_UN_PLNNDAUDTJUSTFCTN" || strTableName=="HSE_SCHDULCHNGRSN" || strTableName=="HSE_AUDTPLNENTRY" || strTableName=="HSE_AUDTPLNAPPRVL" || strTableName=="HSE_AUDTPLNAPPRVL" || strTableName=="HSE_AUDTNTFCTN" || strTableName=="HSE_AUDTEXCUTNRSULT" || strTableName=="HSE_AUDTACTNSENTRY" || strTableName=="HSE_AUDTACTNSCNFRMTN")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTSTNDRDS","HSE_AUDTSTNDRDS","AUDTSTNDRDS_SRL","AUDTSTNDRDS_SRL");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}			
		else if (strTableName=="HSE_AUDTCLUSS")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTCLUSS","HSE_AUDTCLUSS","AUDTCLUSS_SRL","AUDTCLUSS_SRL");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////

		}
		else if (strTableName=="HSE_AUDT")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDT","HSE_AUDT","AUDT_SRL","AUDT_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDT_AUDTUNT","AUDT_AUDTUNT_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDT_AUDTUNT","MAINLINK");

			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_CRTFCTTYP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_CRTFCTTYP","HSE_CRTFCTTYP","CRTFCTTYP_SRL","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTRL")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTRL","HSE_AUDTRL","AUDTRL_SRL","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_CRTFCTPRVDR")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_CRTFCTPRVDR","HSE_CRTFCTPRVDR","CRTFCTPRVDR_SRL","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_UN_PLNNDAUDTJUSTFCTN")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_UN_PLNNDAUDTJUSTFCTN","HSE_UN_PLNNDAUDTJUSTFCTN","UN_PLNNDAUDTJUSTFCTN_SRL","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_SCHDULCHNGRSN")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_SCHDULCHNGRSN","HSE_SCHDULCHNGRSN","SCHDULCHNGRSN_SRL","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_DFULTDSTRBUTNLST")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_DFULTDSTRBUTNLST","HSE_DFULTDSTRBUTNLST","","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_CRTFDAUDTRS")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_CRTFDAUDTRS","HSE_CRTFDAUDTRS","","PrmKy","");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_CRTFDAUDTRS_CRTFCTS","CRTFDAUDTRS_CRTFCTS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_CRTFDAUDTRS_CRTFCTS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_VW_CRTFDAUDTRS")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_VW_CRTFDAUDTRS","HSE_VW_CRTFDAUDTRS","","PrmKy","");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_CRTFDAUDTRS_CRTFCTS","CRTFDAUDTRS_CRTFCTS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_CRTFDAUDTRS_CRTFCTS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTPLNENTRY")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTPLNENTRY","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTPLNENTRY_AUDTS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTPLNAPPRVL")
		{
			//m_nScreenId=ID_AUDITPLANAPPROVAL;
			//////////////////////////////////////////////////////////////////////////
			//strTableName=strScreenTag;
			//strScreenTag="HSE_AUDTPLNENTRY";
			pmpListOfParameters=InitializeParameters("HSE_AUDTPLNAPPRVL","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTNTFCTN")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTNTFCTN","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTNTFCTN_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTNTFCTN_AUDTS","MAINLINK");
			AddTabInfo(pmpListOfParameters,"DetailsTableName","HSE_AUDTNTFCTN_AUDTS","HSE_AUDTPLNENTRY_AUDTS");

			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY",strCriteria);	
			/////////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.37
			EnableToolbar("HSE_AUDTNTFCTN","",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_AUDTNTFCTN","",TOOLBAR_DELETE,FALSE);
			//RQ-12-2021.36
			EnableToolbar("HSE_AUDTCHCKLST_DTLS",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_AUDTCHCKLST_DTLS",TOOLBAR_DELETE,FALSE);			
			/////////////////////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTEXCUTNRSULT")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTEXCUTNRSULT","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTEXCUTNRSULT_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTEXCUTNRSULT_AUDTS","MAINLINK");
			AddTabInfo(pmpListOfParameters,"DetailsTableName","HSE_AUDTEXCUTNRSULT_AUDTS","HSE_AUDTPLNENTRY_AUDTS");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY",strCriteria);	
			//RQ-12-2021.43
			EnableToolbar("HSE_AUDTEXCUTNRSULT","",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_AUDTEXCUTNRSULT","",TOOLBAR_DELETE,FALSE);
			/////////////////////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTACTNSENTRY")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTACTNSENTRY","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTACTNSENTRY_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTACTNSENTRY_AUDTS","MAINLINK");
			AddTabInfo(pmpListOfParameters,"DetailsTableName","HSE_AUDTACTNSENTRY_AUDTS","HSE_AUDTPLNENTRY_AUDTS");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY",strCriteria);	
			/////////////////////////////////////////////////////////////
			//RQ-12-2021.63
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			/////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////
			//RQ-12-2021.64
			EnableToolbar("HSE_AUDTACTNSENTRY_AUDTS",TOOLBAR_NEW,FALSE);
			/////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		//--Commented the following section according to RQ-1-2022.25 
		/*else if (strTableName=="HSE_AUDTACTNSCNFRMTN")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTACTNSCNFRMTN","HSE_AUDTACTNSCNFRMTN","AUDTACTNSCNFRMTN_PLNN","MasterKey");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS","AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS","MAINLINK");
			//AddTabInfo(pmpListOfParameters,"DetailsTableNames","HSE_AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS","HSE_AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS");

			
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			/////////////////////////////////////////////////////////////
			//RQ-12-2021.69
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			EnableToolbar("",TOOLBAR_DELETE,FALSE);
			/////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}*/
		//else if (strTableName=="HSE_AUDTACTNSCNFRMTN")
		else if (strTableName=="HSE_AUDTACTNCNFRM")			
		{
			//RQ-1-2022.25
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTACTNCNFRM","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTACTNSENTRY_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTACTNSENTRY_AUDTS","MAINLINK");
			AddTabInfo(pmpListOfParameters,"DetailsTableName","HSE_AUDTACTNSENTRY_AUDTS","HSE_AUDTPLNENTRY_AUDTS");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY",strCriteria);	
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			///RQ-1-2022.31
			EnableToolbar("",TOOLBAR_DELETE,FALSE);
			////////////
			EnableToolbar("HSE_AUDTACTNSENTRY_AUDTS",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		///////////////		
		else if (strTableName=="HSE_AUDTCHCKLST")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTCHCKLST","HSE_AUDTCHCKLST","AUDTCHCKLST_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTCHCKLST_DTLS","AUDTCHCKLST_DTLS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTCHCKLST_DTLS","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTCHCKLST_LNK") //#POPUPSCREEN2
		{
			/*
				Steps to open pop up screen in a locked version
				1- exec master.[dbo].[Process_Copy_Screen] '66','HSE_AUDTCHCKLST','HSE_AUDTCHCKLST_LNK','RQ-12-2021.8'
				2- Add section like #POPUPSCREEN1 in CHSEMSCommonCategory::ShowMenuScreenV2
				3- Add section like #POPUPSCREEN2 in CHSEMSCommonCategory::ShowMenuScreenV2
				4- Add section like #POPUPSCREEN in CAuditModuleCategory::DisplayCustomButtonClicked
				5- Add tabs if found as in #Tabs
			*/
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTCHCKLST_LNK","HSE_AUDTCHCKLST","AUDTCHCKLST_SRL","PrmKy","MainLink");
			////////////////////////////////////////////////////////////////////////////////////
			//#Tabs
			//Add the following lines if the screen has tabs and repeat for all tabs
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTCHCKLST_DTLS","AUDTCHCKLST_DTLS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTCHCKLST_DTLS","MainLink");
			////////////////////////////////////////////////////////////////////////////////////
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTCHCKLST",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			////////////////////////////////////////////////////////////////////////////////////
			//#Tabs
			//Add the following lines if the screen has tabs and repeat for all tabs
			EnableToolbar("HSE_AUDTCHCKLST_DTLS",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_AUDTCHCKLST_DTLS",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_AUDTCHCKLST_DTLS",TOOLBAR_SAVE,FALSE);
			////////////////////////////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTCLUSSPPUP_LNK") 
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTCLUSSPPUP_LNK","HSE_IDNTFYAUDTCLUSSPPUP","IDNTFYAUDTCLUSSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTCLUSSPPUP",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTCLUSSPPUP_EDT") 
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ_HM_17_8_23.01.16.02
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTCLUSSPPUP_EDT","HSE_IDNTFYAUDTCLUSSPPUP","IDNTFYAUDTCLUSSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTCLUSSPPUP",strCriteria);	
			/////////////////////////////////////////////////////////
			//Bugs-9-2023.21
			//pAuditModuleCategory->LockScreen(true);
			//EnableToolbar("",TOOLBAR_NEW,FALSE);
			//////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTLCTNSPPUP_LNK") //#2
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTLCTNSPPUP_LNK","HSE_IDNTFYAUDTLCTNSPPUP","IDNTFYAUDTLCTNSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTLCTNSPPUP",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		
		else if (strTableName=="HSE_IDNTFYNDDTLSPPUP_LNK") //#3
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYNDDTLSPPUP_LNK","HSE_IDNTFYNDDTLSPPUP","IDNTFYNDDTLSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYNDDTLSPPUP",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTRSTMPPUP_LNK") //#4
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTRSTMPPUP_LNK","HSE_IDNTFYAUDTRSTMPPUP","IDNTFYAUDTRSTMPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTRSTMPPUP",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTRSTMPPUP_NTFC") //
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTRSTMPPUP_NTFC","HSE_IDNTFYAUDTRSTMPPUP","IDNTFYAUDTRSTMPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTRSTMPPUP",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_LNKDAUDTCLUSSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_LNKDAUDTCLUSSPPUP","HSE_LNKDAUDTCLUSSPPUP","LNKDAUDTCLUSSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_LNKDAUDTLCTNPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_LNKDAUDTLCTNPPUP","HSE_LNKDAUDTLCTNPPUP","LNKDAUDTLCTNPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_ACTULAUDTPRSNSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_ACTULAUDTPRSNSPPUP","HSE_ACTULAUDTPRSNSPPUP","ACTULAUDTPRSNSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTCLUSSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTCLUSSPPUP","HSE_AUDTCLUSSPPUP","AUDTCLUSSPPUP_SRL","AUDTCLUSSPPUP_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_AUDTCLUSSPPUP_FNDNGS","AUDTCLUSSPPUP_FNDNGS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_AUDTCLUSSPPUP_FNDNGS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			/////////////////////////////////////////////////////////////
			//RQ-12-2021.65
			EnableToolbar("HSE_AUDTCLUSSPPUP_FNDNGS",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_AUDTCLUSSPPUP_FNDNGS",TOOLBAR_DELETE,FALSE);
			/////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTCNCLUSNPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTCNCLUSNPPUP","HSE_AUDTCNCLUSNPPUP","AUDTCNCLUSNPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTSTRNGTHPNTSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTSTRNGTHPNTSPPUP","HSE_AUDTSTRNGTHPNTSPPUP","AUDTSTRNGTHPNTSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTWKPNTSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_AUDTWKPNTSPPUP","HSE_AUDTWKPNTSPPUP","AUDTWKPNTSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_CLUS_LVLRSULTPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_CLUS_LVLRSULTPPUP","HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_CLUS_LVLRSULTPPUP_FNDNGS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			////////////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.33
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			EnableToolbar("",TOOLBAR_DELETE,FALSE);
			////////////////////////////////////////////////////////////////////////////////
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTCLUSSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTCLUSSPPUP","HSE_IDNTFYAUDTCLUSSPPUP","IDNTFYAUDTCLUSSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTLCTNSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTLCTNSPPUP","HSE_IDNTFYAUDTLCTNSPPUP","IDNTFYAUDTLCTNSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTRSTMPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTRSTMPPUP","HSE_IDNTFYAUDTRSTMPPUP","IDNTFYAUDTRSTMPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYNDDTLSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYNDDTLSPPUP","HSE_IDNTFYNDDTLSPPUP","IDNTFYNDDTLSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYSHFTGRUPSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYSHFTGRUPSPPUP","HSE_IDNTFYSHFTGRUPSPPUP","IDNTFYSHFTGRUPSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IMPRVMNTOPRTUNTYPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_IMPRVMNTOPRTUNTYPPUP","HSE_IMPRVMNTOPRTUNTYPPUP","IMPRVMNTOPRTUNTYPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_MRPRSNSTNTFYPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_MRPRSNSTNTFYPPUP","HSE_MRPRSNSTNTFYPPUP","MRPRSNSTNTFYPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_RCMMNDDACTNSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_RCMMNDDACTNSPPUP","HSE_RCMMNDDACTNSPPUP","RCMMNDDACTNSPPUP_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_RCMMNDDACTNSPPUP_RTCUSS","RCMMNDDACTNSPPUP_RTCUSS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_RCMMNDDACTNSPPUP_RTCUSS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_VWRCMMNDDACTNSPPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_VWRCMMNDDACTNSPPUP","HSE_VWRCMMNDDACTNSPPUP","VWRCMMNDDACTNSPPUP_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_VWRCMMNDDACTNSPPUP_RTCUSS","VWRCMMNDDACTNSPPUP_RTCUSS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_VWRCMMNDDACTNSPPUP_RTCUSS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IDNTFYAUDTRSTMPPUP_AER") //
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.40
			pmpListOfParameters=InitializeParameters("HSE_IDNTFYAUDTRSTMPPUP_AER","HSE_IDNTFYAUDTRSTMPPUP","IDNTFYAUDTRSTMPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_IDNTFYAUDTRSTMPPUP",strCriteria);	
			//RQ-12-2021.41
			EnableToolbar("HSE_IDNTFYAUDTRSTMPPUP_AER","",TOOLBAR_DELETE,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_CLUS_LVLRSULTPPUP_LNK")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.10
			pmpListOfParameters=InitializeParameters("HSE_CLUS_LVLRSULTPPUP_LNK","HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_CLUS_LVLRSULTPPUP_FNDNGS","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CLUS_LVLRSULTPPUP",strCriteria);	
			//RQ-1-2022.11
			EnableToolbar("HSE_CLUS_LVLRSULTPPUP_LNK","",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_CLUS_LVLRSULTPPUP_LNK","",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}

		else if (strTableName=="HSE_CLUS_LVLRSULTPPUP_AAE")
		{
			//////////////////////////////////////////////////////////////////////////
			//Bugs-4-2022.11
			pmpListOfParameters=InitializeParameters("HSE_CLUS_LVLRSULTPPUP_AAE","HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_SRL","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_CLUS_LVLRSULTPPUP_FNDNGS_AAE","CLUS_LVLRSULTPPUP_FNDNGS_SRL");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_CLUS_LVLRSULTPPUP_FNDNGS_AAE","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CLUS_LVLRSULTPPUP",strCriteria);	
			EnableToolbar("HSE_CLUS_LVLRSULTPPUP_AAE","",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_CLUS_LVLRSULTPPUP_AAE","",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGADTE_CNTCTPRSN")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.39
			pmpListOfParameters=InitializeParameters("HSE_TgAdte_CntctPrsn","HSE_Adte_CntctPrsn","Adte_CntctPrsn_Srl","PrmKy","MainLink");
			//pmpListOfParameters->SetAt("Msg",(CObject*)msg);
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_Adte_CntctPrsn",strCriteria);	
			
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_ACTULAUDTPRSNSPPUP_NTFC")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.43
			pmpListOfParameters=InitializeParameters("HSE_ACTULAUDTPRSNSPPUP_NTFC","HSE_ACTULAUDTPRSNSPPUP","ACTULAUDTPRSNSPPUP_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_ACTULAUDTPRSNSPPUP",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}

		else if (strTableName=="HSE_AUDTSTRNGTHPNTSPPUP_MAIN")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.52
			pmpListOfParameters=InitializeParameters("HSE_AUDTSTRNGTHPNTSPPUP_MAIN","HSE_AUDTSTRNGTHPNTSPPUP_MAIN","AUDTSTRNGTHPNTSPPUP_Srl","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTCNCLUSNPPUP_MAIN")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.52
			pmpListOfParameters=InitializeParameters("HSE_AUDTCNCLUSNPPUP_MAIN","HSE_AUDTCNCLUSNPPUP_MAIN","AUDTCNCLUSNPPUP_Srl","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_AUDTWKPNTSPPUP_MAIN")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.52
			pmpListOfParameters=InitializeParameters("HSE_AUDTWKPNTSPPUP_MAIN","HSE_AUDTWKPNTSPPUP_MAIN","AUDTWKPNTSPPUP_Srl","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_IMPRVMNTOPRTUNTYPPUP_MAIN")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-1-2022.52
			pmpListOfParameters=InitializeParameters("HSE_IMPRVMNTOPRTUNTYPPUP_MAIN","HSE_IMPRVMNTOPRTUNTYPPUP_MAIN","IMPRVMNTOPRTUNTYPPUP_Srl","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCRRCTNACT")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.7
			pmpListOfParameters=InitializeParameters("HSE_TGCRRCTNACT","HSE_CRRCTNACTNS","CRRCTNACTNS_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CRRCTNACTNS",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCRRCTNACT_AAC")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-4-2022.32
			pmpListOfParameters=InitializeParameters("HSE_TGCRRCTNACT_AAC","HSE_CRRCTNACTNS","CRRCTNACTNS_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CRRCTNACTNS",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGDPRTMNTGRP")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.11
			pmpListOfParameters=InitializeParameters("HSE_TGDPRTMNTGRP","HSE_DPRTMNTGRP","","DPRTMNTGRP_GRPN","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_DPRTMNTGRP",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCLAUSGRP")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.13
			pmpListOfParameters=InitializeParameters("HSE_TGCLAUSGRP","HSE_CLAUSGRP","","CLAUSGRP_GRPN","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CLAUSGRP",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGSMLNC")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.4
			pmpListOfParameters=InitializeParameters("HSE_TGSMLNC","HSE_SMLNC","","","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_SMLNC",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGSMLNC_LNK")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.9
			pmpListOfParameters=InitializeParameters("HSE_TGSMLNC_LNK","HSE_SMLNC","","","");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_SMLNC",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGRTCSS")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-2-2022.8
			pmpListOfParameters=InitializeParameters("HSE_TGRTCSS","HSE_RTCSS","RtCss_LNRNUM","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_TGRTCSSRCMNDDACTN","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_RTCSS",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGRTCSS_AAC")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-4-2022.27
			pmpListOfParameters=InitializeParameters("HSE_TGRTCSS_AAC","HSE_RTCSS","RtCss_LNRNUM","PrmKy","MainLink");
			AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			AddTabInfo(pmpListOfParameters,"DetailsKeys","HSE_TGRTCSSRCMNDDACTN","MAINLINK");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_RTCSS",strCriteria);	
			pAuditModuleCategory->LockScreen(true);
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_TgRtCssRcmnddActn",TOOLBAR_NEW,FALSE);
			EnableToolbar("HSE_TgRtCssRcmnddActn",TOOLBAR_DELETE,FALSE);
			EnableToolbar("HSE_TgRtCssRcmnddActn",TOOLBAR_SAVE,FALSE);

			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		//Process_Add_New_PopUp #2

		//////////////////////////////////////////////////////////////////////////////////
		else if (strTableName=="HSE_TGCRENTRY")
		{
		////////////////////////////////////////////////////////
		//RQ-SF-9-1-23_2
//			m_nScreenId=ID_NEWCARMODULE_CARENTRY;
			pmpListOfParameters=InitializeParameters("HSE_TGCRENTRY","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			pNewCarEntryCategory->SetEnableMainScreen(TRUE,TRUE,TRUE);
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCRRVW")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ-SF-9-1-23_3_1
//			m_nScreenId=ID_NEWCARMODULE_CARENTRY;
			pmpListOfParameters=InitializeParameters("HSE_TGCRRVW","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			pNewCarEntryCategory->SetEnableMainScreen(TRUE);
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCRAPRVL")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ-SF-9-1-23_4_1
//			m_nScreenId=ID_NEWCARMODULE_CARENTRY;
			pmpListOfParameters=InitializeParameters("HSE_TGCRAPRVL","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			pNewCarEntryCategory->SetEnableMainScreen(FALSE);
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGACTNSENTRY")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ-SF-9-1-23_5_1
			pmpListOfParameters=InitializeParameters("HSE_TGACTNSENTRY","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			pNewCarEntryCategory->SetEnableMainScreen(FALSE);
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGACTNSRVIW")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ-SF-9-1-23_6_1
			pmpListOfParameters=InitializeParameters("HSE_TGACTNSRVIW","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			pNewCarEntryCategory->SetEnableMainScreen(TRUE);
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCRFLOUP")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ-SF-9-1-23_9_1
			pmpListOfParameters=InitializeParameters("HSE_TGCRFLOUP","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			//AddTabInfo(pmpListOfParameters,"DetailsSerials","HSE_TGRTCSSRCMNDDACTN","RTCSSRCMNDDACTN_ACTNNO");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			//RQ-SF-9-1-23_9_1 (Main) screen
			//Disable standard buttons (NEW) & (DELETE)
			pNewCarEntryCategory->SetEnableMainScreen(TRUE);
			/////////
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCREDTNG")
		{
			//////////////////////////////////////////////////////////////////////////			
			//RQ_SF_23_5_23_1.9.1
			pmpListOfParameters=InitializeParameters("HSE_TGCREDTNG","HSE_CRENTRY","CRENTRY_CRN","PrmKy","CRENTRY_CRYR");
			pNewCarEntryCategory=new CNewCarEntryCategory(m_pCategory,pmpListOfParameters);
			//RQ_SF_23_5_23_1.9.1 (Main) screen
			//DISABLE standard buttons (NEW) & (DELETE)
			pNewCarEntryCategory->SetEnableMainScreen(TRUE);
			/////////
			pNewCarEntryCategory->ShowMenuScreenV2("HSE_CRENTRY",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		//BUG_HM_2_8_23.02: Added HSE_TGCRRCTVACTNS_ACTENT
		else if (strTableName=="HSE_TGCRRCTVACTNS" || strTableName=="HSE_TGCRRCTVACTNS_RVW" ||strTableName=="HSE_TGCRRCTVACTNSFLWUP" || strTableName=="HSE_TGCRRCTVACTNS_ACTENT")
		{
			//////////////////////////////////////////////////////////////////////////			
			pmpListOfParameters=InitializeParameters(strTableName.GetBuffer(0),"HSE_CRRCTVACTNS","CrrctvActns_Srl","PrmKy","CrrctvActns_LnkWthMn");
			pmpListOfParameters->SetAt("Msg",(CObject*)msg);
			if(msg)
				gstrCARDate=(char*)msg;

			pCorrectiveActionEntry=new CCorrectiveActionEntry(m_pCategory,pmpListOfParameters,strCriteria);
			//RQ_HM_6_3_23_2: In New Actions Review (Root Causes) Tab > (Corrective Actions) popup DISABLE standard buttons (NEW) & (DELETE) 
			if(strTableName=="HSE_TGCRRCTVACTNS_RVW")
				pCorrectiveActionEntry->SetEnableMainScreen(TRUE,FALSE,FALSE);
			////////////////////////
			//RQ_SF_15_3_23_3: New (CAR Follow-up) > (Root Causes) Tab > (Corrective Actions) popup Disable standard buttons (NEW) & (DELETE)
			if(strTableName=="HSE_TGCRRCTVACTNSFLWUP")
				pCorrectiveActionEntry->SetEnableMainScreen(TRUE,FALSE,FALSE);
			////////////////////////
			pCorrectiveActionEntry->ShowMenuScreenV2("HSE_CRRCTVACTNS",strCriteria);	
			pCorrectiveActionEntry=NULL;
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}

		else if (strTableName=="HSE_TGACTNSRCVD")
		{
			//m_nScreenId=ID_NEWCARMODULE_ACTIONSRECEIVED;
			//////////////////////////////////////////////////////////////////////////			
			pmpListOfParameters=InitializeParameters(strTableName.GetBuffer(0),"HSE_vwActnsRcvd","","PrmKy");
			pCorrectiveActionEntry=new CCorrectiveActionEntry(m_pCategory,pmpListOfParameters,strCriteria);
			pCorrectiveActionEntry->SetEnableMainScreen(TRUE,FALSE,FALSE);
			pCorrectiveActionEntry->ShowMenuScreenV2("HSE_vwActnsRcvd",strCriteria);	
			pCorrectiveActionEntry=NULL;
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}

		else if (strTableName=="HSE_TGACTNSUNDREXEC")
		{
			//m_nScreenId=ID_NEWCARMODULE_ACTIONSRECEIVED;
			//////////////////////////////////////////////////////////////////////////			
			pmpListOfParameters=InitializeParameters("HSE_TGACTNSUNDREXEC","HSE_vwActnsRcvd","","PrmKy");
			pCorrectiveActionEntry=new CCorrectiveActionEntry(m_pCategory,pmpListOfParameters,"where CRENTRY_CRSTT=");
			//pCorrectiveActionEntry->SetEnableMainScreen(TRUE,TRUE,TRUE);
			//RQ-SF-9-1-23_8_1 (Main) screen
			//Disable standard buttons (NEW) & (DELETE)
			pCorrectiveActionEntry->SetEnableMainScreen(TRUE);
			/////////
			pCorrectiveActionEntry->ShowMenuScreenV2("HSE_vwActnsRcvd",strCriteria);	
			pCorrectiveActionEntry=NULL;
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////
		//BUG_HM_27_7_23.01
		else if(strTableName=="HSE_COMPPRF")
		{
			BSTR bstrCriteria=0;
			m_pCategory->GetHSEObj()->ShowCompanyProfile(bstrCriteria);
			return 0;
		}
		///////////////////////////////

		/////////////////////////////
		//BUG_HM_1_3_23_2 
		else if(strTableName=="HSE_TGWRKLOCWRKLOC" || strTableName=="HSE_WRKLOCWRKLOC")
		{
			AfxMessageBox("You are browsing from details");
			return 0;
		}
		///////////////////////////////
		else if(strTableName=="HSE_ADTRSLTCNFRMTN")
		{
			//m_nScreenId=ID_PERFORMANCE_AUDITING_AUDITRESULTCONFIRMATION
			//////////////////////////////////////////////////////////////////////////
			pmpListOfParameters=InitializeParameters("HSE_ADTRSLTCNFRMTN","HSE_AUDTPLNENTRY","AUDTPLNENTRY_PLNN","MasterKey","AUDTPLNENTRY_AUDTYR");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			///////////////////////////////////////
			//RQ-9-2023.50
			//In (main) screen, DISABLE (NEW) & (DELETE) buttons
			pAuditModuleCategory->SetEnableMainScreen(TRUE);
			////////////////////////////////////////////
			pAuditModuleCategory->ShowMenuScreenV2("HSE_AUDTPLNENTRY","");
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (strTableName=="HSE_TGCVRDSTNDRDSPOPUP")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-10-2023.13
			pmpListOfParameters=InitializeParameters("HSE_TGCVRDSTNDRDSPOPUP","HSE_CVRDSTNDRDS","CVRDSTNDRDS_SRL","MasterKey","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CVRDSTNDRDS",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
			
		else if (strTableName=="HSE_TGCVRDSTNDRDSPOPUPLOCKED")
		{
			//////////////////////////////////////////////////////////////////////////
			//RQ-10-2023.13
			pmpListOfParameters=InitializeParameters("HSE_TGCVRDSTNDRDSPOPUPLOCKED","HSE_CVRDSTNDRDS","CVRDSTNDRDS_SRL","PrmKy","MainLink");
			pAuditModuleCategory=new CAuditModuleCategory(m_pCategory,pmpListOfParameters);
			pAuditModuleCategory->SetEnableMainScreen(FALSE);
			pAuditModuleCategory->ShowMenuScreenV2("HSE_CVRDSTNDRDS",strCriteria);	
			return 0;
			//////////////////////////////////////////////////////////////////////////
		}
		else if(strTableName=="HSE_TGSMMRYNCDSCRPTN")
			m_nScreenId=ID_POPUPS_SUMMARY_NC_DESCRIPTION;

		else if(strTableName=="HSE_ROL")
			m_nScreenId=ID_POPUPS_SUMMARY_NC_DESCRIPTION;

		if(strInitialTableName==strTableName &&	ninitialScreenId==m_nScreenId)
		{
			AfxMessageBox("General Error 1972");
			return 0;
		}
		
		ShowMenuScreenV2(strTableName,m_nScreenId, strCriteria,msg);
		strScreenTag="";
		return 0;
	}
	if (strScreenTag!="")
	{		
		return ParentManage(strScreenTag,m_bEnableMainScreen,m_bEnableMainScreen,m_bEnableMainScreen,strScreenTableName,strCriteria);
	}
	return 0;
}

BOOL CHSEMSCommonCategory::ValidFileSize(CString strFileType,double nFileSize,CString strCmpnyNam)
{
	if(checkAttachmentFormat(strCmpnyNam))	{	
		HWND lhWnd = ::GetActiveWindow ();
		double nMaxFileSize = GetMaxFileSize(strFileType);
		CString strErrorMsg;
		if(nMaxFileSize == 0) {		
			strErrorMsg.Format("File format for this file not recorded in the file format screen.", nMaxFileSize);	
			MessageBox(lhWnd, strErrorMsg, "Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if(checkAttachmentSize(strCmpnyNam)) {
			if((nFileSize) > nMaxFileSize) {		
				//strErrorMsg.Format("According your system policy\nMax. file size to be less than or equal to %.2f MB.  ", nMaxFileSize);			
				strErrorMsg.Format("According to your system policy :\nMax. file size to be less than or equal to %.2f KB.  ", nMaxFileSize);			
				MessageBox(lhWnd, strErrorMsg, "Policy Violation!", MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	return TRUE;
}

double CHSEMSCommonCategory::GetMaxFileSize(CString strFileType)
{
	CString strSQL;
	strSQL.Format("SELECT ATCHMNTFRMT_MAXATCHDSZ FROM CMN_ATCHMNTFRMT WHERE ATCHMNTFRMT_EXTNSNNAM = '%s'",strFileType);
	double nMaxFileSize = atof(GetRecordSetData(strSQL).GetBuffer(0));
	return nMaxFileSize;
}

CString CHSEMSCommonCategory::GetSingleCompanyName() {
	CString strSQL;
	CString strCompanyName;
	strSQL.Format("SELECT COUNT(0) AS COMPNY_NUM FROM HSE_COMPPRF");
	int nCompanyNum = atof(GetRecordSetData(strSQL).GetBuffer(0));
	if(nCompanyNum == 1) {
		strSQL.Format("SELECT COMPPRF_COD FROM HSE_COMPPRF");	
		strCompanyName = GetRecordSetData(strSQL).GetBuffer(0);
	}
	return strCompanyName;
}


bool CHSEMSCommonCategory::checkAttachmentFormat(CString strCmpnyNam) {
	CString strSQL;
	bool bCheckFileFormat = false;	
	strSQL.Format("SELECT HSEPLC_CHKATCHFRMT FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strCheckFileFormat = GetRecordSetData(strSQL).GetBuffer(0);
	strCheckFileFormat.MakeLower();
	if(strCheckFileFormat == "true") {
		bCheckFileFormat = true;
	} 
	return bCheckFileFormat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//RQ-8-2021.3
bool CHSEMSCommonCategory::checkRequireCAR() {
	CString strSQL;
	bool bCheck = false;	
	strSQL.Format("SELECT HSEPLCCAR_DFLRQRCR FROM HSE_HSEPLCCAR");
	CString strcheckRequireCAR = GetRecordSetData(strSQL).GetBuffer(0);
	strcheckRequireCAR.MakeLower();
	if(strcheckRequireCAR == "y") {
		bCheck = true;
	} 
	return bCheck;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


bool CHSEMSCommonCategory::checkAttachmentSize(CString strCmpnyNam) {
	CString strSQL;
	bool bCheckFileSize = false;
	strSQL.Format("SELECT HSEPLC_CHKATCHSZ FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strCheckFileSize = GetRecordSetData(strSQL).GetBuffer(0);
	strCheckFileSize.MakeLower();
	if(strCheckFileSize == "true") {
		bCheckFileSize = true;
	} 
	return bCheckFileSize;
}

int CHSEMSCommonCategory::getEmployeeAge(CString strEmpCode) {
	CString strSQL;
	int nEmployeeAge = 0;
	strSQL.Format("(SELECT YEAR(GETDATE())-YEAR(EMPLOYEE_BrthDat) FROM CMN_EMPLOYEE WHERE EMPLOYEE_BrthDat IS NOT NULL AND EMPLOYEE_CODE = '%s')",strEmpCode);
	nEmployeeAge = atoi(GetRecordSetData(strSQL).GetBuffer(0));	
	return nEmployeeAge;
}

int CHSEMSCommonCategory::getEmployeeServiceYears(CString strEmpCode) {
	CString strSQL;
	int nEmployeeServiceYrs = 0;
	strSQL.Format("(SELECT YEAR(GETDATE())-YEAR(EMPLOYEE_JOINDATE) FROM CMN_EMPLOYEE WHERE EMPLOYEE_JOINDATE IS NOT NULL AND EMPLOYEE_CODE = '%s')",strEmpCode);
	nEmployeeServiceYrs = atoi(GetRecordSetData(strSQL).GetBuffer(0));	
	return nEmployeeServiceYrs;
}


int CHSEMSCommonCategory::getEmployeeTotalExpYears(CString strEmpCode) {
	CString strSQL;
	int nEmployeeTotalExpYrs = 0;
	strSQL.Format("(SELECT YEAR(GETDATE())-YEAR(EMPLOYEE_JOINDATE)+EMPLOYEE_PrvsExprncYrs FROM CMN_EMPLOYEE WHERE EMPLOYEE_JOINDATE IS NOT NULL AND EMPLOYEE_CODE = '%s')",strEmpCode);
	nEmployeeTotalExpYrs = atoi(GetRecordSetData(strSQL).GetBuffer(0));	
	return nEmployeeTotalExpYrs;
}

/**
 * get company currency
 * From HSE POLICY SCREEN
 */
CString CHSEMSCommonCategory::getCompanyCurreny(CString strCmpnyNam) {
	CString strCmpnyCrncy,strSQL;	
	strSQL.Format("SELECT HMCRNY FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	strCmpnyCrncy = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCmpnyCrncy;
}

/**
 * get maximun accepted risk rank value from policy screen
 */
CString CHSEMSCommonCategory::getMaxAcptdRskRnk(CString strCompnyNam) {
	CString strMaxAcptdRskRnk,strSQL;	
	strSQL.Format("SELECT HSEPLC_MXRNKACPTDRSK FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCompnyNam);
	strMaxAcptdRskRnk = GetRecordSetData(strSQL).GetBuffer(0);	
	return strMaxAcptdRskRnk;
}

/**
 * get company name from site name
 */
CString CHSEMSCommonCategory::getCmpnyNamFrmSit(CString strSitNam) {
	CString strCmpnyNam,strSQL;	
	strSQL.Format("SELECT STE_CmpnyName FROM HSE_STE WHERE MAINLINK = '%s'",strSitNam);
	strCmpnyNam = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCmpnyNam;
}


bool CHSEMSCommonCategory::hasPendingRequiredActions(CString strTableName,CString strKeyFldVal) {	
	CString strSQL,strPendingRqrdActns = "",strActnFldNam,strLnkFldNam,strRqrCarFldNam,strRqrdActnSts;
	bool bPendingActionExist = false;
	
	strActnFldNam = CString(strTableName);
	strActnFldNam.Replace("HSE_","");
	strActnFldNam = strActnFldNam+"_ACTNNO";

	strLnkFldNam = CString(strTableName);
	strLnkFldNam.Replace("HSE_","");
	strLnkFldNam = strLnkFldNam +"_LNK";
	
	strRqrCarFldNam = CString(strTableName);
	strRqrCarFldNam.Replace("HSE_","");
	strRqrCarFldNam = strRqrCarFldNam + "_RQRCAR";

	strRqrdActnSts = CString(strTableName);
	strRqrdActnSts.Replace("HSE_","");
	strRqrdActnSts = strRqrdActnSts + "_CARSTS";

	strSQL.Format("SELECT %s FROM %s WHERE (%s = %s) AND (%s = 1) AND (%s = 1)",strActnFldNam,strTableName,strLnkFldNam,strKeyFldVal,strRqrCarFldNam,strRqrdActnSts);
	
	//For Audit
	if(strTableName == "HSE_ADTACTVTRQRDACTN"){		
		strSQL.Format("SELECT %s FROM %s WHERE %s IN (SELECT MAINLINK FROM HSE_ADTCHCKLSTPLN WHERE ADTCHCKLSTPLN_LNK IN(SELECT MAINLINK FROM HSE_ADTPLNENTACTVT WHERE ADTPLNENTACTVT_LNK = %s)) AND (ADTACTVTRQRDACTN_RQRCAR = 1) AND (ADTACTVTRQRDACTN_CARSTS = 1)",strActnFldNam,strTableName,strLnkFldNam,strKeyFldVal);
	} 
	strPendingRqrdActns = GetRecordSetData(strSQL).GetBuffer(0);	
	if(strPendingRqrdActns != "") {
		CString strPendingActions = "";
		CString strActnNum=_T("");
		int i = 0; // substring index to extract
		while (AfxExtractSubString(strActnNum, strPendingRqrdActns, i,'\2'))
		{   
			if(strPendingActions != "")
				strPendingActions = strPendingActions + ","; 
			strPendingActions = strPendingActions + strActnNum;
		    i++;
		}
		MessageBox(0, "Please close Required Action(s) with Action No.(s) : '"+strPendingActions+"' before closing record.", "Error", MB_OK | MB_ICONERROR);
		bPendingActionExist = true;
	}	
	return bPendingActionExist;	 
}

/** 
 * function Not Used
 */
void CHSEMSCommonCategory::getRskAnalysisCntrlMsr() {
	CString strRskRankCls = FormGetField("HSE_RSKANLYS","RSKANLYS_RSKCLS"); 
	int nRskRnk	 = atoi(FormGetField("HSE_RSKANLYS","RSKANLYS_RNK1")); 
	int nMainLnk = atoi(FormGetField("HSE_RSKANLYS","MAINLINK"));
	CString strSQL;
	strSQL.Format("EXECUTE getRskAnalysisCntrlMsr '%s',%d,%d",strRskRankCls,nRskRnk,nMainLnk);
	ExecuteSQL(strSQL);
	RefreshScreen("HSE_TGCNTRLMSRS",REFRESH_SCREEN);	
}

void CHSEMSCommonCategory::InsertActionRecordIntoTracingTab(CString strMainTablePart,CString strLinkFieldPart,ACTION act)
{
	CString strScreenTag = m_strScreenTag;
	m_strScreenTag       = getOwnerScreenTag();	
	CString strSourceScreenName = GetScrCptnByTag(66,m_strScreenTag,"");
	CString strUserName = GetUserLogin();
	CString strAction="";	
	CString strLinkFieldNam = "";
	CString strMainTable=CString("HSE_")+strMainTablePart;	
	CString strTracingLnkField=strMainTablePart+CString("TRC")+CString("_LNK");	
	if(strLinkFieldPart.CompareNoCase("MAINLINK")!=0) {
		strLinkFieldNam = strMainTablePart+CString("_")+strLinkFieldPart;
	} else {
		strLinkFieldNam = "MAINLINK";
	}
	//Modification : because this function called from another screen i should switch tag.	
	CString linkFieldVal = FormGetField(strMainTable,strLinkFieldNam);	
	if(strScreenTag == "HSE_TGRjctRsn") {
		linkFieldVal=FormGetField("HSE_RJCTRSN","RJCTRSN_LINKWITHMAIN");	
	}
	switch (act)
	{
	case APPROVE:
		strAction="Approved";
		break;
	case REJECT:
		strAction="Rejected from Confirmation";
		break;
	case REJECTF:
		strAction="Rejected from follow-up";
		break;
	case CLOSED:
		strAction="Closed";
		break;
	case COMPLETE:
		strAction="Completed";
		break;
	case CONFIRM:
		strAction="Confirmed";
		break;
	case CREATED:
		strAction="Created";
		break;
	case ADTRSADTEES:
		strAction = "Auditors and auditees selection";
		break;
	case CHKLSTENTRY:
		strAction = "Audit check list entry";
		break;
	case CHKLSTFDBCK:
		strAction = "Audit checklist feedback";
		break;
	case CARFDBCK:
		strAction = "Corrective action feedback";
		break;
	case CARFLWUPVST:
		strAction = "Closed After Follow-Up Visit";
		break;
	case DELETED:
		strAction = "Deleted";
		break;
	case ACTIVATED:
		strAction = "Activated";
		break;
	default:
		break;
	}	
	CString strSQL;
	strSQL.Format("EXECUTE InsertRecIntoTracingTab '%s','%s','%s','%s','%s','%s'",strMainTablePart,strLinkFieldPart,strAction,strSourceScreenName,strUserName,linkFieldVal);
	ExecuteSQL(strSQL);		
	if(strAction!="Created") {
		RefreshScreen("",REFRESH_SELECTED);
	}	
	//return to old screen tag
	m_strScreenTag=strScreenTag;
}

void CHSEMSCommonCategory::GenerateTXNNo(CString strFormTag)
{
	CString strSQL;
	CString strKeyFldVal = getTXNKeyFldVal(strFormTag);
	if(strFormTag == "" || strKeyFldVal == "") {
		return;
	}
	strSQL.Format("EXECUTE generateNewTXNNum '%s',%s",strFormTag,strKeyFldVal);
	ExecuteSQL(strSQL);			
	RefreshScreen("",REFRESH_SELECTED);		
}

/**
 * Incident Medical Report
 * 
 */
void CHSEMSCommonCategory::completeIncdntMdclRprt(CString strKeyVal) {
	CString strSQL;	
	if(strKeyVal == "") {
		return;
	}
	strSQL.Format("EXECUTE completeIncdntMdclRprt '%s'",strKeyVal);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}

void CHSEMSCommonCategory::closeIncdntMdclRprt(CString strKeyVal) {
	CString strSQL;	
	if(strKeyVal == "") {
		return;
	}
	strSQL.Format("EXECUTE closeIncdntMdclRprt '%s'",strKeyVal);
	ExecuteSQL(strSQL);	
	RefreshScreen("",REFRESH_SELECTED);	
}


/**
 * Get Key field value for HSE Main TXN's (Risk-Potential Hazard-Performance-Accident-Audit-Corrective-
 * Loss Accident - Near Miss - Site Survey - Incdient Flash report) for other TXN's return ""
 */
CString CHSEMSCommonCategory::getTXNKeyFldVal(CString strFormTag) {
	CString strKeyFldVal = "";
	CString strTableNam  = "";
	CString strKeyFldNam = "";
	if(strFormTag == "HSE_TGRSKASSMNTENT")
	{
		strTableNam	 = "HSE_RSKASSMNTENT";
		strKeyFldNam = "RSKASSMNTENT_ASSMNTNO";				
	}			
	else if(strFormTag == "HSE_TGPTNLHZRDENT")
	{
		strTableNam	 = "HSE_PTNLHZRDENT";
		strKeyFldNam = "PTNLHZRDENT_PTNLHZRDNO";				
	}			
	else if(strFormTag == "HSE_TGPERFMSRMNTENT")
	{
		strTableNam	 = "HSE_PERFMSRMNTENT";
		strKeyFldNam = "PERFMSRMNTENT_VSTNO";	
	}			
	else if(strFormTag == "HSE_TGACDNTENT")
	{
		strTableNam	 = "HSE_ACDNTENT";
		strKeyFldNam = "ACDNTENT_ACDNTNUM";				
	}			
	else if(strFormTag == "HSE_TGADTPLNENT")
	{
		strTableNam	 = "HSE_ADTPLNENT";
		strKeyFldNam = "MAINLINK";				
	}			
	else if(strFormTag == "HSE_TGPTWSRGSTRENT")
	{
		strTableNam	 = "HSE_PTWSRGSTRENT";
		strKeyFldNam = "PTWSRGSTRENT_PTWSNUM";					
	}			
	else if(strFormTag == "HSE_TGCRCTVEACCENT")
	{
		strTableNam	 = "HSE_CRCTVEACCENT";
		strKeyFldNam = "CRCTVEACCENT_CARMODELNUM";					
	}			
	else if(strFormTag == "HSE_TGLOSSACDNTENT")
	{
		strTableNam	 = "HSE_LOSSACDNTENT";
		strKeyFldNam = "LOSSACDNTENT_ACTHZRDNO";				
	}			
	else if(strFormTag == "HSE_TGNRSTMISCENT")
	{
		strTableNam	 = "HSE_vwNRSTMISCENT";
		strKeyFldNam = "NRSTMISCENT_NRSTMISCNUM";					
	}	
	else if(strFormTag == "HSE_TGSITSRVYENT")
	{
		strTableNam	 = "HSE_SITSRVYENT";
		strKeyFldNam = "SITSRVYENT_SITSRVYNO";					
	}
	else if(strFormTag == "HSE_TGINCDNTFLSHRPRT")
	{
		strTableNam	 = "HSE_INCDNTFLSHRPRT";
		strKeyFldNam = "MAINLINK";		
	}
	else if(strFormTag == "HSE_TGENVMSRENTRY")
	{
		strTableNam	 = "HSE_ENVMSRENTRY";
		strKeyFldNam = "ENVMSRENTRY_PRMKY";
	}
	if(strTableNam != "" && strKeyFldNam != "") {
		strKeyFldVal = FormGetField(strTableNam,strKeyFldNam);
	}
	return strKeyFldVal;
}

/**
 *	This funtion to update status for all screens 
 */
bool CHSEMSCommonCategory::UpdateStatus(MultiSelectedRows *pMultiSelectedRows,int nStatus,CString tableNamePart,CString keyFldNam)
{
	CString strSQL;
	CString strKeyFieldArray="";
	int RcrdCount = 0;
	RcrdCount = pMultiSelectedRows->lCount;	
	for(int nSel = 0 ;nSel < RcrdCount;nSel++)
	{
		CString strRec= pMultiSelectedRows->pRow[nSel].strFullRecord;    
		CString strKeyField	= GetFieldValue(strRec,keyFldNam);		
		strKeyFieldArray +=strKeyField;
		strKeyFieldArray +=",";
	}
	strKeyFieldArray.TrimRight(",");
	strKeyFieldArray.TrimLeft(",");
	if(strKeyFieldArray.IsEmpty()) {
		return false;
	} else {
		strSQL.Format("EXECUTE updateTXNSts '%s','%s','%s','%s'",strKeyFieldArray,itoa(nStatus),tableNamePart,keyFldNam);
		ExecuteSQL(strSQL);
		return true;
	}
}

/**
 * get department Email by department Code
 */
CString CHSEMSCommonCategory::getDepMail(CString strDepCode) {	
	CString strSQL;
	strSQL.Format("SELECT ISNULL(DEPART_EMAIL,'') FROM CMN_DEPART WHERE DEPART_CODE = '%s'",strDepCode);
	CString strMail= GetRecordSetData(strSQL).GetBuffer(0);	
	return strMail;
}


void CHSEMSCommonCategory::sendGenerateCARNotification(CString strTableName) {
	CString strSQL;	 
	CString strDepMail;
	CString strCARSerial = "";
	CString strSenderName	  = GetUserLogin();			
	CString strDepFld(strTableName);
	strDepFld.Replace("HSE_","");
	strDepFld = strDepFld + "_BYDEP";

	CString strFormTag(strTableName);
	strFormTag.Replace("HSE_","HSE_Tg");
	int nMsgBody = getTXNMsgBody(strTableName);
	if(nMsgBody == ID_MAIL_UNKNOWN)
		return;
	strSQL.Format("SELECT requiredActionMainLink,correctiveActionSrl,DEPART_EMAIL FROM ##generatedCARKeysTable JOIN %s ON ##generatedCARKeysTable.requiredActionMainLink = %s.MAINLINK JOIN CMN_DEPART ON %s = DEPART_CODE",strTableName,strTableName,strDepFld);	
	CPolyDbRecordset *pDaoRecset =new CPolyDbRecordset();
	pDaoRecset->Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if(!pDaoRecset->IsEOF())
	{
		while(!pDaoRecset->IsEOF())
		{
			pDaoRecset->GetFieldValue("DEPART_EMAIL",strDepMail);
			pDaoRecset->GetFieldValue("correctiveActionSrl",strCARSerial);			
			CString strSenderName  = "HSEMS System";	
			CString strMailSubject = "Received CAR , Auto E-mail From HSEMS";						
			EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
			CString strMailBody = eMailMsgHandler->FormatMailBody(nMsgBody,strFormTag);
			try {
				strDepMail="hossam.elmostafa@first-egyptian.com";
				strMailBody="A Test";
				m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strDepMail,strMailBody,"");		
			}
			catch(...) {
				AfxMessageBox("Faild to send Mail");
			}
			pDaoRecset->MoveNext();					
		}		
	}
	pDaoRecset->Close();	
	delete pDaoRecset;
}


void CHSEMSCommonCategory::sendUpdateCARNotification(CString strDepMail,CString strCARSerial,CString strTableName,int nMsgNum) 
{
	/*CString strSenderName	  = GetUserLogin();			
	CString strSubject		  = "Update CAR Notification";	
	CString strMail	= "Please be notified that CAR with serial '"+strCARSerial+"' Updated.";
	CString strAttachFilePath	  = "Test.txt";
	bool bSendMail  = m_pCategory->GetHSEObj()->sendMail(strSenderName,strSubject,strDepMail,strMail,strAttachFilePath);*/
		
	CString strSenderName  = "HSEMS System";	
	CString strMailSubject = "Updated CAR , Auto E-mail From HSEMS";			
	EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
	CString strMailBody = eMailMsgHandler->FormatMailBody(nMsgNum,strTableName);
	m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strDepMail,strMailBody,"");
}

void CHSEMSCommonCategory::sendCARNotAccptdNotification(CString strDepMail,CString strCARSerial) 
{
	/*CString strSenderName	  = GetUserLogin();			
	CString strSubject		  = "CAR Not accepted";	
	CString strMail	= "Please be notified that CAR with serial '"+strCARSerial+"' is not accepted.";
	CString strAttachFilePath	  = "Test.txt";
	bool bSendMail  = m_pCategory->GetHSEObj()->sendMail(strSenderName,strSubject,strDepMail,strMail,strAttachFilePath);*/	

	CString strSenderName  = "HSEMS System";	
	CString strMailSubject = "CAR Not Accepted, Auto E-mail From HSEMS ";			
	EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
	CString strMailBody = eMailMsgHandler->FormatMailBody(ID_MAIL_CAR_NOT_ACCPTD,"");
	m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strDepMail,strMailBody,"");
}

void CHSEMSCommonCategory::sendwrkNotAccptdNotification(CString strDepMail,CString strCARSerial,CString strTableName,int nMsgNum) 
{
	/*CString strSenderName	  = GetUserLogin();			
	CString strSubject		  = "Work for CAR :"+strCARSerial;	
	CString strMail	= "Please be notified that work for CAR with serial '"+strCARSerial+"' is not accepted.";
	CString strAttachFilePath	  = "Test.txt";
	bool bSendMail  = m_pCategory->GetHSEObj()->sendMail(strSenderName,strSubject,strDepMail,strMail,strAttachFilePath);*/

	CString strSenderName  = "HSEMS System";	
	CString strMailSubject = "CAR Work Not Accepted, Auto E-mail From HSEMS";			
	EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
	CString strMailBody = eMailMsgHandler->FormatMailBody(nMsgNum,strTableName);
	m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strDepMail,strMailBody,"");
}

void CHSEMSCommonCategory::closeCARNotification(CString strDepMail,CString strCARSerial) 
{
	/*CString strSenderName	  = GetUserLogin();			
	CString strSubject		  = "CAR closed";	
	CString strMail	= "Please be notified that CAR with serial '"+strCARSerial+"' is closed.";
	CString strAttachFilePath	  = "";
	bool bSendMail  = m_pCategory->GetHSEObj()->sendMail(strSenderName,strSubject,strDepMail,strMail,strAttachFilePath);*/
	
	CString strSenderName  = "HSEMS System";	
	CString strMailSubject = "CAR Closing, Auto E-mail From HSEMS";			
	EMailMsgHandler *eMailMsgHandler = new EMailMsgHandler(m_pCategory);
	CString strMailBody = eMailMsgHandler->FormatMailBody(ID_MAIL_CAR_CLOSE,"");
	m_pCategory->GetHSEObj()->sendMail(strSenderName,strMailSubject,strDepMail,strMailBody,"");


}

CString CHSEMSCommonCategory::getHSEDepMail(CString strCmpnyNam){
	CString strSQL;
	strSQL.Format("SELECT ISNULL(DEPART_EMAIL,'') FROM CMN_DEPART WHERE DEPART_CODE = (SELECT HSEPlc_OwnrDprt FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s')",strCmpnyNam);
	CString strMail= GetRecordSetData(strSQL).GetBuffer(0);	
	return strMail;
}


CString CHSEMSCommonCategory::IsMultiMtrlRlsAllowed(CString strCmpnyNam) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(HSEPlc_AlwMltRcrdMtrlRlsTb,'0') FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strMtrlRlsAllowed = GetRecordSetData(strSQL).GetBuffer(0);	
	return strMtrlRlsAllowed;
}

CString CHSEMSCommonCategory::IsPhysicaldeleteAllowed(CString strCmpnyNam){
	CString strSQL;
	strSQL.Format("SELECT ISNULL(HSEPlc_AlwPhysclDlt,'0') FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'",strCmpnyNam);
	CString strPhysicaldeleteAllowed = GetRecordSetData(strSQL).GetBuffer(0);	
	return strPhysicaldeleteAllowed;
}

CString CHSEMSCommonCategory::getAuditeeMail(CString strAdtGrp,CString strAdtCOD){
	CString strSQL;
	strSQL.Format("SELECT ISNULL(ADTGRPADTE_EMAIL,'') FROM HSE_ADTGRPADTE WHERE ADTGRPADTE_COD = '%s' AND DETAILLINK = '%s'",strAdtCOD,strAdtGrp);
	CString strAuditeeMail = GetRecordSetData(strSQL).GetBuffer(0);	
	return strAuditeeMail;
}

int CHSEMSCommonCategory::getTXNMsgBody(CString strTableName) {
	int nRetVal = ID_MAIL_UNKNOWN;
	strTableName.MakeUpper();
	if(strTableName == "HSE_PERFMSRMNTENTRQRDACTN") {
		nRetVal = ID_MAIL_Prfrmnc_CAR;	
	} else if (strTableName == "HSE_SITSRVYENTRQRDACTN") {
		nRetVal = ID_MAIL_SiteSurvey_CAR;
	} else if (strTableName == "HSE_NRSTMISCENTRQDACTN") {
		nRetVal = ID_MAIL_Observation_CAR;
	} else if (strTableName == "HSE_ACDNTENTRECMNDACC") {
		nRetVal = ID_MAIL_Incident_CAR;
	} else if (strTableName == "HSE_ADTACTVTRQRDACTN") {
		nRetVal = ID_MAIL_Audit_CAR;
	}
	return nRetVal;
}

int CHSEMSCommonCategory::getUpdateCARMsgNum(CString strTableName) {
	int nRetVal = ID_MAIL_UNKNOWN;
	strTableName.MakeUpper();
	if(strTableName == "HSE_PERFMSRMNTENTRQRDACTN") {
		nRetVal = ID_MAIL_Prfrmnc_UPDATE_CAR;	
	} else if (strTableName == "HSE_SITSRVYENTRQRDACTN") {
		nRetVal = ID_MAIL_SiteSurvey_UPDATE_CAR;
	} else if (strTableName == "HSE_NRSTMISCENTRQDACTN") {
		nRetVal = ID_MAIL_Observation_UPDATE_CAR;
	} else if (strTableName == "HSE_ACDNTENTRECMNDACC") {
		nRetVal = ID_MAIL_Incident_UPDATE_CAR;
	} else if (strTableName == "HSE_ADTACTVTRQRDACTN") {
		nRetVal = ID_MAIL_Audit_UPDATE_CAR;
	}
	return nRetVal;
}

int CHSEMSCommonCategory::getCARWrkNotAcptdMsgNum(CString strTableName) {
	int nRetVal = ID_MAIL_UNKNOWN;
	strTableName.MakeUpper();
	if(strTableName == "HSE_PERFMSRMNTENTRQRDACTN") {
		nRetVal = ID_MAIL_Prfrmnc_CAR_WRK_NOT_ACCPTD;	
	} else if (strTableName == "HSE_SITSRVYENTRQRDACTN") {
		nRetVal = ID_MAIL_SiteSurvey_CAR_WRK_NOT_ACCPTD;
	} else if (strTableName == "HSE_NRSTMISCENTRQDACTN") {
		nRetVal = ID_MAIL_Observation_CAR_WRK_NOT_ACCPTD;
	} else if (strTableName == "HSE_ACDNTENTRECMNDACC") {
		nRetVal = ID_MAIL_Incident_CAR_WRK_NOT_ACCPTD;
	} else if (strTableName == "HSE_ADTACTVTRQRDACTN") {
		nRetVal = ID_MAIL_Audit_CAR_WRK_NOT_ACCPTD;
	}
	return nRetVal;
}

/**
 *	generate new CAR 
 * 
 *
 */
void CHSEMSCommonCategory::generateCAR(CString strFormTag,CString strSubFormTag)
{
	CString strTabTag = CString(strSubFormTag);
	_bstr_t bstrTableName;
	CString strTableName = "";
	if(strTabTag != "") //to handle Required Actions Audit header screen			
		strTableName = GetTrueTableName (66,strFormTag.AllocSysString(),strTabTag.AllocSysString());
	else {
		strTableName = strFormTag;
		strTableName.Replace("HSE_TG","HSE_");
		strTabTag = strFormTag;
	}		
	CString strFieldName(strTableName);
	strFieldName.Replace("HSE_","");
	strFieldName = strFieldName + "_LNK";
	CString strTXNKey = FormGetField(strTableName,strFieldName);
	generateCARSP(strTXNKey,strTabTag);	
	sendGenerateCARNotification(strTableName);
	if(strSubFormTag != "")
		RefreshScreen(strSubFormTag,REFRESH_SELECTED);
	else 
		RefreshScreen("",REFRESH_SELECTED);				//to handle Audit,Site Survey cases
}


/**
 *	Add new version
 *  This function increase version number by one when custom button add new version pressed in any screen
 */
void CHSEMSCommonCategory::addNewVersion(CString strTableName,CString strFormTag,CString strSubFormTag,CString strFieldMainPartName){
	CString strVrsnNoName=CString(strFieldMainPartName)+CString("_VRSNNO");
	CString strVrsnNoValue=FormGetField(strTableName,strVrsnNoName);

	CString strDescName=CString(strFieldMainPartName)+CString("_DESC");
	CString strDescValue=FormGetField(strTableName,strDescName);

	CString strTrnsNamDescName=CString(strFieldMainPartName)+CString("_TRNSNAMDESC");
	CString strTrnsNamDescValue=FormGetField(strTableName,strTrnsNamDescName);
	
	CString strClsName=CString(strFieldMainPartName)+CString("_CLS");
	CString strClsValue=FormGetField(strTableName,strClsName);
	
	CString strSubClsName=CString(strFieldMainPartName)+CString("_SUBCLS");
	CString strSubClsValue=FormGetField(strTableName,strSubClsName);

	CString strAtchDatName=CString(strFieldMainPartName)+CString("_ATCHDAT");
	CString strAtchDatValue=FormGetField(strTableName,strAtchDatName);

	CString strAtchUsrName=CString(strFieldMainPartName)+CString("_ATCHUSR");
	CString strAtchUsrValue=FormGetField(strTableName,strAtchUsrName);

	CString strCmntName=CString(strFieldMainPartName)+CString("_CMNT");
	CString strCmntValue=FormGetField(strTableName,strCmntName);

	CString strLnkName=CString(strFieldMainPartName)+CString("_LNK");
	CString strLnkValue=FormGetField(strTableName,strLnkName);

	CString strAtchRcrdDatName=CString(strFieldMainPartName)+CString("_ATCHRCRDDAT");
	CString strAtchRcrdDatValue=FormGetField(strTableName,strAtchRcrdDatName);	
	
	int nVrsnNoValue=atoi(strVrsnNoValue);
	nVrsnNoValue++;
	CString strNewVrsnNoValue=itoa(nVrsnNoValue);
	DoToolBarAction(TOOLBAR_NEW,strFormTag,strSubFormTag);
			
	FormSetField(strTableName,strVrsnNoName,strNewVrsnNoValue);
	FormSetField(strTableName,strDescName,strDescValue);
	FormSetField(strTableName,strTrnsNamDescName,strTrnsNamDescValue);
	FormSetField(strTableName,strClsName,strClsValue);
	FormSetField(strTableName,strSubClsName,strSubClsValue);
	FormSetField(strTableName,strAtchDatName,strAtchDatValue);
	FormSetField(strTableName,strAtchUsrName,strAtchUsrValue);
	FormSetField(strTableName,strCmntName,strCmntValue);
	FormSetField(strTableName,strLnkName,strLnkValue);
	FormSetField(strTableName,strAtchRcrdDatName,strAtchRcrdDatValue);
}

/**
 *	Update CAR
 */
void CHSEMSCommonCategory::updateCAR(CString strFormTag,CString strSubFormTag){
	CString strTabTag = CString(strSubFormTag);
	_bstr_t bstrTableName;
	CString strTableName = "";
	if(strTabTag != "") //to handle Required Actions Audit header screen			
		strTableName = GetTrueTableName (66,strFormTag.AllocSysString(),strTabTag.AllocSysString());
	else {				
		strTableName = strFormTag;
		strTableName.Replace("HSE_TG","HSE_");
		strTabTag = strFormTag;
	}		
	CString strFieldName(strTableName);
	strFieldName.Replace("HSE_","");
	strFieldName = strFieldName + "_LNK";
	CString strTXNKey = FormGetField(strTableName,strFieldName);
	CString strCARKey = FormGetField(strTableName,"CAR_KEY");
	
	bool bUpdateCAR = false;
	bUpdateCAR = updateCARSP(strTXNKey,strTabTag,strCARKey);	
	if(bUpdateCAR) {
		//get department e-mail
		CString strDepCode(strTableName);
		strDepCode.Replace("HSE_","");
		strDepCode = strDepCode + "_BYDEP";
		CString strDepCodeVal = FormGetField(strTableName,strDepCode);
		CString strDepMail    = getDepMail(strDepCodeVal);

		CString strCARSerialFldNam(strTableName);
		strCARSerialFldNam.Replace("HSE_","");
		strCARSerialFldNam = strCARSerialFldNam + "_CARNo";			
		CString strCARSerial = FormGetField(strTableName,strCARSerialFldNam);
		//getMsgNumber
		int nMsgNum = getUpdateCARMsgNum(strTableName);
		if(nMsgNum != ID_MAIL_UNKNOWN) {
			sendUpdateCARNotification(strDepMail,strCARSerial,strTableName,nMsgNum);	
		}
		if(strSubFormTag != "")
			RefreshScreen(strSubFormTag,REFRESH_SELECTED);
		else 
			RefreshScreen("",REFRESH_SELECTED);				//to handle Audit,Site Survey cases
	}
}

/**
 *	Cancel CAR
 */
void CHSEMSCommonCategory::cancelCAR(CString strFormTag,CString strSubFormTag){
	CString strTabTag = CString(strSubFormTag);		
	_bstr_t bstrTableName;
	CString strTableName = "";
	if(strTabTag != "") //to handle Required Actions Audit header screen			
		strTableName = GetTrueTableName (66,strFormTag.AllocSysString(),strTabTag.AllocSysString());
	else {
		strTableName = strFormTag;
		strTableName.Replace("HSE_TG","HSE_");
		strTabTag = strFormTag;
	}	
	CString strTXNKey = FormGetField(strTableName,"MAINLINK");
	CString strCARKey = FormGetField(strTableName,"CAR_KEY");
	cancelCARSP(strTXNKey,strTabTag,strCARKey);
	if(strSubFormTag != "")
		RefreshScreen(strSubFormTag,REFRESH_SELECTED);
	else 
		RefreshScreen("",REFRESH_SELECTED);				//to handle Audit,Site Survey cases
}

/**
 *	open job hazard consequnce screen
 */
void CHSEMSCommonCategory::opnJobHzrdCnsqncs() {
	CString strDefValues,
			strSQL,
			strKeyField;			
	strKeyField = FormGetField("HSE_JOBSTPHZRD","MAINLINK");
	strDefValues.Format("CNSQNCS_LNKWTHDTL|%s",strKeyField);					
	strSQL.Format("SELECT * FROM HSE_CNSQNCS WHERE ( CNSQNCS_LNKWTHDTL = %s) ",strKeyField);	
	//Screen name changed from Consequences to Effects
	//ShowScreen(0,"HSE_TgCnsqncs","Consequences",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
	ShowScreen(0,"HSE_TgCnsqncs","Effects",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
}


/**
 *	Open Audit Group check list
 */
void CHSEMSCommonCategory::opnAdtGrpAdtChkLst(){
	CString strDefValues,
			strSQL,
			strKeyField;			
	strKeyField = FormGetField("HSE_ADTGRPADTE","MAINLINK");
	strDefValues.Format("ADTCHCKLST_LNKWTHDTL|%s",strKeyField);					
	strSQL.Format("SELECT * FROM HSE_ADTCHCKLST WHERE ( ADTCHCKLST_LNKWTHDTL = %s) ",strKeyField);		
	ShowScreen(0,"HSE_TgAdtChcklst","Audit Checklist",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
}

/**
 *	Accept Work (from Required Action tab\Screen
 */
void CHSEMSCommonCategory::accptWrk(CString strTableName,CString strFormTag,CString strSubFormTag){
	//to handle Audit
	if(strSubFormTag == "") 
		strSubFormTag = strFormTag;
	CString strTXN = FormGetField(strTableName,"MAINLINK");
	CString strSQL = "EXECUTE acceptRqrdActnCAR '"+strSubFormTag+"',"+strTXN;
	ExecuteSQL(strSQL);
	if(strSubFormTag != strFormTag)
		RefreshScreen(strSubFormTag,REFRESH_SELECTED);
	else 
		RefreshScreen("",REFRESH_SELECTED);				//to handle Audit,Site Survey cases
}


/**
 * reject work (from Required Action tab\screen)
 * work Not Accepted
 */
void CHSEMSCommonCategory::rjctWrk(CString strTableName,CString strFormTag,CString strSubFormTag){
	//to handle Audit
	if(strSubFormTag == "") 
		strSubFormTag = strFormTag;

	CString strTXN = FormGetField(strTableName,"MAINLINK");
	CString strRltdCARKey = FormGetField(strTableName,"CAR_KEY");
	CString strSQL = "EXECUTE rjctRqrdActnCAR '"+strSubFormTag+"',"+strTXN+","+strRltdCARKey;
	long lRetVal   = ExecuteSQL(strSQL);
	if(lRetVal != 0) {		
		//get department mail
		CString strDepCode(strTableName);
		strDepCode.Replace("HSE_","");
		strDepCode = strDepCode + "_BYDEP";
		CString strDepCodeVal = FormGetField(strTableName,strDepCode);
		CString strDepMail    = getDepMail(strDepCodeVal);			

		CString strCARSerialFldNam(strTableName);
		strCARSerialFldNam.Replace("HSE_","");
		strCARSerialFldNam   = strCARSerialFldNam + "_CARNo";			
		CString strCARSerial = FormGetField(strTableName,strCARSerialFldNam);
		int nMsgNum = getCARWrkNotAcptdMsgNum(strTableName);
		if(nMsgNum != ID_MAIL_UNKNOWN) {
			sendwrkNotAccptdNotification(strDepMail,strCARSerial,strTableName,nMsgNum);
		}
		if(strSubFormTag != strFormTag)
			RefreshScreen(strSubFormTag,REFRESH_SELECTED);
		else 
			RefreshScreen("",REFRESH_SELECTED);				//to handle Audit,Site Survey cases
	}
}

CString CHSEMSCommonCategory::GetKeyFieldName(CString strTableName) 
{
	CString keyFldNam = "";
	if(strTableName.Find("RSKASS")!= -1)
		keyFldNam = "RSKASSMNTENT_ASSMNTNO";		
	else if(strTableName.Find("PERFMSRMNT")!= -1)
		keyFldNam = "PERFMSRMNTENT_VSTNO";		
	else if(strTableName.Find("ACDNT")!= -1)
		keyFldNam = "ACDNTENT_ACDNTNUM";
	else if(strTableName.Find("ADTPLN")!= -1 && strTableName.Find("HSE_ADTPLNENTAUDITORS")== -1 &&strTableName.Find("HSE_ADTPLNENTAUDITEES") == -1 )
		keyFldNam = "MAINLINK";	
	else if(strTableName.Find("PTW")!= -1)
		keyFldNam = "PTWSRGSTRENT_PTWSNUM";	
	else if(strTableName.Find("CRCTV")!= -1  || strTableName.Find("CARFLWUP")!= -1 )
		keyFldNam = "CRCTVEACCENT_CARMODELNUM";		
	else if(strTableName.Find("NRSTMISC")!= -1)
		keyFldNam = "NRSTMISCENT_NRSTMISCNUM";	
	else if(strTableName.Find("SITSRVY")!= -1)
		keyFldNam = "SITSRVYENT_SITSRVYNO";	
	return keyFldNam;
}

void CHSEMSCommonCategory::insertIntoTracing(CString strFormTag,ACTION action) 
{
	//101_30020_1
	//Risk assesment
	if(strFormTag == "HSE_TGRSKASSMNTENT" || strFormTag == "HSE_TGRSKASSMNTINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("RSKASSMNTENT","ASSMNTNO",action);
	}
	//201_32020_1
	//Potential Hazard Entry
	else if(strFormTag == "HSE_TGPTNLHZRDENT")
	{
		setOwnerScreenTag("HSE_TGPTNLHZRDENT");
		InsertActionRecordIntoTracingTab("PTNLHZRDENT","PTNLHZRDNO",action);
	}
	//301_34020_1
	//Performance
	else if(strFormTag == "HSE_TGPERFMSRMNTENT" || strFormTag == "HSE_TGPERFMSRMNTINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("PERFMSRMNTENT","VSTNO",action);
	}
	//404_40020_1
	//Accident
	else if(strFormTag == "HSE_TGACDNTENT" || strFormTag == "HSE_TGACDNTINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("ACDNTENT","ACDNTNUM",action);
	}
	//601_46020_1
	//Auditing
	else if(strFormTag == "HSE_TGADTPLNENT" || strFormTag == "HSE_TGADTPLNINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("ADTPLNENT","MAINLINK",action);
	}
	//501_44020_1
	//PTW
	else if(strFormTag == "HSE_TGPTWSRGSTRENT" || strFormTag == "HSE_TGPTWSRGSTRINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("PTWSRGSTRENT","PTWSNUM",action);
	}
	//701_43020_1
	//Corrective action
	else if(strFormTag == "HSE_TGCRCTVEACCENT" || strFormTag == "HSE_TGCRCTVEACCINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("CRCTVEACCENT","CARMODELNUM",action);
	}
	//Loss accident
	else if(strFormTag == "HSE_TGLOSSACDNTENT")
	{
		setOwnerScreenTag("HSE_TGLOSSACDNTENT");
		InsertActionRecordIntoTracingTab("LOSSACDNTENT","ACTHZRDNO",action);
	}
	//Near miss Entry
	else if(strFormTag == "HSE_TGNRSTMISCENT" || strFormTag == "HSE_TGNRSTMISCINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("NRSTMISCENT","NRSTMISCNUM",action);
	}
	else if(strFormTag == "HSE_TGVCLACDNTENT")
	{
		setOwnerScreenTag("HSE_TGVCLACDNTENT");
		InsertActionRecordIntoTracingTab("VCLACDNTENT","VCLACDNTNO",action);
	} 
	else if(strFormTag == "HSE_TGSITSRVYENT" || strFormTag == "HSE_TGSITSRVYINQ")
	{
		setOwnerScreenTag(strFormTag);
		InsertActionRecordIntoTracingTab("SITSRVYENT","SITSRVYNO",action);
	}
}


void CHSEMSCommonCategory::OpenMsonRstLog(CString ScreenTag,int ScreenMode)
{
	CString strSQL ;		
	CString strMainLinkFeild = FormGetField("HSE_ACDNTENTOURVHCLS","MAINLINK");	
	////////////////////////////////////////////////////////////////////////////
	//Bugs-10-2021.56
	if(strMainLinkFeild!="")
	{
	////////////////////////////////////////////////////////////////////////////
		CString strDefValues;
		strDefValues.Format("LINK|%s",strMainLinkFeild);	
		strSQL.Format("SELECT * FROM HSE_MSNRSLOG WHERE LINK = %s",strMainLinkFeild);
		ShowScreen(0,"HSE_TgMsnRsLog","Mission Rest Log",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);	
	}
}

/**
 * a.ali,2-11-2019
 * Moved from Accident Category to be used in Incident Medical Report Screen
 * 
 * Update :8/10/2020
 * HSE BGC-EZZ Requirements_2892020
 * This screen will be for view only
 *
 */
void CHSEMSCommonCategory::OpenBodyParts(CString strFormTag)
{	
	m_strScreenTag = "HSE_TGHMNBODYPART";		
	
	CString strSQL="",
			strDefValues,
			strTableNam,
			strKeyFldNam;	
	bool bLocked = true;	
	if(strFormTag == "HSE_TGINCDNTMDCLRPRT") {
		strTableNam  = "HSE_INCDNTMDCLRPRTINJRDPRSNS";
		strKeyFldNam = "INCDNTMDCLRPRTINJRDPRSNS_LNRNUM";
		bLocked = false;
	} else {
		strTableNam  = "HSE_ACDNTENTINJRDPRSN";
		strKeyFldNam = "ACDNTENTINJRDPRSN_LNRNUM";
	}
	CString strKeyField=FormGetField(strTableNam,strKeyFldNam);
	//strSQL.Format("SELECT * FROM HSE_HMNBODYPART WHERE (HMNBODYPART_LNK = '%s' AND HMNBODYPART_SRC = '%s')",strKeyField,strFormTag);
	strSQL.Format("SELECT * FROM HSE_HMNBODYPART WHERE (HMNBODYPART_LNK = '%s')",strKeyField);
	//if(strFormTag == "HSE_TGACDNTENT") {
	//	bLocked = false;
	//}
	//strDefValues.Format("HMNBODYPART_LNK|%s|HMNBODYPART_SRC|%s",strKeyField,strFormTag);
	strDefValues.Format("HMNBODYPART_LNK|%s",strKeyField);
	ShowScreen(0,"HSE_TGHMNBODYPART","Human body parts",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}

/**
 *  Update : Moved from AccidentCategory to handle Incident Medical Report Screen
 *
 *	Open Injury Type pop-up screen
 *	@param strFormTag if screen opened from personnel accident entry screen , open it for edit else open it locked
 *
 * Update :8/10/2020
 * HSE BGC-EZZ Requirements_2892020
 * This screen will be for view only
 */
void CHSEMSCommonCategory::openInjuryType(CString strFormTag)
{	
	m_strScreenTag = "HSE_TGINJRYTP";	
	CString strSQL="",
		strDefValues,
		strTableNam,
		strKeyFldNam;
	bool bLocked = true;	
	if(strFormTag == "HSE_TGINCDNTMDCLRPRT") {
		strTableNam  = "HSE_INCDNTMDCLRPRTINJRDPRSNS";
		strKeyFldNam = "INCDNTMDCLRPRTINJRDPRSNS_LNRNUM";
		bLocked = false;
	} else {
		strTableNam  = "HSE_ACDNTENTINJRDPRSN";
		strKeyFldNam = "ACDNTENTINJRDPRSN_LNRNUM";
		strFormTag = "HSE_TGACDNTENT";
	}
	CString strKeyField=FormGetField(strTableNam,strKeyFldNam);						
	//strSQL.Format("SELECT * FROM HSE_INJRYTP WHERE (INJRYTP_LNK = '%s' AND INJRYTP_SRC = '%s')",strKeyField,strFormTag);	
	strSQL.Format("SELECT * FROM HSE_INJRYTP WHERE (INJRYTP_LNK = '%s')",strKeyField);	
	//if(strFormTag =="HSE_TGACDNTENT") {
	//	bLocked = false;
	//}
	//strDefValues.Format("INJRYTP_LNK|%s|INJRYTP_SRC|%s",strKeyField,strFormTag);
	strDefValues.Format("INJRYTP_LNK|%s",strKeyField);
	ShowScreen(0,"HSE_TGINJRYTP","Injury Type",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
}


CString CHSEMSCommonCategory::getMsrArLineNo(CString strMsrNo) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(ENVMNTRACTVTSPLTNTMSRAREA_LNNO)+1,1) AS MnfstRrmNo FROM HSE_ENVMNTRACTVTSPLTNTMSRAREA WHERE MAINLINK = %s",strMsrNo);
	CString strMsrArLineNo = GetRecordSetData(strSQL).GetBuffer(0);	
	return strMsrArLineNo;	
}

/**
 *	get Next serial 
 *  
 **/
CString CHSEMSCommonCategory::getNxtSrl(CString strSerialFldNam,CString strTblNam,CString strTabLinkFldNam,CString strHeaderFldVal,CString strWhereCluase){
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(%s)+1,1) AS New_Serial FROM %s ",strSerialFldNam,strTblNam);
	if(strTabLinkFldNam != "" && strHeaderFldVal != "") {
		CString strWhere;
		if(strWhereCluase=="")
			strWhere.Format("WHERE %s = '%s'",strTabLinkFldNam,strHeaderFldVal);
		else
			strWhere.Format("WHERE %s and %s = '%s'",strWhereCluase,strTabLinkFldNam,strHeaderFldVal);
		strSQL = strSQL + strWhere;
	}
	else if(strWhereCluase!="")
	{
		CString strWhere;
		strWhere.Format("WHERE %s",strWhereCluase);
		strSQL = strSQL + strWhere;
	}
	CString strNewSerial = GetRecordSetData(strSQL).GetBuffer(0);	
	return strNewSerial;
}

/**
 * This function comes from CMS by Eng Hossam
 * function name was : UpdateRecordStatusAndInsertIntoTracingTab , I changed it to OpenReasonsScr and make function only Open Reason screen 
 * and return true if the user enter reason and close the screen,then it return true and I complete record cycle.
 * Old function signature
 * bool CHSEMSCommonCategory::UpdateRecordStatusAndInsertIntoTracingTab(CString strNewState,CString strStateID,CString strReasonCaption,CString strActionReasonGroup)
 * 
 */
bool CHSEMSCommonCategory::OpenReasonsScr(CString strReasonCaption,CString strActionReasonGroup,bool bEdit)
{
	InitRandomReasonsParam();
	m_strActionReasonGroup=strActionReasonGroup;
	m_strReasonCaption=strReasonCaption;
	CString strCriteria;
	strCriteria.Format("WHERE Rsns_RfScrn='%s' AND Rsns_RfRec='%s' ",m_FSATableName,m_strInitial);
	CString strDefValues="";
	m_bEnableResults=bEdit;
	CCommonCategoryWrapper::ShowScreen(0,"CMS_Rsns",strReasonCaption,NORMAL_MODE,true,strCriteria,"",strDefValues,"",false,(IReasonCaller*)this);
	//After setting reason into the screen	
	int ReasonsCount;
	CString strGetCount;
	strGetCount.Format("SELECT COUNT(0) as CNT FROM CMS_RSNS WHERE RSNS_RFREC='%s'",m_strFinal);
	CPolyDbRecordset PolyDbRecordset;
	PolyDbRecordset.Open(AFX_DAO_USE_DEFAULT_TYPE,strGetCount);
	if(!PolyDbRecordset.IsEOF())
	{
		PolyDbRecordset.GetFieldValue("CNT",ReasonsCount);
		if(ReasonsCount>0)
		{
			//return CompleteUpdateRecordStatusAndInsertIntoTracingTab(strNewState,strStateID,m_strFinal);
			return true;
		}
	}
	return false;
}

bool CHSEMSCommonCategory::CompleteUpdateRecordStatusAndInsertIntoTracingTab(CString strNewState,CString strStateID,CString m_strFinal)
{
	if(m_strPrefix=="" ||  m_strStatusFieldName=="")
		return false;
	
	long lRecordsTracing=0;
	CString strMainLink=FormGetField(m_FSATableName,m_strMainLink);
	CString strTrcng_Sttus="";
	short curFormType=m_pHSEMSPolyDb->GetFormType();
	if(strStateID!="")
	{
		if(curFormType==BROWSABLE)
		{
			CString UpdateStatus="UPDATE " + m_strAppPrefix + "_" + m_strPrefix +" SET " + m_strStatusFieldName + " = " + strStateID + " WHERE " + m_strMainLink + " = '" + strMainLink +"'";
			try
			{
				lRecordsTracing=ExecuteSQL(UpdateStatus);
			}
			catch(...)
			{
			}
		}
		else
		{
			m_pHSEMSPolyDb->FormSetField(m_FSATableName,m_strStatusFieldName,strStateID,-1);
		}
		strTrcng_Sttus=strStateID;
	}
	//• Insert record into (tracing) tab, with (@T = ”Cancelled”) & linked to its “cancel” reasons.  Refer to (CFT tracing) tab.
	CString strActionDscr=strNewState;
	CString strScreen=m_strScreen;
	CString strUsrName= FormGetField(m_FSATableName,m_strPrefix+"_LstMdfyBy");
	if(strTrcng_Sttus=="")
		strTrcng_Sttus=FormGetField(m_FSATableName,m_strStatusFieldName);
	CString strInsertTracing;
	strInsertTracing.Format("INSERT INTO %s_%s_Trcng (%s_Trcng_ActnDT,%s_Trcng_ActnDscrptn,%s_Trcng_SurcScrn, \
		%s_Trcng_UsrNm,%s_Trcng_CFTSttus,MainLink,NEWRECORD) values (SYSDATETIME(),'%s','%s','%s','%s','%s','Y')", \
		m_strAppPrefix,m_strPrefix,m_strPrefix,m_strPrefix,m_strPrefix,m_strPrefix,m_strPrefix,strActionDscr,strScreen,strUsrName,strTrcng_Sttus,strMainLink);
	try
	{
		//lRecordsTracing=ExecuteSQL(strInsertTracing);
	}
	catch(...)
	{
	}

	long lRecordsReasons=0;
	CString strUpdateReasons;
	strUpdateReasons.Format("UPDATE CMS_Rsns SET RfRec=(select top(1) PrmKy from %s where NewRecord='Y' AND %s_Trcng_ActnDscrptn='%s') where Rsns_RfRec='%s'",m_TracingTabTag,m_strPrefix,strNewState,m_strFinal);
	try
	{
		//lRecordsReasons=ExecuteSQL(strUpdateReasons);
	}
	catch(...)
	{
	}


	CString strUpdateTrace;
	strUpdateTrace.Format("UPDATE %s set NewRecord='N' where NewRecord='Y' AND %s_Trcng_ActnDscrptn='%s'",m_TracingTabTag,m_strPrefix,strNewState);
	try
	{
		//ExecuteSQL(strUpdateTrace);
	}
	catch(...)
	{
	}
#define BUTTON_INDEX_SAVE		2
	if(lRecordsTracing!=0)
	{			
		if(curFormType==BROWSABLE)
			m_pHSEMSPolyDb->FormMakeRefresh(-1);
		else
			m_pHSEMSPolyDb->FormDoAction(-1, BUTTON_INDEX_SAVE);		
	}

	return lRecordsTracing!=0;
	
}

///////////////////////////////////////////////
//Implement IReasonCaller 
void  CHSEMSCommonCategory::getParameters(CString &strOwnerScreenTag,CString &strCallerIdInitial,CString &strCallerIdFinal,CString &strPrmKey,bool &bEnableResults,CString &strReasonsFilter,CString &strRequiredCaption)
{
	strOwnerScreenTag=m_FSATableName;
	if(m_strInitial=="")
		InitRandomReasonsParam();
	strCallerIdInitial=m_strInitial;
	strCallerIdFinal=m_strFinal;
	if(m_bEnableResults==false) //Comming from View_Reasons button
		strPrmKey=FormGetField(m_TracingTabTag,m_TracingTabPrimKey);
	bEnableResults=m_bEnableResults;
	strReasonsFilter.Format("ActnRsn_RsnGrup='%s'",m_strActionReasonGroup);
	if(m_strReasonCaption!="")
		strRequiredCaption=m_strReasonCaption;
	else
		strRequiredCaption="Reasons";
}
//Implement IReasonCaller 
///////////////////////////////////////////////

void CHSEMSCommonCategory::InitRandomReasonsParam()
{
	m_strInitial=GenerateRandomString(10,18);
	m_strFinal=GenerateRandomString(200,18);
}

CString CHSEMSCommonCategory::GenerateRandomString(int RndSeed,int nLength)
{
	CString strRetVal;
	srand (time(NULL)+RndSeed);
	for (int i=0 ;i<nLength;i++)
	{
		int nRand=rand() % 25 ;
		char chr='A'+nRand;
		strRetVal=strRetVal+chr;
	}
	return strRetVal;
}


long CHSEMSCommonCategory::ShowScreenV2(CString strTableName,int nScreenId,CString strCriteria,void* msg)
{
	m_pCategory->GetHSEObj()->SetCurrentMenuItemID(nScreenId);
	m_pCategory->SetMenuID(nScreenId);
	m_nScreenId=nScreenId;
	CString strScreenTag="";
#define CMS_REASONS					1111
#define CMS_ACTOIN_REASON	1112
	switch (m_nScreenId)
	{
	case CMS_REASONS:
		{
			if(msg)
				p=msg;
			else
				msg=(IReasonCaller *)p;
			CReasonsCategory* pReasonsCategory;
			pReasonsCategory=new CReasonsCategory(m_pCategory,(IReasonCaller *)msg);
			pReasonsCategory->ShowScreen("CMS_Rsns",strCriteria);
		}
		break;
		case CMS_ACTOIN_REASON:
			strScreenTag="CMS_ActnRsn";
			break;

	}
	if(strTableName!="" && strScreenTag!="")
	{
		strTableName.MakeUpper();
		strScreenTag.MakeUpper();
		if(strTableName!=strScreenTag)
		{
			strScreenTag="";
			m_nScreenId=-1;
		}
	}
	if (strTableName!="" && strScreenTag=="" && m_nScreenId<=0) 
	{
		if (strTableName!=strScreenTag) 
			strScreenTag=strTableName;
		if (strTableName=="CMS_RSNS")
				m_nScreenId=CMS_REASONS;
		else if (strTableName=="CMS_ACTNRSN")
				m_nScreenId=CMS_ACTOIN_REASON;
		else
		{
			AfxMessageBox("This screeen is not implemented yet");
			return 0;
		}
		
		ShowScreenV2(strTableName,m_nScreenId, strCriteria,msg);
		strScreenTag="";
		return 0;
	}
	if (strScreenTag!="")
		return ParentManage(strScreenTag,m_bEnableMainScreen,m_bEnableMainScreen,m_bEnableMainScreen,"",strCriteria);
	return 0;

}


CString CHSEMSCommonCategory::getFileSize(CString strFilePath) {
	CString strSize;
	CFile file(strFilePath, CFile::modeRead);
	double nFileSize = file.GetLength() / 1024;
	file.Close(); 
	strSize.Format("%.0f", nFileSize);
	return strSize;
}

CString CHSEMSCommonCategory::getFileExtension(CString strFilePath) {
	CFile file(strFilePath, CFile::modeRead);
	CString strFileExt	=	file.GetFileName();
	strFileExt.Delete(0, strFileExt.ReverseFind('.') + 1);
	file.Close(); 
	strFileExt.MakeUpper();	
	return strFileExt;
}


void CHSEMSCommonCategory::openInjueryRsltScr(CString strFormTag)
{
	CString strSQL="",strDefValues,strKeyField;
	bool bLocked = true;
	m_strScreenTag = "HSE_TgInjryAnlysis";

	if(strFormTag =="HSE_TGINCDNTMDCLRPRT" || strFormTag =="HSE_TGINCDNTMDCLRPRTFLWUP") {
		bLocked = false;
		strKeyField = FormGetField("HSE_INCDNTMDCLRPRTINJRDPRSNS","INCDNTMDCLRPRTINJRDPRSNS_LNRNUM");
	} else {
		strKeyField = FormGetField("HSE_ACDNTENTINJRDPRSN","ACDNTENTINJRDPRSN_LNRNUM");
	}				
	strSQL.Format("SELECT * FROM HSE_InjryAnlysis WHERE (InjryAnlysis_LINK = '%s')",strKeyField);			
	strDefValues.Format("InjryAnlysis_LINK|%s",strKeyField);
	ShowScreen(0,"HSE_TgInjryAnlysis","Injury Analysis",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);	
}

bool CHSEMSCommonCategory::isItemRqrChkQOH(CString strItemKey) {
	bool bItemRqrChkQOH = true;
	CString strSQL;
	strSQL.Format("SELECT ISNULL(WSTITMS_CHKQOH,'YES') FROM HSE_WSTITMS WHERE WSTITMS_ITMCD = '%s'",strItemKey);
	CString strItemRqrChkQOH = GetRecordSetData(strSQL).GetBuffer(0);	
	strItemRqrChkQOH.MakeUpper();
	if(strItemRqrChkQOH == "NO")
		bItemRqrChkQOH = false;	
	return bItemRqrChkQOH;	
}

/**************************************************************/
/* Update History
/*
/* Req
/* 
/* Bugs
/* - Bugs-3-2021.155
/*
/*
/* @strUserID user ID
/* @bgetFirstDepOnly bool if true get only first department
/*						  else get all departments
/**************************************************************/
CString CHSEMSCommonCategory::getEmpDep(CString strUserID,bool bgetFirstDepOnly) {
	CString strSQL;
	CString strAllDep = "";
	if(bgetFirstDepOnly)
		strSQL.Format("SELECT TOP 1 ISNULL(EMPLOYEE_WSHOP,'') FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '"+strUserID+"'");
	else 
		strSQL.Format("SELECT ISNULL(EMPLOYEE_WSHOP,'') FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '"+strUserID+"'");
	//Bugs-3-2021.155
	//CString strEmpDep = GetRecordSetData(strSQL).GetBuffer(0);	
	//return strEmpDep;
	CString strEmpDep = GetRecordSetData(strSQL).GetBuffer(0);
	if(strEmpDep != "" && !bgetFirstDepOnly) {		
		CString strDep=_T("");
		int index = 0;
		while (AfxExtractSubString(strDep, strEmpDep,index,'\2'))
		{   
			if(strDep != "" && strAllDep.Find(strDep) == -1) {
				if(strAllDep != "")
					strAllDep = strAllDep+",'"+strDep+"'"; 
				else
					strAllDep = "'"+strDep+"'"; 
			}
			index++;
		}	
	} else {
		strAllDep = strEmpDep;
	}
	return strAllDep;		
}


//General helper functions 
void CHSEMSCommonCategory::getNewSerialNoForATab(CString strMasterTable,CString strMasterKey,CString strDetailTable,CString strDetailLink,CString strSerialField) {
	//If strMasterKey is empty then it is a Master Screen and the Serial Number should be on the whole table
	CString strSQL;
	if(strMasterKey!="")
	{
		CString strMainLinkVal = FormGetField(strMasterTable,strMasterKey);		
		strSQL.Format("SELECT ISNULL(MAX(%s)+1,1) FROM %s WHERE %s = '%s'",strSerialField,strDetailTable,strDetailLink,strMainLinkVal);
		CString strCurrentMsrNo = GetRecordSetData(strSQL).GetBuffer(0);	
		FormSetField(strDetailTable,strSerialField,strCurrentMsrNo);
	}
	else
	{
		strSQL.Format("SELECT ISNULL(MAX(%s)+1,1) FROM %s ",strSerialField,strMasterTable);
		CString strCurrentMsrNo = GetRecordSetData(strSQL).GetBuffer(0);	
		FormSetField(strMasterTable,strSerialField,strCurrentMsrNo);
	}
	
}

CMapStringToOb* CHSEMSCommonCategory::InitializeParameters(char* pszFSATableName,char* pszFormTable,char* pszSerialFieldName,char* pszMasterKeyField,char *pszMainSerialDependentKey,char *pszSpecial)
{
	CMapStringToOb* pmpListOfParameters=new CMapStringToOb;
	pmpListOfParameters->SetAt("FSATableName",(CObject*)(void*)pszFSATableName);
	pmpListOfParameters->SetAt("FormTable",(CObject*)(void*)pszFormTable);
	pmpListOfParameters->SetAt("SerialFieldName",(CObject*)(void*)pszSerialFieldName);
	pmpListOfParameters->SetAt("MasterKeyField",(CObject*)(void*)pszMasterKeyField);
	pmpListOfParameters->SetAt("SpecialData",(CObject*)(void*)pszSpecial);
	if(pszMainSerialDependentKey)
		pmpListOfParameters->SetAt("MainSerialDependentKey",(CObject*)(void*)pszMainSerialDependentKey);
	return pmpListOfParameters;
}

void CHSEMSCommonCategory::AddTabInfo(CMapStringToOb*pmpListOfParameters,char *KeyName,char * pszTabName,char * pszKeyValue)
{
	CMapStringToOb *pmpDetailsSerials=new CMapStringToOb;
	pmpDetailsSerials->SetAt(pszTabName,(CObject*)(void*)pszKeyValue);
	pmpListOfParameters->SetAt(KeyName,(CObject*)pmpDetailsSerials);
}

//OpenPopUpScreen(strTableName,strSubFormTag,"PRMKY","HSE_AUDTPLNENTRY","HSE_AUDTPLNENTRY","Audit Plan Entry","","AUDTPLNENTRY_PLNN",ID_AUDITACTIONSENTRY);
CString gstrDepartment;
void CHSEMSCommonCategory::OpenPopUpScreen(CString strParentTableName,CString strParentFormTag,CString strParentTablePKName,CString strPopUpTbleName,CString strPopUpTagName,CString strPopUpEngCaption,CString strPopUpArbCaption,CString strPopUpLinkField,int nScreenID,CString strAddedCrireia)
{
	CString strSQL ;	
	bool bLocked = false;//HSE_AUDTNTFCTN_AUDTS,
	CString strMainLinkFeild = FormGetField(strParentTableName,strParentTablePKName);	
	if(strMainLinkFeild == "")
	{
		AfxMessageBox("There are no details linked with this record");
		return;	
	}
	setOwnerScreenTag(strParentFormTag);	
	strSQL.Format("WHERE %s = '%s'",strPopUpLinkField,strMainLinkFeild);
	/*if (strAddedCrireia!="")
		strSQL+=" AND " + strAddedCrireia;*/
	CString strDefault;
	strDefault.Format("%s|%s",strPopUpLinkField,strMainLinkFeild);
	gstrDepartment=FormGetField(strParentFormTag,"AUDTPLNENTRY_AUDTS_AUDTDPRTMNT");
	ShowScreen(-1,strPopUpTagName,strPopUpEngCaption,NORMAL_MODE,false,strSQL,"",strDefault,"",bLocked,(void*)(strAddedCrireia.GetBuffer(0)),nScreenID,NULL);
}

CString CHSEMSCommonCategory::GetEmployeeCodeForLoginUser()
{
	CString strSql;
	CString strUserName = GetUserLogin();
	strSql.Format("select EMPLOYEE_CODE from CMN_EMPLOYEE where EMPLOYEE_USRID='%s'",strUserName);
	CString strEmployeeCode=GetRecordSetData(strSql,1);
	if(strEmployeeCode=="")
	{
		AfxMessageBox("There is no employee linked with the login id, Please solve the problem and try again");
		strEmployeeCode="";
	}
	else if(strstr(strEmployeeCode,"|"))
	{
		AfxMessageBox("There are many employees linked with the login id, Please solve the problem and try again");
		strEmployeeCode="";
	}
	return strEmployeeCode;

}

/*void CHSEMSCommonCategory::AddTabSerialInfo(CMapStringToOb*pmpListOfParameters,char * pszTabName,char * pszTabSerialField)
{
	CMapStringToOb *pmpDetailsSerials=new CMapStringToOb;
	pmpDetailsSerials->SetAt(pszTabName,(CObject*)(void*)pszTabSerialField);
	pmpListOfParameters->SetAt("DetailsSerials",(CObject*)pmpDetailsSerials);
}


void CHSEMSCommonCategory::AddTabMsterLinkInfo(CMapStringToOb*pmpListOfParameters,char * pszTabName,char * pszTabLinkField)
{
	CMapStringToOb *pmpDetailsSerials=new CMapStringToOb;
	pmpDetailsSerials->SetAt(pszTabName,(CObject*)(void*)pszTabLinkField);
	pmpListOfParameters->SetAt("DetailsKeys",(CObject*)pmpDetailsSerials);
}
*/
//To be removed just testing git merge
