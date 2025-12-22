#ifndef EnvrnmntAspctReviewCategory_HEADER
#define EnvrnmntAspctReviewCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvrnmntAspctReviewCategory : public CHSEMSCommonCategory  
{
public:
	EnvrnmntAspctReviewCategory(CCategory *pCategory);
	virtual ~EnvrnmntAspctReviewCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);		
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	long ShowEnvronmntlAspctReview(CString strCriteria);
	CString allowAspctReview();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void acceptEnvAspect(CString strPrmryKey);
	void rejectEnvAspect(CString strPrmryKey);
};

#endif

