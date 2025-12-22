// SitSrvyRqrdActn.cpp: implementation of the SitSrvyRqrdActn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SitSrvyRqrdActn.h"
#include "HSEMSApp.h"
#include "HSEMS.h"
#include "HSEobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SitSrvyRqrdActn::SitSrvyRqrdActn(CCategory *pCategory):SitSrvyCategory(pCategory) {
	m_pCategory = pCategory;
}


SitSrvyRqrdActn::~SitSrvyRqrdActn()
{

}

HRESULT SitSrvyRqrdActn::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strScreenTag(pScreenReady->Form_Tag);	
	BOOL bDisableCstmBtns = m_pCategory->GetHSEObj()->getDisableSitSrvyRqrdActnBtn();	
	if(bDisableCstmBtns == TRUE)
	{
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SITSRVYENTRQRDACTN_GENERATECAR","visible","false");	
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SitSrvyEntRQRDACTN_ViewRelatedCAR","visible","false");	
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SitSrvyEntRQRDACTN_CnlCAR","visible","false");	
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SitSrvyEntRQRDACTN_UpdtCAR","visible","false");	
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SitSrvyEntRQRDACTN_WrkAcptd","visible","false");	
		m_pHSEMSPolyDb->ChangeCustomButtonProperty(-1,0,"SitSrvyEntRQRDACTN_WrkNotAcptd","visible","false");	
		//also Enable (new-save buttons)
		EnableToolbar("",TOOLBAR_NEW,TRUE);
		EnableToolbar("",TOOLBAR_SAVE,TRUE);
	} else {
		EnableToolbar("",TOOLBAR_NEW,FALSE);
		EnableToolbar("",TOOLBAR_SAVE,FALSE);
	}
	return SitSrvyCategory::DisplayScreenReady(pScreenReady);
}

long SitSrvyRqrdActn::ShowSitSrvyRqrdActn(CString strCriteria)
{
	m_nCurrentCard=ID_SITSRVY_RQRDACTN_POPUP;
	return ParentManage("HSE_TgSitSrvyRqrdActn",TRUE,TRUE,TRUE,"HSE_SitSrvyRqrdActn",strCriteria);
}