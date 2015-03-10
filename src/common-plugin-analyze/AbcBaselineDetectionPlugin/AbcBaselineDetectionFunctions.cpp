/**
 * @file AbcBaselineDetectionFunctions.cpp
 * @brief implements of AbcBaselineDetection plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.07.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AbcBaselineDetectionFunctions.h"
#include "AbcBaselineDetectionManager.h"


using namespace kome::baseline::abc;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define WINDOW_WIDTH_PARAM_NAME	    "window_width"
#define NOISE_FACTOR_PARAM_NAME	    "noise_factor"
#define SMOOTHING_WIDTH_PARAM_NAME	"smoothing_width"


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

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// manager
	AbcBaselineDetectionManager& mgr = AbcBaselineDetectionManager::getInstance();

	// set parameters
	double windowWidth = 1.0;
	double noiseFactor = 3.0;
	int smoothingWidth = 10;

	if( settings != NULL ) {
		windowWidth = settings->getDoubleValue( WINDOW_WIDTH_PARAM_NAME, 1.0 );
		noiseFactor = settings->getDoubleValue( NOISE_FACTOR_PARAM_NAME, 3.0 );
		smoothingWidth = settings->getIntValue( SMOOTHING_WIDTH_PARAM_NAME, 10 );
	}

	mgr.setWindowWidth( windowWidth );
	mgr.setNoiseFactor( noiseFactor );
	mgr.setSmoothingWidth( smoothingWidth );

	// get baseline
	mgr.getBaseline( *src, *baseline );

	return ret;
}
