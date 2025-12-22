class LogclDelTblObj
{
private:
	CString strTagNam;
	CString strTblNam;
	CString strHeaderStatFldNam;    
	CString strDelStateVal;
	CString strTracingTbl;
	CString strTracingTblStatFldName;
	CString strTracingTblStatFldVal;
	CString strTracingTblUsrNamFld;


public:
	CString getTracingTblUsrNamFld();
	void setTracingTblUsrNamFld(CString tracingTblUsrNamFld);
	CString getTracingTblStatFldVal();
	void setTracingTblStatFldVal(CString tracingTblStatFldVal);
	CString getTracingTblStatFldName();
	void setTracingTblStatFldName(CString tracingTblStatFldName);
	CString getTracingTbl();
	void setTracingTbl(CString tracingTbl);
	CString getTagNam();
	void setTagNam(CString tagNam);
	CString getTblNam();
	void setTblNam(CString tblNam);
	CString getHeaderStatFldNam();
	void setHeaderStatFldNam(CString headerStatFldNam);
	CString getDelStateVal();
	void setDelStateVal(CString delStateVal);

};
