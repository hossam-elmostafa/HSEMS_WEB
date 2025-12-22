// VehchileAccidentInquiry.h: interface for the VehchileAccidentInquiry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEHCHILEACCIDENTINQUIRY_H__DC6E1595_AAE2_40F7_99D6_9FE278661C8C__INCLUDED_)
#define AFX_VEHCHILEACCIDENTINQUIRY_H__DC6E1595_AAE2_40F7_99D6_9FE278661C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VehicleAccident.h"
class VehchileAccidentInquiry  : public VehicleAccident
{
public:
	VehchileAccidentInquiry(CCategory *pCategory);
	virtual ~VehchileAccidentInquiry();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	long ShowVehchileAccidentInquiry(CString strCriteria);

};

#endif // !defined(AFX_VEHCHILEACCIDENTINQUIRY_H__DC6E1595_AAE2_40F7_99D6_9FE278661C8C__INCLUDED_)
