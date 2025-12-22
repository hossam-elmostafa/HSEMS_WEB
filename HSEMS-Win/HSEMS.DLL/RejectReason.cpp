// RejectReason.cpp: implementation of the CRejectReason class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RejectReason.h"
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRejectReason::CRejectReason(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
	//Bugs-3-2021.63
	setgbUpdateRejectedRecord(false);
}

CRejectReason::~CRejectReason()
{

}
/*********************************************************************************************************/
HRESULT CRejectReason::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{

	CString strButton_Name=pCustomButtonClicked->Button_Name;
	CString strForm_Tag=pCustomButtonClicked->Form_Tag;
	CString strTrcLink=FormGetField("HSE_RJCTRSN","RJCTRSN_TRACINGLNK");
	if(strButton_Name=="RJCTRSN_BTN_OK")
	{
		//Rule ID : 102_300~1_3 CR03-03
		//Rule Description : Reject the record
		if(strTrcLink == "0")
			RejectReasonOk();				
	}
	else if(strButton_Name=="RJCTRSN_BTN_CANCEL")
	{	if(strTrcLink == "0" ||strTrcLink =="")
		setgbUpdateRejectedRecord(false);
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}
/*********************************************************************************************************/
//Helper function
//If user press ok and accept reject reasons
/*********************************************************************************************************/
void CRejectReason::RejectReasonOk()
{
		CString strRejectFrom;
		CString keyFieldPart=getSrcKyFldNm();	
		CString strMainLink=FormGetField("HSE_RJCTRSN","MAINLINK");
		if(strMainLink=="")
		{
			setgbUpdateRejectedRecord(false);
			return;
		}
		
		/*CString strSourceModule=FormGetField("HSE_RJCTRSN","RJCTRSN_MODULETYPE");
		if(strSourceModule.Find("-L1")!=-1)
		{
			strSourceModule.Replace("-L1","");
			InsertActionRecordIntoTracingTab(strSourceModule,keyFieldPart,REJECT);
		}
		else if(strSourceModule.Find("-L2")!=-1)
		{
			strSourceModule.Replace("-L2","");
			InsertActionRecordIntoTracingTab(strSourceModule,keyFieldPart,REJECTF);
		}*/

		setgbUpdateRejectedRecord(true);
		m_pHSEMSPolyDb->FormDoAction(LIST_FORM,UNLOAD);

		//RefreshScreen("",0);
}
/*********************************************************************************************************/
HRESULT CRejectReason::DisplayScreenReady(SCREENREADY *pSreenReady)
{
	//Related to CR03-3
	//If user open reject screen to view record only open it locked 
	//else open screen editable
	SetScreenAttrib();
	return CHSEMSCommonCategory::DisplayScreenReady(pSreenReady);
}


HRESULT CRejectReason::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	int nRecordStatus(pToolBarButtonClicked->nRecordStatus);
	CString strFullRecord = pToolBarButtonClicked->Full_Record;
	CString strTableName= pToolBarButtonClicked->Table_Name;
	CString strSubFormTag = pToolBarButtonClicked->SubForm_Tag;
	CString strFormTag=pToolBarButtonClicked->Form_Tag;
	if(strBtnName=="NEW" && pToolBarButtonClicked->iComplete == 1)
	{
		//polydb_task
		//set default screen values
		CString strRjctRsnSrcScr	= m_pCategory->GetHSEObj()->getstrSrcForRjctRsn();
		CString strKeyValForRjctScr = m_pCategory->GetHSEObj()->getstrKeyValForRjctScr();		
		FormSetField("HSE_RJCTRSN","RJCTRSN_LINKWITHMAIN",strKeyValForRjctScr);				
		FormSetField("HSE_RJCTRSN","RJCTRSN_MODULETYPE",strRjctRsnSrcScr);		
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

//Set Screen attrib according to open button
//if open from reject open it editable
//else open it locked
//according to CR03-3
void  CRejectReason::SetScreenAttrib()
{
	bool bOpenRejectScreenLocked=getgbOpenRejectScreenLocked();
	if (bOpenRejectScreenLocked==true)
	{
		//LockScreen(TRUE);		
		
		//EnableToolbar("",TOOLBAR_SAVE,FALSE);
		//EnableToolbar("",TOOLBAR_NEW,FALSE);
		//EnableToolbar("",TOOLBAR_DELETE,FALSE);
		//SetFieldVisibleAttrib("HSE_RJCTRSN","RJCTRSN_RJCTRSN","ALWAYSLOCKED");
		//SetFieldVisibleAttrib("HSE_RJCTRSN","RJCTRSN_DESC","ALWAYSLOCKED");
		//SetFieldVisibleAttrib("HSE_RJCTRSN","RJCTRSN_CMNT","ALWAYSLOCKED");		
	}
	else if(bOpenRejectScreenLocked==false)
	{
		//EnableToolbar("",TOOLBAR_SAVE,TRUE);
		//EnableToolbar("",TOOLBAR_NEW,TRUE);
		//EnableToolbar("",TOOLBAR_DELETE,TRUE);
		
		//LockScreen(FALSE);
	}
}
/*********************************************************************************************************/
HRESULT CRejectReason::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	//Get browsing Field name
	CString strFieldName= pBeforBrowsingButtonClick->Field_Name;
	if(strFieldName=="RJCTRSN_RJCTRSN")
	{
		CString strLinkWithMain=FormGetField("HSE_RJCTRSN","RJCTRSN_LINKWITHMAIN");
		CString strModuleType=FormGetField("HSE_RJCTRSN","RJCTRSN_MODULETYPE");
		CString strSQL;
		CString strCriteria;
		strSQL.Format("SELECT RJCTRSN_RJCTRSN FROM HSE_RJCTRSN WHERE RJCTRSN_LINKWITHMAIN = '%s' AND RJCTRSN_MODULETYPE = '%s' AND RJCTRSN_TRACINGLNK = 0",strLinkWithMain,strModuleType);
		//polydb_task
		//CString strSelectedRejectReason=(char*)m_pSilentRS->GetRecordSetData(strSQL.AllocSysString(),-1);
		CString strSelectedRejectReason=GetRecordSetData(strSQL.AllocSysString(),-1);
		strSelectedRejectReason.Replace("\2","','");
		if(strSelectedRejectReason.IsEmpty()!=true)
		{
			/*int nlength=strSelectedRejectReason.GetLength();
			nlength-=2;
			CString strFrmSelectedRejectReason=CString("'")+strSelectedRejectReason.Left(nlength);			
			strCriteria.Format("WHERE (ENTRJCRSN_COD NOT IN (%s) and ENTRJCRSN_STS = 1)",strFrmSelectedRejectReason);*/

			strCriteria.Format("WHERE (ENTRJCRSN_COD NOT IN ('%s') and ENTRJCRSN_STS = 1)",strSelectedRejectReason);
		}
		else
		{
			strCriteria="";
		}
			pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strCriteria.AllocSysString(); 		
	}
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

long CRejectReason::ShowRejectreason(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_RJCT;
	return ParentManage("HSE_TGRjctRsn",TRUE,TRUE,TRUE,"HSE_RjctRsn",strCriteria);
}

