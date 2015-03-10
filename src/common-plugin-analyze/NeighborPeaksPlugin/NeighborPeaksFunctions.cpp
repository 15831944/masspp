/**
 * @file NeighborPeaksFunctions.cpp
 * @brief implements of NeighborPeaks plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "NeighborPeaksFunctions.h"               
#include "NeighborPeakValuesCalculator.h" 

using namespace kome::search::neighbor;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// search spectrum peak
kome::objects::Variant searchSpectrumPeak( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	NeighborPeakValuesCalculator* peakVal = new NeighborPeakValuesCalculator( true );
	peakVal->setSettings( settings );

	ret.prim.pt = peakVal;
	
	return ret;
}

// search chromtogram peak
kome::objects::Variant searchChromatogramPeak( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// settings
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );

	NeighborPeakValuesCalculator* peakVal = new NeighborPeakValuesCalculator( false );
	peakVal->setSettings( settings );

	ret.prim.pt = peakVal;

	return ret;
}
