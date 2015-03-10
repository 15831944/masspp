/**
 * @file ThermoIOPluginFunction.cpp
 * @brief Thermo IO Plugin Functions
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ThermoIOPluginFunctions.h"
#include "ThermoSampleSet.h"

using namespace kome::io::thermo;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


kome::objects::Variant openRaw( kome::objects::Parameters* params ) {
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
	ThermoSampleSet* sampleSet = new ThermoSampleSet();

	// set file
	ret.prim.pt = sampleSet;

	return ret;
}

