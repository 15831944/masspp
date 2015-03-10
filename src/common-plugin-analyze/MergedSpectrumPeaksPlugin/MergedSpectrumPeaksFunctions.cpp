/**
 * @file MergedSpectrumPeaksFunctions.cpp
 * @brief implements of MergedSpectrumPeaks plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrumPeaksFunctions.h"                  
//#include "MergedSpectrumPeaks.h"
#include "MergedSpecPeakPosDetect.h"

using namespace kome::position::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// peak position
kome::objects::Variant detectPeakPosition( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	MergedSpecPeakPosDetect* peakPos = new MergedSpecPeakPosDetect( true, true );
	peakPos->setSettings( settings );
	ret.prim.pt = peakPos;
	//// progress
	//kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );
	//if( progress == NULL ) {
	//	progress = &kome::core::Progress::getIgnoringProgress();
	//}
	//
	//// sample getter
	//kome::ident::SampleGetter* getter = kome::plugin::PluginCallTool::getSampleGetter( params );

	//// peaks
	//kome::objects::Peaks2D* peaks = kome::plugin::PluginCallTool::getPeaks2D( params );
	//if( peaks == NULL ) {
	//	return ret;
	//}
	//
	//// get position
	//MergedSpectrumPeaks peakPos( settings, getter );
	//peakPos.execute( peaks, *progress );

	return ret;
}
