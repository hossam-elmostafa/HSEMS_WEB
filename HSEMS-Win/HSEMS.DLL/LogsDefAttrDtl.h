/**
 *	Handle Log definition Attribute Detail screen
 *
 */
#include "HSEMSCommonCategory.h"

class LogsDefAttrDtl : public CHSEMSCommonCategory  
{
public:
	LogsDefAttrDtl(CCategory *pCategory);
	virtual ~LogsDefAttrDtl();
	long ShowLogsDefAttrDtl(CString strCriteria);	
};

