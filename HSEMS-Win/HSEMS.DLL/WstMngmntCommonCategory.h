#ifndef WstMngmntCommonCategory_HEADER
#define WstMngmntCommonCategory_HEADER


#include "HSEMSCommonCategory.h"

class WstMngmntCommonCategory : public CHSEMSCommonCategory  
{
public:
	WstMngmntCommonCategory(CCategory *pCategory);
	virtual ~WstMngmntCommonCategory();
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	CString getQuantityBefore(CString strZoneCode,CString strItemCode);
	double getConvertRate(CString strConvrtdTo,CString strItm);	
};

#endif