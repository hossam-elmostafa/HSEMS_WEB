#ifndef _EVENTIMPL
#define _EVENTIMPL
#include "AFXPRIV2.H"

//#define Interface EventSink

#define IMPLTYPE_MASK \
	(IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE | IMPLTYPEFLAG_FRESTRICTED)

#define IMPLTYPE_DEFAULTSOURCE \
	(IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE)

#define INTERFACE_IMPL(baseClass,Interface) \
	STDMETHODIMP_(ULONG) baseClass::X##Interface::AddRef()\
	{return 1;}\
	STDMETHODIMP_(ULONG) baseClass::X##Interface::Release()\
	{return 0;}\
	STDMETHODIMP baseClass::X##Interface::QueryInterface(REFIID iid, LPVOID* ppvObj)\
	{METHOD_PROLOGUE_EX_(baseClass,Interface)\
		IID iid2;\
		pThis->GetEventIID(&iid2);\
		if (IsEqualIID(iid, IID_IUnknown) ||\
			IsEqualIID(iid, IID_IDispatch) ||\
			IsEqualIID(iid, iid2))\
		{	*ppvObj = this;	AddRef();return S_OK;}\
		else{return E_NOINTERFACE;}}\
	STDMETHODIMP baseClass::X##Interface::GetTypeInfoCount(unsigned int*)\
	{return E_NOTIMPL;}\
	STDMETHODIMP baseClass::X##Interface::GetTypeInfo(unsigned int, LCID, ITypeInfo**)\
	{return E_NOTIMPL;}\
	STDMETHODIMP baseClass::X##Interface::GetIDsOfNames(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*)\
	{return E_NOTIMPL;}\
	STDMETHODIMP baseClass::X##Interface::Invoke(DISPID dispid, REFIID, LCID, unsigned short wFlags,\
	DISPPARAMS* pDispParams, VARIANT* pvarResult,\
	EXCEPINFO* pExcepInfo, unsigned int* puArgError)\
	{UNUSED(wFlags);\
	METHOD_PROLOGUE_EX(baseClass,Interface)\
	ASSERT(wFlags == DISPATCH_METHOD);\
	AFX_EVENT event(AFX_EVENT::event, dispid, pDispParams, pExcepInfo,\
		puArgError);\
	pThis->OnEvent(-1,&event,NULL);\
	if (pvarResult != NULL)\
		::VariantClear(pvarResult);\
	return event.m_hResult;}\
	BOOL baseClass::GetEventIID(IID* piid)\
	{if(m_iidInterface!=GUID_NULL){*piid=m_iidInterface;return TRUE;}\
		*piid = GUID_NULL;ASSERT(m_pObject != NULL);LPPROVIDECLASSINFO2 pPCI2 = NULL;\
		if (SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo2,(LPVOID*)&pPCI2)))\
		{ASSERT(pPCI2 != NULL);\
		if (SUCCEEDED(pPCI2->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, piid)))\
				ASSERT(!IsEqualIID(*piid, GUID_NULL));else ASSERT(IsEqualIID(*piid, GUID_NULL));pPCI2->Release();}\
		LPPROVIDECLASSINFO pPCI = NULL;\
		if (IsEqualIID(*piid, GUID_NULL) &&	SUCCEEDED(m_pObject->QueryInterface(IID_IProvideClassInfo,(LPVOID*)&pPCI)))\
		{ASSERT(pPCI != NULL);LPTYPEINFO pClassInfo = NULL;\
			if (SUCCEEDED(pPCI->GetClassInfo(&pClassInfo)))\
			{ASSERT(pClassInfo != NULL);LPTYPEATTR pClassAttr;\
				if (SUCCEEDED(pClassInfo->GetTypeAttr(&pClassAttr)))\
				{ASSERT(pClassAttr != NULL);ASSERT(pClassAttr->typekind == TKIND_COCLASS);int nFlags;HREFTYPE hRefType;\
					for (unsigned int i = 0; i < pClassAttr->cImplTypes; i++)\
					{if (SUCCEEDED(pClassInfo->GetImplTypeFlags(i, &nFlags)) &&	((nFlags & IMPLTYPE_MASK) == IMPLTYPE_DEFAULTSOURCE))\
						{LPTYPEINFO pEventInfo = NULL;\
							if (SUCCEEDED(pClassInfo->GetRefTypeOfImplType(i,&hRefType)) && SUCCEEDED(pClassInfo->GetRefTypeInfo(hRefType,&pEventInfo)))\
							{ASSERT(pEventInfo != NULL);LPTYPEATTR pEventAttr;\
								if (SUCCEEDED(pEventInfo->GetTypeAttr(&pEventAttr))){ASSERT(pEventAttr != NULL);*piid = pEventAttr->guid;pEventInfo->ReleaseTypeAttr(pEventAttr);}\
									pEventInfo->Release();}\
							break;}}\
					pClassInfo->ReleaseTypeAttr(pClassAttr);}\
				pClassInfo->Release();}\
				pPCI->Release();}\
		return (!IsEqualIID(*piid, GUID_NULL));}\
		void baseClass::EstablishConnectionWithEventSrc(IID iidInterface)\
		{m_iidInterface=GUID_NULL;m_pObject->QueryInterface(IID_IConnectionPointContainer,(void**)&m_pConnectionPointContainer);m_dwCookie=0;if(iidInterface==GUID_NULL)GetEventIID(&iidInterface);m_iidInterface=iidInterface;m_pConnectionPointContainer->FindConnectionPoint(iidInterface,&m_pConnectionPoint);m_pConnectionPoint->Advise(&m_xEventSink,&m_dwCookie);\
		}\
		void baseClass::EndConnectionWithEventSrc()\
		{m_pConnectionPoint->Unadvise(m_dwCookie);m_pConnectionPoint->Release();m_pConnectionPointContainer->Release();\
		}\


#define BEGIN_INTERFACE_PART_NEW(Interface)\
protected:\
	void	EstablishConnectionWithEventSrc(IID iidInterface=GUID_NULL);\
	void	EndConnectionWithEventSrc();\
	BOOL	GetEventIID(IID* piid);\
	IUnknown* volatile m_pObject;\
	IConnectionPointContainer *m_pConnectionPointContainer;\
	IConnectionPoint *m_pConnectionPoint;\
	DWORD	m_dwCookie;\
	IID		m_iidInterface;\
public:\
	BEGIN_INTERFACE_PART(Interface, IDispatch)\
		STDMETHOD(GetTypeInfoCount)(unsigned int*);\
		STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);\
		STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);\
		STDMETHOD(Invoke)(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*,\
						  VARIANT*, EXCEPINFO*, unsigned int*);\
	END_INTERFACE_PART(Interface)\

#endif
