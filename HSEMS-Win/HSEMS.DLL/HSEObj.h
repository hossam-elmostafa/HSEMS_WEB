
// HSEObj.h : Declaration of the CHSEObj

#ifndef __HSEOBJ_H_
#define __HSEOBJ_H_

#include "resource.h"       // main symbols
#include <atlwin.h>

/////////////////////////////////////////////////////////////////////////////
// CHSEObj
#include "CMNObjBase.h"
class CDongleListen;
class ATL_NO_VTABLE CHSEObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHSEObj, &CLSID_HSEObj>,
	public IDispatchImpl<IHSEObj, &IID_IHSEObj, &LIBID_HSEMSLib>,
	CCMNObjBase
{
public:
	CHSEObj();
	~CHSEObj();
	void	UpdateFirstSysByAllowDuplicateItems(BOOL AllowDuplicateItems);
	CString ReadLFAValidation(CString strApp,CString strTag,CString strLinkName,CString strField);
	void UpdateLFAValidation(_DB * pdb,CString strApp,CString strTag,CString strLinkName,CString strField,CString strValidation);
DECLARE_REGISTRY_RESOURCEID(IDR_HSEOBJ)

BEGIN_COM_MAP(CHSEObj)
	COM_INTERFACE_ENTRY(IHSEObj)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IHSEObj
public:
	STDMETHOD(GetDBType)();
	STDMETHOD(SetFormRect)(int nLeft, int nTop, int nRight, int nBottom);
	STDMETHOD(ShowMailServerSetting)();
	STDMETHOD(ShowMailWizard)();
	STDMETHOD(ShowAutoMailSetupScreen)();
	STDMETHOD(GetUserName)(BSTR* bstrName);
	STDMETHOD(DoFunction)(int nCurrentCard,BSTR bstrTableName,int nStatus,BOOL bModal,BSTR bstrWhereClause);
	STDMETHOD(SetFirstDesktopObj)(LPDISPATCH lpFirstDesktopObj);
	STDMETHOD(ActivateForm)(BSTR bstrFormID,BOOL bByTag, int nSkipNo,BOOL bActivateLoadedScreen);
	STDMETHOD(GetConnectionString)(/*[out]*/ BSTR* bstrConnection);
	STDMETHOD(MapUserFromApp)(BSTR bstrName,int nAppID);
	STDMETHOD(BuildMenu)(long lMenuHandle,BOOL bLanguage);
	STDMETHOD(UpdateMenu)(int nAppCode,long lMenuHandle);
	STDMETHOD(ShowReportsPriv)();
	STDMETHOD(GetPaneString)(BSTR* strPaneText);
	STDMETHOD(SetModal)(BOOL bModalVal);
	STDMETHOD(CloseAll)();
	STDMETHOD(SetSystemPath)();
	STDMETHOD(UserVerify)(int nUserCode,int nMenuItem,BSTR* bstrCaption);
	STDMETHOD(ShowUsersList)();
	STDMETHOD(ShowFieldsPriv)();
	STDMETHOD(ShowMenuScreen)(long lMenuHandle);
	STDMETHOD(ShowUsersScreen)(BOOL* bValid);
	STDMETHOD(ShowSetupScreen)();
	STDMETHOD(SetUserID)();
	STDMETHOD(SetLanguage)(BOOL bArabic);
	STDMETHOD(GetLanguage)(BOOL *bArabic);
	STDMETHOD(Initialize)(int nAppCode,BSTR bstrProfile);
	STDMETHOD(SetParent)(long pParent);
	STDMETHOD(GetHelpFileName)(BSTR *bstrFolder);
	STDMETHOD(SetFastSearchCriteria)(BSTR bstrCriteria);
	STDMETHOD(GetUserID)();
	STDMETHOD(SetCurrentMenuItemID)(UINT unMenuID);
	STDMETHOD(ReportsReportsList)();
	STDMETHOD(AnalysisReportTypes)();
	STDMETHOD(UtilitiesCodeSegments)(BSTR strAppCode);
	STDMETHOD(UtilitiesUserDefinedChecks)();
	STDMETHOD(ShowScreenFromAttachment)(BSTR bstrAttachmentFile);
	STDMETHOD(ShowAbout)();

	STDMETHOD(ShowAccidentEntry)(BSTR bstrCriteria);					//ID_HSE_ACDNTENT
	STDMETHOD(ShowSite)(BSTR bstrCriteria);								//ID_HSE_STE:
	STDMETHOD(ShowSiteType)(BSTR bstrCriteria);							//ID_HSE_SITE_TYPE:
	STDMETHOD(ShowCountry)(BSTR bstrCriteria);							//ID_HSE_CNTRY:
	STDMETHOD(ShowLanguage)(BSTR bstrCriteria);							//ID_HSE_LANG:
	STDMETHOD(ShowAuditCheckListPlan)(BSTR bstrCriteria);				//ID_HSE_AUDITCHECKLISTRSLT:
	STDMETHOD(ShowAuditCheckListResult)(BSTR bstrCriteria);				//ID_HSE_AUDITCHECKLIST:
	STDMETHOD(ShowRejectreason)(BSTR bstrCriteria);						//ID_HSE_RJCT:
	STDMETHOD(ShowAcceptreason)(BSTR bstrCriteria);
	STDMETHOD(ShowApprovReason)(BSTR bstrCriteria);
	STDMETHOD(ShowDepartment)(BSTR bstrCriteria);						//ID_HSE_DPRT:
	STDMETHOD(ShowWorkLocation)(BSTR bstrCriteria);						//ID_HSE_WRKLOC:
	STDMETHOD(ShowTask)(BSTR bstrCriteria);								//ID_HSE_TSK:
	STDMETHOD(ShowControlMeasures)(BSTR bstrCriteria);					//ID_HSE_CTRLMSRMNT:
	//STDMETHOD(ShowControlMeasuresScreen)(BSTR bstrCriteria);
	STDMETHOD(ShowHazard)(BSTR bstrCriteria);							//ID_HSE_HZRD:
	STDMETHOD(ShowPerformanceMeasurementCheckList)(BSTR bstrCriteria);	//ID_HSE_PERFMSRMNTCHKLIST:
	STDMETHOD(ShowAccidentReason)(BSTR bstrCriteria);					//ID_HSE_ACDNTRSN:
	STDMETHOD(ShowAuditors)(BSTR bstrCriteria);							//ID_HSE_ADTORS:
	STDMETHOD(ShowAudites)(BSTR bstrCriteria);							//ID_HSE_ADTEES:
	STDMETHOD(ShowHarmTypes)(BSTR bstrCriteria);						//ID_HSE_HRMTYP:
	STDMETHOD(ShowDetails)(BSTR bstrCriteria);							//ID_HSE_DTL:
	STDMETHOD(ShowCopyofdetail)(BSTR bstrCriteria);						//ID_HSE_DTLCPY:
	STDMETHOD(ShowCARBasis)(BSTR bstrCriteria);							//ID_HSE_CARMODELBAS:
	STDMETHOD(ShowRole)(BSTR bstrCriteria);								//ID_HSE_ROL:
	STDMETHOD(ShowVehicleAccidentQuestions)(BSTR bstrCriteria);			//ID_HSE_VCLQSTNS:
	STDMETHOD(ShowBasicReference)(BSTR bstrCriteria);					//ID_SYSTEMSETTINGS_BASICREFERENCE:
	STDMETHOD(ShowEnvironmentCondition)(BSTR bstrCriteria);				//ID_SYSTEMSETTINGS_ENVIRONMENTCONDITION:
	STDMETHOD(ShowPersonalProtectiveEquipmentpopUp)(BSTR bstrCriteria); //ID_SYSTEMSETTINGS_PERSONALPROTECTIVEEQUIPMENT:
	STDMETHOD(ShowPersonalProtectiveEquipmentScreen)(BSTR bstrCriteria);
	STDMETHOD(ShowCurrency)(BSTR bstrCriteria);							//ID_SYSTEMSETTINGS_CURRENCY:
	STDMETHOD(ShowSignatures)(BSTR bstrCriteria);						//ID_SYSTEMSETTINGS_SIGNATURES:
	STDMETHOD(ShowInjuryType)(BSTR bstrCriteria);						//ID_SYSTEMSETTINGS_INJURYTYPE:
	STDMETHOD(ShowInjuryTypepopUpscreen)(BSTR bstrCriteria);			//ID_HSE_INJUERYTYPE_POP_UP:
	STDMETHOD(ShowRewardType)(BSTR bstrCriteria);						//ID_SYSTEMSETTINGS_REWARDTYPE:
	STDMETHOD(ShowPotentialSeverity)(BSTR bstrCriteria);				//ID_SYSTEMSETTINGS_POTENTIALSEVERITY:
	STDMETHOD(ShowPerformanceMeasureScore)(BSTR bstrCriteria);			//ID_HSE_PERFMSRMNTSCOR:
	STDMETHOD(ShowAccidentType)(BSTR bstrCriteria);						//ID_HSE_ACDNTTYP:
	STDMETHOD(ShowAccidentMedium)(BSTR bstrCriteria);					//ID_HSE_ACDNTMDM:
	STDMETHOD(ShowTypeofLoss)(BSTR bstrCriteria);						//ID_HSE_TYPLOSS:
	STDMETHOD(ShowHumanBodyParts)(BSTR bstrCriteria);					//ID_HSE_HMNBODYPART:
	STDMETHOD(ShowTaskAuditResult)(BSTR bstrCriteria);					//ID_HSE_TSKADTRSLT:
	STDMETHOD(ShowPTWType)(BSTR bstrCriteria);							//ID_HSE_PTWSTYP:
	STDMETHOD(ShowRiskSeverityRatingClass)(BSTR bstrCriteria);			//ID_HSE_RSKSVRTYRATCLS:
	STDMETHOD(ShowRiskLikelihoodRating)(BSTR bstrCriteria);				//ID_HSE_RSKLKLYHODRAT:
	STDMETHOD(ShowEmployee)(BSTR bstrCriteria);							//ID_HSE_EMP:
	STDMETHOD(ShowRiskRankDescription)(BSTR bstrCriteria);				//ID_HSE_RSKRNKDESC:
	STDMETHOD(ShowHSEProceduresList)(BSTR bstrCriteria);				//ID_HSE_HSEPRCDRLIST:
	STDMETHOD(ShowPTWResponsibility)(BSTR bstrCriteria);				//ID_HSE_PTWSRSPNSBLTY:
	STDMETHOD(ShowEntryRejectReasons)(BSTR bstrCriteria);				//ID_HSE_ENTRJCRSN:
	STDMETHOD(ShowHSERegulations)(BSTR bstrCriteria);					//ID_HSE_HSERGLR:
	STDMETHOD(ShowNecessaryControlMeasures)(BSTR bstrCriteria);			//ID_HSE_NCSSRYCTRLMSRMNT:
	STDMETHOD(ShowRiskAssessmentEntry)(BSTR bstrCriteria);				//ID_HSE_RSKASSMNTENT:
	STDMETHOD(ShowRiskAssessmentApproval)(BSTR bstrCriteria);			//ID_HSE_RSKASSMNTAPR:
	STDMETHOD(ShowRiskAssessmentFollowUp)(BSTR bstrCriteria);			//ID_HSE_RSKASSMNTFOLLWUPDT:
	//STDMETHOD(ShowAuditors)(BSTR bstrCriteria);						//ID_HSE_AUDITORS:
	STDMETHOD(ShowAuditees)(BSTR bstrCriteria);							//ID_HSE_AUDITEES:
	STDMETHOD(ShowPotentialHazardEntry)(BSTR bstrCriteria);				//ID_HSE_PTNLHZRDENT:
	STDMETHOD(ShowPotentialHazardConfirmation)(BSTR bstrCriteria);		//ID_HSE_PTNLHZRDCNFRMTN:
	STDMETHOD(ShowPotentialhazardFollowUp)(BSTR bstrCriteria);			//ID_HSE_PTNLHZRDFOLLOWUP:
	STDMETHOD(ShowLossAccidentEntry)(BSTR bstrCriteria);				//ID_HSE_LOSSACDNTENT:
	STDMETHOD(ShowLossAccidentConfirmation)(BSTR bstrCriteria);			//ID_HSE_LOSSACDNTCNFRMTN:
	STDMETHOD(ShowPerformanceMeasurementEntry)(BSTR bstrCriteria);		//ID_HSE_PERFMSRMNTENT:
	STDMETHOD(ShowPerformanceMeasurementEvaluation)(BSTR bstrCriteria); //ID_HSE_PERFMSRMNTEVAL:
	STDMETHOD(ShowPerformanceMeasurementFollowUp)(BSTR bstrCriteria);	//ID_HSE_PERFMSRMNTFLWUP:
	STDMETHOD(ShowNearMissEntry)(BSTR bstrCriteria);					//ID_HSE_NRSTMISCENT:
	STDMETHOD(ShowNearMissConfirmation)(BSTR bstrCriteria);				//ID_HSE_NRSTMISCCNFRMTN:
	STDMETHOD(ShowNearMissFollowUp)(BSTR bstrCriteria);					//ID_HSE_NRSTMISCFLWUP:
	STDMETHOD(ShowNearMissReward)(BSTR bstrCriteria);					//ID_NEARMISS_NEARMISSREWARD:	
	STDMETHOD(ShowAccidentConfirmation)(BSTR bstrCriteria);				//ID_HSE_ACDNTCNFRMTN:
	STDMETHOD(ShowAccidentFollowup)(BSTR bstrCriteria);					//ID_HSE_ACDNTFOLLWUPDT:
	STDMETHOD(ShowCorrectiveActionEntry)(BSTR bstrCriteria);			//ID_HSE_CRCTVEACCENT:
	STDMETHOD(ShowCorrectiveActionConfirmation)(BSTR bstrCriteria);		//ID_HSE_CRCTVEACCCNFRMTN:
	STDMETHOD(ShowCorrectiveActionFollowupvisit)(BSTR bstrCriteria);	//ID_HSE_CRCTVACTNFLWUPVSTS:
	STDMETHOD(ShowPTWRegisterEntry)(BSTR bstrCriteria,BSTR bstrScreenDefaults);					//ID_HSE_PTWSRGSTRENT:
	STDMETHOD(ShowPTWRegisterConfirmation)(BSTR bstrCriteria);			//ID_HSE_PTWSRGSTRCNFRMTN:	
	STDMETHOD(ShowPTWsRgstrCls)(BSTR bstrCriteria);						//ID_HSE_PTWsRgstrCls:
	STDMETHOD(ShowAuditPlanEntry)(BSTR bstrCriteria);					//ID_HSE_ADTPLNENT:
	STDMETHOD(ShowAuditPlanApproval)(BSTR bstrCriteria);				//ID_HSE_ADTPLNAPR:
	STDMETHOD(ShowAuditPlanFlwUp)(BSTR bstrCriteria); 
	STDMETHOD(ShowAuditPlanAuditorsandAuditees)(BSTR bstrCriteria);		//ID_HSE_ADTPLNADTRSNADTES:
	STDMETHOD(ShowAuditPlanPreparechecklist)(BSTR bstrCriteria);		//ID_HSE_ADTPLNPRPERCHKLST:
	STDMETHOD(ShowAuditPlanAuditchecklistfeedback)(BSTR bstrCriteria);	//ID_HSE_ADTPLNCHKLSTFDBCK:
	STDMETHOD(ShowVehicleAccidentEntry)(BSTR bstrCriteria);				//ID_HSE_VCLACDNTENT:
	STDMETHOD(ShowVehicleAccidentConfirmation)(BSTR bstrCriteria);		//ID_HSE_VCLACDNTCNFRMTN:
	STDMETHOD(ShowVehicleAccidentFollowUp)(BSTR bstrCriteria);	//ID_HSE_VCLACDNTFLWUP:
	STDMETHOD(ShowAttachmentclassification)(BSTR bstrCriteria); //ID_HSE_ATCHCLSFC:
	STDMETHOD(ShowHSEPolicy)(BSTR bstrCriteria);				//ID_HSE_HSEPLC:
	STDMETHOD(ShowCompanyProfile)(BSTR bstrCriteria);			//ID_HSE_COMPPRF:
	STDMETHOD(ShowBodyParts)(BSTR bstrCriteria);				//ID_HSE_HUMNBDYPRT:
	STDMETHOD(ShowPotentialhazardinquiry)(BSTR bstrCriteria);	//ID_HSE_PTNLHZRDINQ:
	STDMETHOD(ShowRiskInquiry)(BSTR bstrCriteria);				//ID_HSE_RSKASSMNTINQ:
	STDMETHOD(ShowPerformanceInquiry)(BSTR bstrCriteria);		//ID_HSE_PERFMSRMNTINQ:
	STDMETHOD(ShowAccident)(BSTR bstrCriteria);					//ID_HSE_ACDNTINQ:
	STDMETHOD(ShowAudit)(BSTR bstrCriteria);					//ID_HSE_ADTPLNINQ:
	STDMETHOD(ShowCAR)(BSTR bstrCriteria);						//ID_HSE_CRCTVEACCINQ:
	STDMETHOD(ShowVehchileAccidentInquiry)(BSTR bstrCriteria);  //ID_HSE_VCLACDNTINQ:
	STDMETHOD(ShowNearMissInquiry)(BSTR bstrCriteria);			//ID_HSE_NRSTMISCINQ:
	STDMETHOD(ShowPTW)(BSTR bstrCriteria);						//ID_HSE_PPTWINQ:
	STDMETHOD(ShowLossAccidentInquiry)(BSTR bstrCriteria);		//ID_HSE_LOSSACCDNTINQ:
	STDMETHOD(ShowHSEProceduresListInquiry)(BSTR bstrCriteria); //ID_HSE_HSEPROCDRSLSTINQ:
	STDMETHOD(ShowRiskAnalysis)(BSTR bstrCriteria);		  //ID_RISKANALYSIS:
	STDMETHOD(ShowHSEToolsEquipments)(BSTR bstrCriteria); //ID_HSE_TOOLEQUIP:
	STDMETHOD(ShowSubDetails)(BSTR bstrCriteria);		  //ID_HSE_SUBDETAILS:
	STDMETHOD(ShowHSEToolsEquipmentsDistribution)(BSTR bstrCriteria); //ID_HSE_TOOLSEQUIPDSTRBTN:
	STDMETHOD(ShowPopupEntry)(BSTR bstrFormTag,BSTR bstrCriteria);
	STDMETHOD(ShowDtlForChkLst)(BSTR bstrCriteria);		//ID_HSE_DTLFORCHKLIST
	STDMETHOD(ShowNearMissTypes)(BSTR bstrCriteria);
	STDMETHOD(ShowJobPtntlHzrdsLIB)(BSTR bstrCriteria);	//ID_HSE_JobPtntlHzrdsLib
	STDMETHOD(ShowPrecautions)(BSTR bstrCriteria);		//ID_HSE_PRECAUTIONS
	STDMETHOD(ShowContactMethods)(BSTR bstrCriteria);	//ID_HSE_CNTCT_MTHDS
	STDMETHOD(ShowAtchmntFrmts)(BSTR bstrCriteria);
	STDMETHOD(ShowTrades)(BSTR bstrCriteria);			//ID_HSE_TRADES
	STDMETHOD(ShowFirstInfo)(BSTR bstrCriteria);		//ID_HSE_FIRSTINFO
	STDMETHOD(ShowDocType)(BSTR bstrCriteria);			//ID_HSE_DOCTYP
	STDMETHOD(ShowAdtActvtRqrdActn)(BSTR bstrCriteria);	//
	STDMETHOD(ShowRskSvrtyCntrlMsr)(BSTR bstrCriteria);	//ID_HSE_RSKSVRTYCNTRLMSR
	STDMETHOD(ShowAdtChkLstPopUp)(BSTR bstrCriteria);	//ID_HSE_AdtChkLstPopUp
	STDMETHOD(ShowHSEPrjcts)(BSTR bstrCriteria);		//ID_HSE_Prjcts
	STDMETHOD(ShowHSEAdtTyp)(BSTR bstrCriteria);		//ID_HSE_AdtTyp
	STDMETHOD(ShowHSERskRat)(BSTR bstrCriteria);		//ID_HSE_RskRat
	STDMETHOD(ShowHSEIncdntPrd)(BSTR bstrCriteria);		//ID_HSE_IncdntPrd
	STDMETHOD(ShowHSEJobStp)(BSTR bstrCriteria);		//ID_HSE_JobStp
	STDMETHOD(ShowHSECnsqncs)(BSTR bstrCriteria);		//ID_HSE_Cnsqncs
	STDMETHOD(ShowHSEAdtGrp)(BSTR bstrCriteria);		//ID_HSE_AdtGrp
	STDMETHOD(ShowHSEAdtChcklst)(BSTR bstrCriteria);	//ID_HSE_AdtChcklst
	STDMETHOD(ShowSitSrvyEnt)(BSTR bstrCriteria);		//ID_HSE_SitSrvyEnt
	STDMETHOD(ShowSitSrvyCnfrmtn)(BSTR bstrCriteria);	//ID_HSE_SitSrvyCnfrmtn
	STDMETHOD(ShowSitSrvyFlwup)(BSTR bstrCriteria);		//ID_HSE_SitSrvyFlwup
	STDMETHOD(ShowActualAuditors)(BSTR bstrCriteria);	//
	STDMETHOD(ShowAdtRschdl)(BSTR bstrCriteria);		//ID_HSE_AdtRschdl
	STDMETHOD(ShowAdtRschdlBrws)(BSTR bstrCriteria);	//ID_HSE_AdtRschdlBrws
	STDMETHOD(ShowAdtCncl)(BSTR bstrCriteria);			//ID_HSE_AdtCncl
	STDMETHOD(ShowIncdntFlshRprt)(BSTR bstrCriteria);	//ID_HSE_IncdntFlshRprt
	STDMETHOD(ShowHSERltdOprtns)(BSTR bstrCriteria);	
	STDMETHOD(ShowHSEMtrlRlsTyp)(BSTR bstrCriteria);	
	STDMETHOD(ShowHSECntnmntTyp)(BSTR bstrCriteria);
	STDMETHOD(ShowHSEMtrlRlsCat)(BSTR bstrCriteria);
	STDMETHOD(ShowHSERlsMtrl)(BSTR bstrCriteria);
	STDMETHOD(ShowHSEUOM)(BSTR bstrCriteria);
	STDMETHOD(ShowHSEMtrlRlsTo)(BSTR bstrCriteria);
	STDMETHOD(ShowHSESplClsfctn)(BSTR bstrCriteria);
	STDMETHOD(ShowHSEAstTyp)(BSTR bstrCriteria);
	STDMETHOD(ShowSitSrvyEntInq)(BSTR bstrCriteria);
	STDMETHOD(ShowInjryAnlysisPopup)(BSTR bstrCriteria);
	STDMETHOD(ShowHRCTRLBrws)(BSTR bstrCriteria);
	STDMETHOD(ShowHSELogsDef)(BSTR bstrCriteria);
	STDMETHOD(ShowLogsDefAttrDtl)(BSTR bstrCriteria);
	STDMETHOD(ShowHSELogsEnt)(BSTR bstrCriteria);
	STDMETHOD(ShowLogsEntDtlCnt)(BSTR bstrCriteria);
	STDMETHOD(ShowHSEJobStpDfntn)(BSTR bstrCriteria);
	STDMETHOD(ShowHSECnsqncsDfntn)(BSTR bstrCriteria);
	STDMETHOD(ShowSitSrvyRqrdActn)(BSTR bstrCriteria);	
	STDMETHOD(ShowSftyInstrctn)(BSTR bstrCriteria);	
	STDMETHOD(getDocumentationFolderPath)(BSTR* bstrPath);
	STDMETHOD(genIFCRep)();
	STDMETHOD(genEGPCMonthlyRep)();
	STDMETHOD(genTGKPIRep)();
	STDMETHOD(genFlashForms)();	
	STDMETHOD(ShowVclTyp)(BSTR bstrCriteria);
	STDMETHOD(ShowEnvronmntlAspctEntry)(BSTR bstrCriteria);
	STDMETHOD(ShowEnvronmntlAspctReview)(BSTR bstrCriteria);
	STDMETHOD(ShowDrillPlanEntry)(BSTR bstrCriteria);
	STDMETHOD(ShowDrillPlanAprv)(BSTR bstrCriteria);
	STDMETHOD(ShowDrillPlanExecution)(BSTR bstrCriteria);
	STDMETHOD(ShowIncdntFlshRprtRvw)(BSTR bstrCriteria);	//ID_HSE_IncdntFlshRprtRvw
	STDMETHOD(ShowIncdntFlshRprtInq)(BSTR bstrCriteria);	//ID_HSE_IncdntFlshRprtInq
	STDMETHOD(ShowIncdntInvstgtnEntry)(BSTR bstrCriteria);	//ID_HSE_AcdntIncdntInvstgtnEntry
	STDMETHOD(ShowIncdntInvstgtnAprv)(BSTR bstrCriteria);	//ID_HSE_AcdntIncdntInvstgtnAprv
	STDMETHOD(ShowMsnRsLog)(BSTR bstrCriteria);				//ID_HSE_MsnRsLog
	STDMETHOD(ShowDrlFndngs)(BSTR bstrCriteria);			//ID_HSE_DrlFndngs
	STDMETHOD(ShowHSECntrctr)(BSTR bstrCriteria);			//ID_HSE_CNTRCTR
	STDMETHOD(ShowHSEMnDrlTyp)(BSTR bstrCriteria);			//ID_HSE_MnDrlTyp
	STDMETHOD(ShowHSEIncdntMdclRprt)(BSTR bstrCriteria);	//ID_HSE_IncdntMdclRprt
	STDMETHOD(ShowInvlvdPrts)(BSTR bstrCriteria);			//ID_HSE_INVLVDPRTS
	STDMETHOD(ShowPckngMthds)(BSTR bstrCriteria);			//ID_HSE_PckngMthds
	STDMETHOD(ShowWstItms)(BSTR bstrCriteria);				//ID_HSE_WstItms
	STDMETHOD(ShowWstZns)(BSTR bstrCriteria);				//ID_HSE_WstZns
	STDMETHOD(ShowWstTrnsprtrs)(BSTR bstrCriteria);			//ID_HSE_WstTrnsprtrs
	STDMETHOD(ShowWstTrtmntPlnts)(BSTR bstrCriteria);		//ID_HSE_WstTrtmntPlnts
	STDMETHOD(ShowWstRcvngEntry)(BSTR bstrCriteria);		//ID_HSE_WstRcvngEntry
	STDMETHOD(ShowWstDspslEntry)(BSTR bstrCriteria);		//ID_HSE_WstDspslEntry	
	STDMETHOD(ShowEnvMntrActvts)(BSTR bstrCriteria);		//ID_HSE_EnvMntrActvts	
	STDMETHOD(ShowEnvMntrPlan)(BSTR bstrCriteria);			//ID_HSE_EnvMntrPlan	
	STDMETHOD(ShowEnvMntrExctn)(BSTR bstrCriteria);			//ID_HSE_EnvMntrExctn	
	STDMETHOD(ShowEnvMsrTyps)(BSTR bstrCriteria);			//ID_HSE_EnvMsrTyps
	STDMETHOD(ShowEnvMsrEntry)(BSTR bstrCriteria);			//ID_HSE_EnvMsrEntry
	STDMETHOD(ShowEnvMsrAprvl)(BSTR bstrCriteria);			//ID_HSE_EnvMsrAprvl
	STDMETHOD(ShowEnvMsrEntryPoltantPrpCAR)(BSTR bstrCriteria);			//ID_HSE_EnvMsrEntryPoltantPrpCAR
	STDMETHOD(ShowObsrvtnTrgts)(BSTR bstrCriteria);			//ID_HSE_ObsrvtnTrgts
	STDMETHOD(ShowEQTyps)(BSTR bstrCriteria);				//ID_HSE_EQTyps
	STDMETHOD(ShowEQInspctnRqust)(BSTR bstrCriteria);		//ID_HSE_EQInspctnRqust
	STDMETHOD(ShowEQInspctnRsult)(BSTR bstrCriteria);		//ID_HSE_EQInspctnRsult
	STDMETHOD(ShowEQInspctnWrk)(BSTR bstrCriteria);			//ID_HSE_EQInspctnWrk
	STDMETHOD(ShowEQInspctnInqry)(BSTR bstrCriteria);		//ID_HSE_EQInspctnInqry	
	STDMETHOD(ShowEnvMntrActvtsPltnt)(BSTR bstrCriteria,BSTR strComeFrom);	//ID_HSE_EnvMntrActvtsPltnt
	STDMETHOD(ShowHSEERTList)(BSTR bstrCriteria);			//ID_HSE_ERTList
	STDMETHOD(ShowHSEStpEffct)(BSTR bstrCriteria);			//ID_HSE_StpEffct
	STDMETHOD(ShowHSERskLvl)(BSTR bstrCriteria);			//ID_HSE_RskLvl
	STDMETHOD(ShowHSETopEvnts)(BSTR bstrCriteria);			//ID_HSE_TopEvnts
	STDMETHOD(ShowHSETopEvntsThrtsBrs)(BSTR bstrCriteria);	//ID_HSE_TopEvntsThrtsBrs
	STDMETHOD(ShowHSEEnvmntrRqrdActn)(BSTR bstrCriteria);	//ID_HSE_EnvmntrRqrdActn
	STDMETHOD(ShowLfSavngRls)(BSTR bstrCriteria);			//ID_HSE_LfSavngRls
	STDMETHOD(ShowAwrnsCrs)(BSTR bstrCriteria);				//ID_HSE_AwrnsCrs		
	STDMETHOD(ShowAwrnsPlnEntry)(BSTR bstrCriteria);		//ID_HSE_AwrnsPlnEntry	
	STDMETHOD(ShowAwrnsPlnAprvl)(BSTR bstrCriteria);		//ID_HSE_AwrnsPlnAprvl	
	STDMETHOD(ShowAwrnsPlnExctn)(BSTR bstrCriteria);		//ID_HSE_AwrnsPlnExctn	
	STDMETHOD(ShowAwrnsPlnInq)(BSTR bstrCriteria);			//ID_HSE_AwrnsPlnInq				
	STDMETHOD(ShowAwrnsPlnCrsAtnds)(BSTR bstrCriteria);		//ID_HSE_AWRNSPLN_CRS_Atnds		
	//STDMETHOD(ShowCMSRsns)(BSTR bstrCriteria);
	STDMETHOD(ShowCMSActnRsn)(BSTR bstrCriteria);
	STDMETHOD(ShowRscuEntts)(BSTR bstrCriteria);			//ID_HSE_RscuEntts	
	STDMETHOD(ShowCnfndSpcTyps)(BSTR bstrCriteria);			//ID_HSE_CnfndSpcTyps
	STDMETHOD(ShowRscuHzrds)(BSTR bstrCriteria);			//ID_HSE_RscuHzrds
	STDMETHOD(ShowRscuTls)(BSTR bstrCriteria);				//ID_HSE_RscuTls
	STDMETHOD(ShowRscuCmmunctnMthds)(BSTR bstrCriteria);	//ID_HSE_RscuCmmunctnMthds	
	STDMETHOD(ShowRscuPlnEntry)(BSTR bstrCriteria);		//ID_HSE_RscuPlnEntry
	STDMETHOD(ShowRscuPlnInqury)(BSTR bstrCriteria);	//ID_HSE_RscuPlnInqury
	STDMETHOD(ShowHzrdPrcs)(BSTR bstrCriteria);			//ID_Hzrd_Procs	
	STDMETHOD(ShowAreaHzrds)(BSTR bstrCriteria);		//ID_Area_Hzrds
	STDMETHOD(ShowAHzrdsPtntlHzrdCtrlMsr)(BSTR bstrCriteria);		//ID_HSE_AHzrdsPtntlHzrdCtrlMsr
	STDMETHOD(ShowAreaHzrdsPtntlHzrdEffct)(BSTR bstrCriteria);		//ID_HSE_AreaHzrdsPtntlHzrdEffct
	
	//HSE BGC-EZZ Requirements_2892020	
	STDMETHOD(ShowRskTyp)(BSTR bstrCriteria);		//ID_HSE_RskTyp
	STDMETHOD(ShowMdclAuthrty)(BSTR bstrCriteria);	//ID_HSE_MdclAuthrty
	STDMETHOD(ShowMdclDcsn)(BSTR bstrCriteria);		//ID_HSE_MdclDcsn
	STDMETHOD(ShowAudtrs)(BSTR bstrCriteria);		//ID_HSE_Audtrs
	STDMETHOD(ShowEnvrnmntlMsrs)(BSTR bstrCriteria);//ID_HSE_EnvrnmntlMsrs	
	STDMETHOD(ShowIncdntMdclRprtFlwUp)(BSTR bstrCriteria);//ID_HSE_IncdntMdclRprtFlwUp	
	STDMETHOD(ShowDrillInquiry)(BSTR bstrCriteria);	//ID_Drill_Inquiry
	STDMETHOD(ShowCrctveAccRcvd)(BSTR bstrCriteria);		//ID_HSE_CrctveAccRcvd
	STDMETHOD(ShowCrctveAccUndrExctn)(BSTR bstrCriteria);	//ID_HSE_CrctveAccUndrExctn
	STDMETHOD(ShowCrctveAccJobVrfctn)(BSTR bstrCriteria);	//ID_HSE_CrctveAccJobVrfctn
	
	STDMETHOD(genMonthlyReports)();
	
	STDMETHOD(ShowDrlFndngsUnPlndScnros)(BSTR bstrCriteria);	//ID_HSE_DrlFndngsUnPlndScnros
	
	STDMETHOD(ShowSitSrvyTyps)(BSTR bstrCriteria);				//ID_HSE_Srvy_Typs

	///////////////////////////////////////////////////////////////////
	STDMETHOD(ShowScreen)(int nScreenId,BSTR bstrCriteria,BSTR strTableName);

	
public:
	BOOL	tryit();
	void	CreateRegHandle();
	BOOL	PrepareBeforeOpenAttachmet(CString strScreenTag,CString strCriteria,CString strSubject);
	bool	IsMenuEnabled(long lMenuID);
	BOOL	GetRecodingStatus(){return m_bRecodingEnabled;}
	int		GetDisplayStatus(){	return m_nStatus;}
	int		ShowLoginScreen();
	BOOL	IsAdminUser(int nUserID);
	void	FireCloseAll();
	void	Remove(CCategory * pCategory);
	BOOL 	IsArabic();
	BOOL 	IsHijri(){return m_bHijri;};
	CString GetFastSearchCriteria(){return m_strFastSearchCriteria;};
	int		DoFunction(int nCurrentCard,CString strTableName,int nStatus=NORMAL_MODE,BOOL bModal=FALSE,CString strCriteria = "",CString strComesFrom="",CString strScreenDefauls="",CString strReturnBrowseValue="",int nScreenId=-1,void* msg=NULL,CCommonCategoryWrapper **pCommonCategoryWrapper=NULL);
	UINT	DefineSetupScreen();
	UINT	DefineUserScreen();
	UINT	DefineMenuScreen(long lMenuHandle);
	UINT	DefineUsersList();
	BOOL	DefineUserID();
	void	DefineFieldsPriv();
	BOOL	DefineUsersVerfy(int nMenuItem,CString &strCaption);
	BOOL	IsModalPolyDb();
	CString GetScreenDefauls();
	//a.ali 02/05/2021
	void SetScreenDefauls(CString strScreenDefaults);
	bool	ValidateDongle();
	static long DBInit(_DB	*pHSEMSPolyDb,CToolsListener* pToolsListener,BOOL bExtended=FALSE);
	long	GetParent(){return m_lpParent;};
	void	FillTags();
	_bstr_t ShowNotepad (_bstr_t bstrValue,_bstr_t bstrEnCaption,enum TextStyle nStyle,int nType,BOOL bLocked,COLORREF bkColor);
	void	SetModalPollyDB(BOOL bStatus){if(!m_bLockModalFlag)m_bModalPolyDb = bStatus;};
	CString GetDataPath(){return m_strDataPath;};
	CString GetSystemPath()	{return m_strSystemPath;};
	int GetSystemDBType(){return m_nSystemDBType;};
	void	CheckForUser(CString strUserFullName);
	CString GetWorkingFolder();
	static	CString GetCurrentProfile(){	return m_strCurrentProfile;};
	void	InitializeTables();

	//a.ali
	CString getstrKeyValForRjctScr();
	CString getstrKeyValForAccptScr();
	CString getstrKeyValForApprvlScr();
	void setstrKeyValForRjctScr(CString strKeyVal);
	void setstrKeyValForAccptScr(CString strKeyVal);
	void setstrKeyValForApprvlScr(CString strKeyVal);
	void setstrKeyValForPopUpScr(CString strScreen,CString strKeyVal);
	CString getstrSrcForRjctRsn();
	CString getstrSrcForAccptRsn();
	CString getstrSrcForApprvlRsn();
	CString getstrKeyValForPopUpScr(CString strScreen);
	void setstrSrcForRjctRsn(CString strSrcScr);
	void setstrSrcForAccptRsn(CString strSrcScr);
	void setstrSrcForApprvlRsn(CString strSrcScr);
	void createTagHashtable();
	void setbNewMode(CString strScrTag,bool bNewMode);
	bool getbNewMode(CString strScrTag);
	void setnAuditsKeyFld(int nAdtKeyFld);
	int getnAuditsKeyFld();
	void setstrDep(CString strDep);
	CString getstrDep();
	CString sendMail(CString strSenderName,CString strSubject,CString strRecipientMailAdd,CString strMail,CString strAttachFilePath);
	bool getbLogMultiRec();
	void setbLogMultiRec(bool bLogMultiRec);
	//CString getDocumentationFolderPath();
	void InitEGPCObj();
	void setDisableSitSrvyRqrdActnBtn(BOOL bDisable);
	BOOL getDisableSitSrvyRqrdActnBtn();

	HRESULT	ShowScreenIn(int nScreenId,BSTR bstrCriteria,BSTR strTableName,void* msg=NULL,CCommonCategoryWrapper **pCommonCategoryWrapper=NULL);

protected:
	CCategory *MakeCategory();
	void CreateConnectionString();

protected:
	CRect				m_rcFormRect;
	CString				m_strFastSearchCriteria;
	int					m_nSystemDBType;
	CString				m_strDataPath,
						m_strSystemPath,
						m_strPaneText;
	CWinApp				*m_pHSEMSApp;
	CWnd				*m_pParentWnd;
	CString				m_strScreenDefauls;
	BOOL				m_bModalPolyDb,
						m_bLockModalFlag,
						m_bArabic,
						m_bModalEnv;
	BOOL				m_bRecodingEnabled,
						m_bWaitingForExportedMenuID;
	CConnection			m_Connection;
	int					m_nCodeID,
						m_nStatus;
	long				m_lpParent;
	_DB					*m_pHSEMSSYSPolyDb;
	CObList				*m_pCategoryList;
	static				CString m_strStaticDataPath;
	static				CString m_strStaticSystemPath;
	_FNotepadObjPtr		m_pNotepadeObj;

	

public:
	IRegHandlePtr		m_pRegHandle;
	//a.ali
	//ISendMailObjPtr	m_SendMailObj;
	_EmailMngrPtr		m_SendMailObj;
	_EGPCPtr			m_EGPCObj;

	_MonthlyReportClsPtr pMonthlyReportObj;


	static int			m_nCurrentDBEngine;
	static BOOL			m_bHijri,
						m_bRejectQtyInMRS,
						m_bUseManulSerialInOrders,
						m_bGeneratePartNumFromQut;
	static CString		m_strConnection;
	static CString		m_strUserFullName;
	static CString		m_strLoginUserName;
	static CString		m_lEmployeeDept;
	static CString		m_strHelpFileName;
	static CString		m_strCurrentProfile;
	static long			//m_lEmployeeBudget, //RQ-9-2023.5 m_lEmployeeBudget is not used
						m_lEmpSetting;
	static int			m_nSegLength;
	static short		m_User;
	CString				m_strDB;
	CMapStringToString	m_MapMenuIDTOTags;
	CMapStringToString	m_MapScreenTOKey;

	//a.ali
	CString m_strKeyValForRjctScr;
	CString m_strKeyValForAccptScr;
	CString m_strKeyValForApprvlScr;
	CString m_strSrcForRjctRsn;
	CString m_strSrcForAccptRsn;
	CString m_strSrcForApprvlRsn;
	CString gstrScreen_tag;
	CString gstrSrcKyFldNm;
	bool gbUpdateRejectedRecord;
	bool gbUpdateAcceptedRecord;
	bool gbUpdateApprovedRecord;
	
	bool gbOpenRejectScreenLocked;
	bool gbOpenAcceptScreenLocked;

	CMapStringToString m_ScreenTagForTracing;
	bool m_bNewMode;
	int m_nAuditsKeyFld;
	CString m_strDep;
	CMapStringToString m_ScreensInNewMode;
	bool m_bLogMultiRec;
	BOOL m_bDisableSitSrvyRqrdActnBtn;	

	//This member will be used to define app which open screen
	int m_callerAppCode;

private:
	void CreateGlabalTempTable();
};

#endif
