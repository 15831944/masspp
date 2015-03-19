/**
 * @file IdentifyManager.cpp
 * @brief implements of IdentifyManager class
 *
 * @author OKADA.H
 * @date 2013/07/29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "IdentifyManager.h"

using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// get IdentifyManager object
IdentifyManager& IdentifyManager::getInstance() {
	// create object
	static IdentifyManager mgr;

	return mgr;
}
