// PrsonlProtctveEquip.cpp: implementation of the PrsonlProtctveEquip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrsonlProtctveEquip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrsonlProtctveEquip::PrsonlProtctveEquip(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}


PrsonlProtctveEquip::~PrsonlProtctveEquip()
{

}
HRESULT PrsonlProtctveEquip::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick) {
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	//a.ali 3-11-2013
	//In the (Protective Equipment) combo list, exclude selected records for same "accident No."
	if(strFieldName == "PRSNLPRTCTVEQPMNT_PRTCTVEQPMNT") {
		CString strLink = FormGetField("HSE_PrsnlPrtctvEqpmnt","PRSNLPRTCTVEQPMNT_LINK");
		if(strLink.IsEmpty()) {
			strLink = " ";
		}
		CString strSQL;
		strSQL.Format("WHERE (PRSNLPRTCTVEQ_COD NOT IN (SELECT PRSNLPRTCTVEQPMNT_PRTCTVEQPMNT FROM HSE_PRSNLPRTCTVEQPMNT WHERE PRSNLPRTCTVEQPMNT_LINK = '%s'))",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}		
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);	
}

long PrsonlProtctveEquip::ShowPersonalProtectiveEquipmentpopUp(CString strCriteria)
{
	m_nCurrentCard=ID_SYSTEMSETTINGS_PERSONALPROTECTIVEEQUIPMENT;
	return ParentManage("HSE_TGPRSNLPRTCTVEQPMNT",TRUE,TRUE,TRUE,"HSE_PRSNLPRTCTVEQ",strCriteria);	
}

long PrsonlProtctveEquip::ShowPersonalProtectiveEquipmentScreen(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PRSNLPRTCTVE_POP_UP;
	return ParentManage("HSE_TGPRSNLPRTCTVEQ",TRUE,TRUE,TRUE,"HSE_PRSNLPRTCTVEQ",strCriteria);
}