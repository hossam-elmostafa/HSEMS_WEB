// VehicleAccidentConfirmationCategory.h: interface for the VehicleAccidentConfirmationCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEHICLEACCIDENTCONFIRMATIONCATEGORY_H__A444EDEA_53AC_4903_B10C_CACC0021EA49__INCLUDED_)
#define AFX_VEHICLEACCIDENTCONFIRMATIONCATEGORY_H__A444EDEA_53AC_4903_B10C_CACC0021EA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VehicleAccident.h"

class VehicleAccidentConfirmationCategory : public VehicleAccident  
{
public:
	VehicleAccidentConfirmationCategory(CCategory *pCategory);
	virtual ~VehicleAccidentConfirmationCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	void ConfirmVehicleAccident(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowVehicleAccidentConfirmation(CString strCriteria);

};

#endif // !defined(AFX_VEHICLEACCIDENTCONFIRMATIONCATEGORY_H__A444EDEA_53AC_4903_B10C_CACC0021EA49__INCLUDED_)
