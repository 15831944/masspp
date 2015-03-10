/**
 * @file MultiAlgorithmLabeling.cpp
 * @brief implements of MultiAlgorithmLabeling class
 *
 * @author S.Tanaka
 * @date 2007.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MultiAlgorithmLabeling.h"

#include <math.h>
#include <boost/bind.hpp>


using namespace kome::labeling::multi;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MULTI_ALGORITHM_SHORT_NAME				"multi"


// constructor
MultiAlgorithmLabeling::MultiAlgorithmLabeling() {
}

// destructor
MultiAlgorithmLabeling::~MultiAlgorithmLabeling() {
}

// execute labeling
void MultiAlgorithmLabeling::executeLabeling(
		kome::plugin::PluginFunctionItem& item,
		kome::objects::Parameters& params,
		kome::objects::SettingParameterValues* settings
) {
	// spectrum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( &params );
	if( spec == NULL ) {
		return;
	}

	// xy data
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( &params );
	if( xyData == NULL ) {
		return;
	}

	// peaks
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( &params );
	if( peaks == NULL ) {
		return;
	}

	// create parameters
	kome::objects::Parameters p;

	kome::plugin::PluginCallTool::setSpectrum( p, *spec );
	kome::plugin::PluginCallTool::setXYData( p, *xyData );
	kome::plugin::PluginCallTool::setPeaks( p, *peaks );

	if( settings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( p, *settings );
	}

	// peak picking
	item.getCall()->invoke( &p );
}
