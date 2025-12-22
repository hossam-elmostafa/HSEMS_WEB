// HSEObj.cpp : Implementation of CHSEObj
#include "stdafx.h"
#include "HSEMS.h"
#include "HSEObj.h"
#include "Category.h"
#include "HSEMSApp.h"
#include "DongleListen.H"
#include "atldbcli.H"
#include "msdasql.H"
#include "ToolsListener.h"
#include "Utility.h"
#include "AnalysisReports.h"
#include "AccidentEntryCategory.h"
#include "RiskAssessmentEntryCategory.h"
#include "RiskAssessmentApprovalCategory.h"
#include "RiskAssessmentFollowUpCategory.h"
#include "PotentialHazardCategory.h"
#include "PotentialHazardConfirmationCategory.h"
#include "PotentialHazardEntryCategory.h"
#include "PotentialHazardFollowUp.h"
#include "PotentialHazardInquiry.h"
#include "HSEMSCommonCategory.h"
#include "AuditCheckListResultCategory.h"
#include "AuditCheckListPlanCategory.h"
#include "RejectReason.h"
#include "AcceptReason.h"
#include "HierarchyControl.h"
#include "HazardCategory.h"
#include "PrformnceMsurmntChcklistCategory.h"
#include "AccidentReasonCategory.h"
#include "Auditors.h"
#include "Auditess.h"
#include "HrmTyp.h"
#include "ShwDetails.h"
#include "PrsonlProtctveEquip.h"
#include "InjueryType.h"
#include "HumanBodyParts.h"
#include "NecessaryControlMeasures.h"
#include "LossAccidentEntryCategory.h"
#include "LossAccidentConfirmationCategory.h"
#include "PerformanceMeasurementEntryCategory.h"
#include "PerformanceMeasurementEvaluationCategory.h"
#include "PerformanceMeasurementFollowUpCategory.h"
#include "NearMissEntryCategory.h"
#include "NearMissConfirmationCategory.h"
#include "NearMissFollowUpCategory.h"
#include "NearMissReward.h"
#include "AccidentConfirmationCategory.h"
#include "AccidentFollowUpCategory.h"
#include "CAREntryCategory.h"
#include "CARConfirmationCategory.h"
#include "CARFlwupVstCategory.h"
#include "PtwEntryCategory.h"
#include "PtwConfirmationCategory.h"
#include "PtwClsCategory.h"
#include "AuditPlanEntryCategory.h"
#include "AuditPlanApprovalCategory.h"
#include "AuditPlanADTRSNADTES.h"
#include "AuditPlanPrepreChkLst.h"
#include "AuditPlanChckLstFdbck.h"
#include "VehicleAccidentEntryCategory.h"
#include "VehicleAccidentConfirmationCategory.h"
#include "VehicleAccidentFollowUpCategory.h"
#include "RiskAssesmentInquiry.h"
#include "PerformanceMEasurementInquiry.h"
#include "AccidentInquiry.h"
#include "AuditInquiry.h"
#include "CARInquiry.h"
#include "VehchileAccidentInquiry.h"
#include "NearMissInquiry.h"
#include "PTWInquiry.h"
#include "LossAccidentInq.h"
#include "RiskAnalysis.h"
#include "HSEToolEquipDistrib.h"
#include "AuditPlanFollowUpCategory.h"
#include "NearMissTypesCategory.h"
#include "AccidentTypesCategory.h"
#include "JobCategory.h"
#include "JobPtntlHzrdsLib.h"
#include "SitSrvyEntCategory.h"
#include "SitSrvyCnfrmtnCategory.h"
#include "SitSrvyFlwupCategory.h"
#include "IncdntFlshRprtCategory.h"
#include "SitSrvyInqCategory.h"
#include "LogsDefCategory.h"
#include "LogsDefAttrDtl.h"
#include "LogsEntCategory.h"
#include "LogsEntDtlCntCategory.h"
#include "SitSrvyRqrdActn.h"
#include "EnvrnmntAspctCategory.h"
#include "EnvrnmntAspctReviewCategory.h"
#include "DrillPlanEntryCategory.h"
#include "DrillPlanAprvCategory.h"
#include "DrillPlanExecutionCategory.h"
#include "IncdntInvstgtnEntryCategory.h"
#include "IncdntInvstgtnAprvCategory.h"
#include "WstRcvngEntCategory.h"
#include "WstDspslEntCategory.h"
#include "EnvMntrPlanCategory.h"
#include "EnvMntrExctnCategory.h"
#include "EnvMsrEntryCategory.h"
#include "EnvMsrAprvlCategory.h"
#include "EQInspctnCategory.h"
#include "EnvMntrMsrPopupCategory.h"
#include "AwrnsPlnEntryCategory.h"
#include "AwrnsPlnAprvlCategory.h"
#include "AwrnsPlnExctnCategory.h"
#include "AwrnsPlnInqCategory.h"
#include "ReasonsCategory.h"
#include "RscuPlnCategory.h"
#include "AreaHzrdsCategory.h"
#include "CARRcvdCategory.h"
#include "CARUndrExctnCategory.h"
#include "CARJobVrfctnCategory.h"
#include "ApproveCAR.h"


#include <windows.h>
#include <winbase.h>
#define	SILENTPOLYDBRECORDSET				4
/////////////////////////////////////////////////////////////////////////////
// CHSEObj
UINT m_unCurMenuID;
int CHSEObj::m_nCurrentDBEngine=DB_NOTDEFINED;
BOOL CHSEObj::m_bHijri =FALSE;
BOOL CHSEObj::m_bRejectQtyInMRS=FALSE;
BOOL CHSEObj::m_bUseManulSerialInOrders = FALSE;
BOOL CHSEObj::m_bGeneratePartNumFromQut = FALSE;
CString CHSEObj::m_strConnection="";
CString CHSEObj::m_strStaticDataPath="";
CString CHSEObj::m_strStaticSystemPath="";
CString CHSEObj::m_strUserFullName="";
CString CHSEObj::m_strLoginUserName="";
CString CHSEObj::m_lEmployeeDept="";
CString CHSEObj::m_strHelpFileName="";
//long CHSEObj::m_lEmployeeBudget=0;//RQ-9-2023.5 m_lEmployeeBudget is not used
long CHSEObj::m_lEmpSetting=0;
int CHSEObj::m_nSegLength=0;
short CHSEObj::m_User = 0;
CString CHSEObj::m_strCurrentProfile = "";
CString strConnection="";
BOOL bFirstTime=true;


CString GlstrVersionNo = "7.2.7"; //Accepted on 17/3/24


IFirstDesktopPtr	gpFirstDesktopObj;

//Send Mail Function
//typedef bool (*SendMailfn)(char* strSenderMailAdd,char* strSenderPass,char* strSenderName,
//						   char* strSubject,char* strRecipientMailAdd,char* strMailBody,
//						   char* strAttachFilePath);
//SendMailfn SendMail;

CHSEObj::CHSEObj()
{
	HRESULT hr = m_SendMailObj.CreateInstance(__uuidof(EmailMngr));
	if(hr!=0)
	{
		MessageBox(NULL,"Cannot initialize SendMail object","Prompt",0);
		return;
	}
	m_pRegHandle = NULL;
	m_strScreenDefauls = "";
	m_rcFormRect.SetRect(0,0,0,0);
	m_MapMenuIDTOTags.RemoveAll();
	m_MapScreenTOKey.RemoveAll();
	m_bModalPolyDb			=FALSE;
	m_bLockModalFlag		=FALSE;
	m_bArabic				=FALSE;
	m_bModalEnv				=FALSE;
	extern CHSEMSApp theApp;
	theApp.SetHSEObj(this);
	m_nStatus				=NORMAL_MODE;
	m_bRecodingEnabled		=FALSE;
	m_pHSEMSSYSPolyDb			=new _DB;
	m_pCategoryList			=new CObList;
	m_lpParent = 0;
	m_nCodeID=-1;
	FillTags();
	hr =m_pNotepadeObj.CreateInstance(__uuidof(FNotepadObj));
	if(hr!=0)
	{
		MessageBox(NULL,"Cannot initialize Notpad object","Prompt",0);
		return;
	}	
	hr = m_EGPCObj.CreateInstance(__uuidof(EGPC));
	if(hr!=0)
	{
		MessageBox(NULL,"Cannot initialize EGPC object","Prompt",0);
		return;
	}
	hr=	pMonthlyReportObj.CreateInstance(__uuidof(MonthlyReportCls));	
	if(hr!=0)
	{
		MessageBox(NULL,"Cannot initialize Monthly Report object","Prompt",0);
		return;
	}
	/*
	CString strServerName	= m_Connection.m_pSetupConnection->GetServerName();
	CString strCatalog		= m_Connection.m_pSetupConnection->GetSQLServerCatalog();
	CString strPassword		= m_Connection.m_pSetupConnection->GetSQLServerPassword();
	CString strID		= m_Connection.m_pSetupConnection->GetSQLServerUserID();
	*/
	 //HMODULE hModule = LoadLibrary("SendMailLib.dll");
	 //SendMail = (SendMailfn) GetProcAddress(hModule, "Send");
	 //if(!SendMail) {
	 //	return;
	 //}
	//a.ali
	m_strKeyValForRjctScr = "";
	m_strKeyValForAccptScr= "";
	m_strKeyValForApprvlScr= "";
	m_strSrcForRjctRsn	  = "";
	m_strSrcForAccptRsn ="";
	m_strSrcForApprvlRsn ="";
	m_bNewMode = false;
	m_bLogMultiRec = false;
	gbUpdateRejectedRecord=false;
	gbUpdateAcceptedRecord=false;
	gbUpdateApprovedRecord=false;
	
	gbOpenRejectScreenLocked=false;
	gbOpenAcceptScreenLocked=false;

	m_nAuditsKeyFld=-1;
	m_bDisableSitSrvyRqrdActnBtn=false;

}

CHSEObj::~CHSEObj()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	try
	{
		m_pRegHandle.Release();
		CloseAll();
		delete m_pHSEMSSYSPolyDb;
		delete m_pCategoryList;		
	}
	catch(...)
	{

	}

	try
	{
		if(m_pNotepadeObj)
		{
			m_pNotepadeObj.Release();
			m_pNotepadeObj=NULL;
		}
		if(m_SendMailObj)
		{
			m_SendMailObj.Release();
			m_SendMailObj=NULL;
		}
		if(pMonthlyReportObj) 
		{
			pMonthlyReportObj.Release();
			pMonthlyReportObj=NULL;
		}
	}
	catch(...)
	{
	}
}

void CHSEObj::Remove(CCategory * pCategory)
{
	try
	{
		POSITION position=m_pCategoryList->Find((CObject*)pCategory);
		if(position)
			m_pCategoryList->RemoveAt( position );
	}
	catch(...)
	{
		return;
	}

}

CCategory *CHSEObj::MakeCategory()
{
	CWaitCursor wc;
	CCategory *pCategory=new CCategory(m_strDataPath,m_pParentWnd,this,m_pHSEMSSYSPolyDb);
	
	// BY AAZMY ////////////////////
	if (!pCategory->m_bConnected)
	{
		delete pCategory;
		pCategory = NULL;
		return NULL;
	}
	////////////////////////////////
	m_pCategoryList->AddTail(pCategory);
	pCategory->SetFormRect(m_rcFormRect);
	pCategory->SetMenuID(m_unCurMenuID);
	return pCategory;
}

STDMETHODIMP CHSEObj::CloseAll()
{
	while(!m_pCategoryList->IsEmpty())
	{
		CCategory * pCategory=(CCategory *)m_pCategoryList->RemoveHead();
		try
		{
			if(AfxIsValidAddress(pCategory,sizeof(CCategory)))//this line added because of the rubbish data of calling the screen from HSEMS module
			{
				pCategory->GetMyCCategory()->OnScreenExit();
				delete pCategory;
			}
		}
		catch(...)
		{
		}
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::SetParent(long pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_pParentWnd=(CWnd*)pParent;
	m_lpParent= pParent;
	return S_OK;
}


void CHSEObj::InitEGPCObj() {
	BSTR strServerName	= m_Connection.m_pSetupConnection->GetSQLServerMachineName().AllocSysString();
	BSTR strCatalog		= m_Connection.m_pSetupConnection->GetSQLServerCatalog().AllocSysString();
	BSTR strPassword	= m_Connection.m_pSetupConnection->GetSQLServerPassword().AllocSysString();
	BSTR strID		    = m_Connection.m_pSetupConnection->GetSQLServerUserID().AllocSysString();
	
	_bstr_t serverName(strServerName,TRUE);
	_bstr_t catalog(strCatalog,TRUE);
	_bstr_t password(strPassword,TRUE);
	_bstr_t userID(strID,TRUE);

	if(m_EGPCObj != NULL) {							
		m_EGPCObj->InitializeDB(serverName,userID,password,catalog,-1);
	}
}

STDMETHODIMP CHSEObj::Initialize(int nAppCode,BSTR bstrProfile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_Connection.SetApplicationCode(APP_CODE);
	CString strProfile = CString (bstrProfile);
			
	if (strProfile != "")
	{
		m_strCurrentProfile = strProfile;
	}
	HRESULT hr = S_OK;
	try
	{
		//GMS Integration
		CString strProfile =m_Connection.m_pSetupConnection->GetCurrentProfile();
		m_Connection.m_pPrivConnection->SetApplicationInfo(APP_CODE,strProfile );
		m_Connection.m_pSetupConnection->SetApplicationInfo(APP_CODE,strProfile);
		m_strLoginUserName=m_Connection.m_pPrivConnection->GetUserLoginName(m_User);
		m_strUserFullName = m_Connection.m_pPrivConnection->GetUserFullName(m_User);
		
		//set caller app code
		m_callerAppCode = nAppCode;

		////////////////////////////////////////////////////////////////////////////////

		SetSystemPath();
		CreateConnectionString();
		long lRetVal=0;
//		CString strFormat;
//		strFormat.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=First2000",m_strSystemPath);
		lRetVal = m_pHSEMSSYSPolyDb->DBInit(m_nSystemDBType,m_strSystemPath,m_strSystemPath,APP_CODE,-1,m_strHelpFileName,APP_ODBC,m_strCurrentProfile);
		if(bFirstTime)
		{
			bFirstTime = false;
			CString strServerName = m_Connection.m_pSetupConnection->GetServerName();
			int nProtocol = m_Connection.m_pSetupConnection->GetProtocolType();
			m_strLoginUserName=m_Connection.m_pPrivConnection->GetUserLoginName(m_User);
			m_strUserFullName = m_Connection.m_pPrivConnection->GetUserFullName(m_User);
			CreateRegHandle();
			InitializeTables();			
		}
		createTagHashtable();
		CString strUserLogin = m_Connection.m_pPrivConnection->GetUserLoginName(m_User);//GetUserLogin();
		CString strSQL="IF OBJECT_ID('tempdb..##TEMP_TABLE') IS NULL CREATE TABLE #TEMP_TABLE(USERNAME VARCHAR(50)) INSERT INTO #TEMP_TABLE(USERNAME) VALUES('"+strUserLogin+"')";
		//ExecuteSQL(strSQL.AllocSysString(),-1);

		//Initialize EGPC Object
		InitEGPCObj();
	}
	catch(...)
	{
		hr = S_FALSE;
	}

	return hr;
}

void CHSEObj::InitializeTables()
{
	_DB*	pPolyDb = new _DB();
	CHSEObj::DBInit(pPolyDb,NULL);
	
	CString strSQL;
	
	// Initialize HSEMS_SETUP


	// Set Language


	delete pPolyDb;	
}

STDMETHODIMP CHSEObj::SetLanguage(BOOL bArabic)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_bArabic=bArabic;
	BSTR bstrFolder;
	GetHelpFileName(&bstrFolder);
	return S_OK;
}

STDMETHODIMP CHSEObj::GetLanguage(BOOL *bArabic)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*bArabic=m_bArabic;
	return S_OK;
}

BOOL CHSEObj::IsArabic()
{
	return m_bArabic;
}

int CHSEObj::DoFunction(int nCurrentCard,CString strTableName,int nStatus,BOOL bModal,CString strCriteria,CString strComesFrom,CString strScreenDefauls,CString strReturnBrowseValue,int nScreenId,void* msg,CCommonCategoryWrapper **pCommonCategoryWrapper)
{
	int nRetVal=0;
	m_strScreenDefauls = strScreenDefauls;
	CWaitCursor wc;
	ATOM ThisScreenAtom=FindAtom(strTableName);
		if(ThisScreenAtom)
		DeleteAtom(ThisScreenAtom);
	
	strTableName.MakeUpper();
	m_nStatus=nStatus;
	if (strComesFrom == "CREATELIKE")
		m_nStatus=EDIT_MODE;

	if (strComesFrom == "ITEM_TREE") // Means Comes From Item Tree
	{
		m_nStatus=NORMAL_MODE;

	}

	if (nCurrentCard == -1) // means Related record selected event
		SetFastSearchCriteria(strCriteria.AllocSysString());
	
	//if(m_nStatus==NORMAL_MODE || bModal)
	if(bModal)
	{
		m_bModalPolyDb=TRUE;
		m_bLockModalFlag=TRUE;
	}

	if (strComesFrom != "ITEM_TREE") 
	{
		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		if (strTableName == "")
		{
			CString strCurrentCard;
			strCurrentCard.Format("%ld",nCurrentCard);
			m_MapMenuIDTOTags.Lookup(strCurrentCard,strTableName);
		}
		
		///////////////////////////////////////////////////////////////
		bool found = false;
		if(strTableName == "HSE_TGACDNTENT")  
		{
			ShowAccidentEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
			
		}
		else if(strTableName == "HSE_TGSTE")
		{
			ShowSite(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSITETYPE")
		{
			ShowSiteType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMN_COUNTRIES")
		{
			ShowCountry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLANG")
		{
			ShowLanguage(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTCHCKLSTRSLT")
		{
			ShowAuditCheckListPlan(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTCHCKLSTPLN")
		{
			ShowAuditCheckListResult(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRJCTRSN" || strTableName == "HSE_RJCTRSN")
		{
			ShowRejectreason(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRRVWINF" || strTableName == "HSE_CRRVWINF")
		{
			ShowAcceptreason(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_CRAPPRVLINF" || strTableName == "HSE_TGCRAPPRVLINF")
		{
			ShowApprovReason(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMN_DEPART")		
		{
			ShowDepartment(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWRKLOC")
		{
			ShowWorkLocation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTSK")
		{
			ShowTask(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHRCTRL")
		{
			ShowControlMeasures(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHZRD")
		{
			ShowHazard(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTCHKLIST")
		{
			ShowPerformanceMeasurementCheckList(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTRSN")
		{
			ShowAccidentReason(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNENTAUDITORS")
		{
			ShowAuditors(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNENTAUDITEES")
		{
			ShowAudites(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHRMTYP")
		{
			ShowHarmTypes(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGDTL")
		{
			ShowDetails(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCPYDTL")
		{
			ShowCopyofdetail(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCARMODELBAS")
		{
			ShowCARBasis(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGROL")
		{
			ShowRole(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGVCLQSTNS")
		{
			ShowVehicleAccidentQuestions(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGBSCREF")
		{
			ShowBasicReference(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVRNMNTCND")
		{
			ShowEnvironmentCondition(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPRSNLPRTCTVEQ")
		{
			ShowPersonalProtectiveEquipmentScreen(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPRSNLPRTCTVEQPMNT")
		{
			ShowPersonalProtectiveEquipmentpopUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "CMN_CURENCY")
		{
			ShowCurrency(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSGNTUR")
		{
			ShowSignatures(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINJRYTYP")
		{
			ShowInjuryType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINJRYTP")
		{
			ShowInjuryTypepopUpscreen(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRWRDTYP")
		{
			ShowRewardType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTNTLSVRTY")
		{
			ShowPotentialSeverity(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTSCOR")
		{
			ShowPerformanceMeasureScore(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTTYP")
		{
			ShowAccidentType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTMDM")
		{
			ShowAccidentMedium(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTYPLOSS")
		{
			ShowTypeofLoss(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHMNBODYPART")
		{
			ShowHumanBodyParts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTSKADTRSLT")
		{
			ShowTaskAuditResult(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTWSTYP")
		{
			ShowPTWType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKSVRTRATCLS")
		{
			ShowRiskSeverityRatingClass(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKLKLYHODRAT")
		{
			ShowRiskLikelihoodRating(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "CMN_EMPLOYEE")
		{
			ShowEmployee(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKRNKDESC")
		{
			ShowRiskRankDescription(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHSEPRCDRLIST")
		{
			ShowHSEProceduresList(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTWSRSPNSBLTY")
		{
			ShowPTWResponsibility(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENTRJCRSN")
		{
			ShowEntryRejectReasons(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHSERGLR")
		{
			ShowHSERegulations(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNCSSRYCTRLMSRMNT")
		{
			ShowNecessaryControlMeasures(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKASSMNTENT")
		{
			ShowRiskAssessmentEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKASSMNTAPR")
		{
			ShowRiskAssessmentApproval(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKASSMNTFOLLWUPDT")
		{
			ShowRiskAssessmentFollowUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAUDITORS")
		{
			ShowAuditors(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAUDITEES")
		{
			ShowAuditees(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTENT")
		{
			ShowPerformanceMeasurementEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTEVAL")
		{
			ShowPerformanceMeasurementEvaluation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTFLWUP")
		{
			ShowPerformanceMeasurementFollowUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRSTMISCENT")
		{
			ShowNearMissEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRSTMISCCNFRMTN")
		{
			ShowNearMissConfirmation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRSTMISCFLWUP")
		{
			ShowNearMissFollowUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRMISRWARD")
		{
			ShowNearMissReward(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTCNFRMTN")
		{
			ShowAccidentConfirmation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTFOLLWUPDT")
		{
			ShowAccidentFollowup(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCENT")
		{
			ShowCorrectiveActionEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCCNFRMTN")
		{
			ShowCorrectiveActionConfirmation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCARFLWUPVSTS")
		{
			ShowCorrectiveActionFollowupvisit(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTWSRGSTRENT")
		{
			ShowPTWRegisterEntry(strCriteria.AllocSysString(),strScreenDefauls.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTWSRGSTRCNFRMTN")
		{
			ShowPTWRegisterConfirmation(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGPTWSRGSTRCLS")
		{
			ShowPTWsRgstrCls(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNENT")
		{
			ShowAuditPlanEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNAPR")
		{
			ShowAuditPlanApproval(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNFOLLWUP") 
		{	
			ShowAuditPlanFlwUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNADTRSANDADTS")
		{
			ShowAuditPlanAuditorsandAuditees(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNPRPRCHKLST")
		{
			ShowAuditPlanPreparechecklist(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNCHKLSTFDBCK")
		{
			ShowAuditPlanAuditchecklistfeedback(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGATCHCLSFC")
		{
			ShowAttachmentclassification(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHSEPLC")
		{
			ShowHSEPolicy(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCOMPPRF")
		{
			ShowCompanyProfile(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHUMNBDYPRT")
		{
			ShowBodyParts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKASSMNTINQ")
		{
			ShowRiskInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPERFMSRMNTINQ")
		{
			ShowPerformanceInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACDNTINQ")
		{
			ShowAccident(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTPLNINQ")
		{
			ShowAudit(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCINQ")
		{
			ShowCAR(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGVCLACDNTINQTN")
		{
			ShowVehchileAccidentInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRSTMISCINQ")
		{
			ShowNearMissInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTWSRGSTRINQ")
		{
			ShowPTW(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLOSSACDNTINQ")
		{
			ShowLossAccidentInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGHSEPRCDRLISTINQ")
		{
			ShowHSEProceduresListInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKANLYS" || strTableName == "HSE_RSKANLYS")
		{
			ShowRiskAnalysis(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTOOLEQUIP")
		{
			ShowHSEToolsEquipments(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTOOLSEQUIPDSTRBTN")
		{
			ShowHSEToolsEquipmentsDistribution(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}	
		else if(strTableName == "HSE_ENTRJCRSN" || strTableName == "HSE_TGENTRJCRSN")
		{
			ShowEntryRejectReasons(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if (strTableName == "HSE_TGCNCRNDPT")
		{
			ShowPopupEntry(strTableName.AllocSysString(),strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if (strTableName == "HSE_TGACTLADTPLNENTAUDITORS") {
			ShowActualAuditors(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGACTLADTPLNENTAUDITEES") {
			ShowAudites(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGANSWRDTL") {
			ShowDtlForChkLst(strCriteria.AllocSysString());
			goto restOfDoFunc;
		} 
		else if(strTableName == "HSE_TGSUBDTL") {
			ShowSubDetails(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGNRMISTYPES") {
			ShowNearMissTypes(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPTNTLHZRDSLIB") {
			ShowJobPtntlHzrdsLIB(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPRCTNS") {
			ShowPrecautions(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMN_TRADES") {
			ShowTrades(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGFIRSTINFO") {
			ShowFirstInfo(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMN_DOCTYP") {
			ShowDocType(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}						 
		else if(strTableName == "HSE_TGADTACTVTRQRDACTN") {
			ShowAdtActvtRqrdActn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKSVRTYCNTRLMSR") {
			ShowRskSvrtyCntrlMsr(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGJOBSTP") {
			ShowHSEJobStp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCNSQNCS") {
			ShowHSECnsqncs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTGRP") {
			ShowHSEAdtGrp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTCHCKLST") {
			ShowHSEAdtChcklst(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}	
		else if(strTableName == "HSE_TGSITSRVYENT") {
			ShowSitSrvyEnt(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSITSRVYCNFRMTN") {
			ShowSitSrvyCnfrmtn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		} 
		else if(strTableName == "HSE_TGSITSRVYFLWUP") {
			ShowSitSrvyFlwup(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTRSCHDL") {
			ShowAdtRschdl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGADTCNCL") {
			ShowAdtCncl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTFLSHRPRT") {
			ShowIncdntFlshRprt(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRLTDOPRTNS") {
			ShowHSERltdOprtns(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMTRLRLSTYP") {
			ShowHSEMtrlRlsTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCNTNMNTTYP") {
			ShowHSECntnmntTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		if(strTableName == "HSE_TGMTRLRLSCAT") {
			ShowHSEMtrlRlsCat(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRLSMTRL") {
			ShowHSERlsMtrl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGUOM") {
			ShowHSEUOM(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMTRLRLSTO") {
			ShowHSEMtrlRlsTo(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSPLCLSFCTN") {
			ShowHSESplClsfctn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGASTTYP") {
			ShowHSEAstTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSITSRVYINQ") {
			ShowSitSrvyEntInq(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINJRYANLYSIS") {
			ShowInjryAnlysisPopup(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLOGSDFNTN") {
			ShowHSELogsDef(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLOGSDFNTNATRBTSDTL") {
			ShowLogsDefAttrDtl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}	
		else if(strTableName == "HSE_TGLOGSENT") {
			ShowHSELogsEnt(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLOGSENTDTLCNT") {
			ShowLogsEntDtlCnt(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGJOBSTPDFNTN") {
			ShowHSEJobStpDfntn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCNSQNCSDFNTN") {
			ShowHSECnsqncsDfntn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSITSRVYRQRDACTN") {
			ShowSitSrvyRqrdActn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGVCLTYP") {
			ShowVclTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_ASPCTSENTRYATENTRY") {
			ShowEnvronmntlAspctEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_ASPCTSRVWATRVW") {
			ShowEnvronmntlAspctReview(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_DRLLPLNENTRYATENTRY") {
			ShowDrillPlanEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_DRLLPLNAPPRVLATAPPRVL") {
			ShowDrillPlanAprv(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_DRLLEXCUTNATEXCUTN") {
			ShowDrillPlanExecution(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTFLSHRPRTRVEW") {
			ShowIncdntFlshRprtRvw(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTFLSHRPRTINQ") {
			ShowIncdntFlshRprtInq(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTINVSTGTNENTRY") {
			ShowIncdntInvstgtnEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTINVSTGTNAPRVL") {
			ShowIncdntInvstgtnAprv(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMSNRSLOG") {
			ShowMsnRsLog(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGDRLFNDNGS") {
			ShowDrlFndngs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCNTRCTR") {
			ShowHSECntrctr(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPRJCTS") {
			ShowHSEPrjcts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMNDRLTYP") {
			ShowHSEMnDrlTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINCDNTMDCLRPRT"){
			ShowHSEIncdntMdclRprt(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGINVLVDPRTS"){
			ShowInvlvdPrts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGPCKNGMTHDS"){
			ShowPckngMthds(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTITMS"){
			ShowWstItms(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTZNS"){
			ShowWstZns(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTTRNSPRTRS"){
			ShowWstTrnsprtrs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTTRTMNTPLNTS"){
			ShowWstTrtmntPlnts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTRCVNGENTRY"){
			ShowWstRcvngEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGWSTDSPSLENTRY"){
			ShowWstDspslEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMNTRACTVTS"){
			ShowEnvMntrActvts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMNTRPLAN"){
			ShowEnvMntrPlan(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMNTREXCTN"){
			ShowEnvMntrExctn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMSRTYPS"){
			ShowEnvMsrTyps(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMSRENTRY"){
			ShowEnvMsrEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMSRAPRVL"){
			ShowEnvMsrAprvl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMSRENTRYPOLTANTPRPCAR"){
			ShowEnvMsrEntryPoltantPrpCAR(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGOBSRVTNTRGTS"){
			ShowObsrvtnTrgts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_EQTYPS"){
			ShowEQTyps(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_EQINSPCTNRQUST"){
			ShowEQInspctnRqust(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_EQINSPCTNRSULT"){
			ShowEQInspctnRsult(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_EQINSPCTNWRK"){
			ShowEQInspctnWrk(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_EQINSPCTNINQRY"){
			ShowEQInspctnInqry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMNTRACTVTSPLTNT"){
			ShowEnvMntrActvtsPltnt(strCriteria.AllocSysString(),strComesFrom.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGERTLST"){
			ShowHSEERTList(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSTPEFFCT"){
			ShowHSEStpEffct(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKLVL"){
			ShowHSERskLvl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTOPEVNTS"){
			ShowHSETopEvnts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGTOPEVNTSTHRTSBRS"){
			ShowHSETopEvntsThrtsBrs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVMNTRRQRDACTN"){
			ShowHSEEnvmntrRqrdActn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGLFSAVNGRLS"){
			ShowLfSavngRls(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAWRNSCRS"){
			ShowAwrnsCrs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAWRNSPLNENTRY"){
			ShowAwrnsPlnEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAWRNSPLNAPRVL"){
			ShowAwrnsPlnAprvl(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAWRNSPLNEXCTN"){
			ShowAwrnsPlnExctn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAWRNSPLNINQ"){
			ShowAwrnsPlnInq(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGAWRNSPLN_CRS_ATNDS"){
			ShowAwrnsPlnCrsAtnds(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMS_RSNS"){
			//ShowCMSRsns(strCriteria.AllocSysString());
			//ID_CMS_Rsns
			ShowScreenIn(-1,strCriteria.AllocSysString(),strTableName.AllocSysString(),msg);
			goto restOfDoFunc;
		}
		else if(strTableName == "CMS_ACTNRSN"){
			ShowScreenIn(nScreenId,strCriteria.AllocSysString(),strTableName.AllocSysString(),msg);
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUENTTS"){
			ShowRscuEntts(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCNFNDSPCTYPS"){
			ShowCnfndSpcTyps(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUHZRDS"){
			ShowRscuHzrds(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUTLS"){
			ShowRscuTls(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUCMMUNCTNMTHDS"){
			ShowRscuCmmunctnMthds(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUPLNENTRY"){
			ShowRscuPlnEntry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSCUPLNINQURY"){
			ShowRscuPlnInqury(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGHZRDPRCS"){
			ShowHzrdPrcs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAREAHZRDS"){
			ShowAreaHzrds(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAHZRDSPTNTLHZRDCTRLMSR"){
			ShowAHzrdsPtntlHzrdCtrlMsr(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGAREAHZRDSPTNTLHZRDEFFCT"){
			ShowAreaHzrdsPtntlHzrdEffct(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGRSKTYP"){
			ShowRskTyp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMDCLAUTHRTY"){
			ShowMdclAuthrty(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGMDCLDCSN"){
			ShowMdclDcsn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGAUDTRS"){
			ShowAudtrs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGENVRNMNTLMSRS"){
			ShowEnvrnmntlMsrs(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_TGINCDNTMDCLRPRTFLWUP"){
			ShowIncdntMdclRprtFlwUp(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGDRLFNDNGSUNPLNDSCNROS"){
			ShowDrlFndngsUnPlndScnros(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}		
		else if(strTableName == "HSE_DRLLPLNINQ"){
			ShowDrillInquiry(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCRCVD"){
			ShowCrctveAccRcvd(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCUNDREXCTN"){
			ShowCrctveAccUndrExctn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGCRCTVEACCJOBVRFCTN"){
			ShowCrctveAccJobVrfctn(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "HSE_TGSITSRVYTYPS"){
			ShowSitSrvyTyps(strCriteria.AllocSysString());
			goto restOfDoFunc;
		}
		else if(strTableName == "CMN_ATCHMNTFRMT"){
			////////////////////////////
			//Bugs-12-2023.1
			ShowAtchmntFrmts(strCriteria.AllocSysString());
			goto restOfDoFunc;
			////////////////////////////
		}
		else if (strTableName != "") {
			ShowScreenIn(nScreenId,strCriteria.AllocSysString(),strTableName.AllocSysString(),msg,pCommonCategoryWrapper);
			goto restOfDoFunc;
		}
	}
	restOfDoFunc:
	//TODO : review this 
	//Changed by a.ali
	//Date : 28/02/2017
	//When Closing screen check if parent is Modal then don't empty m_strScreenDefauls member
	//as I assume that if screen is modal then this screen must be list pop-up and if user
	//add new record again then default values must be present.
	//TODO : This line should be un-commented 
	POSITION pos= m_pCategoryList->GetTailPosition();	
	//m_pCategoryList->GetPrev(pos);	
	CObject* prevObj=m_pCategoryList->GetAt(pos);
	CCategory *prevCategory= (CCategory *)prevObj;
	if (prevCategory != NULL)
	{
		bool bPrevModal = prevCategory->IsModalPolyDb();
		//if(!bPrevModal)
		//	m_strScreenDefauls = "";
	}
	m_bModalPolyDb=FALSE;
	m_bLockModalFlag=FALSE;
	m_nStatus=NORMAL_MODE;
	if(ThisScreenAtom)
	{
		AddAtom(strTableName);
	}
	wc.Restore();
	wc.~CWaitCursor();
	return nRetVal;
}

STDMETHODIMP CHSEObj::DoFunction(int nCurrentCard,BSTR bstrTableName,int nStatus,BOOL bModal,BSTR bstrWhereClause)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
	CString strTableName=OLE2T(bstrTableName);
	CString strWhereClause=OLE2T(bstrWhereClause);
	
	DoFunction(nCurrentCard,strTableName,nStatus,bModal,strWhereClause);

	return S_OK;
}

STDMETHODIMP CHSEObj::SetUserID()
{
	DefineUserID();
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSetupScreen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	bFirstTime = true;
	if(!m_pCategoryList->IsEmpty())
	if(MessageBox(NULL,"All Screens Must be closed First,Proceed?(Y/N)","Prompt",MB_YESNO) ==IDYES)
	{
		CloseAll();
	}
	else 
		return S_OK;
	if(DefineSetupScreen()==-1)
	{
		return S_OK;
		//CloseAll();
		//delete this;
		//FatalAppExit(0,"Because data you canceled is must fatal error occurred \nRun application again.");
	}

	int nDatabaseType = m_Connection.m_pSetupConnection->GetApplicationDBType();
	m_bHijri = m_Connection.m_pSetupConnection->GetHijriState();
	CString strServerName = m_Connection.m_pSetupConnection->GetServerName();
	m_Connection.m_pSetupConnection->GetProtocolType();
	m_nCurrentDBEngine =nDatabaseType;
	CreateConnectionString();
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowUsersScreen(BOOL* bValid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strServerName = m_Connection.m_pSetupConnection->GetServerName();
	int nProtocol = m_Connection.m_pSetupConnection->GetProtocolType();

	BOOL bValidUser = DefineUserID();
	if(bValidUser)
	{		
		CString strProfile =m_Connection.m_pSetupConnection->GetCurrentProfile();
		m_Connection.m_pPrivConnection->SetApplicationInfo(APP_CODE,strProfile );
		m_Connection.m_pSetupConnection->SetApplicationInfo(APP_CODE,strProfile);
		m_strLoginUserName=m_Connection.m_pPrivConnection->GetUserLoginName(m_User);
		m_strUserFullName = m_Connection.m_pPrivConnection->GetUserFullName(m_User);
		
		CreateConnectionString();
		if(m_lEmpSetting==0)
			CheckForUser(m_strLoginUserName);
		
	}
	*bValid = bValidUser;
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowMenuScreen(long lMenuHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DefineMenuScreen(lMenuHandle);

	return S_OK;
}

STDMETHODIMP CHSEObj::ShowFieldsPriv()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DefineFieldsPriv();

	return S_OK;
}


STDMETHODIMP CHSEObj::ShowUsersList()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	DefineUsersList();

	return S_OK;
}

STDMETHODIMP CHSEObj::UserVerify(int nUserCode, int nMenuItem,BSTR* bstrCaption)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strCaption;
	BOOL bReVal = DefineUsersVerfy(nMenuItem,strCaption);
	*bstrCaption = strCaption.AllocSysString();
	return bReVal ;
}


UINT CHSEObj::DefineSetupScreen()
{
	CString str=m_Connection.m_pSetupConnection->GetSystemDBName();
	int nRetVal;
	nRetVal = m_Connection.m_pSetupConnection->ShowSetup(str,"", "", TRUE, IsArabic(),APP_CODE,m_User);
	if(nRetVal ==-1)
	{
		return -1;
	}
	if(m_User ==0)
		m_User = nRetVal;

	Initialize(APP_CODE,m_strCurrentProfile.AllocSysString());
	return 1;
}

UINT CHSEObj::DefineUserScreen()
{
	m_Connection.m_pSecurityConnection->ShowUsersScreen(STR_APP_CODE,TRUE,IsArabic(),m_User);
	return -1;
}

UINT CHSEObj::DefineUsersList()
{
	m_Connection.m_pSecurityConnection->ShowUsersScreen(STR_APP_CODE,TRUE,IsArabic(),m_User);
	return -1;
}


UINT CHSEObj::DefineMenuScreen(long lMenuHandle)
{
	m_Connection.m_pPrivConnection->ShowMenusItem(STR_APP_CODE, TRUE,IsArabic(),FALSE,m_User);
	return -1;
}

BOOL CHSEObj::DefineUserID()
{
	//AfxMessageBox("Msg 0");
	m_User=m_Connection.m_pSecurityConnection->ShowLogScreen(STR_APP_CODE,TRUE,IsArabic(),GlstrVersionNo);
	//AfxMessageBox("Msg 1");
	m_strCurrentProfile = m_Connection.m_pSetupConnection->GetCurrentProfile();

	if(m_User <= 0)
		return FALSE;

	m_strLoginUserName = m_Connection.m_pPrivConnection->GetUserLoginName(m_User);
	//AfxMessageBox("Msg 2");
	return TRUE;
}

void CHSEObj::DefineFieldsPriv()
{
	m_Connection.m_pPrivConnection->ShowMenusDetails(STR_APP_CODE, TRUE, IsArabic(),m_User);
}

BOOL CHSEObj::DefineUsersVerfy(int nMenuItem,CString &strCaption)
{
	BOOL bRetVal = FALSE;
	if (nMenuItem<=0)
	{
		CString strRetVal;
		m_MapMenuIDTOTags.Lookup(strCaption,strRetVal);
		nMenuItem = atol(strRetVal);
	}
	bRetVal = m_Connection.m_pPrivConnection->MenuVerify(STR_APP_CODE, m_User,nMenuItem);
	return bRetVal ;
}

STDMETHODIMP CHSEObj::SetSystemPath()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_Connection.m_pSetupConnection->SetApplicationInfo(APP_CODE,m_strCurrentProfile);
	m_strSystemPath = m_Connection.m_pSetupConnection->GetSystemDBName();

	if(m_strSystemPath =="")
	{
		m_User=m_Connection.m_pSecurityConnection->ShowLogScreen(STR_APP_CODE,TRUE,IsArabic(),GlstrVersionNo);
		m_strSystemPath = m_Connection.m_pSetupConnection->GetSystemDBName();
	}

	if (m_strSystemPath.Find("SQLOLEDB") != -1)
		m_nSystemDBType = DB_SQLSERVER;
	else
		m_nSystemDBType = DB_ACCESS;

	m_strStaticSystemPath = m_strSystemPath;
	return S_OK;
}

CString CHSEObj::GetScreenDefauls()
{
	return m_strScreenDefauls;
}

//a.ali 02/05/2021
void CHSEObj::SetScreenDefauls(CString strScreenDefaults)
{
	m_strScreenDefauls = strScreenDefaults;
}

BOOL CHSEObj::IsModalPolyDb()
{
	return m_bModalPolyDb;
};

STDMETHODIMP CHSEObj::SetModal(BOOL bModalVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_bModalEnv = bModalVal;
	return S_OK;
}

long CHSEObj::DBInit(_DB *pHSEMSPolyDb,CToolsListener* pToolsListener,BOOL bExtended)
{
	CWaitCursor wc;
	long lRetVal=0;
	pHSEMSPolyDb->SetCalendarType(m_bHijri);
	if(!bExtended)
	{
		lRetVal = pHSEMSPolyDb->DBInit(m_nCurrentDBEngine,strConnection, m_strStaticSystemPath,APP_CODE,m_User,m_strHelpFileName,APP_ODBC,m_strCurrentProfile);
		//try{
			//pHSEMSPolyDb->DBExecSql("CREATE TABLE #TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))",PDBRS_LOAD_FROM_GMS_DB,1);
		//IF OBJECT_ID('tempdb..#TEMP_HSE_TABLE') IS NULL

			//pHSEMSPolyDb->DBExecSql("IF COL_LENGTH('tempdb..#TEMP_HSE_TABLE', 'KEYrec') IS NULL begin CREATE TABLE #TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50)) end",PDBRS_LOAD_FROM_GMS_DB,1);
		/*}
		catch{
		}*/
	}
	else
		lRetVal = pHSEMSPolyDb->DBInitEx(strConnection);


	if(lRetVal<= 0)
	{
		// BY AAZMY
		//MessageBox(NULL,"Cannot initialize connecteion\n\rPlease review your connection setting","Prompt",0);
		return -1;
	}
	try
	{
		if(pToolsListener != NULL)
			pToolsListener->m_pToolsObj->DBInit(APP_CODE, m_User,m_nCurrentDBEngine,strConnection.GetBuffer(0),m_strStaticSystemPath,GetCurrentProfile());
	}
	catch (_com_error& e) 
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		CString strErrior;
		strErrior.Format("Code = %08lx\n Code meaning = %s\nSource = %s\nDescription = %s\n", e.Error(),e.ErrorMessage(),(LPCTSTR) bstrSource,(LPCTSTR) bstrDescription);
	}
	return lRetVal;
}

void CHSEObj::FireCloseAll()
{
	CloseAll();
}

void CHSEObj::CreateConnectionString()
{
	int nDatabaseType = m_Connection.m_pSetupConnection->GetApplicationDBType();
	m_bHijri = m_Connection.m_pSetupConnection->GetHijriState();
	m_nCurrentDBEngine= nDatabaseType ;
	CString strOldConnection = strConnection;
	switch(m_nCurrentDBEngine)
	{
	case DB_ORACLE:							
		{
			CString strServiceName,strUserName,strPassWord,strProvider;
			strServiceName = m_Connection.m_pSetupConnection->GetOracleDataSource();
			strUserName = m_Connection.m_pSetupConnection->GetOracleUserID();
			strPassWord = m_Connection.m_pSetupConnection->GetOraclePassword();
			short sVer =m_Connection.m_pSetupConnection->GetOracleVersion();
			if(sVer==7)
				strConnection.Format("Provider=MSDAORA.1;Password=%s;User ID=%s;Data Source=%s;Persist Security Info=True",strPassWord,strUserName,strServiceName);
			else
				strConnection.Format("Provider=OraOLEDB.Oracle.1;Password=%s;User ID=%s;Data Source=%s;Persist Security Info=True",strPassWord,strUserName,strServiceName);

			m_strPaneText.Format("Oracle : %s/%s",strUserName,strServiceName);
			m_strDB = strUserName;
		}
		break;
	case DB_SQLSERVER:
		{
			
			CString strUserID,strPassWord,strServerName,strCatalogName;
			strUserID=m_Connection.m_pSetupConnection->GetSQLServerUserID();
			strPassWord=m_Connection.m_pSetupConnection->GetSQLServerPassword();
			strServerName=m_Connection.m_pSetupConnection->GetSQLServerMachineName();
			strCatalogName=m_Connection.m_pSetupConnection->GetSQLServerCatalog();
			strConnection.Format("Provider=SQLOLEDB.1;Persist Security Info=True;User ID=%s;Password=%s;Initial Catalog=%s;Data Source=%s;Workstation ID=%s",strUserID,strPassWord,strCatalogName,strServerName,m_strUserFullName);
			m_strPaneText.Format("SQL Server : %s/%s/%s",strUserID,strServerName,strCatalogName);
			m_strDB = strCatalogName;
		}
		break;
	case DB_ACCESS:
		{			
			m_strStaticDataPath = m_Connection.m_pSetupConnection->GetApplicationDataBaseName();
			//strConnection.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=First2000",m_strStaticDataPath);
			strConnection = m_strStaticDataPath;
			m_strPaneText.Format("Access");
		}
		break;
	}		
	if(m_strConnection == strConnection && m_strConnection !="")
		return;
	m_strConnection= strConnection;
	m_pHSEMSSYSPolyDb->CloseConnection();
}

STDMETHODIMP CHSEObj::GetPaneString(BSTR *strPaneText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*strPaneText = m_strPaneText.AllocSysString();
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowReportsPriv()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	IReportsObjPtr pReportsObj;
	pReportsObj.CreateInstance(__uuidof(ReportsObj));
	CString strOpen;
	_bstr_t bstr;
	bstr.Assign(strConnection.AllocSysString());
	try
	{
		pReportsObj->SetPolyDb(m_nCurrentDBEngine,bstr,STR_APP_CODE,APP_ODBC,m_User,IsArabic(),FALSE);
	}
	catch(_com_error &e)
	{
		_bstr_t s=e.Description();
	}
	long lHWnd = pReportsObj->GetMenuHandle();
	m_Connection.m_pPrivConnection->UpdateMenu(STR_APP_CODE, (long)lHWnd,m_User,IsArabic(),TRUE);
	m_Connection.m_pPrivConnection->ShowMenusItem(STR_APP_CODE, TRUE,IsArabic(),TRUE,m_User);
	return S_OK;
}

void CHSEObj::CheckForUser(CString strUserFullName)
{
	CString strSQL;
	long lVal=0;
	CPolyDbRecordset DaoRecSet;
	strUserFullName.MakeUpper();
	strSQL.Format("SELECT COUNT(1) AS FOUNDEMPLOYEE,EMPLOYEE_AUTHORITYB FROM CMN_EMPLOYEE WHERE EMPLOYEE_CODE = '%s' GROUP BY EMPLOYEE_AUTHORITYB",strUserFullName);
	long lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("FOUNDEMPLOYEE",lVal);
		//DaoRecSet.GetFieldValue("EMPLOYEE_AUTHORITYB",m_lEmployeeBudget); //RQ-9-2023.5 m_lEmployeeBudget is not used
		DaoRecSet.Close();
	}
}

STDMETHODIMP CHSEObj::UpdateMenu(int nAppCode, long lMenuHandle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strFormat;
	strFormat.Format("%d",nAppCode);
	m_Connection.m_pPrivConnection->UpdateMenu(strFormat.GetBuffer(NULL),lMenuHandle,m_User,IsArabic(),FALSE);
	return S_OK;
}

STDMETHODIMP CHSEObj::BuildMenu(long lMenuHandle,BOOL bLanguage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strFormat;
	strFormat.Format("%d",APP_CODE);
	m_Connection.m_pPrivConnection->MenuBuild(strFormat.GetBuffer(NULL),lMenuHandle,0,bLanguage,FALSE);
	CreateGlabalTempTable();
	return S_OK;
}

void CHSEObj::FillTags()
{
	CString strID;

//	strID.Format("%d",ID_UPDATE_RES);
//	m_MapMenuIDTOTags.SetAt("UPDATE_RES",strID);
//	m_MapMenuIDTOTags.SetAt(strID,"UPDATE_RES");

	strID.Format("%d",ID_HSE_ACDNTENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTENT");

	strID.Format("%d",ID_HSE_STE);
	m_MapMenuIDTOTags.SetAt("HSE_TGSTE",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGSTE");

	strID.Format("%d",ID_HSE_SITE_TYPE);
	m_MapMenuIDTOTags.SetAt("HSE_TGSITETYPE",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGSITETYPE");

	strID.Format("%d",ID_HSE_CNTRY);
	//m_MapMenuIDTOTags.SetAt("HSE_TGCNTRY",strID);
	m_MapMenuIDTOTags.SetAt("CMN_COUNTRIES",strID);
	//m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCNTRY");
	m_MapMenuIDTOTags.SetAt(strID,"CMN_COUNTRIES");

	strID.Format("%d",ID_HSE_LANG);
	m_MapMenuIDTOTags.SetAt("HSE_TGLANG",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLANG");

	strID.Format("%d",ID_HSE_AUDITCHECKLISTRSLT);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTCHCKLSTRSLT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTCHCKLSTRSLT");

	strID.Format("%d",ID_HSE_AUDITCHECKLIST);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTCHCKLSTPLN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTCHCKLSTPLN");

	strID.Format("%d",ID_HSE_RJCT);
	m_MapMenuIDTOTags.SetAt("HSE_TGRjctRsn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRjctRsn");

	strID.Format("%d",ID_HSE_DPRT);
	//m_MapMenuIDTOTags.SetAt("HSE_TGDPRT",strID);
	m_MapMenuIDTOTags.SetAt("CMN_DEPART",strID);	
	//m_MapMenuIDTOTags.SetAt(strID,"HSE_TGDPRT");
	m_MapMenuIDTOTags.SetAt(strID,"CMN_DEPART");
	

	strID.Format("%d",ID_HSE_WRKLOC);
	m_MapMenuIDTOTags.SetAt("HSE_TGWRKLOC",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGWRKLOC");

	strID.Format("%d",ID_HSE_TSK);
	m_MapMenuIDTOTags.SetAt("HSE_TGTSK",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGTSK");

	strID.Format("%d",ID_HSE_CTRLMSRMNT);
	m_MapMenuIDTOTags.SetAt("HSE_TGHRCTRL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHRCTRL");

	strID.Format("%d",ID_HSE_HZRD);
	m_MapMenuIDTOTags.SetAt("HSE_TGHZRD",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHZRD");

	strID.Format("%d",ID_HSE_PERFMSRMNTCHKLIST);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTCHKLIST",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTCHKLIST");

	strID.Format("%d",ID_HSE_ACDNTRSN);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTRSN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTRSN");

	strID.Format("%d",ID_HSE_ADTORS);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNENTAUDITORS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNENTAUDITORS");

	strID.Format("%d",ID_HSE_ADTEES);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNENTAUDITEES",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNENTAUDITEES");

	strID.Format("%d",ID_HSE_HRMTYP);
	m_MapMenuIDTOTags.SetAt("HSE_TGHRMTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHRMTYP");

	strID.Format("%d",ID_HSE_DTL);
	m_MapMenuIDTOTags.SetAt("HSE_TGDTL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGDTL");

	strID.Format("%d",ID_HSE_DTLCPY);
	m_MapMenuIDTOTags.SetAt("HSE_TGCPYDTL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCPYDTL");

	strID.Format("%d",ID_HSE_CARMODELBAS);
	m_MapMenuIDTOTags.SetAt("HSE_TGCARMODELBAS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCARMODELBAS");

	strID.Format("%d",ID_HSE_ROL);
	m_MapMenuIDTOTags.SetAt("HSE_TGROL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGROL");

	strID.Format("%d",ID_HSE_VCLQSTNS);
	m_MapMenuIDTOTags.SetAt("HSE_TGVCLQSTNS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGVCLQSTNS");

	strID.Format("%d",ID_SYSTEMSETTINGS_BASICREFERENCE);
	m_MapMenuIDTOTags.SetAt("HSE_TGBSCREF",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGBSCREF");

	strID.Format("%d",ID_SYSTEMSETTINGS_ENVIRONMENTCONDITION);
	m_MapMenuIDTOTags.SetAt("HSE_TGENVRNMNTCND",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGENVRNMNTCND");

	strID.Format("%d",ID_SYSTEMSETTINGS_PERSONALPROTECTIVEEQUIPMENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGPRSNLPRTCTVEQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPRSNLPRTCTVEQ");

	strID.Format("%d",ID_HSE_PRSNLPRTCTVE_POP_UP);
	m_MapMenuIDTOTags.SetAt("HSE_TGPRSNLPRTCTVEQPMNT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPRSNLPRTCTVEQPMNT");

	strID.Format("%d",ID_SYSTEMSETTINGS_CURRENCY);
	//m_MapMenuIDTOTags.SetAt("HSE_TGCRRNCY",strID);
	m_MapMenuIDTOTags.SetAt("CMN_CURENCY",strID);
	//m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCRRNCY");
	m_MapMenuIDTOTags.SetAt(strID,"CMN_CURENCY");

	strID.Format("%d",ID_SYSTEMSETTINGS_SIGNATURES);
	m_MapMenuIDTOTags.SetAt("HSE_TGSGNTUR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGSGNTUR");

	strID.Format("%d",ID_SYSTEMSETTINGS_INJURYTYPE);
	m_MapMenuIDTOTags.SetAt("HSE_TGINJRYTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGINJRYTYP");

	strID.Format("%d",ID_HSE_INJUERYTYPE_POP_UP);
	m_MapMenuIDTOTags.SetAt("HSE_TGINJRYTP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGINJRYTP");

	strID.Format("%d",ID_SYSTEMSETTINGS_REWARDTYPE);
	m_MapMenuIDTOTags.SetAt("HSE_TGRWRDTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRWRDTYP");

	strID.Format("%d",ID_SYSTEMSETTINGS_POTENTIALSEVERITY);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTNTLSVRTY",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTNTLSVRTY");

	strID.Format("%d",ID_HSE_PERFMSRMNTSCOR);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTSCOR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTSCOR");

	strID.Format("%d",ID_HSE_ACDNTTYP);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTTYP");

	strID.Format("%d",ID_HSE_ACDNTMDM);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTMDM",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTMDM");

	strID.Format("%d",ID_HSE_TYPLOSS);
	m_MapMenuIDTOTags.SetAt("HSE_TGTYPLOSS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGTYPLOSS");

	strID.Format("%d",ID_HSE_HMNBODYPART);
	m_MapMenuIDTOTags.SetAt("HSE_TGHMNBODYPART",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHMNBODYPART");

	strID.Format("%d",ID_HSE_TSKADTRSLT);
	m_MapMenuIDTOTags.SetAt("HSE_TGTSKADTRSLT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGTSKADTRSLT");

	strID.Format("%d",ID_HSE_PTWSTYP);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTWSTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTWSTYP");

	strID.Format("%d",ID_HSE_RSKSVRTYRATCLS);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKSVRTRATCLS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKSVRTRATCLS");

	strID.Format("%d",ID_HSE_RSKLKLYHODRAT);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKLKLYHODRAT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKLKLYHODRAT");

	strID.Format("%d",ID_HSE_EMP);
	//m_MapMenuIDTOTags.SetAt("HSE_TGEMP",strID);
	m_MapMenuIDTOTags.SetAt("CMN_EMPLOYEE",strID);	
	//m_MapMenuIDTOTags.SetAt(strID,"HSE_TGEMP");
	m_MapMenuIDTOTags.SetAt(strID,"CMN_EMPLOYEE");	

	strID.Format("%d",ID_HSE_RSKRNKDESC);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKRNKDESC",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKRNKDESC");

	strID.Format("%d",ID_HSE_HSEPRCDRLIST);
	m_MapMenuIDTOTags.SetAt("HSE_TGHSEPRCDRLIST",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHSEPRCDRLIST");

	strID.Format("%d",ID_HSE_PTWSRSPNSBLTY);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTWSRSPNSBLTY",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTWSRSPNSBLTY");

	strID.Format("%d",ID_HSE_ENTRJCRSN);
	m_MapMenuIDTOTags.SetAt("HSE_TGENTRJCRSN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGENTRJCRSN");

	strID.Format("%d",ID_HSE_HSERGLR);
	m_MapMenuIDTOTags.SetAt("HSE_TGHSERGLR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHSERGLR");

	strID.Format("%d",ID_HSE_NCSSRYCTRLMSRMNT);
	m_MapMenuIDTOTags.SetAt("HSE_TGNCSSRYCTRLMSRMNT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNCSSRYCTRLMSRMNT");

	strID.Format("%d",ID_HSE_RSKASSMNTENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKASSMNTENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKASSMNTENT");

	strID.Format("%d",ID_HSE_RSKASSMNTAPR);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKASSMNTAPR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKASSMNTAPR");

	strID.Format("%d",ID_HSE_RSKASSMNTFOLLWUPDT);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKASSMNTFOLLWUPDT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKASSMNTFOLLWUPDT");

	strID.Format("%d",ID_HSE_AUDITORS);
	m_MapMenuIDTOTags.SetAt("HSE_TGAUDITORS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGAUDITORS");

	strID.Format("%d",ID_HSE_AUDITEES);
	m_MapMenuIDTOTags.SetAt("HSE_TGAUDITEES",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGAUDITEES");

	strID.Format("%d",ID_HSE_PTNLHZRDENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTNLHZRDENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTNLHZRDENT");

	strID.Format("%d",ID_HSE_PTNLHZRDCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTNLHZRDCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTNLHZRDCNFRMTN");

	strID.Format("%d",ID_HSE_PTNLHZRDFOLLOWUP);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTNLHZRDFOLLOWUP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTNLHZRDFOLLOWUP");

	strID.Format("%d",ID_HSE_LOSSACDNTENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGLOSSACDNTENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLOSSACDNTENT");

	strID.Format("%d",ID_HSE_LOSSACDNTCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGLOSSACDNTCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLOSSACDNTCNFRMTN");

	strID.Format("%d",ID_HSE_PERFMSRMNTENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTENT");

	strID.Format("%d",ID_HSE_PERFMSRMNTEVAL);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTEVAL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTEVAL");

	strID.Format("%d",ID_HSE_PERFMSRMNTFLWUP);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTFLWUP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTFLWUP");

	strID.Format("%d",ID_HSE_NRSTMISCENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGNRSTMISCENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNRSTMISCENT");

	strID.Format("%d",ID_HSE_NRSTMISCCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGNRSTMISCCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNRSTMISCCNFRMTN");

	strID.Format("%d",ID_HSE_NRSTMISCFLWUP);
	m_MapMenuIDTOTags.SetAt("HSE_TGNRSTMISCFLWUP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNRSTMISCFLWUP");

	strID.Format("%d",ID_NEARMISS_NEARMISSREWARD);
	m_MapMenuIDTOTags.SetAt("HSE_TgNrMisRward",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgNrMisRward");

	strID.Format("%d",ID_HSE_ACDNTCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTCNFRMTN");

	strID.Format("%d",ID_HSE_ACDNTFOLLWUPDT);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTFOLLWUPDT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTFOLLWUPDT");

	strID.Format("%d",ID_HSE_CRCTVEACCENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGCRCTVEACCENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCRCTVEACCENT");

	strID.Format("%d",ID_HSE_CRCTVEACCCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGCRCTVEACCCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCRCTVEACCCNFRMTN");

	strID.Format("%d",ID_HSE_CRCTVACTNFLWUPVSTS);
	m_MapMenuIDTOTags.SetAt("HSE_TGCARFLWUPVSTS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCARFLWUPVSTS");

	strID.Format("%d",ID_HSE_PTWSRGSTRENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTWSRGSTRENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTWSRGSTRENT");

	strID.Format("%d",ID_HSE_PTWSRGSTRCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTWSRGSTRCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTWSRGSTRCNFRMTN");

	strID.Format("%d",ID_HSE_ADTPLNENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNENT");

	strID.Format("%d",ID_HSE_ADTPLNAPR);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNAPR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNAPR");

	strID.Format("%d",ID_HSE_ADTPLNADTRSNADTES);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNADTRSANDADTS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNADTRSANDADTS");

	strID.Format("%d",ID_HSE_ADTPLNPRPERCHKLST);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNPRPRCHKLST",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNPRPRCHKLST");

	strID.Format("%d",ID_HSE_ADTPLNCHKLSTFDBCK);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNCHKLSTFDBCK",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNCHKLSTFDBCK");

	strID.Format("%d",ID_HSE_VCLACDNTENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGVCLACDNTENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGVCLACDNTENT");

	strID.Format("%d",ID_HSE_VCLACDNTCNFRMTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGVCLACDNTCNFRMTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGVCLACDNTCNFRMTN");

	strID.Format("%d",ID_HSE_VCLACDNTFLWUP);
	m_MapMenuIDTOTags.SetAt("HSE_TGVCLACDNTFLWUP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGVCLACDNTFLWUP");

	strID.Format("%d",ID_HSE_ATCHCLSFC);
	m_MapMenuIDTOTags.SetAt("HSE_TGATCHCLSFC",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGATCHCLSFC");

	strID.Format("%d",ID_HSE_HSEPLC);
	m_MapMenuIDTOTags.SetAt("HSE_TGHSEPLC",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHSEPLC");

	strID.Format("%d",ID_HSE_COMPPRF);
	m_MapMenuIDTOTags.SetAt("HSE_TGCOMPPRF",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCOMPPRF");

	strID.Format("%d",ID_HSE_HUMNBDYPRT);
	m_MapMenuIDTOTags.SetAt("HSE_TGHUMNBDYPRT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHUMNBDYPRT");

	strID.Format("%d",ID_HSE_PTNLHZRDINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTNLHZRDINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTNLHZRDINQ");

	strID.Format("%d",ID_HSE_RSKASSMNTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKASSMNTINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKASSMNTINQ");

	strID.Format("%d",ID_HSE_PERFMSRMNTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGPERFMSRMNTINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPERFMSRMNTINQ");

	strID.Format("%d",ID_HSE_ACDNTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGACDNTINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGACDNTINQ");

	strID.Format("%d",ID_HSE_ADTPLNINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTPLNINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTPLNINQ");

	strID.Format("%d",ID_HSE_CRCTVEACCINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGCRCTVEACCINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCRCTVEACCINQ");

	strID.Format("%d",ID_HSE_VCLACDNTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGVCLACDNTINQTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGVCLACDNTINQTN");

	strID.Format("%d",ID_HSE_NRSTMISCINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGNRSTMISCINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNRSTMISCINQ");

	strID.Format("%d",ID_HSE_PPTWINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGPTWSRGSTRINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPTWSRGSTRINQ");

	strID.Format("%d",ID_HSE_LOSSACCDNTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGLOSSACDNTINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLOSSACDNTINQ");

	strID.Format("%d",ID_HSE_HSEPROCDRSLSTINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TGHSEPRCDRLISTINQ",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGHSEPRCDRLISTINQ");

	strID.Format("%d",ID_RISKANALYSIS);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKANLYS",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKANLYS");

	strID.Format("%d",ID_HSE_TOOLEQUIP);
	m_MapMenuIDTOTags.SetAt("HSE_TGTOOLEQUIP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGTOOLEQUIP");

	strID.Format("%d",ID_HSE_TOOLSEQUIPDSTRBTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGTOOLSEQUIPDSTRBTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGTOOLSEQUIPDSTRBTN");

	
	strID.Format("%d",ID_HSE_SUBDETAILS);
	m_MapMenuIDTOTags.SetAt("HSE_TGSUBDTL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGSUBDTL");

	strID.Format("%d",ID_HSE_DTLFORCHKLIST);
	m_MapMenuIDTOTags.SetAt("HSE_TGANSWRDTL",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGANSWRDTL");

	strID.Format("%d",ID_HSE_NearMissTypes);
	m_MapMenuIDTOTags.SetAt("HSE_TGNrMisTypes",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGNrMisTypes");
	
	strID.Format("%d",ID_HSE_JobPtntlHzrdsLib);
	m_MapMenuIDTOTags.SetAt("HSE_TgPtntlHzrdsLib",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgPtntlHzrdsLib");

	strID.Format("%d",ID_HSE_PRECAUTIONS);
	m_MapMenuIDTOTags.SetAt("HSE_TGPrctns",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGPrctns");

	strID.Format("%d",ID_HSE_CNTCT_MTHDS);
	m_MapMenuIDTOTags.SetAt("CMN_CntctMthds",strID);
	m_MapMenuIDTOTags.SetAt(strID,"CMN_CntctMthds");

	strID.Format("%d",ID_HSE_ATCHMNTFRMTS);
	m_MapMenuIDTOTags.SetAt("CMN_AtchmntFrmt",strID);
	m_MapMenuIDTOTags.SetAt(strID,"CMN_AtchmntFrmt");

	strID.Format("%d",ID_HSE_TRADES);
	m_MapMenuIDTOTags.SetAt("CMN_TRADES",strID);
	m_MapMenuIDTOTags.SetAt(strID,"CMN_TRADES");

	strID.Format("%d",ID_HSE_FIRSTINFO);
	m_MapMenuIDTOTags.SetAt("HSE_TGFirstInfo",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGFirstInfo");

	strID.Format("%d",ID_HSE_DOCTYP);
	m_MapMenuIDTOTags.SetAt("CMN_DOCTYP",strID);
	m_MapMenuIDTOTags.SetAt(strID,"CMN_DOCTYP");

	strID.Format("%d",ID_HSE_ADTACTVTRQRDACTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGADTACTVTRQRDACTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGADTACTVTRQRDACTN");

	strID.Format("%d",ID_HSE_RSKSVRTYCNTRLMSR);
	m_MapMenuIDTOTags.SetAt("HSE_TGRSKSVRTYCNTRLMSR",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGRSKSVRTYCNTRLMSR");

	strID.Format("%d",ID_HSE_AdtChkLstPopUp);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtChkLstPopUp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtChkLstPopUp");

	strID.Format("%d",ID_HSE_Prjcts);
	m_MapMenuIDTOTags.SetAt("HSE_TgPrjcts",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgPrjcts");

	strID.Format("%d",ID_HSE_AdtTyp);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtTyp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtTyp");
	
	strID.Format("%d",ID_HSE_RskRat);
	m_MapMenuIDTOTags.SetAt("HSE_TgRskRat",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgRskRat");

	strID.Format("%d",ID_HSE_IncdntPrd);
	m_MapMenuIDTOTags.SetAt("HSE_TgIncdntPrd",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgIncdntPrd");

	strID.Format("%d",ID_HSE_JobStp);
	m_MapMenuIDTOTags.SetAt("HSE_TgJobStp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgJobStp");

	strID.Format("%d",ID_HSE_Cnsqncs);
	m_MapMenuIDTOTags.SetAt("HSE_TgCnsqncs",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgCnsqncs");

	strID.Format("%d",ID_HSE_AdtGrp);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtGrp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtGrp");

	strID.Format("%d",ID_HSE_AdtChcklst);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtChcklst",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtChcklst");

	strID.Format("%d",ID_HSE_SitSrvyEnt);
	m_MapMenuIDTOTags.SetAt("HSE_TgSitSrvyEnt",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgSitSrvyEnt");

	strID.Format("%d",ID_HSE_SitSrvyCnfrmtn);
	m_MapMenuIDTOTags.SetAt("HSE_TgSitSrvyCnfrmtn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgSitSrvyCnfrmtn");

	strID.Format("%d",ID_HSE_SitSrvyFlwup);
	m_MapMenuIDTOTags.SetAt("HSE_TgSitSrvyFlwup",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgSitSrvyFlwup");

	strID.Format("%d",ID_HSE_AdtRschdl);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtRschdl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtRschdl");

	strID.Format("%d",ID_HSE_AdtRschdlBrws);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtRschdlBrws",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtRschdlBrws");

	strID.Format("%d",ID_HSE_AdtCncl);
	m_MapMenuIDTOTags.SetAt("HSE_TgAdtCncl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAdtCncl");

	strID.Format("%d",ID_HSE_IncdntFlshRprt);
	m_MapMenuIDTOTags.SetAt("HSE_TgIncdntFlshRprt",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgIncdntFlshRprt");

	strID.Format("%d",ID_HSE_RltdOprtns);
	m_MapMenuIDTOTags.SetAt("HSE_TgRltdOprtns",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgRltdOprtns");

	strID.Format("%d",ID_HSE_MtrlRlsTyp);
	m_MapMenuIDTOTags.SetAt("HSE_TgMtrlRlsTyp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgMtrlRlsTyp");

	strID.Format("%d",ID_HSE_CntnmntTyp);
	m_MapMenuIDTOTags.SetAt("HSE_TgCntnmntTyp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgCntnmntTyp");

	strID.Format("%d",ID_HSE_MtrlRlsCat);
	m_MapMenuIDTOTags.SetAt("HSE_TgMtrlRlsCat",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgMtrlRlsCat");

	strID.Format("%d",ID_HSE_RlsMtrl);
	m_MapMenuIDTOTags.SetAt("HSE_TgRlsMtrl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgRlsMtrl");

	strID.Format("%d",ID_HSE_UOM);
	m_MapMenuIDTOTags.SetAt("HSE_TgUOM",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgUOM");

	strID.Format("%d",ID_HSE_MtrlRlsTo);
	m_MapMenuIDTOTags.SetAt("HSE_TgMtrlRlsTo",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgMtrlRlsTo");

	strID.Format("%d",ID_HSE_SplClsfctn);
	m_MapMenuIDTOTags.SetAt("HSE_TgSplClsfctn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgSplClsfctn");

	strID.Format("%d",ID_HSE_AstTyp);
	m_MapMenuIDTOTags.SetAt("HSE_TgAstTyp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgAstTyp");
	
	strID.Format("%d",ID_HSE_SITSRVYINQ);
	m_MapMenuIDTOTags.SetAt("HSE_TgSitSrvyInq",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgSitSrvyInq");


	strID.Format("%d",ID_HSE_HRCTRLBDTLRWS);
	m_MapMenuIDTOTags.SetAt("HSE_TgLogsDfntn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgLogsDfntn");


	strID.Format("%d",ID_HSE_LOGS_DEF);
	m_MapMenuIDTOTags.SetAt("HSE_TgHRCTRLBrws",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TgHRCTRLBrws");

	
	strID.Format("%d",ID_HSE_LOGS_ENT);
	m_MapMenuIDTOTags.SetAt("HSE_TGLOGSENT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLOGSENT");


	strID.Format("%d",ID_HSE_LOGS_ENT_DTL_CNT);
	m_MapMenuIDTOTags.SetAt("HSE_TGLOGSENTDTLCNT",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGLOGSENTDTLCNT");

	strID.Format("%d",ID_HSE_JOB_STP_DFNTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGJOBSTPDFNTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGJOBSTPDFNTN");


	strID.Format("%d",ID_HSE_CNSQ_DFNTN);
	m_MapMenuIDTOTags.SetAt("HSE_TGCNSQNCSDFNTN",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_TGCNSQNCSDFNTN");


	//HSE_RV////////////////////////////////////////////
	strID.Format("%d",HSE_ASPECT_TYPE);
	m_MapMenuIDTOTags.SetAt("HSE_AspctTyp",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctTyp");

	strID.Format("%d",HSE_ASPECT_ACTIVITY);
	m_MapMenuIDTOTags.SetAt("HSE_AspctActvty",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctActvty");

	strID.Format("%d",HSE_ASPECT_FREQUENCY);
	m_MapMenuIDTOTags.SetAt("HSE_AspctFrquncy",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctFrquncy");

	strID.Format("%d",HSE_ASPECT_UOM);
	m_MapMenuIDTOTags.SetAt("HSE_AspctUOM",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctUOM");

	strID.Format("%d",HSE_ASPECT_STATE);
	m_MapMenuIDTOTags.SetAt("HSE_AspctStt",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctStt");

	strID.Format("%d",HSE_CHEMICAL_ITEM_FREQUENCY);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclItmFrquncy",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclItmFrquncy");

	strID.Format("%d",HSE_CHEMICAL_HAZARD_HEALTH);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclHzrdHlth",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclHzrdHlth");

	strID.Format("%d",HSE_CHEMICAL_HAZARD_FLAMMABILITY);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclHzrdFlmmblty",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclHzrdFlmmblty");

	strID.Format("%d",HSE_CHEMICAL_HAZARD_RE_ACTIVITY);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclHzrdR_Actvty",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclHzrdR_Actvty");

	strID.Format("%d",HSE_CHEMICAL_HAZARD_SPECIAL);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclHzrdSpcl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclHzrdSpcl");

	strID.Format("%d",HSE_DRILL_TYPE);
	m_MapMenuIDTOTags.SetAt("HSE_DrllTyp_VW",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_DrllTyp_VW");

	strID.Format("%d",HSE_EXTERNAL_SUPPORT);
	m_MapMenuIDTOTags.SetAt("HSE_ExtrnlSupprt",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ExtrnlSupprt");

	strID.Format("%d",HSE_EMT_LIST);
	m_MapMenuIDTOTags.SetAt("HSE_EMTLst",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_EMTLst");

	strID.Format("%d",HSE_DRILL_ROLE);
	m_MapMenuIDTOTags.SetAt("HSE_DrllRl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_DrllRl");


	strID.Format("%d",HSE_INTERNAL_RESOURCE);
	m_MapMenuIDTOTags.SetAt("HSE_IntrnlRsurc",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_IntrnlRsurc");

	strID.Format("%d",HSE_ASPECTS_ENTRY_ENTRY);
	m_MapMenuIDTOTags.SetAt("HSE_AspctsEntryAtEntry",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctsEntryAtEntry");


	strID.Format("%d",HSE_ASPECTS_REVIEW_REVIEW);
	m_MapMenuIDTOTags.SetAt("HSE_AspctsRvwAtRvw",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_AspctsRvwAtRvw");

	strID.Format("%d",HSE_CHEMICAL_HAZARD);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclHzrd",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclHzrd");

	strID.Format("%d",HSE_CHEMICAL_ITEMS_DEFINITION);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclItmsDfntn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclItmsDfntn");

	strID.Format("%d",HSE_CHEMICAL_REGISTER);
	m_MapMenuIDTOTags.SetAt("HSE_ChmclRgstr",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_ChmclRgstr");

	strID.Format("%d",HSE_DRILL_PLAN_ENTRY_ENTRY);
	m_MapMenuIDTOTags.SetAt("HSE_DrllPlnEntryAtEntry",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_DrllPlnEntryAtEntry");

	strID.Format("%d",HSE_DRILL_PLAN_APPROVAL_APPROVAL);
	m_MapMenuIDTOTags.SetAt("HSE_DrllPlnApprvlAtApprvl",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_DrllPlnApprvlAtApprvl");

	strID.Format("%d",HSE_DRILL_EXECUTION_EXECUTION);
	m_MapMenuIDTOTags.SetAt("HSE_DrllExcutnAtExcutn",strID);
	m_MapMenuIDTOTags.SetAt(strID,"HSE_DrllExcutnAtExcutn");
	////////////////////////////////////////////////////
}

STDMETHODIMP CHSEObj::MapUserFromApp(BSTR bstrName, int nAppID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_strLoginUserName = CString(bstrName);
	m_User = (short)m_Connection.m_pPrivConnection->GetUserIDForApp(nAppID,CString(bstrName),APP_CODE);
	m_strUserFullName = m_Connection.m_pPrivConnection->GetUserFullName(m_User);
	return S_OK;
}

STDMETHODIMP CHSEObj::GetConnectionString(BSTR *bstrConnection)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*bstrConnection = m_strConnection.AllocSysString();
	return S_OK;
}

STDMETHODIMP CHSEObj::GetUserName(BSTR *bstrName)
{
	CString strData;
	strData.Format("%s >>> %s",m_strLoginUserName,m_strUserFullName);
	*bstrName = strData.AllocSysString();
	return S_OK;
}
STDMETHODIMP CHSEObj::GetHelpFileName(BSTR *bstrFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	CString strFile = "HSEMSENGLISHHELP.HLP";
	if(IsArabic())
	{
		strFile = "HSEMSARABICHELP.HLP";
	}
	CString strFolderName=m_Connection.m_pSetupConnection->GetWorkingFolder();
	CString strHelpFileName;
	strHelpFileName.Format("%s\\HELP\\%s",strFolderName,strFile);
	m_strHelpFileName  = strHelpFileName;
	*bstrFolder = m_strHelpFileName.AllocSysString();
	return S_OK;
}

void CHSEObj::CreateRegHandle()
{
	if (m_pRegHandle == NULL)
	{
		m_pRegHandle.CreateInstance(__uuidof(RegHandle));//create instance from registry handle
		m_pRegHandle->SetSettingMode(WM_DB);
		m_pRegHandle->SetProfileName(APP_NAME);
	}
	m_pRegHandle->SetConnectionString(m_strConnection.AllocSysString());
}

_bstr_t CHSEObj::ShowNotepad (_bstr_t bstrValue,_bstr_t bstrEnCaption,enum TextStyle nStyle,int nType,BOOL bLocked,COLORREF bkColor)
{
	if(m_pNotepadeObj ==NULL) 
	{
		HRESULT hr =m_pNotepadeObj.CreateInstance(__uuidof(FNotepadObj));
		if(hr)
			return "";
	}
	try
	{
		return m_pNotepadeObj->ShowNotepad (bstrValue,bstrEnCaption,nStyle,nType,bLocked,RGB(255,255,255));
	}
	catch(_com_error &e)
	{
		_bstr_t str_t = e.Description();
		return "";
	}
}

int CHSEObj::ShowLoginScreen()
{
	return m_Connection.m_pSecurityConnection->ShowLogScreen(STR_APP_CODE,TRUE,IsArabic(),GlstrVersionNo);
}

BOOL CHSEObj::IsAdminUser(int nUserID)
{
	return m_Connection.m_pSecurityConnection->IsAdminUser(nUserID);
}

STDMETHODIMP CHSEObj::ShowAutoMailSetupScreen()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_DB*	pPolyDb = new _DB();
	CHSEObj::DBInit(pPolyDb,NULL);
	try
	{
		pPolyDb->ShowAutoMailSetupScreen(0);
	}
	catch(_com_error& e)
	{
		CString strErrorDiscription = (char*)e.Description();
		AfxMessageBox(strErrorDiscription );
	}

	return S_OK;
}

STDMETHODIMP CHSEObj::ShowMailWizard()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_DB*	pPolyDb = new _DB();
	CHSEObj::DBInit(pPolyDb,NULL);
	try
	{
		pPolyDb->ShowMailWizard();
	}
	catch(_com_error& e)
	{
		CString strErrorDiscription = (char*)e.Description();
		AfxMessageBox(strErrorDiscription );
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowMailServerSetting()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_DB*	pPolyDb = new _DB();
	CHSEObj::DBInit(pPolyDb,NULL);
	try
	{
		pPolyDb->ShowMailServerSetting();
	}
	catch(_com_error& e)
	{
		CString strErrorDiscription = (char*)e.Description();
		AfxMessageBox(strErrorDiscription );
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::SetFormRect(int nLeft, int nTop, int nRight, int nBottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	m_rcFormRect.CopyRect(CRect(nLeft, nTop, nRight, nBottom));
	if(m_pCategoryList)
	{
		for (int nCounter=0;nCounter < m_pCategoryList->GetCount();nCounter ++)
		{			
			try
			{
				POSITION pos = m_pCategoryList->FindIndex(nCounter);
				CCategory * pCategory=(CCategory *)m_pCategoryList->GetNext(pos);
				pCategory->SetFormRect(m_rcFormRect);
				pCategory->GetMyCCategory()->MovePolydbForm();
			}
			catch(...)
			{
			}			
		}
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::ActivateForm(BSTR bstrFormID,BOOL bByTag, int nSkipNo,BOOL bActivateLoadedScreen)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strFormID(bstrFormID);

	CCategory* pListCategory;
	int nItemFound = 0;
	if(m_pCategoryList)
	{
		int lCount=m_pCategoryList->GetCount();
		for (int nCounter=0;nCounter < m_pCategoryList->GetCount();nCounter ++)
		{
			POSITION pos = m_pCategoryList->FindIndex(nCounter);
			CCategory * pCategory=(CCategory *)m_pCategoryList->GetNext(pos);

			if (bByTag)
			{
				if (pCategory->GetMyCCategory()->GetFormTag() == strFormID)
				{
					// Get List Category
					if (nItemFound == 0)
						pListCategory = pCategory;

					if (nItemFound == nSkipNo)
					{
						if (bActivateLoadedScreen)
						{
							if (nSkipNo == 1)
							{
								// Get Current KeyValue Criteria from List Screen
								CString strCriteria = pListCategory->GetMyCCategory()->GetSelectedRecordCriteria();
								// Move to the correct record in 1:Many screen
								pCategory->GetMyCCategory()->FindRecord(strCriteria);
							}
								//Get FastSearch member here and move to this record
							pCategory->GetMyCCategory()->BringWindowToTop();
						}
						return true;
					}
					nItemFound++;	
				}
			}
			else // Called from EXE 
			{
				if (pCategory->GetMyCCategory()->GetScreenCaption() == strFormID)
				{
					pCategory->GetMyCCategory()->BringWindowToTop();
					return true;
				}
			}
		}
	}

	return false;
}

STDMETHODIMP CHSEObj::SetFirstDesktopObj(LPDISPATCH lpFirstDesktopObj)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	gpFirstDesktopObj = (IFirstDesktopPtr)lpFirstDesktopObj;

	return S_OK;
}

STDMETHODIMP CHSEObj::SetFastSearchCriteria(BSTR bstrCriteria)
{
	m_strFastSearchCriteria = bstrCriteria;
	return S_OK;
}

STDMETHODIMP CHSEObj::GetUserID()
{
	return m_User;
}

bool CHSEObj::IsMenuEnabled(long lMenuID)
{
	return m_Connection.m_pPrivConnection->MenuVerify(STR_APP_CODE, m_User,lMenuID);
}

HRESULT CHSEObj::SetCurrentMenuItemID(UINT unMenuID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_unCurMenuID=unMenuID;

	return S_OK;
}

STDMETHODIMP CHSEObj::GetDBType()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	short sRetVal= m_Connection.m_pSetupConnection->GetApplicationDBType();
	return sRetVal;
}

bool CHSEObj::ValidateDongle()
{
	CoInitialize(NULL); //initialize com
	IRegHandlePtr pRegHandle;
	HRESULT hr = pRegHandle.CreateInstance(__uuidof(RegHandle));//create instance from registry handle
	BOOL bRetVal = TRUE;
	if(hr!=0)
	{
		MessageBox(NULL,"Cannot initialize RegistryHandle","Prompt",0);
		return bRetVal;
	}
	pRegHandle->SetSettingMode(WM_XML);
	pRegHandle->SetProfileName (APP_NAME);
	CString strKeyVal=(LPCSTR)pRegHandle->GetSetting ((LPCSTR)"Software\\Microsoft",(LPCSTR)"EventSystemFirst",(LPCSTR)"VAL",(LPCSTR)"");
	if(strKeyVal=="fdhSFhds JLJAFDJFLKSDJKL opIOp JJLJ _jklewoipR432EwkrLKEwj jflekf d4u2394fdsk djf94un5kjlk")
		bRetVal=false;
	else
	{
		pRegHandle->SaveSetting ((LPCSTR)"Software\\Microsoft",(LPCSTR)"EventSystemFirst",(LPCSTR)"VAL",(LPCSTR)"");
		bRetVal =true;
	}
	
	pRegHandle.Release();
	return bRetVal;
}

CString CHSEObj::GetWorkingFolder()
{
	return m_Connection.m_pSetupConnection->GetWorkingFolder();
}

STDMETHODIMP CHSEObj::ReportsReportsList()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_ANALYSIS_REPORTS);	

	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	AFX_MODULE_STATE*m_pPrevModuleState = AfxSetModuleState(AfxGetStaticModuleState());

	CAnalysisReports *pAnalysis=new CAnalysisReports(pCategory);
	pAnalysis->ReportsList(IsArabic());

	AfxSetModuleState(m_pPrevModuleState);

	return S_OK;
}

STDMETHODIMP CHSEObj::AnalysisReportTypes()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_ANALYSIS_REPORTTYPES);	

	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	
	CAnalysisReports  *pAnalysis=new CAnalysisReports (pCategory);
	pAnalysis->ReportTypes();

	return S_OK;
}

STDMETHODIMP CHSEObj::UtilitiesCodeSegments(BSTR strAppCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_UTILITIES_APPLICATIONSETUP);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	CUtility *pUtility = new CUtility(pCategory);
	pUtility->DefineCodeMethod();
	return S_OK;
}

STDMETHODIMP CHSEObj::UtilitiesUserDefinedChecks()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_UTILITIES_USERDEFINEDCHECKS);

	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	
	CUtility *pUtility=new CUtility(pCategory);
	pUtility->DefineUserDefinedChecks();
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowScreenFromAttachment(BSTR bstrAttachmentFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	UINT unMenuId;
	CString strAttachmentFile(bstrAttachmentFile),strMenuId;

	IRegHandlePtr RegObj;
	RegObj.CreateInstance (__uuidof(RegHandle));
	
	RegObj->SetSettingMode(WM_INI);
	RegObj->SetProfileName(strAttachmentFile.AllocSysString());

	CString strScreenTag		= (LPCSTR)RegObj->GetSetting (APP_NAME,"","FORM_TAG","");
	CString strCriteria			= (LPCSTR)RegObj->GetSetting (APP_NAME,"","CRITERIA","");
	CString strSubject			= (LPCSTR)RegObj->GetSetting (APP_NAME,"","SUBJECT","");
	CString strAppCode			= (LPCSTR)RegObj->GetSetting (APP_NAME,"","APP_CODE","");
	CString strButtonID			= (LPCSTR)RegObj->GetSetting (APP_NAME,"","BUTTON_ID","");
	
	strScreenTag.MakeUpper();
	
	/*GET OPEND SCREEN TAG FROM DATA BASE*/
	CString strOpendScreenTag;
	CString strSQL;
	strSQL.Format("SELECT OPEND_TAG FROM CMN_MAILROUTE WHERE APP_CODE=%s AND FORM_TAG='%s' AND BUTTON_ID ='%s'",strAppCode,strScreenTag,strButtonID);
	CPolyDbRecordset DaoRecSet;
	DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if(!DaoRecSet.IsEOF())
		DaoRecSet.GetFieldValue("OPEND_TAG",strOpendScreenTag);
	else
		strOpendScreenTag	=	strScreenTag;

	DaoRecSet.Close();

	if(m_MapMenuIDTOTags.Lookup(strOpendScreenTag,strMenuId))
	{
		
		PrepareBeforeOpenAttachmet(strOpendScreenTag,strCriteria,strSubject);

		unMenuId=atol(strMenuId);
		
		m_pParentWnd->SendMessage(WM_COMMAND,MAKEWPARAM(unMenuId,0),NULL);

	}	
	return S_OK;
}

BOOL CHSEObj::PrepareBeforeOpenAttachmet(CString strScreenTag,CString strCriteria,CString strSubject)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if(!strScreenTag.IsEmpty())
	{
		_DB*	pPolyDb = new _DB();
		CHSEObj::DBInit(pPolyDb,NULL);
		try
		{
			pPolyDb->CreateFilter(strScreenTag,strSubject,strCriteria,TRUE);
		}
		catch(_com_error& e)
		{
			CString strErrorDiscription = (char*)e.Description();
			AfxMessageBox(strErrorDiscription );
		}

	}
	return TRUE;
}

long CHSEObj::ShowPopupEntry(BSTR bstrFormTag,BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(0);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPopupEntry(CString(bstrFormTag),CString(bstrCriteria));
	return S_OK;
}


long CHSEObj::ShowAwrnsCrs(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AwrnsCrs);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAwrnsCrs(CString(bstrCriteria));
	return S_OK;
}


//Awareness Plan Entry
STDMETHODIMP CHSEObj::ShowAwrnsPlnEntry(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AwrnsPlnEntry);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AwrnsPlnEntryCategory(pCategory);
	((AwrnsPlnEntryCategory *)pStaticInfo)->ShowAwrnsPlnEntry(CString(bstrCriteria));
	return S_OK;
}

//Awareness Plan Approval
STDMETHODIMP CHSEObj::ShowAwrnsPlnAprvl(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AwrnsPlnAprvl);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AwrnsPlnAprvlCategory(pCategory);
	((AwrnsPlnAprvlCategory *)pStaticInfo)->ShowAwrnsPlnAprvl(CString(bstrCriteria));
	return S_OK;
}

//Awareness Plan Execution
STDMETHODIMP CHSEObj::ShowAwrnsPlnExctn(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AwrnsPlnExctn);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AwrnsPlnExctnCategory(pCategory);
	((AwrnsPlnExctnCategory *)pStaticInfo)->ShowAwrnsPlnExctn(CString(bstrCriteria));
	return S_OK;
}

//Awareness Plan Inquiry
STDMETHODIMP CHSEObj::ShowAwrnsPlnInq(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AwrnsPlnInq);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AwrnsPlnInqCategory(pCategory);
	((AwrnsPlnInqCategory *)pStaticInfo)->ShowAwrnsPlnInq(CString(bstrCriteria));
	return S_OK;
}

//Course Attendees
STDMETHODIMP CHSEObj::ShowAwrnsPlnCrsAtnds(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AWRNSPLN_CRS_Atnds);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	/////////////////////////////////////////////////////////////////////////////////////////
	//RQ-3-2021.21
	/*CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAwrnsPlnCrsAtnds(CString(bstrCriteria));*/
	CCommonCategoryWrapper *pStaticInfo = new AwrnsPlnCategory(pCategory);
	((AwrnsPlnCategory *)pStaticInfo)->ShowAwrnsPlnCrsAtnds(CString(bstrCriteria));
	/////////////////////////////////////////////////////////////////////////////////////////
	return S_OK;
}

//Survey Type
STDMETHODIMP CHSEObj::ShowSitSrvyTyps(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_Srvy_Typs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSitSrvyTyps(CString(bstrCriteria));
	return S_OK;
}

//Reasons
/*
STDMETHODIMP CHSEObj::ShowCMSRsns(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_CMS_Rsns);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CReasonsCategory(pCategory);
	((CReasonsCategory *)pStaticInfo)->ShowCMSRsns(CString(bstrCriteria));
	return S_OK;
}
*/
//Action Reasons
STDMETHODIMP CHSEObj::ShowCMSActnRsn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_CMS_ActnRsn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCMSActnRsn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAccidentEntry(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_HSE_ACDNTENT);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	CCommonCategoryWrapper *pStaticInfo = new AccidentEntryCategory(pCategory);
	((AccidentEntryCategory *)pStaticInfo)->ShowAccidentEntry(CString(bstrCriteria));
	return S_OK;
}

//Rescue Entities
STDMETHODIMP CHSEObj::ShowRscuEntts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuEntts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRscuEntts(CString(bstrCriteria));
	return S_OK;
}

//Confined Space Types
//Update "HSEMS New requirements_27_10_2020" : Screen name changed to : Rescue Types
STDMETHODIMP CHSEObj::ShowCnfndSpcTyps(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CnfndSpcTyps);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCnfndSpcTyps(CString(bstrCriteria));
	return S_OK;
}

//Rescue Hazards
STDMETHODIMP CHSEObj::ShowRscuHzrds(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuHzrds);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRscuHzrds(CString(bstrCriteria));
	return S_OK;
}

//Rescue Tools
STDMETHODIMP CHSEObj::ShowRscuTls(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuTls);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRscuTls(CString(bstrCriteria));
	return S_OK;
}

//Rescue Communication Methods
STDMETHODIMP CHSEObj::ShowRscuCmmunctnMthds(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuCmmunctnMthds);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRscuCmmunctnMthds(CString(bstrCriteria));
	return S_OK;
}

//Rescue Plan Entry
STDMETHODIMP CHSEObj::ShowRscuPlnEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuPlnEntry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new RscuPlnCategory(pCategory);
	((RscuPlnCategory *)pStaticInfo)->ShowRscuPlnEntry(CString(bstrCriteria));
	return S_OK;
}

//Rescue Plan Inquiry
STDMETHODIMP CHSEObj::ShowRscuPlnInqury(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RscuPlnInqury);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new RscuPlnCategory(pCategory);
	((RscuPlnCategory *)pStaticInfo)->ShowRscuPlnInqury(CString(bstrCriteria));
	return S_OK;
}

//Hazard Process
STDMETHODIMP CHSEObj::ShowHzrdPrcs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_Hzrd_Procs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHzrdPrcs(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSite(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_STE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSite(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSiteType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SITE_TYPE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSiteType(CString(bstrCriteria));
	return S_OK;
}

//Contractors
STDMETHODIMP CHSEObj::ShowHSECntrctr(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CNTRCTR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSECntrctr(CString(bstrCriteria));
	return S_OK;
}

//Life Saving Rules
STDMETHODIMP CHSEObj::ShowLfSavngRls(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LfSavngRls);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowLfSavngRls(CString(bstrCriteria));
	return S_OK;
}

//Observation Targets
STDMETHODIMP CHSEObj::ShowObsrvtnTrgts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ObsrvtnTrgts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowObsrvtnTrgts(CString(bstrCriteria));
	return S_OK;
}

//ERT List
STDMETHODIMP CHSEObj::ShowHSEERTList(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ERTList);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEERTList(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowInvlvdPrts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_INVLVDPRTS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowInvlvdPrts(CString(bstrCriteria));
	return S_OK;
}

//Environmental Measures - recommended CAR's
STDMETHODIMP CHSEObj::ShowEnvMsrEntryPoltantPrpCAR(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMsrEntryPoltantPrpCAR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEnvMsrEntryPoltantPrpCAR(CString(bstrCriteria));
	return S_OK;
}

//Packing Methods
STDMETHODIMP CHSEObj::ShowPckngMthds(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PckngMthds);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPckngMthds(CString(bstrCriteria));
	return S_OK;
}

//Waste Items
STDMETHODIMP CHSEObj::ShowWstItms(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstItms);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowWstItms(CString(bstrCriteria));
	return S_OK;
}

//Waste Zones
STDMETHODIMP CHSEObj::ShowWstZns(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstZns);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowWstZns(CString(bstrCriteria));
	return S_OK;
}

//Step Effect
STDMETHODIMP CHSEObj::ShowHSEStpEffct(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_StpEffct);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEStpEffct(CString(bstrCriteria));
	return S_OK;
}

//Risk Level
STDMETHODIMP CHSEObj::ShowHSERskLvl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RskLvl);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSERskLvl(CString(bstrCriteria));
	return S_OK;
}

//Top Events
STDMETHODIMP CHSEObj::ShowHSETopEvnts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TopEvnts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSETopEvnts(CString(bstrCriteria));
	return S_OK;
}

//Barriers
STDMETHODIMP CHSEObj::ShowHSETopEvntsThrtsBrs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TopEvntsThrtsBrs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSETopEvntsThrtsBrs(CString(bstrCriteria));
	return S_OK;
}

//Measure Required Actions Pop-up screen
STDMETHODIMP CHSEObj::ShowHSEEnvmntrRqrdActn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvmntrRqrdActn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEEnvmntrRqrdActn(CString(bstrCriteria));
	return S_OK;
}

//waste transporters
STDMETHODIMP CHSEObj::ShowWstTrnsprtrs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstTrnsprtrs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowWstTrnsprtrs(CString(bstrCriteria));
	return S_OK;
}

//waste treatment plants
STDMETHODIMP CHSEObj::ShowWstTrtmntPlnts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstTrtmntPlnts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowWstTrtmntPlnts(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEIncdntMdclRprt(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntMdclRprt);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEIncdntMdclRprt(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowCountry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CNTRY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCountry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowLanguage(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LANG);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowLanguage(CString(bstrCriteria));
	return S_OK;
}

//Waste Receiving Entry
STDMETHODIMP CHSEObj::ShowWstRcvngEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstRcvngEntry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new WstRcvngEntCategory(pCategory);
	((WstRcvngEntCategory *)pStaticInfo)->ShowWstRcvngEntry(CString(bstrCriteria));
	return S_OK;
}

//Waste Disposal Entry
STDMETHODIMP CHSEObj::ShowWstDspslEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WstDspslEntry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new WstDspslEntCategory(pCategory);
	((WstDspslEntCategory *)pStaticInfo)->ShowWstDspslEntry(CString(bstrCriteria));
	return S_OK;
}

//Environmental Monitoring Activities
STDMETHODIMP CHSEObj::ShowEnvMntrActvts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMntrActvts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEnvMntrActvts(CString(bstrCriteria));
	return S_OK;
}

//Environmental Measure Types
STDMETHODIMP CHSEObj::ShowEnvMsrTyps(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMsrTyps);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEnvMsrTyps(CString(bstrCriteria));
	return S_OK;
}

//Environmental Monitoring Plan
STDMETHODIMP CHSEObj::ShowEnvMntrPlan(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMntrPlan);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvMntrPlanCategory(pCategory);
	((EnvMntrPlanCategory *)pStaticInfo)->ShowEnvMntrPlan(CString(bstrCriteria));
	return S_OK;
}

//Environmental Monitoring Execution
STDMETHODIMP CHSEObj::ShowEnvMntrExctn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMntrExctn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvMntrExctnCategory(pCategory);
	((EnvMntrExctnCategory *)pStaticInfo)->ShowEnvMntrExctn(CString(bstrCriteria));
	return S_OK;
}

//Environmental Measures Entry
STDMETHODIMP CHSEObj::ShowEnvMsrEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMsrEntry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvMsrEntryCategory(pCategory);
	((EnvMsrEntryCategory *)pStaticInfo)->ShowEnvMsrEntry(CString(bstrCriteria));
	return S_OK;
}

//Environmental Measures Approval
STDMETHODIMP CHSEObj::ShowEnvMsrAprvl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMsrAprvl);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvMsrAprvlCategory(pCategory);
	((EnvMsrAprvlCategory *)pStaticInfo)->ShowEnvMsrAprvl(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAuditCheckListPlan(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AUDITCHECKLISTRSLT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditCheckListPlanCategory(pCategory);
	((AuditCheckListPlanCategory *)pStaticInfo)->ShowAuditCheckListPlan(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAuditCheckListResult(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AUDITCHECKLIST);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditCheckListResultCategory(pCategory);
	((AuditCheckListResultCategory *)pStaticInfo)->ShowAuditCheckListResult(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRejectreason(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RJCT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CRejectReason(pCategory);
	((CRejectReason *)pStaticInfo)->ShowRejectreason(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAcceptreason(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RJCT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CAcceptReason(pCategory);
	((CAcceptReason *)pStaticInfo)->ShowAcceptreason(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowApprovReason(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RJCT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CApproveCAR(pCategory);
	((CApproveCAR *)pStaticInfo)->ShowApproveCAR(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDepartment(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DPRT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDepartment(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEJobStpDfntn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_JOB_STP_DFNTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEJobStpDfntn(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSECnsqncsDfntn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CNSQ_DFNTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSECnsqncsDfntn(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowDrlFndngs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DrlFndngs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDrlFndngs(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDrlFndngsUnPlndScnros(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DrlFndngsUnPlndScnros);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDrlFndngsUnPlndScnros(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDrillInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_Drill_Inquiry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new DrillEntryCategory(pCategory);
	((DrillEntryCategory *)pStaticInfo)->ShowDrillInquiry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowVclTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCL_TYPE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowVclTyp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSitSrvyRqrdActn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SITSRVY_RQRDACTN_POPUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new SitSrvyRqrdActn(pCategory);
	((SitSrvyRqrdActn *)pStaticInfo)->ShowSitSrvyRqrdActn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowWorkLocation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_WRKLOC);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowWorkLocation(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowMsnRsLog(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MSNRSLOG);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowMsnRsLog(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowTask(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TSK);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new JobCategory(pCategory);
	((JobCategory *)pStaticInfo)->ShowJob(CString(bstrCriteria));	//screen name changed from task to job
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowControlMeasures(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CTRLMSRMNT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHierarchyControl(pCategory);
	((CHierarchyControl *)pStaticInfo)->ShowControlMeasures(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHazard(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HZRD);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new HazardCategory(pCategory);
	((HazardCategory *)pStaticInfo)->ShowHazard(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPerformanceMeasurementCheckList(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTCHKLIST);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PrformnceMsurmntChcklistCategory(pCategory);
	((PrformnceMsurmntChcklistCategory *)pStaticInfo)->ShowPerformanceMeasurementCheckList(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAccidentReason(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTRSN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AccidentReasonCategory(pCategory);
	((AccidentReasonCategory *)pStaticInfo)->ShowAccidentReason(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAuditors(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTORS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new Auditors(pCategory);
	((Auditors *)pStaticInfo)->ShowAuditors(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowActualAuditors(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTORS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new Auditors(pCategory);
	((Auditors *)pStaticInfo)->ShowActualAuditors(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAudites(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTEES);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new Auditess(pCategory);
	((Auditess *)pStaticInfo)->ShowAudites(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHarmTypes(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HRMTYP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new HrmTyp(pCategory);
	((HrmTyp *)pStaticInfo)->ShowHarmTypes(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowDetails(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DTL);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDetails(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCopyofdetail(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DTLCPY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new ShwDetails(pCategory);
	((ShwDetails *)pStaticInfo)->ShowCopyofdetail(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCARBasis(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CARMODELBAS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCARBasis(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRole(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ROL);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRole(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowVehicleAccidentQuestions(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCLQSTNS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowVehicleAccidentQuestions(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowBasicReference(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_BASICREFERENCE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowBasicReference(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowEnvironmentCondition(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_ENVIRONMENTCONDITION);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEnvironmentCondition(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPersonalProtectiveEquipmentScreen(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_PERSONALPROTECTIVEEQUIPMENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PrsonlProtctveEquip(pCategory);
	((PrsonlProtctveEquip *)pStaticInfo)->ShowPersonalProtectiveEquipmentScreen(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPersonalProtectiveEquipmentpopUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PRSNLPRTCTVE_POP_UP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PrsonlProtctveEquip(pCategory);
	((PrsonlProtctveEquip *)pStaticInfo)->ShowPersonalProtectiveEquipmentpopUp(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCurrency(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_CURRENCY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCurrency(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowSignatures(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_SIGNATURES);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSignatures(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowInjuryType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_INJURYTYPE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowInjuryType(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowInjuryTypepopUpscreen(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_INJUERYTYPE_POP_UP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CInjueryType(pCategory);
	((CInjueryType *)pStaticInfo)->ShowInjuryTypepopUpscreen(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRewardType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_REWARDTYPE);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRewardType(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPotentialSeverity(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SYSTEMSETTINGS_POTENTIALSEVERITY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPotentialSeverity(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPerformanceMeasureScore(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTSCOR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPerformanceMeasureScore(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAccidentType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTTYP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AccidentTypesCategory(pCategory);
	((AccidentTypesCategory *)pStaticInfo)->ShowAccidentTypes(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAccidentMedium(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTMDM);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAccidentMedium(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowTypeofLoss(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TYPLOSS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowTypeofLoss(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHumanBodyParts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HMNBODYPART);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHumanBodyParts(pCategory);
	((CHumanBodyParts *)pStaticInfo)->ShowHumanBodyParts(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowTaskAuditResult(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TSKADTRSLT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowTaskAuditResult(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTWType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTWSTYP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPTWType(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskSeverityRatingClass(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKSVRTYRATCLS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRiskSeverityRatingClass(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskLikelihoodRating(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKLKLYHODRAT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRiskLikelihoodRating(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowEmployee(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EMP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEmployee(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskRankDescription(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKRNKDESC);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRiskRankDescription(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHSEProceduresList(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HSEPRCDRLIST);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEProceduresList(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTWResponsibility(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTWSRSPNSBLTY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPTWResponsibility(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowEntryRejectReasons(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ENTRJCRSN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEntryRejectReasons(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHSERegulations(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HSERGLR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSERegulations(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowNecessaryControlMeasures(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NCSSRYCTRLMSRMNT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new NecessaryControlMeasures(pCategory);
	((NecessaryControlMeasures *)pStaticInfo)->ShowNecessaryControlMeasures(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskAssessmentEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKASSMNTENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new RiskAssessmentEntryCategory(pCategory);
	((RiskAssessmentEntryCategory *)pStaticInfo)->ShowRiskAssessmentEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowRiskAssessmentApproval(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKASSMNTAPR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new RiskAssessmentApprovalCategory(pCategory);
	RiskAssessmentApprovalCategory *pRiskAssessmentApprovalCategory = (RiskAssessmentApprovalCategory *)pCommonCategoryWrapper;
	HRESULT retVal  = pRiskAssessmentApprovalCategory->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((RiskAssessmentApprovalCategory *)pCommonCategoryWrapper)->ShowRiskAssessmentApproval(CString(bstrCriteria));
	} else {		
		delete pCommonCategoryWrapper;		
	}				
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskAssessmentFollowUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKASSMNTFOLLWUPDT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new RiskAssessmentFollowUpCategory(pCategory);
	RiskAssessmentFollowUpCategory *pRiskAssessmentFollowUpCategory = (RiskAssessmentFollowUpCategory *)pCommonCategoryWrapper;	
	HRESULT retVal  = pRiskAssessmentFollowUpCategory->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		pRiskAssessmentFollowUpCategory->ShowRiskAssessmentFollowUp(CString(bstrCriteria));
	} else {		
		delete pCommonCategoryWrapper;		
	}	
	return S_OK;
}
/*STDMETHODIMP CHSEObj::ShowAuditors(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AUDITORS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	//CCommonCategoryWrapper *pStaticInfo = new AuditorsCategory(pCategory);
	//((AuditorsCategory *)pStaticInfo)->ShowAuditors(CString(bstrCriteria));
	//return S_OK;
}*/
STDMETHODIMP CHSEObj::ShowAuditees(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AUDITEES);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAuditees(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPotentialHazardEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTNLHZRDENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PotentialHazardEntryCategory(pCategory);
	((PotentialHazardEntryCategory *)pStaticInfo)->ShowPotentialHazardEntry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPotentialHazardConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTNLHZRDCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new PotentialHazardConfirmationCategory(pCategory);	
	HRESULT retVal  = ((PotentialHazardConfirmationCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((PotentialHazardConfirmationCategory *)pCommonCategoryWrapper)->ShowPotentialHazardConfirmation(CString(bstrCriteria));
	} else {
		delete pCommonCategoryWrapper;		
	}	
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowPotentialhazardFollowUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTNLHZRDFOLLOWUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PotentialHazardFollowUp(pCategory);
	((PotentialHazardFollowUp *)pStaticInfo)->ShowPotentialhazardFollowUp(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowLossAccidentEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOSSACDNTENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LossAccidentEntryCategory(pCategory);
	((LossAccidentEntryCategory *)pStaticInfo)->ShowLossAccidentEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowLossAccidentConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOSSACDNTCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new LossAccidentConfirmationCategory(pCategory);
	HRESULT retVal  = ((LossAccidentConfirmationCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((LossAccidentConfirmationCategory *)pCommonCategoryWrapper)->ShowLossAccidentConfirmation(CString(bstrCriteria));
	} else {		
		delete pCommonCategoryWrapper;		
	}			
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowPerformanceMeasurementEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PerformanceMeasurementEntryCategory(pCategory);
	((PerformanceMeasurementEntryCategory *)pStaticInfo)->ShowPerformanceMeasurementEntry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPerformanceMeasurementEvaluation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTEVAL);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new PerformanceMeasurementEvaluationCategory(pCategory);	
	PerformanceMeasurementEvaluationCategory *pPerformanceMeasurementEvaluationCategory = (PerformanceMeasurementEvaluationCategory *)pCommonCategoryWrapper;
	HRESULT retVal  = pPerformanceMeasurementEvaluationCategory->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((PerformanceMeasurementEvaluationCategory *)pPerformanceMeasurementEvaluationCategory)->ShowPerformanceMeasurementEvaluation(CString(bstrCriteria));
	} else {
		delete pCommonCategoryWrapper;		
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowPerformanceMeasurementFollowUp(BSTR bstrCriteria)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTFLWUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	CCommonCategoryWrapper *pCommonCategoryWrapper = new PerformanceMeasurementFollowUpCategory(pCategory);	
	PerformanceMeasurementFollowUpCategory *pPerformanceMeasurementFollowUpCategory = (PerformanceMeasurementFollowUpCategory *)pCommonCategoryWrapper;
	HRESULT retVal  = pPerformanceMeasurementFollowUpCategory->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((PerformanceMeasurementFollowUpCategory *)pPerformanceMeasurementFollowUpCategory)->ShowPerformanceMeasurementFollowUp(CString(bstrCriteria));	
	} else {
		delete pCommonCategoryWrapper;
	}	
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowNearMissEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NRSTMISCENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new NearMissEntryCategory(pCategory);
	((NearMissEntryCategory *)pStaticInfo)->ShowNearMissEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowNearMissConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NRSTMISCCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new NearMissConfirmationCategory(pCategory);	
	HRESULT retVal  = ((NearMissConfirmationCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((NearMissConfirmationCategory *)pCommonCategoryWrapper)->ShowNearMissConfirmation(CString(bstrCriteria));
	}		
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowNearMissFollowUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NRSTMISCFLWUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new NearMissFollowUpCategory(pCategory);
	HRESULT retVal  = ((NearMissFollowUpCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((NearMissFollowUpCategory *)pCommonCategoryWrapper)->ShowNearMissFollowUp(CString(bstrCriteria));
	}
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowNearMissReward(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_NEARMISS_NEARMISSREWARD);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new NearMissReward(pCategory);
	((NearMissReward *)pStaticInfo)->ShowNearMissReward(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAccidentConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new AccidentConfirmationCategory(pCategory);
	HRESULT retVal  = ((AccidentConfirmationCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((AccidentConfirmationCategory *)pCommonCategoryWrapper)->ShowAccidentConfirmation(CString(bstrCriteria));
	}
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowAccidentFollowup(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTFOLLWUPDT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new AccidentFollowUpCategory(pCategory);
	HRESULT retVal  = ((AccidentFollowUpCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((AccidentFollowUpCategory *)pCommonCategoryWrapper)->ShowAccidentFollowup(CString(bstrCriteria));
	}
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCorrectiveActionEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CRCTVEACCENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CarEntryCategory(pCategory);
	((CarEntryCategory *)pStaticInfo)->ShowCorrectiveActionEntry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCorrectiveActionConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CRCTVEACCCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CarConfirmationCategory(pCategory);
	((CarConfirmationCategory *)pStaticInfo)->ShowCorrectiveActionConfirmation(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCorrectiveActionFollowupvisit(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CRCTVACTNFLWUPVSTS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CarFlwupVstCategory(pCategory);
	((CarFlwupVstCategory *)pStaticInfo)->ShowCorrectiveActionFollowupvisit(CString(bstrCriteria));
	return S_OK;
}
//Corrective Actions Received
STDMETHODIMP CHSEObj::ShowCrctveAccRcvd(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CrctveAccRcvd);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CARRcvdCategory(pCategory);
	((CARRcvdCategory *)pStaticInfo)->ShowCrctveAccRcvd(CString(bstrCriteria));
	return S_OK;
}
//Corrective Actions Under Execution
STDMETHODIMP CHSEObj::ShowCrctveAccUndrExctn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CrctveAccUndrExctn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CARUndrExctnCategory(pCategory);
	((CARUndrExctnCategory *)pStaticInfo)->ShowCrctveAccUndrExctn(CString(bstrCriteria));
	return S_OK;
}
//Corrective Action Job Verification
STDMETHODIMP CHSEObj::ShowCrctveAccJobVrfctn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CrctveAccJobVrfctn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CARJobVrfctnCategory(pCategory);
	((CARJobVrfctnCategory *)pStaticInfo)->ShowCrctveAccJobVrfctn(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTWRegisterEntry(BSTR bstrCriteria,BSTR bstrScreenDefaults)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTWSRGSTRENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PtwEntryCategory(pCategory);
	((PtwEntryCategory *)pStaticInfo)->ShowPTWRegisterEntry(CString(bstrCriteria),CString(bstrScreenDefaults));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTWRegisterConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTWSRGSTRCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PtwConfirmationCategory(pCategory);
	((PtwConfirmationCategory *)pStaticInfo)->ShowPTWRegisterConfirmation(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTWsRgstrCls(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTWSRGSTRCLS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PtwClsCategory(pCategory);
	((PtwClsCategory *)pStaticInfo)->ShowPTWsRgstrCls(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAuditPlanEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditPlanEntryCategory(pCategory);
	((AuditPlanEntryCategory *)pStaticInfo)->ShowAuditPlanEntry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAuditPlanApproval(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNAPR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;	
	CCommonCategoryWrapper *pCommonCategoryWrapper = new AuditPlanApprovalCategory(pCategory);
	HRESULT retVal  = ((AuditPlanApprovalCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {	
		((AuditPlanApprovalCategory *)pCommonCategoryWrapper)->ShowAuditPlanApproval(CString(bstrCriteria));
		return S_OK;
	} else {
		delete pCommonCategoryWrapper;
		return S_FALSE;
	}	
}

STDMETHODIMP CHSEObj::ShowAuditPlanFlwUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNFLWUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new AuditPlanFollowUpCategory(pCategory);
	HRESULT retVal  = ((AuditPlanFollowUpCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {	
		((AuditPlanFollowUpCategory *)pCommonCategoryWrapper)->ShowAuditPlanFlwUp(CString(bstrCriteria));
		return S_OK;
	} else {
		delete pCommonCategoryWrapper;
		return S_FALSE;
	}
}

STDMETHODIMP CHSEObj::ShowAuditPlanAuditorsandAuditees(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNADTRSNADTES);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditPlanADTRSNADTES(pCategory);
	((AuditPlanADTRSNADTES *)pStaticInfo)->ShowAuditPlanAuditorsandAuditees(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAuditPlanPreparechecklist(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNPRPERCHKLST);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditPlanPrepreChkLst(pCategory);
	((AuditPlanPrepreChkLst *)pStaticInfo)->ShowAuditPlanPreparechecklist(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAuditPlanAuditchecklistfeedback(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNCHKLSTFDBCK);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditPlanChckLstFdbck(pCategory);
	((AuditPlanChckLstFdbck *)pStaticInfo)->ShowAuditPlanAuditchecklistfeedback(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowVehicleAccidentEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCLACDNTENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new VehicleAccidentEntryCategory(pCategory);
	((VehicleAccidentEntryCategory *)pStaticInfo)->ShowVehicleAccidentEntry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowVehicleAccidentConfirmation(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCLACDNTCNFRMTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new VehicleAccidentConfirmationCategory(pCategory);
	HRESULT retVal  = ((VehicleAccidentConfirmationCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((VehicleAccidentConfirmationCategory *)pCommonCategoryWrapper)->ShowVehicleAccidentConfirmation(CString(bstrCriteria));
		return S_OK;
	} else {
		delete pCommonCategoryWrapper;	
		return S_FALSE;
	}			
	
}
STDMETHODIMP CHSEObj::ShowVehicleAccidentFollowUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCLACDNTFLWUP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pCommonCategoryWrapper = new VehicleAccidentFollowUpCategory(pCategory);
	HRESULT retVal  = ((VehicleAccidentFollowUpCategory *)pCommonCategoryWrapper)->OpenScreen("","",0,0,false,false);
	if(retVal == 0) {
		((VehicleAccidentFollowUpCategory *)pCommonCategoryWrapper)->ShowVehicleAccidentFollowUp(CString(bstrCriteria));
		return S_OK;
	} else {
		delete pCommonCategoryWrapper;	
		return S_FALSE;
	}				
}

STDMETHODIMP CHSEObj::ShowSitSrvyEnt(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	SetCurrentMenuItemID(ID_HSE_SitSrvyEnt);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;

	CCommonCategoryWrapper *pStaticInfo = new SitSrvyEntCategory(pCategory);
	((SitSrvyEntCategory *)pStaticInfo)->ShowSitSrvyEnt(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSitSrvyCnfrmtn(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SitSrvyCnfrmtn);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new SitSrvyCnfrmtnCategory(pCategory);
	((SitSrvyCnfrmtnCategory *)pStaticInfo)->ShowSitSrvyCnfrmtn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSitSrvyFlwup(BSTR bstrCriteria) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SitSrvyFlwup);	
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new SitSrvyFlwupCategory(pCategory);
	((SitSrvyFlwupCategory *)pStaticInfo)->ShowSitSrvyFlwup(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAttachmentclassification(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ATCHCLSFC);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAttachmentclassification(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHSEPolicy(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HSEPLC);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEPolicy(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCompanyProfile(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_COMPPRF);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowCompanyProfile(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowBodyParts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HUMNBDYPRT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowBodyParts(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPotentialhazardinquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PTNLHZRDINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PotentialHazardInquiry(pCategory);
	((PotentialHazardInquiry *)pStaticInfo)->ShowPotentialhazardinquiry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowRiskInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKASSMNTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new RiskAssesmentInquiry(pCategory);
	((RiskAssesmentInquiry *)pStaticInfo)->ShowRiskInquiry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSitSrvyEntInq(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SITSRVYINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new SitSrvyInqCategory(pCategory);
	((SitSrvyInqCategory *)pStaticInfo)->ShowSitSrvyEntInq(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowPerformanceInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PERFMSRMNTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PerformanceMEasurementInquiry(pCategory);
	((PerformanceMEasurementInquiry *)pStaticInfo)->ShowPerformanceInquiry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAccident(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ACDNTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AccidentInquiry(pCategory);
	((AccidentInquiry *)pStaticInfo)->ShowAccident(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowAudit(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTPLNINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AuditInquiry(pCategory);
	((AuditInquiry *)pStaticInfo)->ShowAudit(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowCAR(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CRCTVEACCINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CARInquiry(pCategory);
	((CARInquiry *)pStaticInfo)->ShowCAR(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowVehchileAccidentInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_VCLACDNTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new VehchileAccidentInquiry(pCategory);
	((VehchileAccidentInquiry *)pStaticInfo)->ShowVehchileAccidentInquiry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowNearMissInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NRSTMISCINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new NearMissInquiry(pCategory);
	((NearMissInquiry *)pStaticInfo)->ShowNearMissInquiry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowPTW(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PPTWINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new PTWInquiry(pCategory);
	((PTWInquiry *)pStaticInfo)->ShowPTW(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowLossAccidentInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOSSACCDNTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LossAccidentInq(pCategory);
	((LossAccidentInq *)pStaticInfo)->ShowLossAccidentInquiry(CString(bstrCriteria));
	return S_OK;
}
STDMETHODIMP CHSEObj::ShowHSEProceduresListInquiry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HSEPROCDRSLSTINQ);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEProceduresListInquiry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowRiskAnalysis(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_RISKANALYSIS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new RiskAnalysis(pCategory);
	((RiskAnalysis *)pStaticInfo)->ShowRiskAnalysis(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowIncdntFlshRprt(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntFlshRprt);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new IncdntFlshRprtCategory(pCategory);
	((IncdntFlshRprtCategory *)pStaticInfo)->ShowIncdntFlshRprt(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEToolsEquipments(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TOOLEQUIP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEToolsEquipments(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSubDetails(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SUBDETAILS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSubDetails(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDtlForChkLst(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DTLFORCHKLIST);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDtlForChkLst(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowPrecautions(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_PRECAUTIONS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowPrecautions(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEToolsEquipmentsDistribution(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TOOLSEQUIPDSTRBTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEToolEquipDistrib(pCategory);
	((CHSEToolEquipDistrib *)pStaticInfo)->ShowHSEToolsEquipmentsDistribution(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowNearMissTypes(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_NearMissTypes);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new NearMissTypesCategory(pCategory);
	((NearMissTypesCategory *)pStaticInfo)->ShowNearMissTypes(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowJobPtntlHzrdsLIB(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_JobPtntlHzrdsLib);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new JobPtntlHzrdsLib(pCategory);
	((JobPtntlHzrdsLib *)pStaticInfo)->ShowJobPtntlHzrdsLib(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowContactMethods(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CNTCT_MTHDS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowContactMethods(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAtchmntFrmts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ATCHMNTFRMTS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAtchmntFrmts(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowTrades(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_TRADES);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowTrades(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowFirstInfo(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_FIRSTINFO);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowFirstInfo(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDocType(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_DOCTYP);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowDocType(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAdtActvtRqrdActn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_ADTACTVTRQRDACTN);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAdtActvtRqrdActn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowRskSvrtyCntrlMsr(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RSKSVRTYCNTRLMSR);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRskSvrtyCntrlMsr(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAdtChkLstPopUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtChkLstPopUp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAdtChkLstPopUp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEPrjcts(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_Prjcts);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEPrjcts(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEMnDrlTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MnDrlTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEMnDrlTyp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEAdtTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEAdtTyp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSERskRat(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RskRat);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSERskRat(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEIncdntPrd(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntPrd);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEIncdntPrd(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEJobStp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_JobStp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEJobStp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSECnsqncs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_Cnsqncs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSECnsqncs(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEAdtChcklst(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtChcklst);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEAdtChcklst(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEAdtGrp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtGrp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEAdtGrp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAdtRschdl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtRschdl);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAdtRschdl(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHRCTRLBrws(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_HRCTRLBDTLRWS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHRCTRLBrws(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAdtRschdlBrws(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtRschdlBrws);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAdtRschdlBrws(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAdtCncl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AdtCncl);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAdtCncl(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSERltdOprtns(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RltdOprtns);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSERltdOprtns(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEMtrlRlsTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MtrlRlsTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEMtrlRlsTyp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSECntnmntTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_CntnmntTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSECntnmntTyp(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSEMtrlRlsCat(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MtrlRlsCat);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEMtrlRlsCat(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSERlsMtrl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RlsMtrl);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSERlsMtrl(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEUOM(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_UOM);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEUOM(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSEMtrlRlsTo(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MtrlRlsTo);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEMtrlRlsTo(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSESplClsfctn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_SplClsfctn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSESplClsfctn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSEAstTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AstTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowHSEAstTyp(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowInjryAnlysisPopup(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_INJRYANLYSIS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowInjryAnlysisPopup(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowSftyInstrctn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_SAFETY_INSTRUCTIONS);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowSftyInstrctn(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowHSELogsDef(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOGS_DEF);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LogsDefCategory(pCategory);
	((LogsDefCategory *)pStaticInfo)->ShowHSELogsDef(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowLogsDefAttrDtl(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOGS_DEF_ATTR_DTL);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LogsDefCategory(pCategory);
	((LogsDefAttrDtl *)pStaticInfo)->ShowLogsDefAttrDtl(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowHSELogsEnt(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOGS_ENT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LogsEntCategory(pCategory);
	((LogsEntCategory *)pStaticInfo)->ShowHSELogsEnt(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowLogsEntDtlCnt(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_LOGS_ENT_DTL_CNT);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new LogsEntDtlCntCategory(pCategory);
	((LogsEntDtlCntCategory *)pStaticInfo)->ShowLogsEntDtlCnt(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowEnvronmntlAspctEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(HSE_ASPECTS_ENTRY_ENTRY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvrnmntAspctCategory(pCategory);
	((EnvrnmntAspctCategory *)pStaticInfo)->ShowEnvronmntlAspctEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowEnvronmntlAspctReview(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(HSE_ASPECTS_REVIEW_REVIEW);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvrnmntAspctReviewCategory(pCategory);
	((EnvrnmntAspctReviewCategory *)pStaticInfo)->ShowEnvronmntlAspctReview(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDrillPlanEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(HSE_DRILL_PLAN_ENTRY_ENTRY);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new DrillPlanEntryCategory(pCategory);
	((DrillPlanEntryCategory *)pStaticInfo)->ShowDrillPlanEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDrillPlanAprv(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(HSE_DRILL_PLAN_APPROVAL_APPROVAL);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new DrillPlanAprvCategory(pCategory);
	((DrillPlanAprvCategory *)pStaticInfo)->ShowDrillPlanAprv(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowDrillPlanExecution(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(HSE_DRILL_EXECUTION_EXECUTION);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new DrillPlanExecutionCategory(pCategory);
	((DrillPlanExecutionCategory *)pStaticInfo)->ShowDrillPlanExecution(CString(bstrCriteria));
	return S_OK;
}


STDMETHODIMP CHSEObj::ShowIncdntFlshRprtRvw(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntFlshRprtRvw);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new IncdntFlshRprtCategory(pCategory);
	((IncdntFlshRprtCategory *)pStaticInfo)->ShowIncdntFlshRprtRvw(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowIncdntFlshRprtInq(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntFlshRprtInq);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new IncdntFlshRprtCategory(pCategory);
	((IncdntFlshRprtCategory *)pStaticInfo)->ShowIncdntFlshRprtInq(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowIncdntInvstgtnEntry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AcdntIncdntInvstgtnEntry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new IncdntInvstgtnEntryCategory(pCategory);
	((IncdntInvstgtnEntryCategory *)pStaticInfo)->ShowIncdntInvstgtnEntry(CString(bstrCriteria));
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowIncdntInvstgtnAprv(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AcdntIncdntInvstgtnAprv);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new IncdntInvstgtnAprvCategory(pCategory);
	((IncdntInvstgtnAprvCategory *)pStaticInfo)->ShowIncdntInvstgtnAprv(CString(bstrCriteria));
	return S_OK;
}

//EQ Type
STDMETHODIMP CHSEObj::ShowEQTyps(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EQTyps);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEQTyps(CString(bstrCriteria));
	return S_OK;
}

//EQ Inspection Request
STDMETHODIMP CHSEObj::ShowEQInspctnRqust(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EQInspctnRqust);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EQInspctnCategory(pCategory);
	((EQInspctnCategory *)pStaticInfo)->ShowEQInspctnRqust(CString(bstrCriteria));
	return S_OK;
}

//EQ Inspection Result
STDMETHODIMP CHSEObj::ShowEQInspctnRsult(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EQInspctnRsult);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EQInspctnCategory(pCategory);
	((EQInspctnCategory *)pStaticInfo)->ShowEQInspctnRsult(CString(bstrCriteria));
	return S_OK;
}

//EQ Inspection Work
STDMETHODIMP CHSEObj::ShowEQInspctnWrk(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EQInspctnWrk);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EQInspctnCategory(pCategory);
	((EQInspctnCategory *)pStaticInfo)->ShowEQInspctnWrk(CString(bstrCriteria));
	return S_OK;
}

//EQ Inspection Inquiry
STDMETHODIMP CHSEObj::ShowEQInspctnInqry(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EQInspctnInqry);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EQInspctnCategory(pCategory);
	((EQInspctnCategory *)pStaticInfo)->ShowEQInspctnInqry(CString(bstrCriteria));
	return S_OK;
}

//Pollutants
//Update : Screen name changed to Measures (Pop-up screen)
STDMETHODIMP CHSEObj::ShowEnvMntrActvtsPltnt(BSTR bstrCriteria,BSTR bstrComeFrom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvMntrActvtsPltnt);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new EnvMntrMsrPopupCategory(pCategory);
	((EnvMntrMsrPopupCategory *)pStaticInfo)->ShowEnvMntrActvtsPltnt(CString(bstrCriteria),CString(bstrComeFrom));
	return S_OK;
}

//Area Hazards
STDMETHODIMP CHSEObj::ShowAreaHzrds(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_Area_Hzrds);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AreaHzrdsCategory(pCategory);
	((AreaHzrdsCategory *)pStaticInfo)->ShowAreaHzrds(CString(bstrCriteria));
	return S_OK;
}


//Control Measure pop-up screen (from Area Hazard -> Potenial Hazards)
STDMETHODIMP CHSEObj::ShowAHzrdsPtntlHzrdCtrlMsr(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AHzrdsPtntlHzrdCtrlMsr);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AreaHzrdsCategory(pCategory);
	((AreaHzrdsCategory *)pStaticInfo)->ShowAHzrdsPtntlHzrdCtrlMsr(CString(bstrCriteria));
	return S_OK;
}

//Effects pop-up screen (from Area Hazard -> Potenial Hazards)
STDMETHODIMP CHSEObj::ShowAreaHzrdsPtntlHzrdEffct(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_AreaHzrdsPtntlHzrdEffct);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new AreaHzrdsCategory(pCategory);
	((AreaHzrdsCategory *)pStaticInfo)->ShowAreaHzrdsPtntlHzrdEffct(CString(bstrCriteria));
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HSE BGC-EZZ Requirements_2892020

//Risk Type
STDMETHODIMP CHSEObj::ShowRskTyp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_RskTyp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowRskTyp(CString(bstrCriteria));
	return S_OK;
}
//Medical Authority
STDMETHODIMP CHSEObj::ShowMdclAuthrty(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MdclAuthrty);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowMdclAuthrty(CString(bstrCriteria));
	return S_OK;
}
//Medical Decision
STDMETHODIMP CHSEObj::ShowMdclDcsn(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_MdclDcsn);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowMdclDcsn(CString(bstrCriteria));
	return S_OK;
}
//Auditors
STDMETHODIMP CHSEObj::ShowAudtrs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_Audtrs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowAudtrs(CString(bstrCriteria));
	return S_OK;
}
//Environmental Measures
STDMETHODIMP CHSEObj::ShowEnvrnmntlMsrs(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_EnvrnmntlMsrs);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowEnvrnmntlMsrs(CString(bstrCriteria));
	return S_OK;
}
//Incident Medical Report Follow-Up
STDMETHODIMP CHSEObj::ShowIncdntMdclRprtFlwUp(BSTR bstrCriteria)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(ID_HSE_IncdntMdclRprtFlwUp);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	((CHSEMSCommonCategory *)pStaticInfo)->ShowIncdntMdclRprtFlwUp(CString(bstrCriteria));
	return S_OK;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CHSEObj::setstrSrcForRjctRsn(CString strSrcScr) {
	m_strSrcForRjctRsn = strSrcScr;
}

void CHSEObj::setstrSrcForAccptRsn(CString strSrcScr) {
	m_strSrcForAccptRsn = strSrcScr;
}


CString CHSEObj::getstrSrcForRjctRsn() {
	return m_strSrcForRjctRsn;
}

CString CHSEObj::getstrSrcForAccptRsn() {
	return m_strSrcForAccptRsn;
}

void CHSEObj::setstrSrcForApprvlRsn(CString strSrcScr) {
	m_strSrcForApprvlRsn = strSrcScr;
}

CString CHSEObj::getstrSrcForApprvlRsn() {
	return m_strSrcForApprvlRsn;
}

void CHSEObj::setstrKeyValForApprvlScr(CString strKeyVal) {
	m_strKeyValForApprvlScr = strKeyVal;
}

void CHSEObj::setstrKeyValForPopUpScr(CString strScreen,CString strKeyVal)
{
	m_MapScreenTOKey.SetAt(strScreen,strKeyVal);

}

CString CHSEObj::getstrKeyValForPopUpScr(CString strScreen)
{
	CString strKeyVal;
	m_MapMenuIDTOTags.Lookup(strScreen,strKeyVal);
	return strKeyVal;
}

void CHSEObj::setstrKeyValForRjctScr(CString strKeyVal) {
	m_strKeyValForRjctScr = strKeyVal;
}

void CHSEObj::setstrKeyValForAccptScr(CString strKeyVal) {
	m_strKeyValForAccptScr = strKeyVal;
}


CString CHSEObj::getstrKeyValForRjctScr() {
	return m_strKeyValForRjctScr;
}

CString CHSEObj::getstrKeyValForAccptScr() {
	return m_strKeyValForAccptScr;
}

CString CHSEObj::getstrKeyValForApprvlScr() {
	return m_strKeyValForApprvlScr;
}


void CHSEObj::setbLogMultiRec(bool bMultiRec) {
	m_bLogMultiRec = bMultiRec;
}

bool CHSEObj::getbLogMultiRec() {
	return m_bLogMultiRec;
}

void CHSEObj::setbNewMode(CString strScrTag,bool bNewMode) {
	if(bNewMode)
		m_ScreensInNewMode.SetAt(strScrTag,"true");		
	else 
		m_ScreensInNewMode.RemoveKey(strScrTag);
}

bool CHSEObj::getbNewMode(CString strScrTag){
	bool bScrInNewMode = false;
	CString strScrTagFound="";
	m_ScreensInNewMode.Lookup(strScrTag,strScrTagFound);
	if(strScrTagFound != "") 
		bScrInNewMode = true; 	
	return bScrInNewMode;
}

void CHSEObj::setnAuditsKeyFld(int nAdtKeyFld) {
	m_nAuditsKeyFld = nAdtKeyFld;
}

int CHSEObj::getnAuditsKeyFld() {
	return m_nAuditsKeyFld;
}

void CHSEObj::setstrDep(CString strDep) {
	m_strDep = strDep;
}

CString CHSEObj::getstrDep() {
	return m_strDep;
}


//Ahmed Ali
void CHSEObj::createTagHashtable()
{
	//Risk assessment
	m_ScreenTagForTracing.SetAt("HSE_TGRSKASSMNTAPR","RSKASSMNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGRSKASSMNTFOLLWUPDT","RSKASSMNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGRSKASSMNTENT","RSKASSMNTENT");
	//Potential hazard
	m_ScreenTagForTracing.SetAt("HSE_TGPTNLHZRDENT","PTNLHZRDENT");
	m_ScreenTagForTracing.SetAt("HSE_TGPTNLHZRDCNFRMTN","PTNLHZRDENT");
	m_ScreenTagForTracing.SetAt("HSE_TGPTNLHZRDFOLLOWUP","PTNLHZRDENT");
	//Loss Accident
	m_ScreenTagForTracing.SetAt("HSE_TGLOSSACDNTENT","LOSSACDNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGLOSSACDNTCNFRMTN","LOSSACDNTENT");
	//Corrective action
	m_ScreenTagForTracing.SetAt("HSE_TGCRCTVEACCENT","CRCTVEACCENT");
	m_ScreenTagForTracing.SetAt("HSE_TGCRCTVEACCCNFRMTN","CRCTVEACCENT");
	m_ScreenTagForTracing.SetAt("HSE_TGCRCTVEACCFLWUP","CRCTVEACCENT");
	//PTW
	m_ScreenTagForTracing.SetAt("HSE_TGPTWSRGSTRENT","PTWSRGSTRENT");
	m_ScreenTagForTracing.SetAt("HSE_TGPTWSRGSTRCNFRMTN","PTWSRGSTRENT");	
	//Audit
	m_ScreenTagForTracing.SetAt("HSE_TGADTPLNENT","ADTPLNENT");
	m_ScreenTagForTracing.SetAt("HSE_TGADTPLNAPR","ADTPLNENT");
	m_ScreenTagForTracing.SetAt("HSE_TGADTPLNFOLLWUP","ADTPLNENT");
	//Vehicle
	m_ScreenTagForTracing.SetAt("HSE_TGVCLACDNTENT","VCLACDNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGVCLACDNTCNFRMTN","VCLACDNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGVCLACDNTFLWUP","VCLACDNTENT");
	//Accident
	m_ScreenTagForTracing.SetAt("HSE_TGACDNTENT","ACDNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGACDNTCNFRMTN","ACDNTENT");
	m_ScreenTagForTracing.SetAt("HSE_TGACDNTFOLLWUPDT","ACDNTENT");		
	//near miss|
	m_ScreenTagForTracing.SetAt("HSE_TGNRSTMISCENT","NRSTMISCENT");
	m_ScreenTagForTracing.SetAt("HSE_TGNRSTMISCCNFRMTN","NRSTMISCENT");
	m_ScreenTagForTracing.SetAt("HSE_TGNRSTMISCFLWUP","NRSTMISCENT");
	//Site Survey
	m_ScreenTagForTracing.SetAt("HSE_TGSITSRVYENT","SITSRVYENT");
	m_ScreenTagForTracing.SetAt("HSE_TGSITSRVYCNFRMTN","SITSRVYENT");
	m_ScreenTagForTracing.SetAt("HSE_TGSITSRVYFLWUP","SITSRVYENT");

}

CString CHSEObj::sendMail(CString strSenderName,CString strSubject,CString strRecipientMailAdd,CString strMail,CString strAttachFilePath) {		
	CString strSQL;	
	CString strMailAddressKey = "<SERVER_SETTING\\APP_66><APPACCOUNT>";
	strSQL.Format("SELECT REG_VALUE FROM EMAILWIZ WHERE REG_KEY = '%s'",strMailAddressKey);				
	CString strMailSenderAdd;
	CString strMailSenderPass;
	CString strSMTPPort;
	CString strSMTP_IP;
	long lRetVal=0;
	CPolyDbRecordset DaoRecSet;			
	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("REG_VALUE",strMailSenderAdd);		
		DaoRecSet.Close();
	}							
	CString strMailPass = "<SERVER_SETTING\\APP_66><APPPASSWORD>";
	strSQL.Format("SELECT REG_VALUE FROM EMAILWIZ WHERE REG_KEY = '%s'",strMailPass);			
	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("REG_VALUE",strMailSenderPass);		
		DaoRecSet.Close();
	}
	
	CString strSMTPPortKey = "<SERVER_SETTING\\APP_66><SMTPPORT>";
	strSQL.Format("SELECT REG_VALUE FROM EMAILWIZ WHERE REG_KEY = '%s'",strSMTPPortKey);			
	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("REG_VALUE",strSMTPPort);		
		DaoRecSet.Close();
	}

	CString strSMTP_IP_Key = "<SERVER_SETTING\\APP_66><SMTPIP>";
	strSQL.Format("SELECT REG_VALUE FROM EMAILWIZ WHERE REG_KEY = '%s'",strSMTP_IP_Key);			
	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("REG_VALUE",strSMTP_IP);		
		DaoRecSet.Close();
	}
	
	char* MailSenderAdd = 0;
	MailSenderAdd = strMailSenderAdd.GetBuffer(strMailSenderAdd.GetLength());
	strMailSenderAdd.ReleaseBuffer();

	char* MailSenderPass = 0;
	MailSenderPass = strMailSenderPass.GetBuffer(strMailSenderPass.GetLength());
	strMailSenderPass.ReleaseBuffer();
	
	char* SenderName = 0;
	SenderName = strSenderName.GetBuffer(strSenderName.GetLength());
	strSenderName.ReleaseBuffer();

	char* Subject = 0;
	Subject = strSubject.GetBuffer(strSubject.GetLength());
	strSubject.ReleaseBuffer();

	char* RecipientMailAdd = 0;
	RecipientMailAdd = strRecipientMailAdd.GetBuffer(strRecipientMailAdd.GetLength());
	strRecipientMailAdd.ReleaseBuffer();
	
	char* Mail = 0;
	Mail = strMail.GetBuffer(strMail.GetLength());
	strMail.ReleaseBuffer();
	
	char* AttachFilePath = 0;
	AttachFilePath = strAttachFilePath.GetBuffer(strAttachFilePath.GetLength());
	strAttachFilePath.ReleaseBuffer();
	
	bool bMailSent = false;
   
	//_BSTR_t bstrMailSenderAdd(MailSenderAdd); 
	BSTR bstrMailSenderAdd    = CString(MailSenderAdd).AllocSysString();
	BSTR bstrMailSenderPass   = CString(MailSenderPass).AllocSysString();
	BSTR bstrServerMail       = CString(strSMTP_IP).AllocSysString();
	BSTR bstrAttachFilePath   = CString(AttachFilePath).AllocSysString();
	BSTR bstrRecipientMailAdd = CString(RecipientMailAdd).AllocSysString();
	BSTR bstrSubject		  = CString(Subject).AllocSysString();
	strMail.Replace("“","\"\"");
	strMail.Replace("”","\"\"");	
	BSTR bstrMail             = CString(strMail).AllocSysString();				
	
	short port = atoi(strSMTPPort);
	m_SendMailObj->put_EmailAddress(&bstrMailSenderAdd);
	m_SendMailObj->put_UserPassword(&bstrMailSenderPass);
	//m_SendMailObj->AttachFilesChanged(bstrAttachFilePath);
	m_SendMailObj->put_Attachments(&bstrAttachFilePath);
	m_SendMailObj->put_SMTPPortNo(&port);	
	m_SendMailObj->put_SMTPSrvNm(&bstrServerMail);	
	_bstr_t bstrRetVal = m_SendMailObj->SendMail(bstrRecipientMailAdd,"","","",bstrSubject,bstrMail,"");			
	return (LPCTSTR) bstrRetVal;
}



STDMETHODIMP CHSEObj::getDocumentationFolderPath(BSTR *bstrPath) {
	long lRetVal=0;
	CString strSQL,strCmpnyNam,strPath;
	CString strUserID = GetUserID();
	strSQL.Format("SELECT EMPLOYEE_CMPNYCOD FROM CMN_EMPLOYEE WHERE EMPLOYEE_USRID = '%s'",strUserID);
	CPolyDbRecordset DaoRecSet;			
	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("EMPLOYEE_CMPNYCOD",strCmpnyNam);		
		DaoRecSet.Close();
	}							
	if(strCmpnyNam != "") {
		strSQL.Format("SELECT HSEPLC_QMSMNULPATH FROM HSE_HSEPLC WHERE HSEPLC_CMPNYNAM = '%s'");
	} else {
		strSQL.Format("SELECT HSEPLC_QMSMNULPATH FROM HSE_HSEPLC");
	}

	lRetVal = DaoRecSet.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if (lRetVal == 1)
	{
		DaoRecSet.MoveFirst();
		DaoRecSet.GetFieldValue("HSEPLC_QMSMNULPATH",strPath);		
		DaoRecSet.Close();
	}
	*bstrPath = strPath.AllocSysString();
	return S_OK;
}

STDMETHODIMP CHSEObj::genIFCRep() {
	m_EGPCObj->GenerateIFCReport("C:\\First\\HSE");
	return S_OK;
}

STDMETHODIMP CHSEObj::genEGPCMonthlyRep() {
	m_EGPCObj->GenerateEGPCMonthlyReport("C:\\First\\HSE");
	return S_OK;
}

STDMETHODIMP CHSEObj::genTGKPIRep() {
	m_EGPCObj->GenerateTGKPI("C:\\First\\HSE");
	return S_OK;
}

STDMETHODIMP CHSEObj::genFlashForms() {
	m_EGPCObj->GenerateFlashForms("C:\\First\\HSE");	
	return S_OK;
}

STDMETHODIMP CHSEObj::genMonthlyReports() {
	CString OutputFolder=GetWorkingFolder();
	short bReplaceNullWithNotFoundVal=1;
	BSTR strServerName	= m_Connection.m_pSetupConnection->GetSQLServerMachineName().AllocSysString();
	BSTR strCatalog		= m_Connection.m_pSetupConnection->GetSQLServerCatalog().AllocSysString();	
	_bstr_t serverName(_bstr_t(m_strConnection),TRUE);
	_bstr_t catalog(strCatalog,TRUE);
	_bstr_t ErrorDescription=pMonthlyReportObj->RunReport (_bstr_t(m_strConnection),_bstr_t(catalog),_bstr_t(OutputFolder),bReplaceNullWithNotFoundVal);
	CString strMsg=(LPCTSTR)(ErrorDescription);
	if(strMsg!="")
		AfxMessageBox(strMsg);			
	return S_OK;
}

BOOL CHSEObj::getDisableSitSrvyRqrdActnBtn() {
	return m_bDisableSitSrvyRqrdActnBtn;
}

void CHSEObj::setDisableSitSrvyRqrdActnBtn(BOOL bDisable) {
	m_bDisableSitSrvyRqrdActnBtn = bDisable;
}


//From CMS , ShowScreen 
STDMETHODIMP CHSEObj::ShowScreen(int nScreenId,BSTR bstrCriteria,BSTR strTableName)
{
	return ShowScreenIn(nScreenId,bstrCriteria,strTableName);
}

//bstrCriteria = WHERE AUDTPLNENTRY_PLNN = 3
HRESULT CHSEObj::ShowScreenIn(int nScreenId,BSTR bstrCriteria,BSTR strTableName,void* msg,CCommonCategoryWrapper **pCommonCategoryWrapper)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	SetCurrentMenuItemID(nScreenId);
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	/*CHSEMSCommonCategory *pSR = new CHSEMSCommonCategory(pCategory);
	pSR->ShowScreenV2(CString(strTableName),m_unCurMenuID,CString(bstrCriteria),msg);*/

	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);
	/*
	if(pCommonCategoryWrapper)
	{
		*pCommonCategoryWrapper=pStaticInfo;
		pStaticInfo->UpdateEnableMainScreen(false);
		((CHSEMSCommonCategory *)pStaticInfo)->LockScreen(true);
	}*/
	((CHSEMSCommonCategory *)pStaticInfo)->ShowMenuScreenV2(CString(strTableName),m_unCurMenuID,CString(bstrCriteria),msg);
	return S_OK;
}

STDMETHODIMP CHSEObj::ShowAbout()
{
	m_Connection.m_pSecurityConnection->ShowSplashScreen(APP_CODE);
	return S_OK;
}

BOOL CHSEObj::tryit()
{
	CCategory *pCategory=MakeCategory();
	if(pCategory==NULL)
		return S_FALSE;
	CCommonCategoryWrapper *pStaticInfo = new CHSEMSCommonCategory(pCategory);

	CString strTableName="HSE_DRLLPLN_ACTULSCNR";
	CString strSubFormTag="HSE_DRLLPLN_ACTULSCNR";

	CString strCriteria ;	
	bool bLocked = false;
	CString strMainLinkFeild = pStaticInfo->FormGetField(strTableName,"PRMKY");	
	if(strMainLinkFeild == "")
		return FALSE;	
	pCategory->GetHSEObj()->gstrScreen_tag=strSubFormTag;	
	CString strDefValues;
	strDefValues.Format("DRLFNDNGS_LNK|%s",strMainLinkFeild);	
	strCriteria.Format("SELECT * FROM HSE_DRLFNDNGS WHERE DRLFNDNGS_LNK = '%s'",strMainLinkFeild);
	
	
	//m_pCategory->GetHSEObj()->DoFunction(0,strTableName,NORMAL_MODE,false,strCriteria,"",strDefValues,"",-1,NULL);	
	int nRetVal=0;
	m_strScreenDefauls = strDefValues;
	CWaitCursor wc;
	ATOM ThisScreenAtom=FindAtom(strTableName);
	if(ThisScreenAtom)
	DeleteAtom(ThisScreenAtom);
	
	strTableName.MakeUpper();
	m_nStatus=NORMAL_MODE;
	
	SetCurrentMenuItemID(ID_HSE_DrlFndngs);
	//((CHSEMSCommonCategory *)pStaticInfo)->ShowDrlFndngs(strCriteria);
	pStaticInfo->m_nCurrentCard=ID_HSE_DrlFndngs;
	CString strComeFrom = pCategory->GetHSEObj()->gstrScreen_tag;
	BOOL bAllowUpdate = TRUE;
	BOOL bAllowAddNew = TRUE;
	BOOL bAllowDelete = TRUE;
	if(strComeFrom == "HSE_DRLLPLNINQ_ACTULSCNR") {
		bAllowUpdate = FALSE;
		bAllowAddNew = FALSE;
		bAllowDelete = FALSE;		
	}	
	pStaticInfo->ParentManage("HSE_TGDRLFNDNGS",bAllowUpdate,bAllowAddNew,bAllowDelete,"HSE_DRLFNDNGS",strCriteria);	

	POSITION pos= m_pCategoryList->GetTailPosition();	
	//m_pCategoryList->GetPrev(pos);	
	CObject* prevObj=m_pCategoryList->GetAt(pos);
	CCategory *prevCategory= (CCategory *)prevObj;
	if (prevCategory != NULL)
	{
		bool bPrevModal = prevCategory->IsModalPolyDb();
		//if(!bPrevModal)
		//	m_strScreenDefauls = "";
	}
	m_bModalPolyDb=FALSE;
	m_bLockModalFlag=FALSE;
	m_nStatus=NORMAL_MODE;
	if(ThisScreenAtom)
	{
		AddAtom(strTableName);
	}
	wc.Restore();
	wc.~CWaitCursor();
	return nRetVal;
}

void CHSEObj::CreateGlabalTempTable()
{
	return;
	CCategory *pCategory=MakeCategory();
	pCategory->ExecuteSQL(CString("CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))"));
	delete pCategory;
}