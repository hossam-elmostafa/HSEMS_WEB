#ifndef _DONGLELISTEN_H
#define _DONGLELISTEN_H

#include"eventimp.h"
class CCMNObjBase;
class CDongleListen:public CCmdTarget
{
public:
	CDongleListen(CCMNObjBase *pHSEObj,CString strServerName,int iProtocol,int iAppCode,CString strUserName);
	~CDongleListen();
	void SignOut(int iFlag);
	BEGIN_INTERFACE_PART_NEW(EventSink)
	afx_msg void OnEventSignOn(int iFlag,LPCTSTR strMessage);
	DECLARE_EVENTSINK_MAP()
protected:
	bool m_bApplyDongle;
	CCMNObjBase *m_pHSEObj;
	IDnglPrxyPtr m_DnglPrxy;
};
#endif