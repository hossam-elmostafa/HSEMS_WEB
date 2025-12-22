#include "stdafx.h"
#include "HandleDelClass.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HandleDelClass::HandleDelClass(CCategory *pCategory){
	m_pCategory = pCategory;
	//initHadleDelObj();
}

HandleDelClass::~HandleDelClass(){
}

bool HandleDelClass::initHadleDelObj() {
	//read from table in firstSys and Create Array of objects with table structure
	//reason not to access database with every delete operation     

	//1-Create array from LogicalDelTbl Objects
	//2-Assign array object to member in class HandleDeleteClass    
	/*CString strTableName;
	CString strSQL;		
	CString strTagNam;
	CString strTableNam;
	CString strStateFldNam;
	CString strDelStateVal;
	CString strTracingTblNam;
	CString strTracingStateNam;
	CString strTracingStateVal;
	CString strTracingUsrFldNam;
	strSQL.Format("SELECT TAGNAME,TABLENAME,STATEFLDNAME,DELSTATEVAL,TRACINGTBLNAM,TRACINGSTATENAM,TRACINGSTATEVAL,TRACINGUSRFLDNAM FROM LOGICALDELETETABLES");	
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	if(!DaoRecset.IsEOF()) {
		DaoRecset.MoveFirst();
		while(!DaoRecset.IsEOF()) {
			DaoRecset.GetFieldValue("TAGNAME",strTagNam);
			DaoRecset.GetFieldValue("TABLENAME",strTableNam);
			DaoRecset.GetFieldValue("STATEFLDNAME",strStateFldNam);
			DaoRecset.GetFieldValue("DELSTATEVAL",strDelStateVal);
			DaoRecset.GetFieldValue("TRACINGTBLNAM",strTracingTblNam);
			DaoRecset.GetFieldValue("TRACINGSTATENAM",strTracingStateNam);
			DaoRecset.GetFieldValue("TRACINGSTATEVAL",strTracingStateVal);
			DaoRecset.GetFieldValue("TRACINGUSRFLDNAM",strTracingUsrFldNam);						
			LogclDelTblObj* logclDelTblObj = new LogclDelTblObj();
			logclDelTblObj->setTagNam(strTagNam);
			logclDelTblObj->setTblNam(strTableNam);
			logclDelTblObj->setHeaderStatFldNam(strStateFldNam);
			logclDelTblObj->setDelStateVal(strDelStateVal);
			logclDelTblObj->setTracingTbl(strTracingTblNam);
			logclDelTblObj->setTracingTblStatFldName(strTracingStateNam);
			logclDelTblObj->setTracingTblStatFldVal(strTracingStateVal);
			logclDelTblObj->setTracingTblUsrNamFld(strTracingUsrFldNam);			
			m_vlogclDelTblObj.push_back(*logclDelTblObj);
			DaoRecset.MoveNext();
		}	
	}*/		
	return false;
}

/**
 * Logical delete record
 * get data from firstSys database table : 'logicalDeleteTables'
 */
bool HandleDelClass::performLogclDel(CString strTagNam,CString keyFldVal,CString strUsrName,CString strSrcScr,CString strSrvrDate) {
	bool bRecLogicalDel = false;
	
	//CHSEMSCommonCategory *HSECommonCatObj = new CHSEMSCommonCategory(m_pCategory);	
	//CString strPhysicaldeleteAllowed = HSECommonCatObj->IsPhysicaldeleteAllowed(strCmpnyNam);	
	//if physical delete allowed then perform physical delete
	//if(strPhysicaldeleteAllowed == "True" || strPhysicaldeleteAllowed == "1") {
	//	return bRecLogicalDel;
	//}

	CString strSQL;		
	CString strKeyFld;
	CString strTableNam;
	CString strStateFldNam;
	CString strDelStateVal;
	CString strTracingTblNam;
	CString strTracingStateNam;
	CString strTracingStateVal;
	CString strTracingUsrFldNam;
	CString strTracingLnkFldNam;
	
	strSQL.Format("SELECT TRUETABLENAME,KEYFIELD,STATEFLDNAME,DELSTATEVAL,TRACINGTBLNAM,TRACINGSTATENAM,TRACINGSTATEVAL,TRACINGUSRFLDNAM,TRACINGLNKFLDNAM FROM LOGICALDELETETABLES WHERE TABLENAME = '%s'",strTagNam);	
	CPolyDbRecordset DaoRecset(PDBRS_LOAD_FROM_SYS_DB);
	DaoRecset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
	
	if(!DaoRecset.IsEOF()) {					
		DaoRecset.GetFieldValue("TRUETABLENAME",strTableNam);
		DaoRecset.GetFieldValue("KEYFIELD",strKeyFld);
		DaoRecset.GetFieldValue("STATEFLDNAME",strStateFldNam);
		DaoRecset.GetFieldValue("DELSTATEVAL",strDelStateVal);
		DaoRecset.GetFieldValue("TRACINGTBLNAM",strTracingTblNam);
		DaoRecset.GetFieldValue("TRACINGSTATENAM",strTracingStateNam);
		DaoRecset.GetFieldValue("TRACINGSTATEVAL",strTracingStateVal);
		DaoRecset.GetFieldValue("TRACINGUSRFLDNAM",strTracingUsrFldNam);	
		DaoRecset.GetFieldValue("TRACINGLNKFLDNAM",strTracingLnkFldNam);
		//TODO : review below code
		//Check if record already deleted (logical delete) then record should physical deleted 
		strSQL.Format("SELECT %s FROM %s WHERE %s = %s",strStateFldNam,strTableNam,strKeyFld,keyFldVal);
		CString strRecStatus = m_pCategory->GetMyCCategory()->GetValueFromSQL(strSQL.AllocSysString(),strStateFldNam);
		if(strRecStatus == strDelStateVal)
			return false;
		
		strSQL.Format("UPDATE %s SET %s = %s WHERE %s = %s",strTableNam,strStateFldNam,strDelStateVal,strKeyFld,keyFldVal);
		long lRetVal = m_pCategory->ExecuteSQL(strSQL);
		if(lRetVal == 1) {
			bRecLogicalDel = true;
		}		
		CString strTracingDateFldNam = strTracingTblNam;
		strTracingDateFldNam.Replace("HSE_","");
		strTracingDateFldNam = strTracingDateFldNam + "_DATTIM";			
		//Insert tracing data in tracing table record if exist.
		if(strTracingTblNam != "") {
			strSQL.Format("INSERT INTO %s (%s,%s,%s,SRCSCRN,%s) VALUES ('%s','%s',%s,'%s','%s') ",strTracingTblNam,strTracingStateNam,strTracingUsrFldNam,strTracingLnkFldNam,strTracingDateFldNam,strTracingStateVal,strUsrName,keyFldVal,strSrcScr,strSrvrDate);
			m_pCategory->ExecuteSQL(strSQL);		
		}
	}	
	return bRecLogicalDel;
}
