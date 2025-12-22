#include "stdafx.h"
#include "HSEMS.h"
#include "WstRcvngEntCategory.h"
#include "HSEobj.h"

WstRcvngEntCategory::WstRcvngEntCategory(CCategory *pCategory):WstMngmntCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_WSTRCVNGENTRY";
	m_ScreenTgName="HSE_TGWSTRCVNGENTRY";
}

WstRcvngEntCategory::~WstRcvngEntCategory(){}

//Show Screen
long WstRcvngEntCategory::ShowWstRcvngEntry(CString strCriteria)
{	
	m_nCurrentCard=ID_HSE_WstRcvngEntry;
	return ParentManage("HSE_TGWSTRCVNGENTRY",TRUE,TRUE,TRUE,"HSE_WSTRCVNGENTRY",strCriteria);	
}

//Display Field Change
HRESULT WstRcvngEntCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{	
	CString strFldName = pFieldChange->Field_Name;	
	if(strFldName == "WSTRCVNGENTRY_TRNSTZN" || strFldName == "WSTRCVNGENTRY_ITMCOD") {
		CString strZoneCode = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_TRNSTZN");
		CString strItemCode = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_ITMCOD");
		if(strZoneCode != "" && strItemCode != "") {
			FormSetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_QOHBFR",getQuantityBefore(strZoneCode,strItemCode));
		}
	} 	
	else if (strFldName == "WSTRCVNGENTRY_RCVDQTY"){
		CString strItemKey = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_ITMCOD");
		//a.ali,30-6-2020,Ref : Urgent - HSEMS Discovered BUG's	Wed 24 Jun 
		CString strQONBefore = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_QOHBFR");
		CString strRcvdQuantity = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_RCVDQTY");				
		//CString strConvrtdTo = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_CNVRTDTO");
		//CString strItemCode = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_ITMCOD");		
		//double dCovrtRat = getConvertRate(strConvrtdTo,strItemCode);		
		double dQONBefore = atof(strQONBefore);
		double dRcvdQuantity = atof(strRcvdQuantity);

		/**
		 * HSEMS New requirements_2_11_2020
		 * Waste Receiving Entry: 
		 *	Allow (received QTY = 0) when the (waste item, check QOH) = No : 
		 *	If selected (waste item, check QOH) = Yes then entered (received QTY) should > 0 .. as current 
		 *	If selected (waste item, check QOH) = No then entered (received QTY) should >= 0
		 * 
		 *	According to Eng Hossam update Item with Check QOH = NO then don't show element in browsing`
		 *
		if(isItemRqrChkQOH(strItemKey)){			
			if(dRcvdQuantity <= 10) {
				//AfxMessageBox("Received QTY must be > 0");
				//*pFieldChange->iCancle = 1;
				FormSetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_RCVDQTY","");				
				//return S_FALSE;
			}				
		}*/


		//double dQONAfter = dQONBefore + (dRcvdQuantity*dCovrtRat);
		double dQONAfter = dQONBefore + dRcvdQuantity;
		CString strQONAfter,strRcvdQnty;
		strQONAfter.Format("%5.3f",dQONAfter);
		//strRcvdQnty.Format("%5.3f",dRcvdQuantity*dCovrtRat);
		FormSetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_QOHAFTR",strQONAfter);
		//FormSetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_CNVRTDQTY",strRcvdQnty);
	} 
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

HRESULT WstRcvngEntCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(pToolBarButtonClicked->iComplete==0)
	{	
		if(strBtnName=="DELETE") {
			CString strRecStatus = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_ENTRYSTTS");
			int nRecStatus = atoi(strRecStatus);
			if(nRecStatus >= 5){
				EnableToolbar("",TOOLBAR_DELETE,FALSE);
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT WstRcvngEntCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="WSTRCVNGENTRY_VWCMPLTDENTRS")
	{
		ChangeCriteria("HSE_TGWSTRCVNGENTRY","","WHERE (WSTRCVNGENTRY_ENTRYSTTS=5)");
		LockRecord("","",true);
	} else if(strButtonName=="WSTRCVNGENTRY_RTRNTOENTRY") {
		CString strPrmryKy = FormGetField("HSE_WSTRCVNGENTRY","WstRcvngEntry_PrmKy");
		returnWasteRcvEntry(strPrmryKy);
	} else if(strButtonName=="WSTRCVNGENTRY_ENTRYCMPLTD") {	
		CString strPrmryKy = FormGetField("HSE_WSTRCVNGENTRY","WstRcvngEntry_PrmKy");
		completeWasteRcvEntry(strPrmryKy);		
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

//Get quantity before recieving
/*CString WstRcvngEntCategory::getQuantityBefore(CString strZoneCode,CString strItemCode) {
	CString strSQL;
	strSQL.Format("SELECT WSTZNSALWDITMS_QOH FROM HSE_WSTZNSALWDITMS WHERE WSTZNSALWDITMS_ITMCOD = '%s' AND WSTZNSALWDITMS_LNK = '%s'",strItemCode,strZoneCode);
	CString strQuantity = GetRecordSetData(strSQL).GetBuffer(0);	
	return strQuantity;
}*/

//Get convert rate
/*double WstRcvngEntCategory::getConvertRate(CString strConvrtdTo,CString strItm) {
	CString strSQL,strConvTo;
	if(strConvrtdTo == "TON") {
		strConvTo = "UOM_CNVRTTOTN";
	} else {
		strConvTo = "UOM_CNVRTTOM3";
	}
	strSQL.Format("SELECT %s FROM HSE_UOM WHERE UOM_COD = (SELECT WSTITMS_UOM FROM HSE_WSTITMS WHERE WSTITMS_ITMCD = '%s')",strConvTo,strItm);
	CString strConvRat = GetRecordSetData(strSQL).GetBuffer(0);
	double nConvRat = atof(strConvRat);
	return nConvRat;
}*/

void WstRcvngEntCategory::completeWasteRcvEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE Complete_Waste_Entry '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}

void WstRcvngEntCategory::returnWasteRcvEntry(CString strPrmryKey) 
{
	CString strSQL;	
	strSQL.Format("EXECUTE Return_compltd_waste_to_entry '%s'",strPrmryKey);
	ExecuteSQL(strSQL);
	RefreshScreen("",REFRESH_SELECTED);
}




