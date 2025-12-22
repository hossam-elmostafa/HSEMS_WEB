#include "stdafx.h"
#include "HSEMS.h"
#include "WstMngmntCommonCategory.h"
#include "HSEobj.h"

WstMngmntCommonCategory::WstMngmntCommonCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
}

WstMngmntCommonCategory::~WstMngmntCommonCategory(){}

HRESULT WstMngmntCommonCategory::DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion)
{
	CString strFormTag(pRecordRepostion->Form_Tag);
	CString strRecStatus;
	if(strFormTag == "HSE_TGWSTRCVNGENTRY") {
		strRecStatus = FormGetField("HSE_WSTRCVNGENTRY","WSTRCVNGENTRY_ENTRYSTTS");
	} else if (strFormTag == "HSE_TGWSTDSPSLENTRY") {
		strRecStatus = FormGetField("HSE_WSTDSPSLENTRY","WSTDSPSLENTRY_ENTRYSTTS");
	}
	if(pRecordRepostion->iComplete==1)
	{		 
		if(strRecStatus=="5")
		{			
			EnableToolbar("",TOOLBAR_DELETE,FALSE);		
			EnableToolbar("",TOOLBAR_NEW,FALSE);
			EnableToolbar("",TOOLBAR_SAVE,FALSE);
			LockRecord("","",true);
		}		
	}
	return CHSEMSCommonCategory::DisplayRecordRepostion(pRecordRepostion);
}

//Get quantity before recieving
CString WstMngmntCommonCategory::getQuantityBefore(CString strZoneCode,CString strItemCode) {
	CString strSQL;
	strSQL.Format("SELECT WSTZNSALWDITMS_QOH FROM HSE_WSTZNSALWDITMS WHERE WSTZNSALWDITMS_ITMCOD = '%s' AND WSTZNSALWDITMS_LNK = '%s'",strItemCode,strZoneCode);
	CString strQuantity = GetRecordSetData(strSQL).GetBuffer(0);	
	return strQuantity;	
}

//Get convert rate
double WstMngmntCommonCategory::getConvertRate(CString strConvrtdTo,CString strItm) {
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
}

