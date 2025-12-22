#ifndef IncdntInvstgtnAprvCategory_HEADER
#define IncdntInvstgtnAprvCategory_HEADER

#include "AccidentCategory.h"

class IncdntInvstgtnAprvCategory : public AccidentCategory  
{
public:
	IncdntInvstgtnAprvCategory(CCategory *pCategory);
	virtual ~IncdntInvstgtnAprvCategory();
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowIncdntInvstgtnAprv(CString strCriteria);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	void CloseAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	void RejectAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
};

#endif

