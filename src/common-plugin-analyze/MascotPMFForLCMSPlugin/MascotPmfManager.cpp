/**
 * @file MascotPmfManager.cpp
 * @brief implements of MascotPmfManager class
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MascotPmfManager.h"


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MascotPmfManager::MascotPmfManager() {
}

// destructor
MascotPmfManager::~MascotPmfManager() {
}

// get PMF search engine
MascotSearchEngineBase* MascotPmfManager::getPmfEngine() {
	return &m_pmfEngine;
}

// get instance
MascotPmfManager& MascotPmfManager::getInstance() {
	// create the object
	static MascotPmfManager mgr;

	return mgr;
}
