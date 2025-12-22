// VehicleAccidentFollowUpCategory.h: interface for the VehicleAccidentFollowUpCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEHICLEACCIDENTFOLLOWUPCATEGORY_H__471AAD72_E62A_42EC_B061_FE0185685E8A__INCLUDED_)
#define AFX_VEHICLEACCIDENTFOLLOWUPCATEGORY_H__471AAD72_E62A_42EC_B061_FE0185685E8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VehicleAccident.h"

class VehicleAccidentFollowUpCategory : public VehicleAccident  
{
public:
	VehicleAccidentFollowUpCategory(CCategory *pCategory);
	virtual ~VehicleAccidentFollowUpCategory();
	HRESULT OpenScreen(CString strCaption,CString strSQL,long lMenuID,long lParam,BOOL bModal,BOOL bFilterMode);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	HRESULT DisplayScreenReady(SCREENREADY *pScreenReady);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	void CloseVehicleAccident(CUSTOMBUTTONCLICKED* pCustomButtonClicked);
	long ShowVehicleAccidentFollowUp(CString strCriteria);
};

#endif // !defined(AFX_VEHICLEACCIDENTFOLLOWUPCATEGORY_H__471AAD72_E62A_42EC_B061_FE0185685E8A__INCLUDED_)
