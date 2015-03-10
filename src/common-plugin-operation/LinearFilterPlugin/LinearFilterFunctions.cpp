/**
 * @file LinearFilterFunctions.cpp
 * @brief implements of LinearFilter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LinearFilterFunctions.h"
#include "LinearFilterManager.h"


using namespace kome::filter::linear;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define WIDTH_PARAM_NAME		"width"
#define STRENGTH_PARAM_NAME		"strength"
#define SIGMA_PARAM_NAME		"sigma"


// Low-Pass Filter
kome::objects::Variant executeLowPass( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// get paramters
	int width = 1;
	if( settings != NULL ) {
		width = settings->getIntValue( WIDTH_PARAM_NAME, 1 );
	}

	// execute
	LinearFilterManager& mgr = LinearFilterManager::getInstance();
	mgr.averaging( width, *src, *dst );

	return ret;
}

// High-Pass Filter
kome::objects::Variant executeHighPass( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// get paramters
	int width = 1;
	double strength = 1.0;
	if( settings != NULL ) {
		width = settings->getIntValue( WIDTH_PARAM_NAME, 1 );
		strength = settings->getDoubleValue( STRENGTH_PARAM_NAME, 1.0 );
	}
	
	// execute
	LinearFilterManager& mgr = LinearFilterManager::getInstance();
	mgr.sharpening( strength, width, *src, *dst );

	return ret;
}

// Gaussian Filter
kome::objects::Variant executeGaussian( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get data points
	kome::core::XYData* src = kome::plugin::PluginCallTool::getXYData( params );
	if( src == NULL ) {
		return ret;
	}

	kome::core::XYData* dst = kome::plugin::PluginCallTool::getUpdatedXYData( params );
	if( dst == NULL ) {
		return ret;
	}

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	// get paramters
	int width = 1;
	double sigma = 1.0;
	if( settings != NULL ) {
		width = settings->getIntValue( WIDTH_PARAM_NAME, 1 );
		sigma = settings->getDoubleValue( SIGMA_PARAM_NAME, 1.0 );
	}

	// execute
	LinearFilterManager& mgr = LinearFilterManager::getInstance();
	mgr.gaussian( sigma, width, *src, *dst );

	return ret;
}
