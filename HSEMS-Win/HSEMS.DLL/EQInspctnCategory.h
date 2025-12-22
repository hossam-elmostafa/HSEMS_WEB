#ifndef EQInspctnCategory_HEADER
#define EQInspctnCategory_HEADER

#include "HSEMSCommonCategory.h"

class EQInspctnCategory : public CHSEMSCommonCategory  
{
public:
	EQInspctnCategory(CCategory *pCategory);
	virtual ~EQInspctnCategory();
	long ShowEQInspctnRqust(CString strCriteria);
	long ShowEQInspctnRsult(CString strCriteria);
	long ShowEQInspctnWrk(CString strCriteria);
	long ShowEQInspctnInqry(CString strCriteria);
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	CString getCurrentEQInspctnNo(CString strCurrentYear);
	CString getCurrentEQInspctnDfctNo(CString strMainLink);
	void completeEQInspctn(CString strPrmKy);
	void reldEQInspctnChckLst(CString strPrmKy);
	void rjctEQInspctnDataEntry(CString strPrmKy);
	void rjctEQInspctn(CString strPrmKy);
	void holdEQInspctn(CString strPrmKy);
	void accptEQInspctn(CString strPrmKy);
	void stopDefectedEQ(CString strPrmKy);
	void resumeEQWrk(CString strPrmKy);
	void exitDefectedEQ(CString strPrmKy);
	void canclEQWrk(CString strPrmKy);
	void cmpltEQWrk(CString strPrmKy);
	int getRltdInspctnChcklstCount(CString strNewValue);	
	void viewInspctnRqstHstry();
};

#endif