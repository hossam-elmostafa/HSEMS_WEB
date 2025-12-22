#include "stdafx.h"
#include "HSEMS.h"
#include "EQInspctnCategory.h"
#include "HSEobj.h"

EQInspctnCategory::EQInspctnCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
}

EQInspctnCategory::~EQInspctnCategory(){}


long EQInspctnCategory::ShowEQInspctnRqust(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EQInspctnRqust;
	return ParentManage("HSE_EQINSPCTNRQUST",TRUE,TRUE,TRUE,"HSE_EQINSPCTN",strCriteria);	
}

long EQInspctnCategory::ShowEQInspctnRsult(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EQInspctnRsult;
	return ParentManage("HSE_EQINSPCTNRSULT",TRUE,FALSE,FALSE,"HSE_EQINSPCTN",strCriteria);	
}

long EQInspctnCategory::ShowEQInspctnWrk(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EQInspctnWrk;
	return ParentManage("HSE_EQINSPCTNWRK",TRUE,FALSE,FALSE,"HSE_EQINSPCTN",strCriteria);	
}

long EQInspctnCategory::ShowEQInspctnInqry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_EQInspctnInqry;
	return ParentManage("HSE_EQINSPCTNINQRY",TRUE,FALSE,FALSE,"HSE_EQINSPCTN",strCriteria);	
}

HRESULT EQInspctnCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1)
	{	
		if(strSubFormTag == "") {
			if (strBtnName=="SAVE") {
				RefreshScreen("",REFRESH_SELECTED);
			}
			else if(strBtnName=="NEW") {			
				CString strCurrentYear = FormGetField("HSE_EQINSPCTN","EQINSPCTN_INSPCTNYR");
				FormSetField("HSE_EQINSPCTN","EQINSPCTN_INSPCTNN",getCurrentEQInspctnNo(strCurrentYear));						
			}
		}
	} else {																		//iComplete == 0 
		if(strSubFormTag == "") {
			if (strBtnName=="SAVE") {				
				CString strSQL;
				CString strEqTyp	= FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQTYP");				
				CString strEqSrlNum = FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQSRLN");				
				CString strEqPltNum = FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQPLTN");				
				if(strEqPltNum != "") {
					strSQL.Format("SELECT COUNT(0) AS CNT FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '%s') AND (EQINSPCTN_EQSRLN = '%s') AND (EQINSPCTN_EQPLTN = '%s') AND (EQInspctn_InspctnSttus < 5)",strEqTyp,strEqSrlNum,strEqPltNum);									
				} else {
					strSQL.Format("SELECT COUNT(0) AS CNT FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '%s') AND (EQINSPCTN_EQSRLN = '%s') AND (EQINSPCTN_EQPLTN IS NULL) AND (EQInspctn_InspctnSttus < 5)",strEqTyp,strEqSrlNum);				
				}							
				int EqCount;
				CPolyDbRecordset PolyDbRecordset;
				PolyDbRecordset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
				if(!PolyDbRecordset.IsEOF())
				{
					PolyDbRecordset.GetFieldValue("CNT",EqCount);
					if(EqCount>0)
					{						
						CString strMsg="This Equipment entered before!";
						AfxMessageBox(strMsg,MB_ICONINFORMATION|MB_TASKMODAL);
					}
				}
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}


HRESULT EQInspctnCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{		
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFldName(pFieldChange->Field_Name);
	CString strNewValue = pFieldChange->New_Value;	
	if(strFldName == "EQINSPCTN_EQTYP") {
		int nRltdInspctnChcklstCount = getRltdInspctnChcklstCount(strNewValue);	
		if(nRltdInspctnChcklstCount == 0) {
			ShowMessageEx(IDS_INSPCTN_RQST_EMPTY_CHKLST);
		}
	//Moved from FirstSysDB
	//[EQInspctn_InspctnChcklst_DfctFund = 'N']:= Y <ELSE> [EQInspctn_InspctnChcklst_DfctFund = 'Y' AND EQInspctn_InspctnChcklst_ActnDn = 'N']:=N <ELSE> [EQInspctn_InspctnChcklst_DfctFund = 'Y' AND EQInspctn_InspctnChcklst_ActnDn = 'Y']:=Y
	} else if (strFldName == "EQINSPCTN_INSPCTNCHCKLST_DFCTFUND") {
		if(strNewValue == 'N') {
			FormSetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD","Y");
		} else {
			CString strActnDone = FormGetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_ACTNDN");
			if(strActnDone == 'Y') {
				FormSetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD","Y");
			} else {
				FormSetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD","N");
			}
		}	
	} else if (strFldName == "EQINSPCTN_INSPCTNCHCKLST_ACTNDN") {
		if(strNewValue == 'Y') {
			FormSetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD","Y");
		} else {
			FormSetField("HSE_EQINSPCTN_INSPCTNCHCKLST","EQINSPCTN_INSPCTNCHCKLST_INSPCTNACCPTD","N");
		}
	} else if (strFldName == "EQINSPCTN_EQENTRYDT" || strFldName == "EQINSPCTN_EQWRKNGDYS" || strFldName == "EQINSPCTN_EQWRKNGHURS") {		
		COleDateTime dtDate;
		ParseDateTime(&dtDate,FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQENTRYDT"));	
		int nDays  = atoi(FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQWRKNGDYS"));
		int nHours = atoi(FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQWRKNGHURS"));
		dtDate+=COleDateTimeSpan( nDays, nHours, 0, 0 );				
		FormSetField("HSE_EQINSPCTN","EQINSPCTN_EXPCTDEXTDT",dtDate.Format());
	}
	//TODO : this code must be reviewed , only defect during work must be handeled the problem is I can't get tab tag in edit mode in tabs.
	//if (strSubFormTag == "HSE_EQINSPCTNWRK_DFCTSDURNGWRK") {			
		CString strLineNo = FormGetField("HSE_EQINSPCTN_DFCTSDURNGWRK","EQINSPCTN_DFCTSDURNGWRK_LINENO");
		if(strLineNo == "") {
			CString strMainLink = FormGetField("HSE_EQINSPCTN","EQINSPCTN_PRMKY");
			CString strCurrentEQInspctnDfctNo = getCurrentEQInspctnDfctNo(strMainLink);
			FormSetField("HSE_EQINSPCTN_DFCTSDURNGWRK","EQINSPCTN_DFCTSDURNGWRK_LINENO",strCurrentEQInspctnDfctNo);
		}			
	//}	
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}



HRESULT EQInspctnCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "") {
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	}
	CString strPrmKy = FormGetField("HSE_EQINSPCTN","EQINSPCTN_PRMKY");
	if(strButtonName=="EQINSPCTN_ENTRYCMPLTD")
	{
		completeEQInspctn(strPrmKy);		
	} else if (strButtonName == "INSPCTNCHCKLST_RELOADCHCKLST") {
		int nRet=0;
		nRet = MessageBox(NULL,"Are you sure to re-load the checklist?","Prompt",MB_YESNO | MB_ICONEXCLAMATION |MB_TASKMODAL);
		if(nRet == IDYES) {
			reldEQInspctnChckLst(strPrmKy);
		}
	} else if (strButtonName == "EQINSPCTN_DATAENTRYRJCTD") {
		rjctEQInspctnDataEntry(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQINSPCTNRJCTD") {
		rjctEQInspctn(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQHOLD") {
		holdEQInspctn(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQACPTD") {
		accptEQInspctn(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQSTPDDFCTD") {
		stopDefectedEQ(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQRSMWRK") {
		resumeEQWrk(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQEXTDFCTS") {
		exitDefectedEQ(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQEXTWRKCNCLD") {
		canclEQWrk(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_EQEXTWRKCMPLTD") {
		cmpltEQWrk(strPrmKy);
	} else if (strButtonName == "EQINSPCTN_VWEQUNDRENTRY") {
	} else if (strButtonName == "EQINSPCTN_VWRJCTDEQ") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS<5)");
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=8)");
	} else if (strButtonName == "EQINSPCTN_VWONHOLDEQ") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=9)");
	} else if (strButtonName == "EQINSPCTN_VWWRKEQ") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=10)");
	} else if (strButtonName == "EQINSPCTN_VWMNTHSTPDEQDFCT") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=11) AND MONTH(EQInspctn_EQEntryDT) = MONTH(GETDATE())");
	} else if (strButtonName == "EQINSPCTN_VWMNTHEXTEQDFCT") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=18) AND MONTH(EQInspctn_EQEntryDT) = MONTH(GETDATE())");
	} else if (strButtonName == "EQINSPCTN_VWMNTHEXTEQJOBCNCLD") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=19) AND MONTH(EQInspctn_EQEntryDT) = MONTH(GETDATE())");
	} else if (strButtonName == "EQINSPCTN_VWMNTHEXTEQJOBCMPLTD") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","WHERE (EQINSPCTN_INSPCTNSTTUS=20) AND MONTH(EQInspctn_EQEntryDT) = MONTH(GETDATE())");
	} else if (strButtonName == "EQINSPCTN_VWALL") {
		ChangeCriteria("HSE_EQINSPCTNINQRY","","");
	} else if (strButtonName == "EQINSPCTN_VWINSPCTNRQSTHSTRY") {
		viewInspctnRqstHstry();
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

CString EQInspctnCategory::getCurrentEQInspctnNo(CString strCurrentYear) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(EQINSPCTN_INSPCTNN)+1,1) FROM HSE_EQINSPCTN WHERE EQINSPCTN_INSPCTNYR = '%s'",strCurrentYear);
	CString strCurrentMsrNo = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCurrentMsrNo;	
}

CString EQInspctnCategory::getCurrentEQInspctnDfctNo(CString strMainLink) {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(EQINSPCTN_DFCTSDURNGWRK_LINENO)+1,1) FROM HSE_EQINSPCTN_DFCTSDURNGWRK WHERE MAINLINK = '%s'",strMainLink);
	CString strCurrentMsrNo = GetRecordSetData(strSQL).GetBuffer(0);	
	return strCurrentMsrNo;	
}

void EQInspctnCategory::completeEQInspctn(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE completeEQInspctn '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::reldEQInspctnChckLst(CString strPrmKy)
{		
	CString strSQL;	
	//RQ-3-2021.9
	strSQL.Format("EXECUTE reldEQInspctnChckLst '%s','%s'",strPrmKy,"Checklist is re-loaded");
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::rjctEQInspctnDataEntry(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE rjctEQInspctnDataEntry '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::rjctEQInspctn(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE rjctEQInspctn '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::holdEQInspctn(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE holdEQInspctn '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::accptEQInspctn(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE AcceptEQInspctn '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::stopDefectedEQ(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE stopDefectedEQ '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::resumeEQWrk(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE resumeEQWrk '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::exitDefectedEQ(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE exitDefectedEQ '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::canclEQWrk(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE canclEQWrk '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void EQInspctnCategory::cmpltEQWrk(CString strPrmKy)
{		
	CString strSQL;		
	strSQL.Format("EXECUTE cmpltEQWrk '%s'",strPrmKy);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

int EQInspctnCategory::getRltdInspctnChcklstCount(CString strNewValue) {	
	CString strSQL;
	strSQL.Format("SELECT ISNULL(COUNT(0),0) AS CHECK_LIST_COUNT FROM HSE_EQTYPS_INSPCTNCHCKLST WHERE MAINLINK = '%s'",strNewValue);		
	return atoi(GetRecordSetData(strSQL,1));
}

//HSE Comments 26-08-2020
//Add custom-button to view inspection request history records for the same EQ (type, serial, plate) 
void EQInspctnCategory::viewInspctnRqstHstry()
{
	CString strSQL,strDefValues;		
	CString strEqTyp	= FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQTYP");				
	CString strEqSrlNum = FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQSRLN");				
	CString strEqPltNum = FormGetField("HSE_EQINSPCTN","EQINSPCTN_EQPLTN");		
	

	if(strEqPltNum != "") {
		strSQL.Format("SELECT * FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '%s') AND (EQINSPCTN_EQSRLN = '%s') AND (EQINSPCTN_EQPLTN = '%s') AND (EQINSPCTN_INSPCTNSTTUS >= 5)",strEqTyp,strEqSrlNum,strEqPltNum);				
		strDefValues.Format("EQINSPCTN_EQTYP|%s|EQINSPCTN_EQSRLN|%s|EQINSPCTN_EQPLTN|%s",strEqTyp,strEqSrlNum,strEqPltNum);
	} else {
		strSQL.Format("SELECT * FROM HSE_EQINSPCTN WHERE (EQINSPCTN_EQTYP = '%s') AND (EQINSPCTN_EQSRLN = '%s') AND (EQINSPCTN_EQPLTN IS NULL) AND (EQINSPCTN_INSPCTNSTTUS >= 5)",strEqTyp,strEqSrlNum);				
		strDefValues.Format("EQINSPCTN_EQTYP|%s|EQINSPCTN_EQSRLN|%s",strEqTyp,strEqSrlNum);
	}	
	strDefValues.Format("EQINSPCTN_EQTYP|%s|EQINSPCTN_EQSRLN|%s|EQINSPCTN_EQPLTN|%s",strEqTyp,strEqSrlNum,strEqPltNum);	
	ShowScreen(0,"HSE_EQINSPCTNINQRY","Inspection Request History",NORMAL_MODE,false,strSQL,"",strDefValues,"",false);	
}



