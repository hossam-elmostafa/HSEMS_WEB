// HrmTyp.cpp: implementation of the HrmTyp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HrmTyp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HrmTyp::HrmTyp(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

HrmTyp::~HrmTyp()
{

}

HRESULT HrmTyp::DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick)
{
	CString strFieldName  = pBeforBrowsingButtonClick->Field_Name;	
	/*
	 * Linked Control tab removed
	if(strFieldName=="HRMTYPLNKCTRLMSRMNT_CTRLMSRMNT")
	{		
		CString strMsrGrp=FormGetField("HSE_HRMTYPLNKCTRLMSRMNT","HRMTYPLNKCTRLMSRMNT_GRP");								
		if(strMsrGrp.IsEmpty())
			strMsrGrp=" ";
		CString strSQL;
		strSQL.Format("WHERE (CTRLMSRMNTCTRLMSRMNT_LNK ='%s')",strMsrGrp);
		pBeforBrowsingButtonClick->pBrowsingInfo->BrowsingSQL= strSQL.AllocSysString(); 				
	}*/
	return CHSEMSCommonCategory::DisplayBeforBrowsingButtonClick(pBeforBrowsingButtonClick);
}

HRESULT HrmTyp::DisplayFieldChange (FIELDCHANGE *pFieldChange)
{
	/*
	 * Linked Control tab removed
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	if(strFieldName=="HRMTYPLNKCTRLMSRMNT_GRP")
		FormSetField("HSE_HRMTYPLNKCTRLMSRMNT" , "HRMTYPLNKCTRLMSRMNT_CTRLMSRMNT" , "");
	*/
	return CHSEMSCommonCategory::DisplayFieldChange (pFieldChange);
}

long HrmTyp::ShowHarmTypes(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_HRMTYP;
	return ParentManage("HSE_TGHRMTYP",TRUE,TRUE,TRUE,"HSE_HRMTYP",strCriteria);
}