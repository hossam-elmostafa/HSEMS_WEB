// AccidentReasonCategory.h: interface for the AccidentReasonCategory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCIDENTREASONCATEGORY_H__7C111BB8_21D5_46F1_B09D_7C70A85629D2__INCLUDED_)
#define AFX_ACCIDENTREASONCATEGORY_H__7C111BB8_21D5_46F1_B09D_7C70A85629D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSEMSCommonCategory.h"

class AccidentReasonCategory : public CHSEMSCommonCategory  
{
public:
	AccidentReasonCategory(CCategory *pCategory);
	virtual ~AccidentReasonCategory();
	HRESULT DisplayCustomButtonClicked(tag_CustomButtonClicked* pCustomButtonClicked);
	void SetScreenFormTag(CString strScreenTag) {m_strScreenTag = strScreenTag;};
	CString GetScreenFormTag() {return m_strScreenTag;};
	long ShowAccidentReason(CString strCriteria);

private:
	void OpenSubDetailScreen();

};

#endif // !defined(AFX_ACCIDENTREASONCATEGORY_H__7C111BB8_21D5_46F1_B09D_7C70A85629D2__INCLUDED_)
