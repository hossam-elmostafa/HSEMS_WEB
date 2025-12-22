#ifndef EnvMsrAprvlCategory_HEADER
#define EnvMsrAprvlCategory_HEADER

#include "HSEMSCommonCategory.h"

class EnvMsrAprvlCategory : public CHSEMSCommonCategory  
{
public:
	EnvMsrAprvlCategory(CCategory *pCategory);
	virtual ~EnvMsrAprvlCategory();
	long ShowEnvMsrAprvl(CString strCriteria);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void rejectEnvMsr(CString strPrmKy);
	void approveEnvMsr(CString strPrmKy);
};

#endif

