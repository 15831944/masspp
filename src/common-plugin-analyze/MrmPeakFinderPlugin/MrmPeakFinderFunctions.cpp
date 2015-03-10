/**
 * @file MrmPeakFinderFunctions.cpp
 * @brief implements of MrmPeakFinder plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MrmPeakFinderFunctions.h" 
//#include "MrmPeakFinder.h"
#include "MrmPeakPosDetect.h"


using namespace kome::position::mrm;


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

	// Mrm Peak positon detector
	MrmPeakPosDetect* peakPos = new MrmPeakPosDetect();
	peakPos->setSettings( settings );
	ret.prim.pt = peakPos;

	return ret;
}
