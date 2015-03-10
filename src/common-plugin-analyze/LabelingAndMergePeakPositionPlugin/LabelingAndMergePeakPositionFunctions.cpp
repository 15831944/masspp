/**
 * @file LabelingAndMergePeakPositionFunctions.cpp
 * @brief implements of LabelingAndMergePeakPosition plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.11.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LabelingAndMergePeakPositionFunctions.h"   
#include "MergePeakPosDetect.h"

using namespace kome::position::merge;


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

	// merge peak position detector
	MergePeakPosDetect* peakPos = new MergePeakPosDetect( false, true );
	peakPos->setSettings( settings );
	peakPos->setLastError( "" );	//ƒGƒ‰[î•ñ‰Šú‰»

	ret.prim.pt = peakPos;

	return ret;
}
