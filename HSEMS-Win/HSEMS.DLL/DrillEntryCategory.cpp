#include "stdafx.h"
#include "HSEMS.h"
#include "DrillEntryCategory.h"
#include "HSEobj.h"

DrillEntryCategory::DrillEntryCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_ASPCTS";
}

DrillEntryCategory::~DrillEntryCategory(){}

HRESULT DrillEntryCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(strBtnName=="SAVETAB" || strBtnName=="DELETE"){
		if(pToolBarButtonClicked->iComplete==1) {						 							   
			if(strSubFormTag=="HSE_DRLLPLN_PLNNDSCNR" || strSubFormTag=="HSE_DRLLPLN_ACTULSCNR" || strSubFormTag=="HSE_DRLLPLN_UNPLNDSCNROS") {
				RefreshScreen("",REFRESH_SCREEN);
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT DrillEntryCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strSubFormTag(pRecordRepostion->SubForm_Tag);	
	if(strSubFormTag == "HSE_DRLLPLN_EMT" ||strSubFormTag == "HSE_DRLLPLN_INTRNLRSURCS" ||strSubFormTag == "HSE_DRLLPLN_EXTRNLSUPPRT" || strSubFormTag == "HSE_DRLLEXCUTNATEXCUTN_INVLVDPRTS" || strSubFormTag == "HSE_DRLLPLN_ERT") {
		CString strSrcScr = FormGetField(strSubFormTag,"src_Scr");
		strSrcScr.MakeUpper();
		if(strSrcScr == strFormTag) {
		  EnableToolbar(strSubFormTag,TOOLBAR_DELETE,TRUE);
		} else {
		  EnableToolbar(strSubFormTag,TOOLBAR_DELETE,FALSE);
		}
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

long DrillEntryCategory::ShowDrillInquiry(CString strCriteria)
{
	m_nCurrentCard=ID_Drill_Inquiry;
	return ParentManage("HSE_DRLLPLNINQ",FALSE,FALSE,FALSE,"HSE_DRLLPLN_VW",strCriteria);
}


HRESULT DrillEntryCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	CString strTableName(pCustomButtonClicked->Table_Name);
	if(strButtonName=="DRILL_FINDINGS"){
		openDrillFindingScr(strTableName,strSubFormTag);
		//m_pCategory->GetHSEObj()->tryit();
	} else if (strButtonName == "DRILL_FINDINGS_UNPLNDSCNROS"){
		openDrillFindingUnplndScnrosScr(strTableName,strSubFormTag);
	}	
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void DrillEntryCategory::openDrillFindingScr(CString strTableName,CString strSubFormTag){
//	OpenPopUpScreen(strTableName,strSubFormTag,"PRMKY","HSE_AUDTPLNENTRY","HSE_AUDTPLNENTRY","Audit Plan Entry","","AUDTPLNENTRY_PLNN",ID_AUDITACTIONSENTRY);
//	return;
	CString strSQL ;	
	bool bLocked = false;
	CString strMainLinkFeild = FormGetField(strTableName,"PRMKY");	
	if(strMainLinkFeild == "")
		return;	
	setOwnerScreenTag(strSubFormTag);	
	CString strDefValues;
	strDefValues.Format("DRLFNDNGS_LNK|%s",strMainLinkFeild);	
	strSQL.Format("SELECT * FROM HSE_DRLFNDNGS WHERE DRLFNDNGS_LNK = '%s'",strMainLinkFeild);
	ShowScreen(0,"HSE_TGDRLFNDNGS","Drill Findings",NORMAL_MODE,false,strSQL,"",strDefValues,"",bLocked);
}

void DrillEntryCategory::openDrillFindingUnplndScnrosScr(CString strTableName,CString strSubFormTag){
	CString strSQL ;	
	bool bLocked = false;
	CString strMainLinkFeild = FormGetField(strTableName,"PRMKY");	
	if(strMainLinkFeild == "")
		return;
	setOwnerScreenTag(strSubFormTag);	
	CString strDefValues;
	strDefValues.Format("DrlFndngsUnPlndScnros_LNK|%s",strMainLinkFeild);	
	strSQL.Format("SELECT * FROM HSE_DrlFndngsUnPlndScnros WHERE DrlFndngsUnPlndScnros_LNK= '%s'",strMainLinkFeild);
	ShowScreen(0,"HSE_TGDRLFNDNGSUNPLNDSCNROS","Drill Findings",NORMAL_MODE,false,strSQL,"",strDefValues,"",bLocked);
}


