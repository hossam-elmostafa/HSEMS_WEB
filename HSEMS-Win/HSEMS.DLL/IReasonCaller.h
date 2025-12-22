#if !defined(IReasonCaller_H)
#define IReasonCaller_H

interface IReasonCaller
{
	public:
		virtual void getParameters(CString &strOwnerScreenTag,CString &strCallerIdInitial,CString &strCallerIdFinal,CString &strPrmKey,bool &bEnableResults,CString &strReasonsFilter,CString &strRequiredCaption)=0;
};


#endif