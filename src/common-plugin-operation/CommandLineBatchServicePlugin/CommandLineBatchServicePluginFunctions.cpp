/**
 * @file CommandLineBatchServicePluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author satstnka
 * @date 2012.03.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CommandLineBatchServicePluginFunctions.h"
#include "CommandLineBatchService.h"


using namespace kome::batchservice::command;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// batch service
kome::objects::Variant getBatchService( kome::objects::Parameters* ) {
	// manger
	CommandLineBatchServiceManager& mgr = CommandLineBatchServiceManager::getInstance();

	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;
	ret.prim.pt = mgr.getService();

	return ret;
}
