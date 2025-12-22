#include "stdafx.h"
#include "HSEMS.h"
#include "AwrnsPlnCategory.h"
#include "HSEobj.h"

CString gstrCourseType="";
AwrnsPlnCategory::AwrnsPlnCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory					= pCategory;
	m_strMainLink				= "SerialPK";//"AwrnsPln_Yr"; RQ-8-2021.9
	m_strStatusFieldName	= "AwrnsPln_Sttus";
	m_strPrefix						= "AwrnsPln";
	

}

AwrnsPlnCategory::~AwrnsPlnCategory(){}


HRESULT AwrnsPlnCategory::DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked)
{
	CString strBtnName(pToolBarButtonClicked->Button_Name);
	CString strSubFormTag(pToolBarButtonClicked->SubForm_Tag);			
	if(strBtnName=="SAVETAB") {			
		//FormSetField("HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_LSTMDFYDT",GetFullDateTime("%d/%m/%Y"));
		//FormSetField("HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_LSTMDFYBY",GetUserLogin());	
		RefreshScreen("",REFRESH_SELECTED);
	}	
	
	if(strBtnName=="SAVE") {			
		if(pToolBarButtonClicked->iComplete==0)
		{
			CString strFormTag=(*pToolBarButtonClicked).Form_Tag;
			//////////////////////////////////////////////////////////////////////////////////
			//RQ-8-2021.9
			if(strFormTag=="HSE_TgAwrnsPlnEntry")
			{
				CString strLineNo = FormGetField(strFormTag,"AWRNSPLN_PLNN");
				if(strLineNo == "0") {
					CString strYear=FormGetField("HSE_TGAWRNSPLNENTRY","AWRNSPLN_YR");
					CString strSqlGetNewSerial;
					strSqlGetNewSerial.Format("SELECT ISNULL(MAX(AWRNSPLN_PLNN)+1,1) FROM HSE_AWRNSPLN WHERE AWRNSPLN_YR = %s",strYear);
					CString strCurrentMsrNo = GetRecordSetData(strSqlGetNewSerial).GetBuffer(0);	
					FormSetField("HSE_AWRNSPLN","AWRNSPLN_PLNN",strCurrentMsrNo);				
				}
			}
			//////////////////////////////////////////////////////////////////////////////////
			
		}
	}
	return CHSEMSCommonCategory::DisplayToolBarButtonClicked(pToolBarButtonClicked);
}

CString gstrKeyField;
CString gstrCourseName; 

HRESULT AwrnsPlnCategory::DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked)
{		
	CString strButtonName(pCustomButtonClicked->Button_Name);	
	strButtonName.MakeUpper();
	CString strFullRecord(pCustomButtonClicked->Full_Record);	
	CString strSubFormTag(pCustomButtonClicked->SubForm_Tag);

	/////////////////////////////////////////////////////////////////////////////////
	//RQ-3-2021.5
	bool bLocked=false;
	if(strSubFormTag=="HSE_TGAWRNSPLNINQ_CRS")
		bLocked=true;
	/////////////////////////////////////////////////////////////////////////////////

	if(strButtonName=="AWRNSPLNEXCTN_CRS_ENTRATNDS") {
		CString strAwrnsPlnYr = FormGetField("HSE_AWRNSPLN","AWRNSPLN_YR");
		enterAttendees(strAwrnsPlnYr,strSubFormTag);			
		
	} else if (strButtonName=="AWRNSPLN_VWRJCTRSN") {		
		CString strKeyField=FormGetField("HSE_AWRNSPLN_TRCNG","AWRNSPLN_TRCNG_REFRJCTRSN");
		if(strKeyField != "") {
			m_strScreenTag = "CMS_RSNS";	
			CString strSQL="";
			CString strDefValues;
			bool bLocked = true;
			strSQL.Format("SELECT * FROM CMS_RSNS WHERE (RSNS_RFREC = '%s')",strKeyField);	
			strDefValues.Format("RSNS_RFREC|%s",strKeyField);
			ShowScreen(0,"CMS_RSNS","Reject Reasons",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
		}
	}
	/*RQ-2-2021(Batch2).38,41
		Awareness Plan Entry	Add more capabilities	"•        (Courses) TAB
		o        Remove (target department) and Add custom-button (Target Departments) to enter multiple departments, if any"
	*/
	if(strButtonName=="TARGET_DEPARTMENTS") {
		//Open PopUp Screen Target Departments attached with this Course record
		CString strSQL;
		CString strDefValues;
		gstrKeyField  =FormGetField(strSubFormTag,"PrmKY");		//"HSE_TGAWRNSPLNENTRY_CRS"
		strSQL.Format("SELECT * FROM HSE_AwrnsPln_Crs_TrgtDprt WHERE (TrgtDprt_PrntCrs = %s) ",gstrKeyField);
		m_strScreenTag = "HSE_TgAwrnsPln_Crs_TrgtDprt";
		ShowScreen(0,m_strScreenTag,"Target Departments",NORMAL_MODE,true,strSQL,"","","",bLocked);		
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	/*RQ-2-2021(Batch2).39,42
		Awareness Plan Entry	Add more capabilities		"•        (Courses) TAB
		o        Remove (instructor info) frame & all related fields and  Add custom-button (Instructors) to enter multiple instructors, if any"
	*/
	if(strButtonName=="INSTRUCTORS_INFO") {
		//Open PopUp Screen Instructors attached with this Course record
		CString strSQL;
		CString strDefValues;
		gstrKeyField =FormGetField(strSubFormTag,"PrmKY");		//"HSE_TGAWRNSPLNENTRY_CRS"
		gstrCourseName =FormGetField(strSubFormTag,"AWRNSPLN_CRS_CRSNAM");//This is a must field so it should have a value
		strSQL.Format("SELECT * FROM HSE_AwrnsPln_Crs_Instrctr WHERE (Instrctr_PrntCrs = %s) ",gstrKeyField);
		m_strScreenTag = "HSE_TgAwrnsPln_Crs_Instrctr";
		ShowScreen(0,m_strScreenTag,"Instructors",NORMAL_MODE,true,strSQL,"","","",bLocked);		
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	return CHSEMSCommonCategory::DisplayCustomButtonClicked(pCustomButtonClicked);
}


HRESULT AwrnsPlnCategory::DisplayFieldChange(FIELDCHANGE *pFieldChange)
{
	CString strFormTag(pFieldChange->Form_Tag);
	CString strSubFormTag(pFieldChange->SubForm_Tag);
	CString strFieldName(pFieldChange->Field_Name);
	CString strNewValue = pFieldChange->New_Value;	
	if(strFieldName=="AWRNSPLN_CRS_CRTFCTISUD")
	{		
		if(strNewValue == "Yes") {
			SetFieldVisibleAttrib("HSE_TGAWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_ISUDAT","MUST");
			SetFieldVisibleAttrib("HSE_TGAWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_EXPRYDAT","MUST");
		} else {
			FormSetField("HSE_AWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_ISUDAT","");
			FormSetField("HSE_AWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_EXPRYDAT","");
			SetFieldVisibleAttrib("HSE_TGAWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_ISUDAT","ALWAYSLOCKED");
			SetFieldVisibleAttrib("HSE_TGAWRNSPLNEXCTN_CRS","AWRNSPLN_CRS_EXPRYDAT","ALWAYSLOCKED");			
		}

	}
	////////////////////////////////////////////////////////////////////
	/*RQ-2-2021(Batch2).40,RQ-2-2021(Batch2).43
		Awareness Plan Entry	 Ability to insert different compinations for all parameters without any constraint	"•        (Courses) TAB
		o       Remove current index(Month, Week, Course) and add a line number as index"
	*/

	
	strFieldName.MakeUpper();
	if(strFormTag=="HSE_TGAWRNSPLN_CRS_ATNDS")
	{
		//Do nothing
	}
	else if(strFieldName.Find("AWRNSPLN_CRS")>=0)
	{
		if(strFormTag=="")
			strFormTag="HSE_TGAWRNSPLNENTRY_CRS";
		else
			strFormTag+="_CRS";
		
		CString strLineNo = FormGetField(strFormTag,"AWRNSPLN_CRS_SRIL");
		if(strLineNo == "") {
			getNewSerialNoForATab("HSE_AwrnsPln","SerialPK","HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_LNK","AWRNSPLN_CRS_SRIL");//Bugs-10-2021.23
			
		}
	}
	////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayFieldChange(pFieldChange);
}

void AwrnsPlnCategory::enterAttendees(CString strAwrnsPlnYr,CString strSubFormTag) 
{
	CString strKeyField=FormGetField("HSE_AWRNSPLN_CRS","PRMKY");
	m_strScreenTag = "HSE_TGAWRNSPLN_CRS_Atnds";	
	CString strSQL="";
	CString strDefValues;
	bool bLocked = true;
	strSQL.Format("SELECT * FROM HSE_AWRNSPLN_CRS_Atnds WHERE (AWRNSPLN_CRS_Atnds_LNK = '%s')",strKeyField);	
	if(strSubFormTag =="HSE_TGAWRNSPLNEXCTN_CRS") {
		bLocked = false;
	}	
	strDefValues.Format("AWRNSPLN_CRS_Atnds_LNK|%s",strKeyField);
	
	gstrCourseType=FormGetField("HSE_AWRNSPLN_CRS","AWRNSPLN_CRS_CRSTYP");
	ShowScreen(0,"HSE_TGAWRNSPLN_CRS_Atnds","Course Attendees",NORMAL_MODE,true,strSQL,"",strDefValues,"",bLocked);
}

HRESULT AwrnsPlnCategory::DisplayScreenReady(SCREENREADY *pScreenReady)
{
	CString strFormTag=(*pScreenReady).Form_Tag;
	////////////////////////////////////////////////////////////////////////////////////////////
	//RQ-3-2021.21
	//If (courses TAB\course type) combo = “awareness” then LOCK the (attendee status) combo
#define	ONE_TO_MANY	2
	short curFormType=m_pHSEMSPolyDb->GetFormType();
	if(curFormType==ONE_TO_MANY)
	{
		if(strFormTag.Find("HSE_TGAWRNSPLN_CRS_ATNDS")!=-1 && gstrCourseType=="A")
		{
			SetFieldVisibleAttrib(strFormTag,"AWRNSPLN_CRS_ATNDS_ATNDSTTS","ALWAYSLOCKED");
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	return CHSEMSCommonCategory::DisplayScreenReady(pScreenReady);
}
