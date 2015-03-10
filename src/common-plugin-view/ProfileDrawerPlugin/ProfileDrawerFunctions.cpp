/**
 * @file ProfileDrawerFunctions.cpp
 * @brief implements of ProfileDrawerFunctions plug-in
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ProfileDrawerFunctions.h"
#include "ProfileDrawer.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DRAWING_CHROM_STATUS_KEY			"DRAWING_CHROMATOGRAM"
#define DRAWING_SPEC_STATUS_KEY				"DRAWING_SPECTRUM"
#define DRAWING_STATUS_VALUE_PROFILE		"profile"
#define DRAWING_STATUS_VALUE_IMPULSE		"impulse"


// draw spectrum profile
kome::objects::Variant drawSpectrumProfile( kome::objects::Parameters* params ) {
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
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	drawer.drawSpectrum( spectrum, graphics );

	return ret;
}

// draw spectrum Impulse @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant drawSpectrumImpulse( kome::objects::Parameters* params ) {
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
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	drawer.drawSpectrum( spectrum, graphics );

	return ret;
}
// close spectrum
kome::objects::Variant updateSpectrum( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	// get profile drawer
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	// delete spectrum data
	drawer.deleteSpectrumData( spectrum );

	return ret;
}

// draw chromatogram profile
kome::objects::Variant drawChromatogramProfile( kome::objects::Parameters* params ) {
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
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	drawer.drawChromatogram( chromatogram, graphics );

	return ret;
}

// draw chromatogram impulse @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant drawChromatogramImpulse( kome::objects::Parameters* params ) {
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
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	drawer.drawChromatogram( chromatogram, graphics );


	return ret;
}
// close chromatogram
kome::objects::Variant updateChromatogram( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	// get profile drawer
	ProfileDrawer& drawer = ProfileDrawer::getInstance();

	// delete chromatogram data
	drawer.deleteChromatogramData( chromatogram );

	return ret;
}

// set chromatogram drawing graph status ( profile ) @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant setChromatogramDrawingGraphStatusProfile( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_CHROM_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE_PROFILE ) == 0 ) {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, DRAWING_STATUS_VALUE_PROFILE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}
// set chromatogram drawing graph status ( impulse ) @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant setChromatogramDrawingGraphStatusImpulse( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_CHROM_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE_IMPULSE ) == 0 ) {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_CHROM_STATUS_KEY, DRAWING_STATUS_VALUE_IMPULSE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}
// set spectrum drawing graph status ( profile ) @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant setSpectrumDrawingGraphStatusProfile( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_SPEC_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE_PROFILE ) == 0 ) {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, DRAWING_STATUS_VALUE_PROFILE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}

// set spectrum drawing graph status ( impulse ) @date 2011.02.28 <Add> M.Izumi 
kome::objects::Variant setSpectrumDrawingGraphStatusImpulse( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	std::string val = NVL( statusMgr.getValue( DRAWING_SPEC_STATUS_KEY ), "" );

	// set status
	if( val.compare( DRAWING_STATUS_VALUE_IMPULSE ) == 0 ) {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, "" );
	}
	else {
		statusMgr.setValue( DRAWING_SPEC_STATUS_KEY, DRAWING_STATUS_VALUE_IMPULSE );
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}
