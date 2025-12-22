#ifndef WstRcvngEntCategory_HEADER
#define WstRcvngEntCategory_HEADER

#include "WstMngmntCommonCategory.h"

class WstRcvngEntCategory : public WstMngmntCommonCategory  
{
public:
	WstRcvngEntCategory(CCategory *pCategory);
	virtual ~WstRcvngEntCategory();
	long ShowWstRcvngEntry(CString strCriteria);
	HRESULT DisplayFieldChange(FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	//CString getQuantityBefore(CString strZoneCode,CString strItemCode);
	//double getConvertRate(CString strConvrtdTo,CString strItm);
	void completeWasteRcvEntry(CString strPrmryKey);
	void returnWasteRcvEntry(CString strPrmryKey);	
};

#endif

