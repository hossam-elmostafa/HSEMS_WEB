#ifndef EnvMntrMsrPopupCategory_HEADER
#define EnvMntrMsrPopupCategory_HEADER


#include "EnvMntrCategory.h"

class EnvMntrMsrPopupCategory : public EnvMntrCategory  
{
public:
	EnvMntrMsrPopupCategory(CCategory *pCategory);
	virtual ~EnvMntrMsrPopupCategory();
	long ShowEnvMntrActvtsPltnt(CString strCriteria,CString strComeFrom);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif

