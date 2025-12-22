#include "stdafx.h"
#include "Utility.h"

void CUtility::DefineCodeMethod()
{
	m_nCurrentCard=ID_UTILITIES_APPLICATIONSETUP;
	m_pHSEMSPolyDb->ShowSegmentsSystem(m_pCategory->GetParentHWnd());
}

void CUtility::DefineUserDefinedChecks()
{
	m_nCurrentCard=ID_UTILITIES_USERDEFINEDCHECKS;
	m_pHSEMSPolyDb->ShowUserDefinedChecks();
}


