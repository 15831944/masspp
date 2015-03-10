/**
 * @file ScaleDrawerFunctions.cpp
 * @brief implements of ScaleDrawerFunctions plug-in
 *
 * @author S.Tanaka
 * @date 2007.03.01
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ScaleDrawerFunctions.h"
#include "ScaleDrawer.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define DRAWING_SPEC_STATUS_KEY			"DRAWING_SPEC_SCALE"
#define DRAWING_CHROM_STATUS_KEY	    "DRAWING_CHROM_SCALE"
#define DRAWING_DATAMAP_STATUS_KEY		"DRAWING_DATAMAP_SCALE"
#define DRAWING_STATUS_VALUE			"standard"


// prepare to draw spectrum
kome::objects::Variant prepareSpectrumScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// prepare
	ScaleDrawer::getInstance().prepareToDraw( graphics );

	return ret;
}

// draw spectrum foreground
kome::objects::Variant drawSpectrumScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// get profile drawer
	ScaleDrawer& drawer = ScaleDrawer::getInstance();

	// draw foreground
	drawer.drawScale( spectrum, graphics );

	return ret;
}

// prepare to draw chromatogram
kome::objects::Variant prepareChromatogramScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// prepare
	ScaleDrawer::getInstance().prepareToDraw( graphics );

	return ret;
}

// draw chromatogram foreground
kome::objects::Variant drawChromatogramScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// get profile drawer
	ScaleDrawer& drawer = ScaleDrawer::getInstance();

	// draw scale
	drawer.drawScale( chromatogram, graphics );

	return ret;
}

// prepare to draw data map
kome::objects::Variant prepareDataMapScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// prepare
	ScaleDrawer::getInstance().prepareToDraw( graphics );

	return ret;
}

// draw data map foreground
kome::objects::Variant drawDataMapScale( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// get profile drawer
	ScaleDrawer& drawer = ScaleDrawer::getInstance();

	// draw scale
	drawer.drawScale( dataMap, graphics );

	return ret;
}

// set chromatogram drawing scale status
kome::objects::Variant setChromatogramDrawingScaleStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_CHROM_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE ) == 0 ) {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, DRAWING_STATUS_VALUE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// set spectrum drawing scale status
kome::objects::Variant setSpectrumDrawingScaleStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_SPEC_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE ) == 0 ) {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, DRAWING_STATUS_VALUE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// set datamap drawing scale status
kome::objects::Variant setDatamapDrawingScaleStatus( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_DATAMAP_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE ) == 0 ) {
		statusMgr.setValue( DRAWING_DATAMAP_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_DATAMAP_STATUS_KEY, DRAWING_STATUS_VALUE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}
