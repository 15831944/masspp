/**
 * @file RawWaveformFunctions.cpp
 * @brief interfaces of RawWaveform plug-in functions
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "RawWaveformFunctions.h"
#include "RawWaveformManager.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// open raw data
kome::objects::Variant openRawData( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get objects
	kome::objects::Variant var  = kome::plugin::PluginCallTool::getActiveObject( params );

	kome::objects::MsDataVariant obj( var );
	kome::objects::Spectrum* selectSpec = obj.getSpectrum();
	kome::objects::Chromatogram* selectChrom = obj.getChromatogram();

	if( selectSpec == NULL && selectChrom == NULL ){
		LOG_ERROR( FMT("Failed to get objects.") );
		return ret;
	}

	// manager
	RawWaveformManager& mgr  = RawWaveformManager::getInstance();

	if( selectSpec != NULL ){
		ret.prim.boolVal = mgr.openRawWaveformSpectrum( selectSpec );
	}
	
	if( selectChrom != NULL ){
		ret.prim.boolVal = mgr.openRawWaveformChromatogram( selectChrom );
	}

	return ret;
}

// コンテクストメニュー ( spec )
kome::objects::Variant openSpectrumFromContextMenu( kome::objects::Parameters* params ){
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get spectrum
	kome::objects::Spectrum* spec = obj.getSpectrum();
	
	// manager
	RawWaveformManager& mgr  = RawWaveformManager::getInstance();
	if( spec != NULL ) {
		ret.prim.boolVal = mgr.openRawWaveformSpectrum( spec );
	}

	ret.prim.boolVal = true;

	return ret;
}

//　コンテクストメニュー ( chrom )
kome::objects::Variant openChromatogramFromContextMenu( kome::objects::Parameters* params ){
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get chromatogram
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	
	// manager
	RawWaveformManager& mgr  = RawWaveformManager::getInstance();
	if( chrom != NULL ){
		ret.prim.boolVal = mgr.openRawWaveformChromatogram( chrom );
	}
	
	return ret;
}
