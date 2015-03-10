/**
 * @file IntensityRateChargeDetectFunctions.cpp
 * @brief implements of IntensityRateChargeDetect plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.10.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "IntensityRateChargeDetectFunctions.h"        
#include "IntensityRateChargeDetect.h"


using namespace kome::charge::ir;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// detect peaks
kome::objects::Variant detectCharge( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// charge detect object
	IntensityRateChargeDetect detector;

	// get xy data object
	kome::core::XYData* xyData = kome::plugin::PluginCallTool::getXYData( params );
	if( xyData == NULL ) {
		return ret;
	}

	// get peaks object
	kome::objects::Peaks* peaks = kome::plugin::PluginCallTool::getPeaks( params );
	if( peaks == NULL ) {
		return ret;
	}

	// charge detect
	detector.getCharge( *xyData, *peaks );

	return ret;
}
