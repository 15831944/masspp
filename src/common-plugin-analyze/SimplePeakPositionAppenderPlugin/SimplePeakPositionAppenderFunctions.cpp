/**
 * @file SimplePeakPositionAppenderFunctions.cpp
 * @brief implements of SimplePeakPositionAppender plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.04.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SimplePeakPositionAppenderFunctions.h"
#include "SimplePeakPosDetect.h"

using namespace kome::position::simple;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




// peak position
kome::objects::Variant addPeakPositions( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		return ret;
	}

	SimplePeakPosDetect* peakDetect = new SimplePeakPosDetect( false, true );
	peakDetect->setSettings( settings );

	ret.prim.pt = peakDetect;

	//// peak
	//kome::objects::Peaks2D* peaks = kome::plugin::PluginCallTool::getPeaks2D( params );
	//if( peaks == NULL ) {
	//	return ret;
	//}

	//// progress
	//kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	//if( progress == NULL ) {
	//	progress = &kome::core::Progress::getIgnoringProgress();
	//}

	//// get position
	//double rt = settings->getDoubleValue( RT_PARAM_NAME, -1.0 );
	//double mz = settings->getDoubleValue( MZ_PARAM_NAME, -1.0 );
	//int charge = settings->getIntValue( CHARGE_PARAM_NAME, -1 );

	//if( rt < 0.0 && mz < 0.0 ) {
	//	LOG_ERROR_CODE( FMT( "Both of RT and m/z is empty. At least one of RT and m/z is required. Input RT or m/z." ), ERR_OTHER );
	//	return ret;
	//}

	//// add position
	//kome::objects::Peak2DElement* peak = peaks->createPeak( rt, mz, -1.0 );
	//peak->setCharge( charge );

	//// fill progress
	//progress->fill();

	return ret;
}
