/**
 * @file PeakLabelDrawerFunctions.cpp
 * @brief implements of PeakLabelDrawerFunctions plug-in
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakLabelDrawerFunctions.h"
#include "PeakLabelDrawer.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DRAWING_SPEC_STATUS_KEY	    "DRAWING_SPEC_PEAKS"
#define DRAWING_CHROM_STATUS_KEY    "DRAWING_CHROM_PEAKS"
#define DRAWING_STATUS_VALUE		"peak label"


// draw label
kome::objects::Variant drawSpecPeaksLabel( kome::objects::Parameters* params ) {
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

	// draw label
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.drawLabel( spectrum, graphics );

	return ret;
}

// get space to draw label
kome::objects::Variant setSpecGraphMargin( kome::objects::Parameters* params ) {
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

	// get space to draw label
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.prepareDraw( spectrum, graphics );

	return ret;
}

// deletes spectrum peak information
kome::objects::Variant deleteSpectrumPeakInfo( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	// delete spectrum peak information
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.deleteDrawData( spectrum );
	drawer.deleteRectListMap( spectrum );

	return ret;
}

// draw label
kome::objects::Variant drawChromPeaksLabel( kome::objects::Parameters* params ) {
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

	// draw label
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.drawLabel( chromatogram, graphics );

	return ret;
}

// get space to draw label
kome::objects::Variant setChromGraphMargin( kome::objects::Parameters* params ) {
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

	// get space to draw label
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.prepareDraw( chromatogram, graphics );

	return ret;
}

// deletes chromatogram peak information
kome::objects::Variant deleteChromatogramPeakInfo( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	// delete chromatogram peak information
	PeakLabelDrawer& drawer = PeakLabelDrawer::getInstance();
	drawer.deleteDrawData( chromatogram );
	drawer.deleteRectListMap( chromatogram );

	return ret;
}

// set drawing chromatogram peaks status
kome::objects::Variant setChromatogramDrawingPeaksStatus( kome::objects::Parameters* ) {
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

// set drawing spectrum peaks status
kome::objects::Variant setSpectrumDrawingPeaksStatus( kome::objects::Parameters* ) {
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

// update drawing label settings
kome::objects::Variant updateDrawingLabelSeetings( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// referesh
	kome::window::WindowTool::refresh();

	return ret;
}

// @date 2011.03.29 <Add> M.Izumi ->
// button down (spectrum)
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}
	
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// on button down
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.downButton( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// button down (chromatogram)
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}
	
	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// on button down
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.downButton( *chromatogram, *mouseEvent, *graphics, canvas );

	return ret;
}

// mouse move (spectrum)
kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );

	// on mouse move
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.moveMouse( *spectrum, *mouseEvent, *graphics, canvas );
	
	return ret;
}

// mouse move (chromatogram)
kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	// get parameters
	kome::objects::Chromatogram* chromatogram = kome::plugin::PluginCallTool::getChromatogram( params );
	if( chromatogram == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// on mouse move
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.moveMouse( *chromatogram, *mouseEvent, *graphics, canvas );
	
	return ret;
}

// button up (spectrum)
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// on button up
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.upButton( *mouseEvent, *graphics, false );

	return ret;
}

// button up (chromatogram)
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}
		
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// on button up
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.upButton( *mouseEvent, *graphics, true );

	return ret;
}

// on double click (spectrum)
kome::objects::Variant onSpecDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// on double click
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal= mgr.dblClick( *mouseEvent, *graphics, false );

	return ret;
}

// on double click (chromatogram)
kome::objects::Variant onChromDblClick( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// get parameters
	kome::evt::MouseEvent* mouseEvent = kome::plugin::PluginCallTool::getMouseEvent( params );
	if( mouseEvent == NULL ) {
		return ret;
	}

	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ) {
		return ret;
	}

	// on double click
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	ret.prim.boolVal = mgr.dblClick( *mouseEvent, *graphics, true );

	return ret;
}

// draw spectrum foreground
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params ) {
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

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// draw
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	mgr.drawForeground( *spectrum, *graphics, canvas );

	return ret;
}

// draw chromatogram foreground
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params ) {
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

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	
	// draw
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	mgr.drawForeground( *chromatogram, *graphics, canvas );

	return ret;
}
// @date 2011.03.29 <Add> M.Izumi <-

// draw 2D peaks
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params ) {
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

	// draw peaks
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	mgr.drawPeaks( *dataMap, *graphics );

	return ret;
}

// draw 2D peaks
kome::objects::Variant draw2dClusters( kome::objects::Parameters* params ) {
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

	// draw peaks
	PeakLabelDrawer& mgr = PeakLabelDrawer::getInstance();
	mgr.drawClusters( *dataMap, *graphics );

	return ret;
}
