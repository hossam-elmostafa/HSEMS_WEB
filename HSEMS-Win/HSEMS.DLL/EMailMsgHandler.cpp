#include "stdafx.h"
#include "EMailMsgHandler.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

EMailMsgHandler::EMailMsgHandler(CCategory *pCategory){	
	m_pCategory = pCategory;	
}

EMailMsgHandler::~EMailMsgHandler(){
}


/**
 *
 * 
 */
CString EMailMsgHandler::FormatMailBody(int nMsgID,CString strFormTag) {		
	CString strSQL;
	CString strMsgBody;
	strSQL.Format("SELECT MSGBODY FROM HSE_MAILMSG WHERE MSGID = %d",nMsgID);
	strMsgBody = m_pCategory ->GetMyCCategory()-> GetValueFromSQL(strSQL.AllocSysString(),"MSGBODY");		
	CString strTemp;
	CString strLine=strtok(strMsgBody.GetBuffer(0),"\n");	
	while (strLine != "")
	{
		strLine.Replace("\n","");
		int nStartPos = strLine.Find("[$");
		int nEndPos	  = strLine.Find("]");	
		if(nStartPos != -1 && nEndPos != -1) {
			CString strVariableName = strLine.Mid(nStartPos+2,(nEndPos)-(nStartPos+2));	
 			CString strParmVal = m_pCategory->GetMyCCategory()->getScreenFldVal(strVariableName,strFormTag);
			strLine.Replace("[$"+strVariableName+"]",strParmVal);
		}
		strTemp += strLine;
		strLine=strtok(NULL,"\n");
	}
	//For Test
	//AfxMessageBox(strTemp);
	return strTemp;
}