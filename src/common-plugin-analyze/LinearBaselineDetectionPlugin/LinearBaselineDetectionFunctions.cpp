/**
 * @file LinearBaselineDetectionFunctions.cpp
 * @brief implements of LinearBaselineDetection plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LinearBaselineDetectionFunctions.h"            
#include "LinearBaselineDetection.h"


using namespace kome::baseline::linear;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// detect baseline
kome::objects::Variant detectBaseline( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get object
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}
	kome::core::XYData* baseline = kome::plugin::PluginCallTool::getBaseline( params );
	if( baseline == NULL ) {
		return ret;
	}

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// manager
	LinearBaselineDetection detector( settings );

	// get baseline
	detector.getBaseline( *src, *baseline );

	return ret;
}
