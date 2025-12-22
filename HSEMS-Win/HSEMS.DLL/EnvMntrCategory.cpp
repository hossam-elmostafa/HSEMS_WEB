#include "stdafx.h"
#include "HSEMS.h"
#include "EnvMntrCategory.h"
#include "HSEobj.h"

EnvMntrCategory::EnvMntrCategory(CCategory *pCategory):CHSEMSCommonCategory(pCategory) {
	m_pCategory = pCategory;
}

EnvMntrCategory::~EnvMntrCategory(){}


