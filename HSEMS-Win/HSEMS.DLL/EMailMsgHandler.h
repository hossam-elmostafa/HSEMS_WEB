#include "stdafx.h"
#include "Category.h"


class EMailMsgHandler
{
	
public:
	EMailMsgHandler(CCategory *pCategory);
	virtual ~EMailMsgHandler();
	CString FormatMailBody(int nMsgID,CString strFormTag);

private:
	CCategory *m_pCategory;
};
