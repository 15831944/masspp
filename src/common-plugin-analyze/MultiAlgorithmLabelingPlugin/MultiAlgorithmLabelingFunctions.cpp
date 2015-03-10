/**
 * @file MultiAlgorithmLabelingFunctions.cpp
 * @brief implements of MultiAlgorithmLabeling plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MultiAlgorithmLabelingFunctions.h"     
#include "MultiAlgorithmLabeling.h"


using namespace kome::labeling::multi;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PROFILE_MODE_PARAM_NAME				   "prof_mode"
#define CENTROID_MODE_PARAM_NAME			   "centroid_mode"

#define SPECTRUM_PEAK_PICKING_FUNC_TYPE		   "SPEC_PEAK_PICKING"
#define CHARGE_DETECT_PARENT_PEAK_FUNC_TYPE	   "CHARGE_PEAK_PICKING"


// detect peaks
kome::objects::Variant detectPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MultiAlgorithmLabeling labeling;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	kome::objects::SettingParameterValues* subSettings = NULL;

	// get detector
	const char* alg = NULL;
	if( settings != NULL ) {
		if( spec->isCentroidMode() ) {
			alg = settings->getParameterValue( CENTROID_MODE_PARAM_NAME );
			subSettings = settings->getSubParameterValues( CENTROID_MODE_PARAM_NAME, NVL( alg, "" ) );
		}
		else {
			alg = settings->getParameterValue( PROFILE_MODE_PARAM_NAME );
			subSettings = settings->getSubParameterValues( PROFILE_MODE_PARAM_NAME, NVL( alg, "" ) );
		}
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, NVL( alg, "" ) );

	if( item == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector." ) );
		return ret;
	}
	LOG_DEBUG( FMT( "Multi Labeling [select \"%s\"]", item->getLongName() ) );

	// labeling
	labeling.executeLabeling( *item, *params, subSettings );

	return ret;
}

// detect parent peaks
kome::objects::Variant detectParentPeaks( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// manager
	MultiAlgorithmLabeling labeling;

	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	kome::objects::SettingParameterValues* subSettings = NULL;

	// get detector
	const char* alg = NULL;
	if( settings != NULL ) {
		if( spec->isCentroidMode() ) {
			alg = settings->getParameterValue( CENTROID_MODE_PARAM_NAME );
			subSettings = settings->getSubParameterValues( CENTROID_MODE_PARAM_NAME, NVL( alg, "" ) );
		}
		else {
			alg = settings->getParameterValue( PROFILE_MODE_PARAM_NAME );
			subSettings = settings->getSubParameterValues( PROFILE_MODE_PARAM_NAME, NVL( alg, "" ) );
		}
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( CHARGE_DETECT_PARENT_PEAK_FUNC_TYPE, NVL( alg, "" ) );

	if( item == NULL ) {
		LOG_ERROR( FMT( "Failed to get the peak detector." ) );
		return ret;
	}
	LOG_DEBUG( FMT( "Multi Labeling [select \"%s\"]", item->getLongName() ) );

	// labeling
	labeling.executeLabeling( *item, *params, subSettings );

	return ret;
}
