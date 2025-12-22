// ReasonsCategory.cpp: implementation of the CReasonsCategory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReasonsCategory.h"

#define	EXIT			11

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReasonsCategory::CReasonsCategory(CCategory *pCategory,IReasonCaller *pCaller):CHSEMSCommonCategory(pCategory)
{
	m_pCaller=pCaller;
	m_pCaller->getParameters(m_strOwnerScreenTag,m_CallerIdInitial,m_CallerIdFinal,m_strPrmKey,m_bEnableResults,m_strReasonsFilter,m_strCaption);
	if (m_strPrmKey=="")
		m_strPrmKey=m_CallerIdInitial;
	//m_bList=true;
	m_strScreenKey=m_strOwnerScreenTag.Mid(0,7);
}

CReasonsCategory::~CReasonsCategory()
{

}


UINT CReasonsCategory::ShowScreen(CString strScreenTag,CString strCriteria)
{	
	if(m_bEnableResults!=0) //Putting reasons
		strCriteria.Format("WHERE Rsns_RfRec='%s' AND RSNS_RFSCRN like '%s%%'",m_strPrmKey,m_strScreenKey);
	else// Viewing reasons
		strCriteria.Format("WHERE RfRec=%s AND RSNS_RFSCRN like '%s%%'",m_strPrmKey,m_strScreenKey);
	//strCriteria.Format("WHERE RfRec=%s ",m_strPrmKey); //Temporarly until adjusting DB
	CString strCancelInsertedRecords;	
	FormEnableButton(3,0,"CUSTOM_APPLY_REASONS",m_bEnableResults,true);
	m_pCategory->SetFastSearchCriteria("");
	UINT retval=ParentManage(strScreenTag,m_bEnableResults,m_bEnableResults,m_bEnableResults,"",strCriteria);
	return retval;
}


HRESULT CReasonsCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString Tab_tag = pScreenReady->SubForm_Tag;
	ScreenReady(Tab_tag);
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}

void CReasonsCategory::ScreenReady(LPCTSTR FormTag)
{
	m_pHSEMSPolyDb->FormChangeCaption(-1,m_strCaption);
	if(m_bEnableResults)
	{
		CString strFormTag=(CString)FormTag;
		short formTyp=m_pHSEMSPolyDb->GetFormType();
		m_bList=(formTyp==3);
		//if (strFormTag.Find("_LIST")==-1 )
		if(!m_bList) //not a List
		{
			CString strCallerID=FormGetField("CMS_RSNS","Rsns_RfRec");		
			if(strCallerID=="")
			{
				//m_bList=false;
				m_pHSEMSPolyDb->FieldSetAttribSingle("Rsns_RfScrn","",-1,"","","",m_strOwnerScreenTag,-1,"","","","","",-1,-1,"");
				m_pHSEMSPolyDb->FieldSetAttribSingle("Rsns_RfRec","",-1,"","","",m_CallerIdInitial,-1,"","","","","",-1,-1,"");
				m_pHSEMSPolyDb->FormDoAction(2,3);
				m_pHSEMSPolyDb->FormDoAction(2,1);
			}
		}
	}
}


void CReasonsCategory::ButtonClicked(LPCTSTR Caption, LPCTSTR Form_Tag, LPCTSTR Table_Name, LPCTSTR Master_Full_Recoed,LPCTSTR Details_Full_Recoe,LPCTSTR Field_Caption)
{
	CString strCaption=(CString)Caption;
	strCaption.MakeUpper();
	if(strCaption=="APPLY_REASONS" && m_bEnableResults)
	{
		//short formTyp=m_pHSEMSPolyDb->GetFormType();
		if(m_bList==true)
		//if(formTyp==3) // A list
		{
			CString strUpdateReason;
			strUpdateReason.Format("UPDATE CMS_Rsns SET Rsns_RfRec='%s' WHERE Rsns_RfRec='%s'",m_CallerIdFinal,m_CallerIdInitial);
			try
			{
				long lRecords=ExecuteSQL(strUpdateReason);
				if(lRecords>0)
				{
					m_pHSEMSPolyDb->FormDoAction(-1,EXIT);
					m_pHSEMSPolyDb->FormDoAction(-1,3);
				}
				else
				{
					AfxMessageBox("Can not Apply results on an empty list");
				}

			}
			catch(...)
			{
			}
		}
		else
		{
			AfxMessageBox("Applying results should be from the list view");
		}
	}
}


void CReasonsCategory::BeforeBrowseButtonClick(LPCTSTR Form_Tag,LPCTSTR Link_Tag, LPCTSTR FieldName, LPCTSTR CellContent, LPCTSTR BrowsingTable, LPCTSTR BrowsingSQL, LPCTSTR BrowsingField, LPCTSTR BrowsingRetField, LPCTSTR Full_Record)
{
	CString strFieldName=FieldName;
	if (strFieldName=="RSNS_RSN")
	{
		CString strBrowsingSQL=BrowsingSQL;
		if (strBrowsingSQL.Find("WHERE")!=-1)
		{
			strBrowsingSQL+=" AND " + m_strReasonsFilter;
		}
		else
		{
			strBrowsingSQL+=" WHERE " + m_strReasonsFilter;
		}

		m_pHSEMSPolyDb->FieldSetAttribSingle(FieldName,"",-1,"","","","",-1,"",strBrowsingSQL,"","","",-1,-1,"");
	}
}

void CReasonsCategory::OnScreenExit()
{
	//short formTyp=m_pHSEMSPolyDb->GetFormType();
	if(m_bList)
	//if(formTyp==3) // A list
	{
		CString strDeleteInitialRecordsIfAny;
		strDeleteInitialRecordsIfAny.Format("DELETE FROM CMS_Rsns WHERE Rsns_RfRec='%s'",m_CallerIdInitial);
		try
		{
			long lRecords=ExecuteSQL(strDeleteInitialRecordsIfAny);
			if(lRecords>0)
			{
				AfxMessageBox("Exitting without appying reasons");
				//m_pCMSPolyDb->FormDoAction(2, 2);
				//m_pCMSPolyDb->FormMakeRefresh(2);
			}
		}
		catch(...)
		{
		}
	}

}