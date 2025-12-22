#ifndef IncdntInvstgtnEntryCategory_HEADER
#define IncdntInvstgtnEntryCategory_HEADER

#include "AccidentCategory.h"

class IncdntInvstgtnEntryCategory : public AccidentCategory  
{
public:
	IncdntInvstgtnEntryCategory(CCategory *pCategory);
	virtual ~IncdntInvstgtnEntryCategory();
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	long ShowIncdntInvstgtnEntry(CString strCriteria);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	CString allowIncdntInvstgtnEntry();
	void runAftrDltIncdntInvstgtn(CString strPrmryKey);
	void CompleteAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif

