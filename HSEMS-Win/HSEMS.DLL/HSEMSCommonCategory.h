// HSEMSCommonCategory.h: interface for the CHSEMSCommonCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSEMSCOMMONCATEGORY_H__1E87484B_9B47_4A9A_B21B_E4EABE43A2A3__INCLUDED_)
#define AFX_HSEMSCOMMONCATEGORY_H__1E87484B_9B47_4A9A_B21B_E4EABE43A2A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommonCategoryWrapper.h"
#include "IReasonCaller.h" 

enum ACTION
{
 APPROVE,
 REJECT,
 CLOSED,
 COMPLETE,
 CONFIRM,
 REJECTF,
 CREATED,
 ADTRSADTEES,
 CHKLSTENTRY,
 CHKLSTFDBCK, 
 CARFDBCK, 
 CARFLWUPVST, 
 DELETED,
 ACTIVATED,
 CANCELED
};

//From FCI.H
#define I_M_H_AL_NOR			0
#define I_UNIQUE				1
#define I_LOCKED				2
#define I_IN_DEC				3
#define I_LINKED				4
#define I_PRIMARY				5
#define I_INDEXES				6
#define I_HIDEINLIST			7


// attribute
#define ATT_MUST			0	
#define ATT_HIDDEN			1	
#define ATT_NORMAL			3
#define ATT_EN_HIDDEN		11	
#define ATT_AR_HIDDEN		111
#define ATT_ALWAYSLOCKED	2
#define ATT_DBMUST			30
#define ATT_AL_DBMUST		5	//ATT_ALWAYSLOCKED + ATT_DBMUST
#define ATT_HI_DBMUST		6	//ATT_HIDDEN	   + ATT_DBMUST by mahmoud 24 sep 2004
#define ATT_UNIQUE			1
#define ATT_G_UNIQUE		2
#define ATT_LOCKED			1
#define ATT_INCREMENT		0
#define ATT_DECREMENT		1
#define ATT_LINKED			1
#define ATT_MULTISELECT		2
#define ATT_KEYFIELD		3
#define LIKE_FIELD			4	//ATT_LINKED + LIKE_FIELD


//Mail template
#define ID_MAIL_UNKNOWN				-1
#define ID_MAIL_Observation_Complete 1
#define ID_MAIL_Auditee_Complete     2
#define ID_MAIL_Prfrmnc_CAR			 3
#define ID_MAIL_SiteSurvey_CAR		 4
#define ID_MAIL_Observation_CAR		 5
#define ID_MAIL_Incident_CAR		 6
#define ID_MAIL_Audit_CAR			 7
#define ID_MAIL_CAR_NOT_ACCPTD		 8
#define ID_MAIL_CAR_CLOSE			 9
#define ID_MAIL_Prfrmnc_UPDATE_CAR		10
#define ID_MAIL_SiteSurvey_UPDATE_CAR	11
#define ID_MAIL_Observation_UPDATE_CAR	12
#define ID_MAIL_Incident_UPDATE_CAR		13
#define ID_MAIL_Audit_UPDATE_CAR		14
#define ID_MAIL_Prfrmnc_CAR_WRK_NOT_ACCPTD		15
#define ID_MAIL_SiteSurvey_CAR_WRK_NOT_ACCPTD	16
#define ID_MAIL_Observation_CAR_WRK_NOT_ACCPTD	17
#define ID_MAIL_Incident_CAR_WRK_NOT_ACCPTD		18
#define ID_MAIL_Audit_CAR_WRK_NOT_ACCPTD		19

#define ID_POPUPS_SUMMARY_NC_DESCRIPTION	3005
#define ID_POPUPS_SUMMARY_NC_DESCRIPTION	3006

class CHSEMSCommonCategory : public CCommonCategoryWrapper,IReasonCaller  
{
public:
	CHSEMSCommonCategory(CCategory *pCategory);
	virtual ~CHSEMSCommonCategory();
	//CString GetHSEMSPolicyFullRecord(CString PlcFieldName);
	//CString GetMaxTracingSerial(CString tableName,CString columnName,CString strWhereCondtn);
	void InsertRecordIntoTracing(CString actionName,CString tableName,CString linkField,CString rejectReason);
	void InsertActionRecordIntoTracingTab(CString strMainTablePart,CString strLinkFieldPart,ACTION act);	
	//void InsertFlagIntoRjctRsn(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	bool CHSEMSCommonCategory::IsNewRjcRsnExist(CString tableName,CString keyField);
	//void UpdateRjctRsnSrl(CString maxTracingSerial,CString strMainTablePart,CString keyField);
	bool CheckPolicy(CString fieldName);
	CString GetPolicyTabField(CString strTableName,CString fieldName);
	bool UpdateStatus(MultiSelectedRows *pMultiSelectedRows,int nStatus,CString tableNamePart,CString keyValue);
	bool EnableField(TOOLBARBUTTONCLICKED *pToolBarButtonClicked,CString FieldToCheck,CString FieldToEnable);
	int getNumberOfRecord(CString tableName,CString strKeyField,CString strColumnName);
	bool SetRjctRsnFlag(TOOLBARBUTTONCLICKED *pToolBarButtonClicked,CString keyField);
	//Overrides
	virtual HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	//Overrides
	virtual HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	//bool UpdateStatusForAudit(MultiSelectedRows *pMultiSelectedRows,int nStatus,CString keyValue,CString strFormTag);	
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayTabSelected(tag_TabSelected *pTabSelected);
	bool showApproveMessageBox(CString strModuleName,CString strKeyField);
	void selectEmployeeType(FIELDCHANGE *pFieldChange,CString strTableTagName);
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	void Reject(CString strSourceModule,CString strKeyField);
	void Accept(CString strSourceModule,CString strKeyField,bool bLocked=false);
	void Approve(CString strSourceModule,CString strKeyField,bool bLocked=false);
	void setSrcKyFldNm(CString strKeyField);
	CString getSrcKyFldNm();
	void setOwnerScreenTag(CString strScreenTag);
	CString getOwnerScreenTag();
	void viewRejectReason(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void setgbOpenRejectScreenLocked(bool bOpenRejectScreenLocked);
	bool getgbOpenRejectScreenLocked();
	bool getgbOpenAcceptScreenLocked();
	void setgbUpdateRejectedRecord(bool bRejectCurrentRecord);
	void setgbUpdateAcceptedRecord(bool bAcceptCurrentRecord);
	void setgbUpdateApprovedRecord(bool bApproveCurrentRecord);
	bool getgbUpdateRejectedRecord();
	bool getgbUpdateAcceptedRecord();
	bool getgbUpdateApprovedRecord();
	void GenerateTXNNo(CString strFormTag);
	void OpenRequiredActionScreen(CString ScreenTag,int ScreenMode);
	CString getDefCurrency(CString strCmpnyNam);
	void setChangedRecInfo(CString strTableName);
	CString getTXNKeyFldVal(CString strFormTag);
	CString getCmpnyNamFrmSit(CString strSitNam);
	void generateCAR(CString strFormTag,CString strSubFormTag);
	void addNewVersion(CString strTableName,CString strFormTag,CString strSubFormTag,CString strFieldMainPartName);
	void updateCAR(CString strFormTag,CString strSubFormTag);
	void cancelCAR(CString strFormTag,CString strSubFormTag);
	void opnJobHzrdCnsqncs();
	void opnAdtGrpAdtChkLst();
	void accptWrk(CString strTableName,CString strFormTag,CString strSubFormTag);
	void rjctWrk(CString strTableName,CString strFormTag,CString strSubFormTag);
	CString GetKeyFieldName(CString strTableName);
	void insertIntoTracing(CString strFormTag,ACTION action);
	void OpenMsonRstLog(CString ScreenTag,int ScreenMode);
	CString getFileSize(CString strFilePath);
	CString getFileExtension(CString strFilePath);
	CString getMsrArLineNo(CString strMsrNo); 
	CString getPrevInjry(CString strTableNam,CString strFldNam,CString strEmpNum,CString strSit,CString strLoc);
	void openInjueryRsltScr(CString strFormTag);
	bool isItemRqrChkQOH(CString strItemKey);
	CString getEmpDep(CString strUserID,bool bgetFirstDepOnly);

	//Implement IReasonCaller 
	virtual void getParameters(CString &strOwnerScreenTag,CString &strCallerIdInitial,CString &strCallerIdFinal,CString &strPrmKey,bool &bEnableResults,CString &strReasonsFilter,CString &strRequiredCaption);

	/***********************************************************************************************************************/
	long ShowSite(CString strCriteria);
	long ShowSiteType(CString strCriteria);
	long ShowCountry(CString strCriteria);
	long ShowLanguage(CString strCriteria);
	long ShowDepartment(CString strCriteria);
	long ShowWorkLocation(CString strCriteria);
	long ShowTask(CString strCriteria);
	long ShowDetails(CString strCriteria);
	long ShowCARBasis(CString strCriteria);
	long ShowRole(CString strCriteria);
	long ShowVehicleAccidentQuestions(CString strCriteria);
	long ShowBasicReference(CString strCriteria);
	long ShowEnvironmentCondition(CString strCriteria);
	long ShowCurrency(CString strCriteria);
	long ShowSignatures(CString strCriteria);
	long ShowInjuryType(CString strCriteria);
	long ShowRewardType(CString strCriteria);
	long ShowPotentialSeverity(CString strCriteria);
	long ShowPerformanceMeasureScore(CString strCriteria);	
	long ShowAccidentMedium(CString strCriteria);
	long ShowTypeofLoss(CString strCriteria);
	long ShowTaskAuditResult(CString strCriteria);
	long ShowPTWType(CString strCriteria);
	long ShowRiskSeverityRatingClass(CString strCriteria);
	long ShowRiskLikelihoodRating(CString strCriteria);
	long ShowEmployee(CString strCriteria);
	long ShowRiskRankDescription(CString strCriteria);
	long ShowHSEProceduresList(CString strCriteria);
	long ShowPTWResponsibility(CString strCriteria);
	long ShowEntryRejectReasons(CString strCriteria);
	long ShowHSERegulations(CString strCriteria);
	long ShowAuditees(CString strCriteria);
	long ShowAttachmentclassification(CString strCriteria);
	long ShowHSEPolicy(CString strCriteria);
	long ShowCompanyProfile(CString strCriteria);
	long ShowBodyParts(CString strCriteria);
	long ShowHSEProceduresListInquiry(CString strCriteria);
	long ShowHSEToolsEquipments(CString strCriteria);
	long ShowPopupEntry(CString strFormTag, CString strCriteria);
	long ShowControlMeasuresScreen(CString strCriteria);
	long ShowSubDetails(CString strCriteria);
	long ShowDtlForChkLst(CString strCriteria);	
	long ShowPrecautions(CString strCriteria);
	long ShowContactMethods(CString strCriteria);
	long ShowAtchmntFrmts(CString strCriteria);
	long ShowTrades(CString strCriteria);
	long ShowFirstInfo(CString strCriteria);
	long ShowDocType(CString strCriteria);
	long ShowAdtActvtRqrdActn(CString strCriteria);
	long ShowRskSvrtyCntrlMsr(CString strCriteria);
	long ShowAdtChkLstPopUp(CString strCriteria);
	long ShowHSEPrjcts(CString strCriteria);
	long ShowHSEAdtTyp(CString strCriteria);
	long ShowHSERskRat(CString strCriteria);
	long ShowHSEIncdntPrd(CString strCriteria);
	long ShowHSEJobStp(CString strCriteria);
	long ShowHSECnsqncs(CString strCriteria);
	long ShowHSEAdtGrp(CString strCriteria);
	long ShowHSEAdtChcklst(CString strCriteria);
	long ShowAdtRschdl(CString strCriteria);
	long ShowAdtRschdlBrws(CString strCriteria);
	long ShowAdtCncl(CString strCriteria);	
	long ShowHSERltdOprtns(CString strCriteria);	
	long ShowHSEMtrlRlsTyp(CString strCriteria);	
	long ShowHSECntnmntTyp(CString strCriteria);	
	long ShowHSEMtrlRlsCat(CString strCriteria);	
	long ShowHSERlsMtrl(CString strCriteria);	
	long ShowHSEUOM(CString strCriteria);	
	long ShowHSEMtrlRlsTo(CString strCriteria);	
	long ShowHSESplClsfctn(CString strCriteria);	
	long ShowHSEAstTyp(CString strCriteria);
	long ShowInjryAnlysisPopup(CString strCriteria);
	long ShowHRCTRLBrws(CString strCriteria);
	long ShowHSEJobStpDfntn(CString strCriteria);
	long ShowHSECnsqncsDfntn(CString strCriteria);
	long ShowSftyInstrctn(CString strCriteria);
	long ShowVclTyp(CString strCriteria);
	long ShowMsnRsLog(CString strCriteria);
	long ShowDrlFndngs(CString strCriteria);
	long ShowHSECntrctr(CString strCriteria);
	long ShowHSEMnDrlTyp(CString strCriteria);
	long ShowHSEIncdntMdclRprt(CString strCriteria);
	long ShowInvlvdPrts(CString strCriteria);
	long ShowPckngMthds(CString strCriteria);
	long ShowWstItms(CString strCriteria);
	long ShowWstZns(CString strCriteria);
	long ShowWstTrnsprtrs(CString strCriteria);
	long ShowWstTrtmntPlnts(CString strCriteria);
	long ShowEnvMntrActvts(CString strCriteria);
	long ShowEnvMsrTyps(CString strCriteria);
	long ShowEnvMsrEntryPoltantPrpCAR(CString strCriteria);
	long ShowObsrvtnTrgts(CString strCriteria);
	long ShowEQTyps(CString strCriteria);
	long ShowEnvMntrActvtsPltnt(CString strCriteria,CString strComeFrom);
	long ShowHSEERTList(CString strCriteria);
	long ShowHSEStpEffct(CString strCriteria);
	long ShowHSERskLvl(CString strCriteria);
	long ShowHSETopEvnts(CString strCriteria);
	long ShowHSETopEvntsThrtsBrs(CString strCriteria);
	long ShowHSEEnvmntrRqrdActn(CString strCriteria);
	long ShowLfSavngRls(CString strCriteria);
	long ShowAwrnsCrs(CString strCriteria);
	long ShowAwrnsPlnCrsAtnds(CString strCriteria);

	long ShowCMSRsns(CString strCriteria);
	long ShowCMSActnRsn(CString strCriteria);
	
	long ShowRscuEntts(CString strCriteria);
	long ShowCnfndSpcTyps(CString strCriteria);
	long ShowRscuHzrds(CString strCriteria);
	long ShowRscuTls(CString strCriteria);
	long ShowRscuCmmunctnMthds(CString strCriteria);	
	long ShowHzrdPrcs(CString strCriteria);
	
	//////////////////////////////////////////////////////////////////////////
	// HSE BGC-EZZ Requirements_2892020
	long ShowRskTyp(CString strCriteria);
	long ShowMdclAuthrty(CString strCriteria);
	long ShowMdclDcsn(CString strCriteria);
	long ShowAudtrs(CString strCriteria);
	long ShowEnvrnmntlMsrs(CString strCriteria);
	long ShowIncdntMdclRprtFlwUp(CString strCriteria);
	long ShowDrlFndngsUnPlndScnros(CString strCriteria);
	//////////////////////////////////////////////////////////////////////////

	//Survey Type
	long ShowSitSrvyTyps(CString strCriteria);

	//long ShowAccidentType(CString strCriteria);
	/***********************************************************************************************************************/

	CString IsPhysicaldeleteAllowed(CString strCmpnyNam);

	long ShowScreenV2(CString strTableName,int nScreenId,CString strCriteria,void* msg);

	//HSE_RV
	
	long ShowMenuScreenV2(CString strTableName,int nScreenId,CString strCriteria,void* msg);
	UINT ShowMenuScreenV2(CString strScreenTag,CString strCriteria);

	void LockScreen(BOOL bLock);
	CString GetEmployeeCodeForLoginUser();

	CString	m_strActionReasonGroup,
		    m_strReasonCaption;

	CString m_FSATableName;//RQ-12-2023.3
			

protected:
	BOOL m_bDisableSerial;
	CString m_strSrcKyFldNm;
	char* m_pszThirdLevelScreens;



	int m_nMaxLateDays;
////////////////////////////////////////////////////////////////
	//Employee Screen
//	bool m_IsDriver=false;
//	int m_EmployeeNo;
//	int m_NoOfDrvLcRecords;
///////////////////////////////////////////////////////////////


	//Eng Hossam Reject Reason Screen Attributes
	CString	m_strInitial,
			m_strFinal;
	bool	m_bEnableResults;
	CString GenerateRandomString(int RndSeed,int nLength);
	CString m_TracingTabTag,
		m_TracingTabPrimKey,
		m_strScreen,
		m_strMainLink,
		
		m_strStatusFieldName,
		m_strPrefix,
		m_strAppPrefix;
		


	void EmptySeverityRankInChangeSeverityRating(CString strTableName , CString strFieldName,CString strTablePart);
	void EmptySiteHirarchyInChange(CString strTableName ,CString strFieldName,CString strTablePart);
	void EmptyHazardHirarchyInChange(CString strTableName ,CString strFieldName,CString strTablePart);
	void EmptySubClassInChangeClass(CString strTableName ,CString strFieldName,CString strTablePart);
	void EmptyEmployeeInfoWhenChangeCompanyEmplyee(FIELDCHANGE *pFieldChange,CString strTableName ,CString strFieldName,CString strTablePart);
	void EmptySeverityRate(CString strTableName ,CString strFieldName,CString strTablePart);
	void EmptyDetlCodeInChangGrp(CString strTableName ,CString strFieldName,CString strTablePart);
	CString GetTableName(CString strTag);
	CString GetTablePartOfFieldName(CString strTag);
	CString FilterSubClassOnSelectedClass(CString strSubFormTag ,CString strFieldName);
	CString GetTableNameBrwsOnEmp(CString FieldName);
	CString GetLnkFldNameForBrowseTable(CString strTableName);
	CString FilterAditeesEmpOnAudteeDpt(CString ActivityKeyField);
	CString GetSlctdEmp(CString strFieldName,CString strTableName,CString LnkFieldName,CString LnkFieldValue);
	CString FilterEmployees(CString DeptWhereCondition,CString strSelectedEmployees);
	CString GetAllEmployees(CString DeptWhereCondition);
	double GetMaxFileSize(CString strFileType);
	BOOL ValidFileSize(CString strFileType,double nFileSize,CString strCmpnyNam);
		
	//polydb_task
	//HRESULT FormSetField(CString strTable,CString strField,CString strValue);
	
	CString GetKeyField(CString strTableName,MultiSelectedRows *pMultiSelectedRows);
	//void manageCommentsTabToolBar(RECORDREPOSTION *pRecordRepostion);
	void manageCommentsTabToolBar(CString strFormTag,CString strSubFormTag);
	void enableCmntTabFields(CString strSubFormTag);
	void disableCmntTabFields(CString strSubFormTag);
	void enableCmntTabToolBar(CString strFormTag,CString strSubFormTag);
	void disableCmntTabToolBar(CString strFormTag,CString strSubFormTag);
	void setCmntScreenSource(CString strFormTag,CString strSubFormTag,CString strTableName);
	void openRelatedCARs(CString strCARNo);
	void generateCARSP(CString strTXN,CString strSubFormtag);
	int  GetMaxLateDays(CString strCmpnyNam);
	bool IsIssueDateValid(CString strIssueDate,CString strCmpnyNam);
	CString getOwnerDept(CString strCmnyNam);
	void filterAdtChkLst(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	CString GetSingleCompanyName();
	bool checkAttachmentFormat(CString strCmpnyNam);
	bool checkRequireCAR() ;
	bool checkAttachmentSize(CString strCmpnyNam);
	int getEmployeeAge(CString strEmpCode);
	int getEmployeeServiceYears(CString strEmpCode);
	int getEmployeeTotalExpYears(CString strEmpCode);
	CString getCompanyCurreny(CString strCmpnyNam);
	bool hasPendingRequiredActions(CString strTableName,CString strKeyFldVal);
	void getRskAnalysisCntrlMsr();
	int getAdtAdvDays(CString strCompnyNam);
	bool IsAuditValidToAssignRes(CString strAdtTodate,CString strCmnyNam);
	bool updateCARSP(CString strTXN,CString strSubFormtag,CString strCARNum);
	void cancelCARSP(CString strTXN,CString strSubFormtag,CString strCARNum);
	int getAdtMinNotificationDays(CString strCompnyNam);
	CString getDepMail(CString strDepCode);
	void sendGenerateCARNotification(CString strTableName);
	void sendUpdateCARNotification(CString strDepMail,CString strCARSerial,CString strTableName,int nMsgNumber);
	void closeCARNotification(CString strDepMail,CString strCARSerial);
	void sendwrkNotAccptdNotification(CString strDepMail,CString strCARSerial,CString strTableName,int nMsgNum);
	void sendCARNotAccptdNotification(CString strDepMail,CString strCARSerial); 
	CString getHSEDepMail(CString strCmpnyNam);
	CString getAuditeeMail(CString strAdtGrp,CString strAdtCOD);
	int getTXNMsgBody(CString strTableName);
	int getUpdateCARMsgNum(CString strTableName);
	int getCARWrkNotAcptdMsgNum(CString strTableName);
	CString IsMultiMtrlRlsAllowed(CString strCmpnyNam);	
	CString getMaxAcptdRskRnk(CString strCompnyNam);
	void OpenBodyParts(CString strFormTag = "");
	void openInjuryType(CString strFormTag);
	void completeIncdntMdclRprt(CString strKeyVal);
	void closeIncdntMdclRprt(CString strKeyVal);

	bool OpenReasonsScr(CString strReasonCaption,CString strActionReasonGroup,bool bEdit);
	bool CompleteUpdateRecordStatusAndInsertIntoTracingTab(CString strNewState,CString strStateID,CString m_strFinal);
		

//	int GetEmployeeDriverLicenseRecord(m_EmployeeNo);
//	bool CheckDriver(m_EmployeeNo);
//	bool IsEmployeeDataValidForSave(m_EmployeeNo);


	//polydb
	//HRESULT SetFieldVisibleAttrib(CString strScreenTag,CString strFieldName,CString strAttribute);
	HRESULT ShowListScreen(CString strList_Tag,CString strList_Title,CString strSQL_Statement,CString strKeyField,CString strRecKeyValue,BOOL bMultiSelection,tagMultiSelectedRows* pstrFieldsValues,BOOL bIsModal=FALSE,CString strAttrib = "");
	void LockTab(CString strMainScreenTag,CString strTabTag,BOOL Locked);
	long OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal=TRUE,BOOL bFilterMode=FALSE);	
	CString getNxtSrl(CString strSerialFldNam,CString strTblNam,CString strTabLinkFldNam = "",CString strHeaderFldVal = "",CString strWhereCluase = "");

	//General helper functions 
	void getNewSerialNoForATab(CString strMasterTable,CString strMasterKey,CString strDetailTable,CString strDetailLink,CString strSerialField); 
	CMapStringToOb* InitializeParameters(char* strFSATableName,char* strFormTable,char* strSerialFieldName,char* strMasterKeyField,char *pszMainSerialDependentKey=NULL,char *pszSpecial=NULL);
	/*void AddTabSerialInfo(CMapStringToOb*pmpListOfParameters,char * pszTabName,char * pszTabSerialField);
	void AddTabMsterLinkInfo(CMapStringToOb*pmpListOfParameters,char * pszTabName,char * pszTabLinkField);*/
	void AddTabInfo(CMapStringToOb*pmpListOfParameters,char *KeyName,char * pszTabName,char * pszKeyValue);

	void OpenPopUpScreen(CString strParentTableName,CString strParentFormTag,CString strParentTablePKName,CString strPopUpTbleName,CString strPopUpTagName,CString strPopUpEngCaption,CString strPopUpArbCaption,CString strPopUpLinkField,int nScreenID,CString strAddedCrireia="");
	
private:	
	void InitRandomReasonsParam();
};

#endif // !defined(AFX_HSEMSCOMMONCATEGORY_H__1E87484B_9B47_4A9A_B21B_E4EABE43A2A3__INCLUDED_)
