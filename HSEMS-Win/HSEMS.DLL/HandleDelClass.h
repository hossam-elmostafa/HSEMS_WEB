#include "stdafx.h"
#include "Category.h"
#include <vector>
#include "LogclDelTblObj.h"
#include "HSEMSCommonCategory.h"




class HandleDelClass 
{
public:
	HandleDelClass(CCategory *pCategory);
	virtual ~HandleDelClass();
	bool initHadleDelObj();

private:
	std::vector<LogclDelTblObj> m_vlogclDelTblObj;
	CCategory *m_pCategory;

public :
	bool performLogclDel(CString strTagName,CString keyFldVal,CString strUsrName,CString strSrcScr,CString strSrvrDate);
};
