////StaticCategory.h
#ifndef _CUTILITY_H
#define _CUTILITY_H
class CUtility : public CCommonCategory
{
public:
	CUtility(CCategory *pCategory):CCommonCategory(pCategory)
	{
		m_pCategory = pCategory;
	};
//	~CUtility();
	void DefineCodeMethod();
	void DefineUserDefinedChecks();
};
#endif
