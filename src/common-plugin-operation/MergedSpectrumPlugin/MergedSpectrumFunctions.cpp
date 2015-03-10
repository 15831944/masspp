/**
 * @file MergedSpectrumFunctions.cpp
 * @brief implements of merged spectrum plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MergedSpectrumFunctions.h"
#include "MergedSpectrumManager.h"
#include "AverageSpecOperaton.h"

using namespace kome::operation::merged;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CHROMATOGRAM_RANGE_STATUS_NAME	   "CHROMATOGRAM_RANGE_ACTION"
#define CHROMATOGRAM_RANGE_STATUS_VALUE	   "merged spectrum"


// merge spectra
kome::objects::Variant mergeSpectra( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
 
	// merge
	MergedSpectrumManager& mgr = MergedSpectrumManager::getInstance();
	ret.prim.boolVal = mgr.mergeAllSpectra();

	return ret;
}

// show Averaged Spectrum
kome::objects::Variant showAveragedSpectrum( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get chromatogram
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	// get range
	double startMz = double();
	double endMz = double();
	if( !kome::plugin::PluginCallTool::getRange( params, &startMz, &endMz ) ) {
		return ret;
	}

	// create Averaged Spectrum dialog
	MergedSpectrumManager& mgr = MergedSpectrumManager::getInstance();
	mgr.createAveragedSpectrum( chromatogram, startMz, endMz );

	return ret;
}
//2012/02/28 add by t.okuno <- This plugin displays a range choice screen.

// get operation
kome::objects::Variant getAverageSpec( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new AverageSpecOperaton();
	
	return ret;
}
