////Category.h
#ifndef _LISTLISTEN_H
#define _LISTLISTEN_H


class CListlistener
{
public:
	CListlistener(){};
	~CListlistener(){};
	virtual void SetData(LPCTSTR lpszRetVal,LPCTSTR lpszAllData)=0;
	
protected:	
	void SetDataSink(CWnd *pSinkControl);

protected:	
	CWnd	*m_pDataSinkWnd;
};

#endif
