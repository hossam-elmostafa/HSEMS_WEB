#ifndef EnvrnmntAspctCategory_HEADER
#define EnvrnmntAspctCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvrnmntAspctCategory : public CHSEMSCommonCategory  
{
public:
	EnvrnmntAspctCategory(CCategory *pCategory);
	virtual ~EnvrnmntAspctCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);		
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	long ShowEnvronmntlAspctEntry(CString strCriteria);
	CString getAspctYr();
	CString getAspctMonth();
	CString allowAspctEntry();
	//CString getEmpDep(CString strUserID);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void completeEntry(CString strPrmryKey);
	CString getfRskLvl(CString fstrRskRnk);
};

#endif

