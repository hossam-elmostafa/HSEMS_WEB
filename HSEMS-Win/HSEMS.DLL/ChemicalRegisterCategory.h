#ifndef ChemicalRegisterCategory_HEADER
#define ChemicalRegisterCategory_HEADER

#include "HSEMSCommonCategory.h"

class ChemicalRegisterCategory : public CHSEMSCommonCategory  
{
public:
	ChemicalRegisterCategory(CCategory *pCategory);
	virtual ~ChemicalRegisterCategory();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);			
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);

protected:
	void UpdateQuantities();
};

#endif

