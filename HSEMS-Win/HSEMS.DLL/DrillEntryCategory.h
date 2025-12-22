#ifndef DrillEntryCategory_HEADER
#define DrillEntryCategory_HEADER

#include "HSEMSCommonCategory.h"

class DrillEntryCategory : public CHSEMSCommonCategory  
{
public:
	DrillEntryCategory(CCategory *pCategory);
	virtual ~DrillEntryCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	long ShowDrillInquiry(CString strCriteria);
	void openDrillFindingScr(CString strTableName,CString strSubFormTag);
	void openDrillFindingUnplndScnrosScr(CString strTableName,CString strSubFormTag);
};

#endif

