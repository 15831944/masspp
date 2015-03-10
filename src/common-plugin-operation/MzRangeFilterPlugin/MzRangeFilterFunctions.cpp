/**
 * @file MzRangeFilterFunctions.cpp
 * @brief implements of m/z range filter plug-in functions
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MzRangeFilterFunctions.h"
#include "MzRangeFilterManager.h"
#include "MzRangeFilterOperation.h"

using namespace kome::mzfilter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// open dialog
kome::objects::Variant openMzRangeFilterDialog( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	MzRangeFilterManager& mgr = MzRangeFilterManager::getInstance();
	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	
	// open dialog
	ret.prim.boolVal = mgr.openDialog( var );
	
	return ret;
}

// get m/z range filter operation
kome::objects::Variant getmzRangeFilterOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new MzRangeFilterOperation();
	
	return ret;
}
