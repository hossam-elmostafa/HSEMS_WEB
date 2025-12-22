#include "stdafx.h"
#include "LogclDelTblObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*LogclDelTblObj::LogclDelTblObj(){
}
LogclDelTblObj::~LogclDelTblObj(){
}*/

CString LogclDelTblObj::getTracingTblUsrNamFld (){
	return strTracingTblUsrNamFld;
}

void LogclDelTblObj::setTracingTblUsrNamFld(CString tracingTblUsrNamFld) {
	strTracingTblUsrNamFld = tracingTblUsrNamFld;
}

CString LogclDelTblObj::getTracingTblStatFldVal(){
	return strTracingTblStatFldVal;
}

void LogclDelTblObj::setTracingTblStatFldVal(CString tracingTblStatFldVal) {
	strTracingTblStatFldVal	= tracingTblStatFldVal;
}

CString LogclDelTblObj::getTracingTblStatFldName (){
	return strTracingTblStatFldName;
}

void LogclDelTblObj::setTracingTblStatFldName(CString tracingTblStatFldName) {
	strTracingTblStatFldName = tracingTblStatFldName;
}

CString LogclDelTblObj::getTracingTbl (){
	return strTracingTbl;
}

void LogclDelTblObj::setTracingTbl(CString tracingTbl) {
	strTracingTbl =	tracingTbl;
}

CString LogclDelTblObj::getTagNam (){
	return strTagNam;
}

void LogclDelTblObj::setTagNam(CString tagNam) {
	strTagNam =	tagNam;
}

CString LogclDelTblObj::getTblNam(){
	return	strTblNam;
}

void LogclDelTblObj::setTblNam(CString tblNam) {
	strTblNam =	tblNam;
}

CString LogclDelTblObj::getHeaderStatFldNam (){
	return strHeaderStatFldNam;
}

void LogclDelTblObj::setHeaderStatFldNam(CString headerStatFldNam) {
	strHeaderStatFldNam = headerStatFldNam;
}

CString LogclDelTblObj::getDelStateVal(){
	return strDelStateVal;
}

void LogclDelTblObj::setDelStateVal(CString delStateVal) {
	strDelStateVal = delStateVal;
}