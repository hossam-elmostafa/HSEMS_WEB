#include "stdafx.h"
#include "HSEMS.h"
#include "AuditModuleCategory.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CAuditModuleCategory *pgScreenNeedReresh=NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAuditModuleCategory::CAuditModuleCategory(CCategory *pCategory,CMapStringToOb* pmpListOfParameters):CHSEMSCommonCategory(pCategory)
{
	//HSE Audit Module 25/11/2021
	
	m_pmpDetailsSerials=NULL;
	m_mpDetailsKeyFields=NULL;
	m_mpDetailsTableName=NULL;
	m_pMsg=NULL;
	m_pCategory = pCategory;
	m_strMasterKeyField="";
	m_strMainSerialDependentKey="";
	ASSERT(pmpListOfParameters);
	if(pmpListOfParameters)
	{
		CObject* pObj;
		if(pmpListOfParameters->Lookup("FSATableName",pObj))
			m_FSATableName=(char*)pObj;
		
		if(pmpListOfParameters->Lookup("FormTable",pObj))
			m_strFormTable=(char*)pObj;

		if(pmpListOfParameters->Lookup("SerialFieldName",pObj))
			m_strSerialFieldName=(char*)pObj;
		
		if(pmpListOfParameters->Lookup("MasterKeyField",pObj))
			m_strMasterKeyField=(char*)pObj;	

		if(pmpListOfParameters->Lookup("MainSerialDependentKey",pObj))
			m_strMainSerialDependentKey=(char*)pObj;	

		

		if(pmpListOfParameters->Lookup("DetailsSerials",pObj))
			m_pmpDetailsSerials=(CMapStringToOb*)(CObject*)pObj;

		if(pmpListOfParameters->Lookup("DetailsKeys",pObj))
			m_mpDetailsKeyFields=(CMapStringToOb*)(CObject*)pObj;

		if(pmpListOfParameters->Lookup("DetailsTableName",pObj))
			m_mpDetailsTableName=(CMapStringToOb*)(CObject*)pObj;

		if(pmpListOfParameters->Lookup("Msg",pObj))
			m_pMsg=pObj;
		delete pmpListOfParameters;
		pmpListOfParameters=NULL;
	}
	//////////////////////////////////////////////////////////////
	m_bFirstTime=true;		

}

CAuditModuleCategory::~CAuditModuleCategory()
{
	if(m_pmpDetailsSerials)
		delete m_pmpDetailsSerials;
	if(m_mpDetailsKeyFields)
		delete m_mpDetailsKeyFields;
}

HRESULT CAuditModuleCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strFormTag(pScreenReady->Form_Tag);
	if(strFormTag=="HSE_AUDTACTNSENTRY")
	{
		/////////////////////////////////////////////////////////////////////////////////////
		//RQ-4-2022.43
		CString strEmpDep = getEmpDep(m_pCategory->GetUserLogin(),true);//Bugs-4-2022.23
		CString strOwnerDept = getOwnerDept("");
		if(strOwnerDept!=strEmpDep) //RQ-4-2022.44
		{
			CString strQuery;
			strQuery.Format("where (AUDTPLNENTRY_AUDTS_AUDTSTTUS='9' AND isnull(AUDTPLNENTRY_AUDTS_CRSENTCMP,'N')!='Y' and AudtPlnEntry_Audts_AudtUnt in(select PrmKy from HSE_Audt_AudtUnt where Audt_AudtUnt_Dprtmnt='%s'))",strEmpDep);
			m_pHSEMSPolyDb->LinkSetAttribSingle("HSE_AudtActnsEntry_Audts", "Audits", "MasterKey", "HSE_AudtPlnEntry_Audts", "MainLink", strQuery, 0, "AudtPlnEntry_Audts_AudtN|MainLink", 0, 	0, "");
		}
		/////////////////////////////////////////////////////////////////////////////////////
	}

	if(strFormTag=="HSE_CLUS_LVLRSULTPPUP")
	{
		/////////////////////////////////////////////////////
		//RQ_HM_17_8_23.01.17.05
		//If (HSE Policy > Audit > Audit Result on Clause Level) = Yes, then
		//•  Hide the (ISO Standard) field
		CString strPolicyAdt_AdtRsltOnClsLvl= GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_ENTADTRSLONCLSLVL");
		if(strPolicyAdt_AdtRsltOnClsLvl=='Y')
			FieldSetAttribSingle("CLUS_LVLRSULTPPUP_ISOSTNDRD","",-1,"","HIDDEN","","A",-1,"","","","","[For Action|A][For Improvements|M]",0,0,"");

		/////////////////////////////////////////////////////


	}
	
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

void CAuditModuleCategory::CalculateNewSerial(CString strFormTag,CString strSubFormTag)
{
	if (strSubFormTag=="")//The main screen
	{
		if(m_strSerialFieldName!="")
		{
			//HSE Audit Module 25/11/2021
			CString strNewSrl ;
			if(m_strMainSerialDependentKey=="")
				strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,"","");
			else
			{
				CString strKeyFieldValue=FormGetField(strFormTag,m_strMainSerialDependentKey); 
				strNewSrl = getNxtSrl(m_strSerialFieldName,m_strFormTable,m_strMainSerialDependentKey,strKeyFieldValue);
			}
			FormSetField(strFormTag,m_strSerialFieldName,strNewSrl);
			/////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		if(m_pmpDetailsSerials && m_mpDetailsKeyFields)
		{
			//HSE Audit Module 25/11/2021
			CObject* pObj;
			CString strSerialFldNam="";
			CString strDetailKeyField="";
										
			if(m_pmpDetailsSerials->Lookup(strSubFormTag,pObj))
			{
				strSerialFldNam=(char*)pObj;
			}
							
			if(m_mpDetailsKeyFields->Lookup(strSubFormTag,pObj))
			{
				strDetailKeyField=(char*)pObj;
			}

			CString strDetailTableName=strSubFormTag;
			strDetailTableName.Replace("TG","");
			if(m_mpDetailsTableName!=NULL)
			{
				if(m_mpDetailsTableName->Lookup(strSubFormTag,pObj))
				{
					strDetailTableName=(char*)pObj;
				}
			}
			
			if(strSerialFldNam!="" && strDetailKeyField!="" && m_strMasterKeyField!="")
			{
				CString strMasterKeyFieldValue=FormGetField(strFormTag,m_strMasterKeyField); 
				CString strNewSrl = getNxtSrl(strSerialFldNam,strDetailTableName,strDetailKeyField,strMasterKeyFieldValue);
				FormSetField(strSubFormTag,strSerialFldNam,strNewSrl);
			}
			//////////////////////////////////////////////////////
		}
	}
}

HRESULT CAuditModuleCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	CString strFormTag(pToolBarButtonClicked->Form_Tag);
	CString strFull_Record= pToolBarButtonClicked->Full_Record;

	if(strBtnName=="NEW")
	{
		if(pToolBarButtonClicked->iComplete==1)
		{
			///////////////////////////////////////////
			//RQ_HM_26_7_23.02
			if (strFormTag=="HSE_AUDTPLNENTRY")
			{
				CString ServerYear=GetCurrentYear();
				FormSetField(strFormTag,"AUDTPLNENTRY_AUDTYR",ServerYear);
			}
			//////////////////////////////////////////
			CalculateNewSerial(strFormTag,strSubFormTag);
			if(strSubFormTag=="HSE_AUDTPLNENTRY_AUDTS")
			{
				//HSE Audit Module 25/11/2021
				CString strMainLink=FormGetField(strSubFormTag,"MainLink");
				if(strMainLink!="")
				{
					CString strSQL;
					//Removed due to RQ-1-2022.51
					/*strSQL.Format("EXECUTE Update_Audit_Plan_Entry_Audits_Expected_man_days %s",strMainLink);
					CString strExpectedManDays= GetRecordSetData(strSQL).GetBuffer(0);	
					FormSetField(strSubFormTag,"AUDTPLNENTRY_AUDTS_EXPCTDMNDYS",strExpectedManDays);*/
				}
				///////////////////////////////////////////////////////
			}
			else if(strSubFormTag=="HSE_CLUS_LVLRSULTPPUP_FNDNGS")
			{
				///////////////////////////////////////////////////////
				//RQ-1-2022.44
				CString strMainLink=FormGetField(strFormTag,"MainLink");
				if(strMainLink!="")
				{
					CString strSQL;
					strSQL.Format("EXECUTE GetDepartmentCodeForFinding %s",strMainLink);
					CString strDepartmentCode= GetRecordSetData(strSQL).GetBuffer(0);	
					FormSetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_ACTDPR",strDepartmentCode);
					//FormSetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_RSLSTT","O");
					LinkFieldsAttribSingle("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","",-1,"","","","I",-1,"","","","CODE","[Minor|I][Major|J]",0,0,"");
					FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","I");

					///////////////////////////////////////////////////
					//RQ-2-2022.2
					strSQL.Format("exec GetPolicyValue 'HSE_HSEPLC_ADT','HSEPLC_ADT_INSRF_N_'");
					CString strInsRefNum= GetRecordSetData(strSQL).GetBuffer(0);
					strSQL.Format("exec GetPolicyValue 'HSE_HSEPLC_ADT','HSEPLC_ADT_IMSMNG'");
					CString strIMSMgr= GetRecordSetData(strSQL).GetBuffer(0);
					FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_INSRF_N_",strInsRefNum);
					FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_IMSMNG",strIMSMgr);
				}
				///////////////////////////////////////////////////////
			}
		}
	}
	
	if(strBtnName=="SAVE" || strBtnName=="EXIT" )
	{
		if(pToolBarButtonClicked->iComplete==0)
		{
			if(strFormTag=="HSE_IDNTFYAUDTRSTMPPUP" || strFormTag=="HSE_IDNTFYAUDTRSTMPPUP_NTFC" || strFormTag=="HSE_IDNTFYAUDTRSTMPPUP_AER")
			{
				////////////////////////////////////////////////////////////////////////////////////////////
				//Bugs-12-2021.25
				//RQ-12-2021.25
				//RQ-12-2021.82
				CString strAuditLeader=FormGetField(strFormTag,"IDNTFYAUDTRSTMPPUP_AUDTLDR"); 
				if(strAuditLeader=="Y")
				{
					//Check if there is another Audit Leader
					CString strMainLink=FormGetField(strFormTag,"MainLink");
					CString strPrimKey=FormGetField(strFormTag,"PrmKy");
					if(strPrimKey=="")
						strPrimKey="-1";
					CString strSQL;
					////////////////////////////////////////////////////////////////////////
					//Bugs-1-2022.2
					strSQL.Format("EXECUTE Check_PreviousAudit_Leaders %s,%s",strMainLink,strPrimKey);
					////////////////////////////////////////////////////////////////////////
					CString strCountPreviousLeader= GetRecordSetData(strSQL).GetBuffer(0);	
					int nCountPreviousLeader=atoi(strCountPreviousLeader.GetBuffer(0));
					if(nCountPreviousLeader>0)
					{
 						AfxMessageBox("Only one audit leader is allowed");
						FormSetField(strFormTag,"IDNTFYAUDTRSTMPPUP_AUDTLDR","N");
					}
				}
				////////////////////////////////////////////////////////////////////////////////////////////
			}
			if (strFormTag=="HSE_DFULTDSTRBUTNLST")
			{
				//HSE Audit Module 25/11/2021
				CheckEmail(strFormTag,"DFULTDSTRBUTNLST_E_ML",pToolBarButtonClicked->iCancle);
			}
			else if (strFormTag=="HSE_MRPRSNSTNTFYPPUP")
			{
				//HSE Audit Module 25/11/2021
				CheckEmail(strFormTag,"MRPRSNSTNTFYPPUP_E_ML",pToolBarButtonClicked->iCancle);
				////////////////////////////////////////////////////////////
			}
			else if(strFormTag=="HSE_CRTFDAUDTRS")
			{
				//Bugs-12-2021.9
				CheckEmail(strFormTag,"CRTFDAUDTRS_E_ML",pToolBarButtonClicked->iCancle);
				////////////////////////////////////////////////////////////
			}
			else if(strFormTag=="HSE_IDNTFYAUDTRSTMPPUP_NTFC" )
			{
				/////////////////////////////////////////////////////////////////////
				//RQ-1-2022.1
				if ((*pToolBarButtonClicked).Button_Name!=CString("EXIT"))// Hossam: When trying to exit then it is not needed to check for any field since it must be empty already
					CheckEmail(strFormTag,"IDNTFYAUDTRSTMPPUP_E_ML",pToolBarButtonClicked->iCancle);
				/////////////////////////////////////////////////////////////////////
			}
			else if(strFormTag=="HSE_IDNTFYAUDTRSTMPPUP_AER" )
			{
				/////////////////////////////////////////////////////////////////////
				//RQ-1-2022.4
				if ((*pToolBarButtonClicked).Button_Name!=CString("EXIT"))// Hossam: When trying to exit then it is not needed to check for any field since it must be empty already
					CheckEmail(strFormTag,"IDNTFYAUDTRSTMPPUP_E_ML",pToolBarButtonClicked->iCancle);
				/*CString strEmail=FormGetField(strFormTag,"IDNTFYAUDTRSTMPPUP_E_ML"); 
				if(strEmail=="")
				{
					AfxMessageBox("Email is empty, please check"); 
				}*/
				/////////////////////////////////////////////////////////////////////
			}
			else if(strFormTag=="HSE_VW_CRTFDAUDTRS" )
			{
				/////////////////////////////////////////////////////////////////////
				//RQ-4-2022.18
				CheckEmail(strFormTag,"VEMPLOYEE_E_ML",pToolBarButtonClicked->iCancle,true);
				/////////////////////////////////////////////////////////////////////

			}

			else if(strFormTag=="HSE_AUDTPLNENTRY" & strBtnName=="SAVE")
			{
				////////////////////////////////////////////////////////////////////////////
				//RQ_HM_17_8_23.01.14.02
				//The fields (Last Change By) & (Last Change DT) should be updated each time the user click SAVE 
				//in the main screen
				//AUDTPLNENTRY_LSTCHNBY
				//AUDTPLNENTRY_LSTCHNDT
				CString strUserName = GetUserLogin();
				//FormSetField(strSubFormTag,"ACTNSENTRY_CRR_ACTFLLDT",m_pHSEMSPolyDb->GetServerDate()); 
				CString strDate=m_pHSEMSPolyDb->GetServerDate();
				FormSetField(strFormTag,"AUDTPLNENTRY_LSTCHNBY",strUserName); 
				//FormSetField(strFormTag,"AUDTPLNENTRY_LSTCHNDT",strDate+CString(" 00:00")); 
				////////////////////////////////////////////////////////////////////////////
			}

		}

		if(pToolBarButtonClicked->iComplete==1)
		{
			if(strFormTag=="HSE_IDNTFYAUDTCLUSSPPUP")
			{
				if(strSubFormTag=="")
				{
					//HSE Audit Module 25/11/2021
					CString strMainLink=FormGetField(strFormTag,"MainLink");
					CString strSQL;	
					strSQL.Format("EXECUTE Update_Audit_Plan_Entry_Audits_Expected_man_days %s",strMainLink);
					ExecuteSQL(strSQL);
					/////////////////////////////////////////////////////////////////////////////
				}
			}
			else if(strFormTag=="HSE_IDNTFYAUDTRSTMPPUP_AER")
			{
				////////////////////////////////////////////////////////////////////////////
				//RQ-1-2022.53
				CString strParentMainLink=FormGetField(strFormTag,"MainLink");
				CString strSQL;	
				strSQL.Format("EXECUTE Update_AUDTPLNENTRY_AUDTS_ACTLMNDYS %s",strParentMainLink);
				ExecuteSQL(strSQL);
				if(pgScreenNeedReresh)
					pgScreenNeedReresh->RefreshScreen("",REFRESH_SELECTED);	
				////////////////////////////////////////////////////////////////////////////

			}
		}
	}

	else if(strBtnName=="SAVETAB")
	{
		///////////////////////////////////////////////////////////////////////////////////////////
		
		if(strFormTag=="HSE_AUDTPLNENTRY")
		{
			if(strSubFormTag=="HSE_AUDTPLNENTRY_AUDTS")
			{
				if(pToolBarButtonClicked->iComplete==1)
				{
					///////////////////////////////////////////////////////////////////////////////////////////
					//Bugs-12-2021.28
					//Due to an unknown problem in the PolyDB the below requirement was transfered to DB trigger
					/* 
					///////////////////////////////////////////////////////////////////////////////////////////
					//RQ-12-2021.10
					CString strAuditeeUnit=FormGetField(strSubFormTag,"AudtPlnEntry_Audts_AudtUnt");
					CString strPrmKey=FormGetField(strSubFormTag,"PrmKy");
					if(strAuditeeUnit!="" && strPrmKey!="")
					{
						//BUG-12-2021.31
						CString strSQL;
						strSQL.Format("EXECUTE Identify_Audit_Clauses %s,%s,0",strAuditeeUnit,strPrmKey);
						ExecuteSQL(strSQL);			
						strSQL.Format("EXECUTE Identify_Audit_Locations %s,%s,0",strAuditeeUnit,strPrmKey);
						ExecuteSQL(strSQL);			
					}
					*/
					///////////////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
		if(strFormTag=="HSE_AUDTEXCUTNRSULT")
		{
			if(strSubFormTag=="HSE_AUDTEXCUTNRSULT_AUDTS")
			{
				if(pToolBarButtonClicked->iComplete==1)
				{
					////////////////////////////////////////////////////////////////////////////
					//RQ-1-2022.53
					CString strParentMainLink=FormGetField(strSubFormTag,"PrmKy");
					CString strSQL;	
					strSQL.Format("EXECUTE Update_AUDTPLNENTRY_AUDTS_ACTLMNDYS %s",strParentMainLink);
					ExecuteSQL(strSQL);
					////////////////////////////////////////////////////////////////////////////
				}
			}
		}
		if(strFormTag=="HSE_CLUS_LVLRSULTPPUP")
		{
			////////////////////////////////////////////////////////////////////////////
			//RQ-3-2022.5
			CString strCurrentNCRNo=FormGetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_NCRN");
			if(strCurrentNCRNo=="")
			{
				CString strParentMainLink=FormGetField(strFormTag,"PrmKy");
				CString strSerial=FormGetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_SRL");
				CString strSQL;	
				strSQL.Format("EXECUTE Update_NCRNo %s,%s",strParentMainLink,strSerial);//You must add "SET NOCOUNT ON;" at the start of the procedure
				CString NewSerial= GetRecordSetData(strSQL).GetBuffer(0);	
				//FormSetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_NCRN","TEST"); It doesn't work!!!
				RefreshScreen("",REFRESH_SELECTED);	
			}
			////////////////////////////////////////////////////////////////////////////			
		} 
	}

	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}
CAuditModuleCategory* gpAuditModuleCurrentParent=NULL;
int gnMainLink=0;
HRESULT CAuditModuleCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strFormTag(pCustomButtonClicked->Form_Tag);
	//HSE Audit Module 25/11/2021
 	if(strFormTag=="HSE_AUDTPLNENTRY")
	{
		//Master Table
		if(strSubFormTag=="")
		{
			if(strButtonName=="PLAN_ENTRY_COMPLETED")
			{
				CString strMainLink=FormGetField(strFormTag,"MasterKey");
				CString strSQL;
				strSQL.Format("UPDATE HSE_AUDTPLNENTRY set AudtPlnEntry_AudtYr=AudtPlnEntry_AudtYr where MasterKey=%s and MasterKey not in (select MainLink from HSE_CVRDSTNDRDS )",strMainLink);
				long lAffected=ExecuteSQL(strSQL);
				if(lAffected>0)
				{
					//RQ-10-2023.13
					CString strResult;
					strResult.Format("There are no linked 'Covered Standards'. Can not procede",lAffected);
					AfxMessageBox(strResult);
				}
				else
				{
					strSQL.Format("EXECUTE Check_Plan %s",strMainLink);
					CString strResult= GetRecordSetData(strSQL).GetBuffer(0);	
					if (strResult=="OK")
					{
						FormSetField(strFormTag,"AUDTPLNENTRY_PLNSTTUS","C");
						DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
						RefreshScreen("",REFRESH_SELECTED);	
					}
					else 
						AfxMessageBox(strResult);
				}
			}
			else if(strButtonName=="COVERED_STANDARDS")
			{
				//RQ-10-2023.13
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_CVRDSTNDRDS","HSE_TgCvrdStndrdsPopUP","Covered Standards","","MAINLINK",-1);
			}
		}
		else //Details
		{
			if(strSubFormTag=="HSE_AUDTPLNENTRY_AUDTS")
			{
				if(strButtonName=="IDENTIFY_AUDIT_CLAUSES")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTCLUSSPPUP","HSE_IDNTFYAUDTCLUSSPPUP","Identify Audit Clauses","","MAINLINK",-1);
				}
				else if(strButtonName=="IDENTIFY_AUDITEE_LOCATIONS")
				{
					gpAuditModuleCurrentParent=this;
					//////////////////////////////////////////////////////////
					//Bugs-10-2023.1
					CString strMainLink=FormGetField(strSubFormTag,"PrmKy");
					gnMainLink=atoi(strMainLink);
					///////////////////////////////////////////////////////////
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTLCTNSPPUP","HSE_IDNTFYAUDTLCTNSPPUP","Identify Auditee Locations","","MAINLINK",-1);
				}
				else if(strButtonName=="IDENTIFY_NEEDED_TOOLS")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYNDDTLSPPUP","HSE_IDNTFYNDDTLSPPUP","Identify Needed Tools","","MAINLINK",ID_POPUPS_IDENTIFYNEEDEDTOOLSPOPUP);
				}
				else if(strButtonName=="IDENTIFY_AUDITORS_TEAM")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP","HSE_IDNTFYAUDTRSTMPPUP","Identify Auditors Team","","MAINLINK",ID_POPUPS_IDENTIFYAUDITORSTEAMPOPUP);
				}
			}

		}
	}
	else if(strFormTag=="HSE_AUDTPLNAPPRVL")
	{
		//Master Table
		if(strSubFormTag=="")
		{
			if(strButtonName=="APPROVE")
			{
				CString strMainLink=FormGetField(strFormTag,"MasterKey");
				FormSetField(strFormTag,"AUDTPLNENTRY_PLNSTTUS","F");
				FormSetField(strFormTag,"AUDTPLNENTRY_APPRVDBY","Login");
				FormSetField(strFormTag,"AUDTPLNENTRY_APPRVDDT","TODAY");			
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
				RefreshScreen("",REFRESH_SELECTED);	
				CString strSQL;
				strSQL.Format("EXECUTE Update_Needed_While_Moving_Audit_Plan %s",strMainLink);
				ExecuteSQL(strSQL);			
			}
			else if(strButtonName=="REJECT")
			{
				FormSetField(strFormTag,"AUDTPLNENTRY_PLNSTTUS","O");
				DoToolBarAction(TOOLBAR_SAVE,strFormTag,"");
				RefreshScreen("",REFRESH_SELECTED);	
			}
			else if(strButtonName=="COVERED_STANDARDS")
			{
				//RQ-10-2023.13
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_CVRDSTNDRDS","HSE_TgCvrdStndrdsPopUPLocked","Covered Standards","","MAINLINK",-1);
			}
		}
		else	//Details
		{
			if(strSubFormTag=="HSE_AUDTPLNAPPRVL_AUDTS")
			{
				if(strButtonName=="IDENTIFY_AUDIT_CLAUSES")
				{								                                                   
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTCLUSSPPUP","HSE_IDNTFYAUDTCLUSSPPUP_LNK","View Audit Clauses","","MAINLINK",ID_POPUPS_AUDITCLAUSESPOPUP_LNK);
				}
				else if(strButtonName=="IDENTIFY_AUDITEE_LOCATIONS")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTLCTNSPPUP_LNK","HSE_IDNTFYAUDTLCTNSPPUP_LNK","View Auditee Locations","","MAINLINK",ID_POPUPS_IDNTFYAUDTLCTNSPPUP_LNK);//#3
				}
				else if(strButtonName=="IDENTIFY_NEEDED_TOOLS")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYNDDTLSPPUP_LNK","HSE_IDNTFYNDDTLSPPUP_LNK","View Needed Tools","","MAINLINK",ID_POPUPS_IDNTFYNDDTLSPPUP_LNK);//#4
				}
				else if(strButtonName=="IDENTIFY_AUDITORS_TEAM")
				{
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP_LNK","HSE_IDNTFYAUDTRSTMPPUP_LNK","View Auditors Team","","MAINLINK",ID_POPUPS_IDNTFYAUDTRSTMPPUP_LNK);//#5
				}
			}
		}
	}
	else if(strFormTag=="HSE_LNKDAUDTCLUSSPPUP")
	{
		///////////////////////////////////////////////////////////////////////////
		//RQ-12-2021.8
		//CCommonCategoryWrapper *pCommonCategoryWrapper=NULL;
		//#POPUPSCREEN
		if(strButtonName=="VIEW_AUDIT_CHECKLIST")
		{
			OpenPopUpScreen(strFormTag,strFormTag,"LnkdAudtClussPpUp_ISOStndrd","HSE_AUDTCHCKLST_LNK","HSE_AUDTCHCKLST_LNK","Audit Check List","","MAINLINK",ID_POPUPS_AUDITCHECKLISTPOPUP_LNK);//,&pCommonCategoryWrapper);
		}
		//pCommonCategoryWrapper->UpdateEnableMainScreen(false);
		///////////////////////////////////////////////////////////////////////////
	}
	else if(strFormTag=="HSE_AUDTNTFCTN")
	{
		if(strSubFormTag=="")
		{
			if(strButtonName=="COVERED_STANDARDS")//Bugs-12-2023.7
			{	//RQ-10-2023.13
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_CVRDSTNDRDS","HSE_TgCvrdStndrdsPopUPLocked","Covered Standards","","MAINLINK",-1);
				///////////////////////////////////
			}
		}
		else if(strSubFormTag=="HSE_AUDTNTFCTN_AUDTS")
		{
			if(strButtonName=="EDIT_AUDITORS_TEAM")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP_NTFC","HSE_IDNTFYAUDTRSTMPPUP_NTFC","Identify Auditors Team","","MAINLINK",-1);
			}
			else if(strButtonName=="EDIT_AUDITORS_TEAM")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP","HSE_IDNTFYAUDTRSTMPPUP","Identify Auditors Team","","MAINLINK",ID_POPUPS_IDENTIFYAUDITORSTEAMPOPUP);
			}
			else if(strButtonName=="EDIT_NEEDED_TOOLS")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYNDDTLSPPUP","HSE_IDNTFYNDDTLSPPUP","Identify Needed Tools","","MAINLINK",ID_POPUPS_IDENTIFYNEEDEDTOOLSPOPUP);
			}
			else if(strButtonName=="EDIT_SHIFT_GROUPS")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYSHFTGRUPSPPUP","HSE_IDNTFYSHFTGRUPSPPUP","Edit Shift Groups","","MAINLINK",ID_POPUPS_IDENTIFYSHIFTGROUPSPOPUP);
			}
			else if(strButtonName=="EDIT_AUDIT_CLAUSES")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTCLUSSPPUP_EDT","HSE_IDNTFYAUDTCLUSSPPUP_EDT","Edit Audit Clauses","","MAINLINK",-1);
			}
			else if(strButtonName=="EDIT_AUDITEE_LOCATIONS")
			{
				//RQ_HM_17_8_23.01.16.02
				//RQ_HM_17_8_23.01.16.04
				//RQ-12-2023.3
				gpAuditModuleCurrentParent=this;
				CString strMainLink=FormGetField(strSubFormTag,"PrmKy");
				gnMainLink=atoi(strMainLink);
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTLCTNSPPUP","HSE_IDNTFYAUDTLCTNSPPUP","Edit Auditee Locations","","MAINLINK",-1);
			}
			else if(strButtonName=="MORE_PERSONS_TO_NOTIFY")
			{
				//RQ-12-2023.8
				gpAuditModuleCurrentParent=this;
				CString strMainLink=FormGetField(strSubFormTag,"PrmKy");
				gnMainLink=atoi(strMainLink);
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_MRPRSNSTNTFYPPUP","HSE_MRPRSNSTNTFYPPUP","More Persons to Notify","","MAINLINK",ID_POPUPS_MOREPERSONSTONOTIFYPOPUP);
			}
			else if(strButtonName=="AUDIT_INFO_COMPLETED")
			{
				/*FormSetField(strSubFormTag,"AUDTPLNENTRY_AUDTS_AUDTSTTUS","5");
				RefreshScreen("",REFRESH_SCREEN);	*/
				//////////////////////////////////////////////////////////////////////////
				//RQ-12-2021.35
				CString strRecordKey=FormGetField(strSubFormTag,"PrmKy");
				CString strSQL;
				strSQL.Format("EXECUTE Notify_Audit_Cmpleted %s",strRecordKey);
				ExecuteSQL(strSQL);			
				RefreshScreen("",REFRESH_SCREEN);	
				//////////////////////////////////////////////////////////////////////////
			}
			else if(strButtonName=="CANCEL_AUDIT")
			{
				//////////////////////////////////////////////////////////////////////////
				//RQ-12-2021.48
				CString strCanceled=FormGetField(strSubFormTag,"AUDTPLNENTRY_AUDTS_AUDTSTTUS");
				if(strCanceled!="8")
				{
					int nRet = MessageBox(NULL,"Are you sure to cancel the audit?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
					if(nRet == IDYES) {
						CString strRecordKey=FormGetField(strSubFormTag,"PrmKy");
						CString strSQL;
						strSQL.Format("EXECUTE Cancel_Audit %s",strRecordKey);
						ExecuteSQL(strSQL);			
						RefreshScreen("",REFRESH_SCREEN);	
					}
				}
				else
					AfxMessageBox("Record is already canceled");
				//////////////////////////////////////////////////////////////////////////
			}
			else if (strButtonName=="EXPECTED_AUDITEE_PERSONS")
			{
				//////////////////////////////////////////
				//RQ-1-2022.43
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_ACTULAUDTPRSNSPPUP","HSE_ACTULAUDTPRSNSPPUP_NTFC","Expected Auditee Persons","","MAINLINK",-1);
				//////////////////////////////////////////
			}

		}
	}
	else if(strFormTag=="HSE_IDNTFYAUDTCLUSSPPUP" || strFormTag=="HSE_IDNTFYAUDTCLUSSPPUP_LNK")
	{
		if(strButtonName=="RE-LOAD_LINKED_CLAUSES")
		{
			///////////////////////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.14
			CString strMainLink=FormGetField(strFormTag,"MAINLINK");
			if(strMainLink!="")
			{
				int nRet = MessageBox(NULL,"Are you sure. This will erase all your updates?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
				if(nRet == IDYES) {
					CString strSQL;
					strSQL.Format("EXECUTE Reset_And_Identify_Audit_Clauses %s",strMainLink);
					ExecuteSQL(strSQL);			
					RefreshScreen("",REFRESH_SCREEN);	
				}
			}
			else
				AfxMessageBox("List is empty");
			///////////////////////////////////////////////////////////////////////////////////////////
		}
		else if(strButtonName=="VIEW_AUDIT_CHECKLIST")
		{
			///////////////////////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.13
			OpenPopUpScreen(strFormTag,strFormTag,"IDNTFYAUDTCLUSSPPUP_ISOStndrd","HSE_AUDTCHCKLST_LNK","HSE_AUDTCHCKLST_LNK","Audit Check List","","MAINLINK",ID_POPUPS_AUDITCHECKLISTPOPUP_LNK);//,&pCommonCategoryWrapper);
			///////////////////////////////////////////////////////////////////////////////////////////
		}
	}

	else if(strFormTag=="HSE_AUDTEXCUTNRSULT" || strFormTag=="HSE_ADTRSLTCNFRMTN")//Bugs-10-2023.3
	{
		if(strSubFormTag=="")
		{
			if(strButtonName=="PLAN_CLOSED")
			{
				///////////////////////////////////////////////////////////////////////////////////////////
				//RQ-12-2021.49
				CString strMainLink=FormGetField(strFormTag,"MasterKey");
				CString strSQL;
				strSQL.Format("EXECUTE Close_Plan %s",strMainLink);
				CString strMessage= GetRecordSetData(strSQL).GetBuffer(0);	
				if(strMessage!="OK")
				{
					AfxMessageBox(strMessage);
				}
				else
					RefreshScreen("",REFRESH_SCREEN);	
				///////////////////////////////////////////////////////////////////////////////////////////
			}
			//"In (main) screen, add the following custom-buttons, to enter feedback on (plan) level :
			else if(strButtonName=="AUDITS_STRENGTH_POINTS")
			{
				//////////////////////////////////////////////////////////
				//RQ-1-2022.52
				//• (Audits Strength Points) .. optional, typical popup as done in (audit) TAB     
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_AUDTSTRNGTHPNTSPPUP_MAIN","HSE_AUDTSTRNGTHPNTSPPUP_MAIN","Audits Strength Points","","MAINLINK",-1);
				//////////////////////////////////////////////////////////	
			}
			else if(strButtonName=="AUDITS_WEAK_POINTS")
			{
				//////////////////////////////////////////////////////////
				//RQ-1-2022.52
				//• (Audits Weak Points) .. optional, typical popup as done in (audit) TAB
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_AUDTWKPNTSPPUP_MAIN","HSE_AUDTWKPNTSPPUP_MAIN","Audits Weak Points","","MAINLINK",-1);
				//////////////////////////////////////////////////////////	
			}
			else if(strButtonName=="AUDITS_IMPROVEMENTS_OPPORTUNITY")
			{
				//////////////////////////////////////////////////////////
				//RQ-1-2022.52
				//• (Audits Improvements Opportunity) .. optional, typical popup as done in (audit) TAB
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_IMPRVMNTOPRTUNTYPPUP_MAIN","HSE_IMPRVMNTOPRTUNTYPPUP_MAIN","Audits Improvements Opportunity","","MAINLINK",-1);
				//////////////////////////////////////////////////////////	
			}
			else if(strButtonName=="AUDITS_CONCLUSION")
			{
				//////////////////////////////////////////////////////////
				//RQ-1-2022.52
				//• (Audits Conclusion) .. optional, typical popup as done in (audit) TAB
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_AUDTCNCLUSNPPUP_MAIN","HSE_AUDTCNCLUSNPPUP_MAIN","Audits Conclusion","","MAINLINK",-1);
				//////////////////////////////////////////////////////////	
			}
			else if(strButtonName=="COVERED_STANDARDS")
			{
				//RQ-10-2023.13
				OpenPopUpScreen(strFormTag,strFormTag,"MasterKey","HSE_CVRDSTNDRDS","HSE_TgCvrdStndrdsPopUPLocked","Covered Standards","","MAINLINK",-1);
			}
		}
		else if(strSubFormTag=="HSE_AUDTEXCUTNRSULT_AUDTS" || strSubFormTag=="HSE_ADTRSLTCNFRMTN_AUDTS")
		{
			if(strButtonName=="CLAUSE-LEVEL_RESULT")
			{
				////////////////////////////////////////////////////////////////////////////////
				//RQ-12-2021.32
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_CLUS_LVLRSULTPPUP","HSE_CLUS_LVLRSULTPPUP","Clause Level Results","","MAINLINK",ID_POPUPS_CLAUSELEVELRESULTPOPUP);
				return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);//Bugs-10-2023.10 since it is called agin downwords with no known reason
				//OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTCLUSSPPUP","HSE_IDNTFYAUDTCLUSSPPUP","Identify Audit Clauses","","MAINLINK",ID_POPUPS_AUDITCLAUSESPOPUP);
				////////////////////////////////////////////////////////////////////////////////
			}
			/*if(strButtonName=="AUDIT_CONCLUSION")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_ACTULAUDTPRSNSPPUP","HSE_ACTULAUDTPRSNSPPUP","Actual Auditee Persons","","MAINLINK",ID_POPUPS_ACTUALAUDITEEPERSONSPOPUP);
			}*/
			if(strButtonName=="IMPROVEMENTS_OPPORTUNITY")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IMPRVMNTOPRTUNTYPPUP","HSE_IMPRVMNTOPRTUNTYPPUP","Improvements Opportunity","","MAINLINK",ID_POPUPS_IMPROVEMENTOPORTUNITYPOPUP);
			}
			if(strButtonName=="AUDIT_CONCLUSION")
			{
				/////////////////////////////////////////////////////////////////////////////
				//Bugs-12-2021
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_AUDTCNCLUSNPPUP","HSE_AUDTCNCLUSNPPUP","Audit Conclusion","","MAINLINK",ID_POPUPS_AUDITCONCLUSIONPOPUP);
				/////////////////////////////////////////////////////////////////////////////
			}
			if(strButtonName=="AUDIT_STRENGTH_POINTS")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_AUDTSTRNGTHPNTSPPUP","HSE_AUDTSTRNGTHPNTSPPUP","Audit Strength Points","","MAINLINK",ID_POPUPS_AUDITSTRENGTHPOINTSPOPUP);
			}
			if(strButtonName=="AUDIT_WEAK_POINTS")
			{
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_AUDTWKPNTSPPUP","HSE_AUDTWKPNTSPPUP","Audit Weak Points","","MAINLINK",ID_POPUPS_AUDITWEAKPOINTSPOPUP);
			}
			else if(strButtonName=="UPDATE_AUDITORS_TEAM")
			{
				/////////////////////////////////////////////////////////////////////////
				//RQ-12-2021.40
				//OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP","HSE_IDNTFYAUDTRSTMPPUP","Identify Auditors Team","","MAINLINK",ID_POPUPS_IDENTIFYAUDITORSTEAMPOPUP);
				pgScreenNeedReresh=this;
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_IDNTFYAUDTRSTMPPUP_AER","HSE_IDNTFYAUDTRSTMPPUP_AER","Update Auditors Team","","MAINLINK",-1);
				/////////////////////////////////////////////////////////////////////////
			}
			else if (strButtonName=="ACTUAL_AUDITEE_PERSONS")
			{
				/*CString strAuditUnit=FormGetField(strSubFormTag,"AudtPlnEntry_Audts_AudtUnt"); 
				CString strCriteria;
				strCriteria.Format("Audt_AudtUnt_Dprtmnt from HSE_AUDT_AUDTUNT where PrmKy=%s",strAuditUnit);			*/
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_ACTULAUDTPRSNSPPUP","HSE_ACTULAUDTPRSNSPPUP","Actual Auditee Persons","","MAINLINK",ID_POPUPS_ACTUALAUDITEEPERSONSPOPUP);
			}
			if(strButtonName=="AUDIT_COMPLETED")
			{
				//RQ-12-2021.45
				CString strRecordKey=FormGetField(strSubFormTag,"PrmKy");
				CString strSQL;
				strSQL.Format("EXECUTE Execution_Audit_Cmpleted %s",strRecordKey);
				ExecuteSQL(strSQL);			
				RefreshScreen("",REFRESH_SCREEN);				
			}
			if(strButtonName=="SUMMARY_NC_DESCRIPTION")
			{
				////////////////////////////////////////////////////////////////////////////////
				//RQ_HM_17_8_23.01.17.02
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_SmmryNcDscrptn","HSE_TgSmmryNcDscrptn","Summary NC Description","","MAINLINK",ID_POPUPS_SUMMARY_NC_DESCRIPTION);
				////////////////////////////////////////////////////////////////////////////////
			}
		}
	}
	else if(strFormTag=="HSE_AUDTACTNCNFRM")
	{
		if(strSubFormTag=="HSE_AUDTACTNSENTRY_AUDTS")
		{
			if(strButtonName=="VIEW_CLAUSES")
			{
				//////////////////////////////////////////////////////////////////////////////
				//RQ-1-2022.25
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_CLUS_LVLRSULTPPUP_LNK","HSE_CLUS_LVLRSULTPPUP_LNK","Audit Clauses","","MAINLINK",-1);
				//////////////////////////////////////////////////////////////////////////////
			}
			else if(strButtonName=="REJECT_ACTIONS")
			{
				
				//////////////////////////////////////////////////////////////////////////////
				//RQ-1-2022.25
				int nRet = MessageBox(NULL,"Are you sure to reject actions?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
				if(nRet == IDYES) {
					CString strPrmKey=FormGetField(strSubFormTag,"PrmKy");
					CString strSQL;
					strSQL.Format("EXECUTE Audit_Reject_Action %s",strPrmKey);
					CString strMessage= GetRecordSetData(strSQL).GetBuffer(0);	
					if(strMessage=="OK" || strMessage=="")
					{
						RefreshScreen("",REFRESH_SCREEN);	
					}
					else
						AfxMessageBox(strMessage);
				}
				
				//////////////////////////////////////////////////////////////////////////////
				/*Removed By Mr Sameer
				//////////////////////////////////////////////////////////////////////////////
				//RQ-4-2022.8
				int nRet = MessageBox(NULL,"Are you sure to reject actions?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
				if(nRet == IDYES) {
					CString strPrmKey=FormGetField(strSubFormTag,"PrmKy");
					CString strSQL;
					strSQL.Format("EXECUTE Audit_Reject_Action_V2 %s",strPrmKey);
					CString strMessage= GetRecordSetData(strSQL).GetBuffer(0);	
					if(strMessage=="OK" || strMessage=="")
					{
						RefreshScreen("",REFRESH_SCREEN);	
					}
					else
						AfxMessageBox(strMessage);
				}
				*/
				//////////////////////////////////////////////////////////////////////////////
			}
			else if(strButtonName=="GENERATE_CARS")
			{
				//RQ-1-2022.25
				//RQ-4-2022.7
				CString strPrmKey=FormGetField(strSubFormTag,"PrmKy");
				CString strSQL;
				strSQL.Format("EXECUTE Audit_Action_Confirmation_Audits_Generate_CARs %s",strPrmKey);
				///////////////////////////////////////////////////////////////
				//--Bugs-4-2022.18
				CString strResult= GetRecordSetData(strSQL).GetBuffer(0);	
				if(strResult=="Ok" || strResult=="")
				{			
					RefreshScreen("",REFRESH_SCREEN);				
				}
				else
					AfxMessageBox (strResult);
				///////////////////////////////////////////////////////////////
				RefreshScreen("",REFRESH_SCREEN);	
			}

		}
	}
	
	else if(strFormTag=="HSE_AUDTACTNSENTRY")
	{
		if(strSubFormTag=="HSE_AUDTACTNSENTRY_AUDTS")
		{
			if(strButtonName=="AUDIT_CLAUSES")
			{
				////////////////////////////////////////////////////////////////////////////////
				//RQ-1-2022.10
				//OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_AUDTCLUSSPPUP","HSE_AUDTCLUSSPPUP","Linked Audit Clauses","","MAINLINK",ID_POPUPS_AUDITCLAUSESPOPUP);
				//Bugs-4-2022.11
				//OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_CLUS_LVLRSULTPPUP_LNK","HSE_CLUS_LVLRSULTPPUP_LNK","Audit Clauses","","MAINLINK",-1);
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_CLUS_LVLRSULTPPUP_AAE","HSE_CLUS_LVLRSULTPPUP_AAE","Audit Clauses","","MAINLINK",-1);
				////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////
			}
			else if(strButtonName=="CARS_ENTRY_COMPLETED")
			{
				CString strCARCompleted=FormGetField(strSubFormTag,"Clus_LvlRsultPpUp_Fndngs_CARCmpltd"); 
				//RQ-1-2022.15
				/////////////////////////////////////////////////////////
				if(strCARCompleted=="Y")
					AfxMessageBox("CAR Entry is already completed");
				else
				{
					////////////////////////////////////////////////////////////////////////////////
					//RQ-1-2022.16
					CString strRecordKey=FormGetField(strSubFormTag,"PrmKy");
					CString strSQL;
					strSQL.Format("EXECUTE Check_CAR_Entry_Completed %s",strRecordKey);
					CString strResult= GetRecordSetData(strSQL).GetBuffer(0);	
					if(strResult=="Ok" || strResult=="")
					{			
						RefreshScreen("",REFRESH_SCREEN);				
					}
					else
						AfxMessageBox (strResult);
					////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}
	if(strFormTag=="HSE_CLUS_LVLRSULTPPUP" || strFormTag=="HSE_CLUS_LVLRSULTPPUP_LNK" || strFormTag=="HSE_CLUS_LVLRSULTPPUP_AAE")
	{
		if(strSubFormTag=="HSE_CLUS_LVLRSULTPPUP_FNDNGS" || strSubFormTag=="HSE_CLUS_LVLRSULTPPUP_FNDNGS_AAE")
		{
			if (strButtonName=="ENTER_SIMILAR_NC")
			{
				//BUG-3-2022.9
				CString strSimilar_NC_Exist =FormGetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_SMLNCEXS"); 
				if(strSimilar_NC_Exist == "Y")
				{
					//RQ-2-2022.4
					OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGSMLNC","HSE_TGSMLNC","Enter Similar NC","","MAINLINK",-1);
				}
			}
			else if (strButtonName=="VIEW_SIMILAR_NC")
			{
				//RQ-2-2022.9
				//Read only
				OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGSMLNC_LNK","HSE_TGSMLNC_LNK","View Similar NC","","MAINLINK",-1);
			}
			else if(strButtonName=="ACTIONS_COMPLETED")
			{
				///////////////////////////////////////////////////////
				//RQ-4-2022.6 --Remove the button
				/*
				///////////////////////////////////////////////////////
				//RQ-2-2022.9
				int nRet = MessageBox(NULL,"Are actions completed for this finding?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
				if(nRet == IDYES) {
					FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_ACTCMP","Y");
				}*/
				///////////////////////////////////////////////////////
	
			}
			else if(strButtonName=="ENTER_CORRECTION_ACTIONS" || strButtonName=="VIEW_CORRECTION_ACTIONS"/*RQ-4-2022.32*/)
			{
				//Bugs-3-2022.12 Moved the code to here from another location below
				///////////////////////////////////////////////////////
				//RQ-2-2022.7
				CString strActionsCompleted=FormGetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_ACTCMP");
				CString strCARCompleted=FormGetField(strSubFormTag,"Clus_LvlRsultPpUp_Fndngs_CARCmpltd"); //RQ-4-2022.4
				if(strActionsCompleted!="Y")
				{
					if(strCARCompleted=="N")//RQ-4-2022.4
					{
						if(strButtonName=="VIEW_CORRECTION_ACTIONS")//RQ-4-2022.32
							OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGCRRCTNACT_AAC","HSE_TGCRRCTNACT_AAC","Correction Actions","","MAINLINK",-1);
						else
							OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGCRRCTNACT","HSE_TGCRRCTNACT","Correction Actions","","MAINLINK",-1);						
					}
					else
						AfxMessageBox("CAR is already completed");
				}
				else
					AfxMessageBox("Action is already completed");
				///////////////////////////////////////////////////////
			}
			else if(strButtonName=="ENTER_CORRECTIVE_ACTIONS" || strButtonName=="VIEW_CORRECTIVE_ACTIONS"/*RQ-4-2022.27*/)
			{
				/*
				//Removed by RQ-4-2022.4
				CString strRqrAction=FormGetField(strSubFormTag,"CLUS_LVLRSULTPPUP_FNDNGS_RQRACT"); 
				if(strRqrAction=="N")
				{
					//RQ-1-2022.17
					////////////////////////////////////////////////////////////////////////////////////////
					AfxMessageBox("Recommened actions are not required for this finding");
					////////////////////////////////////////////////////////////////////////////////////////
				}
				else
				*/
				{
					////////////////////////////////////////////////////////////////////////////////////////
					//Bugs-3-2022.13
					//RQ-2-2022.8,
					CString strActionsCompleted=FormGetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_ACTCMP");
					CString strCARCompleted=FormGetField(strSubFormTag,"Clus_LvlRsultPpUp_Fndngs_CARCmpltd"); 
					if(strActionsCompleted!="Y")
					{
						if(strCARCompleted=="N")//RQ-1-2022.13
						{
							if(strButtonName=="VIEW_CORRECTIVE_ACTIONS")
								OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGRTCSS_AAC","HSE_TGRTCSS_AAC","Root Causes","","MAINLINK",-1);//RQ-4-2022.27
							else
								OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_TGRTCSS","HSE_TGRTCSS","Root Causes","","MAINLINK",-1);
						}
						else
							AfxMessageBox("CAR is already completed");
					}
					else
						AfxMessageBox("Action is already completed");
					////////////////////////////////////////////////////////////////////////////////////////
					//RQ-1-2022.13					
					/* Overwritten by RQ-2-2022.8
					CString strCARCompleted=FormGetField(strSubFormTag,"Clus_LvlRsultPpUp_Fndngs_CARCmpltd"); 
					if(strCARCompleted=="N")
					////////////////////////////////////////////////////////////////////////////////////////
						OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_VWRCMMNDDACTNSPPUP","HSE_VWRCMMNDDACTNSPPUP","Recommended Actions","","MAINLINK",ID_POPUPS_VIEWRECOMMENDEDACTIONSPOPUP);
					else
						AfxMessageBox("CAR is already completed");*/
					////////////////////////////////////////////////////////////////////////////////////////
				}
			}
		}
	}

	
	if(strSubFormTag=="HSE_AUDTCLUSS_LNKDSTNDRDS")
	{
		OpenPopUpScreen(strSubFormTag,strSubFormTag,"KeyField","HSE_AudtChcklst","HSE_AudtChcklst","Audit Check List","","MAINLINK",ID_POPUPS_AUDITCHECKLISTPOPUP);
	}

	else if(strSubFormTag=="HSE_AUDT_AUDTUNT")
	{
		if(strButtonName=="LINKED_AUDIT_CLAUSES")
		{
			OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_LNKDAUDTCLUSSPPUP","HSE_LNKDAUDTCLUSSPPUP","Linked Audit Clauses","","MAINLINK",ID_POPUPS_LNKDAUDTCLUSSPPUP);
		}
		else if(strButtonName=="LINKED_AUDIT_LOCATIONS")
		{
			OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_LNKDAUDTLCTNPPUP","HSE_LNKDAUDTLCTNPPUP","Linked Auditee Locations","","MAINLINK",ID_POPUPS_LNKDAUDTLCTNPPUP);
		}
		else if(strButtonName=="CONTACT_PERSONS")
		{
			CString strAuditeeDepartment=FormGetField(strSubFormTag,"AUDT_AUDTUNT_DPRTMNT"); 
			CString strCriteria;
			strCriteria.Format("AUDT_AUDTUNT_DPRTMNT= '%s'",strAuditeeDepartment);
			OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_Adte_CntctPrsn","HSE_TgAdte_CntctPrsn","Contact Persons","","MAINLINK",-1,strCriteria);
		}
	}


	/*
	//This section is merged with the same section above
	else if(strSubFormTag=="HSE_CLUS_LVLRSULTPPUP_FNDNGS")//"HSE_AUDTCLUSSPPUP_FNDNGS")
	{
		if(strButtonName=="CAR_ENTRY_COMPLETED")
		{
			//RQ-1-2022.15
			////////////////////////////////////////////////////////////////////////////////////////
			CString strPrimaryKey=FormGetField(strSubFormTag,"PrmKy");
			
			CString strSQL;
			strSQL.Format("EXECUTE CheckCarCommpleted %s",strPrimaryKey);
			CString strResult= GetRecordSetData(strSQL).GetBuffer(0);	
			if(strResult=="Ok")
			{
				CString strUpdateSQL;
				strUpdateSQL.Format("update HSE_CLUS_LVLRSULTPPUP_FNDNGS set Clus_LvlRsultPpUp_Fndngs_CARCmpltd='Y' where PrmKy=%s",strPrimaryKey);
				long lRetFieldsCount = ExecuteSQL(strUpdateSQL);
				RefreshScreen("",REFRESH_SELECTED);	
			}
			////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	*/

	else if(strSubFormTag=="HSE_AUDTACTNSCNFRMTN_OUTSTNDNGFNDNGS")
	{
		if(strButtonName=="VIEW_RECOMMENDED_ACTIONS")
		{
			//	RQ-1-2022.22: Remove the button
			//	OpenPopUpScreen(strSubFormTag,strSubFormTag,"PrmKy","HSE_VWRCMMNDDACTNSPPUP","HSE_VWRCMMNDDACTNSPPUP","Recommended Actions","","MAINLINK",ID_POPUPS_VIEWRECOMMENDEDACTIONSPOPUP);
		}
	}

	else if(strFormTag=="HSE_ADTRSLTCNFRMTN" && strSubFormTag=="HSE_ADTRSLTCNFRMTN_AUDTS")
	{
		if(strButtonName=="RESULT_CONFIRMED")
		{
			//	RQ_HM_17_8_23.01.23.01
			/*
			•  SET (Audit Status) = “Confirmed”
			•  SET (@A) = Count (clause-level result) records, WHERE (Conforming = No)
			•  If (@A) > (0) then INSERT record into (CAR Entry) Screen, as shown in next step
			•  Refresh the (Audits) Tab		*/
			//FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_PLNNDENDDT",dtDate.Format());
			
			int nClauseLevelResultCnt=0;
			int nPrmKy=atof(FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","PrmKy"));
			CString strSQL;
			strSQL.Format ("select count(0) from HSE_CLUS_LVLRSULTPPUP where MainLink=%d",nPrmKy);
			int nCount = atof(GetRecordSetData(strSQL).GetBuffer(0));
			if (nCount >0)
			{
				//INSERT record into (CAR Entry) Screen
				if(AuditConfirmation_AddRecordToCAREntry(nPrmKy))
				{
					FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_AUDTSTTUS","10");
					//BUG_HM_31_8_23.01					
					DoToolBarAction(TOOLBAR_SAVE,"HSE_AUDTPLNENTRY","HSE_AUDTPLNENTRY_AUDTS");
					DoToolBarAction(TOOLBAR_SAVE,"HSE_AUDTPLNENTRY","");
					////////////////////////////
					//m_pHSEMSPolyDb->SubMakeRefresh();	
					m_pHSEMSPolyDb->FormMakeRefresh(-1);
				}
			}

		}
		else if(strButtonName=="SUMMARY_NC_DESCRIPTION")
		{
			//RQ_HM_17_8_23.01.17.03
			OpenPopUpScreen(strSubFormTag,"HSE_ADTRSLTCNFRMTN_AUDTS","PrmKy","HSE_SmmryNcDscrptn","HSE_TGSmmryNcDscrptn","Summary NC Description","","MAINLINK",ID_POPUPS_SUMMARY_NC_DESCRIPTION);
		}

		if(strButtonName=="CLAUSE-LEVEL_RESULT")//HSM TEMP since I don`t know yet the exact behaviour of the button
		{
			OpenPopUpScreen(strSubFormTag,"HSE_AUDTEXCUTNRSULT_AUDTS","PrmKy","HSE_CLUS_LVLRSULTPPUP","HSE_CLUS_LVLRSULTPPUP","Clause Level Results","","MAINLINK",ID_POPUPS_CLAUSELEVELRESULTPOPUP);
		}
	}

	else if(strFormTag=="HSE_MRPRSNSTNTFYPPUP")
	{
		if(strButtonName=="RELOAD_LIST")
		{
			/////////////////////////////////////////////////
			//RQ-12-2023.8
			CString strSQL;
			strSQL.Format("EXECUTE Reload_NotifyList %d",gnMainLink);
			ExecuteSQL(strSQL);			
			m_pHSEMSPolyDb->FormMakeRefresh(-1);
			/////////////////////////////////////////////////
		}
	}
	/////////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT CAuditModuleCategory::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	strFieldName.MakeUpper();
	CString strFull_Record  = pBeforBrowsingButtonClick->Full_Record;	
	if (strFieldName=="ACTULAUDTPRSNSPPUP_EMPLY")
	{
		/////////////////////////////////////////////////////////////////////////////
		//RQ-12-2021.42
		CString strMainLink	= GetFieldValue(strFull_Record,"MAINLINK");	
		CString strSQL;
		strSQL.Format("EXECUTE Get_Department_Code_For_Audit %s",strMainLink);
		CString strDepartmentCode= GetRecordSetData(strSQL).GetBuffer(0);	
		CString strFilter;
		strFilter.Format("WHERE (EMPLOYEE_WSHOP ='%s')",strDepartmentCode);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL=strFilter.AllocSysString();
		/////////////////////////////////////////////////////////////////////////////
	}
	else if (strFieldName=="CLUS_LVLRSULTPPUP_FNDNGS_AUDTRNM")
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//RQ-12-2021.29
		CString strMainLink	= GetFieldValue(strFull_Record,"MAINLINK");	
		CString strFilter;
		strFilter.Format("where (PrmKy in (select IdntfyAudtrsTmPpUp_AssgndAudtr from HSE_IdntfyAudtrsTmPpUp where MainLink in (select MainLink from HSE_CLUS_LVLRSULTPPUP where PrmKy=%s)))",strMainLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL=strFilter.AllocSysString();
		///////////////////////////////////////////////////////////////////////////////////////
	}

	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT CAuditModuleCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFormTag(pFieldChange->Form_Tag);
	CString strFieldName(pFieldChange->Field_Name);	
	strFieldName.MakeUpper();
	CString strNewValue(pFieldChange->New_Value);
	//HSE Audit Module 25/11/2021
	if(strFieldName=="AUDTPLNENTRY_AUDTYR")
	{
		CalculateNewSerial(strFormTag,strSubFormTag);
	}
	/////////////////////////////////////////////////////////////////
	if(strFieldName=="CLUS_LVLRSULTPPUP_AUDTSTTUS")
	{
		//////////////////////////////////////////////////
		//RQ-1-2022.44
		
		if(strNewValue=="O")
		{
			FieldSetAttribSingle("CLUS_LVLRSULTPPUP_NCLVL","",-1,"","","","A",-1,"","","","","[For Action|A][For Improvements|M]",0,0,"");
			FormSetField("HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_NCLVL","A");
		}
		else if(strNewValue=="C")
		{
			FieldSetAttribSingle("CLUS_LVLRSULTPPUP_NCLVL","",-1,"","","","N",-1,"","","","","[None|N]",0,0,"");
			FormSetField("HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_NCLVL","N");
		}
		else if(strNewValue=="N")
		{
			FieldSetAttribSingle("CLUS_LVLRSULTPPUP_NCLVL","",-1,"","","","I",-1,"","","","","[Minor|I][Major|J]",0,0,"");
			FormSetField("HSE_CLUS_LVLRSULTPPUP","CLUS_LVLRSULTPPUP_NCLVL","I");
		}
		//////////////////////////////////////////////////

	}
	else if(strFieldName=="CLUS_LVLRSULTPPUP_FNDNGS_RSLSTT")
	{
		//////////////////////////////////////////////////
		//RQ-1-2022.52
		
		if(strNewValue=="O")
		{
			LinkFieldsAttribSingle("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","",-1,"","","","A",-1,"","","","CODE","[For Action|A][For Improvements|M]",0,0,"");
			FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","A");
		}
		else if(strNewValue=="N")
		{
			LinkFieldsAttribSingle("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","",-1,"","","","I",-1,"","","","CODE","[Minor|I][Major|J]",0,0,"");
			FormSetField("HSE_CLUS_LVLRSULTPPUP_FNDNGS","CLUS_LVLRSULTPPUP_FNDNGS_NCLVL","I");
		}
		//////////////////////////////////////////////////

	}
	else if(strFieldName=="AUDTPLNENTRY_AUDTS_PLNNDSTRTDT")
	{
		/////////////////////////////////////////////////////////////
		//RQ-1-2022.51
		CString strSQL;
		strSQL.Format("EXECUTE Get_PLC_ADT_ENDDTDYS");
		CString strPolicyAdtEndDates= GetRecordSetData(strSQL).GetBuffer(0);
		COleDateTime dtDate;
		ParseDateTime(&dtDate,strNewValue);	
		dtDate+=COleDateTimeSpan( atoi(strPolicyAdtEndDates)-1, 0, 0, 0 );//RQ-2-2022.1,Bugs-3-2022.2
		FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_PLNNDENDDT",dtDate.Format());
		/////////////////////////////////////////////////////////////
	}
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT CAuditModuleCategory::DisplayBrowsingSelectedItem(tag_BrowsingSelectedItem *pBrowsingSelectedItem)
{
	CString strSQL;
	CString strFormTag    = pBrowsingSelectedItem->Form_Tag;
	CString strSubFormTag = pBrowsingSelectedItem->SubForm_Tag;
	CString strFieldName  = pBrowsingSelectedItem->BrowsingField;	
	CString strFull_Record= pBrowsingSelectedItem->Full_Record;
	strFieldName.MakeUpper();

	if(strFormTag=="HSE_IDNTFYAUDTCLUSSPPUP")
	{
		if(strSubFormTag=="")
		{
			if(strFieldName=="IDNTFYAUDTCLUSSPPUP_CLUSNM")
			{
				CString strAuditScope	= GetFieldValue(strFull_Record,"AUDTCLUSS_AUDTSCP");	
				CString strSpcsInstrctns= GetFieldValue(strFull_Record,"AUDTCLUSS_SPCLINSTRUCTNS");	
				FormSetField(strFormTag,"IDNTFYAUDTCLUSSPPUP_AUDTSCP",strAuditScope);
				FormSetField(strFormTag,"IDNTFYAUDTCLUSSPPUP_SPCLINSTRUCTNS",strSpcsInstrctns);
			}
			else if(strFieldName=="IDNTFYAUDTCLUSSPPUP_ISOSTNDRD")
			{
				CString strDefltMnDys	= GetFieldValue(strFull_Record,"AUDTCLUSS_LNKDSTNDRDS_DFULTMNDYS");	
				FormSetField(strFormTag,"IDNTFYAUDTCLUSSPPUP_EXPCTDMNDYS",strDefltMnDys);
			}

		}
		else
		{
		}
	}

	return CHSEMSCommonCategory::DisplayBrowsingSelectedItem(pBrowsingSelectedItem);
}

HRESULT CAuditModuleCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag	  = pRecordRepostion->Form_Tag;
	CString strSubFormTag = pRecordRepostion->SubForm_Tag;
	CString strFullRecord = pRecordRepostion->Full_Record;		
	if((*pRecordRepostion).iComplete==1)
	{
		if(strSubFormTag=="HSE_AUDTEXCUTNRSULT_AUDTS")
		{
			CString strPlanned	= GetFieldValue(strFullRecord,"AUDTPLNENTRY_AUDTS_PLNND");	
			if(strPlanned=="N")
			{
				EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,TRUE);			
			}
			else
			{
				EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_DELETE,FALSE);			
			}
		}
		else if(strSubFormTag=="HSE_AUDTNTFCTN_AUDTS")
		{
			//////////////////////////////////////////////////////////////////////////////////////////
			//RQ-12-2021.48
			CString strAudtStatus	= GetFieldValue(strFullRecord,"AUDTPLNENTRY_AUDTS_AUDTSTTUS");
			if(strAudtStatus!="")
			{
				if(strAudtStatus=="8")//Canceled
				{
					EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_SAVE,FALSE);			
				}
				else
				{
					EnableToolbar(strFormTag,strSubFormTag,TOOLBAR_SAVE,TRUE);							
				}
			}		
		} 
		if(strSubFormTag=="HSE_CLUS_LVLRSULTPPUP_FNDNGS")
		{
			CString strParentIsConfirming=FormGetField("HSE_CLUS_LVLRSULTPPUP","Clus_LvlRsultPpUp_Cnf"); 
			if(strParentIsConfirming=="Y")
			{
				EnableTab("HSE_CLUS_LVLRSULTPPUP_FNDNGS",false);
			}
			else
			{
				EnableTab("HSE_CLUS_LVLRSULTPPUP_FNDNGS",true);
			}
		}
	}
	
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

void CAuditModuleCategory::CheckEmail(CString strFormTag,CString strField,long *iCancle,bool bCheckOnly)
{
	CString strEmail=FormGetField(strFormTag,strField); 
	if(strEmail=="")// || strEmail.Find("@")==-1)
	{
		//////////////////////////////////////////
		//RQ-1-2022.40,RQ-1-2022.41,RQ-1-2022.43
		//AfxMessageBox("Email is empty, please check");
		if(bCheckOnly)
			AfxMessageBox("Email is empty, just a warning");
		else
		{
			AfxMessageBox("Email is empty, record can not be saved");
			*(iCancle)=1;
		}
		//////////////////////////////////////////
	}
}

bool CAuditModuleCategory::AuditConfirmation_AddRecordToCAREntry(int nParentPrmKy)
{
	/////////////////////////////////////////////////////
	//RQ-10-2023.16
	bool bRetVal=false;
	CString strEmployeeName=GetEmployeeCodeForLoginUser();
	///////////////
	if(strEmployeeName!="")
	{
		//RQ_HM_17_8_23.01.23.02
		/*
			INSERT record into (CAR Entry) Screen, as follows :
			SET (CAR Date) = current date																	CRENTRY_CRDT
			SET (CAR Year) = current year																	CRENTRY_CRYR
			SET (CAR Priority) = “Medium”																	CRENTRY_CRPRR
			SET (Auto Generated) = Yes																		CRENTRY_ATGNR
		*/
			CString m_strCARDate = GetFullDateTime("%m/%d/%Y");	
			CString m_strCARYear = GetCurrentYear();
			CString strCarPeriority="M";
			CString strAutoGen="Y";
		/*

			SET (CAR Status) = (HSE Policy > Audit > Generated CAR Status)									CRENTRY_CRSTT
			SET (Department) = (HSE Policy > Owner Department)												CRENTRY_DPR
			SET (CAR Source) = (HSE Policy > Audit > CAR Basis)												CRENTRY_CRSRC
		*/
			CString strVal=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_GNRCRSTT");
			CString strCARStatus;
			if(strVal=="")
				strCARStatus="NULL";
			else
				strCARStatus.Format("'%s'",strVal);

			CString m_strDepart;
			strVal=GetPolicyTabField("HSE_HSEPLC","HSEPLC_OWNRDPRT");
			if(strVal=="")
				m_strDepart="NULL";
			else
				m_strDepart.Format("'%s'",strVal);

			CString strCARSrc;
			strVal=GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_CRBSS");
			if(strVal=="")
				strCARSrc="NULL";
			else
				strCARSrc.Format("'%s'",strVal);
		/*

			SET (Employee) = (Employee > Employee No.) of logged-in user									CRENTRY_NM
			SET (Job Title) = driven from (Employee) record							Driven
		*/

		
		 /*
			SET (NC Description) = (Audits) Tab >	(Summary NC Description)								CRENTRY_NCDSC
			SET (Evidence) = (Audits) Tab >			(Summary Evidence)										CRENTRY_EVD
			SET (Site) = (Audits) Tab > (Site)																CRENTRY_ST
			SET (Location) = (Audits) Tab > (Main Location)													CRENTRY_LCT
			SET (Area) = (Audits) Tab > (Main Area)															CRENTRY_AR
			SET (Concerned Department) = (Audits) Tab > (Auditee Department)								CRENTRY_CNCDPR
			SET (Project) = (Audits) Tab > (Project)														CRENTRY_PRJ
			SET (Detail No.) = (Audit Result Entry > Audits > Audit No.)									CRENTRY_DTLN
		*/
			/*
				RQ-10-2023.16
				in (Audits) Tab > click (Result Confirmed) button
				•  When user clicks (Result Confirmed) button, currently the system creates (1) CAR record for the current “audit”.  The mechanism will be changed to create multiple CAR records, one CAR record against each record in ((Clause-Level Result) popup > (Findings) Tab with (require CAR = Yes)).

				Example: If the current record in the Audits tab has (3) records in (Item Level Result) pop-up, and each has (2) records in the (Findings) tab with (CAR Required = Yes), then system should create (6) CAR records into the respective (CAR) table.

				While inserting the CAR record, please :
					 o  Copy the fields (Site, Location, Area, Exact Area) from “findings” record, to similar fields in the CAR record
					 o  Copy (evidence photo) from “findings” record, to (NC photo) field in the CAR record
					 o  Copy (login-user > employee name) to (employee) field in the CAR record

			*/

			/*CString strNCDescription;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_SMMNCDSC"); 
			if(strVal=="")
				strNCDescription="NULL";
			else
				strNCDescription.Format("'%s'",strVal);*/

			/*CString strEvidence;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_SMMEVD"); 
			if(strVal=="")
				strEvidence="NULL";
			else
				strEvidence.Format("'%s'",strVal);*/

			/*
			RQ-10-2023.16
			CString strSite;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_ST"); 
			if(strVal=="")
				strSite="NULL";
			else
				strSite.Format("'%s'",strVal);

			CString strLocation;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_MNLCT");
			if(strVal=="")
				strLocation="NULL";
			else
				strLocation.Format("'%s'",strVal);

			CString strArea;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_MNAR");
			if(strVal=="")
				strArea="NULL";
			else
				strArea.Format("'%s'",strVal);*/

					
			CString StrAudtUnt=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AudtPlnEntry_Audts_AudtUnt");
			CString strQuery;
			strQuery.Format("SELECT Audt_AudtUnt_Dprtmnt FROM HSE_Audt_AudtUnt WHERE PRMKY=%s",StrAudtUnt);
			CString strConcDep;
			strVal= GetRecordSetData(strQuery).GetBuffer(0);	
			if(strVal=="")
				strConcDep="NULL";
			else
				strConcDep.Format("'%s'",strVal);



			CString strPrjct;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_PRJCT");
			if(strVal=="")
				strPrjct="NULL";
			else
				strPrjct.Format("'%s'",strVal);

			CString strAuditNo;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN_AUDTS","AUDTPLNENTRY_AUDTS_AUDTN");
			if(strVal=="")
				strAuditNo="NULL";
			else
				strAuditNo.Format("'%s'",strVal);
			
		/*
			
		
			SET (TXN Year) = (Audit Result Entry > Main > Audit Year)										CRENTRY_TXNYR
			SET (TXN No.) = (Audit Result Entry > Main > Plan No.)											CRENTRY_TXNN
		*/
			CString strTXNYr;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN","AUDTPLNENTRY_AUDTYR");
			if(strVal=="")
				strTXNYr="NULL";
			else
				strTXNYr.Format("'%s'",strVal);

			CString strTXNNo;
			strVal=FormGetField("HSE_ADTRSLTCNFRMTN","AUDTPLNENTRY_PLNN");
			if(strVal=="")
				strTXNNo="NULL";
			else
				strTXNNo.Format("'%s'",strVal);

		/*
			SET (CAR No.) = next available (CAR No.) per current year										CRENTRY_CRN
			SET (CAR Serial No.) = auto serial No. based on (CAR Year & CAR Source)							CRENTRY_CRSRLN
		*/
			
			CString strCurYear=GetCurrentYear();
			CString strSql;
			strSql.Format("select top(1) CRENTRY_CRN from hse_CRENTRY where CRENTRY_cryr=%s order by CRENTRY_CRN desc",strCurYear);
			CString strMaxCARNo= GetRecordSetData(strSql).GetBuffer(0);	
			int dNextCARNo=atoi(strMaxCARNo)+1;
			strSql.Format("select top(1) CRENTRY_CRSRLN from hse_CRENTRY where CRENTRY_cryr=%s and CrEntry_CrSrc='%s' order by CRENTRY_CRSRLN desc",strCurYear,strCARSrc);
			CString strMaxCARSerial= GetRecordSetData(strSql).GetBuffer(0);	
			int m_dNextCARSerial=atoi(strMaxCARSerial)+1;

			
		/*
			When user clicks (Audits) tab > (result confirmed) button, system generates No. CAR records, similar to No. “finding” records.

			In the generated CAR record, there are (3) fields are left blank & it is necessary to fill them as follows :
			- SET (CAR) > (main > NC description) = (Audits) tab > (clause-level result) popup > (findings) tab > (finding description)			CRENTRY_NCDSC
			- SET (CAR) > (main > evidence) = (Audits) tab > (clause-level result) popup > (findings) tab > (evidences)							CRENTRY_EVD
			- SET (CAR) > (main > NC photos) = (Audits) tab > (clause-level result) popup > (findings) tab > (evidence photos)					CRENTRY_NCPHT
		*/

			CString strInsertSql;
			strInsertSql.Format("insert into HSE_CrEntry \
				(CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK,CRENTRY_CRDT,CRENTRY_CRYR,CRENTRY_CRPRR,CRENTRY_ATGNR,CRENTRY_CRSTT,CRENTRY_DPR,CRENTRY_CRSRC, \
				CRENTRY_ST, CRENTRY_LCT, CRENTRY_AR,CRENTRY_EXCAR,CRENTRY_NCDSC,CRENTRY_EVD,CRENTRY_CNCDPR,CRENTRY_PRJ,CRENTRY_DTLN, \
				CRENTRY_TXNYR,CRENTRY_TXNN, CRENTRY_CRN,CRENTRY_CRSRLN,CRENTRY_NM) \
				select PrmKy,'%s','%s','%s','%s',%s,%s,%s,CLUS_LVLRSULTPPUP_FNDNGS_ST,CLUS_LVLRSULTPPUP_FNDNGS_LCT, \
				CLUS_LVLRSULTPPUP_FNDNGS_AR,CLUS_LVLRSULTPPUP_FNDNGS_EXCAR,CLUS_LVLRSULTPPUP_FNDNGS_FNDNGDSCRPTN, \
				CLUS_LVLRSULTPPUP_FNDNGS_EVDNCS,%s,%s,%s,%s,%s,%d,%d,'%s' \
				 from hse_CLUS_LVLRSULTPPUP_FNDNGS where MainLink in \
				(select PrmKy from HSE_CLUS_LVLRSULTPPUP where MainLink=%d) and CLUS_LVLRSULTPPUP_FNDNGS_ACTCMP='Y'",m_strCARDate,	m_strCARYear,	strCarPeriority,strAutoGen,strCARStatus, \
				m_strDepart,strCARSrc,	strConcDep,strPrjct,	strAuditNo,		strTXNYr,		\
				strTXNNo,	dNextCARNo,m_dNextCARSerial,strEmployeeName,nParentPrmKy); //,CLUS_LVLRSULTPPUP_FNDNGS_EVDNCPHTS ,CRENTRY_NCPHT --To be added after solving the problem of insering a photo
			CString str;
			str.Format("IF OBJECT_ID('tempdb..##TEMP_HSE_TABLE') IS NULL CREATE TABLE ##TEMP_HSE_TABLE(KEYrec VARCHAR(50) NOT NULL,VALUErec VARCHAR(50),CONSTRAINT PK_TEMP_HSE_TABLE PRIMARY KEY CLUSTERED (KEYrec))");
			long lRecords=ExecuteSQL(str);
			str.Format("delete from ##TEMP_HSE_TABLE");
			lRecords=ExecuteSQL(str);
			str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('ActionDescription','Creating new CAR')");
			lRecords=ExecuteSQL(str);
			str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('SrcScreen','Audit Result Confirmation')");
			lRecords=ExecuteSQL(str);		
			CString strUserID=m_pCategory->GetUserLogin();
			str.Format("INSERT INTO ##TEMP_HSE_TABLE(KEYrec,VALUErec) VALUES('UserID','%s')",strUserID);
			lRecords=ExecuteSQL(str);

			lRecords=ExecuteSQL(strInsertSql);
			bRetVal=(lRecords>0)?true:false;
			if(lRecords==0)
				AfxMessageBox("There is no findig that require a CAR");//This means that the record couldn't be inserted
			else
			{
				CString strSQL;
				strSQL.Format ("select CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK from HSE_CrEntry where CRENTRY_CRDT='%s'",m_strCARDate);
				CString strPrmKeysOfFindings= GetRecordSetData(strSQL).GetBuffer(0);
				strPrmKeysOfFindings.Replace(char(2),'|');
				CString strPrmKeysOfAFinding=strtok(strPrmKeysOfFindings.GetBuffer(0),"|");
				int num=1;
				while(strPrmKeysOfAFinding!="") //Loop on all findings
				{
					CString strSql;
					strSql.Format ("EXEC CopyImages 'HSEMS','HSE_CLUS_LVLRSULTPPUP_FNDNGS','PrmKy','%s','CLUS_LVLRSULTPPUP_FNDNGS_EVDNCPHTS','HSE_CRENTRY','CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK','%s','CRENTRY_NCPHT'",strPrmKeysOfAFinding,strPrmKeysOfAFinding);
					lRecords=ExecuteSQL(strSql);
					strPrmKeysOfAFinding=strtok(NULL,"|");	
					if (strPrmKeysOfAFinding!="")
					{
						strSql.Format ("UPDATE HSE_CRENTRY set CrEntry_CrN=CrEntry_CrN+%d,CrEntry_CrSrlN=CrEntry_CrSrlN+%d where CRENTRY_CLUS_LVLRSULTPPUP_FNDNGS_PK=%s and CRENTRY_CRDT='%s'",num,num,strPrmKeysOfAFinding,m_strCARDate);
						ExecuteSQL(strSql);
					}
					num++;
				}
				AfxMessageBox("CAR(s) created succesfully");
			}
	}

	return bRetVal;
}		

void CAuditModuleCategory::ConfirmToolBarButtonClickedDone(CString strForm_Tag,short Seleted_Tab,CString strButton_Name,CString strFullRecord,short nLoop,short Toolbar_ID,CString strSubForm_Tag)
{
	if(strForm_Tag=="HSE_IDNTFYAUDTLCTNSPPUP")
	{
		if(strButton_Name=="EXIT")
		{
			//RQ_HM_17_8_23.01.14.06
			/*
			In (Audits) Tab
			SET (Expected Man Days) = SUM ((Identify Other Auditee Locations) popup > (Expected Man Hours)) / 
			(HSE Policy > Audit > No. Man Hours Per Day).   The resulted (Expected Man Days) should be rounded up			
			*/
			int nPlcyNumMnHrsPerDy=atoi(GetPolicyTabField("HSE_HSEPLC_ADT","HSEPLC_ADT_NMNHRSPRDY"));
			if(nPlcyNumMnHrsPerDy==0)//Validate that the demonaitor is not zero
			{
				AfxMessageBox("The number of Man Hours Per Week in the policy is zero. Can not proceed");
			}
			else
			{
				CString strFindSumHrs;
				strFindSumHrs.Format("select isnull(sum(IDNTFYAUDTLCTNSPPUP_EXPMNHRS),0) sm from hse_IDNTFYAUDTLCTNSPPUP where MainLink=%d",gnMainLink);
				int nSumHrs= atoi(GetRecordSetData(strFindSumHrs).GetBuffer(0));	
				int nDays=(int)ceill(((double)nSumHrs)/((double)nPlcyNumMnHrsPerDy));
				CString strDays;
				strDays.Format("%d",nDays);
				if(gpAuditModuleCurrentParent)
				{
					gpAuditModuleCurrentParent->FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_EXPCTDMNDYS",strDays);
					//RQ-10-2023.14
					COleDateTime dtDate;
					CString strStartDate=gpAuditModuleCurrentParent->FormGetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_PLNNDSTRTDT");
					ParseDateTime(&dtDate,strStartDate);	
					dtDate+=COleDateTimeSpan( atoi(strDays)-1, 0, 0, 0 );
					CString strDate=dtDate.Format("%d %m %Y %H:%M:%S");
					CString strMainScreen=((*(CHSEMSCommonCategory*)(&*gpAuditModuleCurrentParent))).m_FSATableName;
					if(strMainScreen=="HSE_AUDTNTFCTN")
						//RQ-12-2023.3
						gpAuditModuleCurrentParent->FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_SCHDULENDDT",strDate);
					else
						gpAuditModuleCurrentParent->FormSetField("HSE_AUDTPLNENTRY_AUDTS","AUDTPLNENTRY_AUDTS_PLNNDENDDT",strDate);
					gpAuditModuleCurrentParent->DoToolBarAction(TOOLBAR_SAVE,"HSE_AUDTPLNENTRY_AUDTS","");
				}
			}
		}
	}
	CHSEMSCommonCategory::ConfirmToolBarButtonClickedDone(strForm_Tag,Seleted_Tab,strButton_Name,strFullRecord,nLoop,Toolbar_ID,strSubForm_Tag);
}