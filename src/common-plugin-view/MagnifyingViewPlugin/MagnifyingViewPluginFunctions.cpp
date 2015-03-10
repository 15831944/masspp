/**
 * @file MagnifyingViewPluginFunctions.cpp
 * @brief interfaces of MagnifyingView plug-in function
 *
 * @author N.Tashiro
 * @date 2012.03.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MagnifyingViewPluginFunctions.h"
#include "MagnifyingViewManager.h"
#include "MagnifyingViewOperation.h"
#include "MagnifyDialog.h"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// spectrum is close
kome::objects::Variant onCloseSpec( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// clear ScaleInfo
	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();
	mgr.clearScaleInfo( spectrum );

	return ret;
}

// on button down (spectrum)
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params ) {
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
	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();
	ret.prim.boolVal = mgr.downButton( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// on double click (spectrum)
kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params ) {
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

	// on double click
	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();
	ret.prim.boolVal = mgr.dblClick( *spectrum, *mouseEvent, *graphics, canvas );

	return ret;
}

// show Magnify
kome::objects::Variant showMagnify( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get specterum
	kome::objects::Spectrum* spec = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spec == NULL ) {
		return ret;
	}

	// get range
	double startMz = double();
	double endMz = double();
	if( !kome::plugin::PluginCallTool::getRange( params, &startMz, &endMz ) ) {
		return ret;
	}

	// canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = (frame == NULL ? NULL : frame->getCanvas());

	// check overlapping.
	int style = (canvas == NULL ? 0 : ((kome::window::Canvas*)canvas)->getCanvasStyle());
	if( (style & kome::window::Canvas::OVERLAPPING) ||
		(style & kome::window::Canvas::MULTIPLE_OBJECTS) ){
		kome::window::WindowTool::showError( "Magnify dialog cannot be used with Overlapping display mode or with multiple spectra.\nUse with a single spectrum in either Single or Stacked display mode." );
		return ret;
	}

	// create Magnify dialog
	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();

	MagnifyDialog dlg( kome::window::WindowTool::getMainWindow(), spec );
	dlg.setSpec( spec );
	dlg.setStartPos( startMz );
	dlg.setEndPos( endMz );
	dlg.setID( -1 );

	if( dlg.ShowModal() == wxID_OK ) {	// SPEC No.85787 @date 2012.04.20 <Mod> M.Izumi
		// [Operation Log]
		// operation ‚ð new ‚µ‚Ä execute

		MagnifyingViewOperation* op = new MagnifyingViewOperation();
		op->setShortName("magnify_view");
		op->setOperationName("Magnifying View");
		op->setOperationType( kome::operation::Operation::TYPE_GUI );
		op->setTargetSample( spec->getSample() );
		op->setSpectrum( spec );
		op->setScaleIndex( -1 );
		op->setScale( (double)dlg.getScale() );
		op->setRange( dlg.getStartPos(), dlg.getEndPos() );
		
		if( op->execute() ) {
			kome::window::WindowTool::refresh();
		}
		else {
			delete op;
		}
	}

	return ret;
}

// on draw spectrum window
kome::objects::Variant onDrawSpecWindowFg( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// draw
	MagnifyingViewManager&	mgr = MagnifyingViewManager::getInstance();
	ret.prim.boolVal = mgr.DrawSpecWindowFg( params );
	
	return ret;
}

// magnifying view operation
kome::objects::Variant getMagnifyingViewOperation( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new MagnifyingViewOperation();
	
	return ret;
}
