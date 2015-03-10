/**
 * @file AIAInputPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author fujita
 * @date 2012.07.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "AIAInputPluginFunctions.h"
#include "AIASampleSet.h"

using namespace kome::io::aia;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open file
kome::objects::Variant openAIA( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::SAMPLE_SET;
	ret.prim.pt = NULL;

	// get path
	const char* path = kome::plugin::PluginCallTool::getPath( params );
	if( path == NULL ) {
		return ret;
	}

	// open
	AIASampleSet* sampleSet = new AIASampleSet();
	
	// set file
	ret.prim.pt = sampleSet;

	return ret;
}
