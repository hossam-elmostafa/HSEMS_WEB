#include "stdafx.h"
#include "HSEMS.h"
#include "ChemicalRegisterCategory.h"
#include "HSEobj.h"

ChemicalRegisterCategory::ChemicalRegisterCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
	m_FSATableName="HSE_CHMCLRGSTR";
}

ChemicalRegisterCategory::~ChemicalRegisterCategory(){}

HRESULT ChemicalRegisterCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	EnableToolbar("",TOOLBAR_DELETE,FALSE);		
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

HRESULT ChemicalRegisterCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);
	if(strBtnName=="SAVE")
	{
		if(pToolBarButtonClicked->iComplete==1) 
		{
/*			AfxMessageBox("1");
			FormMakeRefresh(2);
			AfxMessageBox("2");
			return 0;
*/
			if(strSubFormTag=="") {
				FormSetField("HSE_CHMCLRGSTR","CHMCLRGSTR_LSTMDFYDT",GetFullDateTime("%d/%m/%Y"));
				FormSetField("HSE_CHMCLRGSTR","CHMCLRGSTR_LSTMDFYBY",GetUserLogin());
			} 
			//else if(strSubFormTag=="HSE_CHMCLRGSTR_CHNGHSTRY") {
			//	RefreshScreen("",REFRESH_SELECTED);
			//}
		}
	} else if(strBtnName=="NEW") {
		if(strSubFormTag=="HSE_CHMCLRGSTR_CHNGHSTRY") {
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_PURPS",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_PURPS"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_SGNLWRD",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_SGNLWRD"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_HZRDSTTMNT",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_HZRDSTTMNT"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_PRCUTNSTTMNT",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_PRCUTNSTTMNT"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_RLTDSUPPLR",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_RLTDSUPPLR"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_WST",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_WST"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_QTY",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_QTY"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_UOM",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_UOM"));
			FormSetField(strSubFormTag,"CHMCLRGSTR_CHNGHSTRY_FRQUNCY",FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_FRQUNCY"));
		}
	} else if(strBtnName=="SAVETAB"){
		if(pToolBarButtonClicked->iComplete==1) {
			if(strSubFormTag=="HSE_CHMCLRGSTR_CHNGHSTRY") {
				FormMakeRefresh(2);
			}
			else if(strSubFormTag=="HSE_CHMCLRGSTR_QTYCRD") {
				//UpdateQuantities();
			}
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

HRESULT ChemicalRegisterCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);
	int Count = 0;
	if(strSubFormTag == "")
		Count = pCustomButtonClicked->pMultiSelectedRows->lCount;
	if(strButtonName=="VIEW_ALL_ACTIVE_ITEMS"){
		ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'Y')");
	} else if(strButtonName=="VIEW_ALL_IN-ACTIVE_ITEMS") {
		ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_Actv = 'N')");
	} else if(strButtonName=="VIEW_DEPARTMENT_ITEMS") {
		CString strDprtmnt = FormGetField("HSE_CHMCLRGSTR","ChmclRgstr_DprtmntNm");
		ChangeCriteria("HSE_CHMCLRGSTR","","WHERE (ChmclRgstr_DprtmntNm = '"+strDprtmnt+"')");
	}
	else if(strSubFormTag=="HSE_CHMCLRGSTR_QTYCRD" && strButtonName=="UPDT")
	{
		UpdateQuantities();
	}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}

HRESULT ChemicalRegisterCategory::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	CString strFieldName(pFieldChange->Field_Name);	
	strFieldName.MakeUpper();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//RQ-8-2021.16
	//Hossam 15/9/2021
	if(strFieldName.Find("CHMCLRGSTR_QTYCRD")>=0)
	{
		CString strLineNo = FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QTYCRD_SrlN");
		if(strLineNo == "") {
			getNewSerialNoForATab("HSE_CHMCLRGSTR","ChmclRgstr_PrmryKy","HSE_CHMCLRGSTR_QTYCRD","MainLink","CHMCLRGSTR_QTYCRD_SrlN");
			
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}


void ChemicalRegisterCategory::UpdateQuantities()
{
	/*
		RQ-8-2021.16
		* When saving the record, we need to do the following updates :
			 - If (TXN Type = 1) then SET (QOH After) = (QOH Before) + (TXN QTY) & SET (main > QOH) = (main > QOH) + (TXN QTY)
			 - If (TXN Type > 1) then SET (QOH After) = (QOH Before) - (TXN QTY) & SET (main > QOH) = (main > QOH) - (TXN QTY)"""
		By Hossam 15/9/2021
	*/
	CString strQtyBfr=FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QTYCRD_QHBFR");
	if(strQtyBfr=="")
	{
		DoToolBarAction(TOOLBAR_SAVE,"HSE_CHMCLRGSTR","HSE_CHMCLRGSTR_QTYCRD");
		CString strMasterPK=FormGetField("HSE_CHMCLRGSTR","CHMCLRGSTR_prmryKy");
		CString strTxnType=FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QTYCRD_TXNTYP");
		CString strDtlSrial=FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QtyCrd_SrlN");
		CString strTxnQty=FormGetField("HSE_CHMCLRGSTR_QTYCRD","CHMCLRGSTR_QtyCrd_TxnQty");
		CString strSQL;
		strSQL.Format("exec Chemical_Register_Update_Qty '%s',%s,%s,%d",strMasterPK,strDtlSrial,strTxnQty,strTxnType=="1");
		if(ExecuteSQL(strSQL)<0)	
		{
			SubMakeRefresh();
			m_pHSEMSPolyDb->FormMakeRefresh(-1);
		}
	}
	else
		AfxMessageBox("This record was already used to update");
}



