/**
 * @file PeakPositionsFileImportFunctions.cpp
 * @brief implements of PeakPositionsFileImport plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.12.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakPositionsFileImportFunctions.h"        
#include "FileImportPeakPosDetect.h" 

using namespace kome::position::file;


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
	
	FileImportPeakPosDetect* peakPosDetect = new FileImportPeakPosDetect( false, true );
	peakPosDetect->setSettings( settings );
	ret.prim.pt = peakPosDetect;

	return ret;
}
