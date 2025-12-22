

#include "HSEMSCommonCategory.h"

class CHierarchyControl: public CHSEMSCommonCategory 
{
public:
	CHierarchyControl(CCategory *pCategory);
	virtual ~CHierarchyControl();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowControlMeasures(CString strCriteria);
};