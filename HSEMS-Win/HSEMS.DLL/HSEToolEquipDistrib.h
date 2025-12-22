// HSEToolEquipDistrib.h: interface for the CHSEToolEquipDistrib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSETOOLEQUIPDISTRIB_H__310BA00D_CC61_4781_9EBC_E35283E2C222__INCLUDED_)
#define AFX_HSETOOLEQUIPDISTRIB_H__310BA00D_CC61_4781_9EBC_E35283E2C222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class CHSEToolEquipDistrib : public CHSEMSCommonCategory  
{
public:
	CHSEToolEquipDistrib(CCategory *pCategory);
	virtual ~CHSEToolEquipDistrib();
	HRESULT DisplayBeforBrowsingButtonClick(tag_BeforBrowsingButtonClick *pBeforBrowsingButtonClick);
	long ShowHSEToolsEquipmentsDistribution(CString strCriteria);
};

#endif // !defined(AFX_HSETOOLEQUIPDISTRIB_H__310BA00D_CC61_4781_9EBC_E35283E2C222__INCLUDED_)
