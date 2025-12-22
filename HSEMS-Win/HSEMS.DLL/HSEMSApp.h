#ifndef HSEMSAPP_H
#define	HSEMSAPP_H
class CHSEObj;
class CHSEMSApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void	SetHSEObj(CHSEObj	*pHSEobj)
	{
		m_pHSEobj=pHSEobj;
	};
	CHSEObj	*GetHSEObj()
	{
		return m_pHSEobj;
	};
protected:
	CHSEObj	*m_pHSEobj;
};

#endif	//HSEMSAPP_H
