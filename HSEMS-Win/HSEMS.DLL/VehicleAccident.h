// VehicleAccident.h: interface for the VehicleAccident class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEHICLEACCIDENT_H__C389088E_7E09_4BEF_B47C_3C8AA38AC383__INCLUDED_)
#define AFX_VEHICLEACCIDENT_H__C389088E_7E09_4BEF_B47C_3C8AA38AC383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class VehicleAccident : public CHSEMSCommonCategory  
{
public:
	VehicleAccident(CCategory *pCategory);
	virtual ~VehicleAccident();
	bool CheckLicenseExpireDate();
	HRESULT DisplayToolBarButtonClicked(TOOLBARBUTTONCLICKED *pToolBarButtonClicked);
	HRESULT DisplayRecordRepostion(RECORDREPOSTION *pRecordRepostion);
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void rejectRecord(CString strFormtag);
	int calculateDriverTtlAccdnt(CString strEmpNo);
	HRESULT DisplayBrowsingSelectedItem(BROWSINGSELECTEDITEM  *pBrowsingSelectedItem);
	CString getDeptNameByCod(CString strDeptCod);

};

#endif // !defined(AFX_VEHICLEACCIDENT_H__C389088E_7E09_4BEF_B47C_3C8AA38AC383__INCLUDED_)
