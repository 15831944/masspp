/**
 * @file MascotSearchManager.cpp
 * @brief implements of MascotSearchManager class
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MascotSearchManager.h"


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MascotSearchManager::MascotSearchManager() {
}

// destructor
MascotSearchManager::~MascotSearchManager() {
}

// get PMF search engine
MascotSearchEngineBase* MascotSearchManager::getPmfEngine() {
	return &m_pmfEngine;
}

// get MIS search engine
MascotSearchEngineBase* MascotSearchManager::getMisEngine() {
	return &m_misEngine;
}

// get instance
MascotSearchManager& MascotSearchManager::getInstance() {
	// create the object
	static MascotSearchManager mgr;

	return mgr;
}
