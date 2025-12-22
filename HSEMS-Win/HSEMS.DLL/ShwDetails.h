// ShwDetails.h: interface for the ShwDetails class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHWDETAILS_H__2A95B095_756C_488C_9B2C_E1EEF51FA0A8__INCLUDED_)
#define AFX_SHWDETAILS_H__2A95B095_756C_488C_9B2C_E1EEF51FA0A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HSEMSCommonCategory.h"

class ShwDetails  : public CHSEMSCommonCategory  
{
public:
	ShwDetails(CCategory *pCategory);
	virtual ~ShwDetails();
	HRESULT DisplayFieldChange (FIELDCHANGE *pFieldChange);
	HRESULT DisplayRecordRepostion(tag_RecordRepostion *pRecordRepostion);
	void ChkBoxYesNoValidation();
	long ShowCopyofdetail(CString strCriteria);

};

#endif // !defined(AFX_SHWDETAILS_H__2A95B095_756C_488C_9B2C_E1EEF51FA0A8__INCLUDED_)
