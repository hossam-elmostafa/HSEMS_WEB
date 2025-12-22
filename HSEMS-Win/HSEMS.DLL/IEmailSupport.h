#if !defined _IEMAILSUPPORT_H
#define _IEMAILSUPPORT_H

class IEmailSupport
{
public:
	virtual CString getScreenFldVal(CString strFldName) = 0;
};

#endif