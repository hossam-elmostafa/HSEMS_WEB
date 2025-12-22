// PrformnceMsurmntChcklistCategory.cpp: implementation of the PrformnceMsurmntChcklistCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSEMS.h"
#include "PrformnceMsurmntChcklistCategory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PrformnceMsurmntChcklistCategory::PrformnceMsurmntChcklistCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory)
{
	m_pCategory = pCategory;
}

PrformnceMsurmntChcklistCategory::~PrformnceMsurmntChcklistCategory()
{

}


HRESULT PrformnceMsurmntChcklistCategory::DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked)
{
	//polydb_task nRecordStatus = 0
	//if(pCustomButtonClicked-> nRecordStatus != 0)
	//{
	//Action: Press details button
	//Response : Pop screen details will open
	//note : this requirement not listed in specs but mr sameer update it.
	m_strScreenTag = pCustomButtonClicked->Form_Tag;
	CString strBtnName(pCustomButtonClicked->Button_Name);
	CString strMainKeyFeild = FormGetField("HSE_PERFMSRMNTCHKLIST" ,"PERFMSRMNTCHKLIST_GRP");	
	int nNumberOfRecords = getNumberOfRecord("HSE_PERFMSRMNTCHKLISTSECND" ,strMainKeyFeild,"PERFMSRMNTCHKLISTSECND_LNK");	
	if(nNumberOfRecords != 0)
	{
		if(strBtnName=="PERFMSRMNTCHKLISTSECND_DTLS")
		{
			CString strKeyField=FormGetField("HSE_PERFMSRMNTCHKLISTSECND","MAINLINK");
			CString strSQL="Select DTL_LNK from HSE_DTL Where DTL_LNK = '"+strKeyField+"'";
			CString strKeyFieldFound=GetRecordSetData(strSQL,1);
			m_strScreenTag = pCustomButtonClicked->Form_Tag;
			CString strDefValues;
			//if(strKeyFieldFound.IsEmpty())
			//{
			m_strScreenTag = "HSE_TGDTL";					
			strDefValues.Format("DTL_LNK|%s",strKeyField);	
			strSQL.Format("SELECT * FROM HSE_DTL WHERE (DTL_LNK = '%s')",strKeyField);
			ShowScreen(0,"HSE_TGDTL","Details",NORMAL_MODE,true,strSQL,"",strDefValues,"",false);
			//Bug ID : 8819
			//}
			//else 
			//{
			//	m_strScreenTag = "HSE_TgDtl";
			//	CString strWhereCond="WHERE DTL_LNK= '"+strKeyField+"'";
			//	OpenScreen("Details",strWhereCond,ID_HSE_DTL,0,1);
			//}
			m_strScreenTag = pCustomButtonClicked->Form_Tag;
		}
	}
	//}
	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}



long PrformnceMsurmntChcklistCategory::ShowPerformanceMeasurementCheckList(CString strCriteria)
{
	m_nCurrentCard=ID_HSE_PERFMSRMNTCHKLIST;
	return ParentManage("HSE_TGPERFMSRMNTCHKLIST",TRUE,TRUE,TRUE,"HSE_PERFMSRMNTCHKLIST",strCriteria);
}