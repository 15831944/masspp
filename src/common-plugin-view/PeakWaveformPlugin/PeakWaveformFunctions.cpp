/**
 * @file PeakWaveformFunctions.cpp
 * @brief interfaces of PeakWaveformFunctions plug-in functions
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakWaveformFunctions.h"
#include "PeakWaveformManager.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// open disp peaks
kome::objects::Variant openDispPeaks( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// get objects
	kome::objects::Variant var  = kome::plugin::PluginCallTool::getActiveObject( params );

	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* selectSpec = obj.getSpectrum();
	kome::objects::Chromatogram* selectChrom = obj.getChromatogram();

	PeakWaveformManager& mgr = PeakWaveformManager::getInstance();

	if( selectSpec != NULL ){
		ret.prim.boolVal = mgr.openPeakWaveformSpectrum( selectSpec );
	}

	if( selectChrom != NULL ){
		ret.prim.boolVal = mgr.openPeakWaveformChromatogram( selectChrom );
	}
	
	return ret;
}

// コンテクストメニュー ( spec )
kome::objects::Variant openSpectPeaksFromContextMenu( kome::objects::Parameters* params ){
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get spectrum
	kome::objects::Spectrum* spec = obj.getSpectrum();

	PeakWaveformManager& mgr = PeakWaveformManager::getInstance();
	if( spec != NULL ) {
		ret.prim.boolVal = mgr.openPeakWaveformSpectrum( spec );
	}

	ret.prim.boolVal = true;

	return ret;
}

//　コンテクストメニュー ( chrom )
kome::objects::Variant openChromPeaksFromContextMenu( kome::objects::Parameters* params ){
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get chromatogram
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	
	PeakWaveformManager& mgr = PeakWaveformManager::getInstance();
	if( chrom != NULL ){
		ret.prim.boolVal = mgr.openPeakWaveformChromatogram( chrom );
	}
	
	ret.prim.boolVal = true;

	return ret;
}


