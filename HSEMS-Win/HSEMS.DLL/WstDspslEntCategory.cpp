#include "stdafx.h"
#include "HSEMS.h"
#include "WstDspslEntCategory.h"
#include "HSEobj.h"

WstDspslEntCategory::WstDspslEntCategory(CCategory *pCategory):WstMngmntCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_WstDspslEntry";
	m_ScreenTgName="HSE_TgWstDspslEntry";
}

WstDspslEntCategory::~WstDspslEntCategory(){}

//Show Screen
long WstDspslEntCategory::ShowWstDspslEntry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_WstRcvngEntry;
	return ParentManage("HSE_TGWSTDSPSLENTRY",TRUE,TRUE,TRUE,"HSE_WSTDSPSLENTRY",strCriteria);	
}

//Display Field Change
HRESULT WstDspslEntCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strFldName = pFieldChange->Field_Name;			
	//	UPDATE : HSE Comments 26-08-2020
	//	Used to enter multiple disposal items, 
	//	Move the (item info) fields & (item QTY) fields from main screen, to this new tab, with same calc & update mechanizm
	//	The (item code) would be the primary key.				
	if(strFldName == "WSTDSPSLENTRY_TRNSTZN" || strFldName == "WSTDSPSLENTRYDSPSLITMS_ITMCD") {		
		CString strDirectDisposal = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_DRCTDSPSL");
		if(strDirectDisposal == 'N') {
			CString strZoneCode = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_TRNSTZN");
			CString strItemCode = FormGetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_ITMCD");
			if(isItemRqrChkQOH(strItemCode)){
				if(strZoneCode != "" && strItemCode != "") {
					FormSetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_QOHBFR",getQuantityBefore(strZoneCode,strItemCode));
				}
			} else {
				FormSetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_QOHBFR","0");
				FormSetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_QOHAFTR","0");
			}
		}
	}				
	else if (strFldName == "WSTDSPSLENTRYDSPSLITMS_DSPSLQTY"){															    
		CString strQONBefore = FormGetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_QOHBFR");
		CString strDspslQTY  = FormGetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_DSPSLQTY");					
		CString strItemCode = FormGetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_ITMCD");
				
		double dQONBefore = atof(strQONBefore);
		double dDspslQTY = atof(strDspslQTY);	
		if(isItemRqrChkQOH(strItemCode)){
			if(dQONBefore < dDspslQTY) {
				ShowMessageEx(IDS_WST_QTY_NOT_VALID,"",IV_NOICON,"Error");
				FormSetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_DSPSLQTY","");
			} else {
				double dQONAfter = dQONBefore - dDspslQTY;					
				CString strQONAfter,strRcvdQnty;
				strQONAfter.Format("%5.3f",dQONAfter);
				strDspslQTY.Format("%5.3f",dDspslQTY);
				FormSetField("HSE_WSTDSPSLENTRYDSPSLITMS","WSTDSPSLENTRYDSPSLITMS_QOHAFTR",strQONAfter);
			}		
		}
	}			
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT WstDspslEntCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==1)
	{								
		if (strBtnName=="NEW") {
			if(strSubFormTag == "") {			//Main Screen
				CString strManifestFrmNo = getManifestFrmNo();
				FormSetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_MNFSTFRMNO",strManifestFrmNo);
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT WstDspslEntCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{	
	CString strRecStatus = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_ENTRYSTTS");
	int nRecStatus = atoi(strRecStatus);
	if(nRecStatus >= 5){
		EnableToolbar("",TOOLBAR_DELETE,FALSE);
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);	
}

HRESULT WstDspslEntCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="WSTDSPSLENTRY_VWCMPLTDENTRS")
	{
		ChangeCriteria("HSE_TGWSTDSPSLENTRY","","WHERE (WSTDSPSLENTRY_ENTRYSTTS=5)");
		LockRecord("","",true);
		
	} else if(strButtonName=="WSTDSPSLENTRY_RTRNTOENTRY") {
		CString strPrmryKy = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_PRMKY");
		returnWasteDspslEntry(strPrmryKy);
	} else if(strButtonName=="WSTDSPSLENTRY_ENTRYCMPLTD") {	
		CString strPrmryKy = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_PRMKY");
		completeDspslEntry(strPrmryKy);		
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

void WstDspslEntCategory::completeDspslEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE Complete_Waste_Dspsl_Entry '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void WstDspslEntCategory::returnWasteDspslEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE Return_compltd_waste_Dspsl_to_entry '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

CString WstDspslEntCategory::getManifestFrmNo() {
	CString strSQL;
	strSQL.Format("SELECT ISNULL(MAX(WSTDSPSLENTRY_MNFSTFRMNO)+1,1) AS MnfstRrmNo FROM HSE_WSTDSPSLENTRY");
	CString strMnfstRrmNo = GetRecordSetData(strSQL).GetBuffer(0);	
	return strMnfstRrmNo;	
}
