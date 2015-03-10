/**
 * @file DataMapDrawerFunctions.cpp
 * @brief implements of DataMapDrawerFunctions plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataMapDrawerFunctions.h"
#include "DataMapDrawer.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


	
#define STATUS_NAME			"DRAWING_DATAMAP"
#define STATUS_VALUE		"colormap"


// draw grayscale data map
kome::objects::Variant drawColorMap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.draw( *graphics, *dataMap );

	return ret;
}

// draw bar
kome::objects::Variant drawBar( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// draw
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.drawNaviBar( *graphics, *dataMap );

	return ret;
}

// prepare to draw
kome::objects::Variant prepareDataMap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// prepare
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.prepare( *graphics, *dataMap );

	return ret;
}

// delete bitmap data
kome::objects::Variant deleteBitmap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::DataMapInfo* dataMap = kome::plugin::PluginCallTool::getDataMapInfo( params );
	if( dataMap == NULL ) {
		return ret;
	}

	// delete
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.deleteData( *dataMap );

	return ret;
}

// delete sample
kome::objects::Variant deleteSampleBitmap( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get objects
	kome::objects::Sample* sample = kome::plugin::PluginCallTool::getSample( params );
	if( sample == NULL ) {
		return ret;
	}

	// delete
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.deleteData( *sample );

	return ret;
}

// update settings
kome::objects::Variant updateSettings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// status
	DataMapDrawer& drawer = DataMapDrawer::getInstance();
	drawer.updateStatus();

	// refresh
	kome::window::WindowTool::refresh();	

	return ret;
}

// sets dsample map status clormap
kome::objects::Variant setStatusValue( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// get status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* val = statusMgr.getValue( STATUS_NAME );

	// set status
	if( val != NULL && strcmp( STATUS_VALUE, val ) == 0 ) {
		statusMgr.setValue( STATUS_NAME, "" );
	}
	else {
		statusMgr.setValue( STATUS_NAME, STATUS_VALUE );		
	}

	// refresh
	kome::window::WindowTool::refresh();

	return ret;
}
