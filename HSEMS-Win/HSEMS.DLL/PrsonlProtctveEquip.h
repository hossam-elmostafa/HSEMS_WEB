/**
 *	Handle Personal Protective Equipment
 *	
 *
 *
 */
#include "HSEMSCommonCategory.h"

class PrsonlProtctveEquip : public CHSEMSCommonCategory  
{
public:
	PrsonlProtctveEquip(CCategory *pCategory);
	virtual ~PrsonlProtctveEquip();				
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowPersonalProtectiveEquipmentpopUp(CString strCriteria);
	long ShowPersonalProtectiveEquipmentScreen(CString strCriteria);
};