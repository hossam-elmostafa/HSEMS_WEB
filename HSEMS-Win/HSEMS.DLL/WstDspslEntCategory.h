#ifndef WstDspslEntCategory_HEADER
#define WstDspslEntCategory_HEADER

#include "WstMngmntCommonCategory.h"

class WstDspslEntCategory : public WstMngmntCommonCategory  
{
public:
	WstDspslEntCategory(CCategory *pCategory);
	virtual ~WstDspslEntCategory();
	long ShowWstDspslEntry(CString strCriteria);
	HRESULT DisplayFieldChange(FIELDCHANGE *pFieldChange);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);			
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	void completeDspslEntry(CString strPrmryKey);
	void returnWasteDspslEntry(CString strPrmryKey);
	CString getManifestFrmNo();
};

#endif

