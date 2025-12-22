// HSEToolEquipDistrib.cpp: implementation of the CHSEToolEquipDistrib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "HSEToolEquipDistrib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHSEToolEquipDistrib::CHSEToolEquipDistrib(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

CHSEToolEquipDistrib::~CHSEToolEquipDistrib()
{

}

HRESULT CHSEToolEquipDistrib::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick) {
		
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;
	
	//a.ali 3-11-2013
	//(Item Name) combo, browsing list of (item name) from "HSE Tools & Equipments, items" belonging to selected "Group Name"
	if(strFieldName == "TOOLSEQUIPDSTRBTNITEMS_ITMNAM") {
		//CString strLink = FormGetField("HSE_TOOLSEQUIPDSTRBTNITEMS","TOOLSEQUIPDSTRBTNITEMS_LNK");
		CString strLink = FormGetField("HSE_TOOLSEQUIPDSTRBTN","TOOLSEQUIPDSTRBTN_GRPNAM");
		if(strLink.IsEmpty()) {
			strLink = " ";				
		}
		CString strSQL;
		strSQL.Format("WHERE (TOOLEQUIPITEMS_LNK = '%s')",strLink);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}

	//a.ali 12-07-2017
	//Filter Site according to company
	if(strFieldName == "TOOLSEQUIPDSTRBTNITEMS_SIT") {
		CString strCmpnyNam = FormGetField("HSE_TOOLSEQUIPDSTRBTN","TOOLSEQUIPDSTRBTN_CMPNYNAM");
		if(strCmpnyNam.IsEmpty()) {
			strCmpnyNam = " ";
		}		
		CString strSQL;
		strSQL.Format("WHERE (STE_CMPNYNAME ='%s')",strCmpnyNam);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}

	//a.ali 4-11-2012
	//Filter location according to site selection
	if(strFieldName == "TOOLSEQUIPDSTRBTNITEMS_LOCTN") {
		CString strSite = FormGetField("HSE_TOOLSEQUIPDSTRBTNITEMS","TOOLSEQUIPDSTRBTNITEMS_SIT");
		if(strSite.IsEmpty()) {
			strSite = " ";
		}		
		CString strSQL;
		strSQL.Format("WHERE (WRKLOC_STE ='%s')",strSite);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString();		
	}

	


	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);	
}

long CHSEToolEquipDistrib::ShowHSEToolsEquipmentsDistribution(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_TOOLSEQUIPDSTRBTN;
	return ParentManage("HSE_TGTOOLSEQUIPDSTRBTN",TRUE,TRUE,TRUE,"HSE_TOOLSEQUIPDSTRBTN",strCriteria);
}